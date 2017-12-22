/*
   演示了QItemEditorFactory的使用，包括配置全局的默认editorFactory以及定制column的editorFactory.
   特别注意QItemEditorFactory的释放问题。全局的会自动释放。

   另外也演示了如何使用_CrtSetDbgFlag()进行内存泄露检查，只不过默认输出到win debugger，
   可以转换为visual stuido project，在vs中观察泄露输出消息。

   Qt工程转换成VS工程的方法：
        在windows下，运行Qt Command Prompt。
        输入命令行：
            qmake -tp vc XXX.pro
        会生成文件XXX.vcxproj

   tech:

   .editor class should define the editted property by 'USER', such as
            Q_PROPERTY(QColor color READ color WRITE setColor USER true)
    so QItemEditorFactory can find which the editor can process.

   .QItemEditorFactory.setDefaultFactory()设置的是全局使用的editorFactory

   .QStyledItemDelegate::setItemEditorFactory()设置的是local editorFactory。


*/
#include "widget.h"

#include <QApplication>

#include "setdebugnew.h"

int main(int argc, char *argv[])
{
#ifdef _DEBUG
      _CrtSetDbgFlag ( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );
#endif

    QApplication a(argc, argv);
    Widget w;
    w.show();

    return a.exec();
}
