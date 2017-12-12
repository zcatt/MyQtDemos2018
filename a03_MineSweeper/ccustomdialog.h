#ifndef CCUSTOMDIALOG_H
#define CCUSTOMDIALOG_H

#include <QDialog>

class QLineEdit;

class CCustomDialog : public QDialog
{
    Q_OBJECT
public:
    explicit CCustomDialog(int nWidth, int nHeight, int nMines, QWidget* parent = 0);
    void getInfo(int& nWidth, int& nHeight, int& nMines);

public:
    QLineEdit *m_inputHeight;
    QLineEdit *m_inputWidth;
    QLineEdit *m_inputMines;
};

#endif // CCUSTOMDIALOG_H
