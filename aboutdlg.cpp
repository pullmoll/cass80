#include "aboutdlg.h"
#include "ui_aboutdlg.h"

AboutDlg::AboutDlg(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AboutDlg)
{
    ui->setupUi(this);
    setWindowTitle(tr("About %1").arg(qApp->applicationName()));
}

AboutDlg::~AboutDlg()
{
    delete ui;
}
