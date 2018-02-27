#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

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
    CDraftView *m_pGView;
    CDraftScene *m_pGScene;

    QToolBox *m_pToolBox;
    QListWidget *m_pShapes;

};

#endif // MAINWINDOW_H
