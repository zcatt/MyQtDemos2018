#ifndef FINDDIALOG_H
#define FINDDIALOG_H

#include <QDialog>
#include <QLineEdit>

class FindDialog : public QDialog
{
    Q_OBJECT
public:
    explicit FindDialog(QWidget *parent = nullptr);

    void setSearchText(const QString& text);

signals:

public slots:

protected:
    void closeEvent(QCloseEvent *event) override;

public:
    QLineEdit *m_lineEdit;
};

#endif // FINDDIALOG_H
