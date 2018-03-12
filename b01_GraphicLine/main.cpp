/*
TODO,
    .line item
        .insert new point
        .delete point
        .edit name and desc in property editor
    .text item
        .in-place editing
        .edit name and desc in property editor
    .rect item
        .property editor
    .tool box
    .tree view on all items
    .undo-redo history
    .ini setting
    .output xml file
    .clipboard



DONE,
    .property editor
    .line item
        .new line
        .drag point
        .delete point
        .name and desc item随同移动
        .property editor
    .text item
        .property editor
    .rect item
        .property editor



*/
#include <QApplication>

#include "mainwindow.h"

int main(int argc, char *argv[])
{
    qSetMessagePattern("%{line} %{function}:   %{message}");

    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    return a.exec();
}
