/****************************************************************************
 *
 * Cass80 tool - utility functions and helpers
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
#include "util.h"

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
QString util::hexb(quint32 val, bool upper)
{
    QString hex;
    if (val >= 0xa0U) {
	hex = QString("%1h").arg(val, 3, 16, QChar('0'));
    } else if (val >= 10) {
	hex = QString("%1h").arg(val, 2, 16, QChar('0'));
    } else {
	hex = QString::number(val);
    }
    return upper ? hex.toUpper() : hex;
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
QString util::hexw(quint32 val, bool upper)
{
    QString hex;
    if (val >= 0xa000U) {
	hex = QString("%1h").arg(val, 5, 16, QChar('0'));
    } else if (val >= 10) {
	hex = QString("%1h").arg(val, 4, 16, QChar('0'));
    } else {
	hex = QString::number(val);
    }
    return upper ? hex.toUpper() : hex;
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
QString util::hexd(quint32 val, bool upper)
{
    QString hex;
    if (val >= 0xa0000000UL) {
	hex = QString("%1h").arg(val, 9, 16, QChar('0'));
    } else if (val >= 10) {
	hex = QString("%1h").arg(val, 8, 16, QChar('0'));
    } else {
	hex = QString::number(val);
    }
    return upper ? hex.toUpper() : hex;
}

QString util::x08(quint32 val, bool upper)
{
    QString hex = QString("%1")
		  .arg(val & 0x000000ffu, 2, 16, QChar('0'));
    return upper ? hex.toUpper() : hex;
}

QString util::x16(quint32 val, bool upper)
{
    QString hex = QString("%1")
		  .arg(val & 0x0000ffffu, 4, 16, QChar('0'));
    return upper ? hex.toUpper() : hex;
}

QString util::x32(quint32 val, bool upper)
{
    QString hex = QString("%1")
		  .arg(val, 8, 16, QChar('0'));
    return upper ? hex.toUpper() : hex;
}

/**
 * @brief Read a byte from memory at @p mem
 * @param mem pointer to memory
 * @return byte value from memory
 */
quint32 util::rd08(const quint8* mem)
{
    return
	static_cast<quint32>(mem[0]);
}

/**
 * @brief Read a word from memory at @p mem
 * @param mem pointer to memory
 * @return word value from memory
 */
quint32 util::rd16(const quint8* mem)
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
quint32 util::rd32(const quint8* mem)
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
void util::wr08(quint8* mem, quint32 val)
{
    mem[0] = static_cast<quint8>(val);
}

/**
 * @brief Write a word to memory at @p mem
 * @param mem pointer to memory
 * @param val word value
 */
void util::wr16(quint8* mem, quint32 val)
{
    mem[0] = static_cast<quint8>(val >> 0);
    mem[1] = static_cast<quint8>(val >> 8);
}

/**
 * @brief Write a dword to memory at @p mem
 * @param mem pointer to memory
 * @param val dword value
 */
void util::wr32(quint8* mem, quint32 val)
{
    mem[0] = static_cast<quint8>(val >>  0);
    mem[1] = static_cast<quint8>(val >>  8);
    mem[2] = static_cast<quint8>(val >> 16);
    mem[3] = static_cast<quint8>(val >> 24);
}
