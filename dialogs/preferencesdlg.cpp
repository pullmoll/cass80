#include "preferencesdlg.h"
#include "ui_preferencesdlg.h"

preferencesDlg::preferencesDlg(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::preferencesDlg)
{
    ui->setupUi(this);
}

preferencesDlg::~preferencesDlg()
{
    delete ui;
}

bool preferencesDlg::use_internal_ttf() const
{
    return ui->cb_internal_ttf->isChecked();
}

bool preferencesDlg::prefer_uppercase() const
{
    return ui->cb_uppercase->isChecked();
}

void preferencesDlg::set_use_internal_ttf(bool on)
{
    ui->cb_internal_ttf->setChecked(on);
}

void preferencesDlg::set_prefer_uppercase(bool on)
{
    ui->cb_uppercase->setChecked(on);
}
