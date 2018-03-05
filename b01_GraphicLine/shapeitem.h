#ifndef SHAPEITEM_H
#define SHAPEITEM_H


#include <QGraphicsItem>
#include <QLineF>

class CDraftView;
class CShapeSelection;

template <class T> inline T cgraphicsitem_cast(QGraphicsItem *item)
{
    typedef typename std::remove_cv<typename std::remove_pointer<T>::type>::type Item;
    bool b1 = (int(Item::Type) == int(QGraphicsItem::Type));
    bool b2 = (item && int(Item::Type) == item->type());
    return (b1 || b2) ? static_cast<T>(item) : 0;
}


class C2DItem : public QGraphicsItem
{
public:
    enum ShapeType
    {
        ShapeType_None = 0,  //无效
        ShapeType_Select = ShapeType_None,
        ShapeType_2D,
            //ShapeType_Text,
            ShapeType_Link,
                ShapeType_Line,
            ShapeType_Shape,
                ShapeType_Rect,
                ShapeType_Text,
    };

    enum ShapeItemType
    {
        Type_2D = UserType + 1,
        Type_Line,
        Type_Shape,
        Type_Rect,
        Type_Text,
    };

public:
    static QPainterPath CreateLineOutlineFromPath(const QPainterPath &path, const QPen &pen);
    static QPainterPath CreateShapeOutlineFromPath(const QPainterPath &path, const QPen &pen);
    static void CreateHighlightSelected(C2DItem *item, QPainter *painter
                                        , const QStyleOptionGraphicsItem *option);

    C2DItem(QGraphicsItem *parent = Q_NULLPTR);

    QPen pen() const;
    virtual void setPen(const QPen &pen) = 0;

    bool contains(const QPointF &point) const override;
    bool isObscuredBy(const QGraphicsItem *item) const override;
    QPainterPath opaqueArea() const override;

    QPainterPath shape() const override;
    QRectF boundingRect() const override;

    enum { Type = Type_2D };
    int type() const override;

public:
    QPen m_pen;
    mutable QRectF m_rectBounding;      //in item coord
};


class CShapeItem : public C2DItem
{
public:
    enum
    {
        SelectBorderThickness = 6,     //选中提示边框的厚度， in QGraphicsView coord
    };

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
        NotSelectBorder = 0x01, //选中时不支持显示选中边框(和8个handles)， depend on GraphicsItemFlag::ItemIsSelectable
        NotSizable = 0x02,      //选中边框不支持显示8个handles，不支持大小调整, depend on SelectBorder
    };

    Q_DECLARE_FLAGS(ShapeItemFlags, ShapeItemFlag)


public:
    CShapeItem(QGraphicsItem *parent = Q_NULLPTR);
    ~CShapeItem();

    ShapeItemFlags shapeItemFlags() const;
    void setShapeItemFlag(ShapeItemFlag flag, bool enabled = true);
    void setShapeItemFlags(ShapeItemFlags flags);
    bool isTrackingBorder(void);

    virtual void setPen(const QPen &pen) override;

    virtual void setMinimalSize(const QSizeF& sizeMin);
    QSizeF minimalSize(void) const;


    CShapeSelection* getSelection(void);
    bool beginSelection(CDraftView *pView, QPoint ptView);
    bool endSelection(void);
    bool trackSelection(QPoint ptView);


    QRect viewBoundingRect(QGraphicsView *view);

    //size是新的boudingRect的实际大小，不含pen width.
    virtual void setBoundingRect(QSizeF size) = 0;
    virtual void updateBoundingRect(void) =0;

    QPainterPath shape() const override;

    enum { Type = Type_Shape };
    int type() const override;


protected:
    virtual void hoverEnterEvent(QGraphicsSceneHoverEvent *event) override;
    virtual void hoverMoveEvent(QGraphicsSceneHoverEvent *event) override;
    virtual void hoverLeaveEvent(QGraphicsSceneHoverEvent *event) override;

public:

    quint32 m_nShapeItemFlag;
    QSizeF m_sizeMin;


    CShapeSelection *m_pSelection;

    //以下变量用于实现handle框
    bool m_bTrackingBorder;    //true, 如果鼠标已经点压边框的8个handle，进入resize模式
    int m_nBorderHandleCode;    //m_bTrackingBorder为真时，点压的handle的BorderHandleCode
    QPointF m_ptPressed;   //点压时的位置, in scene coord
    QPointF m_ptTrackPos;   //当前鼠标的位置, in parent coord
    QRectF m_rcPressed;    //点压时的bounding rect, in item coord
};


class CRectItem : public CShapeItem
{

public:
    explicit CRectItem(QGraphicsItem *parent = Q_NULLPTR);
    explicit CRectItem(const QRectF &rc, QGraphicsItem *parent = Q_NULLPTR);
    explicit CRectItem(const QPointF &leftTop, const QPointF &rightBottom, QGraphicsItem *parent = Q_NULLPTR);
    ~CRectItem();

    QRectF rect() const;
    void setRect(QRectF &rc);

    //virtual void setTrackBorder(bool bTrack, int nBHCode, QPointF ptScene) override;
    virtual void setBoundingRect(QSizeF size) override;
    virtual void updateBoundingRect(void) override;

    //QPainterPath shape() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = Q_NULLPTR) override;

    enum { Type = Type_Rect };
    int type() const override;

protected:
    virtual void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    virtual void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;
    virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;

public:
    QRectF m_rc;
};


//用作名字标签时，依附于主item
//自动计算字串的roundingRect.
class CTextItem : public CShapeItem
{
public:
    explicit CTextItem(C2DItem *parent= Q_NULLPTR);

    void clear(void);
    QString text(void) const;
    void setText(const QString &text);

    //QMarginsF QLineEdit::textMargins() const;
    //void setTextMargins(const QMarginsF &margins);

    QRectF rect() const;

    virtual void setBoundingRect(QSizeF size) override;
    virtual void updateBoundingRect(void) override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = Q_NULLPTR) override;

    enum { Type = Type_Text };
    int type() const override;

signals:
    void textChanged(const QString &text);

public:
    QString m_Name;
    //QMarginsF m_margins;

};

#endif // SHAPEITEM_H
