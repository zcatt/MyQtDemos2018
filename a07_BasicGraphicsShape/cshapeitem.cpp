#include <QtWidgets>

#include "cshapeitem.h"

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
    if(item->type() >= Type_Link)
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
        : QGraphicsItem(parent), m_pen(), m_rectBounding()
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
          , m_bTrackBorder(false), m_nBorderHandleCode(BHC_0), m_ptTrackBorder()
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

//在view coord中item的boundingRect
QRect CShapeItem::viewBoundingRect(QGraphicsView *view)
{
    Q_ASSERT(view->scene() == scene());

    QRect rcBoundingInView;
    rcBoundingInView = view->mapFromScene(sceneBoundingRect()).boundingRect();

    return rcBoundingInView;
}


Qt::CursorShape CShapeItem::borderCursor(int nBHCode)
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

void CShapeItem::setBorderCursor(int nBHCode)
{
    setCursor(borderCursor(nBHCode));
    return;
}

//鼠标在selectedBorder上情形，mousePress时设置bTrack=true并保存信息，
//或mouseRelease时设置bTrack=false
//
void CShapeItem::setTrackBorder(bool bTrack, int nBHCode, QPointF ptScene)
{
    m_bTrackBorder = bTrack;
    if(bTrack)
    {
        m_nBorderHandleCode = nBHCode;
        m_ptTrackBorder = ptScene;
        m_ptTrackPos = pos();
        m_rcTrackingBounding = m_rectBounding;
    }
}

void CTriangleItem::setTrackBorder(bool bTrack, int nBHCode, QPointF ptScene)
{
    if(bTrack)
    {
        m_polygonTrack = m_polygon;
    }
    CShapeItem::setTrackBorder(bTrack, nBHCode, ptScene);
}


void CShapeItem::trackBorder(QPointF ptScene)
{
    if(!m_bTrackBorder || m_nBorderHandleCode == BHC_0)
    {
        qDebug()<<"Warning! not in track status.";
        return;
    }

    QSizeF size;
    QPointF ptCur;

    QPointF deltaLocal = mapFromScene(ptScene) - mapFromScene(m_ptTrackBorder);
    QPointF deltaParent;

    qDebug()<<"old bounding = "<<m_rectBounding;

    switch(m_nBorderHandleCode)
    {

    case BHC_RightBottom:
        // adjust boundingRect
        size = m_rcTrackingBounding.adjusted(0, 0, deltaLocal.x(), deltaLocal.y()).size();
        setBoundingRect(size);

        // adjust pos to keep leftTop not to move
        deltaParent = mapToParent(m_rectBounding.topLeft()) - mapToParent(m_rcTrackingBounding.topLeft());
        ptCur  = m_ptTrackPos - deltaParent;
        setPos(ptCur);
        qDebug()<<"new bounding = "<<m_rectBounding;
        break;

    case BHC_LeftTop:
        size = m_rcTrackingBounding.adjusted(deltaLocal.x(), deltaLocal.y(), 0, 0).size();
        setBoundingRect(size);

        deltaParent = mapToParent(m_rectBounding.bottomRight()) - mapToParent(m_rcTrackingBounding.bottomRight());
        ptCur  = m_ptTrackPos - deltaParent;
        setPos(ptCur);
        break;


    case BHC_RightTop:
        size = m_rcTrackingBounding.adjusted(0, deltaLocal.y(), deltaLocal.x(), 0).size();
        setBoundingRect(size);

        // adjust pos to keep leftTop not to move
        deltaParent = mapToParent(m_rectBounding.bottomLeft()) - mapToParent(m_rcTrackingBounding.bottomLeft());
        ptCur  = m_ptTrackPos - deltaParent;
        setPos(ptCur);
        break;

    case BHC_LeftBottom:
        size = m_rcTrackingBounding.adjusted(deltaLocal.x(), 0, 0, deltaLocal.y()).size();
        setBoundingRect(size);

        // adjust pos to keep leftTop not to move
        deltaParent = mapToParent(m_rectBounding.topRight()) - mapToParent(m_rcTrackingBounding.topRight());
        ptCur  = m_ptTrackPos - deltaParent;
        setPos(ptCur);
        break;

    case BHC_CenterTop:
        size = m_rcTrackingBounding.adjusted(0, deltaLocal.y(), 0, 0).size();
        setBoundingRect(size);

        deltaParent = mapToParent(m_rectBounding.bottomRight()) - mapToParent(m_rcTrackingBounding.bottomRight());
        ptCur  = m_ptTrackPos;
        ptCur.ry() -= deltaParent.y();
        setPos(ptCur);
        break;

    case BHC_CenterBottom:
        size = m_rcTrackingBounding.adjusted(0, 0, 0, deltaLocal.y()).size();
        setBoundingRect(size);

        deltaParent = mapToParent(m_rectBounding.topRight()) - mapToParent(m_rcTrackingBounding.topRight());
        ptCur  = m_ptTrackPos;
        ptCur.ry() -= deltaParent.y();
        setPos(ptCur);
        break;

    case BHC_LeftCenter:
        size = m_rcTrackingBounding.adjusted(deltaLocal.x(), 0, 0, 0).size();
        setBoundingRect(size);

        deltaParent = mapToParent(m_rectBounding.topRight()) - mapToParent(m_rcTrackingBounding.topRight());
        ptCur  = m_ptTrackPos;
        ptCur.rx() -= deltaParent.x();
        setPos(ptCur);
        break;

    case BHC_RightCenter:
        size = m_rcTrackingBounding.adjusted(0, 0, deltaLocal.x(),0).size();
        setBoundingRect(size);

        deltaParent = mapToParent(m_rectBounding.topLeft()) - mapToParent(m_rcTrackingBounding.topLeft());
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
 *    CTriangleItem
 *
 */


CTriangleItem::CTriangleItem(QGraphicsItem *parent)
    :CShapeItem(parent), m_polygon(3)
{
    setShapeItemFlag(SelectBorder);
    setPoints(QPointF(-50.,28.8),QPointF(50.,28.8),QPointF(0.,-57.6));
}

#if 0
CTriangleItem::CTriangleItem(const QPointF &center, QGraphicsItem *parent)
    :CShapeItem(parent), m_polygon(3)
{
    setShapeItemFlag(SelectBorder);

    QPointF p1(-50.,28.8);
    QPointF p2(50.,28.8);
    QPointF p3(0.,-57.6);

    p1+=center;
    p2+=center;
    p3+=center;

    setPoints(p1,p2,p3);
}
#endif

CTriangleItem::CTriangleItem(const QPointF &p1, const QPointF &p2, const QPointF &p3, QGraphicsItem *parent)
    :CShapeItem(parent), m_polygon(3)
{
    setShapeItemFlag(SelectBorder);
    setPoints(p1,p2,p3);
}

CTriangleItem::~CTriangleItem()
{}


QPointF CTriangleItem::point(int index) const
{
    return m_polygon.value(index);
}

void CTriangleItem::setPoint(int index, const QPointF &point)
{
    if(index <0 || index > 2)
        return;

    prepareGeometryChange();

    m_polygon[index] = point;
    updateBoundingRect();

    update();
}

void CTriangleItem::setPoints(const QPointF &p1, const QPointF &p2, const QPointF &p3)
{
    prepareGeometryChange();

    m_polygon[0] = p1;
    m_polygon[1] = p2;
    m_polygon[2] = p3;

    updateBoundingRect();
    update();
}
#if 0
QPointF CTriangleItem::center(void) const
{
    QPointF p;
    p = m_polygon[0];
    p += m_polygon[1];
    p += m_polygon[2];
    p /= 3.0;
    return p;
}

void CTriangleItem::setCenter(QPointF& point)
{
    QPointF p;
    p = center();
    p -= point;

    m_polygon[0] -= p;
    m_polygon[1] -= p;
    m_polygon[2] -= p;
}
#endif

#if 0
QRectF CTriangleItem::boundingRect() const
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

QPainterPath CTriangleItem::shape() const
{
    QPainterPath path;
    path.addPolygon(m_polygon);
    return CShapeItem::CreateShapeFromPath(path, m_pen);

}
#endif


void CTriangleItem::setBoundingRect(QSizeF size)
{
    //基于item coord (0,0)的缩放
    QRectF rcBoundingOld = m_rectBounding;

    qreal thickness = (pen().style() == Qt::NoPen) ? 0.0 :m_pen.widthF()/2.0;
    if (thickness > 0.0)
        rcBoundingOld.adjust(thickness, thickness, -thickness, -thickness);

    //
    if(size.width() < m_sizeMin.width())
        size.rwidth() = m_sizeMin.width();
    if(size.height() < m_sizeMin.height())
        size.rheight() = m_sizeMin.height();

    qreal nRatioX, nRatioY;
    nRatioX = (size.width() - 2*thickness) / rcBoundingOld.width();
    nRatioY = (size.height() - 2*thickness) / rcBoundingOld.height();

    qDebug()<<"old points: "<<m_polygon;
    for(QPolygonF::Iterator it=m_polygon.begin(); it != m_polygon.end(); it++)
    {
        it->rx() = it->x() * nRatioX;
        it->ry() = it->y() * nRatioY;
    }
    qDebug()<<"new points: "<<m_polygon;

    prepareGeometryChange();
    qDebug()<<"prev bounding= "<<m_rectBounding;
    updateBoundingRect();
    qDebug()<<"next bounding= "<<m_rectBounding;
    update();
}

void CTriangleItem::updateBoundingRect(void)
{
    qreal thickness = (pen().style() == Qt::NoPen) ? 0.0 :m_pen.widthF()/2.0;

    m_rectBounding = m_polygon.boundingRect();
    if (thickness > 0.0)
        m_rectBounding.adjust(-thickness, -thickness, thickness, thickness);

    qDebug()<< "updateBoundingRect(), boundingRect="<< m_rectBounding;
}

#if 0
void CTriangleItem::drawSelectBorder(QPainter *painter, QRectF rect)
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

void CTriangleItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(widget);
    //painter->setRenderHint(QPainter::Antialiasing, true);

    qDebug()<<"clipPath= "<<painter->clipPath();
    qDebug()<<"clipRegion= "<<painter->clipRegion();


#if 0
    painter->setPen(Qt::red);
    QBrush brush = painter->brush();
    painter->setBrush(Qt::yellow);
    painter->drawPath(shape());
    painter->setBrush(brush);
#endif

    painter->setPen(m_pen);
    painter->setBrush(Qt::red);

    painter->drawPolygon(m_polygon);

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

int CTriangleItem::type() const
{
    return Type;
}


void CTriangleItem::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    qDebug()<<"CTriangleItem::mousePressEvent pos="<<event->scenePos();
    QGraphicsItem::mousePressEvent(event);
}
void CTriangleItem::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    qDebug()<<"CTriangleItem::mouseMoveEvent pos="<<event->scenePos();
    QGraphicsItem::mouseMoveEvent(event);
}
void CTriangleItem::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    qDebug()<<"CTriangleItem::mouseReleaseEvent pos="<<event->scenePos();
    QGraphicsItem::mouseReleaseEvent(event);
}
