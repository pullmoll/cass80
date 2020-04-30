/****************************************************************************
 *
 * Cass80 tool - ROM listing to z80Def XML extractor
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
#include "listing2xml.h"

typedef struct cgenie_symbol_s {
    quint16 addr;
    const char* name;
}   cgenie_symbol_t;

static const cgenie_symbol_t cgenie_symbols[] = {
    {0x0000, "COLD"},
    {0x0008, "RST08"},
    {0x4000, "VRST08"},
    {0x0010, "RST10"},
    {0x4003, "VRST10"},
    {0x0013, "DCBIN"},
    {0x0018, "RST18"},
    {0x4006, "VRST18"},
    {0x001b, "DCBOUT"},
    {0x0020, "RST20"},
    {0x4009, "VRST20"},
    {0x0023, "DCBIO"},
    {0x0028, "RST28"},
    {0x400c, "VRST28"},
    {0x002b, "GETCH"},
    {0x0030, "RST30"},
    {0x400f, "VRST30"},
    {0x0033, "PUTCH"},
    {0x0038, "RST38"},
    {0x003b, "PRTCH"},
    {0x4012, "VRST30"},
    {0x0049, "INKEY"},
    {0x0050, "KBTABLE"},
    {0x0060, "SLEEP"},
    {0x0066, "NMI"},
    {0x006c, "START4"},
    {0x007b, "START6A"},
    {0x008b, "START6B"},
    {0x00e9, "MEMSIZ"},
    {0x0118, "_MEMSIZE"},
    {0x0121, "_COLOUR_BASIC"},
    {0x0140, "START5"},
    {0x014f, "SET"},
    {0x015d, "RESET"},
    {0x016b, "CHECK"},
    {0x019d, "INKEY$"},
    {0x01d9, "CASTOGL"},
    {0x01e4, "TWINKLE"},
    {0x01ed, "CASRBYTE"},
    {0x01fa, "CASRBIT"},
    {0x021c, "CASWBYTE2"},
    {0x021f, "CASWBYTE"},
    {0x023f, "CASWLEADER"},
    {0x024c, "CASRLEADER"},
    {0x02ac, "CASRSTART"},
    {0x3870, "CRTCINIT"},
    {0, NULL}
};

listing2xml::listing2xml()
{

}

bool listing2xml::parse(const QString& input, const QString& output)
{
    QFile listfile(input);
    if (!listfile.open(QIODevice::ReadOnly)) {
	qCritical("Could not open '%s' for reading:\n%s", qPrintable(input),
		  qPrintable(listfile.errorString()));
	return false;
    }

    QTextStream stream(&listfile);
    QStringList comments;
    int lno = 0;

    z80Defs* defs = new z80Defs(output);

    while (!stream.atEnd()) {
	quint32 prev_addr = 0;
	const QString line = stream.readLine();

	++lno;
	if (line.isEmpty())
	    continue;
	if (line.startsWith(QChar(';'))) {
	    comments += line.mid(1);
	    continue;
	}

	bool ok;
	quint32 addr = line.mid(2, 5).toUInt(&ok, 16);
	if (!ok) {
	    addr = prev_addr;
	}

	z80DefObj* def = new z80DefObj();
	def->set_addr0(addr);
	if (!comments.isEmpty()) {
	    // comment line(s) before an address
	    def->set_block_comment(comments);
	}

	QString comment;
	int pos = line.indexOf(QChar(';'), 48);
	if (pos >= 0) {
	    if (addr == prev_addr) {
		// line comment spanning multiple lines
		comment = QString("%1\n%2")
			  .arg(def->line_comment())
			  .arg(line.mid(pos + 1).trimmed());
	    } else {
		comment = line.mid(pos + 1);
	    }
	}
	if (!comment.isEmpty()) {
	    def->set_line_comment(comment);
	}

	z80DefObj::EntryType type = z80DefObj::CODE;
	if (line.indexOf(QStringLiteral("DEFB")) > 0) {
	    type = z80DefObj::DEFB;
	    if (line.indexOf(QChar('"')) > 0) {
		// Text string in double quotes
		type = z80DefObj::TEXT;
		if (line.indexOf(QLatin1String("128+")) > 0) {
		    // Probably a token list
		    type = z80DefObj::TOKEN;
		}
	    }
	}

	if (line.indexOf(QStringLiteral("DEFW")) > 0) {
	    type = z80DefObj::DEFW;
	}
	def->set_type(type);

	if (comments.count() > 0) {
	    QString first = comments.first();
	    if (first.endsWith(QLatin1String(" statement"))) {
		QStringList words = first.split(QChar::Space, Qt::SkipEmptyParts);
		QString symbol = words.first();
		def->set_symbol(symbol);
	    }
	}
	comments.clear();

	if (!def->has_symbol()) {
	    for (size_t i = 0; cgenie_symbols[i].name; i++) {
		if (addr == cgenie_symbols[i].addr) {
		    def->set_symbol(QString::fromLatin1(cgenie_symbols[i].name));
		    break;
		}
	    }
	}

	defs->insert(addr, def);

	prev_addr = addr;
    }

    defs->save();

    return defs;
}
