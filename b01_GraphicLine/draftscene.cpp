#include <QtWidgets>

#include "shapeitem.h"
#include "draftscene.h"
#include "mainwindow.h"

CDraftScene::CDraftScene(MainWindow *pMainWindow, QObject *parent)
        : QGraphicsScene(parent), m_pMainWindow(pMainWindow), m_nCurShapeType(C2DItem::ShapeType_2D)
            , m_pLineItem(0)
{

}

void CDraftScene::mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
    qDebug()<< "scene mouse press, shapeType= "<<m_pMainWindow->GetCurrentShapeType();
#if 1
    if (mouseEvent->button() != Qt::LeftButton)
        return;

    if(m_nCurShapeType != C2DItem::ShapeType_Select)
    {
        switch(m_nCurShapeType)
        {
        case CShapeItem::ShapeType_Line:
            {
                m_pLineItem = new CLineItem;
                qDebug()<<"m_pLineItem=" << m_pLineItem;
                m_pLineItem->setLine(QLineF(mouseEvent->scenePos(), mouseEvent->scenePos()));
                addItem(m_pLineItem);
                emit itemInserted(m_pLineItem);
            }
            break;
        case CShapeItem::ShapeType_Rect:
            {
                qDebug()<<"add one rectItem.";
                CRectItem *rect = new CRectItem;
                QRectF rc(-50,-50,100,100);
                rect->setRect(rc);
                rect->setFlags(QGraphicsItem::ItemIsMovable | QGraphicsItem::ItemIsSelectable);
                //rect->setFlag(QGraphicsItem::ItemIsMovable, true);
                //rect->setFlag(QGraphicsItem::ItemIsSelectable, true);
                addItem(rect);
                rect->setPos(mouseEvent->scenePos());
                emit itemInserted(rect);
            }
            break;
        }
    }
    else
    {
        QGraphicsScene::mousePressEvent(mouseEvent);
    }

#else
    QGraphicsScene::mousePressEvent(mouseEvent);
#endif
}

void CDraftScene::mouseMoveEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
    qDebug()<<"mouse move event, pos=" << mouseEvent->scenePos();
#if 1
    if (/*m_nCurShapeType == CShapeItem::ShapeType_Line && */m_pLineItem != 0)
    {
        qDebug()<<"m_pLineItem=" << m_pLineItem->m_points;
        QLineF newLine(m_pLineItem->point(0), mouseEvent->scenePos());
        m_pLineItem->setLine(newLine);
    }
    else
#endif
    {
        QGraphicsScene::mouseMoveEvent(mouseEvent);
    }
}

void CDraftScene::mouseReleaseEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
#if 1
    if (/*m_nCurShapeType == CShapeItem::ShapeType_Link && */m_pLineItem != 0)
    {
        m_pLineItem = 0;
    }
#endif
    QGraphicsScene::mouseReleaseEvent(mouseEvent);
}
