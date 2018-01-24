#include <QtWidgets>

#include "finddialog.h"
#include "mainwindow.h"

FindDialog::FindDialog(QWidget *parent) : QDialog(parent), m_lineEdit(0)
{
    QLabel *label = new QLabel(tr("Find &what:"));
    m_lineEdit = new QLineEdit;
    label->setBuddy(m_lineEdit);

    QPushButton *findButton = new QPushButton(tr("&Find"));
    findButton->setDefault(true);

    QHBoxLayout *topLayout = new QHBoxLayout;
    topLayout->addWidget(label);
    topLayout->addWidget(m_lineEdit);

    QHBoxLayout *bottomLayout = new QHBoxLayout;
    bottomLayout->addStretch();
    bottomLayout->addWidget(findButton);

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addLayout(topLayout);
    mainLayout->addLayout(bottomLayout);

    setLayout(mainLayout);
    setWindowTitle(tr("Find"));
}


void FindDialog::setSearchText(const QString& text)
{
    m_lineEdit->setText(text);
}


void FindDialog::closeEvent(QCloseEvent *event)
{
    MainWindow *mw = qobject_cast< MainWindow* >( parentWidget());
    if(mw)
    {
        qDebug()<<"dialog pos = "<<pos();
        mw->m_pt = pos();
    }

    QDialog::closeEvent(event);
}
