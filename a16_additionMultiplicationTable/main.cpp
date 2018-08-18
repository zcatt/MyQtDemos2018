#include "maindialog.h"
#include <QtWidgets>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QString locale = QLocale::system().name();

    QString path = QDir::currentPath();
    QTranslator translator;
    translator.load(QString("transsrc_") + locale);
    //translator.load(QString("transsrc_zh"));
    a.installTranslator(&translator);

    CMainDialog w;
    w.show();

    return a.exec();
}
