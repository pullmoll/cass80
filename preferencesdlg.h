#ifndef PREFERENCESDLG_H
#define PREFERENCESDLG_H

#include <QDialog>

namespace Ui {
class preferencesDlg;
}

class preferencesDlg : public QDialog
{
    Q_OBJECT

public:
    explicit preferencesDlg(QWidget *parent = nullptr);
    ~preferencesDlg();

    bool use_internal_ttf() const;
    bool prefer_uppercase() const;

    void set_use_internal_ttf(bool on = false);
    void set_prefer_uppercase(bool on = false);
private:
    Ui::preferencesDlg *ui;
};

#endif // PREFERENCESDLG_H
