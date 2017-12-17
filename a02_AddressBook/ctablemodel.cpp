#include <QDebug>

#include "ctablemodel.h"


CTableModel::CTableModel(QObject *parent): QAbstractTableModel(parent)
{}

CTableModel::~CTableModel()
{
    clear();
}

void CTableModel::clear()
{
    beginResetModel();
    for(int i = 0; i< m_items.count();i++)
    {
        if(m_items.at(i))
        {
            delete m_items.at(i);
            m_items[i] = 0;
        }
    }
    endResetModel();
}



int CTableModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);

    return m_items.count();
}

int CTableModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    //name, icon, address
    return 3;
}


QVariant CTableModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role != Qt::DisplayRole || orientation != Qt::Horizontal)
        return QVariant();

    if(section == 0)
        return tr("Name");
    else if (section == 1)
        return tr("Icon");
    else
        return tr("Address");
}

Qt::ItemFlags CTableModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return Qt::ItemIsEnabled;

    return QAbstractTableModel::flags(index) | (index.column() != 1 ? Qt::ItemIsEditable : Qt::NoItemFlags);
}

QVariant CTableModel::data(const QModelIndex &index, int role) const
{
    if(index.isValid())
    {
        CAddressBookItem *item = m_items.at(index.row());
        if (role == Qt::DisplayRole)
        {
            if(index.column() == 0)
                return item->m_name;
            else if(index.column() == 2)
                return item->m_address;
        }
        else if (role == Qt::DecorationRole)
        {
            if (index.column() == 1)
                return item->m_icon;
        }
        else if (role == Qt::TextAlignmentRole)
        {
            return Qt::AlignCenter;
        }

    }
    return QVariant();
}

bool CTableModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if(!index.isValid()
            || index.row() <0 || index.row()>= m_items.count()
            || role != Qt::EditRole)
        return false;

    CAddressBookItem *item = m_items.at(index.row());
    Q_ASSERT(item != 0);
    if(index.column() == 0)
        item->m_name = value.toString();
    else if (index.column() == 1)
        item->m_icon = value.value<QPixmap>();
    else
        item->m_address = value.toString();

    emit dataChanged(index, index);

    return true;
}

bool CTableModel::insertRows(int position, int rows, const QModelIndex &index)
{
    Q_UNUSED(index);
    beginInsertRows(QModelIndex(), position, position + rows - 1);

    for (int row = 0; row < rows; ++row) {
        CAddressBookItem *item = new CAddressBookItem;
        m_items.insert(position, item);
    }

    endInsertRows();
    return true;
}

bool CTableModel::removeRows(int position, int rows, const QModelIndex &index)
{
    Q_UNUSED(index);
    beginRemoveRows(QModelIndex(), position, position + rows - 1);

    for (int row = 0; row < rows; ++row) {
        m_items.removeAt(position);
    }

    endRemoveRows();
    return true;
}


bool CTableModel::nameLessThan(const QPair<CAddressBookItem*,int> &left,
                               const QPair<CAddressBookItem*,int> &right)
{
    return left.first->m_name < right.first->m_name;
}

bool CTableModel::nameGreaterThan(const QPair<CAddressBookItem*,int> &left,
                                  const QPair<CAddressBookItem*,int> &right)
{
    return left.first->m_name > right.first->m_name;
}

bool CTableModel::addressLessThan(const QPair<CAddressBookItem*,int> &left,
                               const QPair<CAddressBookItem*,int> &right)
{
    return left.first->m_address < right.first->m_address;
}

bool CTableModel::addressGreaterThan(const QPair<CAddressBookItem*,int> &left,
                                  const QPair<CAddressBookItem*,int> &right)
{
    return left.first->m_address > right.first->m_address;
}



void CTableModel::sort(int column, Qt::SortOrder order)
{
    // icon column not support sort.
    if(column == 1)
        return;

    QVector<QPair<CAddressBookItem*, int> > sortTable;

    sortTable.reserve(m_items.count());

    for (int row = 0; row < m_items.count(); ++row) {
        CAddressBookItem *itm = m_items.at(row);
        sortTable.append(QPair<CAddressBookItem*,int>(itm, row));
    }

    LessThan compare;

    //name and address use the same sort algorithm
    if(column == 0)
        compare = (order == Qt::AscendingOrder ? &nameLessThan : &nameGreaterThan);
    else
        compare = (order == Qt::AscendingOrder ? &addressLessThan : &addressGreaterThan);

    std::stable_sort(sortTable.begin(), sortTable.end(), compare);

    QVector<CAddressBookItem*> sorted;
    QModelIndexList from;
    QModelIndexList to;
    const int numRows = m_items.count();
    const int numColumns = 3;
    sorted.reserve(numRows);
    from.reserve(numRows * numColumns);
    to.reserve(numRows * numColumns);

    for (int i = 0; i < numRows; ++i) {

        int r =  sortTable.at(i).second;

        sorted.append(m_items.at(r));
        m_items[r] = 0;

        for (int c = 0; c < numColumns; ++c) {
            from.append(createIndex(r, c));
            to.append(createIndex(i, c));
        }
    }


    emit layoutAboutToBeChanged();

    m_items = sorted;
    changePersistentIndexList(from, to);

    emit layoutChanged();
}
