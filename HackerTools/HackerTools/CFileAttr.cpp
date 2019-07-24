// CFileAttr.cpp: 实现文件
//

#include "stdafx.h"
#include "HackerTools.h"
#include "CFileAttr.h"
#include "afxdialogex.h"
#include <wincrypt.h>


// CFileAttr 对话框

IMPLEMENT_DYNAMIC(CFileAttr, CDialogEx)

CFileAttr::CFileAttr(LPCTSTR file, CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_FileAttr, pParent), m_CurrentFile(file)		//获取当前文件完整路径
	, m_Name(_T(""))
	, m_Type(_T(""))
	, m_Path(_T(""))
	, m_CreatTime(_T(""))
	, m_LVisitTime(_T(""))
	, m_LChangeTime(_T(""))
	, m_Read(FALSE)
	, m_Hide(FALSE)
{

}

CFileAttr::~CFileAttr()
{
}

void CFileAttr::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_Name);
	DDX_Text(pDX, IDC_EDIT4, m_Type);
	DDX_Text(pDX, IDC_EDIT5, m_Path);
	DDX_Text(pDX, IDC_EDIT6, m_CreatTime);
	DDX_Text(pDX, IDC_EDIT2, m_LVisitTime);
	DDX_Text(pDX, IDC_EDIT8, m_LChangeTime);
	DDX_Check(pDX, IDC_CHECK1, m_Read);
	DDX_Check(pDX, IDC_CHECK2, m_Hide);
}


BEGIN_MESSAGE_MAP(CFileAttr, CDialogEx)
END_MESSAGE_MAP()


// CFileAttr 消息处理程序


BOOL CFileAttr::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	//查看文件属性
	CheckFileAttr();

	return TRUE;  
}


//************************************************************
// 函数名称: CheckFileAttr
// 函数说明: 查看文件属性
// 作	 者: GuiShou
// 时	 间: 2019/1/30
// 参	 数: void
// 返 回 值: void
//************************************************************
void CFileAttr::CheckFileAttr()
{
	SHFILEINFO ShInfo = { 0 };
	WCHAR FullPath[MAX_PATH] = {}, *Name;
	SHGetFileInfo(m_CurrentFile, 0, &ShInfo, sizeof(ShInfo), SHGFI_DISPLAYNAME);

	GetFullPathName(m_CurrentFile, MAX_PATH, FullPath, &Name);
	m_Path = FullPath;
	SHGetFileInfo(FullPath, 0, &ShInfo, sizeof(ShInfo), SHGFI_DISPLAYNAME);
	m_Name = ShInfo.szDisplayName;
	SHGetFileInfo(FullPath, 0, &ShInfo, sizeof(ShInfo), SHGFI_TYPENAME);
	m_Type = ShInfo.szTypeName;
	SHGetFileInfo(FullPath, 0, &ShInfo, sizeof(ShInfo), SHGFI_ICON);

	WIN32_FIND_DATA FileInfo = { 0 };
	FindFirstFile(FullPath, &FileInfo);
	FILETIME Time = { 0 };

	//获取创建时间
	FileTimeToLocalFileTime(&FileInfo.ftCreationTime, &Time);
	SYSTEMTIME sysTime = { 0 };
	FileTimeToSystemTime(&Time, &sysTime);
	m_CreatTime.Format(L"%d-%d-%d %d:%d:%d", sysTime.wYear,
		sysTime.wMonth, sysTime.wDay, sysTime.wHour, sysTime.wMinute,
		sysTime.wSecond);
	//获取最后访问时间
	FileTimeToLocalFileTime(&FileInfo.ftLastAccessTime, &Time);
	FileTimeToSystemTime(&Time, &sysTime);
	m_LVisitTime.Format(L"%d-%d-%d %d:%d:%d", sysTime.wYear,
		sysTime.wMonth, sysTime.wDay, sysTime.wHour, sysTime.wMinute,
		sysTime.wSecond);
	//获取最后修改时间
	FileTimeToLocalFileTime(&FileInfo.ftLastWriteTime, &Time);
	FileTimeToSystemTime(&Time, &sysTime);
	m_LChangeTime.Format(L"%d-%d-%d %d:%d:%d", sysTime.wYear,
		sysTime.wMonth, sysTime.wDay, sysTime.wHour, sysTime.wMinute,
		sysTime.wSecond);
	//设置只读复选框
	if (FileInfo.dwFileAttributes&FILE_ATTRIBUTE_READONLY)
		m_Read = TRUE;
	//设置隐藏复选框
	if (FileInfo.dwFileAttributes&FILE_ATTRIBUTE_HIDDEN)
		m_Hide = TRUE;

	UpdateData(FALSE);
}


