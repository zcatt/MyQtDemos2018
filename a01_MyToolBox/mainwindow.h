#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

class QSplitter;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();

public:
    QSplitter *m_pSplitter;

};

#endif // MAINWINDOW_H
