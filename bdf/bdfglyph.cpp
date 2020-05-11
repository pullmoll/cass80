/****************************************************************************
 *
 * Cass80 tool - BDF glyph class (char, unicode, name)
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
#include "bdfglyph.h"

//! Excerpt from the Unicode database for codes 00-7f
static const char* ucd[128] = {
    "control NULL",
    "control START OF HEADING ",
    "control START OF TEXT ",
    "control END OF TEXT ",
    "control END OF TRANSMISSION ",
    "control ENQUIRY ",
    "control ACKNOWLEDGE ",
    "control BELL ",
    "control BACKSPACE ",
    "control CHARACTER TABULATION ",
    "control LINE FEED (LF) ",
    "control LINE TABULATION ",
    "control FORM FEED (FF) ",
    "control CARRIAGE RETURN (CR) ",
    "control SHIFT OUT ",
    "control SHIFT IN ",
    "control DATA LINK ESCAPE ",
    "control DEVICE CONTROL ONE ",
    "control DEVICE CONTROL TWO ",
    "control DEVICE CONTROL THREE ",
    "control DEVICE CONTROL FOUR ",
    "control NEGATIVE ACKNOWLEDGE ",
    "control SYNCHRONOUS IDLE ",
    "control END OF TRANSMISSION BLOCK ",
    "control CANCEL ",
    "control END OF MEDIUM ",
    "control SUBSTITUTE ",
    "control ESCAPE ",
    "control INFORMATION SEPARATOR FOUR ",
    "control INFORMATION SEPARATOR THREE ",
    "control INFORMATION SEPARATOR TWO ",
    "control INFORMATION SEPARATOR ONE ",
    "SPACE",
    "EXCLAMATION MARK",
    "QUOTATION MARK",
    "NUMBER SIGN",
    "DOLLAR SIGN",
    "PERCENT SIGN",
    "AMPERSAND",
    "APOSTROPHE",
    "LEFT PARENTHESIS",
    "RIGHT PARENTHESIS",
    "ASTERISK",
    "PLUS SIGN",
    "COMMA",
    "HYPHEN-MINUS",
    "FULL STOP",
    "SOLIDUS",
    "DIGIT ZERO",
    "DIGIT ONE",
    "DIGIT TWO",
    "DIGIT THREE",
    "DIGIT FOUR",
    "DIGIT FIVE",
    "DIGIT SIX",
    "DIGIT SEVEN",
    "DIGIT EIGHT",
    "DIGIT NINE",
    "COLON",
    "SEMICOLON",
    "LESS-THAN SIGN",
    "EQUALS SIGN",
    "GREATER-THAN SIGN",
    "QUESTION MARK",
    "COMMERCIAL AT",
    "LATIN CAPITAL LETTER A",
    "LATIN CAPITAL LETTER B",
    "LATIN CAPITAL LETTER C",
    "LATIN CAPITAL LETTER D",
    "LATIN CAPITAL LETTER E",
    "LATIN CAPITAL LETTER F",
    "LATIN CAPITAL LETTER G",
    "LATIN CAPITAL LETTER H",
    "LATIN CAPITAL LETTER I",
    "LATIN CAPITAL LETTER J",
    "LATIN CAPITAL LETTER K",
    "LATIN CAPITAL LETTER L",
    "LATIN CAPITAL LETTER M",
    "LATIN CAPITAL LETTER N",
    "LATIN CAPITAL LETTER O",
    "LATIN CAPITAL LETTER P",
    "LATIN CAPITAL LETTER Q",
    "LATIN CAPITAL LETTER R",
    "LATIN CAPITAL LETTER S",
    "LATIN CAPITAL LETTER T",
    "LATIN CAPITAL LETTER U",
    "LATIN CAPITAL LETTER V",
    "LATIN CAPITAL LETTER W",
    "LATIN CAPITAL LETTER X",
    "LATIN CAPITAL LETTER Y",
    "LATIN CAPITAL LETTER Z",
    "LEFT SQUARE BRACKET",
    "REVERSE SOLIDUS",
    "RIGHT SQUARE BRACKET",
    "CIRCUMFLEX ACCENT",
    "LOW LINE",
    "GRAVE ACCENT",
    "LATIN SMALL LETTER A",
    "LATIN SMALL LETTER B",
    "LATIN SMALL LETTER C",
    "LATIN SMALL LETTER D",
    "LATIN SMALL LETTER E",
    "LATIN SMALL LETTER F",
    "LATIN SMALL LETTER G",
    "LATIN SMALL LETTER H",
    "LATIN SMALL LETTER I",
    "LATIN SMALL LETTER J",
    "LATIN SMALL LETTER K",
    "LATIN SMALL LETTER L",
    "LATIN SMALL LETTER M",
    "LATIN SMALL LETTER N",
    "LATIN SMALL LETTER O",
    "LATIN SMALL LETTER P",
    "LATIN SMALL LETTER Q",
    "LATIN SMALL LETTER R",
    "LATIN SMALL LETTER S",
    "LATIN SMALL LETTER T",
    "LATIN SMALL LETTER U",
    "LATIN SMALL LETTER V",
    "LATIN SMALL LETTER W",
    "LATIN SMALL LETTER X",
    "LATIN SMALL LETTER Y",
    "LATIN SMALL LETTER Z",
    "LEFT CURLY BRACKET",
    "VERTICAL LINE",
    "RIGHT CURLY BRACKET",
    "TILDE",
    "control DELETE",
};

bdfGlyphObj::bdfGlyphObj(const uchar ch, const quint32 uc, const QString& name)
    : m_char(ch)
    , m_unicode(uc)
    , m_name(name)
{
}

uchar bdfGlyphObj::ch() const
{
    return m_char;
}

quint32 bdfGlyphObj::uc() const
{
    return m_unicode;
}

QString bdfGlyphObj::name() const
{
    if (m_unicode < 0x80)
	return QString::fromLatin1(ucd[m_unicode]);
    return m_name;
}
