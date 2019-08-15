#include <QtWidgets>
#include "mainwindow.h"
#include "fontwidget.h"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    m_fontWidget = new CFontWidget;
    setCentralWidget(m_fontWidget);

    QMenu* doMenu = menuBar()->addMenu("Do");

    doMenu->addAction("load font file...");

    connect(doMenu, &QMenu::triggered, this, &slotMenuAction);

    resize(600, 400);

}

MainWindow::~MainWindow()
{

}

void MainWindow::slotMenuAction(QAction *action)
{
    if(action->text() == "load font file...")
    {
#if 1
        QString fontFileName = QFileDialog::getOpenFileName(this
                                                            , "font file"
                                                            , "."
                                                            , "TrueType(*.ttf);;OpenType(*.otf)");
#else
        //QString fontFileName("../b03_UseFreetype/SourceHanSerifSC-Medium.otf");
        QString fontFileName("../b03_UseFreetype/HanSerifSC.otf");
//        QString fontFileName("../b03_UseFreetype/cour.ttf");
#endif
        if(!fontFileName.isEmpty())
        {
            m_fontWidget->slotLoadFontFile(fontFileName);
        }
    }
}
