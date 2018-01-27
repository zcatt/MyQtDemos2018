#ifndef CAPTURE_H
#define CAPTURE_H

#include <QScrollArea>

class Capture : public QScrollArea
{
    Q_OBJECT

public:
    Capture(QWidget *parent = 0);
    ~Capture();

public slots:
    void DoCapture(void);
};

#endif // CAPTURE_H
