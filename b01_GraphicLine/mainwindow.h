#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

class QLabel;
class QGraphicsItem;

class CDraftView;
class CDraftScene;
class QToolBox;
class QListWidget;
class QListWidgetItem;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void CreateToolBox(void);
    void AddShapeItems(void);
    int GetCurrentShapeType(void);
    void ResetShapeType(void);

    void createMenuAndToolBar(void);
    void createStatusBar(void);
    void showMousePos(QPoint ptView);

public slots:
    void currentShapeChanged(QListWidgetItem *current, QListWidgetItem *previous);
    void itemInserted(QGraphicsItem *item);

    void zoomIn(void);
    void zoomOut(void);
    void deleteItem(void);
    void bringToFront(void);
    void sendToBack(void);
    void about(void);

public:
    QLabel* m_statusBarPos;

    CDraftView *m_pGView;
    CDraftScene *m_pGScene;

    QToolBox *m_pToolBox;
    QListWidget *m_pShapes;

};

extern MainWindow *mainWin;

#endif // MAINWINDOW_H
