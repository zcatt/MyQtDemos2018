/*
    plugin的创建的要点,
    .pro文件中设定如下。 AddPlugin是plugin name，生成的lib文件的主干名。
               其中$$qtLibraryTarget()在win平台时，debug版本会追加字符d, 并增加后缀。
               The DESTDIR specifics the location the plugin placed where app loads the plugin.
               plugin jason defines some name-values used by plugin.

        TEMPLATE = lib
        CONFIG += plugin
        TARGET = $$qtLibraryTarget(AddPlugin)
        DESTDIR = ../plugins

        ............
        ............

        DISTFILES += \
            addplugin.json

     .定义plugin interface header文件。并使用Q_DECLARE_INTERFACE()声明。 该文件会被app引用
     .定义plugin class. The plugin class should extends CObject and the above interface,
              and it should use Q_PLUGIN_METADATA() and Q_INTERFACES to delcare. The class
              should implement methods declared in the plugin interface.


    app使用plugin的要点,
    .Include the plugin interface header.
    .After QCoreApplication initialization, load plugin from the dir.
    .The case should be considered that plugin cannot be loaded successfully.



 */
#include "widget.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Widget w;
    w.show();

    return a.exec();
}
