#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include "..\AddPlugin\addinterface.h"

class QPushButton;
class QLabel;
class QLineEdit;

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget();
    ~Widget();

    void createGUI(void);
    bool loadPlugin(void);

public slots:
    void add(void);

public:
    CAddInterface *addInterface;

    QLineEdit *m_lineEditA;
    QLineEdit *m_lineEditB;
    QLabel *m_resultLabel;
    QPushButton *m_calcBtn;
};

#endif // WIDGET_H
