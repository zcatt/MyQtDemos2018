/****************************************************************************
**
** Copyright (C) 2006 Trolltech ASA. All rights reserved.
**
** This file is part of the documentation of Qt. It was originally
** published as part of Qt Quarterly.
**
** This file may be used under the terms of the GNU General Public License
** version 2.0 as published by the Free Software Foundation or under the
** terms of the Qt Commercial License Agreement. The respective license
** texts for these are provided with the open source and commercial
** editions of Qt.
**
** If you are unsure which license is appropriate for your use, please
** review the following information:
** http://www.trolltech.com/products/qt/licensing.html or contact the
** sales department at sales@trolltech.com.
**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
**
****************************************************************************/

#include <QApplication>
#include "qtpropertymanager.h"
#include "qteditorfactory.h"
#include "filepathmanager.h"
#include "fileeditfactory.h"
#include "qttreepropertybrowser.h"

int main(int argc, char **argv)
{
    QApplication app(argc, argv);
    QtTreePropertyBrowser *browser = new QtTreePropertyBrowser();

    QtIntPropertyManager *intManager = new QtIntPropertyManager(browser);
    QtProperty *priority = intManager->addProperty("Priority");

    priority->setToolTip("Task Priority");

    intManager->setRange(priority, 1, 5);
    intManager->setValue(priority, 3);

    QtEnumPropertyManager *enumManager = new QtEnumPropertyManager(browser);
    QtProperty *reportType = enumManager->addProperty("Report Type");
    QStringList types;
    types << "Bug" << "Suggestion" << "To Do";
    enumManager->setEnumNames(reportType, types);
    enumManager->setValue(reportType, 1); // current value will be "Suggestion"

    QtGroupPropertyManager *groupManager = new QtGroupPropertyManager(browser);
    QtProperty *task1 = groupManager->addProperty("Task 1");

    task1->addSubProperty(priority);
    task1->addSubProperty(reportType);

    QtSpinBoxFactory *spinBoxFactory = new QtSpinBoxFactory(browser);
    QtEnumEditorFactory *enumFactory = new QtEnumEditorFactory(browser);
    browser->setFactoryForManager(intManager, spinBoxFactory);
    browser->setFactoryForManager(enumManager, enumFactory);

    browser->addProperty(task1);
    browser->show();

    FilePathManager *filePathManager = new FilePathManager(browser);
    QtProperty *example = filePathManager->addProperty("Example");
    filePathManager->setValue(example, "main.cpp");
    filePathManager->setFilter(example, "Source files (*.cpp *.c)");

    FileEditFactory *fileEditFactory = new FileEditFactory(browser);
    browser->setFactoryForManager(filePathManager, fileEditFactory);
    task1->addSubProperty(example);

    int ret =app.exec();

    delete browser;

    return ret;
}

