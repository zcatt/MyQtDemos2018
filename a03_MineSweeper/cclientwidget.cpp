#include <QtWidgets>
#include "cresource.h"
#include "clcdcounterwidget.h"
#include "cminewidget.h"
#include "cclientwidget.h"

CClientWidget::CClientWidget(QWidget *parent) : QWidget(parent)
        , m_pBtn(NULL)
{

    setAutoFillBackground(true);
    //setMinimumSize(10,10);

    QPalette p;
    p= palette();

    QColor c;

    CResource::getColor(CResource::Light, c);
    p.setColor(QPalette::Light, c);

    CResource::getColor(CResource::Normal, c);
    p.setColor(QPalette::Window, c);

    CResource::getColor(CResource::Dark, c);
    p.setColor(QPalette::Dark, c);

    setPalette(p);


    CLcdCounterWidget *pMinesLcdWidget = new CLcdCounterWidget;
    pMinesLcdWidget->setValue(123);

    CLcdCounterWidget *pTickLcdWidget = new CLcdCounterWidget;
    pMinesLcdWidget->setValue(999);

    m_pBtn = new QPushButton;
#if 0
    //消除margins
    m_pBtn->setFixedSize(CResource::EmotionWidth, CResource::EmotionHeight);
#endif
    m_pBtn->setIconSize(QSize(CResource::EmotionWidth, CResource::EmotionHeight));
    m_pBtn->setIcon(*CResource::EmotionImage(CResource::EmotionSmile));

    QFrame *pInfoWidget = new QFrame;
    //pInfoWidget->setMinimumSize(3*24, 24);
    //pInfoWidget->setContentsMargins(6,4,6,4);
    pInfoWidget->setFrameStyle(QFrame::Panel | QFrame::Sunken);
    pInfoWidget->setLineWidth(2);
    pInfoWidget->setMidLineWidth(0);

    m_pMineWidget= new CMineWidget;


    QHBoxLayout *pInfoLayout = new QHBoxLayout;
    pInfoLayout->setContentsMargins(6,4,6,4);
    pInfoLayout->setSizeConstraint(QLayout::SetMinimumSize);
    pInfoLayout->addWidget(pMinesLcdWidget);
    pInfoLayout->addStretch(1);
    pInfoLayout->addWidget(m_pBtn);
    pInfoLayout->addStretch(1);
    pInfoLayout->addWidget(pTickLcdWidget);


    pInfoWidget->setLayout(pInfoLayout);


    QVBoxLayout *pMainLayout = new QVBoxLayout;
    pMainLayout->setContentsMargins(9,9,6,6);
    pMainLayout->setSpacing(6);
    pMainLayout->addWidget(pInfoWidget);
    pMainLayout->addWidget(m_pMineWidget);

    setLayout(pMainLayout);
    //setFixedSize(sizeHint());

    //install event filters
    m_pMineWidget->installEventFilter(this);
    pInfoWidget->installEventFilter(this);
    pMinesLcdWidget->installEventFilter(this);
    pTickLcdWidget->installEventFilter(this);


    //connect signals
    connect(m_pMineWidget, &CMineWidget::mineLeftChanged, pMinesLcdWidget, &CLcdCounterWidget::setValue);
    connect(m_pMineWidget, &CMineWidget::timeChanged, pTickLcdWidget, &CLcdCounterWidget::setValue);
    connect(m_pBtn, &QAbstractButton::clicked, m_pMineWidget, &CMineWidget::newGame);
    connect(m_pMineWidget, &CMineWidget::stateChanged, this, &CClientWidget::stateChanged);

    //load setting

    //pMineWidget->setLevel(CMineWidget::Primary);
}

void CClientWidget::stateChanged(int nState)
{
    int nEmotionId;

    switch(nState)
    {
    case CMineWidget::Ok:
        nEmotionId = CResource::EmotionGlass;
        break;
    case CMineWidget::Bombing:
        nEmotionId = CResource::EmotionSad;
        break;
    case CMineWidget::Ready:
    case CMineWidget::Sweeping:
    default:
        nEmotionId = CResource::EmotionSmile;
        break;
    }
    m_pBtn->setIcon(*CResource::EmotionImage(nEmotionId));
}

bool CClientWidget::eventFilter(QObject *object, QEvent *event)
{
    CMineWidget::State s = m_pMineWidget->getState();
    if(s == CMineWidget::Ready || s == CMineWidget::Sweeping)
    {
        switch (event->type()) {
        case QEvent::MouseButtonPress:
            m_pBtn->setIcon(*CResource::EmotionImage(CResource::EmotionOh));
            break;
        case QEvent::MouseButtonRelease:
            m_pBtn->setIcon(*CResource::EmotionImage(CResource::EmotionSmile));
            break;
        case QEvent::MouseMove:
            break;
        default:
            break;
        }
    }
    return false;
}

void CClientWidget::paintEvent(QPaintEvent *)
{
    QPainter painter(this);

    //already enable autoFillBackground
#if 0
    painter.fillRect(0,0,width(), height(), CResource::color(CResource::Normal));
#endif
    const QPalette& p = palette();
    painter.fillRect(0,0,3, height(), p.light());
    painter.fillRect(0,0,width(), 3, p.light());
}
