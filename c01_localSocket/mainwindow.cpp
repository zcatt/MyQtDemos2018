#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setObjectName("mainWindow");
    setWindowTitle("c01_localSocket demo");



    m_clientInput = new QLineEdit(this);
    m_clientOutputTextEdit = new QTextEdit(this);
    m_clientOutputTextEdit->setReadOnly(true);
    m_sendClient = new QPushButton("send");
    connect(m_sendClient, &QPushButton::clicked, this, &sendClient);

    m_serverInput = new QLineEdit(this);
    m_serverOutputTextEdit = new QTextEdit(this);
    m_serverOutputTextEdit->setReadOnly(true);
    m_sendServer = new QPushButton("send");


    QWidget *center = new QWidget(this);
    QGridLayout *vLayout=new QGridLayout;
    vLayout->addWidget(new QLabel("client:"), 0, 1, 1, 2);
    vLayout->addWidget(new QLabel("input:"), 1, 0);
    vLayout->addWidget(m_clientInput, 1, 1);
    vLayout->addWidget(m_sendClient, 1, 2);
    vLayout->addWidget(new QLabel("output:"), 2, 0);
    vLayout->addWidget(m_clientOutputTextEdit, 2, 1, 1, 2);

    vLayout->addWidget(new QLabel("server:"), 3, 1, 1, 2);
    vLayout->addWidget(new QLabel("input:"), 4, 0);
    vLayout->addWidget(m_serverInput, 4, 1);
    vLayout->addWidget(m_sendServer, 4, 2);
    vLayout->addWidget(new QLabel("output:"), 5, 0);
    vLayout->addWidget(m_serverOutputTextEdit, 5, 1, 1, 2);

    center->setLayout(vLayout);
    setCentralWidget(center);

    m_server = new QLocalServer(this);
    if(!m_server->listen("localSocketDemo"))
    {
        QMessageBox::critical(this, "local server", QString("Fail to start the server: %1").arg(m_server->errorString()));
        close();
        return;
    }
    connect(m_server, &QLocalServer::newConnection, this, &newConnect);

    m_serverOutputTextEdit->append("start server ok....");

    m_client = new QLocalSocket(this);
    connect(m_client, &QLocalSocket::disconnected, m_client, &QLocalSocket::deleteLater);
    connect(m_client, static_cast<void (QLocalSocket::*)(QLocalSocket::LocalSocketError)>(&QLocalSocket::error), this, &clientDisplayError);
    connect(m_client, &QLocalSocket::disconnected, this, &clientDisconnected);
    connect(m_client, &QLocalSocket::readyRead, this, &readClient);

}

MainWindow::~MainWindow()
{}


void MainWindow::newConnect()
{
    m_serverConnect = m_server->nextPendingConnection();
    connect(m_serverConnect, &QLocalSocket::disconnected, m_serverConnect, &QLocalSocket::deleteLater);
    connect(m_serverConnect, &QLocalSocket::disconnected, this, &disconnected);
    connect(m_serverConnect, static_cast<void (QLocalSocket::*)(QLocalSocket::LocalSocketError)>(&QLocalSocket::error), this, &displayError);
    connect(m_serverConnect, &QLocalSocket::readyRead, this, &readServer);

    m_serverOutputTextEdit->append("\naccept one new connection...");

    //connection->disconnectFromServer();
}

void MainWindow::clientDisconnected()
{
    m_clientOutputTextEdit->append("\n client disconnected....");
}


void MainWindow::clientDisplayError(QLocalSocket::LocalSocketError err)
{
    QString s;
    switch (err) {
    case QLocalSocket::ServerNotFoundError:
         s ="\nErr! ServerNotFoundError";
        break;
    case QLocalSocket::ConnectionRefusedError:
        s ="\nErr! ConnectionRefusedError";
        break;
    case QLocalSocket::PeerClosedError:
        s = "\nErr! PeerClosedError";
        break;
    default:
        s =  "\nErr! error occured";
    }

    m_clientOutputTextEdit->append(s);
}

void MainWindow::disconnected()
{
    m_serverOutputTextEdit->append("\n server disconnected....");
    m_serverConnect = 0;
}


void MainWindow::displayError(QLocalSocket::LocalSocketError err)
{
    QString s;
    switch (err) {
    case QLocalSocket::ServerNotFoundError:
         s ="\nErr! ServerNotFoundError";
        break;
    case QLocalSocket::ConnectionRefusedError:
        s ="\nErr! ConnectionRefusedError";
        break;
    case QLocalSocket::PeerClosedError:
        s = "\nErr! PeerClosedError";
        break;
    default:
        s =  "\nErr! error occured";
    }

    m_serverOutputTextEdit->append(s);
}

void MainWindow::readClient()
{
    QTextStream ts(m_client);

    QString s = "\nrcv:  ";
    s += ts.readAll();
    m_clientOutputTextEdit->append(s);
    m_clientOutputTextEdit->append("\nto disconnect.");

    //ts<<"received....";
    //ts.flush();

    m_client->disconnectFromServer();
}

void MainWindow::sendClient()
{
    m_client->abort();
    m_client->connectToServer("localSocketDemo");

    m_clientOutputTextEdit->append("\naccept one new connection...");

    QTextStream ts(m_client);
    QString s = m_clientInput->text();
    ts<<s;

    s.prepend("\nsnd:  ");
    m_clientOutputTextEdit->append(s);

    //ts<<"received....";
    //ts.flush();
}


void MainWindow::readServer()
{
    Q_ASSERT(m_serverConnect);

    QTextStream ts(m_serverConnect);
    QString s = "\nrcv:  ";
    s += ts.readAll();
    m_serverOutputTextEdit->append(s);

    m_serverOutputTextEdit->append("\nsnd back: ok.");
    ts<<"ok.";
    ts.flush();
}


void MainWindow::sendServer()
{
}
