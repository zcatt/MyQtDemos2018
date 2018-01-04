#include <QtWidgets>

#include "cwindow.h"

CWindow::CWindow(QWidget *parent)
    : QWidget(parent)
{
    QLabel *echoLabel = new QLabel(tr("Echo Mode:"));
    QComboBox *echoComboBox = new QComboBox;
    echoComboBox->addItem(tr("Normal"));
    echoComboBox->addItem(tr("Password"));
    echoComboBox->addItem(tr("PasswordEchoOnEdit"));
    echoComboBox->addItem(tr("No Echo"));

    QLabel *validatorLabel = new QLabel(tr("Validator Type:"));
    QComboBox *validatorComboBox = new QComboBox;
    validatorComboBox->addItem(tr("No validator"));
    validatorComboBox->addItem(tr("Integer validator"));
    validatorComboBox->addItem(tr("Double validator"));

    QLabel *alignmentLabel = new QLabel(tr("Alignment Type:"));
    QComboBox *alignmentComboBox = new QComboBox;
    alignmentComboBox->addItem(tr("Left"));
    alignmentComboBox->addItem(tr("Centered"));
    alignmentComboBox->addItem(tr("Right"));

    QLabel *inputMaskLabel = new QLabel(tr("Input Mask Type:"));
    QComboBox *inputMaskComboBox = new QComboBox;
    inputMaskComboBox->addItem(tr("No mask"));
    inputMaskComboBox->addItem(tr("Phone number"));
    inputMaskComboBox->addItem(tr("ISO date"));
    inputMaskComboBox->addItem(tr("License key"));

    QLabel *accessLabel = new QLabel(tr("Read-only:"));
    QComboBox *accessComboBox = new QComboBox;
    accessComboBox->addItem(tr("False"));
    accessComboBox->addItem(tr("True"));

    connect(echoComboBox, SIGNAL(activated(int)),
            this, SLOT(echoChanged(int)));
    connect(validatorComboBox, SIGNAL(activated(int)),
            this, SLOT(validatorChanged(int)));
    connect(alignmentComboBox, SIGNAL(activated(int)),
            this, SLOT(alignmentChanged(int)));
    connect(inputMaskComboBox, SIGNAL(activated(int)),
            this, SLOT(inputMaskChanged(int)));
    connect(accessComboBox, SIGNAL(activated(int)),
            this, SLOT(accessChanged(int)));

    m_lineEdit = new QLineEdit;
    m_lineEdit->setPlaceholderText("Placeholder Text");
    m_lineEdit->setDragEnabled(true);
    m_lineEdit->setFocus();

    QFont font;
    font.setPointSize(16);
    m_lineEdit2 = new QLineEdit;
    m_lineEdit2->setPlaceholderText("drag or drop");
    m_lineEdit2->setFont(font);

    QVBoxLayout *vboxLayout = new QVBoxLayout;
    vboxLayout->addWidget(echoLabel);
    vboxLayout->addWidget(echoComboBox);
    vboxLayout->addWidget(validatorLabel);
    vboxLayout->addWidget(validatorComboBox);
    vboxLayout->addWidget(alignmentLabel);
    vboxLayout->addWidget(alignmentComboBox);
    vboxLayout->addWidget(inputMaskLabel);
    vboxLayout->addWidget(inputMaskComboBox);
    vboxLayout->addWidget(accessLabel);
    vboxLayout->addWidget(accessComboBox);
    vboxLayout->addWidget(m_lineEdit);

    QHBoxLayout *layout = new QHBoxLayout;
    layout->addItem(vboxLayout);
    layout->addWidget(m_lineEdit2);

    setLayout(layout);
}

CWindow::~CWindow()
{
}


void CWindow::echoChanged(int index)
{
    switch (index) {
    case 0:
        m_lineEdit->setEchoMode(QLineEdit::Normal);
        break;
    case 1:
        m_lineEdit->setEchoMode(QLineEdit::Password);
        break;
    case 2:
        m_lineEdit->setEchoMode(QLineEdit::PasswordEchoOnEdit);
        break;
    case 3:
        m_lineEdit->setEchoMode(QLineEdit::NoEcho);
    }
}

void CWindow::validatorChanged(int index)
{
    switch (index) {
    case 0:
        m_lineEdit->setValidator(0);
        break;
    case 1:
        m_lineEdit->setValidator(new QIntValidator(
            m_lineEdit));
        break;
    case 2:
        m_lineEdit->setValidator(new QDoubleValidator(-999.0,
            999.0, 2, m_lineEdit));
    }

    m_lineEdit->clear();

}

void CWindow::alignmentChanged(int index)
{
    switch (index) {
    case 0:
        m_lineEdit->setAlignment(Qt::AlignLeft);
        break;
    case 1:
        m_lineEdit->setAlignment(Qt::AlignCenter);
        break;
    case 2:
        m_lineEdit->setAlignment(Qt::AlignRight);
    }
}

void CWindow::inputMaskChanged(int index)
{
    switch (index) {
    case 0:
        m_lineEdit->setInputMask("");
        break;
    case 1:
        m_lineEdit->setInputMask("+99 99 99 99 99;_");
        break;
    case 2:
        m_lineEdit->setInputMask("0000-00-00");
        m_lineEdit->setText("00000000");
        m_lineEdit->setCursorPosition(0);
        break;
    case 3:
        m_lineEdit->setInputMask(">AAAAA-AAAAA-AAAAA-AAAAA-AAAAA;#");
    }
}

void CWindow::accessChanged(int index)
{
    switch (index) {
    case 0:
        m_lineEdit->setReadOnly(false);
        break;
    case 1:
        m_lineEdit->setReadOnly(true);
    }

}

