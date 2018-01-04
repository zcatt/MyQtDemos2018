/*
    使用QGraphicsView/Scene/Item的一些tips,
    .TODO, link items with decorations, endpoint(arrow, circle, diamond, solid or hollow), color, line style, etc.
    .selected border of items, and resizable
    .TODO, text in items, and in-place editing

 */
#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    return a.exec();
}
