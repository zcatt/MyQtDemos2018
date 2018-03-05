#ifndef LINEITEM_H
#define LINEITEM_H

#include "shapeitem.h"

class CLineSelection;

//线段的端点，一个线通常有两个endpoint.
//
class CEndpoint
{
#if 1
public:
    enum EndShape           //端点的形状
    {
        EndShape_None= 0,     //无形状
        EndShape_Arrow,       //'个'形状

        EndShape_HollowCircle,
        EndShape_SolidCircle,

        EndShape_HollowTriangle,
        EndShape_SolidTriangle,

        EndShape_HollowDiamond,
        EndShape_SolidDiamond,

        EndShape_Custom,
    };
    enum
    {
        EndShapeWidth = 15,     //终端图形应当约束在width*width矩形内

    };

public:
    CEndpoint(EndShape nEndShape = EndShape_None, C2DItem *pOwner = 0);

    void setOwner(C2DItem *pOwner);
    void setEndShape(EndShape nEndShape);

    CTextItem *nameItem(void);
    void setNameItem(CTextItem *nameItem);

    CTextItem *descItem(void);
    void setDescItem(CTextItem *descItem);

    void setPoints(QPointF &from, QPointF &to);


    void drawNone(QPainter *painter);
    void drawArrow(QPainter *painter);
    void drawHollowCircle(QPainter *painter);
    void drawSolidCircle(QPainter *painter);
    void drawHollowTriangle(QPainter *painter);
    void drawSolidTriangle(QPainter *painter);
    void drawHollowDiamond(QPainter *painter);
    void drawSolidDiamond(QPainter *painter);
    virtual void draw(QPainter *painter, const QStyleOptionGraphicsItem *option);
    virtual QPainterPath shape(void) const;

public:
    C2DItem *m_pOwner;
    EndShape m_nEndShape;
    QLineF m_line;            //[0], from; [1], to

    //属性name
    CTextItem *m_pNameItem;

    //描述属性multiple
    CTextItem *m_pDescItem;

    //连接的item
    C2DItem *m_pAttached;

#endif
};



//特别说明，添加line时刻， lineItem的item coord的原点定位于(0,0) in scene coord， 故此时lineItem的坐标系等同于scene坐标。
//
class CLineItem : public C2DItem
{
public:
    enum
    {
        OutlineSpace = 10,      //轮廓到line的留白距离

        NameOffsetX = 0,       //name textItem到参考点的x偏移
        NameOffsetY = -10,       //name textItem到参考点的y偏移

        DescOffsetX = 0,       //desc textItem到参考点的x偏移
        DescOffsetY = 20,       //desc textItem到参考点的y偏移
    };
public:
    explicit CLineItem(QGraphicsItem *parent = Q_NULLPTR);
#if 0
    explicit CLineItem(const QLineF &line, QGraphicsItem *parent = Q_NULLPTR);
    explicit CLineItem(qreal x1, qreal y1, qreal x2, qreal y2, QGraphicsItem *parent = Q_NULLPTR);
#endif
    ~CLineItem();

    void setName(const QString& name);
    void setDesc(const QString& desc);

    QPointF calcTextBasePos(void);
    QPointF calcNamePos(void);
    QPointF calcDescPos(void);


    int pointCount(void) const;
    QPointF point(int index) const;
    void setPoint(int index, const QPointF &ptScene);
    void setLastPoint(const QPointF &ptScene);

    void appendPoint(const QPointF &ptScene);
    void insertPoint(int index, const QPointF &ptScene);
    void removePoint(int index);

    void updateEndpoints(int index = -1);

    QLineF line() const;
    void setLine(const QLineF &line);
    inline void setLine(qreal x1, qreal y1, qreal x2, qreal y2)
                { setLine(QLineF(x1, y1, x2, y2)); }


    virtual void setPen(const QPen &pen) override;
    virtual void updateBoundingRect(void);
    //QRectF boundingRect() const override;
    virtual QPainterPath shape() const override;
#if 0
    bool contains(const QPointF &point) const override;
#endif
    virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = Q_NULLPTR) override;
    virtual void keyPressEvent(QKeyEvent *event) override;


    enum{ Type = Type_Line};
    int type() const override;

protected:
    virtual QVariant itemChange(GraphicsItemChange change, const QVariant &value) override;

public:
    CLineSelection *m_pLineSelection;
    QVector<QPointF> m_points;  //in item coord.

    CTextItem *m_pNameItem;
    CTextItem *m_pDescItem;

    //线条的画笔信息保存在super::m_pen

    //两个端点, this是其owner
    CEndpoint m_endpoints[2];

};



//CLineItem选中时的装饰
class CLineSelection
{

};

#endif // LINEITEM_H
