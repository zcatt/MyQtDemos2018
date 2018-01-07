#include <QtWidgets>

#include "citemnameedit.h"
#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), m_scene(0), m_view(0), m_rectItem(0), m_textItem(0)
{
    m_scene = new QGraphicsScene;
    m_scene->setSceneRect(0,0, 400,300);
    m_view = new QGraphicsView(m_scene);
    setCentralWidget(m_view);

    setWindowTitle(tr("a08_PopupLineEdit"));

    QAction *newRectAction = new QAction(tr("New &rect item"), this);
    newRectAction->setShortcut(tr("F1"));
    connect(newRectAction, SIGNAL(triggered()), this, SLOT(newRect()));

    QAction *popupLineEditAction = new QAction(tr("Popup &line edit"), this);
    popupLineEditAction->setShortcut(tr("F2"));
    connect(popupLineEditAction, SIGNAL(triggered()), this, SLOT(popupLineEdit()));

    QMenu* itemMenu = menuBar()->addMenu(tr("&Edit"));
    itemMenu->addAction(newRectAction);
    itemMenu->addAction(popupLineEditAction);
}

MainWindow::~MainWindow()
{

}


void MainWindow::newRect(void)
{
    static int i =0;
    QPointF pos;
    QGraphicsItem *item = m_scene->focusItem();

    if(item == 0 && !m_scene->items().isEmpty())
    {
        item = m_scene->items().first();
    }

    if(item != 0)
    {
        pos = item->boundingRect().bottomRight();
        pos = item->mapToScene(pos);
    }
    else
    {
        pos = m_view->mapToScene(m_view->width()/2, m_view->height()/2);
    }

    QGraphicsRectItem *rectItem;
    rectItem = new QGraphicsRectItem;
    rectItem->setFlag(QGraphicsItem::ItemIsMovable, true);
    rectItem->setFlag(QGraphicsItem::ItemIsSelectable, true);

    rectItem->setBrush(i++%2 ? Qt::red : Qt::green);

    m_scene->addItem(rectItem);
    rectItem->setRect(0,0, 100, 50);
    rectItem->setPos(pos.x(), pos.y());
    m_view->ensureVisible(rectItem);
    //update();

}

void MainWindow::popupLineEdit(void)
{
    QPointF pos;
    QGraphicsItem *item = m_scene->focusItem();

    if(item == 0 && !m_scene->items().isEmpty())
    {
        item = m_scene->items().first();
    }

    if(item == 0)
        return;

    QRectF rectf;
    rectf = item->boundingRect();
    pos = rectf.center();
    pos = item->mapToScene(pos);
    pos = m_scene->views().first()->mapFromScene(pos);

    QString name;
    name = CItemNameEdit::getName(item, tr("name"));
    if(name.isEmpty())
        return;


    QGraphicsTextItem *textItem = m_scene->addText(name);
    textItem->setPos(pos);
}
