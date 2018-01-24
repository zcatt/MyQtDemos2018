#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QWidget>
#include <QPoint>

class FindDialog;

class MainWindow : public QWidget
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void showDialog(void);

public:
    QPoint m_pt;
    FindDialog *m_findDialog;

};

#endif // MAINWINDOW_H
