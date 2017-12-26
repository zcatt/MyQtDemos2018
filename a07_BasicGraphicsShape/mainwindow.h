#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

class QToolBox;
class QToolButton;
class QListWidget;
class QGraphicsView;
class QGraphicsScene;
class CDraftScene;


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

public:

    QToolBox *m_pToolBox;
    //QToolButton *m_pShapeToolButton;
    //QToolButton *m_pAnnotationToolButton;
    QListWidget *m_pShapes;
    QListWidget *m_pAnnotations;

    QGraphicsView *m_pGView;
    CDraftScene *m_pGScene;
};

#endif // MAINWINDOW_H
