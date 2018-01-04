#ifndef CWINDOW_H
#define CWINDOW_H

#include <QWidget>

class QLineEdit;

class CWindow : public QWidget
{
    Q_OBJECT

public:
    CWindow(QWidget *parent = 0);
    ~CWindow();

public slots:
    void echoChanged(int);
    void validatorChanged(int);
    void alignmentChanged(int);
    void inputMaskChanged(int);
    void accessChanged(int);

private:
    QLineEdit *m_lineEdit;
    QLineEdit *m_lineEdit2;

};

#endif // CWINDOW_H
