/*

扫雷游戏的需求
.菜单结构
    游戏(G)
        开局(N)           New Game
        --------
        初级(P)           Primary
        中级(I)           Intermediate
        高级(E)           advancE
        自定义(C)...       Custom
        ----------
        标记(?)(M)        Mark
        声音（S)           Sound
        ----------
        英雄榜(T)....          champion lis&T
        -----------
        退出(X)               eXit
     帮助（H)
        内容（C)       F1      content
        --------
        关于(A)               about

.布局
    显示分为上下两个部分，
    上部分有一个表情按钮和两个计数器，一个是剩余雷数量，一个是计时。
    下部分是雷区。
        1.未探明区：
        2.已经探明区： 0，1，2，3，4，5，6，7，8
        3.标记区： 小旗
        4.疑问区： ？
        5.地雷区： 引爆，未引爆，错误雷

.操作
    lbtn, 探雷
    rbtn， 标记雷，疑问雷
    rbtn+lbtn， 探周围雷

.级别设定
        级别          宽  高   雷数
        初级          9   9   10
        中级          16  16  40
        高级          30  16  99
        自定义
        最大值         30  24  10~435
        最小值         9  9

        限定条件： 地雷数 in [10, w*h*0.6]
.英雄榜
        初级，中级，高级

.颜色
    亮色， (255，255，255)
    正色， (192,192,192)
    暗色， (128,128,128)


*/

#include "cresource.h"
#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    int res = 0;

    QApplication a(argc, argv);

    CResource::LoadResource();
    MainWindow w;
    w.show();

    res = a.exec();

    CResource::UnloadResource();
    return res;
}
