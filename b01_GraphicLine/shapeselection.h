#ifndef SHAPESELECTION_H
#define SHAPESELECTION_H

#include <QPointF>
#include <QRectF>

#include "shapeitem.h"

class CDraftView;
class CShapeItem;

//负责处理CShapeItem obj的焦点外框，以及8个handle的交互
//需求包括
//.在QGraphicsView中绘制，以保证大小与scene的缩放比例无关
//.8个handle的拖拽
//.支持大小不可调整情况
class CShapeSelection
{
public:
    CShapeSelection(CShapeItem *owner, CDraftView *view = 0);
    static Qt::CursorShape borderCursor(CShapeItem::BoarderHandleCode nBHCode);
    CShapeItem::BoarderHandleCode posCode(QPoint ptView);

    bool beginTracking(CDraftView *pView, QPoint ptView);
    void endTracking(void);
    //bool isInTracking(void);
    void track(QPoint ptView);

    void draw(QPainter *viewPainter);
    void drawSelection(QPainter *viewPainter, const QRectF &rectScene);



public:
    CShapeItem *m_pOwner;
    CDraftView *m_pView;

    //以下变量用于实现handle框
    //bool m_bTracking;    //true, 如果鼠标已经点压边框的8个handle，进入resize模式
    CShapeItem::BoarderHandleCode m_nBorderHandleCode;    //m_bTrackingBorder为真时，点压的handle的BorderHandleCode
    QPointF m_ptPressed;   //点压时的位置, in scene coord
    QPointF m_ptTrackPos;   //点压时鼠标的位置, in parent coord
    QRectF m_rcPressed;    //点压时的bounding rect, in item coord

};


#endif // SHAPESELECTION_H
