/*
    使用QScreen::grabWindow()实现抓屏。
    一个难题是如何在抓屏之前隐藏自己不被抓到。
 */
#include <QtWidgets>
//#include <QScreen>

#include "capture.h"
#include <QApplication>


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QMessageBox box(QMessageBox::Question, QObject::tr("capture"), QObject::tr("Capture screen."), QMessageBox::NoButton);
    //box.setWindowModality(Qt::WindowModal);
    //box.setInformativeText(QObject::tr("Try to capture the primary screen."));

    QPushButton *captureBtn = box.addButton(QObject::tr("Capture"), QMessageBox::AcceptRole);

    box.exec();

    Capture widget;

    if(box.clickedButton() == captureBtn)
    {
        //set the delay time not to capture
        QTimer::singleShot(0, &widget, &Capture::DoCapture);
        //QMetaObject::invokeMethod(&widget, "DoCapture", Qt::QueuedConnection);
    }


    return a.exec();
}
