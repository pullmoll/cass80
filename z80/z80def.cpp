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

/**
 * @brief Return true, if the defintion is valid
 * @return true if valid, or false otherwise
 */
bool z80Def::isValid() const
{
    return m_type != INVALID;
}

/**
 * @brief Return true, if the defintion is exactly for @p addr
 * @param addr address to check
 * @return true if extact match, or false otherwise
 */
bool z80Def::isAt(quint32 addr) const
{
    return addr == m_addr0;
}

/**
 * @brief Return true, if the defintion has a symbol
 * @return true if a symbol is available, or false otherwise
 */
bool z80Def::hasSymbol() const
{
    return !(m_symbol.isNull() || m_symbol.isEmpty());
}

/**
 * @brief Return true, if the defintion has a comment
 * @return true if a comment is available, or false otherwise
 */
bool z80Def::hasComment() const
{
    return !(m_comment.isNull() || m_comment.isEmpty());
}

/**
 * @brief Return the symbol string
 * @return symbol name
 */
QString z80Def::symbol() const
{
    return m_symbol;
}

/**
 * @brief Return the comment string
 * @return comment string
 */
QString z80Def::comment() const
{
    return m_comment;
}

/**
 * @brief Return the original (unmodified) address for the definition
 * @return 32 bit (actually 16 bit used) address
 */
quint32 z80Def::addr0() const
{
    return m_addr0;
}

/**
 * @brief Return the address for the definition
 * @return 32 bit (actually 16 bit used) address
 */
quint32 z80Def::addr() const
{
    return m_addr;
}

/**
 * @brief Return the type for the definition
 * @return one of the @def z80Def::EntryType enumeration values
 */
z80Def::EntryType z80Def::type() const
{
    return m_type;
}

/**
 * @brief Return the string describing the meaning of argument 0
 * @return arg0 string
 */
QString z80Def::arg0() const
{
    return m_arg0;
}

/**
 * @brief Return the parameter for this defition
 * @return parameter value
 */
quint32 z80Def::param() const
{
    return m_param;
}

/**
 * @brief Return the max number of elements for this defition
 * @return max elements value
 */
quint32 z80Def::maxelem() const
{
    return m_maxelem;
}

/**
 * @brief Assignment
 * @param other z80Def to assign
 * @return this z80Def
 */
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

/**
 * @brief Set a new symbol string
 * @param symbol name
 */
void z80Def::setSymbol(const QString& symbol)
{
    m_symbol = symbol;
}

/**
 * @brief Set a new comment string
 * @param comment string
 */
void z80Def::setComment(const QString& comment)
{
    m_comment = comment;
}

/**
 * @brief Set a new symbol address
 * @param addr address value
 */
void z80Def::setAddr(const quint32 addr)
{
    m_addr = addr;
}

/**
 * @brief Set a new definition type
 * @param type one of @def z80Def::EntryType enumeration values
 */
void z80Def::setType(const z80Def::EntryType type)
{
    m_type = type;
}

/**
 * @brief Set a new arg0 string
 * @param arg0 new arg0 string
 */
void z80Def::setArg0(const QString& arg0)
{
    m_arg0 = arg0;
}

/**
 * @brief Set a new parameter value
 * @param param new parameter
 */
void z80Def::setParam(const quint32 param)
{
    m_param = param;
}

/**
 * @brief Set a new max elements value
 * @param maxelem max number of elements for this definition
 */
void z80Def::setMaxelem(const quint32 maxelem)
{
    m_maxelem = maxelem;
}

/**
 * @brief Return a z80Def filled from a QDomElement
 * @param elm const reference to a QDomElement for a defition
 * @return filled in z80Def
 */
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

QDomElement z80Def::toDomElement(QDomDocument& doc, const z80Def& def)
{
    if (!def.isAt(def.addr0())) {
	// Not an original address
	return QDomElement();
    }

    QString type;
    switch (def.type()) {
    case CODE:
	type = xml_val_code;
	break;
    case DEFB:
	type = xml_val_defb;
	break;
    case DEFW:
	type = xml_val_defw;
	break;
    case DEFD:
	type = xml_val_defd;
	break;
    case DEFS:
	type = xml_val_defs;
	break;
    case TEXT:
	type = xml_val_text;
	break;
    case TOKEN:
	type = xml_val_token;
	break;
    default:
	return QDomElement();
    }

    QDomElement elm = doc.createElement(xml_tag_entry);

    if (def.hasSymbol()) {
	QDomElement sym = doc.createElement(xml_tag_symbol);
	QDomText txt = doc.createTextNode(def.symbol());
	sym.appendChild(txt);
	elm.appendChild(sym);
    }

    if (def.hasComment()) {
	QDomElement cmt = doc.createElement(xml_tag_comment);
	QDomText txt = doc.createTextNode(def.comment());
	cmt.appendChild(txt);
	elm.appendChild(cmt);
    }

    elm.setAttribute(xml_att_addr, def.addr0());
    elm.setAttribute(xml_att_type, type);
    if (!def.arg0().isEmpty())
	elm.setAttribute(xml_att_arg0, def.arg0());
    if (0 != def.param())
	elm.setAttribute(xml_att_param, def.param());
    if (0 != def.maxelem())
	elm.setAttribute(xml_att_maxelem, def.maxelem());

    return elm;
}
