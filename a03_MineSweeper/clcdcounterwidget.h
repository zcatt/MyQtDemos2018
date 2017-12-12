#ifndef CLCDCOUNTERWIDGET_H
#define CLCDCOUNTERWIDGET_H

#include <QFrame>

class CLcdCounterWidget: public QFrame
{
    Q_OBJECT
public:
    enum
    {
        Lower = 000,
        Upper = 999
    };

public:
    explicit CLcdCounterWidget(QWidget *parent = nullptr);
    int value(void);
    void inc(void);
    void dec(void);

protected:
    void paintEvent(QPaintEvent *) Q_DECL_OVERRIDE;

public slots:
    void setValue(int nValue);

public:
    int m_nValue;
};

#endif // CLCDCOUNTERWIDGET_H
