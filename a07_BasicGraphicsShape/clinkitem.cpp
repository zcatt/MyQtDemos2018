#include <QtWidgets>

#include "clinkitem.h"


/*
 *
 *    CLinkItem
 *
 */


CLinkItem::CLinkItem(QGraphicsItem *parent)
        :C2DItem(parent), m_points(2)
{
    //setFlag(QGraphicsItem::ItemIsSelectable, false);
}

CLinkItem::CLinkItem(const QLineF &line, QGraphicsItem *parent)
        :C2DItem(parent), m_points(2)
{
    setLine(line);
}

CLinkItem::CLinkItem(qreal x1, qreal y1, qreal x2, qreal y2, QGraphicsItem *parent)
        :C2DItem(parent), m_points(2)
{
    setLine(x1,y1,x2,y2);
}

CLinkItem::~CLinkItem()
{
}

int CLinkItem::pointCount(void) const
{
    return m_points.count();
}

QPointF CLinkItem::point(int index) const
{
    return m_points.value(index);
}

void CLinkItem::setPoint(int index, const QPointF &point)
{
    if(index <0 || index >= m_points.count())
        return;

    prepareGeometryChange();

    m_rectBounding = QRectF();
    m_points[index]= point;

    update();
}

void CLinkItem::insertPoint(int index, const QPointF &point)
{
    if(index <0 || index > m_points.count())
        return;

    prepareGeometryChange();

    m_rectBounding = QRectF();
    m_points.insert(index, point);

    update();
}

void CLinkItem::removePoint(int index)
{
    if(index <0 || index >= m_points.count())
        return;

    prepareGeometryChange();

    m_rectBounding = QRectF();
    m_points.remove(index);

    update();
}

void CLinkItem::setPen(const QPen &pen)
{
    if(m_pen == pen)
        return;
    prepareGeometryChange();
    m_rectBounding = QRectF();
    m_pen = pen;
    update();
}


QLineF CLinkItem::line() const
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

void CLinkItem::setLine(const QLineF &line)
{
    prepareGeometryChange();

    m_rectBounding = QRectF();

    m_points.clear();
    m_points.append(line.p1());
    m_points.append(line.p2());

    update();
}


QRectF CLinkItem::boundingRect() const
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

QPainterPath CLinkItem::shape() const
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

#if 1
    return C2DItem::CreateShapeFromPath(path, m_pen);
#else
    QPen pen;
    pen.setWidthF(5.0);
    return CShapeItem::CreateShapeFromPath(path, pen);
#endif
}




void CLinkItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
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
        C2DItem::CreateHighlightSelected(this, painter, option);
    }
#endif

}

int CLinkItem::type() const
{
    return Type;
}

