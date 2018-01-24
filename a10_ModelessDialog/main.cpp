/*
    modeless dialog的使用。
    对于再次打开的modeless dialog, 希望其出现在上次出现的位置。实现的方案是
    .在closeEvent()中保留当前位置
    .在创建和显示新的modeless dialog时，移动到保存的位置。
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
