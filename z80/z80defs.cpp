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
#include <QTextStream>
#include "z80def.h"
#include "z80defs.h"

static const QLatin1String xml_tag_def("DEF");
static const QLatin1String xml_att_system("SYSTEM");

z80Defs::z80Defs(const QString& filename)
    : m_filename(filename)
    , m_system()
    , m_doc()
    , m_defs()
    , m_dummy(new z80DefObj())
{
    if (nullptr != m_filename)
	load();
    m_dummy->set_type(z80DefObj::CODE);
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
	    z80DefObj* def = new z80DefObj(child);
	    m_defs.insert(def->addr0(), z80Def(def));
	}
    } else {
	res = false;
    }

    return res;
}

bool z80Defs::save(const QString& filename)
{
    if (filename != nullptr)
	m_filename = filename;

    QDomDocument doc(xml_tag_def);
    QDomNode root = doc.createElement(xml_tag_def);
    foreach(const quint32 addr, m_defs.keys()) {
	const z80Def& def = m_defs[addr];
	QDomElement elm = def->toDomElement(doc, def);
	root.appendChild(elm);
    }
    doc.appendChild(root);
    QString xml = doc.toString(4);
    QFile file(m_filename);
    if (!file.open(QIODevice::WriteOnly)) {
	qCritical("Saving failed to create '%s'\n%s",
		  qPrintable(m_filename),
		  qPrintable(file.errorString()));
	return false;
    }
    QTextStream stream(&file);
    stream << xml;

    return QTextStream::Ok == stream.status();
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

z80Def z80Defs::entry(quint32 addr) const
{
    if (m_defs.contains(addr))
	return m_defs.value(addr);

    quint32 last = ~0u;
    // Scan backwards down to -32 bytes for an original entry
    for (quint32 pc = addr - 1; addr - pc < 32; --pc) {
	if (m_defs.contains(pc) && m_defs[pc]->has_symbol()) {
	    last = m_defs[pc]->addr0();
	    break;
	}
    }

    // No definitions at all: return an empty dummy
    if (last == ~0u) {
	m_dummy->set_addr(addr);
	return m_dummy;
    }

    // Create a new z80Def object
    z80DefObj* def = new z80DefObj(*m_defs[last]);
    quint32 addr0 = def->addr0();
    def->set_addr(addr);
    def->set_symbol(QString("%1+%2")
		  .arg(def->symbol())
		  .arg(addr - addr0));
    // And insert into table
    m_defs.insert(addr, z80Def(def));
    return m_defs[addr];
}

z80DefObj::EntryType z80Defs::type(quint32 addr) const
{
    z80Def d = entry(addr);
    if (d.isNull())
	return z80DefObj::INVALID;
    return d->type();
}

void z80Defs::insert(quint32 addr, z80DefObj* def)
{
    m_defs.insert(addr, z80Def(def));
}
