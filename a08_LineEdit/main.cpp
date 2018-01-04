#include "cwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    CWindow w;
    w.show();

    return a.exec();
}
