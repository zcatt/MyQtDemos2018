#include <QtWidgets>

#include "mainwindow.h"
#include "cshapeitem.h"
#include "clinkitem.h"

#include "cdraftscene.h"

CDraftScene::CDraftScene(MainWindow *pMainWindow, QObject *parent)
        : QGraphicsScene(parent), m_pMainWindow(pMainWindow), m_nShapeMode(CShapeItem::ShapeType_0)
            , m_pLine(0)
{

}

void CDraftScene::mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
    qDebug()<< "scene mouse press, shapeType= "<<m_pMainWindow->GetCurrentShapeType();

    if (mouseEvent->button() != Qt::LeftButton)
        return;

    m_nShapeMode = m_pMainWindow->GetCurrentShapeType();
    if(m_nShapeMode != CShapeItem::ShapeType_0)
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
        case CShapeItem::ShapeType_Circle:
        case CShapeItem::ShapeType_Rectangle:
        case CShapeItem::ShapeType_Link:
            m_pLine = new CLinkItem;
            m_pLine->setLine(QLineF(mouseEvent->scenePos(), mouseEvent->scenePos()));
            addItem(m_pLine);
            emit itemInserted(m_pLine);
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

