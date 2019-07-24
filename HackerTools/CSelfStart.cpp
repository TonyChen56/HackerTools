// CSelfStart.cpp: 实现文件
//

#include "stdafx.h"
#include "HackerTools.h"
#include "CSelfStart.h"
#include "afxdialogex.h"
#include <winsvc.h>
#include "CMyTaskSchedule.h"




IMPLEMENT_DYNAMIC(CSelfStart, CDialogEx)

CSelfStart::CSelfStart(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_SelfStart, pParent)
	, m_Path(_T("F:\\FileCleaner2.0.exe"))
	, m_Tips(_T("计划任务和系统服务需使用管理员本启动程序！！！\r\n"))
{

}

CSelfStart::~CSelfStart()
{
}

void CSelfStart::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_Path);
	DDX_Text(pDX, IDC_EDIT2, m_Tips);
}


BEGIN_MESSAGE_MAP(CSelfStart, CDialogEx)
	ON_WM_DROPFILES()
	ON_BN_CLICKED(IDC_Regedit, &CSelfStart::OnBnClickedRegedit)
	ON_BN_CLICKED(IDC_StartDir, &CSelfStart::OnBnClickedStartdir)
	ON_BN_CLICKED(IDC_Task, &CSelfStart::OnBnClickedTask)
	ON_BN_CLICKED(IDC_Service, &CSelfStart::OnBnClickedService)
END_MESSAGE_MAP()


// CSelfStart 消息处理程序


BOOL CSelfStart::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	//管理员模式下取消过滤拖拽消息
	ChangeWindowMessageFilterEx(m_hWnd, WM_DROPFILES, MSGFLT_ALLOW, NULL);
	ChangeWindowMessageFilterEx(m_hWnd, 0x0049, MSGFLT_ALLOW, NULL);

	return TRUE;  
}



//************************************************************
// 函数名称: OnDropFiles 
// 函数说明: 响应拖拽文件按钮
// 作	 者: GuiShou
// 时	 间: 2019/1/21
// 参	 数: HDROP hDropInfo 拖拽文件句柄
// 返 回 值: void
//************************************************************
void CSelfStart::OnDropFiles(HDROP hDropInfo)
{
	//清空文本框的内容
	m_Path = "";

	//获取文件路径
	TCHAR szPath[MAX_PATH] = { 0 };
	DragQueryFile(hDropInfo, 0, szPath, MAX_PATH);

	//检测是否为exe
	LPTSTR pszExtension = PathFindExtension(szPath);
	if (lstrcmp(pszExtension, L".exe") != 0)
	{
		MessageBox(L"请先拖拽有效的exe文件");
		return;
	}
	//显示到控件上
	m_Path = szPath;
	UpdateData(FALSE);

	CDialogEx::OnDropFiles(hDropInfo);
}




//************************************************************
// 函数名称: OnBnClickedRegedit 
// 函数说明: 注册表实现自启动
// 作	 者: GuiShou
// 时	 间: 2019/1/21
// 参	 数: void
// 返 回 值: void
//************************************************************
void CSelfStart::OnBnClickedRegedit()
{
	//检测文件是否存在
	if (GetFileAttributes(m_Path)==INVALID_FILE_ATTRIBUTES)
	{
		MessageBox(L"文件不存在 请重试");
		return;
	}

	//清空文本框的内容
	m_Tips = "";

	//修改注册表
	USES_CONVERSION;
	if (SetRegAutoRun(W2A(m_Path), "GuiShou"))
	{
		m_Tips += "已将目标程序设置到以下注册表项!\r\n";
		m_Tips += "\\HKEY_CURRENT_USER\\SOFTWARE\\Microsoft\\Windows-\r\n";
		m_Tips += "\\CurrentVersion\\Run\r\n";
		m_Tips += "可自行查看或删除\r\n";
	}
	else
	{
		m_Tips += "设置到注册表项失败！\r\n";
	}

	UpdateData(FALSE);
}



//************************************************************
// 函数名称: OnBnClickedStartdir 
// 函数说明: 快速启动目录实现自启动
// 作	 者: GuiShou
// 时	 间: 2019/1/21
// 参	 数: void
// 返 回 值: void
//************************************************************
void CSelfStart::OnBnClickedStartdir()
{
	//检测文件是否存在
	if (GetFileAttributes(m_Path) == INVALID_FILE_ATTRIBUTES)
	{
		MessageBox(L"文件不存在 请重试");
		return;
	}

	//清空文本框的内容
	m_Tips = "";
	USES_CONVERSION;
	if (FastStartDirectory(W2A(m_Path)))
	{
		m_Tips += "目标程序已添加到以下快速启动目录\r\n";
		m_Tips += m_FastStartDir;
		m_Tips += "\r\n";
		m_Tips += "可自行查看或删除\r\n";
	}
	else
	{
		m_Tips += "设置快速启动目录失败！\r\n";
	}

	UpdateData(FALSE);

}


//************************************************************
// 函数名称: OnBnClickedTask 
// 函数说明: 计划任务实现自启动
// 作	 者: GuiShou
// 时	 间: 2019/1/21
// 参	 数: void
// 返 回 值: void
//************************************************************
void CSelfStart::OnBnClickedTask()
{
	//检测文件是否存在
	if (GetFileAttributes(m_Path) == INVALID_FILE_ATTRIBUTES)
	{
		MessageBox(L"文件不存在 请重试");
		return;
	}

	//清空文本框的内容
	m_Tips = "";

	BOOL bRet = FALSE;
	//创建计划任务
	CMyTaskSchedule task;
	USES_CONVERSION;
	bRet = task.NewTask("GuiShou", W2A(m_Path), "", "GuiShou");
	if (FALSE == bRet)
	{
		m_Tips += "Create Task Schedule Error!\r\n";
	}
	else
	{
		m_Tips += "任务计划已设置成功\r\n";
	}

	UpdateData(FALSE);
}




//************************************************************
// 函数名称: OnBnClickedService 
// 函数说明: 系统服务实现自启动
// 作	 者: GuiShou
// 时	 间: 2019/1/21
// 参	 数: void
// 返 回 值: void
//************************************************************
void CSelfStart::OnBnClickedService()
{
	//检测文件是否存在
	if (GetFileAttributes(m_Path) == INVALID_FILE_ATTRIBUTES)
	{
		MessageBox(L"文件不存在 请重试");
		return;
	}

	//清空文本框的内容
	m_Tips = "";

	//创建服务
	USES_CONVERSION;
	BOOL bRet = SystemServiceOperate(W2A(m_Path), Load_Service);
	if (bRet==FALSE)
	{
		m_Tips += "Load_Service Error\r\n";
		UpdateData(FALSE);
		return;
	}
	else
	{
		m_Tips += "Load_Service Success\r\n";
	}
	//启动服务
	bRet = SystemServiceOperate(W2A(m_Path), Start_Service);
	if (bRet == FALSE)
	{
		m_Tips += "Start_Service Error\r\n";
		UpdateData(FALSE);
		return;
	}
	else
	{
		m_Tips += "Start_Service Success\r\n";
	}

	UpdateData(FALSE);
}


//************************************************************
// 函数名称: SystemServiceOperate 
// 函数说明: 服务操作函数
// 作	 者: GuiShou
// 时	 间: 2019/1/21
// 参	 数: void
// 返 回 值: void
//************************************************************
BOOL CSelfStart::SystemServiceOperate(char* csPath, int iOperateType)
{
	BOOL bRet = TRUE;
	char szName[MAX_PATH] = { 0 };
	lstrcpyA(szName, csPath);

	//移除szName指定的路径的目录部分
	PathStripPathA(szName);

	SC_HANDLE shOSCM = NULL, shCS = NULL;
	SERVICE_STATUS ss;
	DWORD dwErrorCode = 0;
	BOOL bSuccess = FALSE;

	//打开服务控制管理器数据库
	shOSCM = OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);
	if (!shOSCM)
	{
		m_Tips += "OpenSCManager Error\r\n";
		return FALSE;
	}

	if (iOperateType!=Load_Service)
	{
		//打开一个已存在的服务
		shCS = OpenServiceA(shOSCM, szName, SERVICE_ALL_ACCESS);
		if (!shCS)
		{
			m_Tips += "OpenService Error\r\n";
			CloseServiceHandle(shOSCM);
			return FALSE;
		}
	}

	switch (iOperateType)
	{
		//创建服务
	case CSelfStart::Load_Service:
	{
		//创建一个随系统启动而启动的服务
		shCS = CreateServiceA(shOSCM, szName, szName, SERVICE_ALL_ACCESS,
			SERVICE_WIN32_OWN_PROCESS | SERVICE_INTERACTIVE_PROCESS,
			SERVICE_AUTO_START, SERVICE_ERROR_NORMAL, csPath, NULL,
			NULL, NULL, NULL, NULL);
		if (!shCS)
		{
			m_Tips += "CreateService Error\r\n";
			bRet = FALSE;
		}
	}
		break;
		//启动服务
	case CSelfStart::Start_Service:
	{
		if (!StartService(shCS,0,NULL))
		{
			m_Tips += "StartService Error\r\n";
			bRet = FALSE;
		}
	}
		break;
		//停止服务
	case CSelfStart::Stop_Service:
	{
		if (!ControlService(shOSCM,SERVICE_CONTROL_STOP,&ss))
		{
			m_Tips += "ControlService Error\r\n";
			bRet = FALSE;
		}
	}
		break;
		//删除服务
	case CSelfStart::Delete_Service:
	{
		if (!DeleteService(shCS))
		{
			m_Tips += "DeleteService Error\r\n";
			bRet = FALSE;
		}
	}
		break;
	default:
		break;
	}

	//关闭句柄
	if (shCS)
	{
		CloseServiceHandle(shCS);
	}
	if (shOSCM)
	{
		CloseServiceHandle(shOSCM);
	}
	return bRet;
}


//************************************************************
// 函数名称: SetRegAutoRun 
// 函数说明: 设置注册表使程序自启动
// 作	 者: GuiShou
// 时	 间: 2019/1/21
// 参	 数: lpszFileName 需要设置自启动的文件路径 
//			 lpszValueName 设置的注册表启动项名
// 返 回 值: BOOL 是否成功
//************************************************************
BOOL CSelfStart::SetRegAutoRun(const char* lpszFileName, const char* lpszValueName)
{
	HKEY hKey;
	//打开注册表键
	if (ERROR_SUCCESS != RegOpenKeyExA(HKEY_CURRENT_USER, "Software\\Microsoft\\Windows\\CurrentVersion\\Run", 0, KEY_WRITE, &hKey))
	{
		m_Tips += "RegOpenKeyExA ERROR\r\n";
		return FALSE;
	}

	//修改注册表键值 实现开机自启动
	if (ERROR_SUCCESS != RegSetValueExA(hKey, lpszValueName, 0, REG_SZ, (BYTE*)lpszFileName, strlen(lpszFileName) + 1))
	{
		RegCloseKey(hKey);
		m_Tips += "RegSetValueExA ERROR\r\n";
		return FALSE;
	}

	//关闭注册表键
	RegCloseKey(hKey);
	return TRUE;
}



//************************************************************
// 函数名称: FastStartDirectory 
// 函数说明: 使用快速启动目录使程序自启动
// 作	 者: GuiShou
// 时	 间: 2019/1/21
// 参	 数: lpszSrcFilePath  源程序路径
//			 lpszDestFileName 快速启动目录路径
// 返 回 值: BOOL 是否成功
//************************************************************
BOOL CSelfStart::FastStartDirectory(const char* lpszSrcFilePath)
{
	BOOL bRet = FALSE;
	char szStartupPath[MAX_PATH] = { 0 };
	char szDestFilePath[MAX_PATH] = { 0 };
	char TempSrcPath[MAX_PATH] = { 0 };

	strcpy_s(TempSrcPath, MAX_PATH,lpszSrcFilePath );
	//获取快速启动目录的路径
	bRet = SHGetSpecialFolderPathA(NULL, szStartupPath, CSIDL_STARTUP, TRUE);

	//保存快速启动目录
	USES_CONVERSION;
	m_FastStartDir = A2W(szStartupPath);
	if (FALSE == bRet)
	{
		return FALSE;
	}

	//获取目标文件名
	PathStripPathA((char*)lpszSrcFilePath);

	//拼接路径
	wsprintfA(szDestFilePath, "%s\\%s", szStartupPath, lpszSrcFilePath);

	//拷贝文件到快速启动目录下
	bRet = CopyFileA(TempSrcPath, szDestFilePath, FALSE);
	if (FALSE == bRet)
	{
		return FALSE;
	}
	return TRUE;
}