#include "stdafx.h"
#include "HackerTools.h"
#include "CInject.h"
#include "afxdialogex.h"
#include <afxpriv.h>
#include <TlHelp32.h>


// CInject 对话框

IMPLEMENT_DYNAMIC(CInject, CDialogEx)

CInject::CInject(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_Inject, pParent)
	, m_Radio(0)
	, m_Edit(_T("请拖入MyHookDll.dll"))
	, m_TipMsg(_T(""))
	, m_PidAndName(_T(""))
{

}

CInject::~CInject()
{
}

void CInject::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Radio(pDX, IDC_RADIO1, m_Radio);
	DDX_Text(pDX, IDC_EDIT1, m_Edit);
	DDX_Text(pDX, IDC_EDIT3, m_TipMsg);
	DDX_Text(pDX, IDC_EDIT2, m_PidAndName);
	DDX_Control(pDX, IDC_EDIT3, m_objEdit);
}


BEGIN_MESSAGE_MAP(CInject, CDialogEx)
	ON_BN_CLICKED(IDC_RADIO1, &CInject::OnBnClickedRadio1)
	ON_BN_CLICKED(IDC_RADIO2, &CInject::OnBnClickedRadio1)
	ON_BN_CLICKED(IDC_RADIO3, &CInject::OnBnClickedRadio1)
	ON_WM_DROPFILES()
	ON_BN_CLICKED(IDC_Inject, &CInject::OnBnClickedInject)
END_MESSAGE_MAP()




BOOL CInject::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	GetDlgItem(IDC_EDIT2)->ShowWindow(FALSE);
	GetDlgItem(IDC_STATIC1)->ShowWindow(FALSE);

	//管理员模式下取消过滤拖拽消息
	ChangeWindowMessageFilterEx(m_hWnd, WM_DROPFILES, MSGFLT_ALLOW, NULL);
	ChangeWindowMessageFilterEx(m_hWnd, 0x0049, MSGFLT_ALLOW, NULL);

	return TRUE; 
}


//************************************************************
// 函数名称: OnBnClickedRadio1
// 函数说明: 响应单选框按钮
// 作	 者: GuiShou
// 时	 间: 2018/12/18
// 参	 数: void
// 返 回 值: void
//************************************************************
void CInject::OnBnClickedRadio1()
{
	UpdateData(TRUE);
	switch (m_Radio)
	{
	case 0:
	{
		m_Edit = L"请拖入MyHookDll.dll";
		GetDlgItem(IDC_EDIT2)->ShowWindow(FALSE);
		GetDlgItem(IDC_STATIC1)->ShowWindow(FALSE);
		m_TipMsg = "";
	}
		break;
	case 1:
	{
		m_Edit = L"请拖入TestDll.dll";
		m_PidAndName = L"请输入需要注入的进程ID";
		GetDlgItem(IDC_EDIT2)->ShowWindow(TRUE);
		GetDlgItem(IDC_STATIC1)->ShowWindow(TRUE);
		SetDlgItemText(IDC_STATIC1, L"请输入进程ID");
		m_TipMsg = "";
	}	
		break;
	case 2:
	{
		m_Edit = L"请拖入TestDll.dll";
		//m_Edit = L"C:\\Users\\GuiShou\\Desktop\\TestDll.dll";
		m_PidAndName = L"请输入需要注入的进程名";
		//m_PidAndName = L"QQ.exe";
		GetDlgItem(IDC_EDIT2)->ShowWindow(TRUE);
		GetDlgItem(IDC_STATIC1)->ShowWindow(TRUE);
		SetDlgItemText(IDC_STATIC1, L"请输入进程名");
		m_TipMsg = "";
	}
	break;
	default:
		break;
	}
	UpdateData(FALSE);
}



//************************************************************
// 函数名称: OnDropFiles 
// 函数说明: 响应拖拽文件
// 作	 者: GuiShou
// 时	 间: 2018/12/18
// 参	 数: HDROP hDropInfo 拖拽文件句柄
// 返 回 值: void
//************************************************************
void CInject::OnDropFiles(HDROP hDropInfo)
{
	//获取文件路径
	TCHAR szPath[MAX_PATH] = { 0 };
	DragQueryFile(hDropInfo, 0, szPath, MAX_PATH);

	//过滤后缀名是否为DLL
	LPTSTR pszExtension=PathFindExtension(szPath);
	if (lstrcmp(pszExtension, L".dll")==0)
	{
		//显示到控件上
		m_Edit = szPath;
		UpdateData(FALSE);
	}
	else
	{
		MessageBox(L"请拖入有效的DLL文件");
	}
	
	CDialogEx::OnDropFiles(hDropInfo);
}



//************************************************************
// 函数名称: OnBnClickedInject 
// 函数说明: 响应开始注入按钮
// 作	 者: GuiShou
// 时	 间: 2018/12/18
// 参	 数: HDROP hDropInfo 拖拽文件句柄
// 返 回 值: void
//************************************************************
void CInject::OnBnClickedInject()
{
	//检测文件是否存在
	if (GetFileAttributes(m_Edit) == INVALID_FILE_ATTRIBUTES)
	{
		MessageBox(L"文件不存在 请重试");
		return;
	}
	UpdateData(TRUE);
	switch (m_Radio)
	{

	case 0:
	{
		if (m_Edit == L"" || m_Edit == L"请拖入MyHookDll.dll")
		{
			MessageBox(L"请拖入有效的DLL文件");
		}
		else
		{
			//全局钩子注入
			SetGlobalHook();
		}
	}
	break;
	case 1:
	{
		if (m_Edit == L"" || m_Edit == L"请拖入TestDll.dll")
		{
			MessageBox(L"请拖入有效的DLL文件");
		}
		else if (m_PidAndName == L"请输入需要注入的进程ID"|| m_PidAndName=="")
		{
			MessageBox(L"请输入需要注入的进程ID");
		}
		else
		{
			USES_CONVERSION;
			char* szDllPath = T2A(m_Edit);
			DWORD dwProcessId = _ttoi(m_PidAndName);

			// 提升当前进程令牌权限
			BOOL bRet = EnableDebugPrivilege(TRUE);
			if (bRet)
			{
				m_TipMsg += L"提权成功\r\n";
			}
			else
			{
				m_TipMsg += L"提权失败\r\n";
			}

			//远程线程注入
			BOOL bIsSuccess = ZwCreateThreadExInjectDll(dwProcessId, szDllPath);
			if (bIsSuccess)
			{
				m_TipMsg += L"远程线程注入成功\r\n";
			}
			else
			{
				m_TipMsg += L"远程线程注入失败\r\n";
			}
		}
		
	}
	break;
	case 2:
	{
		//判断DLL路径是否为空
		if (m_Edit == L"" || m_Edit == L"请拖入TestDll.dll")
		{
			MessageBox(L"请拖入有效的DLL文件");
		}
		//判断进程名是否为空
		else if (m_PidAndName== L"请输入需要注入的进程名"|| m_PidAndName=="")
		{
			MessageBox(L"请输入有效的进程名");
		}
		else
		{
			//提升权限
			EnableDebugPrivilege(TRUE);

			USES_CONVERSION;
			char* szProcessName = T2A(m_PidAndName);
			char* szDllPath = T2A(m_Edit);
			//APC注入
			BOOL bRet = FALSE;
			bRet = APCInject(szProcessName, szDllPath);
			if (bRet)
			{
				m_TipMsg += L"APC注入成功\r\n";
				//设置滚动条滚动到最后一行
				m_objEdit.LineScroll(m_objEdit.GetLineCount());
			}
			else
			{
				m_TipMsg += L"APC注入失败\r\n";
				//设置滚动条滚动到最后一行
				m_objEdit.LineScroll(m_objEdit.GetLineCount());
			}
		}
	}
	break;
	default:
		break;
	}
	UpdateData(FALSE);
}




//************************************************************
// 函数名称: SetGlobalHook 
// 函数说明: 设置全局钩子
// 作	 者: GuiShou
// 时	 间: 2018/12/18
// 参	 数: void
// 返 回 值: void
//************************************************************
void CInject::SetGlobalHook()
{
	//定义函数指针和函数指针变量
	typedef BOOL(*typedef_SetGlobalHook)();
	typedef_SetGlobalHook fnSetGlobalHook = NULL;


	//获取DLL加载基址
	HMODULE hDll = LoadLibrary(m_Edit);
	if (hDll)
	{
		m_TipMsg += L"DLL加载成功\r\n";
	}
	else
	{
		m_TipMsg += L"DLL加载失败\r\n";
	}

	//获取函数地址 给函数指针变量赋值
	fnSetGlobalHook = (typedef_SetGlobalHook)GetProcAddress(hDll, "SetGlobalHook");
	if (fnSetGlobalHook)
	{
		m_TipMsg += L"加载函数地址成功\r\n";
	}
	else
	{
		m_TipMsg += L"加载函数地址失败\r\n";
	}
	//设置全局钩子
	BOOL bRet = fnSetGlobalHook();
	if (bRet)
	{
		m_TipMsg += L"设置全局钩子成功 开始无限弹框\r\n";
	}
	else
	{
		m_TipMsg += L"设置全局钩子失败\r\n";
	}

	UpdateData(FALSE);

}




//************************************************************
// 函数名称: ZwCreateThreadExInjectDll 
// 函数说明: 使用ZwCreateThreadEx 实现远线程注入
// 作	 者: GuiShou
// 时	 间: 2018/12/19
// 参	 数: dwProcessId 线程ID  pszDllFileName dll路径名
// 返 回 值: BOOL 是否成功
//************************************************************
BOOL CInject::ZwCreateThreadExInjectDll(DWORD dwProcessId, char* pszDllFileName)
{
	HANDLE hProcess = NULL;
	SIZE_T dwSize = 0;
	LPVOID pDllAddr = NULL;
	FARPROC pFunProcAddr = NULL;
	HANDLE hRemoteThread = NULL;
	DWORD dwStatus = 0;

	//打开目标进程 获取句柄
	hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, dwProcessId);
	if (hProcess==NULL)
	{
		m_TipMsg += L"打开进程失败\r\n";
		return FALSE;
	}
	else
	{
		m_TipMsg += L"打开进程成功\r\n";
	}

	//在注入的进程中申请内存
	dwSize = strlen(pszDllFileName) + 1;
	pDllAddr = VirtualAllocEx(hProcess, NULL, dwSize, MEM_COMMIT, PAGE_READWRITE);
	if (pDllAddr==NULL)
	{
		m_TipMsg += L"申请内存失败\r\n";
		return FALSE;
	}
	else
	{
		m_TipMsg += L"申请内存成功\r\n";
	}

	//向申请的内存中写入数据
	BOOL bIsSucess = WriteProcessMemory(hProcess, pDllAddr, pszDllFileName, dwSize, NULL);
	if (bIsSucess==FALSE)
	{
		m_TipMsg += L"写入内存失败\r\n";
		return FALSE;
	}
	else
	{
		m_TipMsg += L"写入内存成功\r\n";
	}

	//加载ntdll.dll
	HMODULE hNtdll = LoadLibraryA("ntdll.dll");
	if (hNtdll==NULL)
	{
		m_TipMsg += L"加载ntdll失败\r\n";
		return FALSE;
	}
	else
	{
		m_TipMsg += L"加载ntdll成功\r\n";
	}

	//获取LoadLibraryA函数地址
	pFunProcAddr = GetProcAddress(GetModuleHandleA("Kernel32.dll"), "LoadLibraryA");
	if (pFunProcAddr==NULL)
	{
		m_TipMsg += L"加载LoadLibraryA函数地址失败\r\n";
		return FALSE;
	}
	else
	{
		m_TipMsg += L"加载LoadLibraryA函数地址成功\r\n";
	}

	//获取ZwCreateThread函数地址  ZwCreateThread在64位和32位下的函数声明不一样
#ifdef _WIN64
	typedef DWORD(WINAPI *typedef_ZwCreateThreadEx)(
		PHANDLE ThreadHandle,
		ACCESS_MASK DesiredAccess,
		LPVOID ObjectAttributes,
		HANDLE ProcessHandle,
		LPTHREAD_START_ROUTINE lpStartAddress,
		LPVOID lpParameter,
		ULONG CreateThreadFlags,
		SIZE_T ZeroBits,
		SIZE_T StackSize,
		SIZE_T MaximumStackSize,
		LPVOID pUnkown);
#else
	typedef DWORD(WINAPI *typedef_ZwCreateThreadEx)(
		PHANDLE ThreadHandle,
		ACCESS_MASK DesiredAccess,
		LPVOID ObjectAttributes,
		HANDLE ProcessHandle,
		LPTHREAD_START_ROUTINE lpStartAddress,
		LPVOID lpParameter,
		BOOL CreateSuspended,
		DWORD dwStackSize,
		DWORD dw1,
		DWORD dw2,
		LPVOID pUnkown);
#endif
	typedef_ZwCreateThreadEx ZwCreateThreadEx = (typedef_ZwCreateThreadEx)GetProcAddress(hNtdll, "ZwCreateThreadEx");
	if (ZwCreateThreadEx==NULL)
	{
		m_TipMsg += L"加载ZwCreateThreadEx函数地址失败\r\n";
		return FALSE;
	}
	else
	{
		m_TipMsg += L"加载ZwCreateThreadEx函数地址成功\r\n";
	}
	//使用ZwCreateThreadEx函数创建远程线程 实现DLL注入
	dwStatus = ZwCreateThreadEx(&hRemoteThread, THREAD_ALL_ACCESS, NULL,
	hProcess, (LPTHREAD_START_ROUTINE)pFunProcAddr, pDllAddr, 0, 0, 0, 0, NULL);
	if (hRemoteThread==NULL)
	{
		m_TipMsg += L"远程线程注入失败\r\n";
		return FALSE;
	}
	else
	{
		m_TipMsg += L"远程线程注入成功\r\n";
	}

	//关闭句柄
	CloseHandle(hProcess);
	FreeLibrary(hNtdll);
	return TRUE;

}


//************************************************************
// 函数名称: EnableDebugPrivilege 
// 函数说明: 提升为调试权限
// 作	 者: GuiShou
// 时	 间: 2018/12/19
// 参	 数: fEnable是否提权
// 返 回 值: BOOL 是否成功
//************************************************************
BOOL CInject::EnableDebugPrivilege(BOOL fEnable)
{
	BOOL fOk = FALSE;	HANDLE hToken;
	//以修改权限的方式 打开进程的令牌
	if (OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES, &hToken))
	{
		//令牌权限结构体
		TOKEN_PRIVILEGES tp;
		tp.PrivilegeCount = 1;
		//获得LUID
		LookupPrivilegeValue(NULL, SE_DEBUG_NAME, &tp.Privileges[0].Luid);
		tp.Privileges[0].Attributes = fEnable ? SE_PRIVILEGE_ENABLED : 0;
		//修改权限
		AdjustTokenPrivileges(hToken, FALSE, &tp, sizeof(tp), NULL, NULL);
		fOk = (GetLastError() == ERROR_SUCCESS);
		CloseHandle(hToken);
	}
	return(fOk);
}



//************************************************************
// 函数名称: APCInject 
// 函数说明: 使用APC注入DLL到目标进程
// 作	 者: GuiShou
// 时	 间: 2018/12/19
// 参	 数: pszProcessName 进程名 pszDllName DLL路径名
// 返 回 值: BOOL 是否成功
//************************************************************
BOOL CInject::APCInject(char *pszProcessName, char *pszDllName)
{
	BOOL bRet = FALSE;
	DWORD dwProcessId = 0;
	DWORD *pThreadId = NULL;
	DWORD dwThreadIdLength = 0;
	HANDLE hProcess = NULL;
	HANDLE hThread = NULL;
	PVOID pBaseAddress = NULL;
	PVOID pLoadLibraryAFunc = NULL;
	SIZE_T dwRet = 0;
	DWORD dwDllPathLen = strlen(pszDllName) + 1;
	DWORD i = 0;

	do 
	{
		//根据进程名称获取PID
		dwProcessId = GetPidByPname(pszProcessName);
		if (0>=dwProcessId)
		{
			m_TipMsg += L"获取进程ID失败！\r\n";
			bRet = FALSE;
			break;
		}
		else
		{
			m_TipMsg += L"获取进程ID成功！\r\n";
		}
		//根据PID
		bRet = GetAllTidByPid(dwProcessId, &pThreadId, &dwThreadIdLength);
		if (bRet==FALSE)
		{
			m_TipMsg += L"获取所有线程ID失败\r\n";
			bRet = FALSE;
			break;
		}
		else
		{
			m_TipMsg += L"获取所有线程ID成功\r\n";
		}

		//打开注入进程
		hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, dwProcessId);
		if (hProcess==NULL)
		{
			m_TipMsg += L"打开注入进程失败\r\n";
			bRet = FALSE;
			break;
		}
		else
		{
			m_TipMsg += L"打开注入进程成功\r\n";
		}
		//在注入的进程空间申请内存
		pBaseAddress = VirtualAllocEx(hProcess, NULL, dwDllPathLen, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);
		if (pBaseAddress == NULL)
		{
			m_TipMsg += L"在注入进程申请内存失败\r\n";
			bRet = FALSE;
			break;
		}
		else
		{
			m_TipMsg += L"在注入进程申请内存成功\r\n";
		}
		//向申请的空间中写入DLL路径数据
		WriteProcessMemory(hProcess, pBaseAddress, pszDllName, dwDllPathLen, &dwRet);
		if (dwRet!=dwDllPathLen)
		{
			m_TipMsg += L"在注入进程写入内存失败\r\n";
			bRet = FALSE;
			break;
		}
		else
		{
			m_TipMsg += L"在注入进程写入内存成功\r\n";
		}

		//获取LoadLibrary的地址
		pLoadLibraryAFunc = GetProcAddress(GetModuleHandleA("kernel32.dll"), "LoadLibraryA");
		if (pLoadLibraryAFunc==NULL)
		{
			m_TipMsg += L"获取LoadLibrary地址失败\r\n";
			bRet = FALSE;
			break;
		}
		else
		{
			m_TipMsg += L"获取LoadLibrary地址成功\r\n";
		}

		//遍历线程 插入APC
		for (i = 0; i < dwThreadIdLength; i++)
		{
			//打开线程
			hThread = OpenThread(THREAD_ALL_ACCESS, FALSE, pThreadId[i]);
			if (hThread)
			{
				//插入APC
				QueueUserAPC((PAPCFUNC)pLoadLibraryAFunc, hThread, (ULONG_PTR)pBaseAddress);
				//关闭线程句柄
				CloseHandle(hThread);
				hThread = NULL;
			}
		}
		bRet = TRUE;
	} while (FALSE);

	//释放内存
	if (hProcess)
	{
		CloseHandle(hProcess);
		hProcess = NULL;
	}
	if (hThread)
	{
		delete[] pThreadId;
		pThreadId = NULL;
	}
	return bRet;
}



//************************************************************
// 函数名称: GetAllTidByPid 
// 函数说明: 根据进程ID获取所有线程ID
// 作	 者: GuiShou
// 时	 间: 2018/12/19
// 参	 数: dwProcessId 进程ID  ppThreadId 线程ID  pdwThreadIdLength 线程ID个数
// 返 回 值: BOOL 是否成功
//************************************************************
BOOL CInject::GetAllTidByPid(DWORD dwProcessId, DWORD** ppThreadId, DWORD* pdwThreadIdLength)
{
	DWORD* pThreadId = NULL;
	DWORD dwThreadIdLength = 0;
	DWORD dwBuffLength = 1000;
	THREADENTRY32 te32 = { 0 };
	HANDLE hSnapShot = NULL;
	BOOL bRet = TRUE;

	do 
	{
		//申请内存
		pThreadId = new DWORD[dwBuffLength];
		if (pThreadId==NULL)
		{
			m_TipMsg += L"申请内存失败!\r\n";
			bRet = FALSE;
			break;
		}
		else
		{
			m_TipMsg += L"申请内存成功!\r\n";
		}
		RtlZeroMemory(pThreadId, (dwBuffLength * sizeof(DWORD)));

		//获取线程快照
		RtlZeroMemory(&te32, sizeof(te32));
		te32.dwSize = sizeof(te32);
		hSnapShot = CreateToolhelp32Snapshot(TH32CS_SNAPTHREAD, 0);
		if (hSnapShot == NULL)
		{
			m_TipMsg += L"创建线程快照失败!\r\n";
			bRet = FALSE;
			break;
		}
		else
		{
			m_TipMsg += L"创建线程快照成功!\r\n";
		}

		//获取第一条快照的信息
		bRet = Thread32First(hSnapShot, &te32);
		while (bRet)
		{
			//获取进程对应的线程ID
			if (te32.th32OwnerProcessID==dwProcessId)
			{
				pThreadId[dwThreadIdLength] = te32.th32ThreadID;
				dwThreadIdLength++;
			}
			//遍历下一个线程快照信息
			bRet = Thread32Next(hSnapShot, &te32);
		}
		//返回
		*ppThreadId = pThreadId;
		*pdwThreadIdLength = dwThreadIdLength;
		bRet = TRUE;

	} while (FALSE);

	if (FALSE==bRet)
	{
		if (pThreadId)
		{
			delete[] pThreadId;
			pThreadId = NULL;
		}
	}
	return bRet;
}


//************************************************************
// 函数名称: GetPidByPname 
// 函数说明: 根据进程名获取进程ID
// 作	 者: GuiShou
// 时	 间: 2018/12/19
// 参	 数: pszProcessName 进程名 
// 返 回 值: DWORD 进程ID
//************************************************************
DWORD CInject::GetPidByPname(char* pszProcessName)
{
	DWORD dwProcessId = 0;
	PROCESSENTRY32 pe32 = { 0 };
	HANDLE hSnapshot = NULL;
	BOOL bRet = FALSE;
	RtlZeroMemory(&pe32, sizeof(pe32));
	pe32.dwSize = sizeof(pe32);

	//获取进程快照
	hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if (hSnapshot == NULL)
	{
		m_TipMsg += L"创建进程快照失败\r\n";
		return dwProcessId;
	}
	else
	{
		m_TipMsg += L"创建进程快照成功\r\n";	
	}

	//获取第一条进程快照的信息
	bRet = Process32First(hSnapshot, &pe32);
	while (bRet)
	{
		//获取快照信息
		USES_CONVERSION;
		CString ProcessName = A2T(pszProcessName);
		if (0==lstrcmp(pe32.szExeFile, ProcessName))
		{
			dwProcessId = pe32.th32ProcessID;
			break;
		}
		//遍历下一个进程快照信息
		bRet = Process32Next(hSnapshot, &pe32);
	}
	return dwProcessId;
}
