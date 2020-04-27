/****************************************************************************
 *
 * Cass80 tool - Z80 memory definition class
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
#include <QDomDocument>
#include <QDomElement>

class z80Def
{
public:
    enum EntryType {
	INVALID=-1,
	CODE,	    //!< z80 code
	DEFB,	    //!< define byte(s) (8 bit)
	DEFW,	    //!< define word(s) (16 bit)
	DEFD,	    //!< define dword(s) (32 bit)
	DEFS,	    //!< define space (unitialized)
	TEXT,	    //!< define text (char*)
	TOKEN,	    //!< array of tokens
    };

    explicit z80Def(const QDomElement& elm = QDomElement());
    z80Def(const z80Def& other);

    bool isValid() const;

    QString symbol() const;
    QString comment() const;
    quint32 addr() const;
    EntryType type() const;
    QString arg0() const;
    quint32 param() const;
    quint32 maxelem() const;

    z80Def operator= (const z80Def& other);

    void setSymbol(const QString& symbol = QString());
    void setComment(const QString& comment = QString());
    void setAddr(const quint32 addr = 0);
    void setType(const EntryType type = CODE);
    void setArg0(const QString& arg0 = QString());
    void setParam(const quint32 param = 0);
    void setMaxelem(const quint32 maxelem = 0);

    static z80Def fromDomElement(const QDomElement& elm);
    static QDomElement toDomElement(const QDomDocument& doc, const z80Def& def);
private:
    QString	m_symbol;
    QString	m_comment;
    quint32	m_addr;
    EntryType	m_type;
    QString	m_arg0;
    quint32	m_param;
    quint32	m_maxelem;
};
