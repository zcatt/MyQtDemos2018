#include <QtWidgets>
#include <QStyledItemDelegate>

#include "colorlisteditor.h"
#include "widget.h"

#include "setdebugnew.h"


Widget::Widget(QWidget *parent)
    : QWidget(parent), m_editorFactory(0)
{
    //global editorFactory
    QItemEditorFactory *factory = new QItemEditorFactory;

    QItemEditorCreatorBase *colorListCreator =
        new QStandardItemEditorCreator<ColorListEditor>();

    factory->registerEditor(QVariant::Color, colorListCreator);

    QItemEditorFactory::setDefaultFactory(factory);

    //local editorFactory
    m_editorFactory = new QItemEditorFactory;
    colorListCreator =new QStandardItemEditorCreator<ColorListEditor>();
    m_editorFactory->registerEditor(QVariant::Color, colorListCreator);


    createGUI();

}

Widget::~Widget()
{
    //delete m_editorFactory;
}

void Widget::createGUI()
{
    QList<QString> nameList;
    QList<QColor>  hairList;
    QList<QColor>  eyeList;
    nameList<< tr("Tom")<<tr("Smith")<<tr("Mark");
    hairList<< QColor("aliceblue")<< QColor("aquamarine") << QColor("springgreen");
    eyeList<< QColor("black")<< QColor("grey")<<QColor("brown");

    QStyledItemDelegate *delegate = new QStyledItemDelegate(this);
    delegate->setItemEditorFactory(m_editorFactory);


    QTableWidget *table = new QTableWidget(3, 3);
    table->setItemDelegateForColumn(2, delegate);

    table->setHorizontalHeaderLabels(QStringList() << tr("Name")
                                                   << tr("Hair Color")
                                                   << tr("eye color"));
    table->verticalHeader()->setVisible(false);
    table->resize(200, 50);

    for (int i = 0; i < 3; ++i) {

        QTableWidgetItem *nameItem = new QTableWidgetItem(nameList.at(i));

        QTableWidgetItem *hairItem = new QTableWidgetItem;
        hairItem->setData(Qt::DisplayRole, hairList.at(i));

        QTableWidgetItem *eyeItem = new QTableWidgetItem;
        eyeItem->setData(Qt::DisplayRole, eyeList.at(i));

        table->setItem(i, 0, nameItem);
        table->setItem(i, 1, hairItem);
        table->setItem(i, 2, eyeItem);
    }
    table->resizeColumnToContents(0);
    table->horizontalHeader()->setStretchLastSection(true);

    QGridLayout *layout = new QGridLayout;
    layout->addWidget(table, 0, 0);

    setLayout(layout);

    setWindowTitle(tr("a04_EditorFactory"));
}


