#include "preheader.h"

#include "shapeitem.h"
#include "shapeselection.h"
#include "draftview.h"
#include "mainwindow.h"

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

        foreach(QGraphicsItem* item, selectedItems)
        {
            if(item->type() >= C2DItem::Type_Shape)
            {
                CShapeItem *sitem;
                sitem = static_cast<CShapeItem*>(item);
                Q_ASSERT(sitem != 0);

                CShapeSelection *pSelection;
                pSelection = sitem->getSelection();
                if(pSelection)
                {
                    pSelection->drawSelection(painter, rectScene);
                }
                else
                {
                    CShapeSelection selection(sitem, this);
                    selection.drawSelection(painter, rectScene);
                }
            }                           //if(item->type() >= C2DItem::Type_Shape)
            else
            {
                //TODO
            }
        }

        painter->setWorldTransform(viewTransform);
        painter->setOpacity(opacity);
    }

    //super
    QGraphicsView::drawForeground(painter, rectScene);
}

void CDraftView::mousePressEvent(QMouseEvent *event)
{
    if(event->button() == Qt::LeftButton)
    {
        //满足下述条件，则是对selectedBorder进行操作
        // 1.仅有1个selected item, 且该item是shape类型。
        // 2.鼠标位于selectedBoarder的8个handle block中
        QList<QGraphicsItem*> selectedItems;
        selectedItems = scene()->selectedItems();
        if(selectedItems.count()==1)
        {
            if(selectedItems.first()->type() > C2DItem::Type_Shape)
            {
                CShapeItem *sitem;
                sitem = static_cast<CShapeItem*>(selectedItems.first());
                if(sitem->beginSelection(this, event->pos()))
                {
                    return;
                }
            }
            else if(selectedItems.first()->type() > C2DItem::Type_Line)
            {
                //TODO
            }
        }//if(selectedItems.count()==1)
    }

    qDebug()<<"CDraftView::mousePressEvent default";
    QGraphicsView::mousePressEvent(event);

}


void CDraftView::mouseReleaseEvent(QMouseEvent *event)
{
    qDebug()<< "CDraftView::mouseReleaseEvent, pos="<< event->pos();

    //满足下述条件，则是对selectedBorder进行操作
    // 1.仅有1个selected item
    // 2.鼠标位于selectedBoarder的8个handle block中
    QList<QGraphicsItem*> selectedItems;
    selectedItems = scene()->selectedItems();
    if(selectedItems.count()==1)
    {
        if(selectedItems.first()->type() > C2DItem::Type_Shape)
        {
            CShapeItem *sitem;
            sitem = static_cast<CShapeItem*>(selectedItems.first());
            sitem->trackSelection(event->pos());
            if(sitem->endSelection())
            {
                return; //pass QGraphicsView::mouseReleaseEvent()
            }
        }               //if(selectedItems.first()->type() > C2DItem::Type_Shape)
        else if(selectedItems.first()->type() > C2DItem::Type_Line)
        {
            //TODO
        }

    } //if(selectedItems.count()==1)

    QGraphicsView::mouseReleaseEvent(event);
}

void CDraftView::mouseMoveEvent(QMouseEvent *event)
{
    qDebug()<< "CDraftView::mouseMoveEvent, pos="<< event->pos();
    mainWin->showMousePos(event->pos());

    //满足下述条件，则是对selectedBorder进行操作
    // 1.仅有1个selected item, 且该item是shape类型。
    // 2.鼠标位于selectedBoarder的8个handle block中
    QList<QGraphicsItem*> selectedItems;
    selectedItems = scene()->selectedItems();
    if(selectedItems.count()==1)
    {
        qDebug()<< "CDraftView::mouseMoveEvent,    type="<< selectedItems.first()->type();
        if(selectedItems.first()->type() > C2DItem::Type_Shape)
        {
            CShapeItem *sitem;
            sitem = static_cast<CShapeItem*>(selectedItems.first());
            if(sitem->trackSelection(event->pos()))
            {
                return; //pass QGraphicsView::mouseMoveEvent()
            }
            else
            {
                //非拖动中, 更新cursor
                CShapeSelection selection(sitem, this);
                CShapeItem::BoarderHandleCode nPosCode;
                nPosCode = selection.posCode(event->pos());
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
                    setCursor(CShapeSelection::borderCursor(nPosCode));

                    return; //pass QGraphicsView::mouseMoveEvent()
                }
            }
        }                       //if(selectedItems.first()->type() > C2DItem::Type_Shape)
        else if(selectedItems.first()->type() > C2DItem::Type_Line)
        {
            //TODO
        }

    }       //if(selectedItems.count()==1)

    QGraphicsView::mouseMoveEvent(event);
}

#if 0
void CDraftView::mousePressEvent(QMouseEvent *event)
{
#if 1
    if(event->button() == Qt::LeftButton)
    {
        //满足下述条件，则是对selectedBorder进行操作
        // 1.仅有1个selected item, 且该item是shape类型。
        // 2.鼠标位于selectedBoarder的8个handle block中
        QList<QGraphicsItem*> selectedItems;
        selectedItems = scene()->selectedItems();
        if(selectedItems.count()==1)
        {
            if(selectedItems.first()->type() > C2DItem::Type_Shape)
            {
                CShapeItem *sitem;
                sitem = static_cast<CShapeItem*>(selectedItems.first());


                QRect rcBoundingView;
                rcBoundingView = sitem->viewBoundingRect(this);

                QPoint pt = event->pos();   //in view coord

                CShapeItem::BoarderHandleCode nPosCode;
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
                    qDebug()<<"CDraftView::mousePressEvent press handle";
                    return; //pass QGraphicsView::mousePressEvent()

                }
            }//if(selectedItems.first()->type() > C2DItem::Type_Shape)
            else if(selectedItems.first()->type() > C2DItem::Type_Line)
            {
                //TODO
            }
        }       //if(selectedItems.count()==1)

    }   // if (event->button() == Qt::LeftButton)

    qDebug()<<"CDraftView::mousePressEvent default";
    QGraphicsView::mousePressEvent(event);
#endif

}
#endif

#if 0
void CDraftView::mouseMoveEvent(QMouseEvent *event)
{
    qDebug()<< "CDraftView::mouseMoveEvent, pos="<< event->pos();

#if 1
    //满足下述条件，则是对selectedBorder进行操作
    // 1.仅有1个selected item, 且该item是shape类型。
    // 2.鼠标位于selectedBoarder的8个handle block中
    QList<QGraphicsItem*> selectedItems;
    selectedItems = scene()->selectedItems();
    if(selectedItems.count()==1)
    {
        if(selectedItems.first()->type() > C2DItem::Type_Shape)
        {
            CShapeItem *sitem;
            sitem = static_cast<CShapeItem*>(selectedItems.first());
            //仅在没有mousePress时设置cursor
            if(sitem->isTrackingBorder())
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

                CShapeItem::BoarderHandleCode nPosCode;
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
        }//if(selectedItems.first()->type() > C2DItem::Type_Shape)
        else if(selectedItems.first()->type() > C2DItem::Type_Line)
        {
            //TODO
        }

    }       //if(selectedItems.count()==1)

#endif
    QGraphicsView::mouseMoveEvent(event);
}
#endif

#if 0
void CDraftView::mouseReleaseEvent(QMouseEvent *event)
{
    qDebug()<< "CDraftView::mouseReleaseEvent, pos="<< event->pos();

    //满足下述条件，则是对selectedBorder进行操作
    // 1.仅有1个selected item
    // 2.鼠标位于selectedBoarder的8个handle block中
    QList<QGraphicsItem*> selectedItems;
    selectedItems = scene()->selectedItems();
    if(selectedItems.count()==1)
    {
        if(selectedItems.first()->type() > C2DItem::Type_Shape)
        {
            CShapeItem *sitem;
            sitem = static_cast<CShapeItem*>(selectedItems.first());

            //仅在没有mousePress时设置cursor
            if(sitem->m_bTrackingBorder)
            {                //拖动selectedBorder handle中
                sitem->ungrabMouse();

                //mark
                //QPoint pt = event->pos();
                //sitem->setTrackBorder(false, CShapeItem::BHC_0, mapToScene(pt));
                sitem->setTrackBorder(false);

                //restore cursor
                if(m_bSetBorderCursor)
                {
                    m_bSetBorderCursor = false;
                    unsetCursor();
                }

                return; //pass QGraphicsView::mouseReleaseEvent()
            }
        }//if(selectedItems.first()->type() > C2DItem::Type_Shape)
        else if(selectedItems.first()->type() > C2DItem::Type_Line)
        {
            //TODO
        }

    }       //if(selectedItems.count()==1)

    QGraphicsView::mouseReleaseEvent(event);
}

#endif
