#include <QtWidgets>

#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    createMenuAndToolBar();

#if 1
    QDockWidget *dock1;

    dock1 = new QDockWidget;
    dock1->setObjectName(QLatin1String("dock1"));
    dock1->setWindowTitle(tr("Dock #1"));
    addDockWidget(Qt::LeftDockWidgetArea,dock1);

    QDockWidget *dock2;
    dock2 = new QDockWidget;
    dock2->setObjectName(QLatin1String("dock2"));
    dock2->setWindowTitle(tr("Dock #2"));
    addDockWidget(Qt::RightDockWidgetArea,dock2);

    QDockWidget *dock3;
    dock3 = new QDockWidget;
    dock3->setObjectName(QLatin1String("dock3"));
    dock3->setWindowTitle(tr("Dock #3"));
    addDockWidget(Qt::BottomDockWidgetArea,dock3);
#endif

    QWidget *widget;
    //widget = new QWidget;
    widget = new QCalendarWidget;
    setCentralWidget(widget);

    setObjectName(QLatin1String("mainwindow"));
    setWindowTitle(tr("a0a_DockWidget Demo"));

}

MainWindow::~MainWindow()
{

}

void MainWindow::createMenuAndToolBar()
{
    QAction *loadSettings = new QAction(tr("Load settings"), this);
    connect(loadSettings, SIGNAL(triggered()), this, SLOT(loadSettings()));

    QAction *saveSettings = new QAction(tr("save settings"), this);
    connect(saveSettings, SIGNAL(triggered()), this, SLOT(saveSettings()));

    QAction *clearSettings = new QAction(tr("Clear settings"), this);
    connect(clearSettings, SIGNAL(triggered()), this, SLOT(clearSettings()));

    QAction *toFrontAction = new QAction(QIcon(":/images/bringtofront.png"),
                                tr("Bring to &Front"), this);
    toFrontAction->setShortcut(tr("Ctrl+F"));
    toFrontAction->setStatusTip(tr("Bring item to front"));

    QAction *sendBackAction = new QAction(QIcon(":/images/sendtoback.png"), tr("Send to &Back"), this);
    sendBackAction->setShortcut(tr("Ctrl+T"));
    sendBackAction->setStatusTip(tr("Send item to back"));

    QAction *deleteAction = new QAction(QIcon(":/images/delete.png"), tr("&Delete"), this);
    deleteAction->setShortcut(tr("Delete"));
    deleteAction->setStatusTip(tr("Delete item from diagram"));

    QAction *zoomInAction = new QAction(QIcon(":/images/zoomin.png"), tr("Zoom on"), this);
    zoomInAction->setShortcut(QString("+"));
    zoomInAction->setStatusTip(tr("Zoom in."));

    QAction *zoomOutAction = new QAction(QIcon(":/images/zoomout.png"), tr("Zoom out"), this);
    zoomOutAction->setShortcut(QString("-"));
    zoomOutAction->setStatusTip(tr("Zoom out."));

    QAction *aboutAction = new QAction(tr("A&bout"), this);
    aboutAction->setShortcut(tr("F1"));

    QMenu* itemMenu;

    itemMenu = menuBar()->addMenu(tr("&Settings"));
    itemMenu->addAction(loadSettings);
    itemMenu->addAction(saveSettings);
    itemMenu->addAction(clearSettings);

    itemMenu = menuBar()->addMenu(tr("&Item"));
    itemMenu->addAction(deleteAction);
    itemMenu->addSeparator();
    itemMenu->addAction(toFrontAction);
    itemMenu->addAction(sendBackAction);

    QMenu* viewMenu = menuBar()->addMenu(tr("&View"));
    viewMenu->addAction(zoomInAction);
    viewMenu->addAction(zoomOutAction);

    QMenu* aboutMenu = menuBar()->addMenu(tr("&Help"));
    aboutMenu->addAction(aboutAction);

    QToolBar *editToolBar = addToolBar(tr("Edit"));
    editToolBar->setObjectName("toolbar");
    editToolBar->addAction(deleteAction);
    editToolBar->addAction(toFrontAction);
    editToolBar->addAction(sendBackAction);
    editToolBar->addAction(zoomInAction);
    editToolBar->addAction(zoomOutAction);

}

void MainWindow::loadSettings(void)
{
    QSettings settings(QSettings::IniFormat, QSettings::UserScope, QLatin1String("Org"), QLatin1String("a0a_DockWidget"));
    restoreState(settings.value("MainWindowState").toByteArray());
    restoreGeometry(settings.value("MainWindowGeometry").toByteArray());
}

void MainWindow::saveSettings(void)
{
    QSettings settings(QSettings::IniFormat, QSettings::UserScope, QLatin1String("Org"), QLatin1String("a0a_DockWidget"));
    //QSettings settings(QSettings::IniFormat, QSettings::UserScope, QCoreApplication::organizationName(), QCoreApplication::applicationName());

    settings.setValue("MainWindowState", saveState());
    settings.setValue("MainWindowGeometry", saveGeometry());

}

void MainWindow::clearSettings(void)
{
    QSettings settings(QSettings::IniFormat, QSettings::UserScope, QLatin1String("Org"), QLatin1String("a0a_DockWidget"));
    settings.clear();
}
