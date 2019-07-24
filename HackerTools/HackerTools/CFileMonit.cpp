// CFileMonit.cpp: 实现文件
//

#include "stdafx.h"
#include "HackerTools.h"
#include "CFileMonit.h"
#include "afxdialogex.h"



// CFileMonit 对话框

IMPLEMENT_DYNAMIC(CFileMonit, CDialogEx)

CFileMonit::CFileMonit(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_FileMonit, pParent)
	, m_Tip(_T(""))
	, m_FileMonit(_T(""))
{

}

CFileMonit::~CFileMonit()
{
}

void CFileMonit::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_Tip);
	DDX_Text(pDX, IDC_EDIT4, m_FileMonit);
}


BEGIN_MESSAGE_MAP(CFileMonit, CDialogEx)
END_MESSAGE_MAP()


// CFileMonit 消息处理程序

UINT MonitorFileThreadProc(LPVOID lpVoid);

BOOL CFileMonit::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	////获取所有磁盘信息
	//char Disk[MAX_PATH] = { 0 };
	//GetLogicalDriveStringsA(MAX_PATH, Disk);
	//
	////循环监控所有的磁盘
	//for (int i = 0; Disk[i] != 0; i++)
	//{
	//	CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)MonitorFileThreadProc, &Disk[i], 0, NULL);
	//	i += strlen(&Disk[i]);
	//}
	//
	//监控文件
	MonitorFiles("D:\\");

	return TRUE;  
}




//************************************************************
// 函数名称: MonitorFileThreadProc
// 函数说明: 监控文件的线程回调函数
// 作	 者: GuiShou
// 时	 间: 2019/1/30
// 参	 数: lpVoid 要监控的文件路径
// 返 回 值: void
//************************************************************
UINT MonitorFileThreadProc(LPVOID lpVoid)
{
	char* pszDirectory = (char*)lpVoid;
	//打开目录 获取文件句柄
	HANDLE hDirectory = CreateFileA(pszDirectory, FILE_LIST_DIRECTORY,
	FILE_SHARE_READ|FILE_SHARE_WRITE,NULL,OPEN_EXISTING,FILE_FLAG_BACKUP_SEMANTICS,NULL);
	if (hDirectory==INVALID_HANDLE_VALUE)
	{
		MessageBoxA(0,"CreateFileA Error","Error",MB_OK);
		return 1;
	}
	char szTemp[MAX_PATH] = { 0 };
	BOOL bRet = FALSE;
	DWORD dwRet = 0;
	DWORD dwBufferSize = 2048;

	//申请一个足够大的缓冲区 存放返回的数据
	BYTE* pBuf = new BYTE[dwBufferSize]{ 0 };
	if (pBuf==NULL)
	{
		MessageBoxA(0, "new Error", "Error", MB_OK);
		return 2;
	}
	FILE_NOTIFY_INFORMATION *pFilenotifyInfo = (FILE_NOTIFY_INFORMATION*)pBuf;
	
	//发消息让Edit控件显示内容
	HWND hWnd = FindWindowA(NULL, "文件监控");
	HWND hEdit = GetDlgItem(hWnd, IDC_EDIT4);
	CString Temp;
	//开始循环设置监控
	do 
	{
		//设置监控目录
		bRet = ReadDirectoryChangesW(hDirectory, 
			pFilenotifyInfo, 
			dwBufferSize, 
			TRUE,FILE_NOTIFY_CHANGE_FILE_NAME | FILE_NOTIFY_CHANGE_DIR_NAME |
			FILE_NOTIFY_CHANGE_ATTRIBUTES | FILE_NOTIFY_CHANGE_SIZE |
			FILE_NOTIFY_CHANGE_LAST_WRITE | FILE_NOTIFY_CHANGE_LAST_ACCESS |
			FILE_NOTIFY_CHANGE_CREATION | FILE_NOTIFY_CHANGE_SECURITY, 
			&dwRet, NULL, NULL);
		if (bRet==FALSE)
		{
			MessageBoxA(0, "ReadDirectoryChangesW Error", "Error", MB_OK);
			break;
		}
		
	
		//判断操作类型并显示
		switch (pFilenotifyInfo->Action)
		{
		//新增文件
		case FILE_ACTION_ADDED:
		{
			Temp.Format(L"%s:%s\r\n", L"新增文件", pFilenotifyInfo->FileName);
			break;
		}
		//删除文件
		case FILE_ACTION_REMOVED:
		{
			Temp.Format(L"%s:%s", L"删除文件", pFilenotifyInfo->FileName);
			break;
		}
		//修改文件
		case FILE_ACTION_MODIFIED:
		{
			Temp.Format(L"%s:%s", L"修改文件", pFilenotifyInfo->FileName);
			break;
		}
		//重命名文件
		case FILE_ACTION_RENAMED_OLD_NAME:
		{
			Temp.Format(L"%s:%s", L"重命名文件", pFilenotifyInfo->FileName);
			break;
		}
		case FILE_ACTION_RENAMED_NEW_NAME:
		{
			Temp.Format(L"%s:%s", L"重命名文件", szTemp);
			break;
		}
		default:
			break;
		}
		::SendMessageW(hEdit, WM_SETTEXT, 50, (LPARAM)Temp.	GetBuffer());

	} while (bRet);

	//关闭句柄 释放内存
	CloseHandle(hDirectory);
	delete[] pBuf;
	return 0;
}


//************************************************************
// 函数名称: MonitorFiles
// 函数说明: 使用多线程监控文件
// 作	 者: GuiShou
// 时	 间: 2019/1/30
// 参	 数: char* pszDirectory 要监控的文件路径
// 返 回 值: void
//************************************************************
void CFileMonit::MonitorFiles(char* pszDirectory)
{
	CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)MonitorFileThreadProc, pszDirectory, 0, NULL);
}



