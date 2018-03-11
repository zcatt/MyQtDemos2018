#include "preheader.h"

#include "shapeitem.h"
#include "shapeselection.h"
#include "lineitem.h"

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

//    QRectF rectScene;
//    rectScene = viewTransform.inverted().mapRect(rect);

    QList<QGraphicsItem *> actualItems;
    QList<QGraphicsItem *> selectedItems;

    selectedItems = scene()->selectedItems();

    if(selectedItems.count() > 0)
    {
        //draw the selected border of items.
        const qreal opacity = painter->opacity();
        QTransform viewTransform = painter->worldTransform();

        //QRectF rectItemScene;

        painter->resetTransform();

        foreach(QGraphicsItem* item, selectedItems)
        {
            if(item->type() >= C2DItem::Type_Shape)
            {
                CShapeItem *sitem;
                sitem = dynamic_cast<CShapeItem*>(item);
                Q_ASSERT(sitem != 0);

                CShapeSelection *pSelection;
                pSelection = sitem->getSelection();
                if(pSelection)
                {
                    pSelection->drawSelection(painter, rectScene);
                }
                else
                {
                    //若没有进入到drag handle模式，则仅绘制selection border
                    CShapeSelection selection(sitem, this);
                    selection.drawSelection(painter, rectScene);
                }
            }                           //if(item->type() >= C2DItem::Type_Shape)
            else if (item->type() == C2DItem::Type_Line)
            {
                CLineItem *lineItem;
                lineItem = dynamic_cast<CLineItem*>(item);
                Q_ASSERT(lineItem != 0);

                CLineSelection *pSelection;
                pSelection = lineItem->getSelection();
                if(pSelection)
                {
                    pSelection->drawSelection(painter, rectScene);
                }
                else
                {
                    //若没有进入到drag handle模式，则仅绘制selection border
                    CLineSelection selection(lineItem, this);
                    selection.drawSelection(painter, rectScene);
                }
            }
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
    //mainWin->m_pGView->ensureVisible(QRect(),0,0);

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
            else if(selectedItems.first()->type() == C2DItem::Type_Line)
            {
                CLineItem *lineItem;
                lineItem = dynamic_cast<CLineItem*>(selectedItems.first());
                if(lineItem->beginSelection(this, event->pos()))
                {
                    return;
                }
            }
        }//if(selectedItems.count()==1)
    }

    qDebug()<<"default";
    QGraphicsView::mousePressEvent(event);

}


void CDraftView::mouseReleaseEvent(QMouseEvent *event)
{
    qDebug()<< "pos="<< event->pos();

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
        else if(selectedItems.first()->type() == C2DItem::Type_Line)
        {
            CLineItem *lineItem;
            lineItem = dynamic_cast<CLineItem*>(selectedItems.first());
            lineItem->trackSelection(event->pos());
            if(lineItem->endSelection())
            {
                return; //pass QGraphicsView::mouseReleaseEvent()
            }
        }

    } //if(selectedItems.count()==1)

    QGraphicsView::mouseReleaseEvent(event);
}

void CDraftView::mouseMoveEvent(QMouseEvent *event)
{
    //qDebug()<< "pos="<< event->pos();
    mainWin->showMousePos(event->pos());

    //满足下述条件，则是对selectedBorder进行操作
    // 1.仅有1个selected item, 且该item是shape类型。
    // 2.鼠标位于selectedBoarder的8个handle block中
    QList<QGraphicsItem*> selectedItems;
    selectedItems = scene()->selectedItems();
    if(selectedItems.count()==1)
    {
        //qDebug()<< "    type="<< selectedItems.first()->type();
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
        else if(selectedItems.first()->type() == C2DItem::Type_Line)
        {
            CLineItem *lineItem;
            lineItem = dynamic_cast<CLineItem*>(selectedItems.first());
            if(lineItem->trackSelection(event->pos()))
            {
                return; //pass QGraphicsView::mouseMoveEvent()
            }
            else
            {
                //非拖动中, 更新cursor
                CLineSelection selection(lineItem, this);
                CLineItem::LineHandleCode nPosCode;
                int nIndex;
                nPosCode = selection.posCode(event->pos(), nIndex);

                qDebug()<<"posCode="<<nPosCode;

                if(nPosCode == CLineItem::LHC_0)
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
                    setCursor(CLineSelection::handleCursor(nPosCode));

                    return; //pass QGraphicsView::mouseMoveEvent()
                }
            }
        }

    }       //if(selectedItems.count()==1)

    QGraphicsView::mouseMoveEvent(event);
}
