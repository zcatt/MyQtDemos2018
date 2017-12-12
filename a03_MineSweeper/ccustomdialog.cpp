#include <QtWidgets>

#include "cminewidget.h"
#include "ccustomdialog.h"

CCustomDialog::CCustomDialog(int nWidth, int nHeight, int nMines, QWidget* parent): QDialog(parent)
{
    nHeight = qBound(CMineWidget::BoardHeightMin, nHeight, CMineWidget::BoardHeightMax);
    nWidth = qBound(CMineWidget::BoardWidthMin, nWidth, CMineWidget::BoardWidthMax);
    nMines = qBound(CMineWidget::MineCountMin, nMines, static_cast<int>(nWidth * nWidth * CMineWidget::MineRatio));

    QGridLayout *qlayout = new QGridLayout;

    QLabel *label;
    label = new QLabel(tr("Height:"));
    qlayout->addWidget(label, 0, 0, Qt::AlignRight);
    label = new QLabel(tr("Width:"));
    qlayout->addWidget(label, 1, 0, Qt::AlignRight);
    label = new QLabel(tr("Mines:"));
    qlayout->addWidget(label, 2, 0, Qt::AlignRight);

    QString s;

    m_inputHeight = new QLineEdit(s.setNum(nHeight));
    qlayout->addWidget(m_inputHeight, 0,1);
    m_inputWidth = new QLineEdit(s.setNum(nWidth));
    qlayout->addWidget(m_inputWidth, 1,1);

    m_inputMines = new QLineEdit(s.setNum(nMines));
    qlayout->addWidget(m_inputMines, 2,1);


    QVBoxLayout *vlayout = new QVBoxLayout;

    QDialogButtonBox *buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, Qt::Vertical, this);
    connect(buttonBox, &QDialogButtonBox::rejected, this, &QDialog::reject);
    connect(buttonBox, &QDialogButtonBox::accepted, this, &QDialog::accept);

    vlayout->addWidget(buttonBox);

    QHBoxLayout *hlayout = new QHBoxLayout;
    hlayout->addLayout(qlayout);
    hlayout->addLayout(vlayout);

    setLayout(hlayout);

    layout()->setSizeConstraint(QLayout::SetFixedSize);
}

void CCustomDialog::getInfo(int& nWidth, int& nHeight, int& nMines)
{
    nWidth = m_inputWidth->text().toInt();
    nHeight = m_inputHeight->text().toInt();
    nMines = m_inputMines->text().toInt();

    nHeight = qBound(CMineWidget::BoardHeightMin, nHeight, CMineWidget::BoardHeightMax);
    nWidth = qBound(CMineWidget::BoardWidthMin, nWidth, CMineWidget::BoardWidthMax);
    nMines = qBound(CMineWidget::MineCountMin, nMines, static_cast<int>(nMines * nHeight * CMineWidget::MineRatio));
}
