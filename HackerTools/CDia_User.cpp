#include "stdafx.h"
#include "HackerTools.h"
#include "CDia_User.h"
#include "afxdialogex.h"
#include "CBase.h"
#include "CInject.h"
#include "CStrat.h"
#include "CSelfStart.h"
#include "CPrivilege.h"
#include "CHide.h"
#include "CCompress.h"
#include "CEncrypt.h"
#include "CFunction.h"


// CDia_User 对话框

IMPLEMENT_DYNAMIC(CDia_User, CDialogEx)

CDia_User::CDia_User(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_USER, pParent)
{

}

CDia_User::~CDia_User()
{
}

void CDia_User::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDia_User, CDialogEx)
	ON_BN_CLICKED(IDC_Base, &CDia_User::OnBnClickedBase)
	ON_BN_CLICKED(IDC_Inject, &CDia_User::OnBnClickedInject)
	ON_BN_CLICKED(IDC_Start, &CDia_User::OnBnClickedStart)
	ON_BN_CLICKED(IDC_SelfStart, &CDia_User::OnBnClickedSelfstart)
	ON_BN_CLICKED(IDC_Privilege, &CDia_User::OnBnClickedPrivilege)
	ON_BN_CLICKED(IDC_Hide, &CDia_User::OnBnClickedHide)
	ON_BN_CLICKED(IDC_Compress, &CDia_User::OnBnClickedCompress)
	ON_BN_CLICKED(IDC_Encrypt, &CDia_User::OnBnClickedEncrypt)
	ON_BN_CLICKED(IDC_Other, &CDia_User::OnBnClickedOther)
END_MESSAGE_MAP()




//************************************************************
// 函数名称: OnBnClickedBase
// 函数说明: 基础技术按钮
// 作	 者: GuiShou
// 时	 间: 2018/12/18
// 参	 数: void
// 返 回 值: void
//************************************************************
void CDia_User::OnBnClickedBase()
{
	CBase *m_Base = new CBase;
	m_Base->Create(IDD_Base, NULL);
	m_Base->ShowWindow(SW_SHOW);
}



//************************************************************
// 函数名称: OnBnClickedInject
// 函数说明: 注入技术按钮
// 作	 者: GuiShou
// 时	 间: 2018/12/18
// 参	 数: void
// 返 回 值: void
//************************************************************
void CDia_User::OnBnClickedInject()
{
	CInject *m_Inject = new CInject;
	m_Inject->Create(IDD_Inject, NULL);
	m_Inject->ShowWindow(SW_SHOW);
}



//************************************************************
// 函数名称: OnBnClickedStart
// 函数说明: 启动技术按钮
// 作	 者: GuiShou
// 时	 间: 2019/1/19
// 参	 数: void
// 返 回 值: void
//************************************************************
void CDia_User::OnBnClickedStart()
{
	CStrat* m_Strat = new CStrat;
	m_Strat->Create(IDD_Start, NULL);
	m_Strat->ShowWindow(SW_SHOW);
}



//************************************************************
// 函数名称: OnBnClickedSelfstart
// 函数说明: 自启动技术按钮
// 作	 者: GuiShou
// 时	 间: 2019/1/19
// 参	 数: void
// 返 回 值: void
//************************************************************
void CDia_User::OnBnClickedSelfstart()
{
	CSelfStart* m_SelfStrat = new CSelfStart;
	m_SelfStrat->Create(IDD_SelfStart, NULL);
	m_SelfStrat->ShowWindow(SW_SHOW);
}



//************************************************************
// 函数名称: OnBnClickedPrivilege
// 函数说明: 提权技术按钮
// 作	 者: GuiShou
// 时	 间: 2019/1/23
// 参	 数: void
// 返 回 值: void
//************************************************************
void CDia_User::OnBnClickedPrivilege()
{
	CPrivilege* m_Privilege = new CPrivilege;
	m_Privilege->Create(IDD_Privilege, NULL);
	m_Privilege->ShowWindow(SW_SHOW);
}


//************************************************************
// 函数名称: OnBnClickedHide
// 函数说明: 隐藏技术按钮
// 作	 者: GuiShou
// 时	 间: 2019/1/23
// 参	 数: void
// 返 回 值: void
//************************************************************
void CDia_User::OnBnClickedHide()
{
	CHide* m_Hide = new CHide;
	m_Hide->Create(IDD_Hide, NULL);
	m_Hide->ShowWindow(SW_SHOW);
}



//************************************************************
// 函数名称: OnBnClickedCompress
// 函数说明: 压缩技术按钮
// 作	 者: GuiShou
// 时	 间: 2019/1/24
// 参	 数: void
// 返 回 值: void
//************************************************************
void CDia_User::OnBnClickedCompress()
{
	CCompress* m_Compress = new CCompress;
	m_Compress->Create(IDD_Compress, NULL);
	m_Compress->ShowWindow(SW_SHOW);
}


//************************************************************
// 函数名称: OnBnClickedEncrypt
// 函数说明: 加密技术按钮
// 作	 者: GuiShou
// 时	 间: 2019/1/24
// 参	 数: void
// 返 回 值: void
//************************************************************
void CDia_User::OnBnClickedEncrypt()
{
	CEncrypt* m_Encrypt = new CEncrypt;
	m_Encrypt->Create(IDD_Encrypt, NULL);
	m_Encrypt->ShowWindow(SW_SHOW);
}


//************************************************************
// 函数名称: OnBnClickedOther
// 函数说明: 功能技术按钮
// 作	 者: GuiShou
// 时	 间: 2019/1/29
// 参	 数: void
// 返 回 值: void
//************************************************************
void CDia_User::OnBnClickedOther()
{
	CFunction* m_Function = new CFunction;
	m_Function->Create(IDD_Function, NULL);
	m_Function->ShowWindow(SW_SHOW);
}
