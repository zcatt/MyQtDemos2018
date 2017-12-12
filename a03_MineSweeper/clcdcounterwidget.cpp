#include <QtWidgets>
#include "cresource.h"
#include "clcdcounterwidget.h"


CLcdCounterWidget::CLcdCounterWidget(QWidget *parent) : QFrame(parent)
{
    m_nValue = 0;
#if 1
    setFixedSize(CResource::DigitWidth*3+2, CResource::DigitHeight+2);
#else
    setFixedSize(12, 12);
#endif

    setFrameStyle(QFrame::Sunken | QFrame::Panel);
    setLineWidth(1);
    setMidLineWidth(0);

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
}

int CLcdCounterWidget::value(void)
{
    return m_nValue;
}

void CLcdCounterWidget::setValue(int nValue)
{

    qDebug()<<"setValue="<<nValue;
    m_nValue = qBound((int)Lower, nValue, (int)Upper);
    update();
}

void CLcdCounterWidget::inc(void)
{
    setValue(m_nValue+1);
}

void CLcdCounterWidget::dec(void)
{
    setValue(m_nValue-1);
}

void CLcdCounterWidget::paintEvent(QPaintEvent *e)
{
    QPainter painter(this);
    //painter.fillRect(rect(), Qt::yellow);
    int d3,d2,d1;
    d3 = m_nValue /100;
    d2 = (m_nValue %100) /10;
    d1 = (m_nValue %10);

    int x,y;
    x = 1;
    y =1;

    QPixmap* pPixmap;
    pPixmap = CResource::DigitImage(CResource::Digit0 - d3);
    painter.drawPixmap(x,y, *pPixmap);

    x+= CResource::DigitWidth;
    pPixmap = CResource::DigitImage(CResource::Digit0 - d2);
    painter.drawPixmap(x,y, *pPixmap);

    x+= CResource::DigitWidth;
    pPixmap = CResource::DigitImage(CResource::Digit0 - d1);
    painter.drawPixmap(x,y, *pPixmap);

    QFrame::paintEvent(e);
}
