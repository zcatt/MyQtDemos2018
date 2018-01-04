#ifndef CSHAPEITEM_H
#define CSHAPEITEM_H

#include <QGraphicsItem>
#include <QLineF>

class C2DItem : public QGraphicsItem
{
public:
    enum ShapeType
    {
        ShapeType_0,
        ShapeType_Link,
        ShapeType_Triangle,
        ShapeType_Circle,
        ShapeType_Rectangle,
        ShapeType_Text,
    };

    enum ShapeItemType
    {
        LinkType = UserType + 1,
        RectType,
        TriangleType,
        CircleType,
    };
public:
    static QPainterPath CreateShapeFromPath(const QPainterPath &path, const QPen &pen);
    static void CreateHighlightSelected(C2DItem *item, QPainter *painter
                                        , const QStyleOptionGraphicsItem *option);

    C2DItem(QGraphicsItem *parent = Q_NULLPTR);

    QPen pen() const;
    virtual void setPen(const QPen &pen) = 0;

    bool contains(const QPointF &point) const override;
    bool isObscuredBy(const QGraphicsItem *item) const override;
    QPainterPath opaqueArea() const override;

    QRectF boundingRect() const override;

public:
    QPen m_pen;
    mutable QRectF m_rectBounding;
};


class CShapeItem : public C2DItem
{
public:

    enum BoarderHandleCode
    {
        BHC_0,      //无效
        BHC_LeftTop,
        BHC_CenterTop,
        BHC_RightTop,
        BHC_LeftCenter,
        BHC_RightCenter,
        BHC_LeftBottom,
        BHC_CenterBottom,
        BHC_RightBottom
    };

    enum ShapeItemFlag
    {
        SelectBorder = 0x1,     //选中时显示选中边框和8个方块， depend on GraphicsItemFlag::ItemIsSelectable
        NotSizable = 0x02,      //选中边框不支持大小调整, depend on SelectBorder
    };

    Q_DECLARE_FLAGS(ShapeItemFlags, ShapeItemFlag)

    enum
    {
        SelectBorderThickness = 6,     //选中提示边框的厚度， in QGraphicsView coord
    };

public:
    CShapeItem(QGraphicsItem *parent = Q_NULLPTR);

    ShapeItemFlags shapeItemFlags() const;
    void setShapeItemFlag(ShapeItemFlag flag, bool enabled = true);
    void setShapeItemFlags(ShapeItemFlags flags);

    virtual void setPen(const QPen &pen) override;

    virtual void setMinimalSize(const QSizeF& sizeMin);
    QSizeF minimalSize(void) const;


    QRect viewBoundingRect(QGraphicsView *view);
    Qt::CursorShape borderCursor(int nBHCode);
    void setBorderCursor(int nBHCode);
    virtual void setTrackBorder(bool bTrack, int nBHCode, QPointF ptScene);
    virtual void trackBorder(QPointF ptScene);


    //size是新的boudingRect的实际大小，不含pen width.
    virtual void setBoundingRect(QSizeF size) = 0;
    virtual void updateBoundingRect(void) =0;

    QPainterPath shape() const override;



protected:
    virtual void hoverEnterEvent(QGraphicsSceneHoverEvent *event) override;
    virtual void hoverMoveEvent(QGraphicsSceneHoverEvent *event) override;
    virtual void hoverLeaveEvent(QGraphicsSceneHoverEvent *event) override;

public:
    quint32 m_nShapeItemFlag;
    QSizeF m_sizeMin;

    bool m_bTrackBorder;    //true, 如果鼠标已经点压边框的8个handle，进入resize模式
    int m_nBorderHandleCode;    //m_bTrackBorder为真时，点压的handle的BorderHandleCode
    QPointF m_ptTrackBorder;   //点压时的位置, in scene coord
    QPointF m_ptTrackPos;
    QRectF m_rcTrackingBounding;    //点压时的bounding rect, in item coord
};




class CTriangleItem : public CShapeItem
{
public:
    explicit CTriangleItem(QGraphicsItem *parent = Q_NULLPTR);
    //explicit CTriangleItem(const QPointF &center, QGraphicsItem *parent = Q_NULLPTR);
    explicit CTriangleItem(const QPointF &p1, const QPointF &p2, const QPointF &p3, QGraphicsItem *parent = Q_NULLPTR);
    ~CTriangleItem();


    QPointF point(int index) const;
    void setPoint(int index, const QPointF &point);
    void setPoints(const QPointF &p1, const QPointF &p2, const QPointF &p3);

    QPolygonF polygon() const { return m_polygon; }

    virtual void setTrackBorder(bool bTrack, int nBHCode, QPointF ptScene) override;
    virtual void setBoundingRect(QSizeF size) override;
    virtual void updateBoundingRect(void) override;

    void drawSelectBorder(QPainter *painter, QRectF rect);
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = Q_NULLPTR) override;
    int type() const override;

protected:
    virtual void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    virtual void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;
    virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;



public:
    QPolygonF m_polygon;
    QPolygonF m_polygonTrack;

};

#endif // CSHAPEITEM_H
