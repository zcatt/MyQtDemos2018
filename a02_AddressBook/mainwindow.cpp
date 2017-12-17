#include <QtWidgets>
#include "ctablemodel.h"
#include "ciconitemdelegate.h"
#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{

    m_model = new CTableModel(this);

    QTableView *view1 = new QTableView;
    QTableView *view2 = new QTableView;

    view1->setModel(m_model);
    view1->setSelectionBehavior(QAbstractItemView::SelectRows);
    view1->horizontalHeader()->setStretchLastSection(true);
    view1->verticalHeader()->setHidden(true);
    view1->setEditTriggers(QAbstractItemView::NoEditTriggers);
    view1->setSelectionMode(QAbstractItemView::SingleSelection);
    view1->setSortingEnabled(true);

    CIconItemDelegate *delegate = new CIconItemDelegate(this);
    view1->setItemDelegateForColumn(1, delegate);

    view2->setModel(m_model);
    view2->setSelectionBehavior(QAbstractItemView::SelectRows);
    view2->horizontalHeader()->setStretchLastSection(true);
    view2->verticalHeader()->setHidden(true);
    view2->setEditTriggers(QAbstractItemView::NoEditTriggers);
    view2->setSelectionMode(QAbstractItemView::SingleSelection);
    view2->setSortingEnabled(true);

    QWidget *widget = new QWidget;
    QHBoxLayout *layout = new QHBoxLayout;
    widget->setLayout(layout);
    layout->addWidget(view1);
    layout->addWidget(view2);
    setCentralWidget(widget);
}

MainWindow::~MainWindow()
{

}

bool MainWindow::addEntry(QString name, QPixmap icon, QString address)
{
    m_model->insertRow(0, QModelIndex());
    QModelIndex index;
    index = m_model->index(0,0, QModelIndex());
    m_model->setData(index, name, Qt::EditRole);
    index = m_model->index(0,1, QModelIndex());
    m_model->setData(index, icon, Qt::EditRole);
    index = m_model->index(0,2, QModelIndex());
    m_model->setData(index, address, Qt::EditRole);
    return true;
}
