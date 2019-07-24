// CBase.cpp: 实现文件
//

#include "stdafx.h"
#include "HackerTools.h"
#include "CBase.h"
#include "afxdialogex.h"
#include "resource.h"


// CBase 对话框




IMPLEMENT_DYNAMIC(CBase, CDialogEx)

CBase::CBase(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_Base, pParent)
	, m_Edit(_T(""))
{

}

CBase::~CBase()
{
}

void CBase::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_Edit);
}


BEGIN_MESSAGE_MAP(CBase, CDialogEx)
	ON_BN_CLICKED(IDC_RunSingle, &CBase::OnBnClickedRunsingle)
	ON_BN_CLICKED(IDC_ReleaseRes, &CBase::OnBnClickedReleaseres)
END_MESSAGE_MAP()



//************************************************************
// 函数名称: OnBnClickedRunsingle
// 函数说明: 防双开按钮
// 作	 者: GuiShou
// 时	 间: 2018/12/18
// 参	 数: void
// 返 回 值: void
//************************************************************
void CBase::OnBnClickedRunsingle()
{
	MessageBox(L"防双开已默认开启成功");
}


//************************************************************
// 函数名称: OnBnClickedReleaseres
// 函数说明: 释放资源按钮
// 作	 者: GuiShou
// 时	 间: 2018/12/18
// 参	 数: void
// 返 回 值: void
//************************************************************
void CBase::OnBnClickedReleaseres()
{
	//获取指定模块的资源
	HRSRC hRsrc1 = FindResource(NULL, MAKEINTRESOURCE(IDR_GUISHOU1), TEXT("GUISHOU"));
	HRSRC hRsrc2 = FindResource(NULL, MAKEINTRESOURCE(IDR_GUISHOU2), TEXT("GUISHOU"));

	if (hRsrc1==NULL|| hRsrc2==NULL)
	{
		m_Edit += L"查找资源失败\r\n";
	}
	else
	{
		m_Edit += L"查找资源成功\r\n";
	}
	
	//获取资源大小
	DWORD dwSize1 = SizeofResource(NULL, hRsrc1);
	DWORD dwSize2 = SizeofResource(NULL, hRsrc2);
	if (dwSize1&&dwSize2)
	{
		m_Edit += L"获取资源大小成功\r\n";
	}
	else
	{
		m_Edit += L"获取资源大小失败\r\n";
	}

	//将资源加载到内存
	HGLOBAL hGlobal1 = LoadResource(NULL, hRsrc1);
	HGLOBAL hGlobal2 = LoadResource(NULL, hRsrc2);
	if (hGlobal1&&hGlobal1)
	{
		m_Edit += L"资源加载成功\r\n";
	}
	else
	{
		m_Edit += L"资源加载失败\r\n";
	}

	//锁定资源
	LPVOID lpVoid1 = LockResource(hGlobal1);
	LPVOID lpVoid2 = LockResource(hGlobal2);
	if (lpVoid1&&lpVoid2)
	{
		m_Edit += L"资源锁定成功\r\n";
	}
	else
	{
		m_Edit += L"资源锁定失败\r\n";
	}

	//将资源保存为文件
	FILE* fp1 = NULL;
	FILE* fp2 = NULL;
	fopen_s(&fp1, "MyHookDll.dll", "wb+");
	fopen_s(&fp2, "TestDll.dll", "wb+");
	if (fp1&&fp2)
	{
		m_Edit += L"资源载入本地成功\r\n";
	}
	fwrite(lpVoid1,sizeof(char),dwSize1,fp1);
	fwrite(lpVoid2,sizeof(char),dwSize2,fp2);
	fclose(fp1);
	fclose(fp2);
	UpdateData(FALSE);
}
