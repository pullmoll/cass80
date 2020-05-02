/****************************************************************************
 *
 * Cass80 tool - Cassette XML format loader and writer
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
#include "cass80handler.h"
#include "cass80xml.h"

static const QLatin1String cassette_dtd(
"<!DOCTYPE cassette [\n"
"   <!ATTLIST cassette sha1 CDATA #REQUIRED>\n"
"   <!ELEMENT cassette (machine, format, name?, author?, copyright?, description?, sync, prefix?, blocks, image?, decoded?)>\n"
"           <!ELEMENT machine (trs80|eg2000)>\n"
"           <!ELEMENT format (basic|system)>\n"
"           <!ELEMENT name (#PCDATA)>\n"
"           <!ELEMENT author (#PCDATA)>\n"
"           <!ELEMENT copyright (#PCDATA)>\n"
"           <!ELEMENT description (#PCDATA)>\n"
"           <!ELEMENT sync (#PCDATA)>\n"
"           <!ELEMENT prefix (#PCDATA)>\n"
"           <!ELEMENT blocks (block*)>\n"
"                   <!ATTLIST blocks count CDATA #REQUIRED>\n"
"                   <!ELEMENT block (#PCDATA)>\n"
"                           <!ATTLIST block number CDATA #REQUIRED>\n"
"                           <!ATTLIST block type (basic|system|entry|raw) #REQUIRED>\n"
"                           <!ATTLIST block address CDATA #REQUIRED>\n"
"                           <!ATTLIST block size CDATA #IMPLIED>\n"
"                           <!ATTLIST block line CDATA #IMPLIED>\n"
"                           <!ATTLIST block csum CDATA #IMPLIED>\n"
"                           <!ATTLIST block sha1 CDATA #IMPLIED>\n"
"           <!ELEMENT image (#PCDATA)>\n"
"                   <!ATTLIST image size CDATA #REQUIRED>\n"
"                   <!ATTLIST image sha1 CDATA #IMPLIED>\n"
"           <!ELEMENT decoded (#PCDATA)>\n"
"]>\n");


CasXml::CasXml(QObject* parent)
    : QObject(parent)
{

}

QByteArray CasXml::sha1_digest() const
{
    return m_sha1_digest;
}

Cass80Machine CasXml::machine() const
{
    return m_machine;
}

bool CasXml::basic() const
{
    return m_basic;
}

QString CasXml::hdr_name() const
{
    return m_hdr_name;
}

QString CasXml::hdr_author() const
{
    return m_hdr_author;
}

QString CasXml::hdr_copyright() const
{
    return m_hdr_copyright;
}

QString CasXml::hdr_description() const
{
    return m_hdr_description;
}

quint8 CasXml::sync() const
{
    return m_sync;
}

quint8 CasXml::prefix() const
{
    return m_prefix;
}

QString CasXml::filename() const
{
    return m_filename;
}

CasBlockList CasXml::blocks() const
{
    return m_blocks;
}

QString CasXml::toXml() const
{
    QDomDocument doc = cas_document();
    return doc.toString(4);
}

void CasXml::set_data(const Cass80Handler* h)
{
    if (!h)
	return;
    set_machine(h->machine());
    set_basic(h->basic());
    set_hdr_name(h->hdr_name());
    set_hdr_author(h->hdr_author());
    set_hdr_copyright(h->hdr_copyright());
    set_hdr_description(h->hdr_description());
    set_sync(h->sync());
    set_prefix(h->prefix());
    set_filename(h->filename());
    set_blocks(h->blocks());
}

void CasXml::set_sha1_digest(const QByteArray& digest)
{
    if (digest == m_sha1_digest)
	return;
    m_sha1_digest = digest;
    emit sha1_digest_changed(m_sha1_digest);
}

void CasXml::set_machine(Cass80Machine machine)
{
    if (machine == m_machine)
	return;
    m_machine = machine;
    emit machine_changed(m_machine);
}

void CasXml::set_basic(bool basic)
{
    if (basic == m_basic)
	return;
    m_basic = basic;
    emit basic_changed(m_basic);
}

void CasXml::set_hdr_name(const QString& hdr_name)
{
    if (hdr_name == m_hdr_name)
	return;
    m_hdr_name = hdr_name;
    emit hdr_name_changed(m_hdr_name);
}

void CasXml::set_hdr_author(const QString& hdr_author)
{
    if (hdr_author == m_hdr_author)
	return;
    m_hdr_author = hdr_author;
    emit hdr_author_changed(m_hdr_author);
}

void CasXml::set_hdr_copyright(const QString& hdr_copyright)
{
    if (hdr_copyright == m_hdr_copyright)
	return;
    m_hdr_copyright = hdr_copyright;
    emit hdr_copyright_changed(m_hdr_copyright);
}

void CasXml::set_hdr_description(const QString& hdr_description)
{
    if (hdr_description == m_hdr_description)
	return;
    m_hdr_description = hdr_description;
    emit hdr_description_changed(m_hdr_description);
}

void CasXml::set_sync(quint8 sync)
{
    if (sync == m_sync)
	return;
    m_sync = sync;
    emit sync_changed(m_sync);
}

void CasXml::set_prefix(quint8 prefix)
{
    if (prefix == m_prefix)
	return;
    m_prefix = prefix;
    emit prefix_changed(m_prefix);
}

void CasXml::set_filename(const QString& filename)
{
    if (filename == m_filename)
	return;
    m_filename = filename;
    emit filename_changed(m_filename);
}

void CasXml::set_blocks(const CasBlockList& blocks)
{
    if (blocks == m_blocks)
	return;
    m_blocks = blocks;
    emit blocks_changed(m_blocks);
}

QDomElement CasXml::block_element(QDomDocument& doc, int index) const
{
    const Cass80Block& b = m_blocks[index];
    QDomElement block = doc.createElement(QLatin1String("block"));
    block.setAttribute(QLatin1String("number"), index);

    QString str = QLatin1String("invalid");
    switch (b.type) {
    case BT_INVALID:
	break;
    case BT_BASIC:
	str = QStringLiteral("basic");
	break;
    case BT_SYSTEM:
	str = QStringLiteral("system");
	break;
    case BT_ENTRY:
	str = QStringLiteral("entry");
	break;
    case BT_RAW:
	str = QStringLiteral("raw");
	break;
    }
    block.setAttribute(QLatin1String("type"), str);
    block.setAttribute(QLatin1String("address"), QString("0x%1").arg(b.addr, 4, 16, QChar('0')));
    if (b.type == BT_ENTRY)
	return block;

    block.setAttribute(QLatin1String("size"), QString("0x%1").arg(b.size, 4, 16, QChar('0')));
    if (m_basic)
	block.setAttribute(QLatin1String("line"), b.line);
    block.setAttribute(QLatin1String("csum"), QString("0x%1").arg(b.csum, 2, 16, QChar('0')));

    QByteArray hash = QCryptographicHash::hash(b.data, QCryptographicHash::Sha1);
    block.setAttribute(QLatin1String("sha1"), QString::fromLatin1(hash.toHex()));
    QDomText txt = doc.createTextNode(QString::fromLatin1(b.data.toHex()));
    block.appendChild(txt);

    return block;
}

QDomDocument CasXml::cas_document() const
{
    QDomDocument doc(QLatin1String("cassette"));
    QDomNode root = doc.createElement(QLatin1String("cassette"));
    QDomText txt;
    QString str;

    QDomElement machine = doc.createElement(QLatin1String("machine"));
    switch (m_machine) {
    case MACH_INVALID:
	 str = QStringLiteral("invalid");
	break;
    case MACH_TRS80:
	 str = QStringLiteral("trs80");
	break;
    case MACH_EG2000:
	 str = QStringLiteral("eg2000");
	break;
    }
    txt = doc.createTextNode(str);
    machine.appendChild(txt);
    root.appendChild(machine);

    QDomElement format = doc.createElement(QLatin1String("format"));
    if (m_basic) {
	str = QStringLiteral("basic");
    } else {
	str = QStringLiteral("system");
    }
    txt = doc.createTextNode(str);
    format.appendChild(txt);
    root.appendChild(format);

    QDomElement name = doc.createElement(QLatin1String("name"));
    txt = doc.createTextNode(m_hdr_name);
    name.appendChild(txt);
    root.appendChild(name);

    QDomElement author = doc.createElement(QLatin1String("author"));
    txt = doc.createTextNode(m_hdr_author);
    author.appendChild(txt);
    root.appendChild(author);

    QDomElement copyright = doc.createElement(QLatin1String("copyright"));
    txt = doc.createTextNode(m_hdr_copyright);
    copyright.appendChild(txt);
    root.appendChild(copyright);

    QDomElement description = doc.createElement(QLatin1String("description"));
    txt = doc.createTextNode(m_hdr_description);
    description.appendChild(txt);
    root.appendChild(description);

    QDomElement sync = doc.createElement(QLatin1String("sync"));
    txt = doc.createTextNode(QString("0x%1").arg(m_sync, 2, 16, QChar('0')));
    sync.appendChild(txt);
    root.appendChild(sync);

    QDomElement prefix = doc.createElement(QLatin1String("prefix"));
    txt = doc.createTextNode(QString("0x%1").arg(m_prefix, 2, 16, QChar('0')));
    prefix.appendChild(txt);
    root.appendChild(prefix);

    QDomElement blocks = doc.createElement(QLatin1String("blocks"));
    blocks.setAttribute(QLatin1String("count"), m_blocks.count());

    QByteArray image_data;
    for (int i = 0; i < m_blocks.count(); i++) {
	blocks.appendChild(block_element(doc, i));
	image_data += m_blocks[i].data;
    }

    root.appendChild(blocks);

    QDomElement image = doc.createElement(QLatin1String("image"));
    image.setAttribute(QLatin1String("size"), image_data.size());
    QByteArray hash = QCryptographicHash::hash(image_data, QCryptographicHash::Sha1);
    image.setAttribute(QLatin1String("sha1"), QString::fromLatin1(hash.toHex()));
    txt = doc.createTextNode(QString::fromLatin1(image_data.toHex()));
    image.appendChild(txt);

    root.appendChild(image);

    doc.appendChild(root);
    return doc;
}
