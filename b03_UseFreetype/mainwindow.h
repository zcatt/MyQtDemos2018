#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

class CFontWidget;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();

public slots:
    void slotMenuAction(QAction *action);

public:
    CFontWidget *m_fontWidget;
};

#endif // MAINWINDOW_H
