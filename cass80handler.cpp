/****************************************************************************
 *
 * Cass80 tool - reading and writing cassette image files
 *
 * Copyright (C) 2020 Jürgen Buchmüller <pullmoll@t-online.de>
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 * notice, this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright
 * notice, this list of conditions and the following disclaimer in the
 * documentation and/or other materials provided with the distribution.
 *
 * 3. Neither the name of the copyright holder nor the names of its
 * contributors may be used to endorse or promote products derived from
 * this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 ****************************************************************************/
#include "basictoken.h"
#include "constants.h"
#include "cass80handler.h"

static const QLatin1String g_virtual_tape_file("Colour Genie - Virtual Tape File");

/**
 * @brief offset loader machine code
 *
 * This is the offset loader appended to the original machine code
 * by the COLOFF program. If this piece of code is found at the
 * entry address, the original cassette image can be restored from
 * the list of destination and size entries following this code.
 */
static const uchar g_lmoffset[] = {
    0xf3,                   /* di                  ; disable interrupts  */
    0x21,0xfb,0xc9,         /* ld   hl,$c9fb       ; EI, RET             */
    0x22,0x12,0x40,         /* ld   ($4012),hl     ; store to $4012 ..   */
    0x21,0x3e,0x00,         /* ld   hl,$003e       ; LD A,00h            */
    0x22,0x33,0x40,         /* ld   ($4033),hl     ; store to $4033 ..   */
    0x3e,0xc9,              /* ld   a,$c9          ; RET                 */
    0x32,0x35,0x40,         /* ld   ($4035),a      ; store to $4035      */
    0x32,0xe2,0x41,         /* ld   ($41e2),a      ; and to $41e2        */
    0x06,0x1c,              /* ld   b,$1c          ; 28 DOS commands     */
    0x21,0x52,0x41,         /* ld   hl,$4152       ; jump table at $4152 */
    0x36,0xc3,              /* ld   (hl),$c3       ; JP $013b            */
    0x23,                   /* inc  hl             ;                     */
    0x36,0x3b,              /* ld   (hl),$3b       ; address low         */
    0x23,                   /* inc  hl             ;                     */
    0x36,0x01,              /* ld   (hl),$01       ; address high        */
    0x23,                   /* inc  hl             ;                     */
    0x10,0xf5,              /* djnz $-11           ; next jump vector    */
    0x06,0x15,              /* ld   b,$15          ; 21 DOS functions    */
    0x36,0xc9,              /* ld   (hl),$c9       ; RET                 */
    0x23,                   /* inc  hl             ; skip 3 bytes        */
    0x23,                   /* inc  hl             ;                     */
    0x23,                   /* inc  hl             ;                     */
    0x10,0xf9,              /* djnz $-7            ; next return vector  */
    0xcd,0x0b,0x00,         /* call $000b          ; get address to HL   */
    0x11,0x21,0x00,         /* ld   de,$0021       ; add $0021           */
    0x19,                   /* add  hl,de          ; address of relocate */
    0xe5,                   /* push hl             ; save address        */
    0x21,0x00,0x60,         /* ld   hl,$6000       ; source              */
    0xe3,                   /* ex   (sp),hl        ; swap source/table   */
    0x5e,                   /* ld   e,(hl)         ; get destination lo  */
    0x23,                   /* inc  hl             ;                     */
    0x56,                   /* ld   d,(hl)         ; get destination hi  */
    0x23,                   /* inc  hl             ;                     */
    0x4e,                   /* ld   c,(hl)         ; get count lo        */
    0x23,                   /* inc  hl             ;                     */
    0x46,                   /* ld   b,(hl)         ; get count hi        */
    0x23,                   /* inc  hl             ;                     */
    0x79,                   /* ld   a,c            ; check count for     */
    0xb0,                   /* or   b              ; $0000               */
    0x28,0x05,              /* jr   z,$+7          ; yes, we're done     */
    0xe3,                   /* ex   (sp),hl        ; swap table/source   */
    0xed,0xb0,              /* ldir                ; copy memory block   */
    0x18,0xee,              /* jr   $-18           ; next table entry    */
    0xe1,                   /* pop  hl             ; pop address         */
    0xcd,0xe2,0x41,         /* call $41e2          ; call vector         */
    0xc3                    /* jp   $XXXX          ; start program       */
    /* relocate with the following format follows:                       */
    /* <addr_lo> <addr_hi> <count_lo> <count_hi>                         */
};

/**
 * @brief mover machine code
 *
 * This is the mover appended to the original machine code.
 */
static const uchar g_mover[] = {
        0xf3,                   /* di                  ; disable interrupts  */
        0x21,0x00,0x60,         /* ld   hl,$6000       ; source address      */
        0x11,0x00,0x48,         /* ld   de,$4800       ; destination address */
        0x01,0x00,0x40,         /* ld   bc,$4000       ; size of the move    */
        0xed,0xb0,              /* ldir                ; block move          */
        0xc3                    /* jp   $XXXX          ; start program       */
};

Cass80Handler::Cass80Handler(QObject *parent)
    : QObject(parent)
    , bas(new BasicToken())
    , m_verbose(1)
    , m_machine(MACH_INVALID)
    , m_sync(CAS_TRS80_SYNC)
    , m_hdr_name()
    , m_hdr_author()
    , m_hdr_copyright()
    , m_hdr_description()
    , m_filename()
    , m_blen(256)
    , m_addr(0)
    , m_line(0)
    , m_entry(0)
    , m_size(0)
    , m_prefix(0)
    , m_csum(0)
    , m_basic(false)
    , m_sha1(QCryptographicHash::Sha1)
    , m_digest()
    , m_source()
    , m_blocks()
    , m_total_size(0)
{
}

void Cass80Handler::reset()
{
    m_machine = MACH_INVALID;
    m_sync = CAS_SILENCE;
    m_hdr_name.clear();
    m_hdr_author.clear();
    m_hdr_copyright.clear();
    m_hdr_description.clear();

    m_filename.clear();
    m_addr = 0;
    m_line = 0;
    m_entry = 0;
    m_size = 0;
    m_prefix = 0;
    m_csum = 0;
    m_basic = false;

    m_sha1.reset();
    m_digest.clear();
    m_source.clear();
    m_blocks.clear();
    m_total_size = 0;
}

bool Cass80Handler::isEmpty() const
{
    return m_blocks.isEmpty();
}

bool Cass80Handler::isValid() const
{
    if (m_blocks.isEmpty())
	return false;
    // FIXME: validity check (checksums)?
    return true;
}

machine_e Cass80Handler::machine() const
{
    return m_machine;
}

quint8 Cass80Handler::sync() const
{
    return m_sync;
}

quint8 Cass80Handler::prefix() const
{
    return m_prefix;
}

QString Cass80Handler::hdr_name() const
{
    return m_hdr_name;
}

QString Cass80Handler::hdr_author() const
{
    return m_hdr_author;
}

QString Cass80Handler::hdr_copyright() const
{
    return m_hdr_copyright;
}

QString Cass80Handler::hdr_description() const
{
    return m_hdr_description;
}

QString Cass80Handler::filename() const
{
    return m_filename;
}

quint16 Cass80Handler::blen() const
{
    return m_blen;
}

bool Cass80Handler::basic() const
{
    return m_basic;
}

bool Cass80Handler::load(const QString& filename)
{
    QFile input(filename);
    bool res = false;
    if (input.open(QIODevice::ReadOnly)) {
        res = load(&input);
        input.close();
    }

    return res;
}

bool Cass80Handler::load(QIODevice* device)
{
    CasBlock block;
    decoder_status_e status;
    QByteArray data = device->peek(device->size());
    QByteArray buff;
    int count, pos;
    uchar ch;
    uchar *bp;

    reset();

    int start = data.indexOf(CAS_TRS80_SYNC);
    if (data.indexOf(CAS_CGENIE_SYNC) < start)
	start = data.indexOf(CAS_CGENIE_SYNC);

    if (QString::fromLatin1(data).startsWith(g_virtual_tape_file)) {
        device->seek(32);
        status = ST_COMMENT;
	m_machine = MACH_EG2000;
    } else if (start >= 0) {
        int count = start;
        for (pos = 0; pos < count; pos++)
            if (0x00 != data[pos])
                break;
	if (static_cast<uchar>(data[start]) == CAS_CGENIE_SYNC || pos < start) {
            /* probably Colour Genie SYSTEM or BASIC including an 0xA5 token */
            device->seek(0);
            status = ST_NUL;
	    m_machine = MACH_EG2000;
        } else {
            /* TRS-80 silence header + sync byte */
            device->seek(count);
            status = ST_SILENCE;
	    m_machine = MACH_TRS80;
        }
    } else {
        /* read from the beginning */
        device->seek(0);
	status = ST_NUL;
	m_machine = MACH_EG2000;
    }

    pos = 0;
    count = 0;
    buff.clear();

    while (!device->atEnd()) {
	QByteArray ba = device->read(1);
	ch = static_cast<uchar>(ba[0]);
        switch (status) {
        case ST_INVALID:
            return false;

        case ST_COMMENT:
            switch (ch) {
            case 13:        /* carriage return */
                pos = 0;
                break;
            case 10:        /* line feed */
                if (buff.startsWith(QByteArray("Name       :"))) {
		    m_hdr_name = QString::fromLatin1(buff.mid(13));
		    emit Info(tr("Header Name: %1").arg(m_hdr_name));
                    buff.clear();
                    break;
                }
                if (buff.startsWith(QByteArray("Author     :"))) {
		    m_hdr_author = QString::fromLatin1(buff.mid(13));
		    emit Info(tr("Header Author: %1").arg(m_hdr_author));
                    buff.clear();
		    break;
                }
                if (buff.startsWith(QByteArray("Copyright  :"))) {
		    m_hdr_copyright = QString::fromLatin1(buff.mid(13));
		    emit Info(tr("Header Copyright: %1").arg(m_hdr_copyright));
		    buff.clear();
                    break;
                }
                if (buff.startsWith(QByteArray("Description:"))) {
		    m_hdr_description = QString::fromLatin1(buff.mid(13)).trimmed();
                    buff.clear();
                    break;
                }
		if (m_hdr_description.length()) {
		    m_hdr_description += QChar::LineFeed;
		    m_hdr_description += QString::fromLatin1(buff).trimmed();
                }
                buff.clear();
                pos = 0;
                break;

            case 26:        /* Ctrl-Z (text EOF) */
		if (!m_hdr_description.isEmpty()) {
		    emit Info(tr("Header Description: %1").arg(m_hdr_description));
		}
                status = ST_EOF;
                break;

            default:
                buff += QChar::fromLatin1(ch);
            }
            break;

        case ST_EOF:
            switch (ch) {
            case 0x00:
                status = ST_NUL;
                break;
            default:
		emit Error(QString("EOF: unexpected %1 (0x%2)").arg(ch).arg(ch, 2, 16, QChar('0')));
                qDebug("EOF: unexpected %d (0x%x)", ch, ch);
                break;
            }
            break;

        case ST_SILENCE:
            if (0x00 == ch)
                break;
            status = ST_NUL;
            /* FALLTHROUGH */

        case ST_NUL:
            switch (ch) {
            case CAS_TRS80_SYNC:
		m_sync = ch;
		m_sha1.addData(reinterpret_cast<const char *>(&ch), 1);
                status = ST_HEADER;
                pos = 0;
                count = 8;
                break;
            case CAS_CGENIE_SYNC:
		m_sync = ch;
		m_sha1.addData(reinterpret_cast<const char *>(&ch), 1);
                status = ST_HEADER;
                pos = 0;
                count = 8;
                break;
            default:
                qCritical("NUL: unexpected %d (0x%x)", ch, ch);
                break;
            }
            buff.clear();
            break;

        case ST_HEADER:
            buff += ch;
            pos++;
            count--;
            if (count)
                break;
            bp = reinterpret_cast<uchar *>(buff.data());
	    if (m_verbose > 1) {
		emit Info(tr("HEADER: %1 %2 %3 %4 %5 %6 %7 %8")
			.arg(bp[0], 2, 16, QChar(0))
			.arg(bp[1], 2, 16, QChar(0))
			.arg(bp[2], 2, 16, QChar(0))
			.arg(bp[3], 2, 16, QChar(0))
			.arg(bp[4], 2, 16, QChar(0))
			.arg(bp[5], 2, 16, QChar(0))
			.arg(bp[6], 2, 16, QChar(0))
			.arg(bp[7], 2, 16, QChar(0)));
		qDebug("HEADER: %02x %02x %02x %02x %02x %02x %02x %02x",
		       bp[0], bp[1], bp[2], bp[3], bp[4], bp[5], bp[6], bp[7]);
	    }
            /* look for TRS-80 or Colour Genie SYSTEM tape format */
            if (bp[0] == CAS_SYSTEM_HEADER && bp[7] == CAS_SYSTEM_DATA) {
		m_filename = QString::fromLatin1(buff.mid(1, 6));
                if (m_verbose) {
		    emit Info(tr("SYSTEM tape: '%1'").arg(m_filename));
		    qDebug("SYSTEM tape: '%s'", qPrintable(m_filename));
                }
                status = ST_SYSTEM_COUNT;
		m_sha1.addData(buff, 8);
		m_prefix = buff[0];
		m_basic = false;
            } else if (bp[0] == CAS_TRS80_BASIC_HEADER &&
                       bp[1] == CAS_TRS80_BASIC_HEADER &&
                       bp[2] == CAS_TRS80_BASIC_HEADER) {
		m_filename = QString::fromLatin1(buff.constData() + 3, 1);
                if (m_verbose) {
		    emit Info(tr("TRS-80 BASIC tape: '%1'").arg(m_filename));
		    qDebug("TRS-80 BASIC tape: '%s'", qPrintable(m_filename));
                }
                device->seek(device->pos() - 4);
                status = ST_BASIC_ADDR_LSB;
		m_sha1.addData(buff, 1);
		m_prefix = 0;
		m_basic = true;
            } else {
		m_filename = QString::fromLatin1(buff.constData(), 1);
                if (m_verbose) {
		    emit Info(tr("Colour Genie BASIC tape: '%1'").arg(m_filename));
		    qDebug("Colour Genie BASIC tape: '%s'", qPrintable(m_filename));
                }
                device->seek(device->pos() - 7);
                status = ST_BASIC_ADDR_LSB;
		m_sha1.addData(buff, 1);
		m_prefix = 0;
		m_basic = true;
            }
            break;

        case ST_SYSTEM_BLOCKTYPE:
	    m_sha1.addData(reinterpret_cast<const char *>(&ch), 1);
            switch (ch) {
            case CAS_SYSTEM_DATA:
                status = ST_SYSTEM_COUNT;
                break;
            case CAS_SYSTEM_ENTRY:
                status = ST_SYSTEM_ENTRY_LSB;
                break;
            default:
		emit Error(QString("SYSTEM_BLOCKTYPE: unexpected %1 (0x%2)")
			   .arg(ch)
			   .arg(ch, 2, 16, QChar('0')));
		qCritical("SYSTEM_BLOCKTYPE: unexpected %d (0x%x)", ch, ch);
                break;
            }
            break;

        case ST_SYSTEM_COUNT:
	    m_sha1.addData(reinterpret_cast<const char *>(&ch), 1);
            count = ch;
            if (0 == count)
                count = 256;
	    m_size = count;
            status = ST_SYSTEM_ADDR_LSB;
            break;

        case ST_SYSTEM_ADDR_LSB:
	    m_sha1.addData(reinterpret_cast<const char *>(&ch), 1);
	    m_addr = ch;
	    m_csum = ch;
            status = ST_SYSTEM_ADDR_MSB;
            break;

        case ST_SYSTEM_ADDR_MSB:
	    m_sha1.addData(reinterpret_cast<const char *>(&ch), 1);
	    m_addr = m_addr + 256 * ch;
	    m_csum = m_csum + ch;
            status = ST_SYSTEM_DATA;
            if (m_verbose > 1) {
		emit Info(tr("SYSTEM data block: %1 (0x%2) at %3h")
			  .arg(count)
			  .arg(count, 1, 16, QChar('0'))
			  .arg(m_addr, 4, 16, QChar('0')));
		qDebug("SYSTEM data block: %u (%xh) at %04xh",
		       count, count, m_addr);
            }
            block.type = BT_SYSTEM;
	    block.addr = m_addr;
	    block.size = m_size;
	    block.data.fill(0x00, 1024);
            pos = 0;
            break;

        case ST_SYSTEM_DATA:
	    m_sha1.addData(reinterpret_cast<const char *>(&ch), 1);
            block.data[pos] = ch;
            if (++pos == 1024)
                pos = 1023;
	    m_csum = m_csum + ch;
            if (0 == --count) {
                status = ST_SYSTEM_CSUM;
		block.data.resize(pos);
	    }
            break;

        case ST_SYSTEM_CSUM:
	    m_sha1.addData(reinterpret_cast<const char *>(&ch), 1);
	    if (ch != m_csum) {
		emit Error(QString("SYSTEM_CSUM: block #%1 checksum error (found:0%2 calc:0x%3)")
			   .arg(m_blocks.count())
			   .arg(ch, 2, 16, QChar('0'))
			   .arg(m_csum, 2, 16, QChar('0')));
		qCritical("SYSTEM_CSUM: block #%u checksum error (found:0x%02x calc:0x%02x)",
			m_blocks.count(), ch, m_csum);
		m_csum = ch;
            }
            status = ST_SYSTEM_BLOCKTYPE;
	    block.csum = m_csum;
	    m_total_size += block.size;
            m_blocks += block;
            break;

        case ST_SYSTEM_ENTRY_LSB:
	    m_sha1.addData(reinterpret_cast<const char *>(&ch), 1);
	    m_entry = ch;
            status = ST_SYSTEM_ENTRY_MSB;
            break;

        case ST_SYSTEM_ENTRY_MSB:
	    m_sha1.addData(reinterpret_cast<const char *>(&ch), 1);
	    m_entry = m_entry + 256 * ch;
            block.type = BT_ENTRY;
	    block.addr = m_entry;
            block.size = 0;
            m_blocks += block;
            if (m_verbose) {
		emit Info(tr("SYSTEM entry point: %1 (0x%2)")
			   .arg(m_entry)
			   .arg(m_entry, 4, 16, QChar('0')));
		qDebug("SYSTEM entry point: %u (0x%04x)",
		       m_entry, m_entry);
            }
            status = ST_AFTER_ENTRY;
            block.type = BT_RAW;
            block.size = 0;
            block.data.fill(0, 1024);
            break;

        case ST_BASIC_ADDR_LSB:
	    m_sha1.addData(reinterpret_cast<const char *>(&ch), 1);
	    m_addr = ch;
            status = ST_BASIC_ADDR_MSB;
            break;

        case ST_BASIC_ADDR_MSB:
	    m_sha1.addData(reinterpret_cast<const char *>(&ch), 1);
	    m_addr = m_addr + 256 * ch;
	    if (0 == m_addr) {
                if (m_verbose > 1) {
		    emit Info(tr("BASIC end: %1 (0x%2)")
			       .arg(pos)
			       .arg(pos, 4, 16, QChar('0')));
                }
                status = ST_IGNORE;
                block.type = BT_RAW;
                block.size = 0;
                block.data.fill(0, 1024);
            } else {
                pos = 0;
                status = ST_BASIC_LINE_LSB;
            }
            break;

        case ST_BASIC_LINE_LSB:
	    m_sha1.addData(reinterpret_cast<const char *>(&ch), 1);
	    m_line = ch;
            status = ST_BASIC_LINE_MSB;
            break;

        case ST_BASIC_LINE_MSB:
	    m_sha1.addData(reinterpret_cast<const char *>(&ch), 1);
	    m_line = m_line + 256 * ch;
            status = ST_BASIC_DATA;
            pos = 0;
            block.type = BT_BASIC;
	    block.line = m_line;
	    block.addr = m_addr;
            block.size = 1024;
            block.data.fill(0, 1024);
            if (m_verbose > 1) {
		emit Info(tr("BASIC line: #%1 at %2h")
			   .arg(m_line)
			   .arg(m_addr, 4, 16, QChar('0')));
		qDebug("BASIC line: #%u at %xh", m_line, m_addr);
            }
            break;

        case ST_BASIC_DATA:
	    m_sha1.addData(reinterpret_cast<const char *>(&ch), 1);
            block.data[pos] = ch;
            if (++pos == 1024)
                pos = 1023;
	    if (0x00 == ch) {
		m_size = pos;
		block.size = m_size;
		block.data.resize(m_size);
		m_source += QString("%1 %2")
			  .arg(m_line)
			  .arg(bas->detokenize(block.data, block.size));
		m_total_size += block.size;
		m_blocks += block;
                status = ST_BASIC_ADDR_LSB;
            }
            break;

        case ST_AFTER_ENTRY:
            if (ch == CAS_SYSTEM_DATA) {
                /* another data block after the system entry */
		m_sha1.addData(reinterpret_cast<const char *>(&ch), 1);
                status = ST_SYSTEM_COUNT;
                break;
            }
            if (ch == CAS_SYSTEM_ENTRY) {
                /* another entry block after the system entry */
		m_sha1.addData(reinterpret_cast<const char *>(&ch), 1);
                status = ST_SYSTEM_ENTRY_LSB;
                break;
            }
            status = ST_IGNORE;
            /* FALLTHROUGH */

        case ST_IGNORE:
            /* do not append ch to image SHA1 */
            if (0 == (block.size % 1024)) {
                block.data.resize(block.size + 1024);
            }
            block.data[block.size] = ch;
            block.size += 1;
        }
    }
    m_digest = m_sha1.result();
    emit Info(tr("Loaded %1 blocks (%2 bytes).").arg(m_blocks.count()).arg(m_total_size));
    emit Info(tr("SHA1 of data: %1.").arg(m_digest.toHex().constData()));
    qDebug("Loaded %d blocks. SHA1 = %s", m_blocks.count(), m_digest.toHex().constData());
    return true;
}

bool Cass80Handler::has_lmoffset()
{
    QByteArray mover(reinterpret_cast<const char *>(g_mover), sizeof(g_mover));
    for (int i = 0; i < m_blocks.count(); i++) {
	const uchar* bp = reinterpret_cast<const uchar *>(m_blocks[i].data.constData());
	if (m_blocks[i].size != 15)
	    continue;
	if (bp[0] == mover[0] || bp[1] == mover[1] || bp[4] == mover[4] ||
	    bp[7] == mover[7] || bp[10] == mover[10] || bp[11] == mover[11] || bp[12] == mover[12]) {
	    return true;
	}
    }
    return false;
}

bool Cass80Handler::undo_lmoffset()
{
    QByteArray memory(64*1024, 0x00);
    QByteArray mover(reinterpret_cast<const char *>(g_mover), sizeof(g_mover));
    CasBlock block;
    for (int i = 0; i < m_blocks.count(); i++) {
	CasBlock* cb = &m_blocks[i];
        const uchar* bp = reinterpret_cast<const uchar *>(m_blocks[i].data.constData());
	memory.replace(cb->addr, cb->size, cb->data);
        if (m_blocks[i].size != 15)
            continue;
        if (bp[0] == mover[0] || bp[1] == mover[1] || bp[4] == mover[4] ||
            bp[7] == mover[7] || bp[10] == mover[10] || bp[11] == mover[11] || bp[12] == mover[12]) {
	    emit Info(tr("Found MOVER at address %1h").arg(cb->addr, 4, 16, QChar('0')));
	    qDebug("Found MOVER at address %xh", cb->addr);
	    quint16 src = bp[2] + (256u * bp[3]);
	    quint16 dst = bp[5] + (256u * bp[6]);
	    quint16 size = bp[8] + (256u * bp[9]);
	    quint16 entry = bp[13] + (256u * bp[14]);
	    emit Info(tr("MOVER src:0x%1 dst:0x%2 size:0x%3 entry:0x%4")
		      .arg(src, 4, 16, QChar('0'))
		      .arg(dst, 4, 16, QChar('0'))
		      .arg(size, 4, 16, QChar('0'))
		      .arg(entry, 4, 16, QChar('0')));
	    qDebug("Move src:0x%x dst:0x%x size:0x%x entry:0x%x", src, dst, size, entry);
	    for (quint16 i = 0; i < size; i++) {
                memory[dst+i] = memory[src+i];
		if (dst < src)
		    memory[src+i] = 0;
            }

	    // Remove blocks in source range
	    for (int j = 0; j < m_blocks.count(); j++) {
		CasBlock* cc = &m_blocks[j];
		if (BT_SYSTEM == cc->type &&
			cc->addr >= src &&
			cc->addr + cc->size <= src + size) {
		    m_blocks.takeAt(j);
		    --j;
		}
		if (BT_ENTRY == cc->type) {
		    m_blocks.takeAt(j);
		    --j;
		}
		if (cb == cc) {
		    m_blocks.takeAt(j);
		    --j;
		}
	    }

	    for (quint16 i = 0; i < size; i += m_blen, dst += m_blen) {
		CasBlock block;
		quint8 bcnt = static_cast<quint8>((size - i) > m_blen ? m_blen : size - i);
		block.data = memory.mid(dst, bcnt ? bcnt : 256);
		quint8 csum;
		csum = dst % 256;
		csum += dst / 256;
		foreach(char ch, block.data)
		    csum += static_cast<uchar>(ch);
                block.type = BT_SYSTEM;
                block.csum = csum;
                block.addr = dst;
                block.line = 0;
                block.size = bcnt;
                m_blocks += block;
            }
	    CasBlock block;
            block.type = BT_ENTRY;
            block.addr = entry;
            m_blocks += block;
            return true;
        }
    }
    return true;
}

bool Cass80Handler::save(const QString& filename)
{
    QFile output(filename);
    if (!output.open(QIODevice::WriteOnly)) {
        qCritical("Cannot open '%s' for writing", qPrintable(filename));
        return false;
    }

    QByteArray header;
    QByteArray fname(6, 0x20);
    switch (m_machine) {
    case MACH_EG2000:
	header += static_cast<char>(CAS_CGENIE_SYNC);
	header += static_cast<char>(CAS_SYSTEM_HEADER);
	fname.replace(0, qMin(6, m_filename.length()),
		      m_filename.toUpper().toLatin1());
	header += fname;
	output.write(header);
	foreach(const CasBlock& block, m_blocks) {
	    if (BT_SYSTEM == block.type) {
		QByteArray data;
		data += static_cast<char>(CAS_SYSTEM_DATA);
		data += block.size % 256;
		data += block.addr % 256;
		data += block.addr / 256;
		data += block.data;
		data += block.csum;
		output.write(data);
	    } else if (BT_ENTRY == block.type) {
		QByteArray data;
		data += static_cast<char>(CAS_SYSTEM_ENTRY);
		data += block.addr % 256;
		data += block.addr / 256;
		output.write(data);
	    }
	}
	break;
    case MACH_TRS80:
	header += QByteArray(256, CAS_SILENCE);
	header += static_cast<char>(CAS_TRS80_SYNC);
	header += static_cast<char>(CAS_SYSTEM_HEADER);
	header += m_filename.left(6).toUpper();
	output.write(header);
	foreach(const CasBlock& block, m_blocks) {
	    if (BT_SYSTEM == block.type) {
		QByteArray data;
		data += static_cast<char>(CAS_SYSTEM_DATA);
		data += block.size % 256;
		data += block.addr % 256;
		data += block.addr / 256;
		data += block.data;
		data += block.csum;
		output.write(data);
	    } else if (BT_ENTRY == block.type) {
		QByteArray data;
		data += static_cast<char>(CAS_SYSTEM_ENTRY);
		data += block.addr % 256;
		data += block.addr / 256;
		output.write(data);
	    }
	}
	break;

    default:
	emit Error(tr("Invalid machine - none of EG2000 or TRS80 were detected."));
	return false;
    }
    return true;
}

int Cass80Handler::count() const
{
    return m_blocks.count();
}

CasBlockList Cass80Handler::blocks() const
{
    return m_blocks;
}

CasBlock Cass80Handler::block(int index) const
{
    return m_blocks.value(index);
}

QStringList Cass80Handler::source() const
{
    return m_source;
}

bool Cass80Handler::set_blen(quint16 blen)
{
    if (blen > 256) {
	emit Error(tr("Block length is wrong: %1 (2 <= blen <= 256").arg(blen));
	return false;
    }
    m_blen = blen;
    return true;
}
