#ifndef CASS80INFODLG_H
#define CASS80INFODLG_H

#include <QDialog>

namespace Ui {
class cass80InfoDlg;
}
class Cass80Handler;

class cass80InfoDlg : public QDialog
{
    Q_OBJECT
public:
    explicit cass80InfoDlg(QWidget *parent = nullptr);
    ~cass80InfoDlg();
    void setup(Cass80Handler* cas);

private:
    Ui::cass80InfoDlg *ui;
};

#endif // CASS80INFODLG_H
