#ifndef CICONITEMDELEGATE_H
#define CICONITEMDELEGATE_H

#include <QItemDelegate>

class CIconItemDelegate : public QItemDelegate
{
    Q_OBJECT

public:
    CIconItemDelegate(QObject *parent = Q_NULLPTR);
    void paint(QPainter *painter,
               const QStyleOptionViewItem &option,
               const QModelIndex &index) const override;

};

#endif // CICONITEMDELEGATE_H
