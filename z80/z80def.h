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
#include <QStringList>
#include <QSharedPointer>

class z80DefObj;
typedef QSharedPointer<z80DefObj> z80Def;

class z80DefObj
{
public:
    enum EntryType {
	INVALID=-1,
	CODE,	    //!< Z80 code
	DEFB,	    //!< define byte(s) (8 bit)
	DEFW,	    //!< define word(s) (16 bit)
	DEFD,	    //!< define dword(s) (32 bit)
	DEFS,	    //!< define space (unitialized)
	TEXT,	    //!< define text (char*)
	TOKEN,	    //!< array of tokens
    };

    explicit z80DefObj(const QDomElement& elm = QDomElement());
    z80DefObj(const z80DefObj& other);

    bool isValid() const;
    bool is_at_addr(quint32 addr) const;
    bool has_symbol() const;
    bool has_block_comment() const;
    bool has_line_comment() const;

    QString symbol() const;
    QStringList block_comment() const;
    QString line_comment() const;
    quint32 addr0() const;
    quint32 addr() const;
    EntryType type() const;
    QString arg0() const;
    quint32 param() const;
    quint32 maxelem() const;

    z80DefObj operator= (const z80DefObj& other);

    void set_symbol(const QString& symbol = QString());
    void set_block_comment(const QStringList& lines = QStringList());
    void set_line_comment(const QString& line_comment = QString());
    void set_addr0(const quint32 addr = 0);
    void set_addr(const quint32 addr = 0);
    void set_type(const EntryType type = CODE);
    void set_arg0(const QString& arg0 = QString());
    void set_param(const quint32 param = 0);
    void set_maxelem(const quint32 maxelem = 0);

    static z80DefObj fromDomElement(const QDomElement& elm);
    static QDomElement toDomElement(QDomDocument& doc, const z80Def& def);
private:
    QString	m_symbol;
    QStringList	m_block_comment;
    QString	m_line_comment;
    quint32	m_addr0;
    quint32	m_addr;
    EntryType	m_type;
    QString	m_arg0;
    quint32	m_param;
    quint32	m_maxelem;
};
