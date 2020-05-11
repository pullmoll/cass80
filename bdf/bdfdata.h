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
#pragma once
#include "bdfglyph.h"

#define	DEFAULT_BDF_PIXEL_SIZE	16

/**
 * @brief The bdfData class is a virtual class defining the
 * interface to embedded BDF font generators and translators.
 */
class bdfData
{
public:
    explicit bdfData(int pixelsize = 0, int width = 8, int height = 8);
    virtual bool generate(const QString& fntname, const QString& bdfname) = 0;

    virtual quint32 toUnicode(uchar ch) const;
    virtual uchar fromUnicode(quint32 uc) const;

protected:
    void insert(bdfGlyphObj* glyph);
    void setup_properties();
    int bit_chk(const QByteArray& buff, int x, int y);
    void bit_set(QByteArray& bitmap, int x);

public slots:
    void set_FOUNDRY(const char* str = nullptr);
    void set_FONTNAME_REGISTRY(const char* str = nullptr);
    void set_FAMILY_NAME(const char* str = nullptr);
    void set_WEIGHT_NAME(const char* str = nullptr);
    void set_SLANT(const char* str = nullptr);
    void set_SETWIDTH_NAME(const char* str = nullptr);
    void set_ADD_STYLE_NAME(const char* str = nullptr);
    void set_RESOLUTION_X(int val = 75);
    void set_RESOLUTION_Y(int val = 75);
    void set_SPACING(const char* str = nullptr);
    void set_CHARSET_REGISTRY(const char* str = nullptr);
    void set_CHARSET_ENCODING(const char* str = nullptr);
    void set_DEFAULT_CHAR(int val = 0);
    void set_FONT_DESCENT(int val = 8);
    void set_FONT_ASCENT(int val = 0);
    void set_COPYRIGHT(const char* str = nullptr);
    void set_PIXEL_SIZE(int val = 8);
    void set_POINT_SIZE(int val = 8);
    void set_AVERAGE_WIDTH(int val = 8 * 10);
    void set_CAP_HEIGHT(int val = 8);
    void set_X_HEIGHT(int val = 6);
    void set_SCALABLE_WIDTH(int val = 500);
    void set_SCALABLE_HEIGHT(int val = 0);
    void set_DEVICE_WIDTH(int val = 8);
    void set_DEVICE_HEIGHT(int val = 8);
    void set_BDF_BBOX_W(int val = 8);
    void set_BDF_BBOX_H(int val = 8);
    void set_BDF_BBOX_X(int val = 0);
    void set_BDF_BBOX_Y(int val = 0);

protected:
    int m_pixelsize;
    int m_font_w;
    int m_font_h;

    const char* FOUNDRY;
    const char* FONTNAME_REGISTRY;
    const char* FAMILY_NAME;
    const char* WEIGHT_NAME;
    const char* SLANT;
    const char* SETWIDTH_NAME;
    const char* ADD_STYLE_NAME;
    int RESOLUTION_X;
    int RESOLUTION_Y;
    const char* SPACING;
    const char* CHARSET_REGISTRY;
    const char* CHARSET_ENCODING;
    int DEFAULT_CHAR;
    int FONT_DESCENT;
    int FONT_ASCENT;
    const char* COPYRIGHT;

    int PIXEL_SIZE = DEFAULT_BDF_PIXEL_SIZE;
    int POINT_SIZE = DEFAULT_BDF_PIXEL_SIZE * 10;
    int AVERAGE_WIDTH = DEFAULT_BDF_PIXEL_SIZE * 10;

    int CAP_HEIGHT = DEFAULT_BDF_PIXEL_SIZE;
    int X_HEIGHT = DEFAULT_BDF_PIXEL_SIZE * 3 / 4;

    int SCALABLE_WIDTH = 1000 * RESOLUTION_X / 72;
    int SCALABLE_HEIGHT = 0;   // default
    int DEVICE_WIDTH = 8;
    int DEVICE_HEIGHT = 0;   // default

    int BDF_BBOX_W = DEFAULT_BDF_PIXEL_SIZE;
    int BDF_BBOX_H = DEFAULT_BDF_PIXEL_SIZE;
    int BDF_BBOX_X = 0;
    int BDF_BBOX_Y = 0;

    QHash<uchar,bdfGlyph> m_glyphs;
    QStringList m_properties;
    QHash<uchar,quint32> m_to_unicode;
    QHash<quint32,uchar> m_from_unicode;
};
