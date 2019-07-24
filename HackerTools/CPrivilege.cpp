// CPrivilege.cpp: 实现文件
//

#include "stdafx.h"
#include "HackerTools.h"
#include "CPrivilege.h"
#include "afxdialogex.h"


// CPrivilege 对话框

IMPLEMENT_DYNAMIC(CPrivilege, CDialogEx)

CPrivilege::CPrivilege(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_Privilege, pParent)
	, m_Path(_T("请拖拽TestDll.dll到此处"))
{

}

CPrivilege::~CPrivilege()
{
}

void CPrivilege::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_AllPrivilege, m_List);
	DDX_Text(pDX, IDC_EDIT1, m_Path);
}


BEGIN_MESSAGE_MAP(CPrivilege, CDialogEx)

	ON_BN_CLICKED(IDC_Debug, &CPrivilege::OnBnClickedDebug)
	ON_BN_CLICKED(IDC_PassUAC, &CPrivilege::OnBnClickedPassuac)
	ON_BN_CLICKED(IDC_CheckPrivilege, &CPrivilege::OnBnClickedCheckprivilege)
	ON_WM_DROPFILES()
END_MESSAGE_MAP()




BOOL CPrivilege::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	//管理员模式下取消过滤拖拽消息
	ChangeWindowMessageFilterEx(m_hWnd, WM_DROPFILES, MSGFLT_ALLOW, NULL);
	ChangeWindowMessageFilterEx(m_hWnd, 0x0049, MSGFLT_ALLOW, NULL);

	//初始化ListCtrl
	m_List.InsertColumn(0, L"权限", 0, 250);
	m_List.InsertColumn(1, L"状态", 0, 120);
	
	//设置扩展属性
	m_List.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);

	return TRUE;  
}



//************************************************************
// 函数名称: OnBnClickedDebug
// 函数说明: 提升为Debug权限按钮
// 作	 者: GuiShou
// 时	 间: 2019/1/23
// 参	 数: void
// 返 回 值: void
//************************************************************
void CPrivilege::OnBnClickedDebug()
{

	if (EnablePrivileges(SE_DEBUG_NAME)==FALSE)
	{
		MessageBox(L"提升为Debug权限失败 请尝试管理员运行");
	}
	else
	{
		MessageBox(L"提升为Debug权限成功");
	}

	UpdateData(FALSE);
}


//************************************************************
// 函数名称: OnBnClickedPassuac
// 函数说明: BypassUAC按钮
// 作	 者: GuiShou
// 时	 间: 2019/1/23
// 参	 数: void
// 返 回 值: void
//************************************************************
void CPrivilege::OnBnClickedPassuac()
{
	//检测文件是否存在
	if (GetFileAttributes(m_Path)==INVALID_FILE_ATTRIBUTES)
	{
		MessageBox(L"文件不存在，请重试");
		return;
	}

	//调用rundll32.exe去执行TestDll.dll的导出函数 达到BypassUAC
	char szCmdLine[MAX_PATH] = { 0 };
	char szRundll32Path[MAX_PATH] = "C:\\Windows\\System32\\rundll32.exe";

	
	USES_CONVERSION;
	::sprintf_s(szCmdLine, "%s \"%s\" %s", szRundll32Path, W2A(m_Path), "BypassUAC");
	::WinExec(szCmdLine, SW_HIDE);
}



//************************************************************
// 函数名称: EnablePrivileges
// 函数说明: 提升当前令牌权限
// 作	 者: GuiShou
// 时	 间: 2019/1/23
// 参	 数: const wchar_t * pszPrivilegesName 需要提升的权限名
// 返 回 值: void
//************************************************************
BOOL CPrivilege::EnablePrivileges(const wchar_t* pszPrivilegesName)
{
	HANDLE hToken = NULL;
	LUID luidValue = { 0 };
	TOKEN_PRIVILEGES tokenPrivileges = { 0 };
	BOOL bRet = FALSE;
	DWORD dwRet = 0;

	//打开进程令牌并获取具有TOKEN_ADJUST_PRIVILEGES权限的进程令牌句柄
	bRet = OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES, &hToken);
	if (bRet == FALSE)
	{
		MessageBox(L"OpenProcessToken Error");
		return FALSE;
	}

	//获取需要提升的特权LUID的值
	bRet = LookupPrivilegeValue(NULL, pszPrivilegesName, &luidValue);
	if (bRet == FALSE)
	{
		MessageBox(L"LookupPrivilegeValueA Error");
		return FALSE;
	}

	//设置提升权限的信息
	//设置新特权的数量
	tokenPrivileges.PrivilegeCount = 1;
	//第一个特权对应的LUID的值
	tokenPrivileges.Privileges[0].Luid = luidValue;
	//启用该特权
	tokenPrivileges.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;
	//提升进程令牌访问权限
	bRet = AdjustTokenPrivileges(hToken, FALSE, &tokenPrivileges, 0, NULL, NULL);
	if (bRet == FALSE)
	{
		MessageBox(L"AdjustTokenPrivileges Error");
		return FALSE;
	}
	else
	{
		//根据错误码判断特权是否设置成功
		dwRet = GetLastError();
		if (dwRet == ERROR_SUCCESS)
		{
			return TRUE;
		}
		else
		{
			return FALSE;
		}
	}
	return FALSE;
}




//************************************************************
// 函数名称: OnBnClickedCheckprivilege
// 函数说明: 查看当前进程权限按钮
// 作	 者: GuiShou
// 时	 间: 2019/1/23
// 参	 数: void
// 返 回 值: void
//************************************************************
void CPrivilege::OnBnClickedCheckprivilege()
{

	if (FindAllPrivileges())
	{
		MessageBox(L"查看当前进程权限成功");
	}
	else
	{
		MessageBox(L"查看当前进程权限失败");
	}
}


//************************************************************
// 函数名称: FindAllPrivileges
// 函数说明: 遍历当前进程权限
// 作	 者: GuiShou
// 时	 间: 2019/1/23
// 参	 数: void
// 返 回 值: void
//************************************************************
BOOL CPrivilege::FindAllPrivileges()
{
	//遍历权限 也就是遍历令牌
	HANDLE hToken;
	//打开进程令牌获取句柄
	BOOL bRet = OpenProcessToken(
		GetCurrentProcess(),	//进程句柄
		TOKEN_QUERY,			//访问权限 查询
		&hToken					//返回的令牌句柄
	);
	if (!bRet)
	{
		MessageBox(L"令牌打开失败");
		return FALSE;
	}
	//开始查询令牌中的信息 
	//GetTokenInformation需要调用两次
	//第一次获取要查询信息的大小
	//拿到大小以后根据大小去遍历获取真正的信息
	DWORD dwLength;
	GetTokenInformation(
		hToken,			//令牌句柄
		TokenPrivileges,//查询选项（权限）
		NULL,			//存放返回信息的缓冲区
		NULL,			//缓冲区大小
		&dwLength		//返回长度 以字节为单位
	);
	//根据返回的长度申请空间后再次调用 获取相关信息
	char* pBuf = new char[dwLength] {0};
	GetTokenInformation(
		hToken,
		TokenPrivileges,
		pBuf,
		dwLength,
		&dwLength
	);
	//获取权限列表 
	TOKEN_PRIVILEGES* pTokenPrivileges = (TOKEN_PRIVILEGES*)pBuf;
	//权限列表首地址
	LUID_AND_ATTRIBUTES* pPrivileges = pTokenPrivileges->Privileges;
	//权限个数
	DWORD dwPrivilegesCount = pTokenPrivileges->PrivilegeCount;

	//遍历权限并打印
	for (DWORD i = 0; i < dwPrivilegesCount; i++)
	{
		char strName[100] = {};
		DWORD dwSize = sizeof(strName);
		//查看权限的名称字符串
		LookupPrivilegeNameA(
			NULL,				//系统权限名
			&pPrivileges->Luid, //权限对应的Luid
			strName,			//存放权限名称的字符串缓冲区
			&dwSize				//缓冲区大小
		);
		//显示令牌信息
		CString PrivilegesAtrribute;	//令牌属性
		switch (pPrivileges->Attributes)
		{
		case 0:
			PrivilegesAtrribute = L"未启用";
			break;
		case 1:
			PrivilegesAtrribute = L"默认启用";
			break;
		case 2:
			PrivilegesAtrribute = L"启用";
			break;
		case 3:
			PrivilegesAtrribute = L"启用且默认启用";
			break;
		case 4:
			PrivilegesAtrribute = L"删除";
			break;
		case 8:
			PrivilegesAtrribute = L"被用来访问一个对象或服务";
			break;	
		default:
			break;
		}

		USES_CONVERSION;
		m_List.InsertItem(i, A2W(strName));
		m_List.SetItemText(i, 1, PrivilegesAtrribute);

		pPrivileges++;
	}

	return TRUE;
}


void CPrivilege::OnDropFiles(HDROP hDropInfo)
{
	//清空文本框的内容
	m_Path = "";

	//获取文件路径
	TCHAR szPath[MAX_PATH] = { 0 };
	DragQueryFile(hDropInfo, 0, szPath, MAX_PATH);

	//检测是否为exe
	LPTSTR pszExtension = PathFindExtension(szPath);
	if (lstrcmp(pszExtension, L".dll") != 0)
	{
		MessageBox(L"请先拖拽有效的dll文件");
		return;
	}
	//显示到控件上
	m_Path = szPath;
	UpdateData(FALSE);

	CDialogEx::OnDropFiles(hDropInfo);
}
