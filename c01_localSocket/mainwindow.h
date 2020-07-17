#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtWidgets>
#include <QLocalServer>
#include <QLocalSocket>

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();

public slots:
    void newConnect();
    void clientDisconnected();
    void clientDisplayError(QLocalSocket::LocalSocketError err);
    void disconnected();
    void displayError(QLocalSocket::LocalSocketError err);
    void readClient();
    void sendClient();
    void readServer();
    void sendServer();

public:
    QLineEdit *m_clientInput;
    QTextEdit *m_clientOutputTextEdit;
    QPushButton *m_sendClient;

    QLineEdit *m_serverInput;
    QTextEdit *m_serverOutputTextEdit;
    QPushButton *m_sendServer;

    QLocalServer *m_server;
    QLocalSocket *m_serverConnect;
    QLocalSocket *m_client;
};

#endif // MAINWINDOW_H
