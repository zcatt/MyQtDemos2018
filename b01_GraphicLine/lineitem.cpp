#include "preheader.h"

#include "draftscene.h"
#include "lineitem.h"

/*
 *
 *    CLineItem
 *
 */


CLineItem::CLineItem(QGraphicsItem *parent)
        :C2DItem(parent), m_points(),  m_pNameItem(0), m_pDescItem(0)
        , m_endpoints()
{
    //接受keyEvent
    setFlag(QGraphicsItem::ItemIsFocusable, true);

    //The item clips to its own shape.
    //The item cannot draw or receive mouse, tablet, drag and drop or hover events outside its shape.
    //setFlag(QGraphicsItem::ItemClipsToShape, true);

    //setFlag(QGraphicsItem::ItemIsSelectable, false);

    m_endpoints[0].setOwner(this);
    m_endpoints[1].setOwner(this);
}

#if 0
//param
//  line, in, in scene coord
CLineItem::CLineItem(const QLineF &line, QGraphicsItem *parent)
        :C2DItem(parent), m_points(), m_pNameItem(0), m_pDescItem(0)
{
    //接受keyEvent
    setFlag(QGraphicsItem::ItemIsFocusable, true);
    setLine(line);
}

//param
//  x1,y1,x2,y2, in,   in scene coord
CLineItem::CLineItem(qreal x1, qreal y1, qreal x2, qreal y2, QGraphicsItem *parent)
        :C2DItem(parent), m_points(2), m_pNameItem(0), m_pDescItem(0)
{
    setLine(x1,y1,x2,y2);
}
#endif

CLineItem::~CLineItem()
{
#if 0
    Q_ASSERT(m_pEndpoint[0]);
    Q_ASSERT(m_pEndpoint[1]);

    delete m_pEndpoint[0];
    m_pEndpoint[0] = 0;

    delete m_pEndpoint[1];
    m_pEndpoint[1] = 0;
#endif
}

QPointF CLineItem::calcTextBasePos(void)
{
    int nSeg = m_points.count();
    QPointF pt;

    if(nSeg%2 == 0)
    {
        nSeg = nSeg/2 -1;
        pt = (m_points[nSeg]+m_points[nSeg+1])/2;
    }
    else
    {
        nSeg = nSeg/2;
        pt = m_points[nSeg];
    }

    return pt;
}

//计算name textItem放置的位置， in this lineItem coord
//
QPointF CLineItem::calcNamePos(void)
{
    QPointF pt = calcTextBasePos();
    pt += QPointF(NameOffsetX, NameOffsetY);
    return pt;
}

QPointF CLineItem::calcDescPos(void)
{
    QPointF pt = calcTextBasePos();
    pt += QPointF(DescOffsetX, DescOffsetY);
    return pt;
}

//创建，销毁或设置lineItem关联的name textItem
void CLineItem::setName(const QString& name)
{
    if(name.isEmpty())
    {
        //销毁name textItem
        if(m_pNameItem != 0)
        {
            delete m_pNameItem;
            m_pNameItem = 0;
        }
    }
    else
    {
        if(m_pNameItem == 0)
        {
            //创建name textItem
            m_pNameItem = new CTextItem(this);
            m_pNameItem->setText(name);
            QPointF ptText;
            ptText = calcNamePos();
            m_pNameItem->setPos(ptText);
        }
        else if (m_pNameItem->text() != name)
        {
            //变更text
            m_pNameItem->setText(name);
        }
    }

}

//创建，销毁或设置lineItem关联的description textItem
void CLineItem::setDesc(const QString& desc)
{
    if(desc.isEmpty())
    {
        //销毁name textItem
        if(m_pDescItem != 0)
        {
            delete m_pDescItem;
            m_pDescItem = 0;
        }
    }
    else
    {
        if(m_pDescItem == 0)
        {
            //创建name textItem
            m_pDescItem = new CTextItem(this);
            m_pDescItem->setText(desc);
            QPointF ptText;
            ptText = calcDescPos();
            m_pDescItem->setPos(ptText);
        }
        else if (m_pDescItem->text() != desc)
        {
            //变更text
            m_pDescItem->setText(desc);
        }
    }
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

//return
//  QPainterPath, in item coord
QPainterPath CLineItem::shape() const
{
    QPainterPath path;
    if (m_points.count() <2 || (m_points.count() ==2 && QLineF(m_points[0], m_points[1]) == QLineF()))
        return path;

    path.moveTo(m_points[0]);
    for(int i = 1; i< m_points.count(); i++)
    {
        path.lineTo(m_points[i]);
    }

    //形成闭合路径，避免collidesWidthPath()产生的内部空白选中副作用.
    for(int i = m_points.count() - 1; i>=0; i--)
    {
        path.lineTo(m_points[i]);
    }

    QPen pen = m_pen;
    pen.setWidthF(10);      //留白
    path = CreateShapeOutlineFromPath(path, pen);
    path.addPath(m_endpoints[0].shape());
    path.addPath(m_endpoints[1].shape());

    return path;
}

int CLineItem::pointCount(void) const
{
    return m_points.count();
}

//特别说明，添加line时刻， lineItem的item coord的原点定位于(0,0) in scene coord， 故此时lineItem的坐标系等同于scene坐标。
//return the point, in scene coord
QPointF CLineItem::point(int index) const
{
    //Q_ASSERT(index < m_points.count());
    return mapToScene(m_points[index]);
}

//param
//  index, in, index发生变动，若=-1, 则直接更新.
void CLineItem::updateEndpoints(int index)
{
    if(m_points.count()<2)
    {
        qDebug()<<"CLineItem::updateEndpoints() pass";
        return;
    }

    if(index ==-1 || index == 0 || index == 1)
        m_endpoints[0].setPoints(m_points[1], m_points[0]);

    if(index ==-1 || index == m_points.count()-1 || index == m_points.count()-2)
        m_endpoints[1].setPoints(m_points[m_points.count()-2], m_points[m_points.count()-1]);
}

//特别说明，添加line时刻， lineItem的item coord的原点定位于(0,0) in scene coord， 故此时lineItem的坐标系等同于scene坐标。
//param
// index, in,
// point, in, in item coord
void CLineItem::setPoint(int index, const QPointF &ptScene)
{
    //Q_ASSERT(index < m_points.count());

    prepareGeometryChange();
    QPointF point = mapFromScene(ptScene);
    m_points[index] = point;

    updateEndpoints(index);
    updateBoundingRect();
    update();
}

//特别说明，添加line时刻， lineItem的item coord的原点定位于(0,0) in scene coord， 故此时lineItem的坐标系等同于scene坐标。
void CLineItem::setLastPoint(const QPointF &ptScene)
{
    Q_ASSERT(m_points.count()>0);

    prepareGeometryChange();

    QPointF point = mapFromScene(ptScene);
    //qDebug()<< "old points:"<<m_points;
    m_points.last() = point;
    //qDebug()<< "new points:"<<m_points;

    updateEndpoints();
    updateBoundingRect();
    update();

}

//特别说明，添加line时刻， lineItem的item coord的原点定位于(0,0) in scene coord， 故此时lineItem的坐标系等同于scene坐标。
void CLineItem::appendPoint(const QPointF &ptScene)
{
    prepareGeometryChange();
    QPointF point = mapFromScene(ptScene);
    m_points.append(point);
    updateEndpoints();
    updateBoundingRect();
    update();
}


//特别说明，lineItem的item coord的原点定位于(0,0) in scene coord， 故lineItem的坐标系等同于sceneItem。
//param
// index, in,
// point, in, in item coord
void CLineItem::insertPoint(int index, const QPointF &ptScene)
{
    prepareGeometryChange();
    QPointF point = mapFromScene(ptScene);
    m_points.insert(index, point);
    updateEndpoints(index);
    updateBoundingRect();
    update();
}

void CLineItem::removePoint(int index)
{
    if(m_points.count()==1)
    {
        qDebug()<<"CLineItem::removePoint() pass";
        return;
    }
    prepareGeometryChange();
    m_points.remove(index);
    updateEndpoints();
    updateBoundingRect();
    update();
}

//特别说明，lineItem的item coord的原点定位于(0,0) in scene coord， 故lineItem的坐标系等同于sceneItem。
//return the first lineF seg, in scene coord
QLineF CLineItem::line() const
{
    return QLineF(mapToScene(m_points[0]), mapToScene(m_points[1]));
}

//特别说明，lineItem的item coord的原点定位于(0,0) in scene coord， 故lineItem的坐标系等同于sceneItem。
//set the first lineF seg, in scene coord
void CLineItem::setLine(const QLineF &line)
{
    prepareGeometryChange();

    m_points.resize(2);
    m_points[0] = mapFromScene(line.p1());
    m_points[1] = mapFromScene(line.p2());

    updateEndpoints();
    updateBoundingRect();
    update();
}


//特别说明，lineItem的item coord的原点定位于(0,0) in scene coord， 故lineItem的坐标系等同于sceneItem。
void CLineItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(widget);
    qDebug()<<"paint pos:"<<pos() <<", lines:"<<m_points;

#if 1
    painter->setPen(QPen(Qt::blue, 0, Qt::SolidLine));
    painter->drawPath(shape());
#endif


    //painter->setPen(QPen(Qt::red, 1, Qt::SolidLine));
    painter->setPen(m_pen);
    painter->drawPolyline(m_points.constData(), m_points.count());

    m_endpoints[0].draw(painter, option);
    m_endpoints[1].draw(painter, option);

    if (option->state & QStyle::State_Selected)
        CreateHighlightSelected(this, painter, option);
}

void CLineItem::keyPressEvent(QKeyEvent *event)
{
    qDebug()<<"CLineItem::keyPressEvent()  "<<event->key();

    CDraftScene *pScene = static_cast<CDraftScene*>(scene());
    if(pScene->actionState() == CDraftScene::ActionState_NewLine)
    {
        switch(event->key())
        {
        case Qt::Key_Escape:
            //放弃新增线条操作
            pScene->resetActionState();
            delete this;
            break;
        case Qt::Key_Delete:
        case Qt::Key_Backspace:
            //删除最后一个设定的端点
            if(m_points.count()>2)     //起始点和当前鼠标拖动的点
            {
                removePoint(m_points.count()-2);
            }
            break;
        case Qt::Key_Enter:
        case Qt::Key_Space:
            {
                //setLastPoint(mouseEvent->scenePos());
                pScene->resetActionState();
            }
            break;
        default:
            break;
        }

    }

}


int CLineItem::type() const
{
    return Type;
}


QVariant CLineItem::itemChange(GraphicsItemChange change, const QVariant &value)
{
    if(change == ItemChildRemovedChange)
    {
        QGraphicsItem *item = value.value<QGraphicsItem*>();
        if(item == m_pNameItem)
        {
            m_pNameItem = 0;
            return value;
        }
        else if (item == m_pDescItem)
        {
            m_pDescItem = 0;
            return value;
        }
    }

    return C2DItem::itemChange(change, value);
}





/*
 *
 *   CEndpoint
 *
 *
 */


CEndpoint::CEndpoint(EndShape nEndShape, C2DItem *pOwner): m_pOwner(pOwner)
                            , m_nEndShape(nEndShape), m_line()
                            , m_pNameItem(0), m_pDescItem(0)
                            , m_pAttached(0)
{

}

void CEndpoint::setOwner(C2DItem *pOwner)
{
    m_pOwner = pOwner;
}

void CEndpoint::setEndShape(EndShape nEndShape)
{
    m_nEndShape = nEndShape;
}

CTextItem* CEndpoint::nameItem(void)
{
    return m_pNameItem;
}

void CEndpoint::setNameItem(CTextItem *nameItem)
{
    m_pNameItem = nameItem;
}

CTextItem* CEndpoint::descItem(void)
{
    return m_pDescItem;
}

void CEndpoint::setDescItem(CTextItem *descItem)
{
    m_pDescItem = descItem;
}

//按from到to的方向绘制端点符号，其中to是符号的末端
//param
//  from, to,  in,  in item coord
void CEndpoint::setPoints(QPointF &from, QPointF &to)
{
    m_line.setPoints(from, to);
}

//
void CEndpoint::drawNone(QPainter *painter)
{
    //绘制, x轴方向为箭头方向， (0,0)是箭头尖， width*width矩形范围内
    painter->drawLine(-EndShapeWidth,0, 0,0);
}

void CEndpoint::drawArrow(QPainter *painter)
{
    qDebug()<<"CEndpoint::drawArrow()";

    //绘制
    painter->drawLine(-EndShapeWidth,0, 0,0);
    painter->drawLine(-EndShapeWidth, EndShapeWidth/2, 0,0);
    painter->drawLine(-EndShapeWidth, -EndShapeWidth/2, 0,0);
}

void CEndpoint::drawHollowCircle(QPainter *painter)
{
    qDebug()<<"CEndpoint::drawArrow()";

    //绘制
    QBrush brushOld;
    brushOld = painter->brush();
    painter->setBrush(painter->background());
    QRectF rect(-EndShapeWidth,-EndShapeWidth/2, EndShapeWidth,EndShapeWidth);
#if 1
    painter->drawEllipse(rect);
#else
    painter->drawArc(rect, 0, 360*16);
#endif
    painter->setBrush(brushOld);
}

void CEndpoint::drawSolidCircle(QPainter *painter)
{
    qDebug()<<"CEndpoint::drawArrow()";

    //绘制
    QBrush brushOld;
    brushOld = painter->brush();
    QBrush brush(m_pOwner->pen().color());
    painter->setBrush(brush);
    QRectF rect(-EndShapeWidth,-EndShapeWidth/2, EndShapeWidth,EndShapeWidth);
    painter->drawEllipse(rect);
    painter->setBrush(brushOld);
}

void CEndpoint::drawHollowTriangle(QPainter *painter)
{
     //绘制
    QPointF pt[]={QPointF(0,0)
                  , QPointF(-EndShapeWidth, EndShapeWidth/2)
                  , QPointF(-EndShapeWidth, -EndShapeWidth/2)};
    QBrush brushOld;
    brushOld = painter->brush();
    painter->setBrush(painter->background());
    painter->drawPolygon(pt, 3);
    painter->setBrush(brushOld);
}

void CEndpoint::drawSolidTriangle(QPainter *painter)
{
    //绘制
    QPointF pt[]={QPointF(0,0)
                  , QPointF(-EndShapeWidth, EndShapeWidth/2)
                  , QPointF(-EndShapeWidth, -EndShapeWidth/2)};
    QBrush brushOld;
    brushOld = painter->brush();
    QBrush brush(m_pOwner->pen().color());
    painter->setBrush(brush);
    painter->drawPolygon(pt, 3);
    painter->setBrush(brushOld);
}

void CEndpoint::drawHollowDiamond(QPainter *painter)
{
    qDebug()<<"CEndpoint::drawArrow()";

    //绘制
    QPointF pt[]={QPointF(0,0)
                  , QPointF(-EndShapeWidth/2, EndShapeWidth/2)
                  , QPointF(-EndShapeWidth, 0)
                  , QPointF(-EndShapeWidth/2, -EndShapeWidth/2)
                  };
    QBrush brushOld;
    brushOld = painter->brush();
    painter->setBrush(painter->background());
    painter->drawPolygon(pt, 4);
    painter->setBrush(brushOld);
}

void CEndpoint::drawSolidDiamond(QPainter *painter)
{
    qDebug()<<"CEndpoint::drawArrow()";

    //绘制
    QPointF pt[]={QPointF(0,0)
                  , QPointF(-EndShapeWidth/2, EndShapeWidth/2)
                  , QPointF(-EndShapeWidth, 0)
                  , QPointF(-EndShapeWidth/2, -EndShapeWidth/2)
                  };
    QBrush brushOld;
    brushOld = painter->brush();
    QBrush brush(m_pOwner->pen().color());
    painter->setBrush(brush);
    painter->drawPolygon(pt, 4);
    painter->setBrush(brushOld);
}


void CEndpoint::draw(QPainter *painter, const QStyleOptionGraphicsItem *option)
{
    Q_UNUSED(option);

    if(m_line.isNull())
    {
        qDebug()<<"CEndpoint::draw() pass";
        return;
    }

    //坐标变换
    QTransform orig = painter->transform();
    QTransform trans;

    trans.translate(m_line.p2().x(), m_line.p2().y());
    trans.rotate(-m_line.angle());

    painter->setTransform(trans, true);     //true, combined as pos()!=(0,0)

    //绘制
    switch(m_nEndShape)
    {
    case EndShape_Arrow:
        drawArrow(painter);
        break;
    case EndShape_HollowCircle:
        drawHollowCircle(painter);
        break;
    case EndShape_SolidCircle:
        drawSolidCircle(painter);
        break;
    case EndShape_HollowTriangle:
        drawHollowTriangle(painter);
        break;
    case EndShape_SolidTriangle:
        drawSolidTriangle(painter);
        break;
    case EndShape_HollowDiamond:
        drawHollowDiamond(painter);
        break;
    case EndShape_SolidDiamond:
        drawSolidDiamond(painter);
        break;
    case EndShape_Custom:
    case EndShape_None:
    default:
        drawNone(painter);
        //drawArrow(painter);
        //drawHollowCircle(painter);
        //drawSolidCircle(painter);
        //drawHollowTriangle(painter);
        //drawSolidTriangle(painter);
        //drawHollowDiamond(painter);
        //drawSolidDiamond(painter);
        break;
    }

    //恢复坐标
    painter->setTransform(orig, false);

}


QPainterPath CEndpoint::shape(void) const
{

    QPainterPath path;
    if(m_line.isNull())
    {
        qDebug()<<"CEndpoint::draw() pass";
        return path;
    }

    path.addRect(-EndShapeWidth, -EndShapeWidth/2, EndShapeWidth, EndShapeWidth);

    //坐标变换
    QTransform trans;
    trans.translate(m_line.x2(), m_line.y2());
    trans.rotate(-m_line.angle());


    path = trans.map(path);
    return path;
}



