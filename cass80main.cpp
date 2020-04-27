/****************************************************************************
 *
 * Cass80 tool - Main window class
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
#include <QFontDatabase>
#include <QFileDialog>
#include <QTextCursor>
#include <QSettings>
#include "cass80main.h"
#include "ui_cass80main.h"
#include "cass80handler.h"
#include "cass80xml.h"
#include "cass80infodlg.h"
#include "z80defs.h"
#include "z80dasm.h"

#include "bdfdata.h"

static const QLatin1String key_window_geometry("window_geometry");
static const QLatin1String key_splitter_state("splitter_state");

static const QLatin1String font_family("ColourGenie");

Cass80Main::Cass80Main(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::Cass80Main)
    , m_cas(new Cass80Handler(this))
    , m_bdf(new bdfData(16))
{
    ui->setupUi(this);

    int id = QFontDatabase::addApplicationFont(QStringLiteral(":/cgenie1.ttf"));
    QStringList families = QFontDatabase::applicationFontFamilies(id);
    Q_ASSERT(families.contains(font_family));
    QFont font(font_family);
    ui->te_listing->setFont(font);

    setWindowTitle(tr("Cassette Manager for TRS80 and EG2000 - Version %1").arg(qApp->applicationVersion()));

    QSettings s;
    QByteArray splitter_state = s.value(key_splitter_state).toByteArray();
    QByteArray window_geometry = s.value(key_window_geometry).toByteArray();
    ui->splitter->restoreState(splitter_state);
    restoreGeometry(window_geometry);

    connect(m_cas, SIGNAL(Info(QString)), SLOT(Info(QString)));
    connect(m_cas, SIGNAL(Error(QString)), SLOT(Error(QString)));

    connect(ui->action_Load, SIGNAL(triggered()), SLOT(load()));
    connect(ui->action_Save, SIGNAL(triggered()), SLOT(save()));
    connect(ui->action_Quit, SIGNAL(triggered()), SLOT(close()));

    m_bdf->generate(QLatin1String(":/cgenie1.fnt"),
		    QLatin1String("cgenie1.bdf"));
}

Cass80Main::~Cass80Main()
{
    QSettings s;
    s.setValue(key_splitter_state, ui->splitter->saveState());
    s.setValue(key_window_geometry, saveGeometry());
    delete ui;
}

void Cass80Main::update_cursor()
{
    QTextCursor cursor = ui->tb->textCursor();
    cursor.movePosition(QTextCursor::StartOfLine, QTextCursor::MoveAnchor);
    ui->tb->setTextCursor(cursor);
}

void Cass80Main::Info(QString message)
{
    ui->tb->append(message);
    update_cursor();
}

void Cass80Main::Error(QString message)
{
    ui->tb->setTextColor(qRgb(0xe0,0x40,0x40));
    ui->tb->append(message);
    ui->tb->setTextColor(Qt::black);
    update_cursor();
}

bool Cass80Main::load()
{
    QSettings s;
    QFileDialog dlg;
    QString directory = QStringLiteral("/home/jbu/src/emulators/z80/cgenie/cas/");
    directory = s.value(QLatin1String("directory"), directory).toString();
    dlg.setFileMode(QFileDialog::ExistingFile);
    dlg.setDirectory(directory);
    dlg.setNameFilter(tr("Cassette (*.cas)"));

    if (QDialog::Accepted != dlg.exec())
	return false;

    directory = dlg.directory().canonicalPath();
    s.setValue(QLatin1String("directory"), directory);

    QStringList filenames = dlg.selectedFiles();
    QString filename = filenames.value(0);
    if (filename.isEmpty())
	return false;

    if (!m_cas->load(filename))
	return false;

    if (!m_cas->basic()) {
	QFileInfo info(filename);
	m_filepath = QDir::cleanPath(QString("%1/%2.%3")
		     .arg(info.canonicalPath())
		     .arg(info.baseName())
		     .arg(QStringLiteral("out")));

	if (m_cas->has_lmoffset()) {
	    Info(tr("Found LMOFFSET loader"));
	}
    }

    if (m_cas->basic()) {
	append_listing(m_cas->source());
    } else {
	QByteArray memory(64*1024, 0x00);
	QFile rom(QLatin1String(":/cgenie.rom"));
	if (rom.open(QIODevice::ReadOnly)) {
	    QByteArray data = rom.readAll();
	    memory.replace(0, data.size(), data);
	    rom.close();
	}
	quint16 pc_min = 0xffff;
	quint16 pc_max = 0x0000;
	for (int i = 0; i < m_cas->count(); i++) {
	    CasBlock b = m_cas->block(i);
	    if (b.type == BT_SYSTEM) {
		memory.replace(b.addr, b.size, b.data);
		pc_min = qMin<quint16>(pc_min, b.addr);
		pc_max = qMax<quint16>(pc_max, b.addr + b.size);
	    }
	}

	z80Defs z80defs(QLatin1String(":/cgenie-dasm.xml"));
	z80Dasm z80dasm(true, &z80defs, m_bdf);
	QStringList listing = z80dasm.list(memory, 0x0000, 0xfff0);
	append_listing(listing);
    }

#if DEBUG_XML
    CasXml cas_xml(this);
    cas_xml.set_data(m_cas);
    QString xml = cas_xml.toXml();
    xml = xml.toHtmlEscaped();
#endif

    cass80InfoDlg info(this);
    info.setModal(false);
    info.setup(m_cas);
    info.exec();

    return true;
}

bool Cass80Main::save()
{
    if (m_cas->basic())
	return false;

    if (!m_cas->undo_lmoffset())
	return false;

    m_cas->save(m_filepath);
    return true;

}

void Cass80Main::append_listing(const QString& text)
{
    ui->te_listing->append(text);
}

void Cass80Main::append_listing(const QStringList& list)
{
    ui->te_listing->append(list.join(QChar::LineFeed));
}
