#include <QtWidgets>

#include "chalotextwidget.h"
#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    CHaloTextWidget *pWidget = new CHaloTextWidget;
    setCentralWidget(pWidget);
}

MainWindow::~MainWindow()
{
}
