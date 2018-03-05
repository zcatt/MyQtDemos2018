#include "preheader.h"

#include "shapeitem.h"
#include "draftscene.h"
#include "draftview.h"

#include "mainwindow.h"

MainWindow *mainWin = 0;

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent)
{
    m_pGView = new CDraftView;
    setCentralWidget(m_pGView);

    m_pGScene = new CDraftScene(this);
    //m_pGScene->setSceneRect(QRectF(0, 0, 5000, 5000));
    m_pGScene->setSceneRect(QRectF(0, 0, 500, 300));
    connect(m_pGScene, &CDraftScene::itemInserted, this, &MainWindow::itemInserted);
    m_pGView->setScene(m_pGScene);

    CreateToolBox();
    AddShapeItems();

    createMenuAndToolBar();
    createStatusBar();

    QDockWidget *dock = new QDockWidget;
    dock->setWindowTitle(tr("Tools"));
    dock->setWidget(m_pToolBox);
    addDockWidget(Qt::LeftDockWidgetArea,dock);


    setWindowTitle(tr("Basic Graphic Shapes"));


    mainWin = this;
}

MainWindow::~MainWindow()
{
    mainWin = 0;
}

void MainWindow::CreateToolBox(void)
{
    m_pToolBox = new QToolBox;

    m_pShapes = new QListWidget;
    m_pToolBox->addItem(m_pShapes, tr("Shape"));
}

void MainWindow::AddShapeItems(void)
{
    QListWidgetItem *item;

    item = new QListWidgetItem(tr("Select"));
    item->setData(Qt::UserRole, QVariant(CShapeItem::ShapeType_Select));
    m_pShapes->addItem(item);

    item = new QListWidgetItem(QIcon(":/images/emotion1.png"), tr("line"));
    //item->setFlags(item->flags()& ~(Qt::ItemIsSelectable | Qt::ItemIsUserCheckable/* | Qt::ItemIsEnabled*/));
    item->setData(Qt::UserRole, QVariant(CShapeItem::ShapeType_Line));
    m_pShapes->addItem(item);

    item = new QListWidgetItem(QIcon(":/images/emotion2.png"), tr("rect"));
    item->setData(Qt::UserRole, QVariant(CShapeItem::ShapeType_Rect));
    m_pShapes->addItem(item);

    item = new QListWidgetItem(QIcon(":/images/emotion3.png"), tr("text"));
    item->setData(Qt::UserRole, QVariant(CShapeItem::ShapeType_Text));
    m_pShapes->addItem(item);

    connect(m_pShapes, &QListWidget::currentItemChanged, this, &MainWindow::currentShapeChanged);

}

int MainWindow::GetCurrentShapeType(void)
{
    QWidget *w;
    w = m_pToolBox->currentWidget();
    if(!w)
    {
        qDebug()<<"no currentWidget in m_pToolBox";
        return C2DItem::ShapeType_None;
    }

    QListWidget *pListWidget = qobject_cast<QListWidget*>(w);

    if(!pListWidget)
    {
        qDebug()<<"not a listWidget";
        return C2DItem::ShapeType_2D;
    }

    QListWidgetItem *pItem;
    pItem = pListWidget->currentItem();

    if(!pItem)
    {
        qDebug()<<"no current item.";
        return C2DItem::ShapeType_2D;
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

    //TODO, deleteAction->setEnabled(false);

    QAction *zoomInAction = new QAction(QIcon(":/images/zoomin.png"), tr("Zoom on"), this);
    zoomInAction->setShortcut(QString("+"));
    zoomInAction->setStatusTip(tr("Zoom in."));
    connect(zoomInAction, SIGNAL(triggered()), this, SLOT(zoomIn()));

    QAction *zoomOutAction = new QAction(QIcon(":/images/zoomout.png"), tr("Zoom out"), this);
    zoomOutAction->setShortcut(QString("-"));
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

void MainWindow::createStatusBar(void)
{
    QStatusBar *sb = statusBar();

    m_statusBarPos= new QLabel;
    sb->addPermanentWidget(m_statusBarPos);

    //QGraphicsView默认在开始时是关闭mouseTracking的，直到加入一个QGraphicsItem为止.
    m_pGView->viewport()->setMouseTracking(true);
}

void MainWindow::showMousePos(QPoint ptView)
{
    QPointF pt = m_pGView->mapToScene(ptView);

    QString s;

    s= QString("View(%1,%2) Scene(%3,%4)").arg(ptView.x()).arg(ptView.y()).arg(pt.x()).arg(pt.y());
    m_statusBarPos->setText(s);

    statusBar()->showMessage(s);
}


void MainWindow::currentShapeChanged(QListWidgetItem *current, QListWidgetItem *previous)
{
    Q_UNUSED(previous);
    qDebug()<<"update curShapeType="<<current->data(Qt::UserRole).toInt();
    m_pGScene->m_nCurShapeType = current->data(Qt::UserRole).toInt();
}

void MainWindow::itemInserted(QGraphicsItem *item)
{
    Q_UNUSED(item);
    //当前选择的shape重置为select.
#if 1
    ResetShapeType();
#endif
}

void MainWindow::zoomIn(void)
{
#if 1
    QTransform oldMatrix = m_pGView->transform();
    m_pGView->translate(oldMatrix.dx(), oldMatrix.dy());
    m_pGView->scale(2.0, 2.0);
#else
    qDebug()<<"zoom In";

    //m_pGView->setTransformationAnchor(QGraphicsView::NoAnchor);

    QTransform matrix = m_pGView->transform();
    QTransform vpm = m_pGView->viewportTransform();

    qDebug()<<"         old matrix="<<matrix;
    qDebug()<<"         old vpm="<<vpm;
    m_pGView->translate(50, 50);
    matrix = m_pGView->transform();
    vpm = m_pGView->viewportTransform();
    qDebug()<<"         new matrix="<<matrix;
    qDebug()<<"         new vpm="<<vpm;
#endif
}

void MainWindow::zoomOut(void)
{
#if 1
    QTransform oldMatrix = m_pGView->transform();
    m_pGView->translate(oldMatrix.dx(), oldMatrix.dy());
    m_pGView->scale(0.5, 0.5);

#else
    qDebug()<<"zoom Out";
    //m_pGView->setTransformationAnchor(QGraphicsView::NoAnchor);

    QTransform matrix = m_pGView->transform();
    QTransform vpm = m_pGView->viewportTransform();
    qDebug()<<"zoomOut, old matrix="<<matrix;
    qDebug()<<"         old vpm="<<vpm;
    m_pGView->translate(-50, -50);
    matrix = m_pGView->transform();
    vpm = m_pGView->viewportTransform();
    qDebug()<<"         new matrix="<<matrix;
    qDebug()<<"         new vpm="<<vpm;

#endif
}

void MainWindow::deleteItem(void)
{
    foreach (QGraphicsItem *item, m_pGScene->selectedItems()) {
        //m_pGScene->removeItem(item);
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
