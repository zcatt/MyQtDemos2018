#include <QtWidgets>

#include "shapeitem.h"

/*
 *
 *    C2DItem
 *
 */


//create shape path, = stroke path + origin path
//borrowed from qt src
QPainterPath C2DItem::CreateShapeFromPath(const QPainterPath &path, const QPen &pen)
{
    const qreal penWidthZero = qreal(0.00000001);

    if (path == QPainterPath() || pen == Qt::NoPen)
        return path;

    QPainterPathStroker ps;

    ps.setCapStyle(pen.capStyle());

    if (pen.widthF() <= 0.0)
        ps.setWidth(penWidthZero);
    else
        ps.setWidth(pen.widthF());

    ps.setJoinStyle(pen.joinStyle());
    ps.setMiterLimit(pen.miterLimit());

    QPainterPath p = ps.createStroke(path);

    p.addPath(path);

    return p;
}

//create the selected tip of bounding rect with dash line.
//borrowed from qt src
void C2DItem::CreateHighlightSelected(C2DItem *item, QPainter *painter
                                    , const QStyleOptionGraphicsItem *option)
{
    const QRectF murect = painter->transform().mapRect(QRectF(0, 0, 1, 1));
    if (qFuzzyIsNull(qMax(murect.width(), murect.height())))
        return;

    const QRectF mbrect = painter->transform().mapRect(item->boundingRect());
    if (qMin(mbrect.width(), mbrect.height()) < qreal(1.0))
        return;

    qreal itemPenWidth;
    if(item->type() >= Type_Shape)
    {
        itemPenWidth = item->pen().widthF();
    }
    else
        itemPenWidth = 1.0;

    const qreal pad = itemPenWidth / 2;

    const qreal penWidth = 0; // cosmetic pen

    const QColor fgcolor = option->palette.windowText().color();
    const QColor bgcolor( // ensure good contrast against fgcolor
        fgcolor.red()   > 127 ? 0 : 255,
        fgcolor.green() > 127 ? 0 : 255,
        fgcolor.blue()  > 127 ? 0 : 255);

    painter->setPen(QPen(bgcolor, penWidth, Qt::SolidLine));
    painter->setBrush(Qt::NoBrush);
    painter->drawRect(item->boundingRect().adjusted(pad, pad, -pad, -pad));

    painter->setPen(QPen(option->palette.windowText(), 0, Qt::DashLine));
    painter->setBrush(Qt::NoBrush);
    painter->drawRect(item->boundingRect().adjusted(pad, pad, -pad, -pad));
}

C2DItem::C2DItem(QGraphicsItem *parent)
        : QGraphicsItem(parent), m_pen(Qt::red, 1, Qt::SolidLine), m_rectBounding()
{
    setFlag(QGraphicsItem::ItemIsMovable, true);
    setFlag(QGraphicsItem::ItemIsSelectable, true);
    //setFlag(QGraphicsItem::ItemSendsGeometryChanges, true);
    //setFlag(QGraphicsItem::ItemClipsToShape, true);

    setAcceptHoverEvents(true);
}

int C2DItem::type() const
{
    return Type;
}



QPen C2DItem::pen() const
{
    return m_pen;
}


bool C2DItem::contains(const QPointF &point) const
{
    return QGraphicsItem::contains(point);
}

bool C2DItem::isObscuredBy(const QGraphicsItem *item) const
{
    return QGraphicsItem::isObscuredBy(item);
}

QPainterPath C2DItem::opaqueArea() const
{
    return QGraphicsItem::opaqueArea();
}


QPainterPath C2DItem::shape() const
{
    return QGraphicsItem::shape();
}


QRectF C2DItem::boundingRect() const
{
    return m_rectBounding;
}





/*
 *
 *    CShapeItem
 *
 */


CShapeItem::CShapeItem(QGraphicsItem *parent)
        : C2DItem(parent), m_nShapeItemFlag(0), m_sizeMin(SelectBorderThickness*3 +2, SelectBorderThickness*3 +2)
          , m_bTrackingBorder(false), m_nBorderHandleCode(BHC_0), m_ptPressed(), m_ptTrackPos(), m_rcPressed()
{
}


CShapeItem::ShapeItemFlags CShapeItem::shapeItemFlags() const
{
    return ShapeItemFlags(m_nShapeItemFlag);
}

void CShapeItem::setShapeItemFlag(ShapeItemFlag flag, bool enabled)
{
    if(enabled)
        m_nShapeItemFlag |= flag;
    else
        m_nShapeItemFlag &= ~flag;
}

void CShapeItem::setShapeItemFlags(ShapeItemFlags flags)
{
    m_nShapeItemFlag = flags;
}

int CShapeItem::type() const
{
    return Type;
}



void CShapeItem::setPen(const QPen &pen)
{
    if(m_pen == pen)
        return;

    prepareGeometryChange();
    m_pen = pen;
    updateBoundingRect();
    update();
}

void CShapeItem::setMinimalSize(const QSizeF& sizeMin)
{
    m_sizeMin = sizeMin;
}

QSizeF CShapeItem::minimalSize(void) const
{
    return m_sizeMin;
}

bool CShapeItem::isTrackingBorder(void)
{
    return m_bTrackingBorder;
}

//在view coord中item的boundingRect
QRect CShapeItem::viewBoundingRect(QGraphicsView *view)
{
    Q_ASSERT(view->scene() == scene());

    QRect rcBoundingInView;
    rcBoundingInView = view->mapFromScene(sceneBoundingRect()).boundingRect();

    return rcBoundingInView;
}


Qt::CursorShape CShapeItem::borderCursor(BoarderHandleCode nBHCode)
{
    Qt::CursorShape cursor = Qt::ArrowCursor;
    switch(nBHCode)
    {
    case CShapeItem::BHC_LeftTop:
    case CShapeItem::BHC_RightBottom:
        cursor = Qt::SizeFDiagCursor;
        break;
    case CShapeItem::BHC_RightTop:
    case CShapeItem::BHC_LeftBottom:
        cursor = Qt::SizeBDiagCursor;
        break;
    case CShapeItem::BHC_CenterTop:
    case CShapeItem::BHC_CenterBottom:
        cursor = Qt::SizeVerCursor;
        break;
    case CShapeItem::BHC_RightCenter:
    case CShapeItem::BHC_LeftCenter:
        cursor = Qt::SizeHorCursor;
        break;
    case CShapeItem::BHC_0:
    default:
        break;
    }
    return cursor;
}

void CShapeItem::setBorderCursor(BoarderHandleCode nBHCode)
{
    setCursor(borderCursor(nBHCode));
    return;
}

//鼠标在selectedBorder上情形，mousePress时设置bTrack=true并保存信息，
//或mouseRelease时设置bTrack=false
//
void CShapeItem::setTrackBorder(bool bTrack, BoarderHandleCode nBHCode, QPointF ptScene)
{
    m_bTrackingBorder = bTrack;
    if(bTrack)
    {
        m_nBorderHandleCode = nBHCode;
        m_ptPressed = ptScene;
        m_ptTrackPos = pos();
        m_rcPressed = m_rectBounding;
    }
}

//拖动边框过程中, 根据鼠标位置调整对象的大小和位置。
//params
// ptScene, in, in scene coord.
//
void CShapeItem::trackBorder(QPointF ptScene)
{
    if(!m_bTrackingBorder || m_nBorderHandleCode == BHC_0)
    {
        qDebug()<<"Warning! not in track status.";
        return;
    }

    QSizeF size;
    QPointF ptCur;

    QPointF deltaLocal = mapFromScene(ptScene) - mapFromScene(m_ptPressed);
    QPointF deltaParent;

    qDebug()<<"old bounding = "<<m_rectBounding;

    switch(m_nBorderHandleCode)
    {

    case BHC_RightBottom:
        // adjust boundingRect
        size = m_rcPressed.adjusted(0, 0, deltaLocal.x(), deltaLocal.y()).size();
        setBoundingRect(size);

        // adjust pos to keep leftTop not to move
        deltaParent = mapToParent(m_rectBounding.topLeft()) - mapToParent(m_rcPressed.topLeft());
        ptCur  = m_ptTrackPos - deltaParent;
        setPos(ptCur);
        qDebug()<<"new bounding = "<<m_rectBounding;
        break;

    case BHC_LeftTop:
        size = m_rcPressed.adjusted(deltaLocal.x(), deltaLocal.y(), 0, 0).size();
        setBoundingRect(size);

        deltaParent = mapToParent(m_rectBounding.bottomRight()) - mapToParent(m_rcPressed.bottomRight());
        ptCur  = m_ptTrackPos - deltaParent;
        setPos(ptCur);
        break;


    case BHC_RightTop:
        size = m_rcPressed.adjusted(0, deltaLocal.y(), deltaLocal.x(), 0).size();
        setBoundingRect(size);

        // adjust pos to keep leftTop not to move
        deltaParent = mapToParent(m_rectBounding.bottomLeft()) - mapToParent(m_rcPressed.bottomLeft());
        ptCur  = m_ptTrackPos - deltaParent;
        setPos(ptCur);
        break;

    case BHC_LeftBottom:
        size = m_rcPressed.adjusted(deltaLocal.x(), 0, 0, deltaLocal.y()).size();
        setBoundingRect(size);

        // adjust pos to keep leftTop not to move
        deltaParent = mapToParent(m_rectBounding.topRight()) - mapToParent(m_rcPressed.topRight());
        ptCur  = m_ptTrackPos - deltaParent;
        setPos(ptCur);
        break;

    case BHC_CenterTop:
        size = m_rcPressed.adjusted(0, deltaLocal.y(), 0, 0).size();
        setBoundingRect(size);

        deltaParent = mapToParent(m_rectBounding.bottomRight()) - mapToParent(m_rcPressed.bottomRight());
        ptCur  = m_ptTrackPos;
        ptCur.ry() -= deltaParent.y();
        setPos(ptCur);
        break;

    case BHC_CenterBottom:
        size = m_rcPressed.adjusted(0, 0, 0, deltaLocal.y()).size();
        setBoundingRect(size);

        deltaParent = mapToParent(m_rectBounding.topRight()) - mapToParent(m_rcPressed.topRight());
        ptCur  = m_ptTrackPos;
        ptCur.ry() -= deltaParent.y();
        setPos(ptCur);
        break;

    case BHC_LeftCenter:
        size = m_rcPressed.adjusted(deltaLocal.x(), 0, 0, 0).size();
        setBoundingRect(size);

        deltaParent = mapToParent(m_rectBounding.topRight()) - mapToParent(m_rcPressed.topRight());
        ptCur  = m_ptTrackPos;
        ptCur.rx() -= deltaParent.x();
        setPos(ptCur);
        break;

    case BHC_RightCenter:
        size = m_rcPressed.adjusted(0, 0, deltaLocal.x(),0).size();
        setBoundingRect(size);

        deltaParent = mapToParent(m_rectBounding.topLeft()) - mapToParent(m_rcPressed.topLeft());
        ptCur  = m_ptTrackPos;
        ptCur.rx() -= deltaParent.x();
        setPos(ptCur);
        break;
    }

}


QPainterPath CShapeItem::shape() const
{
    QPainterPath path;

#if 1
    path.addRect(m_rectBounding);
    return path;

#else
    path.addPolygon(m_polygon);
    return CShapeItem::CreateShapeFromPath(path, m_pen);

#endif
}

void CShapeItem::hoverEnterEvent(QGraphicsSceneHoverEvent *event)
{
    qDebug()<<"hover Enter";
}

void CShapeItem::hoverMoveEvent(QGraphicsSceneHoverEvent *event)
{
    qDebug()<<"hover move";

}

void CShapeItem::hoverLeaveEvent(QGraphicsSceneHoverEvent *event)
{
    qDebug()<<"hover Leave";

}




/*
 *
 *    CRectItem
 *
 */


CRectItem::CRectItem(QGraphicsItem *parent)
    :CShapeItem(parent), m_rc(-50,-50,100,100)
{
    setShapeItemFlag(SelectBorder);
}

CRectItem::CRectItem(const QRectF &rc, QGraphicsItem *parent) : CShapeItem(parent)
{
    setShapeItemFlag(SelectBorder);
    m_rc = rc;
}

CRectItem::CRectItem(const QPointF &leftTop, const QPointF &rightBottom, QGraphicsItem *parent)
    :CShapeItem(parent), m_rc(leftTop, rightBottom)
{
    setShapeItemFlag(SelectBorder);
}

CRectItem::~CRectItem()
{}

QRectF CRectItem::rect() const
{
    return m_rc;
}

void CRectItem::setRect(QRectF &rc)
{
    prepareGeometryChange();
    m_rc = rc;
    updateBoundingRect();
    update();
}



#if 0
QRectF CRectItem::boundingRect() const
{
    if(m_rectBounding.isNull())
    {
#if 1
        m_rectBounding = shape().controlPointRect();
        m_rectBounding.adjust(-CShapeItem::SelectBorderThickness, -CShapeItem::SelectBorderThickness
                              ,CShapeItem::SelectBorderThickness, CShapeItem::SelectBorderThickness);
        //m_rectBounding.adjust(-100,-100,100,100);
#else
        qreal pw = (m_pen.style() == Qt::NoPen) ? qreal(0) : pen().widthF();
        if (pw == 0.0)
        {
            m_rectBounding = m_polygon.boundingRect();
        }
        else
        {
            m_rectBounding = shape().controlPointRect();
        }
#endif
    }

    return m_rectBounding;
}

QPainterPath CRectItem::shape() const
{
    QPainterPath path;
    path.addPolygon(m_polygon);
    return CShapeItem::CreateShapeFromPath(path, m_pen);

}
#endif


void CRectItem::setBoundingRect(QSizeF size)
{
    if(size.width() < m_sizeMin.width())
        size.rwidth() = m_sizeMin.width();
    if(size.height() < m_sizeMin.height())
        size.rheight() = m_sizeMin.height();

    qDebug()<<"old rect: "<<m_rc;

    //基于item coord (0,0)的缩放
    QRectF rc(-size.width()/2, -size.height()/2, size.width(), size.height());
    m_rc = rc;

    qreal thickness = (pen().style() == Qt::NoPen) ? 0.0 :m_pen.widthF()/2.0;
    if (thickness > 0.0)
        m_rc.adjust(thickness, thickness, -thickness, -thickness);

    qDebug()<<"new rect: "<<m_rc;

    prepareGeometryChange();

    qDebug()<<"prev bounding= "<<m_rectBounding;
    updateBoundingRect();
    qDebug()<<"next bounding= "<<m_rectBounding;

    update();
}

void CRectItem::updateBoundingRect(void)
{
    qreal thickness = (pen().style() == Qt::NoPen) ? 0.0 :m_pen.widthF()/2.0;

    m_rectBounding = m_rc;
    if (thickness > 0.0)
        m_rectBounding.adjust(-thickness, -thickness, thickness, thickness);

    qDebug()<< "updateBoundingRect(), boundingRect="<< m_rectBounding;
}

#if 0
void CRectItem::drawSelectBorder(QPainter *painter, QRectF rect)
{
    painter->save();

    //border

    QBrush br(Qt::black, Qt::Dense4Pattern);
    QPen pen;
    pen.setBrush(br);
    pen.setWidth(SelectBorderThickness);

    painter->setPen(pen);
    painter->setBrush(Qt::NoBrush);

    QRectF r = rect;
    qreal delta = SelectBorderThickness/2.0;
    r.adjust(delta, delta, -delta, -delta);

    painter->drawRect(r);

    //8 rect
    painter->setPen(Qt::SolidLine);
    painter->setBrush(QBrush(Qt::white,Qt::SolidPattern));

    QRectF handleRect(0,0, SelectBorderThickness, SelectBorderThickness);

    qreal top,bottom,left,right;
    top = rect.top();  bottom = rect.bottom();
    left = rect.left(); right = rect.right();


    handleRect.moveTo(left, top);
    painter->drawRect(handleRect);

    handleRect.moveTo(right - SelectBorderThickness, top);
    painter->drawRect(handleRect);

    handleRect.moveTo(left, bottom -SelectBorderThickness);
    painter->drawRect(handleRect);

    handleRect.moveTo(right - SelectBorderThickness, bottom - SelectBorderThickness);
    painter->drawRect(handleRect);

    qreal center;
    center = (rect.left() + rect.right() - SelectBorderThickness)/2.0;

    handleRect.moveTo(center, top);
    painter->drawRect(handleRect);

    handleRect.moveTo(center, bottom - SelectBorderThickness);
    painter->drawRect(handleRect);

    center = (rect.top() + rect.bottom() - SelectBorderThickness)/2.0;

    handleRect.moveTo(left, center);
    painter->drawRect(handleRect);

    handleRect.moveTo(right - SelectBorderThickness, center);
    painter->drawRect(handleRect);



    painter->restore();
}
#endif

void CRectItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(widget);
    //painter->setRenderHint(QPainter::Antialiasing, true);

    qDebug()<<"clipPath= "<<painter->clipPath();
    qDebug()<<"clipRegion= "<<painter->clipRegion();


#if 1
    painter->setPen(Qt::red);
    QBrush brush = painter->brush();
    painter->setBrush(Qt::yellow);
    painter->drawPath(shape());
    painter->setBrush(brush);
#endif

#if 0
    painter->setPen(m_pen);
    painter->setBrush(Qt::red);

    painter->drawPolygon(m_polygon);
#endif

#if 0
    QPointF p;
    p = pos();
    painter->drawLine(p.x()-400, p.y(), p.x()+400, p.y());
    painter->drawLine(p.x(), p.y() -400, p.x(), p.y()+400);

#endif

    if (option->state & QStyle::State_Selected)
    {
#if 0
        drawSelectBorder(painter, boundingRect());
#else
        CShapeItem::CreateHighlightSelected(this, painter, option);
#endif
    }

}

int CRectItem::type() const
{
    return Type;
}


void CRectItem::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    qDebug()<<"CRectItem::mousePressEvent pos="<<event->scenePos();
    QGraphicsItem::mousePressEvent(event);
}
void CRectItem::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    qDebug()<<"CRectItem::mouseMoveEvent pos="<<event->scenePos();
    QGraphicsItem::mouseMoveEvent(event);
}
void CRectItem::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    qDebug()<<"CRectItem::mouseReleaseEvent pos="<<event->scenePos();
    QGraphicsItem::mouseReleaseEvent(event);
}








/*
 *
 *    CLinkItem
 *
 */


CLineItem::CLineItem(QGraphicsItem *parent)
        :C2DItem(parent), m_points(2)
{
    //setFlag(QGraphicsItem::ItemIsSelectable, false);
}

CLineItem::CLineItem(const QLineF &line, QGraphicsItem *parent)
        :C2DItem(parent), m_points(2)
{
    setLine(line);
}

CLineItem::CLineItem(qreal x1, qreal y1, qreal x2, qreal y2, QGraphicsItem *parent)
        :C2DItem(parent), m_points(2)
{
    setLine(x1,y1,x2,y2);
}

CLineItem::~CLineItem()
{
}


//特别说明，lineItem的item coord的原点定位于(0,0) in scene coord， 故lineItem的坐标系等同于sceneItem。
QVector<QPointF> CLineItem::points(void)
{
    return m_points;
}

void CLineItem::setPen(const QPen &pen)
{
    if(m_pen == pen)
        return;

    prepareGeometryChange();
    m_pen = pen;
    updateBoundingRect();
    update();
}


//特别说明，lineItem的item coord的原点定位于(0,0) in scene coord， 故lineItem的坐标系等同于sceneItem。
void CLineItem::updateBoundingRect(void)
{
    if(m_pen.style() == Qt::NoPen || m_pen.widthF() == 0.0)
    {
        const qreal x1 = m_points[0].x();
        const qreal x2 = m_points[1].x();
        const qreal y1 = m_points[0].y();
        const qreal y2 = m_points[1].y();
        qreal lx = qMin(x1, x2);
        qreal rx = qMax(x1, x2);
        qreal ty = qMin(y1, y2);
        qreal by = qMax(y1, y2);
        m_rectBounding.setRect(lx, ty, rx - lx, by - ty);
    }
    else
    {
        m_rectBounding = shape().controlPointRect();
    }
}

//特别说明，lineItem的item coord的原点定位于(0,0) in scene coord， 故lineItem的坐标系等同于sceneItem。
QPainterPath CLineItem::shape() const
{
    QPainterPath path;
    if (m_points.count() <2 || QLineF(m_points[0], m_points[1]) == QLineF())
        return path;

    path.moveTo(m_points[0]);
    for(int i = 1; i< m_points.count(); i++)
    {
        path.lineTo(m_points[i]);
    }

    QPen pen = m_pen;
    pen.setWidthF(10);
    return CreateShapeFromPath(path, pen);

}

int CLineItem::pointCount(void) const
{
    return m_points.count();
}

//特别说明，lineItem的item coord的原点定位于(0,0) in scene coord， 故lineItem的坐标系等同于sceneItem。
//return the point, in item coord
QPointF CLineItem::point(int index) const
{
    return m_points[index];
}

//特别说明，lineItem的item coord的原点定位于(0,0) in scene coord， 故lineItem的坐标系等同于sceneItem。
//param
// index, in,
// point, in, in item coord
void CLineItem::setPoint(int index, const QPointF &point)
{
    Q_ASSERT(index >= m_points.count());
    prepareGeometryChange();
    m_points[index] = point;
    updateBoundingRect();
    update();
}

//特别说明，lineItem的item coord的原点定位于(0,0) in scene coord， 故lineItem的坐标系等同于sceneItem。
//param
// index, in,
// point, in, in item coord
void CLineItem::insertPoint(int index, const QPointF &point)
{
    prepareGeometryChange();
    m_points.insert(index, point);
    updateBoundingRect();
    update();
}

void CLineItem::removePoint(int index)
{
    prepareGeometryChange();
    m_points.remove(index);
    updateBoundingRect();
    update();
}

//特别说明，lineItem的item coord的原点定位于(0,0) in scene coord， 故lineItem的坐标系等同于sceneItem。
//return the first lineF seg, in item coord
QLineF CLineItem::line() const
{
    return QLineF(m_points[0], m_points[1]);
}

//特别说明，lineItem的item coord的原点定位于(0,0) in scene coord， 故lineItem的坐标系等同于sceneItem。
//set the first lineF seg, in item coord
void CLineItem::setLine(const QLineF &line)
{
    prepareGeometryChange();

    m_points.resize(2);
    m_points[0] = line.p1();
    m_points[1] = line.p2();

    updateBoundingRect();
    update();
}


//特别说明，lineItem的item coord的原点定位于(0,0) in scene coord， 故lineItem的坐标系等同于sceneItem。
void CLineItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    qDebug()<<"paint lines:"<<m_points;
    //painter->setPen(QPen(Qt::red, 1, Qt::SolidLine));
    painter->setPen(m_pen);
    painter->drawLines(m_points);

    if (option->state & QStyle::State_Selected)
        CreateHighlightSelected(this, painter, option);
}

int CLineItem::type() const
{
    return Type;
}









