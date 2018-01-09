#include <QtWidgets>

#include "cshapeitem.h"
#include "cdraftview.h"


CDraftView::CDraftView(QWidget *parent): QGraphicsView(parent)
{
    //setDragMode(QGraphicsView::RubberBandDrag);
    //setDragMode(QGraphicsView::ScrollHandDrag);
    //setInteractive(false);
}

CDraftView::CDraftView(CDraftScene *scene, QWidget *parent): QGraphicsView(scene, parent)
{
//    setDragMode(QGraphicsView::RubberBandDrag);
    //setDragMode(QGraphicsView::ScrollHandDrag);
    //setInteractive(false);
}

CDraftView::~CDraftView()
{}

void CDraftView::drawSelectBorder(QPainter *painter, QGraphicsItem *item, QRect rectViewBounding)
{

    painter->save();

    //border

    QBrush br(Qt::black, Qt::Dense4Pattern);
    QPen pen;
    pen.setBrush(br);
    pen.setWidth(CShapeItem::SelectBorderThickness);

    painter->setPen(pen);
    painter->setBrush(Qt::NoBrush);

    QRectF r = rectViewBounding;
    qreal delta = CShapeItem::SelectBorderThickness/2.0;
    r.adjust(delta, delta, -delta, -delta);

    painter->drawRect(r);

    //8 rect
    painter->setPen(Qt::SolidLine);
    painter->setBrush(QBrush(Qt::white,Qt::SolidPattern));

    QRectF handleRect(0,0, CShapeItem::SelectBorderThickness, CShapeItem::SelectBorderThickness);

    qreal top,bottom,left,right;
    top = rectViewBounding.top();  bottom = rectViewBounding.bottom();
    left = rectViewBounding.left(); right = rectViewBounding.right();


    handleRect.moveTo(left, top);
    painter->drawRect(handleRect);

    handleRect.moveTo(right - CShapeItem::SelectBorderThickness, top);
    painter->drawRect(handleRect);

    handleRect.moveTo(left, bottom - CShapeItem::SelectBorderThickness);
    painter->drawRect(handleRect);

    handleRect.moveTo(right - CShapeItem::SelectBorderThickness, bottom - CShapeItem::SelectBorderThickness);
    painter->drawRect(handleRect);

    qreal center;
    center = (rectViewBounding.left() + rectViewBounding.right() - CShapeItem::SelectBorderThickness)/2.0;

    handleRect.moveTo(center, top);
    painter->drawRect(handleRect);

    handleRect.moveTo(center, bottom - CShapeItem::SelectBorderThickness);
    painter->drawRect(handleRect);

    center = (rectViewBounding.top() + rectViewBounding.bottom() - CShapeItem::SelectBorderThickness)/2.0;

    handleRect.moveTo(left, center);
    painter->drawRect(handleRect);

    handleRect.moveTo(right - CShapeItem::SelectBorderThickness, center);
    painter->drawRect(handleRect);



    painter->restore();
}


void CDraftView::paintEvent(QPaintEvent *event)
{
    //static int count = 0;
    //qDebug()<<"paint " <<count++<<" rect="<<event->rect();

    QGraphicsView::paintEvent(event);
}

void CDraftView::drawBackground(QPainter *painter, const QRectF &rect)
{
    QGraphicsView::drawBackground(painter, rect);
}


void CDraftView::drawForeground(QPainter *painter, const QRectF &rectScene)
{
    //draw the selected border of items.
#if 1
    const qreal opacity = painter->opacity();
    QTransform viewTransform = painter->worldTransform();

    QRectF rectItemScene;

//    QRectF rectScene;
//    rectScene = viewTransform.inverted().mapRect(rect);

    QList<QGraphicsItem *> actualItems;
    QList<QGraphicsItem *> selectedItems;

    selectedItems = scene()->selectedItems();

    if(selectedItems.count() > 0)
    {
        painter->resetTransform();
    }

    foreach(QGraphicsItem* item, selectedItems)
    {
        if(item->type() < C2DItem::Type_Shape)
            continue;
        //QRectF rectBounding = item->boundingRect();
        //rectBounding = item->mapRectToScene(rectBounding);
        rectItemScene = item->sceneBoundingRect();

        if(rectItemScene.intersects(rectScene))
        {
            CShapeItem *sitem;
            sitem = static_cast<CShapeItem*>(item);
            Q_ASSERT(sitem != 0);

            QRect rcBounding;
#if 0
            QPolygon polygon;
            polygon = mapFromScene(rectItemScene);
            rcBounding = polygon.boundingRect();
#endif
            rcBounding = sitem->viewBoundingRect(this);


            drawSelectBorder(painter, item, rcBounding);
        }
    }

    painter->setWorldTransform(viewTransform);
    painter->setOpacity(opacity);
#endif
    //super
    QGraphicsView::drawForeground(painter, rectScene);
}

//返回以rect为外延的select border的8个handle rect code
int CDraftView::posCode(QPoint &point, QRect &rect)
{
    if(!rect.contains(point))
        return CShapeItem::BHC_0;

    QRectF handleRect(0,0, CShapeItem::SelectBorderThickness, CShapeItem::SelectBorderThickness);

    qreal top,bottom,left,right;
    top = rect.top();  bottom = rect.bottom();
    left = rect.left(); right = rect.right();


    handleRect.moveTo(left, top);
    if(handleRect.contains(point))
        return CShapeItem::BHC_LeftTop;

    handleRect.moveTo(right - CShapeItem::SelectBorderThickness, top);
    if(handleRect.contains(point))
        return CShapeItem::BHC_RightTop;

    handleRect.moveTo(left, bottom - CShapeItem::SelectBorderThickness);
    if(handleRect.contains(point))
        return CShapeItem::BHC_LeftBottom;

    handleRect.moveTo(right - CShapeItem::SelectBorderThickness, bottom - CShapeItem::SelectBorderThickness);
    if(handleRect.contains(point))
        return CShapeItem::BHC_RightBottom;

    qreal center;
    center = (rect.left() + rect.right() - CShapeItem::SelectBorderThickness)/2.0;

    handleRect.moveTo(center, top);
    if(handleRect.contains(point))
        return CShapeItem::BHC_CenterTop;


    handleRect.moveTo(center, bottom - CShapeItem::SelectBorderThickness);
    if(handleRect.contains(point))
        return CShapeItem::BHC_CenterBottom;

    center = (rect.top() + rect.bottom() - CShapeItem::SelectBorderThickness)/2.0;

    handleRect.moveTo(left, center);
    if(handleRect.contains(point))
        return CShapeItem::BHC_LeftCenter;

    handleRect.moveTo(right - CShapeItem::SelectBorderThickness, center);
    if(handleRect.contains(point))
        return CShapeItem::BHC_RightCenter;

    return CShapeItem::BHC_0;
}

void CDraftView::mousePressEvent(QMouseEvent *event)
{
#if 1
    if(event->button() == Qt::LeftButton)
    {
        //满足下述条件，则是对selectedBorder进行操作
        // 1.仅有1个selected item
        // 2.鼠标位于selectedBoarder的8个handle block中
        QList<QGraphicsItem*> selectedItems;
        selectedItems = scene()->selectedItems();
        if(selectedItems.count()==1 && selectedItems.first()->type() > C2DItem::Type_Shape)
        {
            CShapeItem *sitem;
            sitem = static_cast<CShapeItem*>(selectedItems.first());

            QRect rcBoundingView;
            rcBoundingView = sitem->viewBoundingRect(this);

            QPoint pt = event->pos();

            int nPosCode;
            nPosCode = posCode(pt, rcBoundingView);
            if(nPosCode == CShapeItem::BHC_0)
            {
                // go through
            }
            else
            {       //处理拖动selectedBorder handles改变item大小
                sitem->grabMouse();
                //mark
                sitem->setTrackBorder(true, nPosCode, mapToScene(pt));
#if 1
                m_bSetBorderCursor = true;
                //sitem->setBorderCursor(nPosCode);
                setCursor(sitem->borderCursor(nPosCode));
#else
                sitem->setBorderCursor(nPosCode);
#endif
                return; //pass QGraphicsView::mousePressEvent()
            }

        }       //if(selectedItems.count()==1)

    }   // if (event->button() == Qt::LeftButton)

#endif

    QGraphicsView::mousePressEvent(event);
}

void CDraftView::mouseMoveEvent(QMouseEvent *event)
{
    qDebug()<< "CDraftView::mouseMoveEvent, pos="<< event->pos();

    //满足下述条件，则是对selectedBorder进行操作
    // 1.仅有1个selected item
    // 2.鼠标位于selectedBoarder的8个handle block中
    QList<QGraphicsItem*> selectedItems;
    selectedItems = scene()->selectedItems();
    if(selectedItems.count()==1 && selectedItems.first()->type() > C2DItem::Type_Shape)
    {
        CShapeItem *sitem;
        sitem = static_cast<CShapeItem*>(selectedItems.first());
        //仅在没有mousePress时设置cursor
        if(sitem->m_bTrackBorder)
        {
            //拖动selectedBorder中
            sitem->trackBorder(mapToScene(event->pos()));
            return; //pass QGraphicsView::mousePressEvent()
        }
        else
        {
            //非拖动中
            QRect rcBoundingView;
            rcBoundingView = sitem->viewBoundingRect(this);

            QPoint pt = event->pos();

            int nPosCode;
            nPosCode = posCode(pt, rcBoundingView);
            if(nPosCode == CShapeItem::BHC_0)
            {
                //复原cursor
                if(m_bSetBorderCursor)
                {
                    m_bSetBorderCursor = false;
                    unsetCursor();
                }
            }
            else
            {
                m_bSetBorderCursor = true;
                //sitem->setBorderCursor(nPosCode);
                setCursor(sitem->borderCursor(nPosCode));

                return; //pass QGraphicsView::mousePressEvent()
            }

        }                //if(!sitem->m_bTrackBorder)
    }       //if(selectedItems.count()==1)


    QGraphicsView::mouseMoveEvent(event);
}

void CDraftView::mouseReleaseEvent(QMouseEvent *event)
{
    qDebug()<< "CDraftView::mouseReleaseEvent, pos="<< event->pos();

    //满足下述条件，则是对selectedBorder进行操作
    // 1.仅有1个selected item
    // 2.鼠标位于selectedBoarder的8个handle block中
    QList<QGraphicsItem*> selectedItems;
    selectedItems = scene()->selectedItems();
    if(selectedItems.count()==1 && selectedItems.first()->type() > C2DItem::Type_Shape)
    {
        CShapeItem *sitem;
        sitem = static_cast<CShapeItem*>(selectedItems.first());

        //仅在没有mousePress时设置cursor
        if(sitem->m_bTrackBorder)
        {                //拖动selectedBorder handle中
            sitem->ungrabMouse();

            //mark
            QPoint pt = event->pos();
            sitem->setTrackBorder(false, CShapeItem::BHC_0, mapToScene(pt));

            //restore cursor
            if(m_bSetBorderCursor)
            {
                m_bSetBorderCursor = false;
                unsetCursor();
            }

            return; //pass QGraphicsView::mouseReleaseEvent()
        }
    }       //if(selectedItems.count()==1)



    QGraphicsView::mouseReleaseEvent(event);
}

