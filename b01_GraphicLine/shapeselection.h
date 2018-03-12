#ifndef SHAPESELECTION_H
#define SHAPESELECTION_H

#include <QPointF>
#include <QRectF>

#include "shapeitem.h"

class QMouseEvent;

class CDraftView;
class CShapeItem;


class C2DSelection
{
public:
    C2DSelection(C2DItem *owner, CDraftView *view);
    virtual ~C2DSelection();

    CDraftView* view(void);
    C2DItem* owner(void);

    void setCursor(const QCursor& cursor);
    void unsetCursor(void);

    virtual bool isInTracking(void) = 0;
    virtual bool beginTracking(QMouseEvent* mouseView) = 0;
    virtual bool endTracking(void) = 0;
    virtual bool track(QMouseEvent* mouseView) = 0;


    virtual void draw(QPainter *viewPainter)= 0;
    virtual void drawSelection(QPainter *viewPainter, const QRectF &rectScene);

public:
    C2DItem *m_pOwner;
    CDraftView *m_pView;

    bool m_bSetCursor;
};

//负责处理CShapeItem obj的焦点外框，以及8个handle的交互
//需求包括
//.在QGraphicsView中绘制，以保证大小与scene的缩放比例无关
//.8个handle的拖拽
//.支持大小不可调整情况
class CShapeSelection : public C2DSelection
{
public:
    static Qt::CursorShape borderCursor(CShapeItem::BoarderHandleCode nBHCode);

    CShapeSelection(CShapeItem *owner, CDraftView *view);

    CShapeItem::BoarderHandleCode posCode(QPoint ptView);

    virtual bool isInTracking(void) override;
    virtual bool beginTracking(QMouseEvent* mouseView) override;
    virtual bool endTracking(void) override;
    virtual bool track(QMouseEvent* mouseView) override;
    virtual void draw(QPainter *viewPainter) override;


public:
#if 0
    CShapeItem *m_pOwner;
    CDraftView *m_pView;
#endif

    //以下变量用于实现handle框
    //bool m_bTracking;    //true, 如果鼠标已经点压边框的8个handle，进入resize模式
    CShapeItem::BoarderHandleCode m_nBorderHandleCode;    //m_bTrackingBorder为真时，点压的handle的BorderHandleCode
    QPointF m_ptPressed;   //点压时的位置, in scene coord
    QPointF m_ptTrackPos;   //点压时鼠标的位置, in parent coord
    QRectF m_rcPressed;    //点压时的bounding rect, in item coord

};


#endif // SHAPESELECTION_H
