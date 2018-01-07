#include <QtWidgets>

#include "citemnameedit.h"

CItemNameEdit::CItemNameEdit(QGraphicsItem *parent)
        : QDialog(parent->scene()->views().first(), Qt::FramelessWindowHint)
{
    setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);

    QHBoxLayout *mainLayout = new QHBoxLayout(this);
    mainLayout->setContentsMargins(0,0,0,0);
    m_lineEdit = new QLineEdit;
    m_lineEdit->setTextMargins(0,0,0,0);
    m_lineEdit->setMaxLength(32);
    m_lineEdit->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
    //m_lineEdit->setFrame(false);
    mainLayout->addWidget(m_lineEdit);

    QPointF pf = parent->mapToScene(parent->boundingRect().center());
    QGraphicsView *view = parent->scene()->views().first();
    QPoint pt = view->mapFromScene(pf);
    pt = view->mapToGlobal(pt);
    move(pt);

    connect(m_lineEdit, &QLineEdit::textChanged, this, textChanged);
    connect(m_lineEdit, &QLineEdit::editingFinished, this, &QDialog::accept);
}

CItemNameEdit::~CItemNameEdit()
{}

CItemNameEdit::textChanged(const QString &text)
{
    QFontMetrics fm(m_lineEdit->font());
    int h = height();
    int w = fm.width(text) + 10;        // 10, see QLineEdit::sizeHint()
    resize(w,h);
}

QString CItemNameEdit::getName(QGraphicsItem *parent, const QString &text, int nMaxLen)
{
    CItemNameEdit *dialog = new CItemNameEdit(parent);
    dialog->m_lineEdit->setText(text);
    if(nMaxLen > 0)
        dialog->m_lineEdit->setMaxLength(nMaxLen);

    const int ret = dialog->exec();

    return dialog->m_lineEdit->text();
}

