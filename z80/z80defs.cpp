/****************************************************************************
 *
 * Cass80 tool - Z80 memory definitions
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
#include <QFile>
#include "z80def.h"
#include "z80defs.h"

static const QLatin1String xml_tag_def("DEF");
static const QLatin1String xml_att_system("SYSTEM");

z80Defs::z80Defs(const QString& filename)
    : m_filename(filename)
    , m_system()
    , m_doc()
    , m_defs()
{
    if (nullptr != m_filename)
	load();
}

bool z80Defs::load(const QString& filename)
{
    if (nullptr != filename)
	m_filename = filename;
    QFile xml(m_filename);
    if (!xml.open(QIODevice::ReadOnly)) {
	qCritical("Could not open '%s' for reading:\n%s", qPrintable(m_filename),
		  qPrintable(xml.errorString()));
	return false;
    }
    QString error_msg;
    int error_line = 0;
    int error_column = 0;
    bool res = m_doc.setContent(&xml, &error_msg, &error_line, & error_column);
    if (!res) {
	qCritical("Loading '%s' failed:\n%s at #%d:%d", qPrintable(m_filename),
		  qPrintable(error_msg), error_line, error_column);
    }

    QDomElement root = m_doc.firstChildElement();
    QString tag_name = root.tagName().toUpper();
    if (tag_name == xml_tag_def) {
	for (QDomElement child = root.firstChildElement(); !child.isNull(); child = child.nextSiblingElement()) {
	    z80Def def = z80Def(child);
	    if (z80Def::INVALID == def.type()) {
		def.setType(m_last_type);
	    } else {
		m_last_type = def.type();
		if (z80Def::DEFS == m_last_type)
		    m_last_type = z80Def::CODE;
	    }
	    m_defs.insert(def.addr(), def);
	}
    } else {
	res = false;
    }

    return res;
}

QString z80Defs::filename() const
{
    return m_filename;
}

QString z80Defs::system() const
{
    return m_system;
}

QMap<quint32, z80Def> z80Defs::defs() const
{
    return m_defs;
}

z80Def z80Defs::def(quint32 addr) const
{
    if (m_defs.contains(addr))
	return m_defs.value(addr);

    // search entry with highest key < addr
    quint32 last = 0;
    foreach(const quint32 key, m_defs.keys()) {
	if (key < addr)
	    last = key;
    }
    z80Def def = m_defs.value(last);
    quint32 addr0 = def.addr();
    def.setAddr(addr);
    def.setSymbol(QString("%1+%2")
		  .arg(def.symbol())
		  .arg(addr - addr0));
    return def;
}

z80Def::EntryType z80Defs::type(quint32 addr) const
{
    return def(addr).type();
}
