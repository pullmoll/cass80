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
#include "def2xml.h"

typedef struct cgenie_symbol_s {
    quint16 addr;
    const char* name;
}   cgenie_symbol_t;

static const QHash<quint32,const char*> cgenie_symbols({
    {0x0000, "COLD"},
    {0x0008, "RST08"},
    {0x0010, "RST10"},
    {0x0013, "DCBIN"},
    {0x0018, "RST18"},
    {0x001b, "DCBOUT"},
    {0x0020, "RST20"},
    {0x0023, "DCBIO"},
    {0x0028, "RST28"},
    {0x002b, "GETCH"},
    {0x0030, "RST30"},
    {0x0033, "PUTCH"},
    {0x0038, "RST38"},
    {0x003b, "PRTCH"},
    {0x0046, "DCBIO"},
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
    {0x01c9, "CLS"},
    {0x01d3, "RANDOM"},
    {0x01d9, "CASTOGL"},
    {0x01e4, "TWINKLE"},
    {0x01ed, "CASRBYTE"},
    {0x01fa, "CASRBIT"},
    {0x021c, "CASWBYTE2"},
    {0x021f, "CASWBYTE"},
    {0x023f, "CASWLEADER"},
    {0x024c, "CASRLEADER"},
    {0x02ac, "CASRSTART"},
    {0x02b2, "SYSTEM"},
    {0x0809, "LOG"},
    {0x0977, "ABS"},
    {0x098a, "SNG"},
    {0x0a7f, "CINT"},
    {0x0ab1, "CSNG"},
    {0x0adb, "CDBL"},
    {0x0b37, "INT"},
    {0x13e7, "SQR"},
    {0x1439, "EXP"},
    {0x14c9, "RND"},
    {0x1541, "COS"},
    {0x1547, "SIN"},
    {0x15a8, "TAN"},
    {0x15bd, "ATN"},
    {0x1b49, "NEW"},
    {0x1ca1, "FOR"},
    {0x1d91, "RESTORE"},
    {0x1da9, "STOP"},
    {0x1dae, "END"},
    {0x1de4, "CONT"},
    {0x1df7, "TRON"},
    {0x1df8, "TROFF"},
    {0x1e00, "DEFSTR"},
    {0x1e03, "DEFINT"},
    {0x1e06, "DEFSNG"},
    {0x1e09, "DEFDBL"},
    {0x1e7a, "CLEAR"},
    {0x1ea3, "RUN"},
    {0x1eb1, "GOSUB"},
    {0x1ec2, "GOTO"},
    {0x1ede, "RETURN"},
    {0x1f05, "DATA"},
    {0x1f07, "ELSE"},
    {0x1f07, "REM"},
    {0x1f21, "LET"},
    {0x1f6c, "ON"},
    {0x1faf, "RESUME"},
    {0x1ff4, "ERROR"},
    {0x2008, "AUTO"},
    {0x2039, "IF"},
    {0x2067, "LPRINT"},
    {0x206f, "PRINT"},
    {0x219a, "INPUT"},
    {0x21ef, "READ"},
    {0x22b6, "NEXT"},
    {0x2608, "DIM"},
    {0x27d4, "FRE"},
    {0x27f5, "POS"},
    {0x2836, "STR$"},
    {0x2a0f, "ASC"},
    {0x2a1f, "CHR$"},
    {0x2a61, "LEFT$"},
    {0x2a91, "RIGHT$"},
    {0x2a9a, "MID$"},
    {0x2ac5, "VAL"},
    {0x2aef, "INP"},
    {0x2afb, "OUT"},
    {0x2b29, "LLIST"},
    {0x2b2e, "LIST"},
    {0x2bc6, "DELETE"},
    {0x2bf5, "CSAVE"},
    {0x2c1f, "CLOAD"},
    {0x2caa, "PEEK"},
    {0x2cb1, "POKE"},
    {0x2e60, "EDIT"},
    {0x3870, "CRTCINIT"},
    {0x4000, "VRST08"},
    {0x4003, "VRST10"},
    {0x4006, "VRST18"},
    {0x4009, "VRST20"},
    {0x400c, "VRST28"},
    {0x400f, "VRST30"},
    {0x4012, "VRST38"},
    {0x4015, "KBDDCB"},
    {0x40A7, "LINEBP"},
    {0x4152, "CVI"},
    {0x4158, "CVS"},
    {0x415b, "DEF"},
    {0x415e, "CVD"},
    {0x4161, "EOF"},
    {0x4164, "LOC"},
    {0x4167, "LOF"},
    {0x416a, "MKI$"},
    {0x416d, "MKS$"},
    {0x4170, "MKD$"},
    {0x4173, "CMD"},
    {0x4179, "OPEN"},
    {0x417c, "FIELD"},
    {0x417f, "GET"},
    {0x4182, "PUT"},
    {0x4185, "CLOSE"},
    {0x4188, "LOAD"},
    {0x418b, "MERGE"},
    {0x418e, "NAME"},
    {0x4191, "KILL"},
    {0x4197, "LSET"},
    {0x419a, "RSET"},
    {0x41a0, "SAVE"},
    {0x41a3, "LINE"},
    {0x4400, "LGR"},
    {0x4800, "FGR"}
});

static const QHash<QString, z80DefObj::EntryType> type_codes({
    {QLatin1String("CODE"), z80DefObj::CODE},
    {QLatin1String("ENTRY"), z80DefObj::CODE},
    {QLatin1String("ASCII"), z80DefObj::TEXT},
    {QLatin1String("BYTE"), z80DefObj::DEFB},
    {QLatin1String("BYTES"), z80DefObj::DEFB},
    {QLatin1String("WORD"), z80DefObj::DEFW},
    {QLatin1String("WORDS"), z80DefObj::DEFW},
    {QLatin1String("SPACE"), z80DefObj::DEFS},
    {QLatin1String("NONE"), z80DefObj::INVALID},
});

def2xml::def2xml()
{
}

bool def2xml::parse(const QString& input, const QString& output)
{
    QFile deffile(input);
    if (!deffile.open(QIODevice::ReadOnly)) {
	qCritical("Could not open '%s' for reading:\n%s", qPrintable(input),
		  qPrintable(deffile.errorString()));
	return false;
    }

    QTextStream stream(&deffile);
    QStringList blockcmt;
    QStringList linecmt;
    QString symbol;
    quint32 addr_old = ~0u;
    int lno = 0;

    z80Defs* defs = new z80Defs(output);
    z80DefObj::EntryType type = z80DefObj::CODE;

    while (!stream.atEnd()) {
	const QString line = stream.readLine().trimmed();

	++lno;

	if (line.isEmpty())
	    continue;

	if (line.startsWith(QChar('#')))
	    continue;

	QStringList fields = line.split(QChar::Tabulation);
	QString addr_str = fields.count() > 0 ? fields.at(0) : QString();
	QString type_str = fields.count() > 1 ? fields.at(1) : QString();
	QString cmnt_str = fields.count() > 2 ? fields.at(2) : QString();

	if (addr_str == QLatin1String("*")) {
	    // block comment continued
	    blockcmt.append(cmnt_str.mid(1).trimmed());
	    continue;
	}

	bool ok;
	quint32 addr = addr_str.toUInt(&ok, 16);
	Q_ASSERT(ok);

	if (addr_old != ~0u) {
	    add_def(defs, addr_old, type, symbol, blockcmt, linecmt);
	    blockcmt.clear();
	    linecmt.clear();
	    symbol.clear();
	}

	addr_old = addr;
	symbol = cgenie_symbols.value(addr);
	z80DefObj::EntryType type_new = type_codes.value(type_str.toUpper(), z80DefObj::INVALID);
	if (z80DefObj::INVALID != type_new) {
	    type = type_new;
	} else if (!type_codes.contains(type_str.toUpper())) {
	    qDebug("%s: line #%d invalid entry type (%s)", __func__,
		   lno, qPrintable(type_str));
	    return false;
	}

	if (cmnt_str.startsWith(QChar(';'))) {
	    blockcmt.append(cmnt_str.mid(1).trimmed());
	} else if (!cmnt_str.isEmpty()) {
	    linecmt.append(cmnt_str);
	}
    }

    defs->save(output);
    delete defs;

    return true;
}

void def2xml::add_def(z80Defs* defs, quint32 pc, z80DefObj::EntryType type, const QString& symbol, const QStringList& blockcmt, const QStringList& linecmt)
{
    if (symbol.isEmpty() && blockcmt.isEmpty() && linecmt.isEmpty())
	return;

    z80Def old = defs->entry(pc);
    if (old.isNull()) {
	z80DefObj *def = new z80DefObj();
	def->set_symbol(symbol);
	def->set_block_comments(blockcmt);
	def->set_line_comments(linecmt);
	def->set_orig(pc);
	def->set_type(type);
	def->set_arg0();
	def->set_param(0);
	def->set_maxelem(0);
	defs->insert(pc, def);
    } else {
	old->set_type(type);
	if (!symbol.isEmpty())
	    old->set_symbol(symbol);
	if (!blockcmt.isEmpty())
	    old->set_block_comments(blockcmt);
	if (!linecmt.isEmpty())
	    old->set_line_comments(linecmt);
    }
}
