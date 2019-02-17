#include <QtWidgets>

#include "tabdialog.h"

CTabDialog::CTabDialog(QWidget *parent): QDialog(parent)
{
    m_tabWidget = new QTabWidget;
    m_tabWidget->addTab(new CRandomTab, tr("Random generator"));

    m_buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok);
    connect(m_buttonBox, &QDialogButtonBox::accepted, this, &CTabDialog::close);

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addWidget(m_tabWidget);
    mainLayout->addWidget(m_buttonBox);
    setLayout(mainLayout);

    setWindowTitle(tr("Toolsuit"));

}

CTabDialog::~CTabDialog()
{}

CRandomTab::CRandomTab(QWidget *parent) : QWidget(parent)
{
    QLabel *minLabel = new QLabel(tr("min:"));
    QLabel *maxLabel = new QLabel(tr("max:"));
    QLabel *countLabel = new QLabel(tr("count:"));
    QLabel *resultLabel = new QLabel(tr("result:"));
    result = new QTextEdit;
    minSpinBox = new QSpinBox;
    maxSpinBox = new QSpinBox;
    countSpinBox = new QSpinBox;

    minSpinBox->setMinimum(0);
    minSpinBox->setMaximum(1000);
    maxSpinBox->setMinimum(0);
    maxSpinBox->setMaximum(1000);

    countSpinBox->setMinimum(1);
    countSpinBox->setMaximum(100);

    minSpinBox->setValue(1);
    maxSpinBox->setValue(100);
    countSpinBox->setValue(10);


    result->setLineWrapMode(QTextEdit::WidgetWidth);
    result->setPlaceholderText("result");


    QPushButton *genButton = new QPushButton(tr("Generate"));

    connect(genButton, &QPushButton::pressed, this, &CRandomTab::genRandom);

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addWidget(minLabel);
    mainLayout->addWidget(minSpinBox);
    mainLayout->addWidget(maxLabel);
    mainLayout->addWidget(maxSpinBox);
    mainLayout->addWidget(countLabel);
    mainLayout->addWidget(countSpinBox);
    mainLayout->addWidget(resultLabel);
    mainLayout->addWidget(result);
    mainLayout->addWidget(genButton);
    mainLayout->addStretch(1);
    setLayout(mainLayout);
}


void CRandomTab::genRandom(void)
{
    int i,j;
    QList<int> numbersList;
    int count = countSpinBox->value();

    qsrand(QTime(0,0,0).secsTo(QTime::currentTime()));
    for(i=0;i<count;i++)
    {
        numbersList.append(minSpinBox->value() + qrand()%(maxSpinBox->value() - minSpinBox->value() +1 ));
        bool flag=true;
        while(flag)
        {
            for(j=0;j<i;j++)
            {
                if(numbersList[i]==numbersList[j])
                {
                    break;
                }
            }
            if(j<i)
            {
                numbersList[i] = minSpinBox->value() + qrand()%(maxSpinBox->value() - minSpinBox->value()+1);
            }
            if(j==i)
            {
                flag=!flag;
            }
        }
    }

    QString res;
    //res.append("Random:  ");
    for(i=0;i<count;i++)
    {
        res.append(QString().setNum(numbersList[i]) + ", ");
        qDebug()<<numbersList[i];
    }
    result->setPlainText(res);
}
