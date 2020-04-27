#pragma once
#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class Cass80Main; }
QT_END_NAMESPACE

class Cass80Handler;
class bdfData;

class Cass80Main : public QMainWindow
{
    Q_OBJECT

public:
    Cass80Main(QWidget *parent = nullptr);
    ~Cass80Main();

private slots:
    void Info(QString message);
    void Error(QString message);
    bool load();
    bool save();

private:
    void append_listing(const QStringList& list);
    void append_listing(const QString& str);
    void update_cursor();
    void reset();
    Ui::Cass80Main *ui;
    Cass80Handler *m_cas;
    bdfData *m_bdf;
    QString m_filepath;
};
