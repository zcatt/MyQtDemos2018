#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>

class QItemEditorFactory;

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = 0);
    ~Widget();

    void createGUI();
public:
    QItemEditorFactory *m_editorFactory;
};

#endif // WIDGET_H
