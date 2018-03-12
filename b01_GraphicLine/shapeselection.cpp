#include "preheader.h"

#include "shapeselection.h"
#include "draftview.h"



/*
 *
 *
 *      C2DSelection
 *
 *
 *      负责selection的显示和交互处理。 selection是item选中时显示的选中框和handle。
 *
*/
C2DSelection::C2DSelection(C2DItem *owner, CDraftView *view): m_pOwner(owner), m_pView(view), m_bSetCursor(false)
{
}

C2DSelection::~C2DSelection()
{
    unsetCursor();
}

CDraftView* C2DSelection::view(void)
{
    return m_pView;
}

C2DItem* C2DSelection::owner(void)
{
    return m_pOwner;
}

void C2DSelection::setCursor(const QCursor& cursor)
{
    m_bSetCursor = true;
    m_pView->setCursor(cursor);

}

void C2DSelection::unsetCursor(void)
{
    if(m_bSetCursor)
    {
        m_bSetCursor = false;
        if(m_pView != 0)
            m_pView->unsetCursor();
    }
}

//params
//  viewPainer, in,
//  rectScene,  in,  in scene coord
//
void C2DSelection::drawSelection(QPainter *viewPainter, const QRectF &rectScene)
{
    Q_ASSERT(m_pOwner != 0);
    Q_ASSERT(m_pView != 0);

    //没有selection border
    if(m_pOwner->itemFlags() &(C2DItem::NotSelectBorder))
    {
        qDebug()<<"pass";
        return;
    }

    QRectF rectItemScene = m_pOwner->sceneBoundingRect();

    //若不在rectScene中，则pass
    if(!rectItemScene.intersects(rectScene))
        return;

    draw(viewPainter);
}


/*
 *
 *
 *      CShapeSelection
 *
 *
 *      8个handle的外接矩形选中框
 *
*/

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

CShapeSelection::CShapeSelection(CShapeItem *owner, CDraftView *view) : C2DSelection(owner, view)
                        , m_nBorderHandleCode(CShapeItem::BHC_0)
                        , m_ptPressed()
                        , m_ptTrackPos()
                        , m_rcPressed()
{
}


//返回以rect为外沿的select border的8个handle rect code
//param
//  ptView, in, in view coord
//
CShapeItem::BoarderHandleCode CShapeSelection::posCode(QPoint ptView)
{
    if(m_pOwner->itemFlags() &(C2DItem::NotSelectBorder | C2DItem::NotSizable))
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

bool CShapeSelection::isInTracking(void)
{
    return (m_nBorderHandleCode != CShapeItem::BHC_0);
}

//return
//  true, begin tracking
//  false, fail to begin tracking.
bool CShapeSelection::beginTracking(QMouseEvent* mouseView)
{
    Q_ASSERT(m_pOwner != 0);
    Q_ASSERT(m_pView != 0);
    Q_ASSERT(m_nBorderHandleCode == CShapeItem::BHC_0);

    if(m_pOwner->itemFlags() & (C2DItem::NotSelectBorder | C2DItem::NotSizable))
    {
        qDebug()<<"pass ";
        return false;
    }

    QPoint ptView = mouseView->pos();   //in view coord

    CShapeItem::BoarderHandleCode nPosCode;
    nPosCode = posCode(ptView);
    if(nPosCode != CShapeItem::BHC_0)
    {
        //处理拖动selectedBorder handles改变item大小
        m_pOwner->grabMouse();

        //mark
        setCursor(borderCursor(nPosCode));

        m_nBorderHandleCode = nPosCode;
        m_ptPressed = m_pView->mapToScene(ptView);
        m_ptTrackPos = m_pOwner->pos();
        m_rcPressed = m_pOwner->boundingRect();

        qDebug()<<" ";
        return true;
    }
    return false;
}

//return
//  true, end selection
//  false, no selection
//
bool CShapeSelection::endTracking(void)
{
    Q_ASSERT(m_pOwner != 0);
    Q_ASSERT(m_pView != 0);

    bool bRes = false;
    if (isInTracking())
    {
        bRes = true;
        m_nBorderHandleCode = CShapeItem::BHC_0;

        unsetCursor();
        m_pOwner->ungrabMouse();
    }
    return bRes;
}

//return
//  true, track
//  false, no track
//
bool CShapeSelection::track(QMouseEvent* mouseView)
{
    CShapeItem *pOwner;
    pOwner = dynamic_cast<CShapeItem*>(m_pOwner);

    Q_ASSERT(pOwner != 0);
    Q_ASSERT(m_pView != 0);

    if(m_nBorderHandleCode == CShapeItem::BHC_0 || (pOwner->itemFlags()&C2DItem::NotSizable))
    {
        qDebug()<<"pass";
        return false;
    }

    QPoint ptView;
    ptView = mouseView->pos();
    qDebug()<<"---------------track ptView="<<ptView;

    QPointF ptScene;
    ptScene = m_pView->mapToScene(ptView);

    QSizeF size;
    QPointF ptCur;

    QPointF deltaLocal = pOwner->mapFromScene(ptScene) - pOwner->mapFromScene(m_ptPressed);
    QPointF deltaParent;

    qDebug()<<"old bounding = "<< pOwner->boundingRect();

    switch(m_nBorderHandleCode)
    {

    case CShapeItem::BHC_RightBottom:
        // adjust boundingRect
        size = m_rcPressed.adjusted(0, 0, deltaLocal.x(), deltaLocal.y()).size();
        pOwner->setBoundingRect(size);

        // adjust pos to keep leftTop not to move
        deltaParent = pOwner->mapToParent(pOwner->boundingRect().topLeft()) - pOwner->mapToParent(m_rcPressed.topLeft());
        ptCur  = m_ptTrackPos - deltaParent;
        pOwner->setPos(ptCur);
        qDebug()<<"new bounding = "<<pOwner->boundingRect();
        break;

    case CShapeItem::BHC_LeftTop:
        size = m_rcPressed.adjusted(deltaLocal.x(), deltaLocal.y(), 0, 0).size();
        pOwner->setBoundingRect(size);

        deltaParent = pOwner->mapToParent(pOwner->boundingRect().bottomRight()) - pOwner->mapToParent(m_rcPressed.bottomRight());
        ptCur  = m_ptTrackPos - deltaParent;
        pOwner->setPos(ptCur);
        break;


    case CShapeItem::BHC_RightTop:
        size = m_rcPressed.adjusted(0, deltaLocal.y(), deltaLocal.x(), 0).size();
        pOwner->setBoundingRect(size);

        // adjust pos to keep leftTop not to move
        deltaParent = pOwner->mapToParent(pOwner->boundingRect().bottomLeft()) - pOwner->mapToParent(m_rcPressed.bottomLeft());
        ptCur  = m_ptTrackPos - deltaParent;
        pOwner->setPos(ptCur);
        break;

    case CShapeItem::BHC_LeftBottom:
        size = m_rcPressed.adjusted(deltaLocal.x(), 0, 0, deltaLocal.y()).size();
        pOwner->setBoundingRect(size);

        // adjust pos to keep leftTop not to move
        deltaParent = pOwner->mapToParent(pOwner->boundingRect().topRight()) - pOwner->mapToParent(m_rcPressed.topRight());
        ptCur  = m_ptTrackPos - deltaParent;
        pOwner->setPos(ptCur);
        break;

    case CShapeItem::BHC_CenterTop:
        size = m_rcPressed.adjusted(0, deltaLocal.y(), 0, 0).size();
        pOwner->setBoundingRect(size);

        deltaParent = pOwner->mapToParent(pOwner->boundingRect().bottomRight()) - pOwner->mapToParent(m_rcPressed.bottomRight());
        ptCur  = m_ptTrackPos;
        ptCur.ry() -= deltaParent.y();
        pOwner->setPos(ptCur);
        break;

    case CShapeItem::BHC_CenterBottom:
        size = m_rcPressed.adjusted(0, 0, 0, deltaLocal.y()).size();
        pOwner->setBoundingRect(size);

        deltaParent = pOwner->mapToParent(pOwner->boundingRect().topRight()) - pOwner->mapToParent(m_rcPressed.topRight());
        ptCur  = m_ptTrackPos;
        ptCur.ry() -= deltaParent.y();
        pOwner->setPos(ptCur);
        break;

    case CShapeItem::BHC_LeftCenter:
        size = m_rcPressed.adjusted(deltaLocal.x(), 0, 0, 0).size();
        pOwner->setBoundingRect(size);

        deltaParent = pOwner->mapToParent(pOwner->boundingRect().topRight()) - pOwner->mapToParent(m_rcPressed.topRight());
        ptCur  = m_ptTrackPos;
        ptCur.rx() -= deltaParent.x();
        pOwner->setPos(ptCur);
        break;

    case CShapeItem::BHC_RightCenter:
        size = m_rcPressed.adjusted(0, 0, deltaLocal.x(),0).size();
        pOwner->setBoundingRect(size);

        deltaParent = pOwner->mapToParent(pOwner->boundingRect().topLeft()) - pOwner->mapToParent(m_rcPressed.topLeft());
        ptCur  = m_ptTrackPos;
        ptCur.rx() -= deltaParent.x();
        pOwner->setPos(ptCur);
        break;
    case CShapeItem::BHC_0:
    default:
        Q_ASSERT(false);
        break;
    }
    return true;
}

//param
//  painter, in,  view painter
void CShapeSelection::draw(QPainter *viewPainter)
{
    QRect rcBounding;           //in CDraftView coord
    rcBounding = m_pOwner->viewBoundingRect(m_pView);

    viewPainter->save();

    //border

    QBrush br(Qt::black, Qt::Dense4Pattern);
    QPen pen;
    pen.setBrush(br);
    pen.setWidth(C2DItem::SelectBorderThickness);

    viewPainter->setPen(pen);
    viewPainter->setBrush(Qt::NoBrush);

    QRectF r = rcBounding;
    qreal delta = C2DItem::SelectBorderThickness/2.0;
    r.adjust(delta, delta, -delta, -delta);

    viewPainter->drawRect(r);

    //8 rect handles
    if(!(m_pOwner->itemFlags() &(C2DItem::NotSizable)))
    {
        viewPainter->setPen(Qt::SolidLine);
        viewPainter->setBrush(QBrush(Qt::white,Qt::SolidPattern));

        QRectF handleRect(0,0, C2DItem::SelectBorderThickness, C2DItem::SelectBorderThickness);

        qreal top,bottom,left,right;
        top = rcBounding.top();  bottom = rcBounding.bottom();
        left = rcBounding.left(); right = rcBounding.right();


        handleRect.moveTo(left, top);
        viewPainter->drawRect(handleRect);

        handleRect.moveTo(right - C2DItem::SelectBorderThickness, top);
        viewPainter->drawRect(handleRect);

        handleRect.moveTo(left, bottom - C2DItem::SelectBorderThickness);
        viewPainter->drawRect(handleRect);

        handleRect.moveTo(right - C2DItem::SelectBorderThickness, bottom - C2DItem::SelectBorderThickness);
        viewPainter->drawRect(handleRect);

        qreal center;
        center = (rcBounding.left() + rcBounding.right() - C2DItem::SelectBorderThickness)/2.0;

        handleRect.moveTo(center, top);
        viewPainter->drawRect(handleRect);

        handleRect.moveTo(center, bottom - C2DItem::SelectBorderThickness);
        viewPainter->drawRect(handleRect);

        center = (rcBounding.top() + rcBounding.bottom() - C2DItem::SelectBorderThickness)/2.0;

        handleRect.moveTo(left, center);
        viewPainter->drawRect(handleRect);

        handleRect.moveTo(right - C2DItem::SelectBorderThickness, center);
        viewPainter->drawRect(handleRect);

    }

    viewPainter->restore();
}

