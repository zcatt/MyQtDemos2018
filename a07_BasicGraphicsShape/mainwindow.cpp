#include <QtWidgets>

#include "cdraftscene.h"
#include "cshapeitem.h"
#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent): QMainWindow(parent)
{
    CreateToolBox();
    AddShapeItems();


    m_pGScene = new CDraftScene(this);
    m_pGScene->setSceneRect(QRectF(0, 0, 500, 500));
    m_pGView = new QGraphicsView(m_pGScene);


    QDockWidget *dock = new QDockWidget;
    dock->setWindowTitle(tr("Tools"));
    dock->setWidget(m_pToolBox);
    addDockWidget(Qt::LeftDockWidgetArea,dock);

    setCentralWidget(m_pGView);

    setWindowTitle(tr("Basic Graphic Shapes"));
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
    item->setData(Qt::UserRole, QVariant(CShapeItem::ShapeType_Line));
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
}
