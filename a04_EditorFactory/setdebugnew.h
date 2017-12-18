#ifndef SET_DEBUG_NEW_H
#define SET_DEBUG_NEW_H

/*
    1.在需要检测的cpp文件中包含setdebugnew.h头文件
    2.main.cpp(在主函数入口处设置_CrtSetDbgFlag ( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );
*/

#ifdef _DEBUG
#define DEBUG_CLIENTBLOCK   new( _CLIENT_BLOCK, __FILE__, __LINE__)
#else
#define DEBUG_CLIENTBLOCK
#endif

#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>

#ifdef _DEBUG
#define new DEBUG_CLIENTBLOCK
#endif

#endif // end SET_DEBUG_NEW_H
