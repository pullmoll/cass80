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
#include <QSpacerItem>
#include <QTextCursor>
#include <QSettings>
#include "cass80main.h"
#include "ui_cass80main.h"
#include "cass80handler.h"
#include "cass80xml.h"

#include "aboutdlg.h"
#include "casinfodlg.h"
#include "preferencesdlg.h"
#include "z80defs.h"
#include "z80dasm.h"

#include "bdfcgenie.h"
#include "bdftrs80.h"
#include "def2xml.h"

#define	GENERATE_BDF	    1
#define	GENERATE_DEFS	    1
#define	USE_GENERATED_DEFS  1

static const QLatin1String g_mysrcdir("/home/jbu/src/emulators/cass80");
static const QLatin1String g_resources(":/resources");

static const QLatin1String key_window_geometry("window_geometry");
static const QLatin1String key_splitter_state("splitter_state");
static const QLatin1String grp_preferences("preferences");
static const QLatin1String key_fontsize("fontsize");
static const QLatin1String key_internal_ttf("internal_ttf");
static const QLatin1String key_uppercase("uppercase");

static const QLatin1String font_family("ColourGenie");

Cass80Main::Cass80Main(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::Cass80Main)
    , m_cas(new Cass80Handler(this))
    , m_bdf1(new bdfCgenie(16))
    , m_bdf2(new bdfTrs80(12))
    , m_fontsize(1.0)
    , m_internal_ttf(false)
    , m_uppercase(true)
{
    ui->setupUi(this);

    setup_actions();
    setup_toolbar();
    setup_preferences();

    setWindowTitle(tr("Cassette Manager for TRS80 and EG2000 - Version %1")
		   .arg(qApp->applicationVersion()));

    QSettings s;
    QByteArray splitter_state = s.value(key_splitter_state).toByteArray();
    QByteArray window_geometry = s.value(key_window_geometry).toByteArray();
    ui->splitter->restoreState(splitter_state);
    restoreGeometry(window_geometry);

    connect(m_cas, SIGNAL(Info(QString)), SLOT(Info(QString)));
    connect(m_cas, SIGNAL(Error(QString)), SLOT(Error(QString)));

#if GENERATE_BDF
    m_bdf1->generate(QLatin1String(":/resources/cgenie1.fnt"),
		    QLatin1String("cgenie1.bdf"));
    m_bdf2->generate(QLatin1String(":/resources/trs80.fnt"),
		    QLatin1String("trs80.bdf"));
#endif

#if GENERATE_DEFS
    def2xml l2x;
    l2x.parse(QStringLiteral(":/resources/cgenie.def"),
	      QString("%1/%2").arg(g_mysrcdir).arg("cgenie-new.xml"));
#endif
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
    cursor.movePosition(QTextCursor::End, QTextCursor::MoveAnchor);
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

bool Cass80Main::about()
{
    AboutDlg dlg(this);
    return QDialog::Accepted == dlg.exec();
}

bool Cass80Main::load()
{
    QSettings s;
    QFileDialog dlg;
    QString directory = s.value(QLatin1String("directory")).toString();
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
	set_listing(m_cas->source());
    } else {
	QByteArray memory(64*1024, 0x00);
	QFile rom(QLatin1String(":/resources/cgenie.rom"));
	if (rom.open(QIODevice::ReadOnly)) {
	    QByteArray data = rom.readAll();
	    memory.replace(0, data.size(), data);
	    rom.close();
	}
	quint16 pc_min = 0xffff;
	quint16 pc_max = 0x0000;
	for (int i = 0; i < m_cas->count(); i++) {
	    Cass80Block b = m_cas->block(i);
	    if (b.type == BT_SYSTEM) {
		memory.replace(b.addr, b.size, b.data);
		pc_min = qMin<quint16>(pc_min, b.addr);
		pc_max = qMax<quint16>(pc_max, b.addr + b.size);
	    }
	}

#if USE_GENERATED_DEFS
	z80Defs z80defs(QString("%1/%2")
			.arg(g_mysrcdir)
			.arg("cgenie-new.xml"));
#else
	z80Defs z80defs(QString("%1/%2")
			.arg(resources)
			.arg("cgenie-dasm.xml"));
#endif
	z80Dasm z80dasm(m_uppercase, &z80defs, m_bdf1);

	pc_min = 0; // Always disassemble ROM as well

	QStringList listing = z80dasm.listing(memory, pc_min, pc_max);
	set_listing(listing);
    }

#if DEBUG_XML
    CasXml cas_xml(this);
    cas_xml.set_data(m_cas);
    QString xml = cas_xml.toXml();
    xml = xml.toHtmlEscaped();
#endif

    update_actions();

    return true;
}

bool Cass80Main::save()
{
    if (m_cas->basic())
	return false;

    m_cas->save(m_filepath);
    return true;

}

void Cass80Main::information()
{
    cass80InfoDlg info(this);
    info.setModal(false);
    info.setup(m_cas);
    info.exec();
}

void Cass80Main::undo_lmoffset()
{
    if (!m_cas->undo_lmoffset())
	return;

    Info(tr("LMOFFSET loader removed."));
    return;
}

void Cass80Main::preferences()
{
    preferencesDlg dlg(this);
    dlg.set_prefer_uppercase(m_uppercase);
    dlg.set_use_internal_ttf(m_internal_ttf);
    if (QDialog::Accepted != dlg.exec())
	return;
    m_uppercase = dlg.prefer_uppercase();
    m_internal_ttf = dlg.use_internal_ttf();

    QSettings s;
    s.beginGroup(grp_preferences);
    s.setValue(key_fontsize, m_fontsize);
    s.setValue(key_uppercase, m_uppercase);
    s.setValue(key_internal_ttf, m_internal_ttf);
    s.endGroup();

    setup_preferences();
}

void Cass80Main::increase_font_size()
{
    QFont font = ui->te_listing->font();
    font.setPointSizeF(font.pointSizeF() * 1.05);
    ui->te_listing->setFont(font);
    font = ui->tb->font();
    font.setPointSizeF(font.pointSizeF() * 1.05);
    ui->tb->setFont(font);
}

void Cass80Main::decrease_font_size()
{
    QFont font = ui->te_listing->font();
    font.setPointSizeF(font.pointSizeF() * 0.95);
    ui->te_listing->setFont(font);
    font = ui->tb->font();
    font.setPointSizeF(font.pointSizeF() * 0.95);
    ui->tb->setFont(font);
}

void Cass80Main::setup_actions()
{
    connect(ui->action_About, SIGNAL(triggered()), SLOT(about()));
    connect(ui->action_AboutQt, SIGNAL(triggered()), qApp, SLOT(aboutQt()));
    connect(ui->action_Load, SIGNAL(triggered()), SLOT(load()));
    connect(ui->action_Save, SIGNAL(triggered()), SLOT(save()));
    connect(ui->action_Information, SIGNAL(triggered()), SLOT(information()));
    connect(ui->action_Quit, SIGNAL(triggered()), SLOT(close()));
    connect(ui->action_Undo_lmoffset, SIGNAL(triggered()), SLOT(undo_lmoffset()));
    connect(ui->action_Preferences, SIGNAL(triggered()), SLOT(preferences()));

    connect(ui->action_Increase_font_size, SIGNAL(triggered()), SLOT(increase_font_size()));
    connect(ui->action_Decrease_font_size, SIGNAL(triggered()), SLOT(decrease_font_size()));
}

void Cass80Main::setup_toolbar()
{
    ui->toolBar->addAction(ui->action_Load);
    ui->toolBar->addAction(ui->action_Save);
    ui->toolBar->addSeparator();
    ui->toolBar->addAction(ui->action_Information);
    ui->toolBar->addSeparator();
    ui->toolBar->addAction(ui->action_Quit);

    update_actions();
}

void Cass80Main::setup_preferences()
{
    QSettings s;
    s.beginGroup(grp_preferences);
    m_fontsize = s.value(key_fontsize, 1.0).toReal();
    m_internal_ttf = s.value(key_internal_ttf, false).toBool();

    if (m_internal_ttf) {
	QString ttf = QString("%1/%2")
		      .arg(g_resources)
		      .arg("/cgenie1.ttf");
	int id = QFontDatabase::addApplicationFont(ttf);
	QStringList families = QFontDatabase::applicationFontFamilies(id);
	Q_ASSERT(families.contains(font_family));
	QFont font(font_family);
	font.setPointSizeF(font.pointSizeF() * m_fontsize);
	ui->te_listing->setFont(font);
    } else {
	QFont font(QLatin1String("Source Code Pro"), -10, QFont::Normal, false);
	font.setPointSizeF(font.pointSizeF() * m_fontsize);
	ui->te_listing->setFont(font);
    }
    m_uppercase = s.value(key_uppercase).toBool();
}

void Cass80Main::update_actions()
{
    ui->action_Information->setEnabled(!m_cas->isEmpty());
    ui->action_Undo_lmoffset->setEnabled(m_cas->has_lmoffset());
}

void Cass80Main::set_listing(const QString& text)
{
    ui->te_listing->setText(text);
}

void Cass80Main::set_listing(const QStringList& list)
{
    ui->te_listing->setText(list.join(QChar::LineFeed));
}
