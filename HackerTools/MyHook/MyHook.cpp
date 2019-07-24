// MyHook.cpp : 定义 DLL 应用程序的导出函数。
//

#include "stdafx.h"

extern HMODULE g_hDllMoudle;	//dll的句柄
//共享内存
#pragma data_seg("mydata")	//创建一个名为mydata的数据段
	HHOOK g_hHook = NULL;
#pragma data_seg()
#pragma comment(linker,"/SECTION:mydata,RWS")  //把mydata数据段设置为可读可写可共享




extern "C" _declspec(dllexport)
//钩子回调函数
LRESULT GetMsgProc(int code, WPARAM wParam, LPARAM lParam)
{
	MessageBoxA(0, "你中毒了", "你中毒了", 0);
	return CallNextHookEx(g_hHook, code, wParam, lParam);
}

extern "C" _declspec(dllexport)
//设置全局钩子
BOOL SetGlobalHook()
{
	g_hHook = SetWindowsHookEx(WH_GETMESSAGE, (HOOKPROC)GetMsgProc, g_hDllMoudle, 0);
	if (g_hHook==NULL)
	{
		return FALSE;
	}
	return TRUE;
}


//卸载全局钩子
