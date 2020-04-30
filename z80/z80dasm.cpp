/****************************************************************************
 *
 * Cass80 tool - Z80 disassembler
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
#include "bdfdata.h"
#include "z80defs.h"
#include "z80dasm.h"
#include "z80token.h"

z80Dasm::z80Dasm(bool upper, const z80Defs* defs, const bdfData* bdf)
    : m_uppercase(upper)
    , m_comment_glyphs(false)
    , m_bytes_per_line(8)
    , m_mnemonic_column(24)
    , m_comment_column(48)
    , m_defs(defs)
    , m_bdf(bdf)
{
    Q_ASSERT(m_defs);
    if (!m_defs) {
	qFatal("%s: no z80Defs* passed to constructor", __func__);
    }
}

/**
 * @brief Return the sign character for an IX/IY offset value
 * @param offset -128 to +127
 * @return sign character
 */
QChar z80Dasm::sign(qint8 offset)
{
    return (offset < 0) ? QChar('-') : QChar('+');
}

/**
 * @brief Return Z80 (TRS-80, Video Genie 3003, Colour Genie EG2000) byte value string
 *
 * This returns a string in the format used by Z80 compilers.
 * It's a hexadecimal encoded number with suffix 'h' appended.
 * If the hex representation does not start with a digit, prepend '0'.
 *
 * @param val byte value
 * @return string for the value
 */
QString z80Dasm::hexb(quint32 val)
{
    QString hex;
    if (val >= 0xa0U) {
	hex = QString("%1h").arg(val, 3, 16, QChar('0'));
    } else if (val >= 10) {
	hex = QString("%1h").arg(val, 2, 16, QChar('0'));
    } else {
	hex = QString::number(val);
    }
    if (m_uppercase)
	return hex.toUpper();
    return hex;
}

/**
 * @brief Return Z80 (TRS-80, Video Genie 3003, Colour Genie EG2000) word value string
 *
 * This returns a string in the format used by Z80 compilers.
 * It's a hexadecimal encoded number with suffix 'h' appended.
 * If the hex representation does not start with a digit, prepend '0'.
 *
 * @param val word value
 * @return string for the value
 */
QString z80Dasm::hexw(quint32 val)
{
    QString hex;
    if (val >= 0xa000U) {
	hex = QString("%1h").arg(val, 5, 16, QChar('0'));
    } else if (val >= 10) {
	hex = QString("%1h").arg(val, 4, 16, QChar('0'));
    } else {
	hex = QString::number(val);
    }
    if (m_uppercase)
	return hex.toUpper();
    return hex;
}

/**
 * @brief Return Z80 (TRS-80, Video Genie 3003, Colour Genie EG2000) dword value string
 *
 * This returns a string in the format used by Z80 compilers.
 * It's a hexadecimal encoded number with suffix 'h' appended.
 * If the hex representation does not start with a digit, prepend '0'.
 *
 * @param val dword value
 * @return string for the value
 */
QString z80Dasm::hexd(quint32 val)
{
    QString hex;
    if (val >= 0xa0000000UL) {
	hex = QString("%1h").arg(val, 9, 16, QChar('0'));
    } else if (val >= 10) {
	hex = QString("%1h").arg(val, 8, 16, QChar('0'));
    } else {
	hex = QString::number(val);
    }
    if (m_uppercase)
	return hex.toUpper();
    return hex;
}

/**
 * @brief Return the absolute offset
 * @param offset signed 7 bit value
 * @return absolute value of the offset as int
 */
int z80Dasm::offs(qint8 offset)
{
    if (offset < 0)
	return -offset;
    return offset;
}

/**
 * @brief Read a byte from memory at @p mem
 * @param mem pointer to memory
 * @return byte value from memory
 */
quint32 z80Dasm::rd08(const quint8* mem)
{
    return
	static_cast<quint32>(mem[0]);
}

/**
 * @brief Read a word from memory at @p mem
 * @param mem pointer to memory
 * @return word value from memory
 */
quint32 z80Dasm::rd16(const quint8* mem)
{
    return
	(static_cast<quint32>(mem[0]) <<  0) |
	(static_cast<quint32>(mem[1]) <<  8);
}

/**
 * @brief Read a dword from memory at @p mem
 * @param mem pointer to memory
 * @return dword value from memory
 */
quint32 z80Dasm::rd32(const quint8* mem)
{
    return
	(static_cast<quint32>(mem[0]) <<  0) |
	(static_cast<quint32>(mem[1]) <<  8) |
	(static_cast<quint32>(mem[2]) << 16) |
	(static_cast<quint32>(mem[3]) << 24);
}

/**
 * @brief Write a byte to memory at @p mem
 * @param mem pointer to memory
 * @param val byte value
 */
void z80Dasm::wr08(quint8* mem, quint32 val)
{
    mem[0] = static_cast<quint8>(val);
}

/**
 * @brief Write a word to memory at @p mem
 * @param mem pointer to memory
 * @param val word value
 */
void z80Dasm::wr16(quint8* mem, quint32 val)
{
    mem[0] = static_cast<quint8>(val >> 0);
    mem[1] = static_cast<quint8>(val >> 8);
}

/**
 * @brief Write a dword to memory at @p mem
 * @param mem pointer to memory
 * @param val dword value
 */
void z80Dasm::wr32(quint8* mem, quint32 val)
{
    mem[0] = static_cast<quint8>(val >>  0);
    mem[1] = static_cast<quint8>(val >>  8);
    mem[2] = static_cast<quint8>(val >> 16);
    mem[3] = static_cast<quint8>(val >> 24);
}

QString z80Dasm::x08(quint32 val)
{
    QString hex = QString("%1")
		  .arg(val & 0x000000ffu, 2, 16, QChar('0'));
    if (m_uppercase)
	return hex.toUpper();
    return hex;
}

QString z80Dasm::x32(quint32 val)
{
    QString hex = QString("%1")
		  .arg(val, 8, 16, QChar('0'));
    if (m_uppercase)
	return hex.toUpper();
    return hex;
}

QString z80Dasm::x16(quint32 val)
{
    QString hex = QString("%1")
		  .arg(val & 0x0000ffffu, 4, 16, QChar('0'));
    if (m_uppercase)
	return hex.toUpper();
    return hex;
}

QString z80Dasm::symbol_w(quint32 ea)
{
    z80Def ent = m_defs->entry(ea);
    if (!ent.isNull() && ent->has_symbol()) {
	if (m_uppercase)
	    return ent->symbol().toUpper();
	return ent->symbol().toLower();
    }
    return hexw(ea);
}

/**
 * @brief Disassemble a DEFB byte or multiple bytes
 *
 * TODO: respect z80DefObj::maxelem() and return a string
 * for possibly multiple bytes, either maxelem or up to
 * the next z80Def entry.
 *
 * @param pc program counter
 * @param pos position in opram
 * @param opram pointer to opcode RAM
 * @return string with bytes defined
 */
QString z80Dasm::dasm_defb(quint32 pc, off_t& pos, const quint8* opram)
{
    Q_UNUSED(pc)
    QString dasm;
    QString str = z80Token::string(z80Token::z80DEFB);
    quint32 ea;
    str.resize(7, QChar::Space);
    dasm += str;
    ea = rd08(opram + pos);
    dasm += hexb(ea);
    pos++;
    return dasm;
}

/**
 * @brief Disassemble a DEFW word or multiple words
 *
 * TODO: respect z80DefObj::maxelem() and return a string
 * for possibly multiple words, either maxelem or up to
 * the next z80Def entry.
 *
 * @param pc program counter
 * @param pos position in opram
 * @param opram pointer to opcode RAM
 * @return string with words defined
 */
QString z80Dasm::dasm_defw(quint32 pc, off_t& pos, const quint8* opram)
{
    Q_UNUSED(pc)
    QString dasm;
    QString str = z80Token::string(z80Token::z80DEFW);
    quint32 ea;
    str.resize(7, QChar::Space);
    dasm += str;
    ea = rd16(opram + pos);
    dasm += hexw(ea);
    pos += 2;
    return dasm;
}

/**
 * @brief Disassemble a DEFD dword or multiple dwords
 *
 * TODO: respect z80DefObj::maxelem() and return a string
 * for possibly multiple dwords, either maxelem or up to
 * the next z80Def entry.
 *
 * @param pc program counter
 * @param pos position in opram
 * @param opram pointer to opcode RAM
 * @return string with words defined
 */
QString z80Dasm::dasm_defd(quint32 pc, off_t& pos, const quint8* opram)
{
    Q_UNUSED(pc)
    QString dasm;
    QString str = z80Token::string(z80Token::z80DEFD);
    quint32 ea;
    str.resize(7, QChar::Space);
    dasm += str;
    ea = rd32(opram + pos);
    dasm += hexd(ea);
    pos += 4;
    return dasm;
}

/**
 * @brief Disassemble a DEFS space
 * @param pc program counter
 * @param pos position in opram
 * @param opram pointer to opcode RAM
 * @return string with the number of bytes in this space
 */
QString z80Dasm::dasm_defs(quint32 pc, off_t& pos, const quint8* opram)
{
    QString dasm;
    QString str = z80Token::string(z80Token::z80DEFS);
    str.resize(7, QChar::Space);
    uint8_t byte = opram[pos];
    int n = 0;
    while (pc + pos < 0x10000 && byte == opram[pos]) {
	n++;
	pos++;
	z80Def ent = m_defs->entry(pc + pos);
	if (ent->type() != z80DefObj::DEFS)
	    break;
    }
    dasm += str + QString::number(n);
    return dasm;
}

/**
 * @brief Disassemble a DEFM text message
 * @param pc program counter
 * @param pos position in opram
 * @param opram pointer to opcode RAM
 * @return string with words defined
 */
QString z80Dasm::dasm_defm(quint32 pc, off_t& pos, const quint8* opram)
{
    QString dasm;
    QString str = z80Token::string(z80Token::z80DEFM);
    bool first = true;
    bool instr = false;
    uchar prev = 0xff;
    str.resize(7, QChar::Space);

    while (0x00 != prev) {
	if (first) {
	    first = false;
	} else if (!instr) {
	    str += QChar(',');
	}

	if (opram[pos] < 0x20 || opram[pos] == 0x7f) {
	    if (instr) {
		str += QChar('"');
		str += QChar(',');
		instr = false;
	    }
	    str += hexb(opram[pos]);
	} else {
	    if (!instr) {
		str += QChar('"');
		instr = true;
	    }
	    uint uc = unicode(opram[pos]);
	    str += QChar(uc);
	}
	prev = opram[pos++];

	z80Def ent = m_defs->entry(pc + pos);
	if (ent->type() != z80DefObj::TEXT)
	    break;
    }

    if (instr) {
	str += QChar('"');
	instr = false;
    }

    dasm += str;
    return dasm;
}

/**
 * @brief Disassemble an array of tokens
 *
 * Each token starts with a byte where bit 7 set, and
 * it ends when the next byte with bit 7 set is found.
 *
 * So the scheme for a list of tokens is:
 * <pre>
 *  DEFM "A"+80h,"BC"
 *  DEFM "D"+80h,"EFGH"
 *  DEFM "I"+80h,"JKL"
 *</pre>
 * which would define the 3 tokens "ABC", "DEFGH", and "IJKL"
 *
 * @param pc program counter
 * @param pos position in opram
 * @param opram pointer to opcode RAM
 * @return string with words defined
 */
QString z80Dasm::dasm_token(quint32 pc, off_t& pos, const quint8* opram)
{
    QString dasm;
    QString str = z80Token::string(z80Token::z80DEFM);
    bool first = true;
    bool instr = false;
    str.resize(7, QChar::Space);

    while (first || 0x00 == (opram[pos] & 0x80)) {
	if (first) {
	    first = false;
	} else if (!instr) {
	    str += QChar(',');
	}
	if (opram[pos] < 0x20 || opram[pos] == 0x7f) {
	    if (instr) {
		str += QChar('"');
		str += QChar(',');
		instr = false;
	    }
	    str += hexb(opram[pos]);
	} else {
	    if (0x80 & opram[pos]) {
		// First byte of a new token
		uint uc = unicode(opram[pos] & 0x7f);
		str += QString("\"%1\"+80h")
		       .arg(QChar(uc));
	    } else {
		if (!instr) {
		    str += QChar('"');
		    instr = true;
		}
		uint uc = unicode(opram[pos]);
		str += QChar(uc);
	    }
	}
	z80Def ent = m_defs->entry(pc + pos);
	if (ent->type() != z80DefObj::TOKEN)
	    break;
	pos++;
    }
    if (instr) {
	str += QChar('"');
	instr = false;
    }
    dasm += str;

    return dasm;
}

/**
 * @brief Disassemble a Z80 opcode
 *
 * Disassemble a single opcode. The distinction between @p oprom and @p opram
 * is here because MAME uses it to provide support for encrypted memory.
 *
 * This means opram could be different from oprom and point to a decrypted
 * memory range for the same program counter.
 *
 * Generally the @p oprom is used for the M1 cycle (the fetch opcode cycle) of
 * the Z80 while @p opram is used for data following that opcode.
 *
 * @param pc program counter
 * @param pos position in opram
 * @param flags flags for the opcode
 * @param oprom pointer to opcode ROM
 * @param opram pointer to opcode RAM
 * @return string with words defined
 */
QString z80Dasm::dasm_code(quint32 pc, off_t& pos, quint32& flags, const quint8* oprom, const quint8* opram)
{
    QString dasm;
    QString ixy = QLatin1String("oops!!");
    qint8 offset = 0;
    quint8 op = oprom[pos++];
    quint8 op1 = 0;
    quint32 ea = 0;
    z80Token d;

    switch (op) {
    case 0xcb:	// CB prefixed instructions (shift, rotate, bit manipulation, ...)
	op = oprom[pos++];
	d = z80Token::mnemonic_cb(op);
	break;

    case 0xed:	// ED prefixed instructions (block move, in, out, 16bit arith, ...)
	op1 = oprom[pos++];
	d = z80Token::mnemonic_ed(op1);
	break;

    case 0xdd:	// DD prefixed instructions (IX register)
	ixy = QLatin1String("ix");
	op1 = oprom[pos++];
	if(op1 == 0xcb) {
	    offset = static_cast<qint8>(opram[pos++]);
	    op1 = opram[pos++]; /* fourth byte from opbase.ram! */
	    d = z80Token::mnemonic_xx_cb(op1);
	} else {
	    d = z80Token::mnemonic_xx(op1);
	}
	break;

    case 0xfd:	// DD prefixed instructions (IY register)
	ixy = QLatin1String("iy");
	op1 = oprom[pos++];
	if (op1 == 0xcb) {
	    offset = static_cast<qint8>(opram[pos++]);
	    op1 = opram[pos++]; /* fourth byte from opbase.ram! */
	    d = z80Token::mnemonic_xx_cb(op1);
	} else {
	    d = z80Token::mnemonic_xx(op1);
	}
	break;

    default:
	d = z80Token::mnemonic_main(op);
	break;
    }

    if (nullptr == d.parameters()) {
	dasm += z80Token::string(d.mnemonic());
    } else {
	const char* params = d.parameters();
	QString str = z80Token::string(d.mnemonic());
	str.resize(7, QChar::Space);
	dasm += str;

	for (int i = 0; params[i]; i++) {
	    switch (params[i]) {
	    case '?':	// illegal opcode
		dasm += QString("%1,%2")
			.arg(hexb(op))
			.arg(hexb(op1));
		break;
	    case 'A':	// address op arg
		ea = rd16(opram + pos);
		pos += 2;
		dasm += symbol_w(ea);
		break;
	    case 'B':   // byte op arg
		ea = opram[pos++];
		dasm += hexb(ea);
		break;
	    case 'N':   // 16 bit immediate
		ea = rd16(opram + pos);
		pos += 2;
		dasm += hexw(ea);
		break;
	    case 'O':   // Offset relative to PC
		offset = static_cast<qint8>(opram[pos++]);
		ea = static_cast<quint16>(pc + offset + 2);
		dasm += symbol_w(ea);
		break;
	    case 'P':   // Port number
		ea = opram[pos++];
		dasm += hexb(ea);
		break;
	    case 'V':   // Restart vector
		ea = op & 0x38;
		dasm += hexb(ea);
		break;
	    case 'W':   // Memory address word
		ea = static_cast<quint32>(opram[pos+0]) |
			(static_cast<quint32>(opram[pos+1]) << 8);
		pos += 2;
		dasm += hexw(ea);
		break;
	    case 'X':	// Signed 7 bit offset
		offset = static_cast<qint8>(opram[pos++]);
		/* fall through */
	    case 'Y':	// IX or IY +/- offset
		dasm += QString("%1%2%3")
			.arg(ixy)
			.arg(sign(offset))
			.arg(hexb(offs(offset)));
		break;
	    case 'I':	// IX or IY
		dasm += ixy;
		break;
	    default:	// Literal character
		dasm += QChar::fromLatin1(params[i]);
	    }
	}
    }

    flags = d.flags();

    return dasm;
}

/**
 * @brief disassemble opcode at @p pc and return number of bytes it takes
 * @param pc current program counter
 * @param pos reference to a counter for the number of bytes of this instruction
 * @param flags reference to a flags value possibly set by the opcode
 * @param oprom pointer to the ROM at address of @p pc
 * @param opram pointer to the RAM at address of @p pc
 */
QString z80Dasm::dasm(quint32 pc, off_t& pos, quint32& flags, const quint8 *oprom, const quint8 *opram)
{
    QString buffer;
    QString dasm;
    QString ixy;
    z80Def def = m_defs->entry(pc);

    pos = 0;
    flags = 0;

    switch (def->type()) {
    case z80DefObj::DEFB:
	dasm += dasm_defb(pc, pos, opram);
	break;

    case z80DefObj::DEFW:
	dasm += dasm_defw(pc, pos, opram);
	break;

    case z80DefObj::DEFD:
	dasm += dasm_defw(pc, pos, opram);
	break;

    case z80DefObj::DEFS:
	dasm += dasm_defs(pc, pos, opram);
	break;

    case z80DefObj::TEXT:
	dasm += dasm_defm(pc, pos, opram);
	break;

    case z80DefObj::TOKEN:
	dasm += dasm_token(pc, pos, opram);
	break;

    case z80DefObj::CODE:
	dasm += dasm_code(pc, pos, flags, oprom, opram);
	break;

    default:
	dasm += QLatin1String("OUCH!");
	pos++;
    }

    if (m_uppercase)
	buffer += dasm.toUpper();
    else
	buffer += dasm.toLower();

    return buffer;
}

QStringList z80Dasm::listing(const QByteArray& memory, quint32 pc_min, quint32 pc_max)
{
    QStringList result;

    const quint8* oprom = reinterpret_cast<const quint8 *>(memory.constData());
    const quint8* opram = reinterpret_cast<const quint8 *>(memory.constData());

    result.reserve(pc_max + 1 - pc_min);

    for (quint32 pc = pc_min; pc <= pc_max; /* */) {
	z80Def def;

	def = m_defs->entry(pc);
	Q_ASSERT(!def.isNull());

	if (def->is_at_addr(pc) && def->has_block_comment()) {
	    QStringList comment = def->block_comment();
	    foreach(const QString& line, comment) {
		result += QString("; %1").arg(line);
	    }
	}

	if (def->is_at_addr(pc) && def->has_symbol()) {
	    QString symbol;
	    if (m_uppercase)
		symbol = def->symbol().toUpper();
	    else
		symbol = def->symbol().toLower();
	    result += QString("\n%1:").arg(symbol);
	}

	off_t bytes = 0;
	quint32 flags = 0;
	QString bdump;
	QString line = dasm(pc, bytes, flags, oprom + pc, opram + pc);
	for (off_t i = 0; i < m_bytes_per_line; i++) {
	    if (i < bytes) {
		bdump += x08(oprom[pc+i]);
	    } else {
		bdump += QLatin1String("  ");
	    }
	}
	QString buffer = QString("  %1: %2 %3")
			 .arg(x16(pc))
			 .arg(bdump)
			 .arg(line);

	if (def->is_at_addr(pc) && def->has_line_comment()) {
	    QString comment = def->line_comment();
	    if (buffer.length() < m_comment_column)
		buffer.resize(m_comment_column, QChar::Space);
	    if (comment.contains(QChar::LineFeed)) {
		QStringList comments = comment.split(QChar::LineFeed);
		buffer += QString("; %1").arg(comments.first());
		result += buffer;
		for (int i = 1; i < comments.count(); i++) {
		    buffer.fill(QChar::Space, m_comment_column);
		    buffer += QString("; %1").arg(comments.at(i));
		    if (i + 1 < comments.count())
			result += buffer;
		}
	    } else {
		buffer += QString("; %1").arg(comment);
	    }
	} else if (m_comment_glyphs) {
	    if (buffer.length() < m_comment_column)
		buffer.resize(m_comment_column, QChar::Space);
	    QString comment;
	    for (quint32 i = 0; i < bytes; i++) {
		uint uc = unicode(oprom[pc+i]);
		if (uc < 0x20) {
		    comment += QChar('.');
		} else {
		    comment += QChar(uc);
		}
	    }
	    buffer += QString("; %1").arg(comment);
	}

	result += buffer;
	if (flags & z80Token::DASMFLAG_FINAL)
	    result += QString();

	// more hex dump
	if (bytes >= m_bytes_per_line) {
	    for (quint32 offs = m_bytes_per_line; offs < bytes; offs += m_bytes_per_line) {

		QString bdump;
		for (off_t i = offs; i < offs + m_bytes_per_line; i++) {
		    if (i < bytes) {
			bdump += x08(oprom[pc+i]);
		    } else {
			bdump += QLatin1String("  ");
		    }
		}
		QString buffer = QString("  %1: %2")
				 .arg(x16(pc))
				 .arg(bdump);

		result += buffer;
	    }
	}

	pc += bytes;
    }

    return result;
}

quint32 z80Dasm::unicode(uchar ch) const
{
    if (m_bdf)
	return m_bdf->cgenie2unicode(ch);
    return QChar::fromLatin1(ch).unicode();
}
