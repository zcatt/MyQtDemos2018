#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

class CTableModel;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();
    bool addEntry(QString name, QPixmap icon, QString address);

public:
    CTableModel *m_model;

};

#endif // MAINWINDOW_H
