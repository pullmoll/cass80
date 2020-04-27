#include "cass80main.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.setApplicationName(QLatin1String("Cass80"));
    a.setApplicationVersion(QLatin1String("0.2.0"));
    a.setOrganizationName(QLatin1String("pullmoll"));
    a.setOrganizationDomain(QLatin1String("mamedev.myds.me"));

    Cass80Main w;
    w.show();
    return a.exec();
}
