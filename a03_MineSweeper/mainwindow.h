#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>


class CMineWidget;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();

    //menu actions
    void createMenus(void);
    void newGame();
    void primary();
    void intermediate();
    void advance();
    void custom();
    void mark(bool checked);
    void sound(bool checked);
    void championList();
    void about();

public:
    CMineWidget *m_pMineWidget;
};

#endif // MAINWINDOW_H
