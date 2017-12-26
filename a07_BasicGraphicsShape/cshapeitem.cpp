#include <QtWidgets>

#include "cshapeitem.h"

//create shape path, = stroke path + origin path
//borrowed from qt src
QPainterPath CShapeItem::CreateShapeFromPath(const QPainterPath &path, const QPen &pen)
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
void CShapeItem::CreateHighlightSelected(CShapeItem *item, QPainter *painter
                                    , const QStyleOptionGraphicsItem *option)
{
    const QRectF murect = painter->transform().mapRect(QRectF(0, 0, 1, 1));
    if (qFuzzyIsNull(qMax(murect.width(), murect.height())))
        return;

    const QRectF mbrect = painter->transform().mapRect(item->boundingRect());
    if (qMin(mbrect.width(), mbrect.height()) < qreal(1.0))
        return;

    qreal itemPenWidth;
    if(item->type() >= LineType)
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



CShapeItem::CShapeItem(QGraphicsItem *parent): QGraphicsItem(parent), m_pen()
{
    setFlag(QGraphicsItem::ItemIsMovable, true);
    setFlag(QGraphicsItem::ItemIsSelectable, true);
    setFlag(QGraphicsItem::ItemSendsGeometryChanges, true);
    //setFlag(QGraphicsItem::ItemClipsToShape, true);
}


QPen CShapeItem::pen() const
{
    return m_pen;
}
void CShapeItem::setPen(const QPen &pen)
{
    if(m_pen == pen)
        return;
    prepareGeometryChange();
    m_pen = pen;
    update();
}





CLineItem::CLineItem(QGraphicsItem *parent)
        :CShapeItem(parent), m_points(2)
{
    //setFlag(QGraphicsItem::ItemIsSelectable, false);
}

CLineItem::CLineItem(const QLineF &line, QGraphicsItem *parent)
        :CShapeItem(parent), m_points(2)
{
    setLine(line);
}

CLineItem::CLineItem(qreal x1, qreal y1, qreal x2, qreal y2, QGraphicsItem *parent)
        :CShapeItem(parent), m_points(2)
{
    setLine(x1,y1,x2,y2);
}

CLineItem::~CLineItem()
{
}

int CLineItem::pointCount(void) const
{
    return m_points.count();
}

QPointF CLineItem::point(int index) const
{
    return m_points.value(index);
}

void CLineItem::setPoint(int index, const QPointF &point)
{
    if(index <0 || index >= m_points.count())
        return;

    prepareGeometryChange();

    m_rectBounding = QRectF();
    m_points[index]= point;

    update();
}

void CLineItem::insertPoint(int index, const QPointF &point)
{
    if(index <0 || index > m_points.count())
        return;

    prepareGeometryChange();

    m_rectBounding = QRectF();
    m_points.insert(index, point);

    update();
}

void CLineItem::removePoint(int index)
{
    if(index <0 || index >= m_points.count())
        return;

    prepareGeometryChange();

    m_rectBounding = QRectF();
    m_points.remove(index);

    update();
}

QLineF CLineItem::line() const
{
    QLineF line;
    if (m_points.count()>=2)
    {
        line.setPoints(m_points.first(), m_points.last());
    }
    else if (m_points.count() == 1)
    {
        line.setPoints(m_points.first(), m_points.first());
    }
    return line;
}

void CLineItem::setLine(const QLineF &line)
{
    prepareGeometryChange();

    m_rectBounding = QRectF();

    m_points.clear();
    m_points.append(line.p1());
    m_points.append(line.p2());

    update();
}


QRectF CLineItem::boundingRect() const
{
#if 1
    if(m_rectBounding.isNull())
    {
        qreal pw = (m_pen.style() == Qt::NoPen) ? qreal(0) : pen().widthF();
        if (pw == 0.0)
        {
            QPolygonF polygon(m_points);
            m_rectBounding = polygon.boundingRect();
        }
        else
        {
            m_rectBounding = shape().controlPointRect();
        }
    }
    return m_rectBounding;
#else
    QRectF rect;
    if(m_points.count()>=2)
    {
        rect = QRectF(m_points.first(), m_points.last());
    }
    qDebug()<<"boundingRect="<<rect;
    return rect;
#endif

}

QPainterPath CLineItem::shape() const
{
    QPainterPath path;

    const QPointF *pd = m_points.constData();
    const QPointF *pe = pd + m_points.size();

    if (pd == pe)
        return path;

    path.moveTo(*pd);
    pd++;

    while (pd != pe) {
        path.lineTo(*pd);
        ++pd;
    }

    QPen pen;
    pen.setWidthF(5.0);
    return CShapeItem::CreateShapeFromPath(path, pen);
}

bool CLineItem::contains(const QPointF &point) const
{
    return QGraphicsItem::contains(point);
}

void CLineItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(widget);
    painter->setRenderHint(QPainter::Antialiasing, true);

#if 0
    painter->setPen(Qt::red);
    QBrush brush = painter->brush();
    painter->setBrush(Qt::yellow);
    painter->drawPath(shape());
    painter->setBrush(brush);
#endif
    qDebug()<<"points count="<< m_points.count();


#if 1
    if (option->state & QStyle::State_Selected)
    {
        painter->setPen(QPen(Qt::red, 1, Qt::SolidLine));
    }
    else
    {
        painter->setPen(m_pen);
    }

    painter->drawLines(m_points);
#else
    painter->drawLines(m_points);

    if (option->state & QStyle::State_Selected)
    {
        CShapeItem::CreateHighlightSelected(this, painter, option);
    }
#endif

}

bool CLineItem::isObscuredBy(const QGraphicsItem *item) const
{
    return QGraphicsItem::isObscuredBy(item);
}

QPainterPath CLineItem::opaqueArea() const
{
    return QGraphicsItem::opaqueArea();
}

int CLineItem::type() const
{
    return CShapeItem::LineType;
}
