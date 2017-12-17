#ifndef CTABLEMODEL_H
#define CTABLEMODEL_H

#include <QAbstractTableModel>
#include <QList>
#include <QPixmap>
#include <QPair>

class CAddressBookItem
{
public:
    QString m_name;
    QPixmap m_icon;
    QString m_address;
};

typedef bool(*LessThan)(const QPair<CAddressBookItem*,int>&,const QPair<CAddressBookItem*,int>&);

class CTableModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    CTableModel(QObject *parent = 0);
    ~CTableModel();
    void clear();

    int rowCount(const QModelIndex &parent) const override;
    int columnCount(const QModelIndex &parent) const override;
    QVariant data(const QModelIndex &index, int role) const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const override;
    Qt::ItemFlags flags(const QModelIndex &index) const override;
    bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole) override;
    bool insertRows(int position, int rows, const QModelIndex &index = QModelIndex()) override;
    bool removeRows(int position, int rows, const QModelIndex &index = QModelIndex()) override;
    void sort(int column, Qt::SortOrder order) override;
    static bool nameLessThan(const QPair<CAddressBookItem*,int> &left, const QPair<CAddressBookItem*,int> &right);
    static bool nameGreaterThan(const QPair<CAddressBookItem*,int> &left, const QPair<CAddressBookItem*,int> &right);
    static bool addressLessThan(const QPair<CAddressBookItem*,int> &left, const QPair<CAddressBookItem*,int> &right);
    static bool addressGreaterThan(const QPair<CAddressBookItem*,int> &left, const QPair<CAddressBookItem*,int> &right);


public:
    QVector<CAddressBookItem*> m_items;
};

#endif // CTABLEMODEL_H
