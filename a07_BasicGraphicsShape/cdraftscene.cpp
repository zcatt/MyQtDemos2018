#include <QtWidgets>

#include "mainwindow.h"
#include "cshapeitem.h"
#include "clinkitem.h"

#include "cdraftscene.h"

CDraftScene::CDraftScene(MainWindow *pMainWindow, QObject *parent)
        : QGraphicsScene(parent), m_pMainWindow(pMainWindow), m_nShapeMode(C2DItem::ShapeType_2D)
            , m_pLine(0)
{

}

void CDraftScene::mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
    qDebug()<< "scene mouse press, shapeType= "<<m_pMainWindow->GetCurrentShapeType();

    if (mouseEvent->button() != Qt::LeftButton)
        return;

    m_nShapeMode = m_pMainWindow->GetCurrentShapeType();
    if(m_nShapeMode != C2DItem::ShapeType_2D)
    {
        m_pMainWindow->ResetShapeType();

        switch(m_nShapeMode)
        {
        case CShapeItem::ShapeType_Triangle:
            {
                CTriangleItem *triangle = new CTriangleItem();
                addItem(triangle);
                triangle->setPos(mouseEvent->scenePos());
                emit itemInserted(triangle);
            }
            break;
        case CShapeItem::ShapeType_Link:
        {
            m_pLine = new CLinkItem;
            m_pLine->setLine(QLineF(mouseEvent->scenePos(), mouseEvent->scenePos()));
            addItem(m_pLine);
            emit itemInserted(m_pLine);
        }
            break;
        case CShapeItem::ShapeType_Circle:
        {
            QGraphicsEllipseItem *circle = new QGraphicsEllipseItem;
            circle->setFlag(QGraphicsItem::ItemIsMovable, true);
            circle->setFlag(QGraphicsItem::ItemIsSelectable, true);

            circle->setRect(-50,-50,50,50);
            addItem(circle);
            circle->setPos(mouseEvent->scenePos());
            emit itemInserted(circle);
        }
            break;
        case CShapeItem::ShapeType_Rectangle:
        {
            QGraphicsRectItem *rect = new QGraphicsRectItem;
            rect->setRect(-50,-50,50,50);
            rect->setFlag(QGraphicsItem::ItemIsMovable, true);
            rect->setFlag(QGraphicsItem::ItemIsSelectable, true);
            addItem(rect);
            rect->setPos(mouseEvent->scenePos());
            emit itemInserted(rect);
        }
            break;
        }
    }
#if 1
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
    if (m_nShapeMode == CShapeItem::ShapeType_Link && m_pLine != 0)
    {
        QLineF newLine(m_pLine->point(0), mouseEvent->scenePos());
        m_pLine->setLine(newLine);
    }
    else
    {
        //qDebug()<<"mouse move event, pos=" << mouseEvent->scenePos();
        QGraphicsScene::mouseMoveEvent(mouseEvent);
    }
}

void CDraftScene::mouseReleaseEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
    if (m_nShapeMode == CShapeItem::ShapeType_Link && m_pLine != 0)
    {
        m_pLine = 0;
    }

    QGraphicsScene::mouseReleaseEvent(mouseEvent);
}

