#ifndef DRAFTSCENE_H
#define DRAFTSCENE_H

#include <QGraphicsScene>

class QObject;
class MainWindow;

class CLineItem;

class CDraftScene : public QGraphicsScene
{
    Q_OBJECT

public:
    CDraftScene(MainWindow *pMainWindow, QObject *parent = 0);

signals:
    void itemInserted(QGraphicsItem *item);
    void itemSelected(QGraphicsItem *item);

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent) override;
    void mouseMoveEvent(QGraphicsSceneMouseEvent *mouseEvent) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *mouseEvent) override;

public:
    MainWindow *m_pMainWindow;
    int m_nCurShapeType; //widget list中当前选定的新建shape类型

    CLineItem *m_pLineItem;

};

#endif // DRAFTSCENE_H
