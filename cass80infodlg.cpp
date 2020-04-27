#include "cass80handler.h"
#include "cass80infodlg.h"
#include "ui_cass80infodlg.h"

cass80InfoDlg::cass80InfoDlg(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::cass80InfoDlg)
{
    ui->setupUi(this);
}

cass80InfoDlg::~cass80InfoDlg()
{
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
}
