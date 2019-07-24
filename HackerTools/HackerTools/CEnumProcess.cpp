// CEnumProcess.cpp: 实现文件
//

#include "stdafx.h"
#include "HackerTools.h"
#include "CEnumProcess.h"
#include "afxdialogex.h"
#include <TlHelp32.h>

// CEnumProcess 对话框

IMPLEMENT_DYNAMIC(CEnumProcess, CDialogEx)

CEnumProcess::CEnumProcess(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_EnumProcess, pParent)
{

}

CEnumProcess::~CEnumProcess()
{
}

void CEnumProcess::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_List);
}


BEGIN_MESSAGE_MAP(CEnumProcess, CDialogEx)
END_MESSAGE_MAP()


// CEnumProcess 消息处理程序


BOOL CEnumProcess::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	BOOL bRet = EnumProcess();
	if (bRet==FALSE)
	{
		MessageBox(L"进程遍历失败！");
	}

	// TODO:  在此添加额外的初始化

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}



//************************************************************
// 函数名称: EnumProcess
// 函数说明: 进程遍历
// 作	 者: GuiShou
// 时	 间: 2019/1/29
// 参	 数: void
// 返 回 值: BOOL 是否成功
//************************************************************
BOOL CEnumProcess::EnumProcess()
{
	//包含进程信息的结构体 这个结构体长度在使用前必须设置
	PROCESSENTRY32 pe = { sizeof(PROCESSENTRY32) };
	//创建一个快照 参数一 要获取的快照类型  参数2 当获取模块 堆信息时 需要指定一个进程ID 所以此处无效
	HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if (hSnap==INVALID_HANDLE_VALUE)
	{
		MessageBox(L"CreateToolhelp32Snapshot Error");
		return FALSE;
	}

	m_List.InsertColumn(0, L"进程名", 0, 200);
	m_List.InsertColumn(1, L"ID", 0, 100);
	m_List.InsertColumn(2, L"优先级", 0, 100);
	m_List.InsertColumn(3, L"父进程ID", 0, 100);
	m_List.InsertColumn(4, L"持有线程数", 0, 100);

	//找到第一个进程的信息 参数1 快照句柄 参数2 进程信息
	if (Process32First(hSnap, &pe) == TRUE)
	{
		//循环遍历其他进程
		do
		{
			int i = 0;
			CString ProcessID;	//进程ID
			CString Priority;	//进程优先级
			CString FatherId;	//父进程ID
			CString ProcessNum; //持有线程数
			//把ID和优先级转成字符串
			ProcessID.Format(L"%d", pe.th32ProcessID);
			Priority.Format(L"%d", pe.pcPriClassBase);
			FatherId.Format(L"%d", pe.th32ParentProcessID);
			ProcessNum.Format(L"%d", pe.cntThreads);

			//把进程名添加到行
			m_List.InsertItem(i, pe.szExeFile);
			//把ID添加到行 最后一个参数是图标索引
			m_List.SetItemText(i, 1, ProcessID);
			//把优先级添加进表格内容
			m_List.SetItemText(i, 2, Priority);
			//把父进程ID添加进去
			m_List.SetItemText(i, 3, FatherId);
			//持有线程数
			m_List.SetItemText(i, 4, ProcessNum);

			i++;
		} while (Process32Next(hSnap, &pe));
	}
	//设置扩展属性
	m_List.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	return TRUE;
}
