#include <QtWidgets>

#include "mainwindow.h"

//QGraphicsProxyWidget + QTextEdit的背景透明实现需要两者各自背景透明
// QGraphicsProxyWidget使用  pl.setBrush(QPalette::Window,QColor(0,0,0,0));
// QTextEdit使用 pl.setBrush(QPalette::Base,QColor(0,0,0,0));

MainWindow::MainWindow(QWidget *parent)
    : QWidget(parent)
{
    QGridLayout *layout = new QGridLayout;

    QGraphicsScene *scene = new QGraphicsScene(this);
    //scene->setBackgroundBrush(Qt::yellow);

    QRadialGradient gradient(0, 0, 800);
    gradient.setSpread(QGradient::RepeatSpread);
    gradient.setColorAt(0, Qt::yellow);
    gradient.setColorAt(1, Qt::red);
    scene->setBackgroundBrush(gradient);

    QPalette pl;
    QTextEdit *edit = new QTextEdit("<h1>hello</h1>, world");
    pl = edit->palette();
    pl.setBrush(QPalette::Base,QColor(255,255,255,0));
    edit->setPalette(pl);
    edit->setFrameStyle(QFrame::Box | QFrame::Plain);
    edit->setWindowTitle("hi");
    edit->setWindowFlag(Qt::WindowTitleHint, true);

    //QGraphicsOpacityEffect *opacityEffect = new QGraphicsOpacityEffect;
    //opacityEffect->setOpacity(1.0);
    //proxy->setGraphicsEffect(opacityEffect);

    QGraphicsProxyWidget *proxy = new QGraphicsProxyWidget;
    pl = proxy->palette();
    //pl.setBrush(QPalette::Window,QColor(0,0,0,0));
    pl.setBrush(QPalette::Window,QColor(255,255,255,150));
    proxy->setPalette(pl);
    proxy->setWidget(edit);

#if 1
    scene->addItem(proxy);
#else
    QGraphicsWidget *gw = new QGraphicsWidget;//(0, Qt::Window);
    gw->setAutoFillBackground(true);
    pl = gw->palette();
    //pl.setBrush(QPalette::Background, Qt::blue);
    pl.setBrush(QPalette::Window, Qt::gray);
    gw->setPalette(pl);
    gw->resize(200,200);
    gw->setContentsMargins(20,40,20,20);
    scene->addItem(gw);

    QGraphicsLinearLayout *glayout = new QGraphicsLinearLayout(gw);
    glayout->addItem(proxy);
#endif
    //proxy->setParentItem(gw);
    //scene->addItem(proxy);

    QGraphicsView *view=new QGraphicsView;
    view->setScene(scene);
    view->resize(600,600);
    view->show();

    layout->addWidget(view, 0, 0);

#if 0
    QTextEdit *edit2 = new QTextEdit("<h1>hello</h1>, world");
    pl = edit2->palette();
    pl.setBrush(QPalette::Base,QColor(0,0,0,0));
    edit2->setPalette(pl);
    layout->addWidget(edit2, 0, 1);
#endif


    setLayout(layout);

    resize(800,600);
#if 0
    QObject *obj = new QObject(this);
    qDebug()<<"dynamic props:";
    for(const QByteArray& dpname: obj->dynamicPropertyNames())
    {
        qDebug()<< QString("%1\t%2").arg(QString(dpname)).arg(property(dpname).toString());
    }

    //proxy->setProperty("abc", QVariant("defgh"));

#if 1
    qDebug()<<"dynamic props:";
    for(const QByteArray& dpname: proxy->dynamicPropertyNames())
    {
        qDebug()<< QString("%1\t%2").arg(QString(dpname)).arg(property(dpname).toString());
    }

    proxy->setProperty("abc", QVariant("defgh"));
    //proxy->setProperty("abc", 42);

    qDebug()<<"dynamic props:";
    for(const QByteArray& dpname: proxy->dynamicPropertyNames())
    {
        QString s = proxy->property(dpname).toString();
        qDebug()<< QString("%1,%2").arg(QString(dpname)).arg(s);
    }
#endif
#endif
}

MainWindow::~MainWindow()
{

}
