#ifndef DRAFTVIEW_H
#define DRAFTVIEW_H

#include <QGraphicsView>

#include "shapeitem.h"
#include "draftscene.h"

class CDraftView : public QGraphicsView
{
    Q_OBJECT

public:
    CDraftView(QWidget *parent = Q_NULLPTR);
    CDraftView(CDraftScene *scene, QWidget *parent = Q_NULLPTR);
    ~CDraftView();

protected:
    void paintEvent(QPaintEvent *event) override;
    void drawBackground(QPainter *painter, const QRectF &rect) override;
    void drawForeground(QPainter *painter, const QRectF &rect) override;

    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;

public:
    bool m_bSetBorderCursor;        //TRUE, 则意味鼠标在selectedBorder上面设置了cursor。
};

#endif // DRAFTVIEW_H
