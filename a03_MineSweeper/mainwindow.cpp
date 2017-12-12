#include <QtDebug>
#include <QtWidgets>

#include "cminewidget.h"
#include "cclientwidget.h"
#include "ccustomdialog.h"

#include "mainwindow.h"



MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{

    CClientWidget *pCenterWidget = new CClientWidget;
    //pCenterWidget->setMinimumSize(200,300);
    //pCenterWidget->setFixedSize(400,600);

    setCentralWidget(pCenterWidget);

    m_pMineWidget = pCenterWidget->getMineWidget();

    createMenus();

#if 1
    //disable resize by dragging border
    layout()->setSizeConstraint(QLayout::SetFixedSize);
    //disable max button
    setWindowFlag(Qt::WindowMaximizeButtonHint, false);
#endif

    mark(true);
    newGame();
}

MainWindow::~MainWindow()
{

}

void MainWindow::newGame()
{
    m_pMineWidget->newGame();
}

void MainWindow::primary()
{
    m_pMineWidget->setLevel(CMineWidget::Primary);
}

void MainWindow::intermediate()
{
    m_pMineWidget->setLevel(CMineWidget::Intermediate);

}

void MainWindow::advance()
{
    m_pMineWidget->setLevel(CMineWidget::Advance);

}

void MainWindow::custom()
{
    int w,h,m;
    m_pMineWidget->getBoardSize(w,h);
    m = m_pMineWidget->getMineCount();

    {
        CCustomDialog dialog(w,h, m, this);

        if(dialog.exec() != QDialog::Accepted)
            return;

        dialog.getInfo(w,h,m);
        m_pMineWidget->setLevel(CMineWidget::Custom);
        m_pMineWidget->setBoardSize(w, h);
        m_pMineWidget->setMineCount(m);
        newGame();
    }
}

void MainWindow::mark(bool checked)
{
    qDebug()<<"mark="<<checked<<endl;
    m_pMineWidget->setMark(checked);
}

void MainWindow::sound(bool checked)
{
    qDebug()<<"sound="<<checked<<endl;
    m_pMineWidget->setSound(checked);
    //todo
}

void MainWindow::championList()
{
    //todo
}

void MainWindow::about()
{
    QMessageBox::about(this, tr("About MineSweeper"),
            tr("One Qt sample created by zcatt@163.com in Dec 2017."));
}


void MainWindow::createMenus(void)
{
/*
    .菜单结构
        游戏(G)
            开局(N)           New Game
            --------
            初级(P)           Primary
            中级(I)           Intermediate
            高级(E)           advancE
            自定义(C)...       Custom
            ----------
            标记(?)(M)        Mark
            声音（S)           Sound
            ----------
            英雄榜(T)....          champion lis&T
            -----------
            退出(X)               eXit
         帮助（H)
            内容（C)       F1      content
            --------
            关于(A)               about
*/
    QAction *pNewGameAct = new QAction(tr("&New Game"), this);
    connect(pNewGameAct, &QAction::triggered, this, &MainWindow::newGame);


    QAction *pPrimaryAct = new QAction(tr("&Primary"), this);
    pPrimaryAct->setCheckable(true);
    connect(pPrimaryAct, &QAction::triggered, this, &MainWindow::primary);

    QAction *pIntermediateAct = new QAction(tr("&Intermediate"), this);
    pIntermediateAct->setCheckable(true);
    connect(pIntermediateAct, &QAction::triggered, this, &MainWindow::intermediate);

    QAction *pAdvanceAct = new QAction(tr("Advanc&e"), this);
    pAdvanceAct->setCheckable(true);
    connect(pAdvanceAct, &QAction::triggered, this, &MainWindow::advance);

    QAction *pCustomAct = new QAction(tr("&Custom..."), this);
    pCustomAct->setCheckable(true);
    connect(pCustomAct, &QAction::triggered, this, &MainWindow::custom);

    QActionGroup *pLevelGroup = new QActionGroup(this);
    pLevelGroup->addAction(pPrimaryAct);
    pLevelGroup->addAction(pIntermediateAct);
    pLevelGroup->addAction(pAdvanceAct);
    pLevelGroup->addAction(pCustomAct);
    pPrimaryAct->setChecked(true);

    QAction *pMarkAct = new QAction(tr("&Mark"), this);
    pMarkAct->setCheckable(true);
    pMarkAct->setChecked(false);
    connect(pMarkAct, &QAction::toggled, this, &MainWindow::mark);

    QAction *pSoundAct = new QAction(tr("&Sound"), this);
    pSoundAct->setCheckable(true);
    pSoundAct->setChecked(false);
    connect(pSoundAct, &QAction::toggled, this, &MainWindow::sound);

    QAction *pChampionListAct = new QAction(tr("Champion lis&t..."), this);
    connect(pChampionListAct, &QAction::triggered, this, &MainWindow::championList);

    QAction *pExitAct = new QAction(tr("E&xit"), this);
    connect(pExitAct, &QAction::triggered, this, &QWidget::close);

    QAction *pAboutAct = new QAction(tr("&About"), this);
    pAboutAct->setStatusTip(tr("About MineSweeper"));
    connect(pAboutAct, &QAction::triggered, this, &MainWindow::about);

    QMenu *pGame = menuBar()->addMenu(tr("&Game"));
    QMenu *pHelp = menuBar()->addMenu(tr("&Help"));

    pGame->addAction(pNewGameAct);
    pGame->addSeparator();

    pGame->addAction(pPrimaryAct);
    pGame->addAction(pIntermediateAct);
    pGame->addAction(pAdvanceAct);
    pGame->addAction(pCustomAct);
    pGame->addSeparator();

    pGame->addAction(pMarkAct);
    pGame->addAction(pSoundAct);
    pGame->addSeparator();

    pGame->addAction(pExitAct);


    pHelp->addAction(pAboutAct);
}
