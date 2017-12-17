#include <QtWidgets>
#include <QApplication>

#include "ciconitemdelegate.h"

CIconItemDelegate::CIconItemDelegate(QObject *parent): QItemDelegate(parent)
{

}

void CIconItemDelegate::paint(QPainter *painter,
           const QStyleOptionViewItem &option,
           const QModelIndex &index) const
{
    if (!index.isValid())
        return;

    QStyleOptionViewItem opt = setOptions(index, option);

    painter->save();
    painter->setClipRect(opt.rect);



    QRect decorationRect;
    decorationRect = opt.rect;

    drawBackground(painter, opt, index);

    QPixmap pm;
    pm = index.data(Qt::DecorationRole).value<QPixmap>();

    qApp->style()->drawItemPixmap(painter, decorationRect, Qt::AlignCenter, pm);

    drawFocus(painter, opt, decorationRect);



    painter->restore();
}
