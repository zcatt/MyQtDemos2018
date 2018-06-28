#include <QtWidgets>
#include "mainwindow.h"
#include "finddialog.h"

MainWindow::MainWindow(QWidget *parent)
    : QWidget(parent), m_pt(-1,-1), m_findDialog(0)
{
    QPushButton *findButton1 = new QPushButton(tr("Find abc"));
    QPushButton *findButton2 = new QPushButton(tr("Find 123"));

    connect(findButton1, &QPushButton::clicked, this, &MainWindow::showDialog);
    connect(findButton2, &QPushButton::clicked, this, &MainWindow::showDialog);

    QVBoxLayout *layout = new QVBoxLayout;
    layout->addWidget(findButton1);
    layout->addWidget(findButton2);
    setLayout(layout);

    setWindowTitle(tr("Modeless dialog"));
}

MainWindow::~MainWindow()
{

}

void MainWindow::showDialog(void)
{
    QPushButton *btn = qobject_cast<QPushButton*>(sender());
    if(!btn)
        return;

    if(!m_findDialog)
    {
        m_findDialog = new FindDialog(this);
    }

    if(m_pt.x()>=0)
        m_findDialog->move(m_pt);

    if(btn->text() == tr("Find abc"))
    {
        m_findDialog->setSearchText(tr("abc"));
    }
    else
    {
        m_findDialog->setSearchText(tr("123"));
    }


    //if(m_pt.x()>= 0)
    m_findDialog->show();
}
