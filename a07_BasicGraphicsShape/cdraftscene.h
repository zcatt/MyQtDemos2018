#ifndef CDRAFTSCENE_H
#define CDRAFTSCENE_H

#include <QGraphicsScene>

#include "mainwindow.h"

class CShapeItem;
class CLineItem;

class CDraftScene : public QGraphicsScene
{
    Q_OBJECT
public:
    CDraftScene(MainWindow *pMainWindow, QObject *parent = 0);

signals:
    void itemInserted(CShapeItem *item);
    void itemSelected(CShapeItem *item);

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent) override;
    void mouseMoveEvent(QGraphicsSceneMouseEvent *mouseEvent) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *mouseEvent) override;

public:
    MainWindow *m_pMainWindow;
    int m_nShapeMode; //ShapeType

    CLineItem *m_pLine;
};

#endif // CDRAFTSCENE_H
