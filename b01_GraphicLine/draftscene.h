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
    enum ActionState        //状态机状态
    {
        ActionState_None,
        ActionState_NewLine,        //增添line
        //ActionState_EditLine,
        ActionState_NewRect,
        //ActionState_EditRect,
    };

public:
    CDraftScene(MainWindow *pMainWindow, QObject *parent = 0);

    ActionState actionState(void);
    void resetActionState(void);


signals:
    void itemInserted(QGraphicsItem *item);
    void itemSelected(QGraphicsItem *item);

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent) override;
    void mouseMoveEvent(QGraphicsSceneMouseEvent *mouseEvent) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *mouseEvent) override;
    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *mouseEvent) override;

public:
    MainWindow *m_pMainWindow;

    //TODO, 以下移动到eventFilter中
    ActionState m_nActionState;
    int m_nCurShapeType; //widget list中当前选定的新建shape类型
    CLineItem *m_pLineItem;

};

#endif // DRAFTSCENE_H
