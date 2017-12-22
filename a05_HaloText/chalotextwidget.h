#ifndef CHALOTEXTWIDGET_H
#define CHALOTEXTWIDGET_H

#include <QWidget>

class QPainter;

class CHaloTextWidget : public QWidget
{
    Q_OBJECT
public:
    explicit CHaloTextWidget(QWidget *parent = nullptr);
protected:
    void paintEvent(QPaintEvent *event) override;
};

#endif // CHALOTEXTWIDGET_H
