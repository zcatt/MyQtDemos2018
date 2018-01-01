#include <QtWidgets>

#include "cdraftscene.h"
#include "cdraftview.h"
#include "cshapeitem.h"
#include "clinkitem.h"
#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent): QMainWindow(parent)
{
    CreateToolBox();
    AddShapeItems();

    QDockWidget *dock = new QDockWidget;
    dock->setWindowTitle(tr("Tools"));
    dock->setWidget(m_pToolBox);
    addDockWidget(Qt::LeftDockWidgetArea,dock);

    m_pGScene = new CDraftScene(this);
    m_pGScene->setSceneRect(QRectF(0, 0, 5000, 5000));
    m_pGView = new CDraftView(m_pGScene);

    setCentralWidget(m_pGView);

    setWindowTitle(tr("Basic Graphic Shapes"));

    createMenuAndToolBar();
}

MainWindow::~MainWindow()
{}

void MainWindow::CreateToolBox(void)
{
    m_pShapes = new QListWidget;
    m_pAnnotations = new QListWidget;

    m_pToolBox = new QToolBox;
    m_pToolBox->addItem(m_pShapes, tr("Shape"));
    m_pToolBox->addItem(m_pAnnotations, tr("Annotation"));
}

void MainWindow::AddShapeItems(void)
{
    QListWidgetItem *item;

    item = new QListWidgetItem(tr("Select"));
    m_pShapes->addItem(item);

    item = new QListWidgetItem(QIcon(":/images/emotion1.png"), tr("line"));
    //item->setFlags(item->flags()& ~(Qt::ItemIsSelectable | Qt::ItemIsUserCheckable/* | Qt::ItemIsEnabled*/));
    item->setData(Qt::UserRole, QVariant(CShapeItem::ShapeType_Link));
    m_pShapes->addItem(item);

    item = new QListWidgetItem(QIcon(":/images/emotion2.png"), tr("triangle"));
    item->setData(Qt::UserRole, QVariant(CShapeItem::ShapeType_Triangle));
    m_pShapes->addItem(item);

    item = new QListWidgetItem(QIcon(":/images/emotion3.png"), tr("circle"));
    item->setData(Qt::UserRole, QVariant(CShapeItem::ShapeType_Circle));
    m_pShapes->addItem(item);

    item = new QListWidgetItem(QIcon(":/images/emotion4.png"), tr("rect"));
    item->setData(Qt::UserRole, QVariant(CShapeItem::ShapeType_Rectangle));
    m_pShapes->addItem(item);
}

int MainWindow::GetCurrentShapeType(void)
{
    QWidget *w;
    w = m_pToolBox->currentWidget();
    if(!w)
    {
        qDebug()<<"no currentWidget in m_pToolBox";
        return CShapeItem::ShapeType_0;
    }

    QListWidget *pListWidget = qobject_cast<QListWidget*>(w);

    if(!pListWidget)
    {
        qDebug()<<"not a listWidget";
        return CShapeItem::ShapeType_0;
    }

    QListWidgetItem *pItem;
    pItem = pListWidget->currentItem();

    if(!pItem)
    {
        qDebug()<<"no current item.";
        return CShapeItem::ShapeType_0;
    }

    return pItem->data(Qt::UserRole).toInt();
}

void MainWindow::ResetShapeType(void)
{
    QWidget *w;
    w = m_pToolBox->currentWidget();
    if(!w)
    {
        qDebug()<<"no currentWidget in m_pToolBox";
        return;
    }

    QListWidget *pListWidget = qobject_cast<QListWidget*>(w);

    if(!pListWidget)
    {
        qDebug()<<"not a listWidget";
        return;
    }

    pListWidget->setCurrentRow(0);
    //pListWidget->setCurrentRow(-1);
}


void MainWindow::createMenuAndToolBar()
{
    QAction *toFrontAction = new QAction(QIcon(":/images/bringtofront.png"),
                                tr("Bring to &Front"), this);
    toFrontAction->setShortcut(tr("Ctrl+F"));
    toFrontAction->setStatusTip(tr("Bring item to front"));
    connect(toFrontAction, SIGNAL(triggered()), this, SLOT(bringToFront()));

    QAction *sendBackAction = new QAction(QIcon(":/images/sendtoback.png"), tr("Send to &Back"), this);
    sendBackAction->setShortcut(tr("Ctrl+T"));
    sendBackAction->setStatusTip(tr("Send item to back"));
    connect(sendBackAction, SIGNAL(triggered()), this, SLOT(sendToBack()));

    QAction *deleteAction = new QAction(QIcon(":/images/delete.png"), tr("&Delete"), this);
    deleteAction->setShortcut(tr("Delete"));
    deleteAction->setStatusTip(tr("Delete item from diagram"));
    connect(deleteAction, SIGNAL(triggered()), this, SLOT(deleteItem()));

    QAction *zoomInAction = new QAction(QIcon(":/images/zoomin.png"), tr("Zoom on"), this);
    zoomInAction->setShortcut(tr("+"));
    zoomInAction->setStatusTip(tr("Zoom in."));
    connect(zoomInAction, SIGNAL(triggered()), this, SLOT(zoomIn()));

    QAction *zoomOutAction = new QAction(QIcon(":/images/zoomout.png"), tr("Zoom out"), this);
    zoomOutAction->setShortcut(tr("-"));
    zoomOutAction->setStatusTip(tr("Zoom out."));
    connect(zoomOutAction, SIGNAL(triggered()), this, SLOT(zoomOut()));


    QAction *aboutAction = new QAction(tr("A&bout"), this);
    aboutAction->setShortcut(tr("F1"));
    connect(aboutAction, SIGNAL(triggered()), this, SLOT(about()));

    QMenu* itemMenu = menuBar()->addMenu(tr("&Item"));
    itemMenu->addAction(deleteAction);
    itemMenu->addSeparator();
    itemMenu->addAction(toFrontAction);
    itemMenu->addAction(sendBackAction);

    QMenu* viewMenu = menuBar()->addMenu(tr("&View"));
    viewMenu->addAction(zoomInAction);
    viewMenu->addAction(zoomOutAction);

    QMenu* aboutMenu = menuBar()->addMenu(tr("&Help"));
    aboutMenu->addAction(aboutAction);

    QToolBar *editToolBar = addToolBar(tr("Edit"));
    editToolBar->addAction(deleteAction);
    editToolBar->addAction(toFrontAction);
    editToolBar->addAction(sendBackAction);
    editToolBar->addAction(zoomInAction);
    editToolBar->addAction(zoomOutAction);

}


void MainWindow::zoomIn(void)
{
    QMatrix oldMatrix = m_pGView->matrix();
    //m_pGView->resetMatrix();
    m_pGView->translate(oldMatrix.dx(), oldMatrix.dy());

    m_pGView->scale(2.0, 2.0);
}

void MainWindow::zoomOut(void)
{
    QMatrix oldMatrix = m_pGView->matrix();
    //m_pGView->resetMatrix();
    m_pGView->translate(oldMatrix.dx(), oldMatrix.dy());

    m_pGView->scale(0.5, 0.5);

}

void MainWindow::deleteItem(void)
{
    foreach (QGraphicsItem *item, m_pGScene->selectedItems()) {
        m_pGScene->removeItem(item);
        delete item;
     }
}

void MainWindow::bringToFront(void)
{
    if (m_pGScene->selectedItems().isEmpty())
        return;

    QGraphicsItem *selectedItem = m_pGScene->selectedItems().first();
    QList<QGraphicsItem *> overlapItems = selectedItem->collidingItems();

    qreal zValue = 0;
    foreach (QGraphicsItem *item, overlapItems) {
        if (item->zValue() >= zValue)
            zValue = item->zValue() + 0.1;
    }
    selectedItem->setZValue(zValue);
}

void MainWindow::sendToBack(void)
{
    if (m_pGScene->selectedItems().isEmpty())
        return;

    QGraphicsItem *selectedItem = m_pGScene->selectedItems().first();
    QList<QGraphicsItem *> overlapItems = selectedItem->collidingItems();

    qreal zValue = 0;
    foreach (QGraphicsItem *item, overlapItems) {
        if (item->zValue() <= zValue)
            zValue = item->zValue() - 0.1;
    }
    selectedItem->setZValue(zValue);
}

void MainWindow::about(void)
{
    QMessageBox::about(this, tr("About Basic Graphics Shape"),
                   tr("The <b>BasicGraphicsShape</b> example shows "
                      "use of the graphics framework and some tips."));
}
