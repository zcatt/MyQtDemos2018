/*



Note on Porting
==================

Guide docs
----------------

Follow the below docs:
* freetype/docs/INSTALL
* freetype/docs/INSTALL.ANY
* freetype/docs/CUSTOMIZE

Configure
----------------

* config the include/freetype/config/ftoption.h
* if you use gnu make, configure the toplevel file 'module.cfg'. The gnu make generated the ftmodule.h.
* if you do not use gnu make, configure the include/freetype/config/ftmodule.h to add  or remove the drivers and components you want  to compile into  the library.
* The default freetype interface to the system is 'src/base/ftsystem.c'. You can use your custom implementation.
* Overriding default configuration and module headers. see the CUSTOMIZE for detail.

Here we directly comment out some modules in ftmodule.h
* sfnt_module_class, truetype/opentype需要此模块
* tt_driver_class, truetype需要此driver
* cff_driver_class, cff/opentype需要此driver
* psnames_module_class and pshinter_module_class， cff依赖这两个
* ft_smooth_render_class，选用这个render

We choose some freetype files to compile, see the freetype.pri in detail

ftsystem.c,
    the default is src/base/ftsystem.c;
    the linux version is builds/unix/ftsystem.c.  You maybe generate the builds/unix/ftconfig.h.



*/

#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    return a.exec();
}
