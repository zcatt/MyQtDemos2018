#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = nullptr);



public slots:
    void open();
    void saveAs();

public:
    QMenu *fileMenu;
    QAction *openAct;
    QAction *saveAsAct;
    QAction *exitAct;
};

#endif // MAINWINDOW_H
