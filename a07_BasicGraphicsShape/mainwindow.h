#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

class QToolBox;
class QToolButton;
class QListWidget;
class QGraphicsView;
class QGraphicsScene;
class CDraftScene;
class CDraftView;


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
    void zoomIn(void);
    void zoomOut(void);
    void deleteItem(void);
    void bringToFront(void);
    void sendToBack(void);
    void about(void);


public:

    QToolBox *m_pToolBox;
    //QToolButton *m_pShapeToolButton;
    //QToolButton *m_pAnnotationToolButton;
    QListWidget *m_pShapes;
    QListWidget *m_pAnnotations;

    CDraftView *m_pGView;
    CDraftScene *m_pGScene;
};

#endif // MAINWINDOW_H
