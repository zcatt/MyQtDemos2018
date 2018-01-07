/*
    .内嵌QLineEdit的QDialog实现in-place edit
    .TODO, QDialog实现单击区域外时自动退出.
    .QLineEdit随内容自动伸缩.




*/
#include <QApplication>
#include "mainwindow.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    return a.exec();
}
