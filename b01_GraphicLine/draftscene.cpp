#include "preheader.h"

#include "shapeitem.h"
#include "lineitem.h"
#include "draftscene.h"
#include "mainwindow.h"

CDraftScene::CDraftScene(MainWindow *pMainWindow, QObject *parent)
        : QGraphicsScene(parent), m_pMainWindow(pMainWindow)
            , m_nActionState(ActionState_None)
            , m_nCurShapeType(C2DItem::ShapeType_2D)
            , m_pLineItem(0)
{

}

CDraftScene::ActionState CDraftScene::actionState(void)
{
    return m_nActionState;
}

void CDraftScene::resetActionState(void)
{
    if(m_nActionState == ActionState_NewLine)
    {
        m_pLineItem = 0;
    }
    m_nActionState = ActionState_None;
}


void CDraftScene::setActionState(ActionState state)
{
    m_nActionState = state;
}


void CDraftScene::mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
    qDebug()<< "shapeType= "<<m_pMainWindow->GetCurrentShapeType();

    bool bDone = false;

    if (mouseEvent->button() == Qt::LeftButton)
    {
        if(m_nActionState == ActionState_None)
        {
            if(m_nCurShapeType != C2DItem::ShapeType_Select)
            {                                   //create new item
                switch(m_nCurShapeType)
                {
                case CShapeItem::ShapeType_Line:
                    {
                        qDebug()<<"new Line start";
                        bDone = true;
                        setActionState(ActionState_NewLine);
                        m_pLineItem = new CLineItem;
                        //m_pLineItem->setLine(QLineF(mouseEvent->scenePos(), mouseEvent->scenePos()));
                        m_pLineItem->appendPoint(mouseEvent->scenePos());
                        m_pLineItem->appendPoint(mouseEvent->scenePos());
                        m_pLineItem->setPos(QPointF(0,0));
                        connect(m_pLineItem, &C2DItem::selectedChange, this, &CDraftScene::itemSelected);

                        addItem(m_pLineItem);
                        emit itemInserted(m_pLineItem);
                        setFocusItem(m_pLineItem);

                    }
                    break;
                case CShapeItem::ShapeType_Rect:
                    {
                        qDebug()<<"add one rectItem.";
                        bDone = true;
                        setActionState(ActionState_NewRect);
                        CRectItem *rect = new CRectItem;
                        QRectF rc(-50,-50,100,100);
                        rect->setRect(rc);
                        //set in CRectItem. rect->setFlags(QGraphicsItem::ItemIsMovable | QGraphicsItem::ItemIsSelectable);
                        //rect->setFlag(QGraphicsItem::ItemIsMovable, true);
                        //rect->setFlag(QGraphicsItem::ItemIsSelectable, true);
                        connect(rect, &C2DItem::selectedChange, this, &CDraftScene::itemSelected);
                        connect(rect, &C2DItem::posChange, this, &CDraftScene::posChange);

                        addItem(rect);
                        rect->setPos(mouseEvent->scenePos());
                        emit itemInserted(rect);
                    }
                    break;
                case CShapeItem::ShapeType_Text:
                    {
                        qDebug()<<"add one textItem.";
                        bDone = true;
                        CTextItem *text = new CTextItem;
                        //text->setFlags(QGraphicsItem::ItemIsMovable | QGraphicsItem::ItemIsSelectable);
                        //rect->setFlag(QGraphicsItem::ItemIsMovable, true);
                        //rect->setFlag(QGraphicsItem::ItemIsSelectable, true);
                        text->setPos(mouseEvent->scenePos());
                        text->setText("Hello,the world.");
                        connect(text, &C2DItem::selectedChange, this, &CDraftScene::itemSelected);

                        addItem(text);
                        emit itemInserted(text);
                    }
                    break;
                }
            }               //if(m_nCurShapeType != C2DItem::ShapeType_Select)
            else
            {
                //编辑现有item
                //在item::mouseXXXEvent()中实现.
            }
        }
        else if(m_nActionState == ActionState_NewLine)
        {
            //新增一个端点
            qDebug()<<"new Line instert one point.";
            bDone = true;
            m_pLineItem->setLastPoint(mouseEvent->scenePos());
            m_pLineItem->appendPoint(mouseEvent->scenePos());
        }
    }
    else if (mouseEvent->button() == Qt::RightButton)
    {
        if(m_nActionState == ActionState_NewLine)
        {               //完成线段新增
            qDebug()<<"new Line over.";
            bDone = true;
            m_pLineItem->setLastPoint(mouseEvent->scenePos());
            m_pLineItem->setName(QString("Name"));
            m_pLineItem->setDesc(QString("Desc"));
            resetActionState();
        }
    }


    if(!bDone)
    {
        QGraphicsScene::mousePressEvent(mouseEvent);
    }


}


void CDraftScene::mouseMoveEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
#if 0
    qDebug()<<"pos="
           << mouseEvent->scenePos()
           << "actionStatus=" << m_nActionState;
#endif
    bool bDone = false;

    if(m_nActionState == ActionState_NewLine)
    {
        //qDebug()<<"m_pLineItem=" << m_pLineItem->m_points;
        bDone = true;
        //QLineF newLine(m_pLineItem->point(0), mouseEvent->scenePos());
        //m_pLineItem->setLine(newLine);
        m_pLineItem->setLastPoint(mouseEvent->scenePos());
    }

    if(!bDone)
    {
        QGraphicsScene::mouseMoveEvent(mouseEvent);
    }
}

void CDraftScene::mouseReleaseEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
    qDebug()<<"pos="
           << mouseEvent->scenePos()
           << "actionStatus=" << m_nActionState;

    bool bDone = false;

    if(m_nActionState == ActionState_NewLine)
    {
        //pass, nothing to do
        bDone = true;
    }

    if(!bDone)
    {
        m_nActionState = ActionState_None;
        QGraphicsScene::mouseReleaseEvent(mouseEvent);
    }
}

void CDraftScene::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
    qDebug()<<"pos="
           << mouseEvent->scenePos()
           << "actionStatus=" << m_nActionState;

    bool bDone = false;

    if(m_nActionState == ActionState_NewLine)
    {
        qDebug()<<"new Line over.";
        bDone = true;
        m_pLineItem->setLastPoint(mouseEvent->scenePos());
        resetActionState();
    }

    if(!bDone)
    {
        QGraphicsScene::mouseDoubleClickEvent(mouseEvent);
    }
}
