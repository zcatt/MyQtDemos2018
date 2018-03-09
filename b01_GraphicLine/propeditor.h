#ifndef PROPEDITOR_H
#define PROPEDITOR_H

#include <QWidget>

class QtProperty;
class QtTreePropertyBrowser;
class QtDoublePropertyManager;
class QtStringPropertyManager;
class QtColorPropertyManager;
class QtFontPropertyManager;
class QtPointFPropertyManager;
class QtSizeFPropertyManager;
class QtRectFPropertyManager;

class C2DItem;

class CPropEditor : public QWidget
{
    Q_OBJECT
public:
    explicit CPropEditor(QWidget *parent = nullptr);

    void setItem(C2DItem *item);

public slots:
    void itemSelected(C2DItem *item, bool bSelected);
    void posChange(C2DItem *item, QPointF pos);

private slots:
    void valueChanged(QtProperty *property, double value);
    void valueChanged(QtProperty *property, const QString &value);
    void valueChanged(QtProperty *property, const QColor &value);
    void valueChanged(QtProperty *property, const QFont &value);
    void valueChanged(QtProperty *property, const QPointF &value);
    void valueChanged(QtProperty *property, const QSizeF &value);
    void valueChanged(QtProperty *property, const QRectF &value);

private:
    void addProperty(QtProperty *property, const QString &id);


public:
    C2DItem *m_pItem;

    QtTreePropertyBrowser *m_pTreeBrowser;
    QtDoublePropertyManager *m_pDoubleManager;
    QtStringPropertyManager *m_pStringManager;
    QtColorPropertyManager *m_pColorManager;
    QtFontPropertyManager *m_pFontManager;
    QtPointFPropertyManager *m_pPointfManager;
    QtSizeFPropertyManager *m_pSizefManager;
    QtRectFPropertyManager *m_pRectfManager;

    QMap<QtProperty *, QString> m_propertyToId;
    QMap<QString, QtProperty *> m_idToProperty;
};

#endif // PROPEDITOR_H
