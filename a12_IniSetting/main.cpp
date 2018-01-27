/*
     使用QSettings将信息保存到ini.
     注意点，
     .ini保存的位置与scope, orgName, appName相关
     .ini中可以group嵌套保存

 */
#include "dialog.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Dialog w;
    w.show();

    return a.exec();
}
