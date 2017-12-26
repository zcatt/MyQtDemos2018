#ifndef CSHAPEITEM_H
#define CSHAPEITEM_H

#include <QGraphicsItem>
#include <QLineF>

class CShapeItem : public QGraphicsItem
{
public:
    enum ShapeType
    {
        ShapeType_0,
        ShapeType_Line,         //TODO, rename to ShapeType_Link
        ShapeType_Triangle,
        ShapeType_Circle,
        ShapeType_Rectangle
    };

    enum ShapeItemType
    {
        LineType = UserType + 1,
        RectType,
        TriangleType,
        CircleType,
    };

public:
    CShapeItem(QGraphicsItem *parent = Q_NULLPTR);
    QPen pen() const;
    void setPen(const QPen &pen);



    static QPainterPath CreateShapeFromPath(const QPainterPath &path, const QPen &pen);
    static void CreateHighlightSelected(CShapeItem *item, QPainter *painter
                                        , const QStyleOptionGraphicsItem *option);

public:
    QPen m_pen;
    mutable QRectF m_rectBounding;
};

//TODO, rename to CLinkItem
//
class CLineItem : public CShapeItem
{
public:
    explicit CLineItem(QGraphicsItem *parent = Q_NULLPTR);
    explicit CLineItem(const QLineF &line, QGraphicsItem *parent = Q_NULLPTR);
    explicit CLineItem(qreal x1, qreal y1, qreal x2, qreal y2, QGraphicsItem *parent = Q_NULLPTR);
    ~CLineItem();


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
    bool contains(const QPointF &point) const override;

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = Q_NULLPTR) override;

    bool isObscuredBy(const QGraphicsItem *item) const override;
    QPainterPath opaqueArea() const override;

    int type() const override;

public:
    QVector<QPointF> m_points;

};

#endif // CSHAPEITEM_H
