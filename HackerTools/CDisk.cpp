// CDisk.cpp: 实现文件
//

#include "stdafx.h"
#include "HackerTools.h"
#include "CDisk.h"
#include "afxdialogex.h"


LPCTSTR TypeName[7] = {
	TEXT("未知设备"),
	TEXT("无效根路径"),
	TEXT("可移动设备"),
	TEXT("不可移动设备"),
	TEXT("网络驱动器"),
	TEXT("CD-ROM 驱动器"),
	TEXT("随机存储器"),
};

// CDisk 对话框

IMPLEMENT_DYNAMIC(CDisk, CDialogEx)

CDisk::CDisk(LPCTSTR name, CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_Disk, pParent), m_path(name)	//用传过来的name初始化m_path
	, m_Disk(_T(""))
	, m_DiskName(_T(""))
	, m_System(_T(""))
	, m_FreeSpace(_T(""))
	, m_TotalSpace(_T(""))
	, m_Info(_T(""))
{

}

CDisk::~CDisk()
{
}

void CDisk::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_Disk);
	DDX_Text(pDX, IDC_EDIT4, m_DiskName);
	DDX_Text(pDX, IDC_EDIT5, m_System);
	DDX_Text(pDX, IDC_EDIT6, m_FreeSpace);
	DDX_Text(pDX, IDC_EDIT7, m_TotalSpace);
	DDX_Text(pDX, IDC_EDIT8, m_Info);
}


BEGIN_MESSAGE_MAP(CDisk, CDialogEx)
END_MESSAGE_MAP()


// CDisk 消息处理程序


BOOL CDisk::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	CheckDisk();	//查看磁盘
	return TRUE;  
}


//************************************************************
// 函数名称: CheckDisk
// 函数说明: 查看磁盘
// 作	 者: GuiShou
// 时	 间: 2019/1/30
// 参	 数: void
// 返 回 值: void
//************************************************************
void CDisk::CheckDisk()
{
	m_Disk = m_path;		//显示磁盘
	UINT type = GetDriveType(m_path);	//获取磁盘类型
	m_Info = TypeName[type];		//显示磁盘信息
	TCHAR Name[MAX_PATH] = { 0 };
	TCHAR System[MAX_PATH] = { 0 };
	//获取硬盘信息
	GetVolumeInformation(m_path, Name, MAX_PATH, NULL, NULL, NULL, System, MAX_PATH);
	//显示磁盘名
	m_DiskName = Name;
	if (m_DiskName == L"")
	{
		m_DiskName = L"本地磁盘";
	}

	m_System = System;	//显示磁盘系统
	//显示磁盘空间
	DWORD a = 0, b = 0, c = 0, d = 0;
	GetDiskFreeSpace(m_path, &a, &b, &c, &d);
	double FreeSize = (c / 1024.0) * a * b / 1024 / 1024;
	double TotalSize = (d / 1024.0) * a * b / 1024 / 1024;
	m_FreeSpace.Format(L"%.2lfGB", FreeSize);
	m_TotalSpace.Format(L"%.2lfGB", TotalSize);

	UpdateData(FALSE);
}
