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
    , m_defs(defs)
    , m_bdf(bdf)
{
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
    if (val >= 0xa0U) {
	return QString("%1H").arg(val, 3, 16, QChar('0'));
    }
    if (val >= 0x0aU) {
	return QString("%1H").arg(val, 2, 16, QChar('0'));
    }
    return QString::number(val);
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
    if (val >= 0xa000U) {
	return QString("%1H").arg(val, 5, 16, QChar('0'));
    }
    if (val >= 0x000aU) {
	return QString("%1H").arg(val, 4, 16, QChar('0'));
    }
    return QString::number(val);
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
    if (val >= 0xa0000000LU) {
	return QString("%1H").arg(val, 9, 16, QChar('0'));
    }
    if (val >= 0x0000000aLU) {
	return QString("%1H").arg(val, 8, 16, QChar('0'));
    }
    return QString::number(val);
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

/**
 * @brief Disassemble a DEFB byte or multiple bytes
 *
 * TODO: respect z80Def::maxelem() and return a string
 * for possibly multiple bytes, either maxelem or up to
 * the next z80Def entry.
 *
 * @param pc program counter
 * @param pos position in opram
 * @param opram pointer to opcode RAM
 * @return string with bytes defined
 */
QString z80Dasm::dasm_defb(quint32 pc, quint32& pos, const quint8* opram)
{
    Q_UNUSED(pc)
    QString dasm;
    QString str = z80Token::string(z80Token::zDEFB);
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
 * TODO: respect z80Def::maxelem() and return a string
 * for possibly multiple words, either maxelem or up to
 * the next z80Def entry.
 *
 * @param pc program counter
 * @param pos position in opram
 * @param opram pointer to opcode RAM
 * @return string with words defined
 */
QString z80Dasm::dasm_defw(quint32 pc, quint32& pos, const quint8* opram)
{
    Q_UNUSED(pc)
    QString dasm;
    QString str = z80Token::string(z80Token::zDEFW);
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
 * TODO: respect z80Def::maxelem() and return a string
 * for possibly multiple dwords, either maxelem or up to
 * the next z80Def entry.
 *
 * @param pc program counter
 * @param pos position in opram
 * @param opram pointer to opcode RAM
 * @return string with words defined
 */
QString z80Dasm::dasm_defd(quint32 pc, quint32& pos, const quint8* opram)
{
    Q_UNUSED(pc)
    QString dasm;
    QString str = z80Token::string(z80Token::zDEFD);
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
QString z80Dasm::dasm_defs(quint32 pc, quint32& pos, const quint8* opram)
{
    QString dasm;
    QString str = z80Token::string(z80Token::zDEFS);
    str.resize(7, QChar::Space);
    uint8_t byte = opram[pos];
    int n = 0;
    while (pc + pos < 0x10000 && byte == opram[pos]) {
	n++;
	pos++;
	if (m_defs && m_defs->def(pc + pos).type() != z80Def::DEFS)
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
QString z80Dasm::dasm_defm(quint32 pc, quint32& pos, const quint8* opram)
{
    QString dasm;
    QString str = z80Token::string(z80Token::zDEFM);
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

	if (m_defs && m_defs->def(pc + pos).type() != z80Def::TEXT)
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
QString z80Dasm::dasm_token(quint32 pc, quint32& pos, const quint8* opram)
{
    QString dasm;
    QString str = z80Token::string(z80Token::zDEFM);
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
	if (m_defs && m_defs->def(pc + pos).type() != z80Def::TOKEN)
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
QString z80Dasm::dasm_code(quint32 pc, quint32& pos, quint32& flags, const quint8* oprom, const quint8* opram)
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
		ea = static_cast<quint32>(opram[pos+0]) |
			(static_cast<quint32>(opram[pos+1]) << 8);
		pos += 2;
		dasm += hexw(ea);
		break;
	    case 'B':   // byte op arg
		ea = opram[pos++];
		dasm += hexb(ea);
		break;
	    case 'N':   // 16 bit immediate
		ea = static_cast<quint32>(opram[pos+0]) |
			(static_cast<quint32>(opram[pos+1]) << 8);
		pos += 2;
		dasm += hexw(ea);
		break;
	    case 'O':   // Offset relative to PC
		offset = static_cast<qint8>(opram[pos++]);
		ea = static_cast<quint16>(pc + offset + 2);
		dasm += hexw(ea);
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

    flags = d.flags(d.mnemonic());

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
QString z80Dasm::dasm(quint32 pc, quint32& pos, quint32& flags, const quint8 *oprom, const quint8 *opram)
{
    QString buffer;
    QString dasm;
    QString ixy;
    z80Def def;

    flags = 0;
    def.setAddr(pc);
    def.setType(z80Def::CODE);
    pos = 0;

    if (m_defs)
	def = m_defs->def(pc);

    switch (def.type()) {
    case z80Def::DEFB:
	dasm += dasm_defb(pc, pos, opram);
	break;

    case z80Def::DEFW:
	dasm += dasm_defw(pc, pos, opram);
	break;

    case z80Def::DEFD:
	dasm += dasm_defw(pc, pos, opram);
	break;

    case z80Def::DEFS:
	dasm += dasm_defs(pc, pos, opram);
	break;

    case z80Def::TEXT:
	dasm += dasm_defm(pc, pos, opram);
	break;

    case z80Def::TOKEN:
	dasm += dasm_token(pc, pos, opram);
	break;

    case z80Def::CODE:
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

    // TODO: z80Token::flags(d.mnemonic()) | DASMFLAG_SUPPORTED
    return buffer;
}

QStringList z80Dasm::list(const QByteArray& memory, quint32 pc_min, quint32 pc_max)
{
    QStringList listing;
    const quint8* oprom = reinterpret_cast<const quint8 *>(memory.constData());
    const quint8* opram = reinterpret_cast<const quint8 *>(memory.constData());

    for (quint32 pc = pc_min; pc < pc_max; /* */) {
	z80Def def;

	if (m_defs) {
	    def = m_defs->def(pc);
	    if (def.isAt(pc)) {
		QString buffer;
		if (def.hasSymbol()) {
		    buffer = QString("\n%1:")
			     .arg(def.symbol());
		    if (def.hasComment()) {
			QString comment = def.comment();
			if (comment.contains(QChar::LineFeed)) {
			    foreach(const QString& line, def.comment().split(QChar::LineFeed)) {
				buffer += QString("; %1")
					  .arg(line);
				listing += buffer;
				buffer.clear();
			    }
			} else {
			    buffer += QString("; %1")
				      .arg(comment);
			}
		    }
		    listing += buffer;
		}
	    }
	}

	quint32 bytes = 0;
	quint32 flags = 0;
	QString line = dasm(pc, bytes, flags, oprom + pc, opram + pc);
	QString buffer = QString("%1: ")
			 .arg(pc, 4, 16, QChar('0'));
	if (m_uppercase)
	    buffer = buffer.toUpper();

	for (quint32 i = 0; i < m_bytes_per_line; i++) {
	    if (i < bytes) {
		QString byte = QString("%1 ").arg(oprom[pc+i], 2, 16, QChar('0'));
		if (m_uppercase)
		    buffer += byte.toUpper();
		else
		    buffer += byte.toLower();
	    } else {
		buffer += QLatin1String("   ");
	    }
	}
	buffer += line;

	if (def.isAt(pc) && def.hasComment()) {
	    if (buffer.length() < 56)
		buffer.resize(56, QChar::Space);
	    buffer += QString("; %1")
		      .arg(def.comment());
	} else if (m_comment_glyphs) {
	    if (buffer.length() < 56)
		buffer.resize(56, QChar::Space);
	    buffer += QLatin1String("; ");
	    for (quint32 i = 0; i < bytes; i++) {
		uint uc = unicode(oprom[pc+i]);
		if (uc < 0x20) {
		    buffer += QChar('.');
		} else {
		    buffer += QChar(uc);
		}
	    }
	}

	listing += buffer;

	// more hex dump
	if (bytes >= m_bytes_per_line) {
	    for (quint32 offs = m_bytes_per_line; offs < bytes; offs += m_bytes_per_line) {
		buffer = QString("%1: ")
			 .arg(pc + offs, 4, 16, QChar('0'));
		if (m_uppercase)
		    buffer = buffer.toUpper();
		for (quint32 i = offs; i < offs+m_bytes_per_line && i < bytes; i++) {
		    QString byte = QString("%1 ").arg(oprom[pc+i], 2, 16, QChar('0'));
		    if (m_uppercase)
			buffer += byte.toUpper();
		    else
			buffer += byte.toLower();
		}
		listing += buffer;
	    }
	}

	pc += bytes;
    }

    return listing;
}

quint32 z80Dasm::unicode(uchar ch) const
{
    if (m_bdf)
	return m_bdf->cgenie2unicode(ch);
    return QChar::fromLatin1(ch).unicode();
}
