/****************************************************************************
 *
 * Cass80 tool - cassette image information dialog
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
#include <QSettings>
#include "cass80handler.h"
#include "cass80infodlg.h"
#include "ui_cass80infodlg.h"

static const QLatin1String key_geometry("geometry");

cass80InfoDlg::cass80InfoDlg(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::cass80InfoDlg)
{
    ui->setupUi(this);
    QSettings s;
    s.beginGroup(objectName());
    restoreGeometry(s.value(key_geometry).toByteArray());
    s.endGroup();
}

cass80InfoDlg::~cass80InfoDlg()
{
    QSettings s;
    s.beginGroup(objectName());
    s.setValue(key_geometry, saveGeometry());
    s.endGroup();
    delete ui;
}

void cass80InfoDlg::setup(Cass80Handler* cas)
{
    switch (cas->machine()) {
    case MACH_EG2000:
	ui->le_machine->setText(tr("EG2000"));
	break;
    case MACH_TRS80:
	ui->le_machine->setText(tr("TRS80"));
	break;
    default:
	ui->le_machine->setText(QString());
	break;
    }
    ui->le_hdr_name->setText(cas->hdr_name());
    ui->le_hdr_author->setText(cas->hdr_author());
    ui->le_hdr_copyright->setText(cas->hdr_copyright());
    ui->tb_hdr_description->setText(cas->hdr_description());

    ui->le_filename->setText(cas->filename());
    ui->le_blen->setText(QString::number(cas->blen()));
    ui->le_nblocks->setText(QString::number(cas->count()));
    ui->le_format->setText(cas->basic() ? QLatin1String("BASIC") : QLatin1String("SYSTEM"));
    ui->le_digest->setText(QString::fromLatin1(cas->digest().toHex()));
}
