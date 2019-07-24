// CCmdResult.cpp: 实现文件
//

#include "stdafx.h"
#include "HackerTools.h"
#include "CCmdResult.h"
#include "afxdialogex.h"


// CCmdResult 对话框

IMPLEMENT_DYNAMIC(CCmdResult, CDialogEx)

CCmdResult::CCmdResult(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_CmdResult, pParent)
	, m_Cmd(_T("ping 127.0.0.1"))
	, m_Result(_T(""))
{

}

CCmdResult::~CCmdResult()
{
}

void CCmdResult::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_Cmd);
	DDX_Text(pDX, IDC_EDIT2, m_Result);
}


BEGIN_MESSAGE_MAP(CCmdResult, CDialogEx)
	ON_BN_CLICKED(IDC_ShowCmd, &CCmdResult::OnBnClickedShowcmd)
END_MESSAGE_MAP()


//************************************************************
// 函数名称: OnBnClickedShowcmd
// 函数说明: 获取CMD的执行结果
// 作	 者: GuiShou
// 时	 间: 2019/1/30
// 参	 数: TCHAR * pszCmd,cmd命令 TCHAR * pszResultBuffer接收CMD结果的缓冲区 DWORD dwResultBufferSize 缓冲区大小
// 返 回 值: BOOL
//************************************************************
BOOL CCmdResult::PipeCmd(char* pszCmd, char * pszResultBuffer, DWORD dwResultBufferSize)
{
	HANDLE hReadPipe = NULL;
	HANDLE hWritePipe = NULL;
	SECURITY_ATTRIBUTES securityAttributes = { 0 };
	BOOL bRet = FALSE;
	STARTUPINFO si = { 0 };
	PROCESS_INFORMATION pi = { 0 };

	// 设定管道的安全属性
	securityAttributes.bInheritHandle = TRUE;
	securityAttributes.nLength = sizeof(securityAttributes);
	securityAttributes.lpSecurityDescriptor = NULL;
	// 创建匿名管道
	bRet = ::CreatePipe(&hReadPipe, &hWritePipe, &securityAttributes, 0);
	if (FALSE == bRet)
	{
		MessageBox(L"CreatePipe Error");
		return FALSE;
	}
	// 设置新进程参数
	si.cb = sizeof(si);
	si.dwFlags = STARTF_USESHOWWINDOW | STARTF_USESTDHANDLES;
	si.wShowWindow = SW_HIDE;
	si.hStdError = hWritePipe;
	si.hStdOutput = hWritePipe;
	// 创建新进程执行命令, 将执行结果写入匿名管道中
	USES_CONVERSION;
	bRet = ::CreateProcess(NULL, A2W(pszCmd), NULL, NULL, TRUE, 0, NULL, NULL, &si, &pi);
	if (FALSE == bRet)
	{
		MessageBox(L"CreateProcess Error");
	}

	// 等待命令执行结束
	::WaitForSingleObject(pi.hThread, INFINITE);
	::WaitForSingleObject(pi.hProcess, INFINITE);
	// 从匿名管道中读取结果到输出缓冲区
	::RtlZeroMemory(pszResultBuffer, dwResultBufferSize);
	::ReadFile(hReadPipe, pszResultBuffer, dwResultBufferSize, NULL, NULL);
	// 关闭句柄, 释放内存
	::CloseHandle(pi.hThread);
	::CloseHandle(pi.hProcess);
	::CloseHandle(hWritePipe);
	::CloseHandle(hReadPipe);

	return TRUE;
}



//************************************************************
// 函数名称: OnBnClickedShowcmd
// 函数说明: 响应执行并获取CMD结果按钮
// 作	 者: GuiShou
// 时	 间: 2019/1/30
// 参	 数: void
// 返 回 值: void
//************************************************************
void CCmdResult::OnBnClickedShowcmd()
{
	UpdateData(TRUE);
	//检测命令是否为空
	if (m_Cmd==L"")
	{
		MessageBox(L"CMD命令不能为空");
		return;
	}
	//清空之前的显示结果
	m_Result = "";

	//获取需要执行的CMD命令
	char szResultBuffer[512] = { 0 };
	DWORD dwResultBufferSize = 512;
	//执行CMD命令 并获取执行结果数据
	USES_CONVERSION;
	if (FALSE==PipeCmd(W2A(m_Cmd), szResultBuffer, 512))
	{
		MessageBox(L"pipe cmd error");
	}
	//显示到界面
	m_Result += A2W(szResultBuffer);
	UpdateData(FALSE);
}

