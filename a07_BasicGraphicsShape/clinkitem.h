#ifndef CLINKITEM_H
#define CLINKITEM_H


#include "cshapeitem.h"

class CLinkItem : public C2DItem
{
public:
    explicit CLinkItem(QGraphicsItem *parent = Q_NULLPTR);
    explicit CLinkItem(const QLineF &line, QGraphicsItem *parent = Q_NULLPTR);
    explicit CLinkItem(qreal x1, qreal y1, qreal x2, qreal y2, QGraphicsItem *parent = Q_NULLPTR);
    ~CLinkItem();

    virtual void setPen(const QPen &pen) override;

    int pointCount(void) const;
    QPointF point(int index) const;
    void setPoint(int index, const QPointF &point);

    void insertPoint(int index, const QPointF &point);
    void removePoint(int index);

    QLineF line() const;
    void setLine(const QLineF &line);
    inline void setLine(qreal x1, qreal y1, qreal x2, qreal y2)
                { setLine(QLineF(x1, y1, x2, y2)); }

    QRectF boundingRect() const override;
    QPainterPath shape() const override;

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = Q_NULLPTR) override;


    int type() const override;

public:
    QVector<QPointF> m_points;
};

#endif // CLINKITEM_H
