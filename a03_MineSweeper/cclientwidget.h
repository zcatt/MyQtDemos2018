#ifndef CCLIENTWIDGET_H
#define CCLIENTWIDGET_H

#include <QWidget>

class QPushButton;
class CMineWidget;

class CClientWidget : public QWidget
{
    Q_OBJECT
public:
    explicit CClientWidget(QWidget *parent = nullptr);
    inline CMineWidget* getMineWidget(void) {return m_pMineWidget;}

    bool eventFilter(QObject *object, QEvent *event) override;

signals:

public slots:
    void stateChanged(int nState);

protected:
    void paintEvent(QPaintEvent *) Q_DECL_OVERRIDE;

public:
    QPushButton *m_pBtn;
    CMineWidget *m_pMineWidget;
};

#endif // CCLIENTWIDGET_H
