#include <QtWidgets>

#include "chalotextwidget.h"

CHaloTextWidget::CHaloTextWidget(QWidget *parent) : QWidget(parent)
{

}

void CHaloTextWidget::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    QColor clrText(Qt::magenta);
    QColor clrHalo = QColor(255-clrText.red(), 255-clrText.green(), 255-clrText.blue());

    QPainterPath pp;
    pp.addText(100,100, QFont("Arial",64), QString("世界和平"));
    //pp.addRect(100,100, 100, 100);


    QPainterPath path;
    QPainterPathStroker pps;
    pps.setCapStyle(Qt::RoundCap);

#if 1
     pps.setJoinStyle(Qt::MiterJoin);   pps.setMiterLimit(10);
     pps.setWidth(10);
     path = pps.createStroke(pp).simplified();
     painter.fillPath(path, clrHalo);

     //painter.setPen(clrText);
     //painter.drawPath(pp);

     //painter.setPen(clrHalo);
     //painter.drawPath(path);

#else
     pps.setWidth(10);
     path = pps.createStroke(pp).united(pp).simplified();

     painter.fillPath(path, clrHalo);
     painter.setPen(clrHalo);
     painter.drawPath(path);
#endif



#if 0
     painter.fillPath(pp, clrText);
#else
     painter.setPen(clrText);
     painter.setFont(QFont("Arial",64));
     painter.drawText(100,100, QString("世界和平"));
#endif

}
