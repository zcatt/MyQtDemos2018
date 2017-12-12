#include <QtWidgets>

#include "mainwindow.h"
#include "ctoolbox.h"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    m_pSplitter = new QSplitter;
    m_pSplitter->setOpaqueResize(false);
    //m_pSplitter->setHandleWidth(10);
    //m_pSplitter->setStyleSheet("QSplitter::handle { background-color: black; height: 1px}");
    //m_pSplitter->setStyleSheet("QSplitter::handle { background-color: black; height: 1px}");

    QToolBox *pTB = new QToolBox;
    //pTB->setBackgroundRole(QPalette::Light);



    QLabel *pLabel1;
    QLabel *pLabel2;

    QWidget *page;
    QVBoxLayout *pLayout;

    page = new QWidget;
    pLayout =new QVBoxLayout(page);
    pLabel1 = new QLabel(tr("label A1"));
    pLabel2 = new QLabel(tr("label A2"));
    pLayout->addWidget(pLabel1);
    pLayout->addWidget(pLabel2);
    pTB->addItem(page, tr("label A"));

    page = new QWidget;
    pLayout =new QVBoxLayout(page);
    pLabel1 = new QLabel(tr("label B1"));
    pLabel2 = new QLabel(tr("label B2"));
    pLayout->addWidget(pLabel1);
    pLayout->addWidget(pLabel2);


    pTB->addItem(page, tr("label B"));

    QLayout *layout = pTB->layout();
    INT spacing = layout->spacing();
    qDebug()<<"spacing value="<<spacing<<endl;
    layout->setSpacing(1);
    pTB->updateGeometry();
    pTB->update();


#if 1
    CToolBox *pCTB = new CToolBox;
    //pCTB->setBackgroundRole(QPalette::Light);

#else
    QLCDNumber  *pLCD = new QLCDNumber(3);
#endif

    page = new QWidget;
    pLayout =new QVBoxLayout(page);
    pLabel1 = new QLabel(tr("label C1"));
    pLabel2 = new QLabel(tr("label C2"));
    pLayout->addWidget(pLabel1);
    pLayout->addWidget(pLabel2);
    pCTB->addItem(page, tr("label C"));

    page = new QWidget;
    pLayout =new QVBoxLayout(page);
    pLabel1 = new QLabel(tr("label D1"));
    pLabel2 = new QLabel(tr("label D2"));
    pLayout->addWidget(pLabel1);
    pLayout->addWidget(pLabel2);


    pCTB->addItem(page, tr("label D"));



    m_pSplitter->addWidget(pTB);
    m_pSplitter->addWidget(pCTB);
    setCentralWidget(m_pSplitter);
    setWindowTitle(tr("CToolBox demo"));

}

MainWindow::~MainWindow()
{

}
