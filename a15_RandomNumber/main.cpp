#include "tabdialog.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    CTabDialog w;
    w.show();

    return a.exec();
}
