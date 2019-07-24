// CDia_Core.cpp: 实现文件
//

#include "stdafx.h"
#include "HackerTools.h"
#include "CDia_Core.h"
#include "afxdialogex.h"


// CDia_Core 对话框

IMPLEMENT_DYNAMIC(CDia_Core, CDialogEx)

CDia_Core::CDia_Core(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_CORE, pParent)
{

}

CDia_Core::~CDia_Core()
{
}

void CDia_Core::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDia_Core, CDialogEx)
END_MESSAGE_MAP()


// CDia_Core 消息处理程序
