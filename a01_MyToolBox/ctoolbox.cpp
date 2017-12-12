#include "ctoolbox.h"
#include <QtWidgets>



QSize CToolBoxButton::sizeHint() const
{
    QSize iconSize(8, 8);

    if (!icon().isNull()) {
        int icone = style()->pixelMetric(QStyle::PM_SmallIconSize, 0, parentWidget() /* QToolBox */);
        iconSize += QSize(icone + 2, icone);
    }
    QSize textSize = fontMetrics().size(Qt::TextSingleLine, text()) + QSize(0, 8);

    QSize total(iconSize.width() + textSize.width(), qMax(iconSize.height(), textSize.height()));
    return total.expandedTo(QApplication::globalStrut());
}

QSize CToolBoxButton::minimumSizeHint() const
{
    if (icon().isNull())
        return QSize();
    int icone = style()->pixelMetric(QStyle::PM_SmallIconSize, 0, parentWidget() /* QToolBox */);
    return QSize(icone + 8, icone + 8);
}

void CToolBoxButton::initStyleOption(QStyleOptionToolBox *option) const
{
    if(!option)
        return;
    option->initFrom(this);
    if(m_bSelected)
        option->state |= QStyle::State_Selected;

    if(isDown())
        option->state |= QStyle::State_Sunken;

    option->text = text();
    option->icon = icon();

    if (option) {

        CToolBox *toolBox = static_cast<CToolBox *>(parentWidget());

        int widgetCount = toolBox->count();
        int currIndex = toolBox->currentIndex();

        if (widgetCount == 1)
        {
            option->position = QStyleOptionToolBox::OnlyOneTab;
        }
        else if (m_nIndexInPage == 0)
        {
            option->position = QStyleOptionToolBox::Beginning;
        }
        else if (m_nIndexInPage == widgetCount - 1)
        {
            option->position = QStyleOptionToolBox::End;
        }
        else
        {
            option->position = QStyleOptionToolBox::Middle;
        }

        if (currIndex == m_nIndexInPage - 1)
        {
            option->selectedPosition = QStyleOptionToolBox::PreviousIsSelected;
        }
        else if (currIndex == m_nIndexInPage + 1)
        {
            option->selectedPosition = QStyleOptionToolBox::NextIsSelected;
        }
        else
        {
            option->selectedPosition = QStyleOptionToolBox::NotAdjacent;
        }
    }
}

void CToolBoxButton::paintEvent(QPaintEvent *e)
{
    QPainter paint(this);
    QString text = QAbstractButton::text();
    QPainter *p = &paint;

    QStyleOptionToolBox opt;
    initStyleOption(&opt);
    style()->drawControl(QStyle::CE_ToolBoxTab, &opt, p, parentWidget());
}





CToolBox::CToolBox(QWidget *parent, Qt::WindowFlags f) : QFrame(parent, f), m_pCurrentPage(NULL)
{
    m_pLayout = new QVBoxLayout(this);
    m_pLayout->setMargin(0);
    setBackgroundRole(QPalette::Button);
}

CToolBox::~CToolBox()
{}


CToolBox::Page* CToolBox::page(QWidget *widget) const
{
    if(!widget)
        return NULL;

    for(PageList::const_iterator i = m_pageList.constBegin(); i!= m_pageList.constEnd();i++)
    {
        if((*i).m_pWidget == widget)
            return (Page*) &(*i);
    }
    return NULL;
}

CToolBox::Page* CToolBox::page(int index)
{
    if(index >=0 && index < m_pageList.count())
        return &m_pageList[index];

    return NULL;
}

const CToolBox::Page* CToolBox::page(int index) const
{
    if(index >=0 && index < m_pageList.count())
        return &m_pageList.at(index);

    return NULL;
}

void CToolBox::updateTabs()
{
    CToolBoxButton *lastButton = m_pCurrentPage ? m_pCurrentPage->m_pButton : 0;
    bool after = false;
    int index = 0;
    for (index = 0; index < m_pageList.count(); ++index) {
        const Page &page = m_pageList.at(index);
        CToolBoxButton *tB = page.m_pButton;
        // update indexes, since the updates are delayed, the indexes will be correct
        // when we actually paint.
        tB->setIndex(index);
        QWidget *tW = page.m_pWidget;

        if (after) {
            QPalette p = tB->palette();
            p.setColor(tB->backgroundRole(), tW->palette().color(tW->backgroundRole()));
            tB->setPalette(p);
            tB->update();
        } else if (tB->backgroundRole() != QPalette::Window) {
            tB->setBackgroundRole(QPalette::Window);
            tB->update();
        }
        after = (tB == lastButton);
    }
}


void CToolBox::relayout()
{
    delete m_pLayout;
    m_pLayout = new QVBoxLayout(this);
    m_pLayout->setMargin(0);
    for (PageList::ConstIterator i = m_pageList.constBegin(); i != m_pageList.constEnd(); ++i) {
        m_pLayout->addWidget((*i).m_pButton);
        m_pLayout->addWidget((*i).m_pScrollArea);
    }
}

void CToolBox::buttonClicked()
{
    CToolBoxButton *tb = qobject_cast<CToolBoxButton*>(sender());
    QWidget* item = 0;
    for (PageList::ConstIterator i = m_pageList.constBegin(); i != m_pageList.constEnd(); ++i)
        if ((*i).m_pButton == tb) {
            item = (*i).m_pWidget;
            break;
        }

    setCurrentIndex(indexOf(item));
}

void CToolBox::widgetDestroyed(QObject* object)
{
    // no verification - vtbl corrupted already
    QWidget *p = (QWidget*)object;

    Page *c = page(p);
    if (!p || !c)
        return;

    m_pLayout->removeWidget(c->m_pScrollArea);
    m_pLayout->removeWidget(c->m_pButton);
    c->m_pScrollArea->deleteLater(); // page might still be a child of sv
    delete c->m_pButton;

    bool removeCurrent = (c == m_pCurrentPage);
    m_pageList.removeAll(*c);

    if (!m_pageList.count())
    {
        m_pCurrentPage = NULL;
        emit currentChanged(-1);
    }
    else if (removeCurrent)
    {
        m_pCurrentPage = NULL;
        setCurrentIndex(0);
    }
}


int CToolBox::insertItem(int index, QWidget *widget, const QIcon &icon, const QString &text)
{
    if (!widget)
        return -1;

    connect(widget, SIGNAL(destroyed(QObject*)), this, SLOT(widgetDestroyed(QObject*)));

    Page c;
    c.m_pWidget = widget;
    c.m_pButton = new CToolBoxButton(this);
    c.m_pButton->setObjectName(QLatin1String("ctoolbox_toolboxbutton"));
    connect(c.m_pButton, SIGNAL(clicked()), this, SLOT(buttonClicked()));

    c.m_pScrollArea = new QScrollArea(this);
    c.m_pScrollArea->setWidget(widget);
    c.m_pScrollArea->setWidgetResizable(true);
    c.m_pScrollArea->hide();
#if 0
    c.m_pScrollArea->setFrameStyle(QFrame::NoFrame);
#else
    c.m_pScrollArea->setFrameStyle(QFrame::Panel);
#endif

    c.setText(text);
    c.setIcon(icon);

    if (index < 0 || index >= (int)m_pageList.count())
    {
        index = m_pageList.count();
        m_pageList.append(c);
        m_pLayout->addWidget(c.m_pButton);
        m_pLayout->addWidget(c.m_pScrollArea);
        if (index == 0)
            setCurrentIndex(index);
    }
    else
    {
        m_pageList.insert(index, c);
        relayout();
        if (m_pCurrentPage) {
            QWidget *current = m_pCurrentPage->m_pWidget;
            int oldindex = indexOf(current);
            if (index <= oldindex) {
                m_pCurrentPage = NULL; // trigger change
                setCurrentIndex(oldindex);
            }
        }
    }

    c.m_pButton->show();

    updateTabs();
    itemInserted(index);
    return index;
}

void CToolBox::removeItem(int index)
{
    if (QWidget *w = widget(index))
    {
        disconnect(w, SIGNAL(destroyed(QObject*)), this, SLOT(widgetDestroyed(QObject*)));
        w->setParent(this);
        // destroy internal data
        widgetDestroyed(w);
        itemRemoved(index);
    }
}

void CToolBox::setItemEnabled(int index, bool enabled)
{
    Page *c = page(index);
    if (!c)
        return;

    c->m_pButton->setEnabled(enabled);
    if (!enabled && c == m_pCurrentPage)
    {
        int curIndexUp = index;
        int curIndexDown = curIndexUp;
        const int count = m_pageList.count();
        while (curIndexUp > 0 || curIndexDown < count-1)
        {
            if (curIndexDown < count-1)
            {
                if (page(++curIndexDown)->m_pButton->isEnabled())
                {
                    index = curIndexDown;
                    break;
                }
            }

            if (curIndexUp > 0)
            {
                if (page(--curIndexUp)->m_pButton->isEnabled())
                {
                    index = curIndexUp;
                    break;
                }
            }
        }
        setCurrentIndex(index);
    }
}

bool CToolBox::isItemEnabled(int index) const
{
    const Page *c = page(index);
    return c && c->m_pButton->isEnabled();
}


void CToolBox::setItemText(int index, const QString &text)
{
    Page *c = page(index);
    if (c)
        c->setText(text);
}

QString CToolBox::itemText(int index) const
{
    const Page *c = page(index);
    return (c ? c->text() : QString());
}

void CToolBox::setItemIcon(int index, const QIcon &icon)
{
    Page *c = page(index);
    if (c)
        c->setIcon(icon);
}

QIcon CToolBox::itemIcon(int index) const
{
    const Page *c = page(index);
    return (c ? c->icon() : QIcon());
}

void CToolBox::setItemToolTip(int index, const QString &toolTip)
{
    Page *c = page(index);
    if (c)
        c->setToolTip(toolTip);
}

QString CToolBox::itemToolTip(int index) const
{
    const Page *c = page(index);
    return (c ? c->toolTip() : QString());
}

void CToolBox::setCurrentIndex(int index)
{
    Page *c = page(index);
    if (!c || m_pCurrentPage == c)
        return;

    c->m_pButton->setSelected(true);
    if (m_pCurrentPage) {
        m_pCurrentPage->m_pScrollArea->hide();
        m_pCurrentPage->m_pButton->setSelected(false);
    }
    m_pCurrentPage = c;
    m_pCurrentPage->m_pScrollArea->show();
    updateTabs();
    emit currentChanged(index);
}

void CToolBox::setCurrentWidget(QWidget *widget)
{
    int i = indexOf(widget);
    if (i >= 0)
        setCurrentIndex(i);
    else
        qWarning("QToolBox::setCurrentWidget: widget not contained in tool box");
}

int CToolBox::currentIndex() const
{
    return m_pCurrentPage ? indexOf(m_pCurrentPage->m_pWidget) : -1;
}

QWidget *CToolBox::currentWidget() const
{
    return m_pCurrentPage ? m_pCurrentPage->m_pWidget : 0;
}

QWidget *CToolBox::widget(int index) const
{
    if (index < 0 || index >= (int) m_pageList.size())
        return NULL;
    return m_pageList.at(index).m_pWidget;
}

int CToolBox::indexOf(QWidget *widget) const
{
    Page *c = (widget ? page(widget) : 0);
    return c ? m_pageList.indexOf(*c) : -1;
}

int CToolBox::count() const
{
    return m_pageList.count();
}

bool CToolBox::event(QEvent *e)
{
    return QFrame::event(e);
}

void CToolBox::itemInserted(int index)
{
    Q_UNUSED(index)
}
void CToolBox::itemRemoved(int index)
{
    Q_UNUSED(index)
}

void CToolBox::showEvent(QShowEvent *e)
{
    QWidget::showEvent(e);
}

void CToolBox::changeEvent(QEvent *ev)
{
    if(ev->type() == QEvent::StyleChange)
        updateTabs();
    QFrame::changeEvent(ev);
}


