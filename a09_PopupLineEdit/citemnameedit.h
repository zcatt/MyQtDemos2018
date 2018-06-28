#ifndef CITEMNAMEEDIT_H
#define CITEMNAMEEDIT_H

#include <QDialog>

class QGraphicsItem;
class QLineEdit;

class CItemNameEdit: public QDialog
{
    Q_OBJECT

public:
    CItemNameEdit(QGraphicsItem *parent);
    ~CItemNameEdit();

    static QString getName(QGraphicsItem *parent, const QString &text, int nMaxLen = 32);

public slots:
    void textChanged(const QString &text);

public:
    QLineEdit *m_lineEdit;

};

#endif // CITEMNAMEEDIT_H
