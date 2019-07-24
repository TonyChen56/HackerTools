// ServiceExeTest.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include "pch.h"
#include <windows.h>
#include<stdio.h>


// 服务入口函数以及处理回调函数
void __stdcall ServiceMain(DWORD dwArgc, char *lpszArgv);
void __stdcall ServiceCtrlHandle(DWORD dwOperateCode);
BOOL TellSCM(DWORD dwState, DWORD dwExitCode, DWORD dwProgress);
void DoTask();

//服务名
char g_szServiceName[MAX_PATH] = "GuiShou.exe";
SERVICE_STATUS_HANDLE g_ServiceStatusHandle = { 0 };	//服务状态句柄


//主进程通知SCM在可执行文件中含有几个服务，
//并且给出每一个服务的ServiceMain回调(Call Back)函数的地址。
int main()
{
	//完成ServiceMain的初始化
	//这个结构体记录了这个服务程序里面所包含的所有服务的名称和服务的入口函数
	SERVICE_TABLE_ENTRY stDispatchTable[] = { {g_szServiceName,(LPSERVICE_MAIN_FUNCTION)ServiceMain},{NULL,NULL} };

	//调用StartServiceCtrlDispatcher和SCM通讯
	//StartServiceCtrlDispatcher会为每一个传递到它的数组中的非空元素产生一个新的线程
	StartServiceCtrlDispatcher(stDispatchTable);

	return 0;
}

// 服务入口函数以及处理回调函数
void __stdcall ServiceMain(DWORD dwArgc, char *lpszArgv)
{
	//1.调用RegisterServiceCtrlHandler函数去通知SCM它的CtrlHandler回调函数的地址
	g_ServiceStatusHandle = RegisterServiceCtrlHandler(g_szServiceName, ServiceCtrlHandle);
	//2.调用SetServiceStatus告诉SCM服务正在继续初始化
	TellSCM(SERVICE_START_PENDING, 0, 1);
	TellSCM(SERVICE_RUNNING, 0, 0);

	// 自己程序实现部分代码放在这里
	// !!注意!! 此处一定要为死循环, 否则在关机再开机的情况(不是点击重启), 不能创建用户进程
	while (TRUE)
	{
		Sleep(5000);
		DoTask();
	}

}

//服务控制函数
void __stdcall ServiceCtrlHandle(DWORD dwOperateCode)
{
	switch (dwOperateCode)
	{
	case SERVICE_CONTROL_PAUSE:
	{
		// 暂停
		TellSCM(SERVICE_PAUSE_PENDING, 0, 1);
		TellSCM(SERVICE_PAUSED, 0, 0);
		break;
	}
	case SERVICE_CONTROL_CONTINUE:
	{
		// 继续
		TellSCM(SERVICE_CONTINUE_PENDING, 0, 1);
		TellSCM(SERVICE_RUNNING, 0, 0);
		break;
	}
	case SERVICE_CONTROL_STOP:
	{
		// 停止
		TellSCM(SERVICE_STOP_PENDING, 0, 1);
		TellSCM(SERVICE_STOPPED, 0, 0);
		break;
	}
	case SERVICE_CONTROL_INTERROGATE:
	{
		// 询问
		break;
	}
	default:
		break;
	}
}

BOOL TellSCM(DWORD dwState, DWORD dwExitCode, DWORD dwProgress)
{
	SERVICE_STATUS ServiceStatus = { 0 };
	BOOL bRet = FALSE;
	RtlZeroMemory(&ServiceStatus, sizeof(ServiceStatus));

	//服务可执行文件的类型
	ServiceStatus.dwServiceType = SERVICE_WIN32_OWN_PROCESS;
	//告诉SCM你的服务的现行状态
	ServiceStatus.dwCurrentState = dwState;	
	//指明服务愿意接受什么样的控制通知 允许停止服务 暂停或继续服务 关闭服务
	ServiceStatus.dwControlsAccepted = SERVICE_ACCEPT_STOP | SERVICE_ACCEPT_PAUSE_CONTINUE | SERVICE_ACCEPT_SHUTDOWN;
	//设置退出码
	ServiceStatus.dwWin32ExitCode = dwExitCode;
	ServiceStatus.dwWaitHint = 3000;

	//设置服务运行状态
	bRet = SetServiceStatus(g_ServiceStatusHandle, &ServiceStatus);
	return bRet;
}

void DoTask()
{
	//自己的代码写在这里
	printf("这里是服务控制程序！");
	system("pause");
}