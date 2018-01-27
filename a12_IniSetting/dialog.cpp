#include <QtWidgets>
#include "dialog.h"

Dialog::Dialog(QWidget *parent)
    : QDialog(parent)
{
    QVBoxLayout *layout = new QVBoxLayout(this);

    QFormLayout *formLayout = new QFormLayout;

    nameEdit = new QLineEdit;
    addrEdit = new QLineEdit;
    fnLabel = new QLabel;
    resultEdit = new QTextEdit;
    resultEdit->setReadOnly(true);
    resultEdit->setMinimumSize(200, 100);

    formLayout->addRow("Name:", nameEdit);
    formLayout->addRow("Addr:", addrEdit);
    formLayout->addRow("iniFileName:", fnLabel);
    formLayout->addRow("Result:", resultEdit);

    layout->addLayout(formLayout);

    QDialogButtonBox *btnBox = new QDialogButtonBox(QDialogButtonBox::Save|QDialogButtonBox::Ok);
    connect(btnBox, &QDialogButtonBox::clicked, this, &Dialog::Do);
    //connect(btnBox, &QDialogButtonBox::accepted, this, &QDialog::accept);

    layout->addWidget(btnBox);
    setLayout(layout);
}

Dialog::~Dialog()
{

}


const static char *sName = "name";
const static char *sAddr = "addr";
const static char *sGroup = "group";
const static char *sSubGroup = "subGroup";

void Dialog::Do(QAbstractButton *btn)
{
    QDialogButtonBox *btnBox = qobject_cast<QDialogButtonBox*>(sender());

    if(btnBox->standardButton(btn) == QDialogButtonBox::Save)
    {
        QSettings settings(QSettings::IniFormat, QSettings::UserScope, QLatin1String("Org"), QLatin1String("App"));
        settings.setIniCodec("UTF-8");
        settings.setValue(sName, nameEdit->text());
        settings.setValue(sAddr, addrEdit->text());

        settings.beginGroup(sGroup);
        settings.setValue(sName, nameEdit->text());
        settings.setValue(sAddr, addrEdit->text());

        settings.beginGroup(sSubGroup);
        settings.setValue(sName, nameEdit->text());
        settings.setValue(sAddr, addrEdit->text());
        settings.endGroup();

        settings.endGroup();


        settings.sync();

        qDebug()<<"ini filename: "<<settings.fileName();
        fnLabel->setText(settings.fileName());

        QFile file(settings.fileName());

        resultEdit->clear();
        if(file.open(QIODevice::ReadOnly | QIODevice::Text))
        {
            QString text= QString::fromUtf8(file.readAll());
            resultEdit->setText(text);
        }
    }
    else
        accept();
}
