/****************************************************************************
 *
 * Cass80 tool - Raw font to BDF converter
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

bdfData::bdfData(int pixelsize, int width, int height)
    : m_pixelsize(pixelsize)
    , m_font_w(width)
    , m_font_h(height)
    , FOUNDRY("Company Name")
    , FONTNAME_REGISTRY("FontName")
    , FAMILY_NAME("FontFamily")
    , WEIGHT_NAME("Regular")
    , SLANT("Upright")
    , SETWIDTH_NAME("Normal")
    , ADD_STYLE_NAME("Ancient")
    , RESOLUTION_X(75)
    , RESOLUTION_Y(75)
    , SPACING("Monospaced")
    , CHARSET_REGISTRY("ISO8859")
    , CHARSET_ENCODING("1")
    , DEFAULT_CHAR(0)
    , FONT_DESCENT(0)
    , FONT_ASCENT(DEFAULT_BDF_PIXEL_SIZE)
    , COPYRIGHT("Copyright Holder (year)")
    , PIXEL_SIZE(height)
    , POINT_SIZE(10*height)
    , AVERAGE_WIDTH(10*width)
    , CAP_HEIGHT(height)
    , X_HEIGHT(height*3/4)
    , SCALABLE_WIDTH(1000*RESOLUTION_X/72)
    , SCALABLE_HEIGHT(0)
    , DEVICE_WIDTH(width)
    , DEVICE_HEIGHT(height)
    , BDF_BBOX_W(width)
    , BDF_BBOX_H(height)
    , BDF_BBOX_X(0)
    , BDF_BBOX_Y(0)
    , m_glyphs()
    , m_properties()
    , m_to_unicode()
    , m_from_unicode()
{
}

quint32 bdfData::toUnicode(uchar ch) const
{
    return m_to_unicode.value(ch);
}

uchar bdfData::fromUnicode(quint32 uc) const
{
    return m_from_unicode.value(uc);
}

void bdfData::set_FOUNDRY(const char* str)
{
    FOUNDRY = str;
}

void bdfData::set_FONTNAME_REGISTRY(const char* str)
{
    FONTNAME_REGISTRY = str;
}

void bdfData::set_FAMILY_NAME(const char* str)
{
    FAMILY_NAME = str;
}

void bdfData::set_WEIGHT_NAME(const char* str)
{
    WEIGHT_NAME = str;
}

void bdfData::set_SLANT(const char* str)
{
    SLANT = str;
}

void bdfData::set_SETWIDTH_NAME(const char* str)
{
    SETWIDTH_NAME = str;
}

void bdfData::set_ADD_STYLE_NAME(const char* str)
{
    ADD_STYLE_NAME = str;
}

void bdfData::set_RESOLUTION_X(int val)
{
    RESOLUTION_X = val;
}

void bdfData::set_RESOLUTION_Y(int val)
{
    RESOLUTION_Y = val;
}

void bdfData::set_SPACING(const char* str)
{
    SPACING = str;
}

void bdfData::set_CHARSET_REGISTRY(const char* str)
{
    CHARSET_REGISTRY = str;
}

void bdfData::set_CHARSET_ENCODING(const char* str)
{
    CHARSET_ENCODING = str;
}

void bdfData::set_DEFAULT_CHAR(int val)
{
    DEFAULT_CHAR = val;
}

void bdfData::set_FONT_DESCENT(int val)
{
    FONT_DESCENT = val;
}

void bdfData::set_FONT_ASCENT(int val)
{
    FONT_ASCENT = val;
}

void bdfData::set_COPYRIGHT(const char* str)
{
    COPYRIGHT = str;
}

void bdfData::set_PIXEL_SIZE(int val)
{
    PIXEL_SIZE = val;
}

void bdfData::set_POINT_SIZE(int val)
{
    POINT_SIZE = val;
}

void bdfData::set_AVERAGE_WIDTH(int val)
{
    AVERAGE_WIDTH = val;
}

void bdfData::set_CAP_HEIGHT(int val)
{
    CAP_HEIGHT = val;
}

void bdfData::set_X_HEIGHT(int val)
{
    X_HEIGHT = val;
}

void bdfData::set_SCALABLE_WIDTH(int val)
{
    SCALABLE_WIDTH = val;
}

void bdfData::set_SCALABLE_HEIGHT(int val)
{
    SCALABLE_HEIGHT = val;
}

void bdfData::set_DEVICE_WIDTH(int val)
{
    DEVICE_WIDTH = val;
}

void bdfData::set_DEVICE_HEIGHT(int val)
{
    DEVICE_HEIGHT = val;
}

void bdfData::set_BDF_BBOX_W(int val)
{
    BDF_BBOX_W = val;
}

void bdfData::set_BDF_BBOX_H(int val)
{
    BDF_BBOX_H = val;
}

void bdfData::set_BDF_BBOX_X(int val)
{
    BDF_BBOX_X = val;
}

void bdfData::set_BDF_BBOX_Y(int val)
{
    BDF_BBOX_Y = val;
}

void bdfData::insert(bdfGlyphObj* glyph)
{
    m_glyphs.insert(glyph->ch(), bdfGlyph(glyph));
    m_to_unicode.insert(glyph->ch(), glyph->uc());
    m_from_unicode.insert(glyph->uc(), glyph->ch());
}

void bdfData::setup_properties()
{
    PIXEL_SIZE = m_pixelsize;
    POINT_SIZE = 10 * m_pixelsize;
    AVERAGE_WIDTH = 10 * m_font_w;

    CAP_HEIGHT = m_font_h;
    X_HEIGHT = m_font_h * 3 / 4;
    SCALABLE_WIDTH = 1000 * RESOLUTION_X / 72;
    SCALABLE_HEIGHT = 0;
    DEVICE_WIDTH = m_font_w;
    DEVICE_HEIGHT = m_font_h;

    BDF_BBOX_W = m_font_w;
    BDF_BBOX_H = m_font_h;
    BDF_BBOX_X = 0;
    BDF_BBOX_Y = 0;

    m_properties.clear();

    m_properties += QString("%1 \"%2\"").arg("FONTNAME_REGISTRY").arg(FONTNAME_REGISTRY);
    m_properties += QString("%1 \"%2\"").arg("FOUNDRY").arg(FOUNDRY);
    m_properties += QString("%1 \"%2\"").arg("FAMILY_NAME").arg(FAMILY_NAME);
    m_properties += QString("%1 \"%2\"").arg("WEIGHT_NAME").arg(WEIGHT_NAME);
    m_properties += QString("%1 \"%2\"").arg("SLANT").arg(SLANT);
    m_properties += QString("%1 \"%2\"").arg("SETWIDTH_NAME").arg(SETWIDTH_NAME);
    m_properties += QString("%1 \"%2\"").arg("ADD_STYLE_NAME").arg(ADD_STYLE_NAME);
    m_properties += QString("%1 %2").arg("PIXEL_SIZE").arg(PIXEL_SIZE);
    m_properties += QString("%1 %2").arg("POINT_SIZE").arg(POINT_SIZE);
    m_properties += QString("%1 %2").arg("RESOLUTION_X").arg(RESOLUTION_X);
    m_properties += QString("%1 %2").arg("RESOLUTION_Y").arg(RESOLUTION_Y);
    m_properties += QString("%1 \"%2\"").arg("SPACING").arg(SPACING);
    m_properties += QString("%1 %2").arg("AVERAGE_WIDTH").arg(AVERAGE_WIDTH);
    m_properties += QString("%1 \"%2\"").arg("CHARSET_REGISTRY").arg(CHARSET_REGISTRY);
    m_properties += QString("%1 \"%2\"").arg("CHARSET_ENCODING").arg(CHARSET_ENCODING);
    m_properties += QString("%1 %2").arg("DEFAULT_CHAR").arg(DEFAULT_CHAR);
    m_properties += QString("%1 %2").arg("FONT_DESCENT").arg(FONT_DESCENT);
    m_properties += QString("%1 %2").arg("FONT_ASCENT").arg(FONT_ASCENT);
    m_properties += QString("%1 \"%2\"").arg("COPYRIGHT").arg(COPYRIGHT);
    m_properties += QString("%1 %2").arg("CAP_HEIGHT").arg(CAP_HEIGHT);
    m_properties += QString("%1 %2").arg("X_HEIGHT").arg(X_HEIGHT);
}

int bdfData::bit_chk(const QByteArray& buff, int x, int y)
{
    if (y < 0 || y >= m_font_h)
	return 0;
    if (x < 0 || x >= m_font_w)
	return 0;
    const uchar bits = *reinterpret_cast<const uchar *>(buff.constData() + y);
    return (bits >> (7 - (x % 8))) & 1;
}

void bdfData::bit_set(QByteArray& bitmap, int x)
{
    if (x < 0 || x >= m_pixelsize)
	return;
    uchar* bits = reinterpret_cast<uchar *>(bitmap.data() + x / 8);
    *bits |= 0x80 >> (x % 8);
}
