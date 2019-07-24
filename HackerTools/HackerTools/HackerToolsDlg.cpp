
// HackerToolsDlg.cpp: 实现文件
//

#include "stdafx.h"
#include "HackerTools.h"
#include "HackerToolsDlg.h"
#include "afxdialogex.h"
#include "CDia_User.h"
#include "CDia_Core.h"
#include "CHelp.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CHackerToolsDlg 对话框



CHackerToolsDlg::CHackerToolsDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_HACKERTOOLS_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CHackerToolsDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TAB1, m_MyTable);
}

BEGIN_MESSAGE_MAP(CHackerToolsDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_COMMAND(ID_32771, &CHackerToolsDlg::OnHelp)
	ON_COMMAND(ID_32772, &CHackerToolsDlg::OnIntroductions)
END_MESSAGE_MAP()


// CHackerToolsDlg 消息处理程序

BOOL CHackerToolsDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	//选项卡初始化
	m_MyTable.InsertItem(0, L"用户工具");
	m_MyTable.InsertItem(1, L"内核工具");

	//给子窗口指针赋值
	m_MyTable.m_Dia[0] = new CDia_User();
	m_MyTable.m_Dia[1] = new CDia_Core();

	//创建子窗口
	m_MyTable.m_Dia[0]->Create(IDD_USER, &m_MyTable);
	m_MyTable.m_Dia[1]->Create(IDD_CORE, &m_MyTable);

	//控制两个窗口的大小
	CRect rc;
	m_MyTable.GetClientRect(rc);
	rc.DeflateRect(2, 23, 2, 2);
	m_MyTable.m_Dia[0]->MoveWindow(rc);
	m_MyTable.m_Dia[1]->MoveWindow(rc);

	//显示第一个子窗口
	m_MyTable.m_Dia[0]->ShowWindow(SW_SHOW);
	m_MyTable.m_Dia[1]->ShowWindow(SW_HIDE);
	
	//开启防双开
	HANDLE hMutex = CreateMutexA(NULL, FALSE, "GuiShou");
	if (hMutex)
	{
		if (GetLastError() == ERROR_ALREADY_EXISTS)
		{
			MessageBox(L"请勿双开本程序 即将退出");
			ExitProcess(-1);
		}
	}
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CHackerToolsDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CHackerToolsDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CHackerToolsDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


//************************************************************
// 函数名称: OnHelp
// 函数说明: 菜单帮助按钮
// 作	 者: GuiShou
// 时	 间: 2018/12/18
// 参	 数: void
// 返 回 值: void
//************************************************************
void CHackerToolsDlg::OnHelp()
{
	CHelp* m_Help = new CHelp;
	m_Help->Create(IDD_Help, NULL);
	m_Help->ShowWindow(SW_SHOW); 
}


//************************************************************
// 函数名称: OnHelp
// 函数说明: 菜单说明按钮
// 作	 者: GuiShou
// 时	 间: 2018/12/18
// 参	 数: void
// 返 回 值: void
//************************************************************
void CHackerToolsDlg::OnIntroductions()
{
	MessageBox(L"Create By GuiShou");
}
