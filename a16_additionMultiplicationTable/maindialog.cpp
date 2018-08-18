#include <QtWidgets>

#include "maindialog.h"

CMainDialog *mainwin;


const char* AddtblUnordered = "addtbl_unordered";
const char* AddtblCount = "addtbl_count";
const char* AddtblAddend1 = "addtbl_add1_";
const char* AddtblAddend2 = "addtbl_add2_";

const char* MultblUnordered = "multbl_unordered";
const char* MultblCount = "multbl_count";
const char* MultblAddend1 = "multbl_add1_";
const char* MultblAddend2 = "multbl_add2_";

const char* AddCount = "add_count";
const char* AddMin1 = "add_min1";
const char* AddMax1 = "add_max1";
const char* AddMin2 = "add_min2";
const char* AddMax2 = "add_max2";

const char* SubCount = "sub_count";
const char* SubMin1 = "sub_min1";
const char* SubMax1 = "sub_max1";
const char* SubMin2 = "sub_min2";
const char* SubMax2 = "sub_max2";

const char* MulCount = "mul_count";
const char* MulMin1 = "mul_min1";
const char* MulMax1 = "mul_max1";
const char* MulMin2 = "mul_min2";
const char* MulMax2 = "mul_max2";

const char* DivCount = "div_count";
const char* DivMin1 = "div_min1";
const char* DivMax1 = "div_max1";
const char* DivMin2 = "div_min2";
const char* DivMax2 = "div_max2";


CMainDialog::CMainDialog(QWidget *parent)
    : QDialog(parent)
{
    mainwin = this;

    qsrand(QTime(0,0,0).secsTo(QTime::currentTime()));

    m_settings = new QSettings(QSettings::IniFormat, QSettings::UserScope, QLatin1String("Org"), QLatin1String("App"), this);
    m_settings->setIniCodec("UTF-8");

    makeTabWidget();

    m_nextShortcut = new QShortcut(QString("N"), this);
    connect(m_nextShortcut, SIGNAL(activated()), this, SLOT(slotNextAction()));

    //m_buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);
    m_buttonBox = new QDialogButtonBox();
    m_buttonBox->addButton(tr("Finish"), QDialogButtonBox::AcceptRole);
    connect(m_buttonBox, &QDialogButtonBox::accepted, this, &QDialog::accept);
    connect(m_buttonBox, &QDialogButtonBox::rejected, this, &QDialog::reject);

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addWidget(m_tabWidget);
    mainLayout->addWidget(m_buttonBox);
    setLayout(mainLayout);


    setWindowTitle(tr("Arithmetic exercise"));

    const QRect availableGeometry = QApplication::desktop()->availableGeometry(this);
    //resize(availableGeometry.width() / 4, availableGeometry.height() / 3);
    resize(860, 600);
    move((availableGeometry.width() - width()) / 2,
         (availableGeometry.height() - height()) / 2);

}

CMainDialog::~CMainDialog()
{}

void CMainDialog::slotNextAction(void)
{
    qDebug()<<"";
    int cur = m_tabWidget->currentIndex();
    switch(cur)
    {
    case TabIndex_AdditionTable:
        m_addTableWidget->slotStartNext();
        break;
    case TabIndex_MultiplicationTable:
        m_mulTableWidget->slotStartNext();
        break;
    case TabIndex_Addition:
        m_addWidget->slotStartNext();
        break;
    case TabIndex_Subtraction:
        m_subWidget->slotStartNext();
        break;
    case TabIndex_Multiplication:
        m_mulWidget->slotStartNext();
        break;
    case TabIndex_Division:
        m_divWidget->slotStartNext();
        break;
    case TabIndex_Config:
    default:
        Q_ASSERT(false);
        break;
    }


}

void CMainDialog::makeTabWidget(void)
{
    m_tabWidget = new QTabWidget;

    m_addTableWidget = new CAdditionTableWidget;
    m_tabWidget->addTab(m_addTableWidget, tr("Addition table"));

    m_mulTableWidget = new CMultiplicationTableWidget;
    m_tabWidget->addTab(m_mulTableWidget, tr("Multiplication table"));

    m_addWidget = new CAdditionWidget;
    m_tabWidget->addTab(m_addWidget, tr("Addition"));

    m_subWidget = new CSubtractionWidget;
    m_tabWidget->addTab(m_subWidget, tr("Subtraction"));

    m_mulWidget = new CMultiplicationWidget;
    m_tabWidget->addTab(m_mulWidget, tr("Multiplication"));

    m_divWidget = new CDivisionWidget;
    m_tabWidget->addTab(m_divWidget, tr("Division"));

    m_cfgWidget = new CConfigWidget;
    //QScrollArea *sa = new QScrollArea;
    //sa->setWidget(m_cfgWidget);
    m_tabWidget->addTab(m_cfgWidget, tr("Configue"));
}


bool CExItem::equal(CExItem &ei)
{
    return ((ei.add1 == add1 && ei.add2 == add2)
                ||(ei.add1 == add2 && ei.add2 == add1)
                );
}

QString CExItem::repr(bool bUnordered, bool bResult)
{
    int a = add1;
    int b = add2;
    if(!bUnordered && a>b)
    {
        a = add2;
        b = add1;
    }

    QString r;
    QChar oper(' ');
    int c = 0;
    switch(op)
    {
    case Op_Add:
        oper = QChar('+');
        c = a+b;
        break;
    case Op_Sub:
        oper = QChar('-');
        c = a-b;
        break;
    case Op_Multiple:
        oper = QChar(0x0d7);
        c = a*b;
        break;
    case Op_Division:
        oper = QChar(0x0f7);
        c = a/b;
        break;
    default:
        Q_ASSERT(false);
        break;
    }

    if(bResult)
    {
        r = QString("%1 %4 %2 = %3").arg(a).arg(b).arg(c).arg(oper);
    }
    else
        r = QString("%1 %3 %2").arg(a).arg(b).arg(oper);

    return r;
}


CAdditionTableWidget::CAdditionTableWidget(QWidget *parent): QWidget(parent)
                                                            , m_cur(0)
{
    readSettings();



    //setBackgroundRole(QPalette::Button);
    //setAutoFillBackground(true);

    QGridLayout *grid = new QGridLayout;

    QFont ft("Consolas");


    m_preLabel = new QLabel("");
    m_preLabel->setFrameStyle(QFrame::Box | QFrame::Sunken);
    m_preLabel->setAlignment(Qt::AlignCenter);
    m_preLabel->setBackgroundRole(QPalette::Button);
    m_preLabel->setAutoFillBackground(true);

    ft.setPixelSize(40);
    m_preLabel->setFont(ft);
    grid->addWidget(m_preLabel, 0, 0, 1,3);

    m_progressingLabel = new QLabel;
    m_progressingLabel->setFrameStyle(QFrame::Box | QFrame::Sunken);
    m_progressingLabel->setAlignment(Qt::AlignCenter);
    //m_progressingLabel->setBackgroundRole(QPalette::Button);
    //m_progressingLabel->setAutoFillBackground(true);
    ft.setPixelSize(30);
    m_progressingLabel->setFont(ft);
    grid->addWidget(m_progressingLabel, 0, 4,1,1);

    m_stopWatch = new CStopWatch;
    grid->addWidget(m_stopWatch, 0, 5,1, 1);

    m_curLabel = new QLabel("");
    m_curLabel->setLineWidth(3);
    m_curLabel->setFrameStyle(QFrame::Box|QFrame::Raised);
    m_curLabel->setAlignment(Qt::AlignCenter);
    m_curLabel->setBackgroundRole(QPalette::Button);
    m_curLabel->setAutoFillBackground(true);
    ft.setPixelSize(100);
    m_curLabel->setFont(ft);
    grid->addWidget(m_curLabel, 1, 0, 2,6);

    m_nextLabel = new QLabel("");
    m_nextLabel->setFrameStyle(QFrame::Box | QFrame::Sunken);
    m_nextLabel->setAlignment(Qt::AlignCenter);
    m_nextLabel->setBackgroundRole(QPalette::Button);
    m_nextLabel->setAutoFillBackground(true);
    ft.setPixelSize(30);
    m_nextLabel->setFont(ft);
    grid->addWidget(m_nextLabel, 3, 0, 1,3);

    m_buttonBox = new QDialogButtonBox();
    m_buttonBox->addButton(tr("Start/Next(N)"), QDialogButtonBox::AcceptRole);
    m_buttonBox->addButton(tr("Stop"), QDialogButtonBox::RejectRole);
    connect(m_buttonBox, &QDialogButtonBox::accepted, this, &CAdditionTableWidget::slotStartNext);
    connect(m_buttonBox, &QDialogButtonBox::rejected, this, &CAdditionTableWidget::slotStop);
    grid->addWidget(m_buttonBox, 4, 2, 1, 2);
    setLayout(grid);

    generateExercise();
}

CAdditionTableWidget::~CAdditionTableWidget()
{}

void CAdditionTableWidget::readSettings(void)
{
    m_bUnordered = VALUE(AddtblUnordered, false).toBool();
    m_exCount = VALUE(AddtblCount, 20).toInt();

    //init data
    for(int i = 1; i< 10; i++)
    {
        m_addend1[i] = VALUE(QString("%1%2").arg(AddtblAddend1).arg(i), true).toBool();
        m_addend2[i] = VALUE(QString("%1%2").arg(AddtblAddend2).arg(i), true).toBool();
    }
    m_addend1[0] = false;
    m_addend2[0] = false;
}

void CAdditionTableWidget::writeSettings(void)
{
    SETVALUE(AddtblUnordered, m_bUnordered);
    SETVALUE(AddtblCount, m_exCount);

    for(int i = 0; i< 9; i++)
    {
        SETVALUE(QString("%1%2").arg(AddtblAddend1).arg(i+1), m_addend1[i+1]);
        SETVALUE(QString("%1%2").arg(AddtblAddend2).arg(i+1), m_addend2[i+1]);
    }
}

bool CAdditionTableWidget::generateExercise(void)
{
    int countAdd1= 0;
    int countAdd2= 0;

    int i;
    for(i = 0; i < 10; i++)
    {
        if(m_addend1[i])
            countAdd1++;
        if(m_addend2[i])
            countAdd2++;
    }

    if(countAdd1 * countAdd2 < 4)
        return false;


    int k;
    for(int i = 0; i< m_exCount; i++)
    {
        m_exItems[i].op = CExItem::Op_Add;

        while(1)
        {
            while(1)
            {
                k = qrand() % 10;
                if(m_addend1[k])
                    break;
            }
            m_exItems[i].add1 = k;

            while(1)
            {
                k = qrand() % 10;
                if(m_addend2[k])
                    break;
            }
            m_exItems[i].add2 = k;

            if(i>0 && m_exItems[i].equal(m_exItems[i-1]))
                continue;
            if(i>1 && m_exItems[i].equal(m_exItems[i-2]))
                continue;

            break;
        }
    }

    m_cur = 0;
    m_preLabel->setText("");
    m_nextLabel->setText("");
    m_curLabel->setText("");
    m_progressingLabel->setText(QString("%1/%2").arg(m_cur).arg(m_exCount));
    m_stopWatch->clear();

    return true;
}

void CAdditionTableWidget::slotStartNext()
{
    if(m_cur ==0)
    {
        generateExercise();
    }

    m_stopWatch->start();
    if(m_cur < m_exCount)
    {
        m_preLabel->setText(m_cur-1 < 0 ? "" : m_exItems[m_cur-1].repr(m_bUnordered, true));
        m_curLabel->setText(m_exItems[m_cur].repr(m_bUnordered, false));
        m_nextLabel->setText(m_cur+1 >= m_exCount ? "" : m_exItems[m_cur+1].repr(m_bUnordered, false));
        m_progressingLabel->setText(QString("%1/%2").arg(m_cur+1).arg(m_exCount));
        m_cur++;
    }
    else
    {
        slotStop();
        QMessageBox::information(this, tr("Addition table"), tr("Game over!"));
    }

}

void CAdditionTableWidget::slotStop()
{
    m_stopWatch->stop();
    m_cur = 0;
}


CMultiplicationTableWidget::CMultiplicationTableWidget(QWidget *parent): QWidget(parent)
                                                                          , m_cur(0)
{
    readSettings();

    //setBackgroundRole(QPalette::Button);
    //setAutoFillBackground(true);

    QGridLayout *grid = new QGridLayout;

    QFont ft("Consolas");


    m_preLabel = new QLabel("");
    m_preLabel->setFrameStyle(QFrame::Box | QFrame::Sunken);
    m_preLabel->setAlignment(Qt::AlignCenter);
    m_preLabel->setBackgroundRole(QPalette::Button);
    m_preLabel->setAutoFillBackground(true);

    ft.setPixelSize(40);
    m_preLabel->setFont(ft);
    grid->addWidget(m_preLabel, 0, 0, 1,3);

    m_progressingLabel = new QLabel;
    m_progressingLabel->setFrameStyle(QFrame::Box | QFrame::Sunken);
    m_progressingLabel->setAlignment(Qt::AlignCenter);
    //m_progressingLabel->setBackgroundRole(QPalette::Button);
    //m_progressingLabel->setAutoFillBackground(true);
    ft.setPixelSize(30);
    m_progressingLabel->setFont(ft);
    grid->addWidget(m_progressingLabel, 0, 4,1,1);

    m_stopWatch = new CStopWatch;
    grid->addWidget(m_stopWatch, 0, 5,1, 1);

    m_curLabel = new QLabel("");
    m_curLabel->setLineWidth(3);
    m_curLabel->setFrameStyle(QFrame::Box|QFrame::Raised);
    m_curLabel->setAlignment(Qt::AlignCenter);
    m_curLabel->setBackgroundRole(QPalette::Button);
    m_curLabel->setAutoFillBackground(true);
    ft.setPixelSize(100);
    m_curLabel->setFont(ft);
    grid->addWidget(m_curLabel, 1, 0, 2,6);

    m_nextLabel = new QLabel("");
    m_nextLabel->setFrameStyle(QFrame::Box | QFrame::Sunken);
    m_nextLabel->setAlignment(Qt::AlignCenter);
    m_nextLabel->setBackgroundRole(QPalette::Button);
    m_nextLabel->setAutoFillBackground(true);
    ft.setPixelSize(30);
    m_nextLabel->setFont(ft);
    grid->addWidget(m_nextLabel, 3, 0, 1,3);

    m_buttonBox = new QDialogButtonBox();
    m_buttonBox->addButton(tr("Start/Next(N)"), QDialogButtonBox::AcceptRole);
    m_buttonBox->addButton(tr("Stop"), QDialogButtonBox::RejectRole);
    connect(m_buttonBox, &QDialogButtonBox::accepted, this, &CMultiplicationTableWidget::slotStartNext);
    connect(m_buttonBox, &QDialogButtonBox::rejected, this, &CMultiplicationTableWidget::slotStop);
    grid->addWidget(m_buttonBox, 4, 2, 1, 2);
    setLayout(grid);

    generateExercise();
}

CMultiplicationTableWidget::~CMultiplicationTableWidget()
{}

void CMultiplicationTableWidget::readSettings(void)
{
    m_bUnordered = VALUE(MultblUnordered, false).toBool();
    m_exCount = VALUE(MultblCount, 20).toInt();

    //init data
    for(int i = 1; i< 10; i++)
    {
        m_addend1[i] = VALUE(QString("%1%2").arg(MultblAddend1).arg(i), true).toBool();
        m_addend2[i] = VALUE(QString("%1%2").arg(MultblAddend2).arg(i), true).toBool();
    }
    m_addend1[0] = false;
    m_addend2[0] = false;
}

void CMultiplicationTableWidget::writeSettings(void)
{
    SETVALUE(MultblUnordered, m_bUnordered);
    SETVALUE(MultblCount, m_exCount);

    for(int i = 0; i< 9; i++)
    {
        SETVALUE(QString("%1%2").arg(MultblAddend1).arg(i+1), m_addend1[i+1]);
        SETVALUE(QString("%1%2").arg(MultblAddend2).arg(i+1), m_addend2[i+1]);
    }
}

bool CMultiplicationTableWidget::generateExercise(void)
{
    int countAdd1= 0;
    int countAdd2= 0;

    int i;
    for(i = 0; i < 10; i++)
    {
        if(m_addend1[i])
            countAdd1++;
        if(m_addend2[i])
            countAdd2++;
    }

    if(countAdd1 * countAdd2 < 4)
        return false;


    int k;
    for(int i = 0; i< m_exCount; i++)
    {
        m_exItems[i].op = CExItem::Op_Multiple;

        while(1)
        {
            while(1)
            {
                k = qrand() % 10;
                if(m_addend1[k])
                    break;
            }
            m_exItems[i].add1 = k;

            while(1)
            {
                k = qrand() % 10;
                if(m_addend2[k])
                    break;
            }
            m_exItems[i].add2 = k;

            if(i>0 && m_exItems[i].equal(m_exItems[i-1]))
                continue;
            if(i>1 && m_exItems[i].equal(m_exItems[i-2]))
                continue;

            break;
        }
    }

    m_cur = 0;
    m_preLabel->setText("");
    m_nextLabel->setText("");
    m_curLabel->setText("");
    m_progressingLabel->setText(QString("%1/%2").arg(m_cur).arg(m_exCount));
    m_stopWatch->clear();

    return true;
}

void CMultiplicationTableWidget::slotStartNext()
{
    if(m_cur ==0)
    {
        generateExercise();
    }

    m_stopWatch->start();
    if(m_cur < m_exCount)
    {
        m_preLabel->setText(m_cur-1 < 0 ? "" : m_exItems[m_cur-1].repr(m_bUnordered, true));
        m_curLabel->setText(m_exItems[m_cur].repr(m_bUnordered, false));
        m_nextLabel->setText(m_cur+1 >= m_exCount ? "" : m_exItems[m_cur+1].repr(m_bUnordered, false));
        m_progressingLabel->setText(QString("%1/%2").arg(m_cur+1).arg(m_exCount));
        m_cur++;
    }
    else
    {
        slotStop();
        QMessageBox::information(this, tr("Multiplication table"), tr("Game over!"));
    }

}

void CMultiplicationTableWidget::slotStop()
{
    m_stopWatch->stop();
    m_cur = 0;
}



CAdditionWidget::CAdditionWidget(QWidget *parent): QWidget(parent)
                                                    , m_cur(0)
{
    readSettings();

    QFont ft("Consolas");

    QGridLayout *grid = new QGridLayout;

    m_preLabel = new QLabel("");
    m_preLabel->setFrameStyle(QFrame::Box | QFrame::Sunken);
    m_preLabel->setAlignment(Qt::AlignCenter);
    m_preLabel->setBackgroundRole(QPalette::Button);
    m_preLabel->setAutoFillBackground(true);

    ft.setPixelSize(40);
    m_preLabel->setFont(ft);
    grid->addWidget(m_preLabel, 0, 0, 1,3);

    m_progressingLabel = new QLabel;
    m_progressingLabel->setFrameStyle(QFrame::Box | QFrame::Sunken);
    m_progressingLabel->setAlignment(Qt::AlignCenter);
    //m_progressingLabel->setBackgroundRole(QPalette::Button);
    //m_progressingLabel->setAutoFillBackground(true);
    ft.setPixelSize(30);
    m_progressingLabel->setFont(ft);
    grid->addWidget(m_progressingLabel, 0, 4,1,1);

    m_stopWatch = new CStopWatch;
    grid->addWidget(m_stopWatch, 0, 5,1, 1);

    m_curLabel = new QLabel("");
    m_curLabel->setLineWidth(3);
    m_curLabel->setFrameStyle(QFrame::Box|QFrame::Raised);
    m_curLabel->setAlignment(Qt::AlignCenter);
    m_curLabel->setBackgroundRole(QPalette::Button);
    m_curLabel->setAutoFillBackground(true);
    ft.setPixelSize(100);
    m_curLabel->setFont(ft);
    grid->addWidget(m_curLabel, 1, 0, 2,6);

    m_nextLabel = new QLabel("");
    m_nextLabel->setFrameStyle(QFrame::Box | QFrame::Sunken);
    m_nextLabel->setAlignment(Qt::AlignCenter);
    m_nextLabel->setBackgroundRole(QPalette::Button);
    m_nextLabel->setAutoFillBackground(true);
    ft.setPixelSize(30);
    m_nextLabel->setFont(ft);
    grid->addWidget(m_nextLabel, 3, 0, 1,3);

    m_buttonBox = new QDialogButtonBox();
    m_buttonBox->addButton(tr("Start/Next(N)"), QDialogButtonBox::AcceptRole);
    m_buttonBox->addButton(tr("Stop"), QDialogButtonBox::RejectRole);
    connect(m_buttonBox, &QDialogButtonBox::accepted, this, &CAdditionWidget::slotStartNext);
    connect(m_buttonBox, &QDialogButtonBox::rejected, this, &CAdditionWidget::slotStop);
    grid->addWidget(m_buttonBox, 4, 2, 1, 2);
    setLayout(grid);

    generateExercise();

}

CAdditionWidget::~CAdditionWidget()
{}

void CAdditionWidget::readSettings(void)
{
    m_exCount = VALUE(AddCount, 20).toInt();
    m_min1 = VALUE(AddMin1, 1).toInt();
    m_max1 = VALUE(AddMax1, 99).toInt();
    m_min2 = VALUE(AddMin2, 1).toInt();
    m_max2 = VALUE(AddMax2, 99).toInt();
}

void CAdditionWidget::writeSettings(void)
{
    SETVALUE(AddCount, m_exCount);
    SETVALUE(AddMin1, m_min1);
    SETVALUE(AddMax1, m_max1);
    SETVALUE(AddMin2, m_min2);
    SETVALUE(AddMax2, m_max2);
}

bool CAdditionWidget::generateExercise(void)
{
    int countAdd1= m_max1 - m_min1 +1;
    int countAdd2= m_max2 - m_min2 +1;

    int i;

    if(countAdd1 * countAdd2 < 4)
        return false;


    for(i = 0; i< m_exCount; i++)
    {
        m_exItems[i].op = CExItem::Op_Add;

        while(1)
        {
            m_exItems[i].add1 = qrand() % countAdd1 + m_min1;
            m_exItems[i].add2 = qrand() % countAdd2 + m_min2;

            if(i>0 && m_exItems[i].equal(m_exItems[i-1]))
                continue;
            if(i>1 && m_exItems[i].equal(m_exItems[i-2]))
                continue;

            break;
        }
    }

    m_cur = 0;
    m_preLabel->setText("");
    m_nextLabel->setText("");
    m_curLabel->setText("");
    m_progressingLabel->setText(QString("%1/%2").arg(m_cur).arg(m_exCount));
    m_stopWatch->clear();

    return true;
}

void CAdditionWidget::slotStartNext()
{
    if(m_cur ==0)
    {
        generateExercise();
    }

    m_stopWatch->start();
    if(m_cur < m_exCount)
    {
        m_preLabel->setText(m_cur-1 < 0 ? "" : m_exItems[m_cur-1].repr(true, true));
        m_curLabel->setText(m_exItems[m_cur].repr(true, false));
        m_nextLabel->setText(m_cur+1 >= m_exCount ? "" : m_exItems[m_cur+1].repr(true, false));
        m_progressingLabel->setText(QString("%1/%2").arg(m_cur+1).arg(m_exCount));
        m_cur++;
    }
    else
    {
        slotStop();
        QMessageBox::information(this, tr("Addition"), tr("Game over!"));
    }

}

void CAdditionWidget::slotStop()
{
    m_stopWatch->stop();
    m_cur = 0;
}



CSubtractionWidget::CSubtractionWidget(QWidget *parent): QWidget(parent)
                                                          , m_cur(0)
{
    readSettings();

    QFont ft("Consolas");

    QGridLayout *grid = new QGridLayout;

    m_preLabel = new QLabel("");
    m_preLabel->setFrameStyle(QFrame::Box | QFrame::Sunken);
    m_preLabel->setAlignment(Qt::AlignCenter);
    m_preLabel->setBackgroundRole(QPalette::Button);
    m_preLabel->setAutoFillBackground(true);

    ft.setPixelSize(40);
    m_preLabel->setFont(ft);
    grid->addWidget(m_preLabel, 0, 0, 1,3);

    m_progressingLabel = new QLabel;
    m_progressingLabel->setFrameStyle(QFrame::Box | QFrame::Sunken);
    m_progressingLabel->setAlignment(Qt::AlignCenter);
    //m_progressingLabel->setBackgroundRole(QPalette::Button);
    //m_progressingLabel->setAutoFillBackground(true);
    ft.setPixelSize(30);
    m_progressingLabel->setFont(ft);
    grid->addWidget(m_progressingLabel, 0, 4,1,1);

    m_stopWatch = new CStopWatch;
    grid->addWidget(m_stopWatch, 0, 5,1, 1);

    m_curLabel = new QLabel("");
    m_curLabel->setLineWidth(3);
    m_curLabel->setFrameStyle(QFrame::Box|QFrame::Raised);
    m_curLabel->setAlignment(Qt::AlignCenter);
    m_curLabel->setBackgroundRole(QPalette::Button);
    m_curLabel->setAutoFillBackground(true);
    ft.setPixelSize(100);
    m_curLabel->setFont(ft);
    grid->addWidget(m_curLabel, 1, 0, 2,6);

    m_nextLabel = new QLabel("");
    m_nextLabel->setFrameStyle(QFrame::Box | QFrame::Sunken);
    m_nextLabel->setAlignment(Qt::AlignCenter);
    m_nextLabel->setBackgroundRole(QPalette::Button);
    m_nextLabel->setAutoFillBackground(true);
    ft.setPixelSize(30);
    m_nextLabel->setFont(ft);
    grid->addWidget(m_nextLabel, 3, 0, 1,3);

    m_buttonBox = new QDialogButtonBox();
    m_buttonBox->addButton(tr("Start/Next(N)"), QDialogButtonBox::AcceptRole);
    m_buttonBox->addButton(tr("Stop"), QDialogButtonBox::RejectRole);
    connect(m_buttonBox, &QDialogButtonBox::accepted, this, &CSubtractionWidget::slotStartNext);
    connect(m_buttonBox, &QDialogButtonBox::rejected, this, &CSubtractionWidget::slotStop);
    grid->addWidget(m_buttonBox, 4, 2, 1, 2);
    setLayout(grid);

    generateExercise();

}

CSubtractionWidget::~CSubtractionWidget()
{}

void CSubtractionWidget::readSettings(void)
{
    m_exCount = VALUE(SubCount, 20).toInt();
    m_min1 = VALUE(SubMin1, 1).toInt();
    m_max1 = VALUE(SubMax1, 99).toInt();
    m_min2 = VALUE(SubMin2, 1).toInt();
    m_max2 = VALUE(SubMax2, 99).toInt();
}

void CSubtractionWidget::writeSettings(void)
{
    SETVALUE(SubCount, m_exCount);
    SETVALUE(SubMin1, m_min1);
    SETVALUE(SubMax1, m_max1);
    SETVALUE(SubMin2, m_min2);
    SETVALUE(SubMax2, m_max2);
}


bool CSubtractionWidget::generateExercise(void)
{
    int countSubtrahend= m_max1 - m_min1 +1;
    int countReminder= m_max2 - m_min2 +1;

    int i;

    if(countSubtrahend * countReminder < 4)
        return false;

    int d;
    for(i = 0; i< m_exCount; i++)
    {
        m_exItems[i].op = CExItem::Op_Sub;

        while(1)
        {
            m_exItems[i].add1 = qrand() % countSubtrahend + m_min1;
            m_exItems[i].add2 = qrand() % countReminder + m_min2;

            if(i>0 && m_exItems[i].equal(m_exItems[i-1]))
                continue;
            if(i>1 && m_exItems[i].equal(m_exItems[i-2]))
                continue;

            d = m_exItems[i].add1;
            m_exItems[i].add1 += m_exItems[i].add2;
            m_exItems[i].add2 = d;

            break;
        }
    }

    m_cur = 0;
    m_preLabel->setText("");
    m_nextLabel->setText("");
    m_curLabel->setText("");
    m_progressingLabel->setText(QString("%1/%2").arg(m_cur).arg(m_exCount));
    m_stopWatch->clear();

    return true;
}

void CSubtractionWidget::slotStartNext()
{
    if(m_cur ==0)
    {
        generateExercise();
    }

    m_stopWatch->start();
    if(m_cur < m_exCount)
    {
        m_preLabel->setText(m_cur-1 < 0 ? "" : m_exItems[m_cur-1].repr(true, true));
        m_curLabel->setText(m_exItems[m_cur].repr(true, false));
        m_nextLabel->setText(m_cur+1 >= m_exCount ? "" : m_exItems[m_cur+1].repr(true, false));
        m_progressingLabel->setText(QString("%1/%2").arg(m_cur+1).arg(m_exCount));
        m_cur++;
    }
    else
    {
        slotStop();
        QMessageBox::information(this, tr("Subtraction"), tr("Game over!"));
    }

}

void CSubtractionWidget::slotStop()
{
    m_stopWatch->stop();
    m_cur = 0;
}






CMultiplicationWidget::CMultiplicationWidget(QWidget *parent): QWidget(parent)
                                                              , m_cur(0)
{
    readSettings();

    QFont ft("Consolas");

    QGridLayout *grid = new QGridLayout;

    m_preLabel = new QLabel("");
    m_preLabel->setFrameStyle(QFrame::Box | QFrame::Sunken);
    m_preLabel->setAlignment(Qt::AlignCenter);
    m_preLabel->setBackgroundRole(QPalette::Button);
    m_preLabel->setAutoFillBackground(true);

    ft.setPixelSize(40);
    m_preLabel->setFont(ft);
    grid->addWidget(m_preLabel, 0, 0, 1,3);

    m_progressingLabel = new QLabel;
    m_progressingLabel->setFrameStyle(QFrame::Box | QFrame::Sunken);
    m_progressingLabel->setAlignment(Qt::AlignCenter);
    //m_progressingLabel->setBackgroundRole(QPalette::Button);
    //m_progressingLabel->setAutoFillBackground(true);
    ft.setPixelSize(30);
    m_progressingLabel->setFont(ft);
    grid->addWidget(m_progressingLabel, 0, 4,1,1);

    m_stopWatch = new CStopWatch;
    grid->addWidget(m_stopWatch, 0, 5,1, 1);

    m_curLabel = new QLabel("");
    m_curLabel->setLineWidth(3);
    m_curLabel->setFrameStyle(QFrame::Box|QFrame::Raised);
    m_curLabel->setAlignment(Qt::AlignCenter);
    m_curLabel->setBackgroundRole(QPalette::Button);
    m_curLabel->setAutoFillBackground(true);
    ft.setPixelSize(100);
    m_curLabel->setFont(ft);
    grid->addWidget(m_curLabel, 1, 0, 2,6);

    m_nextLabel = new QLabel("");
    m_nextLabel->setFrameStyle(QFrame::Box | QFrame::Sunken);
    m_nextLabel->setAlignment(Qt::AlignCenter);
    m_nextLabel->setBackgroundRole(QPalette::Button);
    m_nextLabel->setAutoFillBackground(true);
    ft.setPixelSize(30);
    m_nextLabel->setFont(ft);
    grid->addWidget(m_nextLabel, 3, 0, 1,3);

    m_buttonBox = new QDialogButtonBox();
    m_buttonBox->addButton(tr("Start/Next(N)"), QDialogButtonBox::AcceptRole);
    m_buttonBox->addButton(tr("Stop"), QDialogButtonBox::RejectRole);
    connect(m_buttonBox, &QDialogButtonBox::accepted, this, &CMultiplicationWidget::slotStartNext);
    connect(m_buttonBox, &QDialogButtonBox::rejected, this, &CMultiplicationWidget::slotStop);
    grid->addWidget(m_buttonBox, 4, 2, 1, 2);
    setLayout(grid);

    generateExercise();
}

CMultiplicationWidget::~CMultiplicationWidget()
{}

void CMultiplicationWidget::readSettings(void)
{
    m_exCount = VALUE(MulCount, 20).toInt();
    m_min1 = VALUE(MulMin1, 1).toInt();
    m_max1 = VALUE(MulMax1, 99).toInt();
    m_min2 = VALUE(MulMin2, 1).toInt();
    m_max2 = VALUE(MulMax2, 99).toInt();
}

void CMultiplicationWidget::writeSettings(void)
{
    SETVALUE(MulCount, m_exCount);
    SETVALUE(MulMin1, m_min1);
    SETVALUE(MulMax1, m_max1);
    SETVALUE(MulMin2, m_min2);
    SETVALUE(MulMax2, m_max2);
}

bool CMultiplicationWidget::generateExercise(void)
{
    int count1= m_max1 - m_min1 +1;
    int count2= m_max2 - m_min2 +1;

    int i;

    if(count1 * count2 < 4)
        return false;


    for(i = 0; i< m_exCount; i++)
    {
        m_exItems[i].op = CExItem::Op_Multiple;

        while(1)
        {
            m_exItems[i].add1 = qrand() % count1 + m_min1;
            m_exItems[i].add2 = qrand() % count2 + m_min2;

            if(i>0 && m_exItems[i].equal(m_exItems[i-1]))
                continue;
            if(i>1 && m_exItems[i].equal(m_exItems[i-2]))
                continue;

            break;
        }
    }

    m_cur = 0;
    m_preLabel->setText("");
    m_nextLabel->setText("");
    m_curLabel->setText("");
    m_progressingLabel->setText(QString("%1/%2").arg(m_cur).arg(m_exCount));
    m_stopWatch->clear();

    return true;
}

void CMultiplicationWidget::slotStartNext()
{
    if(m_cur ==0)
    {
        generateExercise();
    }

    m_stopWatch->start();
    if(m_cur < m_exCount)
    {
        m_preLabel->setText(m_cur-1 < 0 ? "" : m_exItems[m_cur-1].repr(true, true));
        m_curLabel->setText(m_exItems[m_cur].repr(true, false));
        m_nextLabel->setText(m_cur+1 >= m_exCount ? "" : m_exItems[m_cur+1].repr(true, false));
        m_progressingLabel->setText(QString("%1/%2").arg(m_cur+1).arg(m_exCount));
        m_cur++;
    }
    else
    {
        slotStop();
        QMessageBox::information(this, tr("Multiplication"), tr("Game over!"));
    }

}

void CMultiplicationWidget::slotStop()
{
    m_stopWatch->stop();
    m_cur = 0;
}



CDivisionWidget::CDivisionWidget(QWidget *parent): QWidget(parent)
                                                  , m_cur(0)
{
    readSettings();
    QFont ft("Consolas");

    QGridLayout *grid = new QGridLayout;

    m_preLabel = new QLabel("");
    m_preLabel->setFrameStyle(QFrame::Box | QFrame::Sunken);
    m_preLabel->setAlignment(Qt::AlignCenter);
    m_preLabel->setBackgroundRole(QPalette::Button);
    m_preLabel->setAutoFillBackground(true);

    ft.setPixelSize(40);
    m_preLabel->setFont(ft);
    grid->addWidget(m_preLabel, 0, 0, 1,3);

    m_progressingLabel = new QLabel;
    m_progressingLabel->setFrameStyle(QFrame::Box | QFrame::Sunken);
    m_progressingLabel->setAlignment(Qt::AlignCenter);
    //m_progressingLabel->setBackgroundRole(QPalette::Button);
    //m_progressingLabel->setAutoFillBackground(true);
    ft.setPixelSize(30);
    m_progressingLabel->setFont(ft);
    grid->addWidget(m_progressingLabel, 0, 4,1,1);

    m_stopWatch = new CStopWatch;
    grid->addWidget(m_stopWatch, 0, 5,1, 1);

    m_curLabel = new QLabel("");
    m_curLabel->setLineWidth(3);
    m_curLabel->setFrameStyle(QFrame::Box|QFrame::Raised);
    m_curLabel->setAlignment(Qt::AlignCenter);
    m_curLabel->setBackgroundRole(QPalette::Button);
    m_curLabel->setAutoFillBackground(true);
    ft.setPixelSize(100);
    m_curLabel->setFont(ft);
    grid->addWidget(m_curLabel, 1, 0, 2,6);

    m_nextLabel = new QLabel("");
    m_nextLabel->setFrameStyle(QFrame::Box | QFrame::Sunken);
    m_nextLabel->setAlignment(Qt::AlignCenter);
    m_nextLabel->setBackgroundRole(QPalette::Button);
    m_nextLabel->setAutoFillBackground(true);
    ft.setPixelSize(30);
    m_nextLabel->setFont(ft);
    grid->addWidget(m_nextLabel, 3, 0, 1,3);

    m_buttonBox = new QDialogButtonBox();
    m_buttonBox->addButton(tr("Start/Next(N)"), QDialogButtonBox::AcceptRole);
    m_buttonBox->addButton(tr("Stop"), QDialogButtonBox::RejectRole);
    connect(m_buttonBox, &QDialogButtonBox::accepted, this, &CDivisionWidget::slotStartNext);
    connect(m_buttonBox, &QDialogButtonBox::rejected, this, &CDivisionWidget::slotStop);
    grid->addWidget(m_buttonBox, 4, 2, 1, 2);
    setLayout(grid);

    generateExercise();

}


CDivisionWidget::~CDivisionWidget()
{}

void CDivisionWidget::readSettings(void)
{
    m_exCount = VALUE(DivCount, 20).toInt();
    m_min1 = VALUE(DivMin1, 1).toInt();
    m_max1 = VALUE(DivMax1, 99).toInt();
    m_min2 = VALUE(DivMin2, 1).toInt();
    m_max2 = VALUE(DivMax2, 99).toInt();
}

void CDivisionWidget::writeSettings(void)
{
    SETVALUE(DivCount, m_exCount);
    SETVALUE(DivMin1, m_min1);
    SETVALUE(DivMax1, m_max1);
    SETVALUE(DivMin2, m_min2);
    SETVALUE(DivMax2, m_max2);
}



bool CDivisionWidget::generateExercise(void)
{
    int count1= m_max1 - m_min1 +1;
    int count2= m_max2 - m_min2 +1;

    int i;

    if(count1 * count2 < 4)
        return false;

    int d;
    for(i = 0; i< m_exCount; i++)
    {
        m_exItems[i].op = CExItem::Op_Division;

        while(1)
        {
            m_exItems[i].add1 = qrand() % count1 + m_min1;
            m_exItems[i].add2 = qrand() % count2 + m_min2;

            if(i>0 && m_exItems[i].equal(m_exItems[i-1]))
                continue;
            if(i>1 && m_exItems[i].equal(m_exItems[i-2]))
                continue;

            d = m_exItems[i].add1;
            m_exItems[i].add1 *= m_exItems[i].add2;
            m_exItems[i].add2 = d;

            break;
        }
    }

    m_cur = 0;
    m_preLabel->setText("");
    m_nextLabel->setText("");
    m_curLabel->setText("");
    m_progressingLabel->setText(QString("%1/%2").arg(m_cur).arg(m_exCount));
    m_stopWatch->clear();

    return true;
}

void CDivisionWidget::slotStartNext()
{
    if(m_cur ==0)
    {
        generateExercise();
    }

    m_stopWatch->start();
    if(m_cur < m_exCount)
    {
        m_preLabel->setText(m_cur-1 < 0 ? "" : m_exItems[m_cur-1].repr(true, true));
        m_curLabel->setText(m_exItems[m_cur].repr(true, false));
        m_nextLabel->setText(m_cur+1 >= m_exCount ? "" : m_exItems[m_cur+1].repr(true, false));
        m_progressingLabel->setText(QString("%1/%2").arg(m_cur+1).arg(m_exCount));
        m_cur++;
    }
    else
    {
        slotStop();
        QMessageBox::information(this, tr("Division"), tr("Game over!"));
    }

}

void CDivisionWidget::slotStop()
{
    m_stopWatch->stop();
    m_cur = 0;
}





CConfigWidget::CConfigWidget(QWidget *parent): QWidget(parent)
{
    //part of additon table

    m_addtblUnordered = new QCheckBox(tr("Unordered"));
    m_addtblUnordered->setCheckState(mainwin->m_addTableWidget->m_bUnordered ? Qt::Checked : Qt::Unchecked);
    connect(m_addtblUnordered, &QCheckBox::toggled, this, &CConfigWidget::refreshAddTbl);

    m_addtblExCount = new QSpinBox;
    m_addtblExCount->setMinimum(1);
    m_addtblExCount->setMaximum(MaxExerciseCount);
    m_addtblExCount->setValue(mainwin->m_addTableWidget->m_exCount);
    connect(m_addtblExCount, static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged),
            this, &CConfigWidget::refreshAddTbl);

    int i;
    for(i = 0; i<9; i++)
    {
        m_addtblAdd1[i] = new QCheckBox(QString().setNum(i+1));
        m_addtblAdd1[i]->setCheckState(mainwin->m_addTableWidget->m_addend1[i+1] ? Qt::Checked : Qt::Unchecked);
        connect(m_addtblAdd1[i], &QCheckBox::toggled, this, &CConfigWidget::refreshAddTbl);

        m_addtblAdd2[i] = new QCheckBox(QString().setNum(i+1));
        m_addtblAdd2[i]->setCheckState(mainwin->m_addTableWidget->m_addend2[i+1] ? Qt::Checked : Qt::Unchecked);
        connect(m_addtblAdd2[i], &QCheckBox::toggled, this, &CConfigWidget::refreshAddTbl);
    }

    QGroupBox *addtblGroupBox = new QGroupBox(tr("Addition table"));
    QVBoxLayout *vbox1 = new QVBoxLayout;
    vbox1->addWidget(m_addtblUnordered);
    QFormLayout* formLayout = new QFormLayout;
    formLayout->addRow(tr("ex count:"), m_addtblExCount);
    vbox1->addLayout(formLayout);

    QHBoxLayout *hbox1 = new QHBoxLayout;
    QGroupBox *add1GroupBox = new QGroupBox(tr("Addend 1"));
    QGroupBox *add2GroupBox = new QGroupBox(tr("Addend 2"));

    QGridLayout *gbox2 = new QGridLayout;
    QGridLayout *gbox3 = new QGridLayout;

    for(i = 0; i<9; i++)
    {
        gbox2->addWidget(m_addtblAdd1[i], i/5, i%5);
        gbox3->addWidget(m_addtblAdd2[i], i/5, i%5);
    }

    add1GroupBox->setLayout(gbox2);
    add2GroupBox->setLayout(gbox3);



    hbox1->addWidget(add1GroupBox);
    hbox1->addWidget(add2GroupBox);


    vbox1->addLayout(hbox1);
    addtblGroupBox->setLayout(vbox1);


    // part of multi table
    m_multblUnordered = new QCheckBox(tr("Unordered"));
    m_multblUnordered->setCheckState(mainwin->m_mulTableWidget->m_bUnordered ? Qt::Checked : Qt::Unchecked);
    connect(m_multblUnordered, &QCheckBox::toggled, this, &CConfigWidget::refreshMulTbl);

    m_multblExCount = new QSpinBox;
    m_multblExCount->setMinimum(1);
    m_multblExCount->setMaximum(MaxExerciseCount);
    m_multblExCount->setValue(mainwin->m_mulTableWidget->m_exCount);
    connect(m_multblExCount, static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged),
            this, &CConfigWidget::refreshMulTbl);

    for(i = 0; i<9; i++)
    {
        m_multblAdd1[i] = new QCheckBox(QString().setNum(i+1));
        m_multblAdd1[i]->setCheckState(mainwin->m_mulTableWidget->m_addend1[i+1] ? Qt::Checked : Qt::Unchecked);
        connect(m_multblAdd1[i], &QCheckBox::toggled, this, &CConfigWidget::refreshMulTbl);

        m_multblAdd2[i] = new QCheckBox(QString().setNum(i+1));
        m_multblAdd2[i]->setCheckState(mainwin->m_mulTableWidget->m_addend2[i+1] ? Qt::Checked : Qt::Unchecked);
        connect(m_multblAdd2[i], &QCheckBox::toggled, this, &CConfigWidget::refreshMulTbl);
    }

    QGroupBox *multblGroupBox = new QGroupBox(tr("Multiplication table"));
    vbox1 = new QVBoxLayout;
    vbox1->addWidget(m_multblUnordered);
    formLayout = new QFormLayout;
    formLayout->addRow(tr("ex count:"), m_multblExCount);
    vbox1->addLayout(formLayout);

    hbox1 = new QHBoxLayout;
    add1GroupBox = new QGroupBox(tr("Multiplier 1"));
    add2GroupBox = new QGroupBox(tr("Multiplier 2"));

    gbox2 = new QGridLayout;
    gbox3 = new QGridLayout;

    for(i = 0; i<9; i++)
    {
        gbox2->addWidget(m_multblAdd1[i], i/5, i%5);
        gbox3->addWidget(m_multblAdd2[i], i/5, i%5);
    }

    add1GroupBox->setLayout(gbox2);
    add2GroupBox->setLayout(gbox3);



    hbox1->addWidget(add1GroupBox);
    hbox1->addWidget(add2GroupBox);


    vbox1->addLayout(hbox1);
    multblGroupBox->setLayout(vbox1);


    // part of addition

    m_addMin1 = new QSpinBox;
    m_addMin1->setMinimum(1);
    m_addMin1->setMaximum(MaxAdd);
    m_addMin1->setValue(mainwin->m_addWidget->m_min1);
    connect(m_addMin1, static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged),
            this, &CConfigWidget::refreshAdd);

    m_addMax1 = new QSpinBox;
    m_addMax1->setMinimum(1);
    m_addMax1->setMaximum(MaxAdd);
    m_addMax1->setValue(mainwin->m_addWidget->m_max1);
    connect(m_addMax1, static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged),
            this, &CConfigWidget::refreshAdd);

    m_addMin2 = new QSpinBox;
    m_addMin2->setMinimum(1);
    m_addMin2->setMaximum(MaxAdd);
    m_addMin2->setValue(mainwin->m_addWidget->m_min2);
    connect(m_addMin2, static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged),
            this, &CConfigWidget::refreshAdd);

    m_addMax2 = new QSpinBox;
    m_addMax2->setMinimum(1);
    m_addMax2->setMaximum(MaxAdd);
    m_addMax2->setValue(mainwin->m_addWidget->m_max2);
    connect(m_addMax2, static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged),
            this, &CConfigWidget::refreshAdd);

    m_addExCount = new QSpinBox;
    m_addExCount->setMinimum(1);
    m_addExCount->setMaximum(MaxExerciseCount);
    m_addExCount->setValue(mainwin->m_addWidget->m_exCount);
    connect(m_addExCount, static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged),
            this, &CConfigWidget::refreshAdd);

    QGroupBox *addGroupBox = new QGroupBox(tr("Addition"));
    QFormLayout* formLayout2 = new QFormLayout;
    formLayout2->addRow(tr("ex count:"), m_addExCount);
    formLayout2->addRow(tr("addend1 min:"), m_addMin1);
    formLayout2->addRow(tr("addend1 max:"), m_addMax1);
    formLayout2->addRow(tr("addend2 min:"), m_addMin2);
    formLayout2->addRow(tr("addend2 max:"), m_addMax2);
    addGroupBox->setLayout(formLayout2);

    // part of subtraction

    m_subMin1 = new QSpinBox;
    m_subMin1->setMinimum(1);
    m_subMin1->setMaximum(MaxAdd-1);
    m_subMin1->setValue(mainwin->m_subWidget->m_min1);
    connect(m_subMin1, static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged),
            this, &CConfigWidget::refreshSub);

    m_subMax1 = new QSpinBox;
    m_subMax1->setMinimum(2);
    m_subMax1->setMaximum(MaxAdd);
    m_subMax1->setValue(mainwin->m_subWidget->m_max1);
    connect(m_subMax1, static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged),
            this, &CConfigWidget::refreshSub);

    m_remMin2 = new QSpinBox;
    m_remMin2->setMinimum(1);
    m_remMin2->setMaximum(MaxAdd);
    m_remMin2->setValue(mainwin->m_subWidget->m_min2);
    connect(m_remMin2, static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged),
            this, &CConfigWidget::refreshSub);

    m_remMax2 = new QSpinBox;
    m_remMax2->setMinimum(1);
    m_remMax2->setMaximum(MaxAdd);
    m_remMax2->setValue(mainwin->m_subWidget->m_max2);
    connect(m_remMax2, static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged),
            this, &CConfigWidget::refreshSub);

    m_subExCount = new QSpinBox;
    m_subExCount->setMinimum(1);
    m_subExCount->setMaximum(MaxExerciseCount);
    m_subExCount->setValue(mainwin->m_subWidget->m_exCount);
    connect(m_subExCount, static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged),
            this, &CConfigWidget::refreshSub);

    QGroupBox *subGroupBox = new QGroupBox(tr("Subtraction"));
    formLayout2 = new QFormLayout;
    formLayout2->addRow(tr("ex count:"), m_subExCount);
    formLayout2->addRow(tr("subtrahend min:"), m_subMin1);
    formLayout2->addRow(tr("subtrahend max:"), m_subMax1);
    formLayout2->addRow(tr("reminder min:"), m_remMin2);
    formLayout2->addRow(tr("reminder max:"), m_remMax2);
    subGroupBox->setLayout(formLayout2);


    // part of multiplication

    m_mulMin1 = new QSpinBox;
    m_mulMin1->setMinimum(1);
    m_mulMin1->setMaximum(MaxAdd);
    m_mulMin1->setValue(mainwin->m_mulWidget->m_min1);
    connect(m_mulMin1, static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged),
            this, &CConfigWidget::refreshMul);

    m_mulMax1 = new QSpinBox;
    m_mulMax1->setMinimum(1);
    m_mulMax1->setMaximum(MaxAdd);
    m_mulMax1->setValue(mainwin->m_mulWidget->m_max1);
    connect(m_mulMax1, static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged),
            this, &CConfigWidget::refreshMul);

    m_mulMin2 = new QSpinBox;
    m_mulMin2->setMinimum(1);
    m_mulMin2->setMaximum(MaxAdd);
    m_mulMin2->setValue(mainwin->m_mulWidget->m_min2);
    connect(m_mulMin2, static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged),
            this, &CConfigWidget::refreshMul);

    m_mulMax2 = new QSpinBox;
    m_mulMax2->setMinimum(1);
    m_mulMax2->setMaximum(MaxAdd);
    m_mulMax2->setValue(mainwin->m_mulWidget->m_max2);
    connect(m_mulMax2, static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged),
            this, &CConfigWidget::refreshMul);

    m_mulExCount = new QSpinBox;
    m_mulExCount->setMinimum(1);
    m_mulExCount->setMaximum(MaxExerciseCount);
    m_mulExCount->setValue(mainwin->m_mulWidget->m_exCount);
    connect(m_mulExCount, static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged),
            this, &CConfigWidget::refreshMul);

    QGroupBox *mulGroupBox = new QGroupBox(tr("Multiplication"));
    formLayout2 = new QFormLayout;
    formLayout2->addRow(tr("ex count:"), m_mulExCount);
    formLayout2->addRow(tr("multiplier1 min:"), m_mulMin1);
    formLayout2->addRow(tr("multiplier1 max:"), m_mulMax1);
    formLayout2->addRow(tr("multiplier2 min:"), m_mulMin2);
    formLayout2->addRow(tr("multiplier2 max:"), m_mulMax2);
    mulGroupBox->setLayout(formLayout2);

    // part of division

    m_divMin1 = new QSpinBox;
    m_divMin1->setMinimum(1);
    m_divMin1->setMaximum(MaxAdd-1);
    m_divMin1->setValue(mainwin->m_divWidget->m_min1);
    connect(m_divMin1, static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged),
            this, &CConfigWidget::refreshDiv);

    m_divMax1 = new QSpinBox;
    m_divMax1->setMinimum(2);
    m_divMax1->setMaximum(MaxAdd);
    m_divMax1->setValue(mainwin->m_divWidget->m_max1);
    connect(m_divMax1, static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged),
            this, &CConfigWidget::refreshDiv);

    m_divMin2 = new QSpinBox;
    m_divMin2->setMinimum(1);
    m_divMin2->setMaximum(MaxAdd);
    m_divMin2->setValue(mainwin->m_divWidget->m_min2);
    connect(m_divMin2, static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged),
            this, &CConfigWidget::refreshDiv);

    m_divMax2 = new QSpinBox;
    m_divMax2->setMinimum(1);
    m_divMax2->setMaximum(MaxAdd);
    m_divMax2->setValue(mainwin->m_divWidget->m_max2);
    connect(m_divMax2, static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged),
            this, &CConfigWidget::refreshDiv);

    m_divExCount = new QSpinBox;
    m_divExCount->setMinimum(1);
    m_divExCount->setMaximum(MaxExerciseCount);
    m_divExCount->setValue(mainwin->m_subWidget->m_exCount);
    connect(m_divExCount, static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged),
            this, &CConfigWidget::refreshSub);

    QGroupBox *divGroupBox = new QGroupBox(tr("Division"));
    formLayout2 = new QFormLayout;
    formLayout2->addRow(tr("ex count:"), m_divExCount);
    formLayout2->addRow(tr("divisor min:"), m_divMin1);
    formLayout2->addRow(tr("divisor max:"), m_divMax1);
    formLayout2->addRow(tr("quotient min:"), m_divMin2);
    formLayout2->addRow(tr("quotient max:"), m_divMax2);
    divGroupBox->setLayout(formLayout2);


    QGridLayout* gridLayout = new QGridLayout;
    gridLayout->addWidget(addtblGroupBox, 0, 0, 1,1);
    gridLayout->addWidget(multblGroupBox, 0, 1, 1,1);
    gridLayout->addWidget(addGroupBox, 1, 0, 1,1);
    gridLayout->addWidget(subGroupBox, 1, 1, 1,1);
    gridLayout->addWidget(mulGroupBox, 2, 0, 1,1);
    gridLayout->addWidget(divGroupBox, 2, 1, 1,1);

    setLayout(gridLayout);
}
CConfigWidget::~CConfigWidget()
{}

void CConfigWidget::refreshAddTbl(void)
{
    CAdditionTableWidget *w = mainwin->m_addTableWidget;
    w->m_bUnordered = m_addtblUnordered->isChecked();
    w->m_exCount = m_addtblExCount->value();
    for(int i = 0; i< 9; i++)
    {
        w->m_addend1[i+1] = m_addtblAdd1[i]->isChecked();
        w->m_addend2[i+1] = m_addtblAdd2[i]->isChecked();
    }

    w->writeSettings();
}

void CConfigWidget::refreshMulTbl(void)
{
    CMultiplicationTableWidget *w = mainwin->m_mulTableWidget;
    w->m_bUnordered = m_addtblUnordered->isChecked();
    w->m_exCount = m_addtblExCount->value();
    for(int i = 0; i< 9; i++)
    {
        w->m_addend1[i+1] = m_addtblAdd1[i]->isChecked();
        w->m_addend2[i+1] = m_addtblAdd2[i]->isChecked();
    }
    w->writeSettings();
}

void CConfigWidget::refreshAdd(void)
{
    CAdditionWidget *w = mainwin->m_addWidget;

    w->m_exCount = m_addExCount->value();

    w->m_min1 = m_addMin1->value();
    w->m_max1 = m_addMax1->value();
    if(w->m_max1 < w->m_min1)
    {
        w->m_max1 = w->m_min1;
        m_addMax1->setValue(w->m_min1);
    }

    w->m_min2 = m_addMin2->value();
    w->m_max2 = m_addMax2->value();
    if(w->m_max2 < w->m_min2)
    {
        w->m_max2 = w->m_min2;
        m_addMax2->setValue(w->m_min2);
    }
    w->writeSettings();
}

void CConfigWidget::refreshSub(void)
{
    CSubtractionWidget *w = mainwin->m_subWidget;

    w->m_exCount = m_subExCount->value();

    w->m_min1 = m_subMin1->value();
    w->m_max1 = m_subMax1->value();
    if(w->m_max1 < w->m_min1)
    {
        w->m_max1 = w->m_min1;
        m_subMax1->setValue(w->m_min1);
    }

    w->m_min2 = m_remMin2->value();
    w->m_max2 = m_remMax2->value();
    if(w->m_max2 < w->m_min2)
    {
        w->m_max2 = w->m_min2;
        m_remMax2->setValue(w->m_min2);
    }
    w->writeSettings();
}

void CConfigWidget::refreshMul(void)
{
    CMultiplicationWidget *w = mainwin->m_mulWidget;

    w->m_exCount = m_mulExCount->value();

    w->m_min1 = m_mulMin1->value();
    w->m_max1 = m_mulMax1->value();
    if(w->m_max1 < w->m_min1)
    {
        w->m_max1 = w->m_min1;
        m_mulMax1->setValue(w->m_min1);
    }

    w->m_min2 = m_mulMin2->value();
    w->m_max2 = m_mulMax2->value();
    if(w->m_max2 < w->m_min2)
    {
        w->m_max2 = w->m_min2;
        m_mulMax2->setValue(w->m_min2);
    }
    w->writeSettings();
}

void CConfigWidget::refreshDiv(void)
{
    CDivisionWidget *w = mainwin->m_divWidget;

    w->m_exCount = m_divExCount->value();

    w->m_min1 = m_divMin1->value();
    w->m_max1 = m_divMax1->value();
    if(w->m_max1 < w->m_min1)
    {
        w->m_max1 = w->m_min1;
        m_divMax1->setValue(w->m_min1);
    }

    w->m_min2 = m_divMin2->value();
    w->m_max2 = m_divMax2->value();
    if(w->m_max2 < w->m_min2)
    {
        w->m_max2 = w->m_min2;
        m_divMax2->setValue(w->m_min2);
    }
    w->writeSettings();
}


CStopWatch::CStopWatch(QWidget *parent) : QLCDNumber(5, parent), m_bStarted(false), m_ms(0)
{
    setSegmentStyle(Filled);
    clear();
    m_timer = new QTimer(this);
    connect(m_timer, &QTimer::timeout, this, &CStopWatch::slotShowTime);
}

void CStopWatch::clear(void)
{
    stop();
    display("00:00");
}

bool CStopWatch::start(void)
{
    bool res = false;
    if(!m_bStarted)
    {
        res = true;
        m_bStarted = true;

        m_ms = 0;
        m_baseTime = QTime::currentTime();
        m_timer->start(1000);
    }
    return res;
}

bool CStopWatch::stop(void)
{
    bool res = false;
    if(m_bStarted)
    {
        m_timer->stop();

        m_bStarted = false;
        res = true;
    }
    return res;
}

void CStopWatch::slotShowTime()
{
    m_ms=m_baseTime.elapsed();
    int sec = m_ms /1000;
    sec = sec % 60;
    int min = m_ms /60000;
    if(min > 99)
        min = min % 99;
    QString text;
    text = QString("%1:%2").arg(min, 2, 10, QLatin1Char('0')).arg(sec,2,10, QLatin1Char('0'));
    display(text);
}

