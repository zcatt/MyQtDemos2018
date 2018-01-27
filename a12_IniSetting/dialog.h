#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>

class QAbstractButton;
class QLineEdit;
class QLabel;
class QTextEdit;

class Dialog : public QDialog
{
    Q_OBJECT

public:
    Dialog(QWidget *parent = 0);
    ~Dialog();

public slots:
    void Do(QAbstractButton *btn);

public:
    QLineEdit *nameEdit;
    QLineEdit *addrEdit;
    QLabel *fnLabel;
    QTextEdit *resultEdit;

};

#endif // DIALOG_H
