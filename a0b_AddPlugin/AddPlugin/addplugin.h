#ifndef ADDPLUGIN_H
#define ADDPLUGIN_H

#include <QObject>
#include <QtPlugin>

#include "addinterface.h"

class AddPlugin: public QObject, public CAddInterface
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID AddInterface_iid FILE "addplugin.json")
    Q_INTERFACES(CAddInterface)

public:
    int add(int a, int b) override;
};

#endif // ADDPLUGIN_H
