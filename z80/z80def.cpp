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
#include "z80def.h"

static const QLatin1String xml_tag_entry("entry");
static const QLatin1String xml_tag_symbol("symbol");
static const QLatin1String xml_tag_comment("comment");
static const QLatin1String xml_att_addr("addr");
static const QLatin1String xml_att_type("type");
static const QLatin1String xml_att_arg0("arg0");
static const QLatin1String xml_att_maxelem("maxelem");
static const QLatin1String xml_att_param("param");

static const QLatin1String xml_val_code("code");
static const QLatin1String xml_val_defb("defb");
static const QLatin1String xml_val_defw("defw");
static const QLatin1String xml_val_defd("defd");
static const QLatin1String xml_val_defs("defs");
static const QLatin1String xml_val_text("text");
static const QLatin1String xml_val_token("token");

z80Def::z80Def(const QDomElement& elm)
    : m_symbol()
    , m_comment()
    , m_addr0(0)
    , m_addr(0)
    , m_type(INVALID)
    , m_arg0()
    , m_param(0)
    , m_maxelem(0)
{
    if (!elm.isNull())
	*this = z80Def::fromDomElement(elm);
}

z80Def::z80Def(const z80Def& other)
{
    m_symbol = other.m_symbol;
    m_comment = other.m_comment;
    m_addr0 = other.m_addr0;
    m_addr = other.m_addr;
    m_type = other.m_type;
    m_arg0 = other.m_arg0;
    m_param = other.m_param;
    m_maxelem = other.m_maxelem;
}

bool z80Def::isValid() const
{
    return m_type != INVALID;
}

bool z80Def::isAt(quint32 addr) const
{
    return addr == m_addr0;
}

bool z80Def::hasSymbol() const
{
    return !(m_symbol.isNull() || m_symbol.isEmpty());
}

bool z80Def::hasComment() const
{
    return !(m_comment.isNull() || m_comment.isEmpty());
}

QString z80Def::symbol() const
{
    return m_symbol;
}

QString z80Def::comment() const
{
    return m_comment;
}

quint32 z80Def::addr() const
{
    return m_addr;
}

z80Def::EntryType z80Def::type() const
{
    return m_type;
}

QString z80Def::arg0() const
{
    return m_arg0;
}

quint32 z80Def::param() const
{
    return m_param;
}

quint32 z80Def::maxelem() const
{
    return m_maxelem;
}

z80Def z80Def::operator=(const z80Def& other)
{
    m_symbol = other.m_symbol;
    m_comment = other.m_comment;
    m_addr0 = other.m_addr0;
    m_addr = other.m_addr;
    m_type = other.m_type;
    m_arg0 = other.m_arg0;
    m_param = other.m_param;
    m_maxelem = other.m_maxelem;
    return *this;
}

void z80Def::setSymbol(const QString& symbol)
{
    m_symbol = symbol;
}

void z80Def::setComment(const QString& comment)
{
    m_comment = comment;
}

void z80Def::setAddr(const quint32 addr)
{
    m_addr = addr;
}

void z80Def::setType(const z80Def::EntryType type)
{
    m_type = type;
}

void z80Def::setArg0(const QString& arg0)
{
    m_arg0 = arg0;
}

void z80Def::setParam(const quint32 param)
{
    m_param = param;
}

void z80Def::setMaxelem(const quint32 maxelem)
{
    m_maxelem = maxelem;
}

z80Def z80Def::fromDomElement(const QDomElement& elm)
{
    z80Def def;
    QString tag_name;
    QString att;

    tag_name = elm.tagName().toLower();
    if (tag_name != xml_tag_entry) {
	qDebug("%s: QDomElement tag is not '%s' but '%s'", __func__,
	       qPrintable(xml_tag_entry), qPrintable(tag_name));
	return def;
    }

    att = xml_att_addr;
    if (elm.attribute(att).isEmpty()) {
	def.m_addr0 = def.m_addr = ~0u;
    } else {
	QString val = elm.attribute(att);
	bool ok;
	def.m_addr0 = def.m_addr = val.toUInt(&ok, 16);
	if (!ok) {
	    qDebug("%s: unknown attribute %s value '%s'", __func__, qPrintable(att), qPrintable(val));
	    def.m_addr0 = def.m_addr = ~0u;
	}
    }

    att = xml_att_arg0;
    if (elm.attribute(att).isEmpty()) {
	def.m_arg0 = nullptr;
    } else {
	def.m_arg0 = elm.attribute(att);
	// TODO: check value?
    }

    att = xml_att_param;
    if (elm.attribute(att).isEmpty()) {
	def.m_param = ~0u;
    } else {
	QString val = elm.attribute(att);
	bool ok;
	def.m_param = val.toUInt(&ok, 16);
	if (!ok) {
	    qDebug("%s: unknown attribute %s value '%s'", __func__, qPrintable(att), qPrintable(val));
	    def.m_param = ~0u;
	}
    }

    att = xml_att_maxelem;
    if (elm.attribute(att).isEmpty()) {
	def.m_maxelem = 0;
    } else {
	QString val = elm.attribute(att);
	bool ok;
	def.m_maxelem = val.toUInt(&ok, 16);;
	if (!ok) {
	    qDebug("%s: unknown attribute %s value '%s'", __func__, qPrintable(att), qPrintable(val));
	    def.m_maxelem = 0;
	}
    }

    att = xml_att_type;
    if (elm.attribute(att).isEmpty()) {
	def.m_type = CODE;
    } else {
	QString type = elm.attribute(att).toLower();
	do {
	    if (type == xml_val_code) {
		def.m_type = CODE;
		continue;
	    }
	    if (type == xml_val_defb) {
		def.m_type = DEFB;
		continue;
	    }
	    if (type == xml_val_defw) {
		def.m_type = DEFW;
		continue;
	    }
	    if (type == xml_val_defd) {
		def.m_type = DEFD;
		continue;
	    }
	    if (type == xml_val_defs) {
		def.m_type = DEFS;
		continue;
	    }
	    if (type == xml_val_text) {
		def.m_type = TEXT;
		continue;
	    }
	    if (type == xml_val_token) {
		def.m_type = TOKEN;
		continue;
	    }
	    qDebug("%s: unknown type value '%s'", __func__, qPrintable(type));
	    def.m_type = CODE;
	} while (0);
    }

    QDomNode child = elm.firstChild();
    for (QDomNode child = elm.firstChild(); !child.isNull(); child = child.nextSibling()) {
	QDomElement e = child.toElement();

	tag_name = e.tagName().toLower();
	if (tag_name == xml_tag_symbol) {
	    QDomText txt = e.firstChild().toText();
	    if (!txt.isNull()) {
		def.m_symbol = txt.data();
		continue;
	    }
	    qDebug("%s: tag '%s' is not a text node", __func__, qPrintable(tag_name));
	}

	if (tag_name == xml_tag_comment) {
	    QDomText txt = e.firstChild().toText();
	    if (!txt.isNull()) {
		def.m_comment = txt.data();
		continue;
	    }
	    qDebug("%s: tag '%s' is not a text node", __func__, qPrintable(tag_name));
	}
    }

    return def;
}
