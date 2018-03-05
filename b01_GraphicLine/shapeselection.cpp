#include "preheader.h"

#include "shapeselection.h"
#include "draftview.h"

CShapeSelection::CShapeSelection(CShapeItem *owner, CDraftView *view) : m_pOwner(owner), m_pView(view)
                        , m_nBorderHandleCode(CShapeItem::BHC_0), m_ptTrackPos(), m_rcPressed()
{
}

Qt::CursorShape CShapeSelection::borderCursor(CShapeItem::BoarderHandleCode nBHCode)
{
    Qt::CursorShape cursor = Qt::ArrowCursor;
    switch(nBHCode)
    {
    case CShapeItem::BHC_LeftTop:
    case CShapeItem::BHC_RightBottom:
        cursor = Qt::SizeFDiagCursor;
        break;
    case CShapeItem::BHC_RightTop:
    case CShapeItem::BHC_LeftBottom:
        cursor = Qt::SizeBDiagCursor;
        break;
    case CShapeItem::BHC_CenterTop:
    case CShapeItem::BHC_CenterBottom:
        cursor = Qt::SizeVerCursor;
        break;
    case CShapeItem::BHC_RightCenter:
    case CShapeItem::BHC_LeftCenter:
        cursor = Qt::SizeHorCursor;
        break;
    case CShapeItem::BHC_0:
    default:
        break;
    }
    return cursor;
}

//返回以rect为外沿的select border的8个handle rect code
//param
//  ptView, in, in view coord
//
CShapeItem::BoarderHandleCode CShapeSelection::posCode(QPoint ptView)
{
    if(m_pOwner->shapeItemFlags() &(CShapeItem::NotSelectBorder | CShapeItem::NotSizable))
        return CShapeItem::BHC_0;

    QRect rcBounding;           //in CDraftView coord
    rcBounding = m_pOwner->viewBoundingRect(m_pView);

    if(!rcBounding.contains(ptView))
        return CShapeItem::BHC_0;

    QRectF handleRect(0,0, CShapeItem::SelectBorderThickness, CShapeItem::SelectBorderThickness);

    qreal top,bottom,left,right;
    top = rcBounding.top();  bottom = rcBounding.bottom();
    left = rcBounding.left(); right = rcBounding.right();


    handleRect.moveTo(left, top);
    if(handleRect.contains(ptView))
        return CShapeItem::BHC_LeftTop;

    handleRect.moveTo(right - CShapeItem::SelectBorderThickness, top);
    if(handleRect.contains(ptView))
        return CShapeItem::BHC_RightTop;

    handleRect.moveTo(left, bottom - CShapeItem::SelectBorderThickness);
    if(handleRect.contains(ptView))
        return CShapeItem::BHC_LeftBottom;

    handleRect.moveTo(right - CShapeItem::SelectBorderThickness, bottom - CShapeItem::SelectBorderThickness);
    if(handleRect.contains(ptView))
        return CShapeItem::BHC_RightBottom;

    qreal center;
    center = (rcBounding.left() + rcBounding.right() - CShapeItem::SelectBorderThickness)/2.0;

    handleRect.moveTo(center, top);
    if(handleRect.contains(ptView))
        return CShapeItem::BHC_CenterTop;


    handleRect.moveTo(center, bottom - CShapeItem::SelectBorderThickness);
    if(handleRect.contains(ptView))
        return CShapeItem::BHC_CenterBottom;

    center = (rcBounding.top() + rcBounding.bottom() - CShapeItem::SelectBorderThickness)/2.0;

    handleRect.moveTo(left, center);
    if(handleRect.contains(ptView))
        return CShapeItem::BHC_LeftCenter;

    handleRect.moveTo(right - CShapeItem::SelectBorderThickness, center);
    if(handleRect.contains(ptView))
        return CShapeItem::BHC_RightCenter;

    return CShapeItem::BHC_0;
}


//return
//  true, begin tracking
//  false, fail to begin tracking.
bool CShapeSelection::beginTracking(CDraftView *pView, QPoint ptView)
{
    Q_ASSERT(m_pOwner != 0);
    Q_ASSERT(pView != 0);

    if(m_pOwner->shapeItemFlags() & (CShapeItem::NotSelectBorder | CShapeItem::NotSizable))
    {
        qDebug()<<"pass beginTracking";
        return false;
    }

    m_pView = pView;


    //QPoint pt = event->pos();   //in view coord

    CShapeItem::BoarderHandleCode nPosCode;
    nPosCode = posCode(ptView);
    if(nPosCode == CShapeItem::BHC_0)
    {
        // go through
    }
    else
    {       //处理拖动selectedBorder handles改变item大小
        m_pOwner->grabMouse();

        //mark
        //m_pOwner->setTrackBorder(true, nPosCode, mapToScene(pt));
        //m_bSetBorderCursor = true;
        m_pView->setCursor(borderCursor(nPosCode));

        m_nBorderHandleCode = posCode(ptView);
        m_ptPressed = m_pView->mapToScene(ptView);
        m_ptTrackPos = m_pOwner->pos();
        m_rcPressed = m_pOwner->boundingRect();

        qDebug()<<"CShapeSelection::beginTracking";
        return true;
    }
    return false;

}

void CShapeSelection::endTracking(void)
{
    Q_ASSERT(m_pOwner != 0);
    Q_ASSERT(m_pView != 0);

    m_pOwner->ungrabMouse();
    m_pView->unsetCursor();
}

void CShapeSelection::track(QPoint ptView)
{
    qDebug()<<"---------------track ptView="<<ptView;
    if(m_nBorderHandleCode == CShapeItem::BHC_0 || (m_pOwner->shapeItemFlags()&CShapeItem::NotSizable))
    {
        qDebug()<<"CShapeSelection::track  pass";
        return;
    }

    QPointF ptScene;
    ptScene = m_pView->mapToScene(ptView);

    QSizeF size;
    QPointF ptCur;

    QPointF deltaLocal = m_pOwner->mapFromScene(ptScene) - m_pOwner->mapFromScene(m_ptPressed);
    QPointF deltaParent;

    qDebug()<<"old bounding = "<<m_pOwner->boundingRect();

    switch(m_nBorderHandleCode)
    {

    case CShapeItem::BHC_RightBottom:
        // adjust boundingRect
        size = m_rcPressed.adjusted(0, 0, deltaLocal.x(), deltaLocal.y()).size();
        m_pOwner->setBoundingRect(size);

        // adjust pos to keep leftTop not to move
        deltaParent = m_pOwner->mapToParent(m_pOwner->boundingRect().topLeft()) - m_pOwner->mapToParent(m_rcPressed.topLeft());
        ptCur  = m_ptTrackPos - deltaParent;
        m_pOwner->setPos(ptCur);
        qDebug()<<"new bounding = "<<m_pOwner->boundingRect();
        break;

    case CShapeItem::BHC_LeftTop:
        size = m_rcPressed.adjusted(deltaLocal.x(), deltaLocal.y(), 0, 0).size();
        m_pOwner->setBoundingRect(size);

        deltaParent = m_pOwner->mapToParent(m_pOwner->boundingRect().bottomRight()) - m_pOwner->mapToParent(m_rcPressed.bottomRight());
        ptCur  = m_ptTrackPos - deltaParent;
        m_pOwner->setPos(ptCur);
        break;


    case CShapeItem::BHC_RightTop:
        size = m_rcPressed.adjusted(0, deltaLocal.y(), deltaLocal.x(), 0).size();
        m_pOwner->setBoundingRect(size);

        // adjust pos to keep leftTop not to move
        deltaParent = m_pOwner->mapToParent(m_pOwner->boundingRect().bottomLeft()) - m_pOwner->mapToParent(m_rcPressed.bottomLeft());
        ptCur  = m_ptTrackPos - deltaParent;
        m_pOwner->setPos(ptCur);
        break;

    case CShapeItem::BHC_LeftBottom:
        size = m_rcPressed.adjusted(deltaLocal.x(), 0, 0, deltaLocal.y()).size();
        m_pOwner->setBoundingRect(size);

        // adjust pos to keep leftTop not to move
        deltaParent = m_pOwner->mapToParent(m_pOwner->boundingRect().topRight()) - m_pOwner->mapToParent(m_rcPressed.topRight());
        ptCur  = m_ptTrackPos - deltaParent;
        m_pOwner->setPos(ptCur);
        break;

    case CShapeItem::BHC_CenterTop:
        size = m_rcPressed.adjusted(0, deltaLocal.y(), 0, 0).size();
        m_pOwner->setBoundingRect(size);

        deltaParent = m_pOwner->mapToParent(m_pOwner->boundingRect().bottomRight()) - m_pOwner->mapToParent(m_rcPressed.bottomRight());
        ptCur  = m_ptTrackPos;
        ptCur.ry() -= deltaParent.y();
        m_pOwner->setPos(ptCur);
        break;

    case CShapeItem::BHC_CenterBottom:
        size = m_rcPressed.adjusted(0, 0, 0, deltaLocal.y()).size();
        m_pOwner->setBoundingRect(size);

        deltaParent = m_pOwner->mapToParent(m_pOwner->boundingRect().topRight()) - m_pOwner->mapToParent(m_rcPressed.topRight());
        ptCur  = m_ptTrackPos;
        ptCur.ry() -= deltaParent.y();
        m_pOwner->setPos(ptCur);
        break;

    case CShapeItem::BHC_LeftCenter:
        size = m_rcPressed.adjusted(deltaLocal.x(), 0, 0, 0).size();
        m_pOwner->setBoundingRect(size);

        deltaParent = m_pOwner->mapToParent(m_pOwner->boundingRect().topRight()) - m_pOwner->mapToParent(m_rcPressed.topRight());
        ptCur  = m_ptTrackPos;
        ptCur.rx() -= deltaParent.x();
        m_pOwner->setPos(ptCur);
        break;

    case CShapeItem::BHC_RightCenter:
        size = m_rcPressed.adjusted(0, 0, deltaLocal.x(),0).size();
        m_pOwner->setBoundingRect(size);

        deltaParent = m_pOwner->mapToParent(m_pOwner->boundingRect().topLeft()) - m_pOwner->mapToParent(m_rcPressed.topLeft());
        ptCur  = m_ptTrackPos;
        ptCur.rx() -= deltaParent.x();
        m_pOwner->setPos(ptCur);
        break;
    case CShapeItem::BHC_0:
    default:
        Q_ASSERT(false);
        break;
    }
}

//param
//  painter, in,  view painter
void CShapeSelection::draw(QPainter *painter)
{
    QRect rcBounding;           //in CDraftView coord
    rcBounding = m_pOwner->viewBoundingRect(m_pView);

    painter->save();

    //border

    QBrush br(Qt::black, Qt::Dense4Pattern);
    QPen pen;
    pen.setBrush(br);
    pen.setWidth(CShapeItem::SelectBorderThickness);

    painter->setPen(pen);
    painter->setBrush(Qt::NoBrush);

    QRectF r = rcBounding;
    qreal delta = CShapeItem::SelectBorderThickness/2.0;
    r.adjust(delta, delta, -delta, -delta);

    painter->drawRect(r);

    if(!(m_pOwner->shapeItemFlags() &(CShapeItem::NotSizable)))
    {
        //8 rect handles
        painter->setPen(Qt::SolidLine);
        painter->setBrush(QBrush(Qt::white,Qt::SolidPattern));

        QRectF handleRect(0,0, CShapeItem::SelectBorderThickness, CShapeItem::SelectBorderThickness);

        qreal top,bottom,left,right;
        top = rcBounding.top();  bottom = rcBounding.bottom();
        left = rcBounding.left(); right = rcBounding.right();


        handleRect.moveTo(left, top);
        painter->drawRect(handleRect);

        handleRect.moveTo(right - CShapeItem::SelectBorderThickness, top);
        painter->drawRect(handleRect);

        handleRect.moveTo(left, bottom - CShapeItem::SelectBorderThickness);
        painter->drawRect(handleRect);

        handleRect.moveTo(right - CShapeItem::SelectBorderThickness, bottom - CShapeItem::SelectBorderThickness);
        painter->drawRect(handleRect);

        qreal center;
        center = (rcBounding.left() + rcBounding.right() - CShapeItem::SelectBorderThickness)/2.0;

        handleRect.moveTo(center, top);
        painter->drawRect(handleRect);

        handleRect.moveTo(center, bottom - CShapeItem::SelectBorderThickness);
        painter->drawRect(handleRect);

        center = (rcBounding.top() + rcBounding.bottom() - CShapeItem::SelectBorderThickness)/2.0;

        handleRect.moveTo(left, center);
        painter->drawRect(handleRect);

        handleRect.moveTo(right - CShapeItem::SelectBorderThickness, center);
        painter->drawRect(handleRect);

    }

    painter->restore();
}

//params
//  viewPainer, in,
//  rectScene,  in,  in scene coord
//
void CShapeSelection::drawSelection(QPainter *viewPainter, const QRectF &rectScene)
{
    Q_ASSERT(m_pOwner != 0);
    Q_ASSERT(m_pView != 0);

    //没有selection border
    if(m_pOwner->shapeItemFlags() &(CShapeItem::NotSelectBorder))
    {
        qDebug()<<"pass CShapeSelection::drawSelection";
        return;
    }

    QRectF rectItemScene = m_pOwner->sceneBoundingRect();

    //若不在rectScene中，则pass
    if(!rectItemScene.intersects(rectScene))
        return;

    draw(viewPainter);
}
