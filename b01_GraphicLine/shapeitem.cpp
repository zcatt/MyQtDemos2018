#include "preheader.h"

#include "draftview.h"
#include "shapeitem.h"
#include "shapeselection.h"
#include "propeditor.h"
#include "mainwindow.h"

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
    : QGraphicsObject(parent) //QGraphicsItem(parent)
        , m_nItemFlag(0)
        , m_pen(Qt::darkRed, 1, Qt::SolidLine)
        , m_brush(Qt::yellow)
        , m_rectBounding()
{
    setFlag(QGraphicsItem::ItemIsMovable, true);
    setFlag(QGraphicsItem::ItemIsSelectable, true);
    setFlag(QGraphicsItem::ItemSendsGeometryChanges, true);
    //setFlag(QGraphicsItem::ItemClipsToShape, true);

    setAcceptHoverEvents(true);
}

C2DItem::~C2DItem()
{

}

int C2DItem::type() const
{
    return Type;
}

C2DItem::ItemFlags C2DItem::itemFlags() const
{
    return ItemFlags(m_nItemFlag);
}

void C2DItem::setItemFlag(ItemFlag flag, bool enabled)
{
    if(enabled)
        m_nItemFlag |= flag;
    else
        m_nItemFlag &= ~flag;
}

void C2DItem::setItemFlags(ItemFlags flags)
{
    m_nItemFlag = flags;
}


QPen C2DItem::pen() const
{
    return m_pen;
}

QBrush C2DItem::brush(void)
{
    return m_brush;
}

void C2DItem::setBrush(const QBrush& brush)
{
    m_brush = brush;
    update();
}

//在view coord中item的boundingRect
QRect C2DItem::viewBoundingRect(QGraphicsView *view)
{
    Q_ASSERT(view->scene() == scene());

    QRect rcBoundingInView;
    QSize size;
    rcBoundingInView = view->mapFromScene(sceneBoundingRect()).boundingRect();

#if 0
    if (rcBoundingInView.width() < C2DItem::SelectBorderThickness)
        size.rwidth() = C2DItem::SelectBorderThickness;
    if (rcBoundingInView.height() < C2DItem::SelectBorderThickness)
        size.rheight() = C2DItem::SelectBorderThickness;

    if(!size.isNull())
        rcBoundingInView.setSize(size);
#endif

    return rcBoundingInView;
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


QVariant C2DItem::itemChange(GraphicsItemChange change, const QVariant &value)
{
    if(change == QGraphicsItem::ItemSelectedChange)
    {
        qDebug()<<"change="<<change<< "select="<<value.toBool();
        onSelect(value.toBool());
        emit selectedChange(this, value.toBool());
    }
    else if (change == QGraphicsItem::ItemPositionChange)
    {
        qDebug()<<"-------------  change="<<change<< "pos="<<value.toPointF();
        emit posChange(this, value.toPointF());
    }
    else
    {
        qDebug()<<"change="<<change;
    }

    return QGraphicsItem::itemChange(change, value);
}



/*
 *
 *    CShapeItem
 *
 */


CShapeItem::CShapeItem(QGraphicsItem *parent)        
        : C2DItem(parent)
            , m_nShapeItemFlag(0)
            , m_sizeMin(SelectBorderThickness*3 +2, SelectBorderThickness*3 +2)
            , m_pSelection(0)
#if 0
            , m_bTrackingBorder(false), m_nBorderHandleCode(BHC_0), m_ptPressed(), m_ptTrackPos(), m_rcPressed()
#endif
{
}

CShapeItem::~CShapeItem()
{
    if(m_pSelection)
        delete m_pSelection;
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
C2DSelection* CShapeItem::getSelection(void)
{
    return m_pSelection;
}

bool CShapeItem::isTrackingSelection(void)
{
    return m_pSelection != 0 && m_pSelection->isInTracking();
}

//鼠标在CDraftView中点中handle，进入拖拽调整大小和位置模式。
//param
//  pView, in,
//  ptView, in,   in view coord
//return
//  true,         进入调整大小模式.
//  false,            条件不具备，不进入调整大小模式.
//
bool CShapeItem::beginSelection(CDraftView *pView,  QMouseEvent* mouseView)
{
    Q_ASSERT(pView != 0);

    if(m_pSelection == 0)
    {
        m_pSelection = new CShapeSelection(this, pView);
    }

    Q_ASSERT(m_pSelection->view() == pView);
    Q_ASSERT(!m_pSelection->isInTracking());

    bool bOk = m_pSelection->beginTracking(mouseView);

#if 0
    if(!bOk)
    {
        delete m_pSelection;
        m_pSelection = 0;
    }
#endif

    qDebug()<<"beginSelection ------------- OK="<<bOk;
    return bOk;
}

//若在调整大小和位置模式，则退出
//return
//  true, end selection
//  false, no selection
//
bool CShapeItem::endSelection(void)
{
    bool bRes = false;

    if(m_pSelection!=0)
    {
        bRes = m_pSelection->endTracking();
#if 0
        delete m_pSelection;
        m_pSelection = 0;
#endif
    }
    return bRes;
}

//return
//  true,  在调整中
//  false， 不在调整中
bool CShapeItem::trackSelection(QMouseEvent* mouseView)
{
#if 0
    qDebug()<<"m_pSelection"<< m_pSelection;
#endif
    bool bRes = false;

    if(m_pSelection != 0)
    {
        bRes = m_pSelection->track(mouseView);
    }
    return bRes;
}




QPainterPath CShapeItem::shape() const
{
    QPainterPath path;

    path.addRect(m_rectBounding);
    return path;
}

void CShapeItem::onSelect(bool bSelected)
{
    if(bSelected)
    {
        qDebug()<<" create selection";
        Q_ASSERT(m_pSelection == 0);

        if(m_pSelection)
            delete m_pSelection;

        CDraftView *pView;
#if 0
        QGraphicsView *pGV;
        pGV = scene()->views().at(0);

        pView = dynamic_cast<CDraftView*>(pGV);
#else
        pView = dynamic_cast<CDraftView*>(scene()->views().at(0));
#endif
        Q_ASSERT(pView != 0);
        m_pSelection = new CShapeSelection(this, pView);
    }
    else
    {
        qDebug()<<" delete selection";
        Q_ASSERT(m_pSelection != 0);

        if(m_pSelection != 0)
        {
            delete m_pSelection;
            m_pSelection = 0;
        }
    }
}


void CShapeItem::onSceneChange(QGraphicsScene *scene)
{
    qDebug()<<"";
}

#if 0
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
#endif



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

void CRectItem::setRect(const QRectF &rc)
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
        m_rectBounding = shape().controlPointRect();
        m_rectBounding.adjust(-CShapeItem::SelectBorderThickness, -CShapeItem::SelectBorderThickness
                              ,CShapeItem::SelectBorderThickness, CShapeItem::SelectBorderThickness);
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

    qDebug()<< "boundingRect="<< m_rectBounding;
}


void CRectItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(widget);
    //painter->setRenderHint(QPainter::Antialiasing, true);

    //qDebug()<<"clipPath= "<<painter->clipPath();
    //qDebug()<<"clipRegion= "<<painter->clipRegion();


    QPen pen = painter->pen();
    QBrush brush = painter->brush();

    painter->setPen(m_pen);
    painter->setBrush(m_brush);
#if 1
    painter->drawRect(m_rectBounding);
#else
    painter->drawPath(shape());
#endif

    if (option->state & QStyle::State_Selected)
    {
#if 0
        drawSelectBorder(painter, boundingRect());
#else
        CShapeItem::CreateHighlightSelected(this, painter, option);
#endif
    }

    painter->setBrush(brush);
    painter->setPen(pen);
}

int CRectItem::type() const
{
    return Type;
}


void CRectItem::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    qDebug()<<"pos="<<event->scenePos();
    qDebug()<<"     flags="<<flags();
    QGraphicsItem::mousePressEvent(event);
}
void CRectItem::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    qDebug()<<"evtPos="<<event->scenePos() << "pos="<<pos();
    qDebug()<<"     flags="<<flags();

    QGraphicsItem::mouseMoveEvent(event);
}
void CRectItem::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    qDebug()<<"pos="<<event->scenePos();
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
    setItemFlag(NotSizable, true);
    setItemFlag(NotSelectBorder, true);
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
    //thickness += SelectBorderThickness;     //handle的margins.

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

    QPen pen = painter->pen();
    QBrush brush = painter->brush();
#if 1
    painter->setPen(m_pen);
    painter->setBrush(m_brush);
    //painter->drawPath(shape());
    painter->drawRect(m_rectBounding);


    QFont fnt;
    //TODO, 设置字形和字号
    painter->setFont(fnt);

#if 1
    painter->drawText(m_rectBounding, 0, m_Name);

    //QRectF rc;
    //rc = m_rectBounding.adjusted(SelectBorderThickness, SelectBorderThickness
    //                             , -SelectBorderThickness,-SelectBorderThickness);
    //painter->drawText(rc, 0, m_Name);
#else
    QFontMetricsF fm(fnt);

    painter->drawText(m_rectBounding.left(), m_rectBounding.top()+fm.ascent(), m_Name);
#endif

#endif


#if 1
    if (option->state & QStyle::State_Selected)
    {
        CShapeItem::CreateHighlightSelected(this, painter, option);
    }
#endif

    painter->setBrush(brush);
    painter->setPen(pen);

}

int CTextItem::type() const
{
    return Type;
}

