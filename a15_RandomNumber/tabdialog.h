#ifndef TABDIALOG_H
#define TABDIALOG_H

#include <QDialog>

class QTabWidget;
class QDialogButtonBox;
class QLabel;
class QSpinBox;
class QTextEdit;


class CTabDialog : public QDialog
{
    Q_OBJECT

public:
    CTabDialog(QWidget *parent = 0);
    ~CTabDialog();

private:
    QTabWidget *m_tabWidget;
    QDialogButtonBox *m_buttonBox;
};


class CRandomTab : public QWidget
{
    Q_OBJECT
public:
    CRandomTab(QWidget *parent = 0);
    void genRandom(void);
public:
    QTextEdit *result;
    QSpinBox *minSpinBox;
    QSpinBox *maxSpinBox;
    QSpinBox *countSpinBox;

};

#endif // TABDIALOG_H
