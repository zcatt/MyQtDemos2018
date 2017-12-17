/*
    需求，
    .2 table views  <-->  1 model
    .inplace edit
    .one address entry contains columns of name, icon,  address
    .show icon in icon column
    .all columns are sortable except icon. sortable feature is supported by reimplementing sort() and not QSortFilterProxyModel

    说明，
    .简单QAbstractTableModel子类实现可以参考QAbstractListModel----QStringListModel
    .QAbstractTableModel子类必须实现rowCount(), columnCount()和data().
    .若editable，则还需实现setData()，以及flags()返回Qt::ItemIsEditable.
    .若可insert/remove items，则需实现insertRows()和removeRows(), insertColumns()和removeColumns()
    .TODO, headerView某列禁止sort，则可以考虑重载mousePress()和mouseRelease(), 检测点按该列时设置setSectionClickable(false)。
        void MyHeaderView::mousePressEvent(QMouseEvent* event)
        {
             const int index = logicalIndexAt(event->pos());
             if (0 <= m_forbinSortList.indexOf(index))
                  setSectionsClickable(false);

             QHeaderView::mousePressEvent(event);

            if (!sectionsClickable())
                  setSectionsClickable(true);
        }

        void MyHeaderView::mouseReleaseEvent(QMouseEvent* event)
        {
             const int index = logicalIndexAt(event->pos());
             if (0 <= m_forbinSortList.indexOf(index))
                  setSectionsClickable(false);

             QHeaderView::mouseReleaseEvent(event);

             if (!sectionsClickable())
                  setSectionsClickable(true);
        }

    .CIconItemDelegate::paint()完成icon列的图形居中显示。 默认的QStyledItemDelegate对于DecorationRole的显示是偏左的，并且无法改变。



*/
#include "mainwindow.h"
#include <QApplication>


void addEntries(MainWindow& w)
{
    QPixmap icon;
    icon.load(":/images/emotion1.png");
    w.addEntry("Tom", icon, "Tom Address.");
    icon.load(":/images/emotion2.png");
    w.addEntry("Smith", icon, "Smith Address.");
    icon.load(":/images/emotion3.png");
    w.addEntry("Kat", icon, "Kat Address.");
    icon.load(":/images/emotion4.png");
    w.addEntry("Matty", icon, "Matty Address.");
}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    addEntries(w);
    w.setGeometry(100,100,800,500);
    w.show();

    return a.exec();
}
