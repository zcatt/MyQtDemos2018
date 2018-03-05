#include "preheader.h"


#include "shapeitem.h"
#include "shapeselection.h"

/*
 *
 *    C2DItem
 *
 */


//borrowed from qt_graphicsItem_shapeFromPath@qt src
//create outline path, = stroke path
//根据pen的线宽创建path的轮廓.
//要求path中仅含线宽为0的线条路径。若线过宽，则其strokPath无法覆盖原始线的区域。
//也不应含有闭合的区域path，理由同上.
//
QPainterPath C2DItem::CreateLineOutlineFromPath(const QPainterPath &path, const QPen &pen)
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

    //p.addPath(path);

    return p;
}

//create shape path, = stroke path + origin path
//borrowed from qt_graphicsItem_shapeFromPath@qt src
QPainterPath C2DItem::CreateShapeOutlineFromPath(const QPainterPath &path, const QPen &pen)
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
            , m_pSelection(Q_NULLPTR)
            , m_bTrackingBorder(false), m_nBorderHandleCode(BHC_0), m_ptPressed(), m_ptTrackPos(), m_rcPressed()
{
}

CShapeItem::~CShapeItem()
{
    if(m_pSelection)
        delete m_pSelection;
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


//return
//  0,   不在调整大小和位置模式
CShapeSelection* CShapeItem::getSelection(void)
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
bool CShapeItem::beginSelection(CDraftView *pView, QPoint ptView)
{
    Q_ASSERT(m_pSelection == 0);

    m_pSelection = new CShapeSelection(this);
    bool bOk = m_pSelection->beginTracking(pView, ptView);

    if(!bOk)
    {
        delete m_pSelection;
        m_pSelection = 0;
    }

    qDebug()<<"CShapeItem::beginSelection ------------- OK="<<bOk;
    return bOk;
}

//若在调整大小和位置模式，则退出
//return
//  true, end selection
//  false, no selection
bool CShapeItem::endSelection(void)
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
bool CShapeItem::trackSelection(QPoint ptView)
{
    qDebug()<<"CShapeItem::trackSelection, m_pSelection"<< m_pSelection;
    if(m_pSelection != 0)
    {

        m_pSelection->track(ptView);
        return true;
    }
    return false;
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


QPainterPath CShapeItem::shape() const
{
    QPainterPath path;

    path.addRect(m_rectBounding);
    return path;
}

void CShapeItem::hoverEnterEvent(QGraphicsSceneHoverEvent *event)
{
    Q_UNUSED(event);
    qDebug()<<"hover Enter";
}

void CShapeItem::hoverMoveEvent(QGraphicsSceneHoverEvent *event)
{
    Q_UNUSED(event);
    qDebug()<<"hover move";

}

void CShapeItem::hoverLeaveEvent(QGraphicsSceneHoverEvent *event)
{
    Q_UNUSED(event);
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
}

CRectItem::CRectItem(const QRectF &rc, QGraphicsItem *parent) : CShapeItem(parent)
{
    m_rc = rc;
}

CRectItem::CRectItem(const QPointF &leftTop, const QPointF &rightBottom, QGraphicsItem *parent)
    :CShapeItem(parent), m_rc(leftTop, rightBottom)
{
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

    //qDebug()<<"clipPath= "<<painter->clipPath();
    //qDebug()<<"clipRegion= "<<painter->clipRegion();


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
 *
 * CTextItem
 *
 *
 */







CTextItem::CTextItem(C2DItem *parent): CShapeItem(parent), m_Name()
{
    setShapeItemFlag(NotSizable, true);

}

void CTextItem::clear(void)
{
    prepareGeometryChange();
    m_Name.clear();
    updateBoundingRect();
    update();
}

QString CTextItem::text(void) const
{
    return m_Name;
}

void CTextItem::setText(const QString &text)
{
    prepareGeometryChange();
    m_Name=text;
    updateBoundingRect();
    update();
}

//QMarginsF QLineEdit::textMargins() const;
//void setTextMargins(const QMarginsF &margins);

QRectF CTextItem::rect() const
{
    return m_rectBounding;
}

void CTextItem::setBoundingRect(QSizeF size)
{
    Q_UNUSED(size);

    Q_ASSERT(false);
}

void CTextItem::updateBoundingRect(void)
{
    if(m_Name.isEmpty())
    {
        return;
        m_rectBounding.setRect(0,0,0,0);
    }

    QFont fnt;

    //TODO, 设置字形和字号

    //qDebug()<<fnt.pointSizeF();
    //fnt.setPointSizeF(12.0);

    QFontMetricsF fm(fnt);
    m_rectBounding = fm.boundingRect(m_Name);

#if 1
    qreal thickness = (pen().style() == Qt::NoPen) ? 0.0 :m_pen.widthF()/2.0;
    qDebug()<<"thickness ="<< thickness;
    thickness+=1;

    if (thickness > 0.0)
        m_rectBounding.adjust(-thickness, -thickness, thickness, thickness);
#endif
}

void CTextItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
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


    QFont fnt;
    //TODO, 设置字形和字号
    painter->setFont(fnt);
#if 0
    QFontMetricsF fm(fnt);

    painter->drawText(m_rectBounding.left(), m_rectBounding.top()+fm.ascent(), m_Name);
#else
    painter->drawText(m_rectBounding, 0, m_Name);
#endif

#endif


#if 1
    if (option->state & QStyle::State_Selected)
    {
        CShapeItem::CreateHighlightSelected(this, painter, option);
    }
#endif

}

int CTextItem::type() const
{
    return Type;
}

