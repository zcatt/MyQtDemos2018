#include "preheader.h"
#include "shapeitem.h"
#include "lineitem.h"

#include "propeditor.h"

CPropEditor::CPropEditor(QWidget *parent) : QWidget(parent)
                    , m_propertyToId(), m_idToProperty()
{
    m_pGroupManager = new QtGroupPropertyManager(this);
    m_pEnumManager = new QtEnumPropertyManager(this);
    m_pDoubleManager = new QtDoublePropertyManager(this);
    m_pStringManager = new QtStringPropertyManager(this);
    m_pColorManager = new QtColorPropertyManager(this);
    m_pFontManager = new QtFontPropertyManager(this);
    m_pPointfManager = new QtPointFPropertyManager(this);
    m_pSizefManager = new QtSizeFPropertyManager(this);
    m_pRectfManager = new QtRectFPropertyManager(this);


    connect(m_pEnumManager, &QtEnumPropertyManager::valueChanged,
                this,  static_cast<void (CPropEditor::*)(QtProperty *, int)>(&CPropEditor::valueChanged));
    connect(m_pDoubleManager, &QtDoublePropertyManager::valueChanged,
                this,  static_cast<void (CPropEditor::*)(QtProperty *, double)>(&CPropEditor::valueChanged));
    connect(m_pStringManager, &QtStringPropertyManager::valueChanged,
                this, static_cast<void (CPropEditor::*)(QtProperty *, const QString&)>(&CPropEditor::valueChanged));
    connect(m_pColorManager, &QtColorPropertyManager::valueChanged,
                this, static_cast<void (CPropEditor::*)(QtProperty *, const QColor&)>(&CPropEditor::valueChanged));
    connect(m_pFontManager, &QtFontPropertyManager::valueChanged,
                this, static_cast<void (CPropEditor::*)(QtProperty *, const QFont&)>(&CPropEditor::valueChanged));
    connect(m_pPointfManager, &QtPointFPropertyManager::valueChanged,
                this, static_cast<void (CPropEditor::*)(QtProperty *, const QPointF& )>(&CPropEditor::valueChanged));
    connect(m_pSizefManager, &QtSizeFPropertyManager::valueChanged,
                this, static_cast<void (CPropEditor::*)(QtProperty *, const QSizeF&)>(&CPropEditor::valueChanged));
    connect(m_pRectfManager, &QtRectFPropertyManager::valueChanged,
                this, static_cast<void (CPropEditor::*)(QtProperty *, const QRectF&)>(&CPropEditor::valueChanged));

    QtDoubleSpinBoxFactory *doubleSpinBoxFactory = new QtDoubleSpinBoxFactory(this);
    QtCheckBoxFactory *checkBoxFactory = new QtCheckBoxFactory(this);
    QtSpinBoxFactory *spinBoxFactory = new QtSpinBoxFactory(this);
    QtLineEditFactory *lineEditFactory = new QtLineEditFactory(this);
    QtEnumEditorFactory *comboBoxFactory = new QtEnumEditorFactory(this);

    m_pTreeBrowser = new QtTreePropertyBrowser;
    m_pTreeBrowser->setRootIsDecorated(true);
    m_pTreeBrowser->setResizeMode(QtTreePropertyBrowser::Interactive);
    m_pTreeBrowser->setAlternatingRowColors(true);

    m_pTreeBrowser->setFactoryForManager(m_pEnumManager, comboBoxFactory);
    m_pTreeBrowser->setFactoryForManager(m_pDoubleManager, doubleSpinBoxFactory);
    m_pTreeBrowser->setFactoryForManager(m_pStringManager, lineEditFactory);
    m_pTreeBrowser->setFactoryForManager(m_pColorManager->subIntPropertyManager(), spinBoxFactory);
    m_pTreeBrowser->setFactoryForManager(m_pFontManager->subIntPropertyManager(), spinBoxFactory);
    m_pTreeBrowser->setFactoryForManager(m_pFontManager->subBoolPropertyManager(), checkBoxFactory);
    m_pTreeBrowser->setFactoryForManager(m_pFontManager->subEnumPropertyManager(), comboBoxFactory);
    m_pTreeBrowser->setFactoryForManager(m_pPointfManager->subDoublePropertyManager(), doubleSpinBoxFactory);
    m_pTreeBrowser->setFactoryForManager(m_pSizefManager->subDoublePropertyManager(), doubleSpinBoxFactory);
    m_pTreeBrowser->setFactoryForManager(m_pRectfManager->subDoublePropertyManager(), doubleSpinBoxFactory);

    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->addWidget(m_pTreeBrowser);
    layout->setMargin(0);
    layout->setSpacing(0);

    m_pItem = 0;
    setItem(0);

}

//property加入到QtTreePropertyBrowser中，并建立prop <--> id的索引map
//
void CPropEditor::addProperty(QtProperty *property, const QString &id)
{
    Q_ASSERT(m_pItem);

    m_propertyToId[property] = id;
    m_idToProperty[id] = property;
    m_pTreeBrowser->addProperty(property);
}

void CPropEditor::setItem(C2DItem *item)
{
    qDebug()<<"item=" << (const void*)item;

    //clear previous item.
    QMap<QtProperty *, QString>::ConstIterator itProp = m_propertyToId.constBegin();
    while (itProp != m_propertyToId.constEnd()) {
        delete itProp.key();
        itProp++;
    }
    m_propertyToId.clear();
    m_idToProperty.clear();

    m_pItem = item;

    if(!item)
    {
        return;
    }

    //set current
    //TODO, 此部分应当分散到各C2DItem类中实现，或者集中到专门类中
    QtProperty *prop;

    if(item->type() == C2DItem::Type_Line)
    {
        CLineItem *lineItem = qobject_cast<CLineItem*>(item);
        Q_ASSERT(lineItem != 0);

        QtProperty *shapeProp;
        QtProperty *nameProp;
        QtProperty *descProp;

        QStringList enumNames;
        QMap<int, QIcon> enumIcons;

        prop = m_pGroupManager->addProperty(tr("End1"));

        shapeProp = m_pEnumManager->addProperty(tr("Shape"));
        enumNames << "None"
                  << "Arrow"
                  << "Hollow Circle" << "SolidCircle"
                  << "Hollow Triangle" <<"Solid Triangle"
                  << "Hollow Diamond" << "Solid Diamond"
                  << "Custom";
        m_pEnumManager->setEnumNames(shapeProp, enumNames);
        enumIcons[0] = QIcon(":/images/emotion1.png");
        enumIcons[1] = QIcon(":/images/emotion2.png");
        enumIcons[2] = QIcon(":/images/emotion3.png");
        enumIcons[3] = QIcon(":/images/emotion4.png");
        enumIcons[5] = QIcon(":/images/emotion1.png");
        enumIcons[6] = QIcon(":/images/emotion2.png");
        enumIcons[7] = QIcon(":/images/emotion3.png");
        enumIcons[8] = QIcon(":/images/emotion4.png");
        enumIcons[9] = QIcon(":/images/emotion1.png");
        m_pEnumManager->setEnumIcons(shapeProp, enumIcons);
        m_pEnumManager->setValue(shapeProp, lineItem->endPoint(0).endShape());
        prop->addSubProperty(shapeProp);

        m_propertyToId[shapeProp] = QLatin1String("endShape1");
        m_idToProperty[QLatin1String("endShap1")] = shapeProp;
        addProperty(prop, QLatin1String("end1"));
        //m_pTreeBrowser->addProperty(prop);

        prop = m_pGroupManager->addProperty(tr("End2"));
        shapeProp = m_pEnumManager->addProperty(tr("Shape"));
        enumNames.clear();
        enumNames << "None"
                  << "Arrow"
                  << "Hollow Circle" << "SolidCircle"
                  << "Hollow Triangle" <<"Solid Triangle"
                  << "Hollow Diamond" << "Solid Diamond"
                  << "Custom";
        m_pEnumManager->setEnumNames(shapeProp, enumNames);
        enumIcons.clear();
        enumIcons[0] = QIcon(":/images/emotion1.png");
        enumIcons[1] = QIcon(":/images/emotion2.png");
        enumIcons[2] = QIcon(":/images/emotion3.png");
        enumIcons[3] = QIcon(":/images/emotion4.png");
        enumIcons[5] = QIcon(":/images/emotion5.png");
        enumIcons[6] = QIcon(":/images/emotion6.png");
        enumIcons[7] = QIcon(":/images/emotion7.png");
        enumIcons[8] = QIcon(":/images/emotion8.png");
        enumIcons[9] = QIcon(":/images/emotion9.png");
        m_pEnumManager->setEnumIcons(shapeProp, enumIcons);
        m_pEnumManager->setValue(shapeProp, lineItem->endPoint(0).endShape());
        prop->addSubProperty(shapeProp);

        m_propertyToId[shapeProp] = QLatin1String("endShape2");
        m_idToProperty[QLatin1String("endShap2")] = shapeProp;
        addProperty(prop, QLatin1String("end2"));
        //m_pTreeBrowser->addProperty(prop);
    }
    else if (item->type() == C2DItem::Type_Text)
    {
        //TODO
    }
    else if (item->type() == C2DItem::Type_Rect)
    {
        CRectItem *rcItem = qobject_cast<CRectItem*>(item);
        Q_ASSERT(rcItem != 0);

        //rect
        QRectF rect = rcItem->rect();
        rect.translate(rcItem->pos());

        prop = m_pRectfManager->addProperty(tr("Rect"));
        m_pRectfManager->setValue(prop, rect);
        addProperty(prop, QLatin1String("rect"));
    }


    //pen color
    prop = m_pColorManager->addProperty(tr("Pen Color"));
    m_pColorManager->setValue(prop, item->pen().color());
    addProperty(prop, QLatin1String("penColor"));

    //brush color
    prop = m_pColorManager->addProperty(tr("Brush Color"));
    m_pColorManager->setValue(prop, item->brush().color());
    addProperty(prop, QLatin1String("brushColor"));
}


void CPropEditor::itemSelected(C2DItem *item, bool bSelected)
{
    qDebug()<<"item="<<(const void*)item;

    setItem(bSelected ? item : 0);
}

void CPropEditor::posChange(C2DItem *item, QPointF pos)
{
    qDebug()<<"item="<<(const void*)item;
    if(item != m_pItem)
        return;

    QtProperty *prop;

    if(item->type() == C2DItem::Type_Line)
    {
        //TODO
    }
    else if (item->type() == C2DItem::Type_Text)
    {
        //TODO
    }
    else if (item->type() == C2DItem::Type_Rect)
    {
        CRectItem *rcItem = qobject_cast<CRectItem*>(item);
        Q_ASSERT(rcItem != 0);


        QRectF rect = rcItem->rect();
        rect.translate(rcItem->pos());
        m_pRectfManager->setValue(m_idToProperty[QLatin1String("rect")], rect);
    }
}


void CPropEditor::valueChanged(QtProperty *property, int value)
{
    if(!m_propertyToId.contains(property))
        return;
    if(!m_pItem)
        return;

    QString id = m_propertyToId[property];

    //TODO, 此部分应当分散到各C2DItem类中实现，或者集中到专门类中
    if(m_pItem->type() == C2DItem::Type_Line)
    {
        CLineItem *lineItem = qobject_cast<CLineItem*>(m_pItem);

        if(id == QLatin1String("endShape1"))
        {
            CEndpoint& endpoint = lineItem->endPoint(0);
            endpoint.setEndShape(static_cast<CEndpoint::EndShape>(value));
        }
        else if(id == QLatin1String("endShape2"))
        {
            CEndpoint& endpoint = lineItem->endPoint(1);
            endpoint.setEndShape(static_cast<CEndpoint::EndShape>(value));
        }
    }
    else if (m_pItem->type() == C2DItem::Type_Text)
    {
        //TODO
    }
    else if (m_pItem->type() == C2DItem::Type_Rect)
    {
        //TODO
    }

}

void CPropEditor::valueChanged(QtProperty *property, double value)
{
    Q_ASSERT(m_pItem);

    //TODO, 此部分应当分散到各C2DItem类中实现，或者集中到专门类中
    if(m_pItem->type() == C2DItem::Type_Line)
    {

    }
    else if (m_pItem->type() == C2DItem::Type_Text)
    {
        //TODO
    }
    else if (m_pItem->type() == C2DItem::Type_Rect)
    {
        //TODO
    }

}

void CPropEditor::valueChanged(QtProperty *property, const QString &value)
{

    Q_ASSERT(m_pItem);

    //TODO, 此部分应当分散到各C2DItem类中实现，或者集中到专门类中
    if(m_pItem->type() == C2DItem::Type_Line)
    {
        //TODO

    }
    else if (m_pItem->type() == C2DItem::Type_Text)
    {
        //TODO
    }
    else if (m_pItem->type() == C2DItem::Type_Rect)
    {
        //TODO
    }

}

//TODO, 此部分应当分散到各C2DItem类中实现，或者集中到专门类中
void CPropEditor::valueChanged(QtProperty *property, const QColor &value)
{
    if(!m_propertyToId.contains(property))
        return;
    if(!m_pItem)
        return;

    QString id = m_propertyToId[property];
#if 0

    if(m_pItem->type() == C2DItem::Type_Line)
    {

    }
    else if (m_pItem->type() == C2DItem::Type_Text)
    {
        //TODO
    }
    else if (m_pItem->type() == C2DItem::Type_Rect)
    {
    }
#endif

    if(id == QLatin1String("penColor"))
    {
        QPen pen = m_pItem->pen();
        pen.setColor(value);
        m_pItem->setPen(pen);
    }
    else if(id == QLatin1String("brushColor"))
    {
        QBrush brush = m_pItem->brush();
        brush.setColor(value);
        m_pItem->setBrush(brush);
    }
}

void CPropEditor::valueChanged(QtProperty *property, const QFont &value)
{
    Q_ASSERT(m_pItem);

    //TODO, 此部分应当分散到各C2DItem类中实现，或者集中到专门类中
    if(m_pItem->type() == C2DItem::Type_Line)
    {
        //TODO

    }
    else if (m_pItem->type() == C2DItem::Type_Text)
    {
        //TODO
    }
    else if (m_pItem->type() == C2DItem::Type_Rect)
    {
        //TODO
    }

}

void CPropEditor::valueChanged(QtProperty *property, const QPointF &value)
{
    Q_ASSERT(m_pItem);

    //TODO, 此部分应当分散到各C2DItem类中实现，或者集中到专门类中
    if(m_pItem->type() == C2DItem::Type_Line)
    {
        //TODO

    }
    else if (m_pItem->type() == C2DItem::Type_Text)
    {
        //TODO
    }
    else if (m_pItem->type() == C2DItem::Type_Rect)
    {
        //TODO
    }

}

void CPropEditor::valueChanged(QtProperty *property, const QSizeF &value)
{
    Q_ASSERT(m_pItem);


    //TODO, 此部分应当分散到各C2DItem类中实现，或者集中到专门类中
    if(m_pItem->type() == C2DItem::Type_Line)
    {
        //TODO

    }
    else if (m_pItem->type() == C2DItem::Type_Text)
    {
        //TODO
    }
    else if (m_pItem->type() == C2DItem::Type_Rect)
    {
        //TODO
    }
}

void CPropEditor::valueChanged(QtProperty *property, const QRectF &value)
{
    if(!m_propertyToId.contains(property))
        return;
    if(!m_pItem)
        return;

    QString id = m_propertyToId[property];


    //TODO, 此部分应当分散到各C2DItem类中实现，或者集中到专门类中
    if(m_pItem->type() == C2DItem::Type_Line)
    {
        //TODO

    }
    else if (m_pItem->type() == C2DItem::Type_Text)
    {
        //TODO
    }
    else if (m_pItem->type() == C2DItem::Type_Rect)
    {
        if(id == QLatin1String("rect"))
        {
            CRectItem *rcItem = qobject_cast<CRectItem*>(m_pItem);
            QRectF rect = value;
            QPointF pos = rect.center();

            rect.translate(-rect.center());
            rcItem->setPos(pos);
            rcItem->setRect(rect);
        }
    }
}

