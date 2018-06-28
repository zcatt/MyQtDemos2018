#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void createMenuAndToolBar(void);

public slots:
    void loadSettings(void);
    void saveSettings(void);
    void clearSettings(void);

};

#endif // MAINWINDOW_H
