#include "preheader.h"

#include "shapeitem.h"
#include "draftscene.h"
#include "draftview.h"
#include "lineitem.h"

/*
 *
 *    CLineItem
 *
 */


CLineItem::CLineItem(QGraphicsItem *parent)
        :C2DItem(parent)
        , m_pSelection(0)
        , m_points(),  m_pNameItem(0), m_pDescItem(0)
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

//index位置的点位置变动导致需更新name和desc的位置
//param
//  index, in,  m_points[index]位置发生变化。 -1， 强制更新.
void CLineItem::updateNameDesc(int index)
{
    bool bUpdate;
    bUpdate = (index>=0) ? false : true;

    if(!bUpdate && (m_pNameItem != 0 || m_pDescItem != 0))
    {
        int size = m_points.size();
        if(size%2 == 0)
        {
            if (index == size/2 || index == size/2 -1)
                bUpdate = true;
        }
        else
        {
            if (index == size/2)
                bUpdate = true;
        }
    }

    if(bUpdate)
    {
        QPointF ptText;
        if(m_pNameItem != 0)
        {
            ptText = calcNamePos();
            m_pNameItem->setPos(ptText);
        }

        if(m_pDescItem != 0)
        {
            ptText = calcDescPos();
            m_pDescItem->setPos(ptText);
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

CEndpoint& CLineItem::endPoint(int nIndex)
{
    if(nIndex < 0 || nIndex >1)
        nIndex = 0;
    return m_endpoints[nIndex];
}



//return
//  0,   不在调整大小和位置模式
CLineSelection* CLineItem::getSelection(void)
{
    return m_pSelection;
}

//鼠标在CDraftView中点中handle，进入拖拽调整大小和位置模式。
//param
//  pView, in,
//  ptView, in,   in view coord
//return
//  true,         进入调整大小模式.
//  false,            条件不具备，不进入调整大小模式.
//
bool CLineItem::beginSelection(CDraftView *pView, QPoint ptView)
{
    Q_ASSERT(m_pSelection == 0);

    m_pSelection = new CLineSelection(this);
    bool bOk = m_pSelection->beginTracking(pView, ptView);

    if(!bOk)
    {
        delete m_pSelection;
        m_pSelection = 0;
    }

    qDebug()<<"beginSelection ------------- OK="<<bOk;
    return bOk;
}

//若在调整大小和位置模式，则退出
//return
//  true, end selection
//  false, no selection
bool CLineItem::endSelection(void)
{
    if(m_pSelection!=0)
    {
        m_pSelection->endTracking();
        delete m_pSelection;
        m_pSelection = 0;
        return true;
    }
    return false;
}

//return
//  true,  在调整中
//  false， 不在调整中
bool CLineItem::trackSelection(QPoint ptView)
{
#if 1
    qDebug()<<"m_pSelection"<< m_pSelection;
#endif

    if(m_pSelection != 0)
    {

        m_pSelection->track(ptView);
        return true;
    }
    return false;
}



bool CLineItem::isTrackingBorder(void)
{
    return (m_pSelection!=0);
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
//return the point, in parent coord
QPointF CLineItem::point(int index) const
{
    //Q_ASSERT(index < m_points.count());
    return mapToParent(m_points[index]);
}

//param
//  index, in, lineItem的index点位置变动引发endPoint发生变动。若=-1, 则根据当前所有点的位置强制更新.
void CLineItem::updateEndpoints(int index)
{
    if(m_points.count()<2)
    {
        qDebug()<<" pass";
        return;
    }

    //endpoints[0]更新
    if(index ==-1 || index == 0 || index == 1)
        m_endpoints[0].setPoints(m_points[1], m_points[0]);

    //endpoinst[1]更新
    if(index ==-1 || index == m_points.count()-1 || index == m_points.count()-2)
        m_endpoints[1].setPoints(m_points[m_points.count()-2], m_points[m_points.count()-1]);
}

//特别说明，添加line时刻， lineItem的item coord的原点定位于(0,0) in scene coord， 故此时lineItem的坐标系等同于scene坐标。
//param
// index, in,
// ptScene, in, in parent coord
void CLineItem::setPoint(int index, const QPointF &ptParent)
{
    //Q_ASSERT(index < m_points.count());

    prepareGeometryChange();
    QPointF point = mapFromParent(ptParent);
    m_points[index] = point;

    updateNameDesc(index);
    updateEndpoints(index);
    updateBoundingRect();
    update();
}

//特别说明，添加line时刻， lineItem的item coord的原点定位于(0,0) in scene coord， 故此时lineItem的坐标系等同于scene坐标。
void CLineItem::setLastPoint(const QPointF &ptParent)
{
    Q_ASSERT(m_points.count()>0);

    prepareGeometryChange();

    QPointF point = mapFromParent(ptParent);
    //qDebug()<< "old points:"<<m_points;
    m_points.last() = point;
    //qDebug()<< "new points:"<<m_points;

    updateNameDesc(-1);
    updateEndpoints();
    updateBoundingRect();
    update();

}

//特别说明，添加line时刻， lineItem的item coord的原点定位于(0,0) in scene coord， 故此时lineItem的坐标系等同于scene坐标。
void CLineItem::appendPoint(const QPointF &ptParent)
{
    prepareGeometryChange();
    QPointF point = mapFromParent(ptParent);
    m_points.append(point);
    updateEndpoints();
    updateBoundingRect();
    update();
}


//特别说明，lineItem的item coord的原点定位于(0,0) in scene coord， 故lineItem的坐标系等同于sceneItem。
//param
// index, in,
// ptParent, in, in parent coord
void CLineItem::insertPoint(int index, const QPointF &ptParent)
{
    prepareGeometryChange();
    QPointF point = mapFromParent(ptParent);
    m_points.insert(index, point);
    updateEndpoints(index);
    updateBoundingRect();
    update();
}

void CLineItem::removePoint(int index)
{
    if(m_points.count()==1)
    {
        qDebug()<<"pass";
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

    QPen pen = painter->pen();
    QBrush brush = painter->brush();


#if 1
    painter->setPen(QPen(Qt::blue, 0, Qt::SolidLine));
    painter->drawPath(shape());
#endif


    painter->setPen(m_pen);
    painter->setBrush(m_brush);

    painter->drawPolyline(m_points.constData(), m_points.count());

    m_endpoints[0].draw(painter, option);
    m_endpoints[1].draw(painter, option);

    if (option->state & QStyle::State_Selected)
        CreateHighlightSelected(this, painter, option);

    painter->setBrush(brush);
    painter->setPen(pen);
}

void CLineItem::keyPressEvent(QKeyEvent *event)
{
    qDebug()<<"key= "<<event->key();

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

CEndpoint::EndShape CEndpoint::endShape(void)
{
    return m_nEndShape;
}

void CEndpoint::setEndShape(EndShape nEndShape)
{
    if(m_nEndShape == nEndShape)
        return;

    m_nEndShape = nEndShape;

    Q_ASSERT(m_pOwner);
    m_pOwner->update();
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
    qDebug()<<" ";

    //绘制
    painter->drawLine(-EndShapeWidth,0, 0,0);
    painter->drawLine(-EndShapeWidth, EndShapeWidth/2, 0,0);
    painter->drawLine(-EndShapeWidth, -EndShapeWidth/2, 0,0);
}

void CEndpoint::drawHollowCircle(QPainter *painter)
{
    qDebug()<<" ";

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
    qDebug()<<" ";

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
    qDebug()<<" ";

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
    qDebug()<<" ";

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
        qDebug()<<"pass";
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
        qDebug()<<"pass";
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



/*
 *
 *
 *      CLineSelection
 *
 */

CLineSelection::CLineSelection(CLineItem *owner, CDraftView *view)
                                    : m_pOwner(owner), m_pView(view)
                                        , m_nLHCode(CLineItem::LHC_0)
                                        , m_ptTrackPos()
                                        //, m_rcPressed()
{

}

Qt::CursorShape CLineSelection::handleCursor(CLineItem::LineHandleCode nLHCode)
{
    Qt::CursorShape cursor = Qt::ArrowCursor;

    switch(nLHCode)
    {
    case CLineItem::LHC_EndHandle:
    case CLineItem::LHC_MidHandle:
    case CLineItem::LHC_NewHandle:
        cursor = Qt::SizeAllCursor;
        break;
    default:
        break;
    }
    return cursor;
}

//map pt in scene coord into pt in parent coord
//
QPointF CLineSelection::mapSceneToParent(QPointF ptScene)
{
    Q_ASSERT(m_pOwner);
    QGraphicsItem *parent;
    parent = m_pOwner->parentItem();
    if(parent != 0)
    {
        return parent->mapFromScene(ptScene);
    }

    return ptScene;
}

//map pt in parent coord into pt in scene coord
//
QPointF CLineSelection::mapParentToScene(QPointF ptParent)
{
    Q_ASSERT(m_pOwner);
    QGraphicsItem *parent;
    parent = m_pOwner->parentItem();
    if(parent != 0)
    {
        return parent->mapToScene(ptParent);
    }

    return ptParent;
}

//返回的line selection的handle code
//param
//  ptView, in, in view coord
//  nIndex, out,  the handl index pressed when LHC_EndHandle or LHC_MidHandle
//                the new handle should be inserted between nIndex and nIndex+1 when LHC_NewHandle
//                invalid when LHC_0
//
CLineItem::LineHandleCode CLineSelection::posCode(QPoint ptView, int& nIndex)
{
    if(m_pOwner->itemFlags() &(C2DItem::NotSelectBorder | C2DItem::NotSizable))
        return CLineItem::LHC_0;

    QRectF handleRect(0,0, CShapeItem::SelectBorderThickness, CShapeItem::SelectBorderThickness);

    QRect rcBounding;           //in CDraftView coord
    rcBounding = m_pOwner->viewBoundingRect(m_pView);

#if 1
    int dx,dy;
    dx = rcBounding.width()>CShapeItem::SelectBorderThickness ? CShapeItem::SelectBorderThickness/2 :0;
    dy = rcBounding.height()>CShapeItem::SelectBorderThickness ? CShapeItem::SelectBorderThickness/2 :0;
    //容纳handle产生的余量
    if(dx> 0 || dy >0)
    rcBounding.adjust(dx, dy, -dx, -dy);
#endif

    if(!rcBounding.contains(ptView))
        return CLineItem::LHC_0;

    QPointF pt;
    int size = m_pOwner->pointCount();
    int i;
    for(i = 0; i<size; i++)
    {
        pt = m_pOwner->point(i);        //in parent coord
        pt = mapParentToScene(pt);
        pt = m_pView->mapFromScene(pt);


        handleRect.moveCenter(pt);
        if(handleRect.contains(ptView))
        {
            nIndex = i;
            return (i == 0 || i== size-1) ? CLineItem::LHC_EndHandle : CLineItem::LHC_MidHandle;
        }
    }
    return CLineItem::LHC_0;
}

bool CLineSelection::beginTracking(CDraftView *pView, QPoint ptView)
{
    Q_ASSERT(m_pOwner != 0);
    Q_ASSERT(pView != 0);

    qDebug()<<" ";

    if(m_pOwner->itemFlags() & (C2DItem::NotSelectBorder | C2DItem::NotSizable))
    {
        qDebug()<<"pass ";
        return false;
    }

    m_pView = pView;

    CLineItem::LineHandleCode nPosCode;
    int nIndex;
    nPosCode = posCode(ptView, nIndex);
    if(nPosCode == CLineItem::LHC_0)
    {
        // go through
    }
    else
    {       //处理拖动selectedBorder handles改变item大小
        m_pOwner->grabMouse();

        //mark
        m_pView->setCursor(handleCursor(nPosCode));

        m_nLHCode = nPosCode;
        m_nIndex = nIndex;
        m_ptPressed = mapSceneToParent(m_pView->mapToScene(ptView));
        m_ptTrackPos = m_pOwner->point(m_nIndex);
        //m_rcPressed = m_pOwner->boundingRect();

        qDebug()<<" ";
        return true;
    }
    return false;
}


void CLineSelection::endTracking(void)
{
    Q_ASSERT(m_pOwner != 0);
    Q_ASSERT(m_pView != 0);

    m_nLHCode = CLineItem::LHC_0;

    m_pOwner->ungrabMouse();
    m_pView->unsetCursor();
}

void CLineSelection::track(QPoint ptView)
{
    qDebug()<<"---------------track ptView="<<ptView;
    if(m_nLHCode == CLineItem::LHC_0 || (m_pOwner->itemFlags() & C2DItem::NotSizable))
    {
        qDebug()<<"pass";
        return;
    }

    QPointF ptParent;
    QPointF ptCur;

    ptParent = mapSceneToParent(m_pView->mapToScene(ptView));

    switch(m_nLHCode)
    {
    case CLineItem::LHC_EndHandle:
    case CLineItem::LHC_MidHandle:
        ptCur = m_ptTrackPos +  ptParent - m_ptPressed;     //in parent coord now.
        //ptCur = m_pOwner->mapFromParent(ptCur);
        Q_ASSERT(m_nIndex >= 0 && m_nIndex < m_pOwner->pointCount());
        m_pOwner->setPoint(m_nIndex, ptCur);
        break;
    case CLineItem::LHC_NewHandle:
        break;
    default:
        Q_ASSERT(false);
        break;
    }
}

//param
//  painter, in,  view painter
void CLineSelection::draw(QPainter *viewPainter)
{

    viewPainter->save();
    viewPainter->setPen(Qt::NoPen);
    viewPainter->setBrush(QBrush(Qt::darkBlue,Qt::SolidPattern));

    QRect rcViewBounding = m_pOwner->viewBoundingRect(m_pView);

    if(rcViewBounding.height() < C2DItem::SelectBorderThickness || rcViewBounding.width() < C2DItem::SelectBorderThickness)
    {
        qDebug()<<"pass. too small to draw.";
        return;
    }

    rcViewBounding.adjust(C2DItem::SelectBorderThickness/2
                          , C2DItem::SelectBorderThickness/2
                          , -C2DItem::SelectBorderThickness/2
                          , -C2DItem::SelectBorderThickness/2
                          );

    QRect handleRect(0,0, C2DItem::SelectBorderThickness, C2DItem::SelectBorderThickness);
    QRect rc;
    QPointF pt;
    QPoint ptView;
    int size;
    int i;
    size = m_pOwner->pointCount();
    for(i = 0; i< size; i++)
    {
        pt = m_pOwner->point(i);        //in parent coord
        pt = mapParentToScene(pt);
        ptView = m_pView->mapFromScene(pt);
        ptView.rx() = qBound(rcViewBounding.left(), ptView.x(), rcViewBounding.right());
        ptView.ry() = qBound(rcViewBounding.top(), ptView.y(), rcViewBounding.bottom());

        handleRect.moveCenter(ptView);
        viewPainter->drawRect(handleRect);
    }

    viewPainter->restore();
}

void CLineSelection::drawSelection(QPainter *viewPainter, const QRectF &rectScene)
{
    Q_ASSERT(m_pOwner != 0);
    Q_ASSERT(m_pView != 0);

    //没有selection border
    if(m_pOwner->itemFlags() &(C2DItem::NotSelectBorder))
    {
        qDebug()<<"pass";
        return;
    }

    QRectF rectItemScene = m_pOwner->sceneBoundingRect();

    //若不在rectScene中，则pass
    if(!rectItemScene.intersects(rectScene))
    {
        return;
    }

    draw(viewPainter);
}





