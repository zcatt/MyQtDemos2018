#include <QtWidgets>

#include "capture.h"

Capture::Capture(QWidget *parent)
    : QScrollArea(parent)
{
}

Capture::~Capture()
{

}

void Capture::DoCapture(void)
{
    QDesktopWidget *desktop = QApplication::desktop();
    QScreen *screen = QApplication::primaryScreen();

    if(screen)
    {
        QPixmap pixmap;
        pixmap = screen->grabWindow(desktop->winId(), 0, 0, -1, -1);
        pixmap = screen->grabWindow(desktop->winId(), 0, 0, -1, -1);
        QLabel *label = new QLabel;
        label->setPixmap(pixmap);

        setGeometry(100,100, 400, 300);
        setWidget(label);

        show();
    }

}
