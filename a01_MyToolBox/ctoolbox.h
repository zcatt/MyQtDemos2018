#ifndef CTOOLBOX_H
#define CTOOLBOX_H

#include <QtWidgets/qframe.h>
#include <QtWidgets/qabstractbutton.h>
#include <QtGui/qicon.h>
#include <qlayout.h>
#include <qlist.h>
#include <QStyleOptionToolBox>

class QScrollArea;
class QSplitter;

class CToolBoxButton: public QAbstractButton
{
    Q_OBJECT
public:
    CToolBoxButton(QWidget *parent): QAbstractButton(parent), m_bSelected(false), m_nIndexInPage(-1)
    {
        setBackgroundRole(QPalette::Window);
        setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Minimum);
        setFocusPolicy(Qt::NoFocus);
    }

    inline void setSelected(bool b) {m_bSelected = b; update();}
    inline void setIndex(int nNewIndex) {m_nIndexInPage = nNewIndex; }

    QSize sizeHint() const override;
    QSize minimumSizeHint() const override;

protected:
    void initStyleOption(QStyleOptionToolBox *opt) const;
    void paintEvent(QPaintEvent *e);

private:
    bool m_bSelected;
    int m_nIndexInPage;

};

class CToolBox : public QFrame
{
    Q_OBJECT
    Q_PROPERTY(int currentIndex READ currentIndex WRITE setCurrentIndex NOTIFY currentChanged)
    Q_PROPERTY(int count READ count)

public:
    explicit CToolBox(QWidget *parent = nullptr, Qt::WindowFlags f = 0);
    ~CToolBox();

    int addItem(QWidget *widget, const QString &text);
    int addItem(QWidget *widget, const QIcon &icon, const QString &text);
    int insertItem(int index, QWidget *widget, const QString &text);
    int insertItem(int index, QWidget *widget, const QIcon &icon, const QString &text);

    void removeItem(int index);

    void setItemEnabled(int index, bool enabled);
    bool isItemEnabled(int index) const;

    void setItemText(int index, const QString &text);
    QString itemText(int index) const;

    void setItemIcon(int index, const QIcon &icon);
    QIcon itemIcon(int index) const;

    void setItemToolTip(int index, const QString &toolTip);
    QString itemToolTip(int index) const;

    int currentIndex() const;
    QWidget *currentWidget() const;
    QWidget *widget(int index) const;
    int indexOf(QWidget *widget) const;
    int count() const;


signals:
    void currentChanged(int index);

public slots:
    void setCurrentIndex(int index);
    void setCurrentWidget(QWidget *widget);

private slots:
    void buttonClicked();
    void widgetDestroyed(QObject*);

protected:
    bool event(QEvent *e);
    virtual void itemInserted(int index);
    virtual void itemRemoved(int index);
    void showEvent(QShowEvent *e);
    void changeEvent(QEvent *);

private:
    Q_DISABLE_COPY(CToolBox)

public:
    struct Page
    {
        CToolBoxButton *m_pButton;
        QScrollArea *m_pScrollArea;
        QWidget *m_pWidget;

        inline void setText(const QString &text) { m_pButton->setText(text); }
        inline void setIcon(const QIcon &is) { m_pButton->setIcon(is); }
#ifndef QT_NO_TOOLTIP
        inline void setToolTip(const QString &tip) { m_pButton->setToolTip(tip); }
        inline QString toolTip() const { return m_pButton->toolTip(); }
#endif
        inline QString text() const { return m_pButton->text(); }
        inline QIcon icon() const { return m_pButton->icon(); }

        inline bool operator==(const Page& other) const
        {
            return m_pWidget == other.m_pWidget;
        }
    };

    typedef QList<Page> PageList;

    Page *page(QWidget *widget) const;
    const Page *page(int index) const;
    Page *page(int index);

    void updateTabs();
    void relayout();

private:
    PageList m_pageList;
    Page *m_pCurrentPage;

    QVBoxLayout *m_pLayout;

    //QSplitter *m_pSplitter;


};


inline int CToolBox::addItem(QWidget *item, const QString &text)
{ return insertItem(-1, item, QIcon(), text); }

inline int CToolBox::addItem(QWidget *item, const QIcon &iconSet,
                              const QString &text)
{ return insertItem(-1, item, iconSet, text); }

inline int CToolBox::insertItem(int index, QWidget *item, const QString &text)
{ return insertItem(index, item, QIcon(), text); }


#endif // CTOOLBOX_H
