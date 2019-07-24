// CStrat.cpp: 实现文件
//

#include "stdafx.h"
#include "HackerTools.h"
#include "CStrat.h"
#include "afxdialogex.h"


//定义DllMain函数指针
typedef BOOL(__stdcall *typedef_DllMain)(HINSTANCE hInstance, DWORD dwReason, LPVOID lpReserved);


// CStrat 对话框
IMPLEMENT_DYNAMIC(CStrat, CDialogEx)

CStrat::CStrat(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_Start, pParent)
	, m_Path(_T(""))
	, m_Tip(_T(""))
{

}

CStrat::~CStrat()
{
}

void CStrat::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_Path);
	DDX_Text(pDX, IDC_EDIT3, m_Tip);
}


BEGIN_MESSAGE_MAP(CStrat, CDialogEx)
	ON_BN_CLICKED(IDC_CreateProcess, &CStrat::OnBnClickedCreateprocess)
	ON_BN_CLICKED(IDC_MemLoadDll, &CStrat::OnBnClickedMemloaddll)
	ON_WM_DROPFILES()
END_MESSAGE_MAP()


// CStrat 消息处理程序


BOOL CStrat::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	//管理员模式下取消过滤拖拽消息
	ChangeWindowMessageFilterEx(m_hWnd, WM_DROPFILES, MSGFLT_ALLOW, NULL);
	ChangeWindowMessageFilterEx(m_hWnd, 0x0049, MSGFLT_ALLOW, NULL);

	return TRUE;  
}


//************************************************************
// 函数名称: OnBnClickedInject
// 函数说明: 三种方式创建进程
// 作	 者: GuiShou
// 时	 间: 2019/1/19
// 参	 数: void
// 返 回 值: void
//************************************************************
void CStrat::OnBnClickedCreateprocess()
{
	//检测文件路径是否为空
	if (m_Path=="")
	{
		MessageBox(L"请先拖拽文件到窗口");
		return;
	}
	//检测后缀是否是exe
	LPTSTR pszExtension = PathFindExtension(m_Path);
	if (lstrcmp(pszExtension, L".exe") != 0)
	{
		MessageBox(L"请先拖拽有效的exe文件");
		return;
	}

	//检测文件是否存在
	if (GetFileAttributes(m_Path)==INVALID_FILE_ATTRIBUTES)
	{
		MessageBox(L"文件不存在 请重试！");
		return;
	}

	//清空提示信息
	m_Tip = " ";


	BOOL bRet = FALSE;
	//以WinExec的方式启动进程
	USES_CONVERSION;
	bRet = WinExecStart(W2A(m_Path), SW_SHOWNORMAL);
	if (bRet)
	{
		m_Tip += "WinExec Run OK\r\n";
	}
	else
	{
		m_Tip += "WinExec Run ERROR\r\n";
	}

	//以ShellExecute的方式启动进程
	bRet = ShellExecuteStart(m_Path, SW_SHOWNORMAL);
	if (bRet)
	{
		m_Tip += "ShellExecute Run OK\r\n";
	}
	else
	{
		m_Tip += "ShellExecute Run ERROR\r\n";
	}

	//以CreateProcess的方式启动进程
	bRet = CreateProcessStart(W2A(m_Path), SW_SHOWNORMAL);
	if (bRet)
	{
		m_Tip += "CreateProcess Run OK\r\n";
	}
	else
	{
		m_Tip += "CreateProcess Run ERROR\r\n";
	}
	UpdateData(FALSE);
}




//************************************************************
// 函数名称: OnBnClickedMemloaddll
// 函数说明: 内存加载DLL
// 作	 者: GuiShou
// 时	 间: 2019/1/19
// 参	 数: void
// 返 回 值: void
//************************************************************
void CStrat::OnBnClickedMemloaddll()
{
	//检测文件路径是否为空
	if (m_Path == "")
	{
		MessageBox(L"请先拖拽文件到窗口");
		return;
	}

	//检测是否为dll
	LPTSTR pszExtension = PathFindExtension(m_Path);
	if (lstrcmp(pszExtension, L".dll") != 0)
	{
		MessageBox(L"请先拖拽有效的dll文件");
		return;
	}

	//检测文件是否存在
	if (GetFileAttributes(m_Path) == INVALID_FILE_ATTRIBUTES)
	{
		MessageBox(L"文件不存在 请重试！");
		return;
	}

	//清空提示信息
	m_Tip = " ";

	//内存加载并运行dll
	if (MmRunDll(m_Path))
	{
		m_Tip += "Dll加载成功\r\n";
	}
	else
	{
		m_Tip += "Dll加载失败\r\n";
	}
	
	UpdateData(FALSE);
}



//************************************************************
// 函数名称: OnDropFiles 
// 函数说明: 响应拖拽文件按钮
// 作	 者: GuiShou
// 时	 间: 2019/1/19
// 参	 数: HDROP hDropInfo 拖拽文件句柄
// 返 回 值: void
//************************************************************
void CStrat::OnDropFiles(HDROP hDropInfo)
{
	//清空文本框的内容
	m_Path = "";

	//获取文件路径
	TCHAR szPath[MAX_PATH] = { 0 };
	DragQueryFile(hDropInfo, 0, szPath, MAX_PATH);

	//显示到控件上
	m_Path = szPath;
	UpdateData(FALSE);

	CDialogEx::OnDropFiles(hDropInfo);
}



//************************************************************
// 函数名称: WinExecStart 
// 函数说明: 以WinExec的方式启动进程
// 作	 者: GuiShou
// 时	 间: 2019/1/20
// 参	 数: char* pszPath,文件路径 UINT icmdshow 显示方式
// 返 回 值: BOOL是否成功
//************************************************************
BOOL CStrat::WinExecStart(char* pszPath, UINT icmdshow)
{
	UINT iRet = 0;
	iRet = WinExec(pszPath, icmdshow);
	if (31 < iRet)
	{
		return TRUE;
	}
	return FALSE;
}



//************************************************************
// 函数名称: ShellExecuteStart 
// 函数说明: 以ShellExecute的方式启动进程
// 作	 者: GuiShou
// 时	 间: 2019/1/20
// 参	 数: char* pszPath,文件路径 UINT icmdshow 显示方式
// 返 回 值: BOOL是否成功
//************************************************************
BOOL CStrat::ShellExecuteStart(CString pszPath, UINT icmdshow)
{
	HINSTANCE hInstance = 0;
	hInstance = ShellExecute(NULL, NULL, pszPath, NULL, NULL, icmdshow);
	if ((DWORD)hInstance > 32)
	{
		return TRUE;
	}
	return FALSE;
}



//************************************************************
// 函数名称: CreateProcessStart 
// 函数说明: 以CreateProcess的方式启动进程
// 作	 者: GuiShou
// 时	 间: 2019/1/20
// 参	 数: char* pszPath,文件路径 UINT icmdshow 显示方式
// 返 回 值: BOOL是否成功
//************************************************************
BOOL CStrat::CreateProcessStart(char* pszPath, UINT icmdshow)
{
	STARTUPINFO si = { 0 };
	PROCESS_INFORMATION pi;
	BOOL bRet = FALSE;
	si.cb = sizeof(si);
	si.dwFlags = STARTF_USESHOWWINDOW;  //指定wShowWindow成员有效
	si.wShowWindow = icmdshow;
	//转换字符集
	USES_CONVERSION;
	LPWSTR path = A2W(pszPath);

	bRet = ::CreateProcess(NULL, path, NULL, NULL, FALSE, CREATE_NEW_CONSOLE, NULL, NULL, &si, &pi);
	if (bRet)
	{
		//不使用的句柄最好关掉
		::CloseHandle(pi.hThread);
		::CloseHandle(pi.hProcess);
		return TRUE;
	}
	return FALSE;
}



//************************************************************
// 函数名称: MmRunDll 
// 函数说明: 在内存中运行dll
// 作	 者: GuiShou
// 时	 间: 2019/1/20
// 参	 数: CString path,文件路径 
// 返 回 值: BOOL 是否成功
//************************************************************
BOOL CStrat::MmRunDll(CString path)
{
	//打开DLL文件
	HANDLE hFile = CreateFile(path, GENERIC_READ | GENERIC_WRITE,
	FILE_SHARE_READ | FILE_SHARE_WRITE,NULL, OPEN_EXISTING, 
	FILE_ATTRIBUTE_ARCHIVE, NULL);
	
	if (hFile==INVALID_HANDLE_VALUE)
	{
		m_Tip += "CreateFile ERROR\r\n";
		return FALSE;
	}
	
	//获取DLL文件大小
	DWORD dwFileSize = GetFileSize(hFile, NULL);
	
	//根据文件大小申请动态内存
	BYTE* lpData = new BYTE[dwFileSize]{0};
	if (lpData==NULL)
	{
		m_Tip += "new ERROR\r\n";
		return FALSE;
	}
	
	//读取DLL到内存中
	DWORD dwRet = 0;
	ReadFile(hFile, lpData, dwFileSize, &dwRet, NULL);

	//将内存中的DLL以内存对齐方式加载到程序中
	LPVOID lpBaseAddress = MmLoadLibrary(lpData, dwFileSize);
	if (lpBaseAddress == NULL)
	{
		m_Tip += "MmLoadLibrary ERROR\r\n";
		return FALSE;
	}

	//获取DLL的导出函数并调用
	typedef BOOL(*typedef_ShowMessage)(char *lpszText, char *lpszCaption);
	typedef_ShowMessage ShowMessage = (typedef_ShowMessage)MmGetProcAddress(lpBaseAddress, "ShowMessage");
	if (ShowMessage == NULL)
	{
		m_Tip += "MmGetProcAddress ERROR\r\n";
		return FALSE;
	}

	ShowMessage("导出函数调用成功", "WOW");

	//释放从内存加载的DLL
	BOOL bRet = MmFreeLibrary(lpBaseAddress);
	if (bRet == FALSE)
	{
		m_Tip += "MmFreeLibrary ERROR\r\n";
		return FALSE;
	}
	UpdateData(FALSE);
	//释放内存 关闭句柄
	delete[] lpData;
	lpData = NULL;
	CloseHandle(hFile);
	return TRUE;
}




//************************************************************
// 函数名称: MmLoadLibrary 
// 函数说明: 将内存中的DLL按照内存对齐的方式加载到程序中
// 作	 者: GuiShou
// 时	 间: 2019/1/20
// 参	 数: LPVOID lpData内存中的DLL基址 , DWORD dwSize DLL文件大小
// 返 回 值: LPVOID 加载成功后的DLL基址
//************************************************************
LPVOID CStrat::MmLoadLibrary(LPVOID lpData, DWORD dwSize)
{
	LPVOID lpBaseAddress = NULL;

	//获取内存中的映像大小
	DWORD dwSizeOfImage = GetSizeOfImage(lpData);

	//在进程中开辟一个可读可写可执行的内存块
	lpBaseAddress = VirtualAlloc(NULL, dwSizeOfImage, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);
	if (lpBaseAddress == NULL)
	{
		m_Tip += "VirtualAlloc ERROR\r\n";
		return NULL;
	}
	RtlZeroMemory(lpBaseAddress, dwSizeOfImage);

	//将内存DLL数据按SectionAlignment大小对齐映射到内存中
	if (FALSE == MmMapFile(lpData, lpBaseAddress))
	{
		m_Tip += "MmMapFile ERROR\r\n";
		return NULL;
	}

	//修复重定位
	if (FixRelocationTable(lpBaseAddress)==FALSE)
	{
		m_Tip += "FixRelocationTable ERROR\r\n";
		return NULL;
	}

	//修复导入表
	if (FixImportTable(lpBaseAddress) == FALSE)
	{
		m_Tip += "FixImportTable ERROR\r\n";
		return NULL;
	}

	//修改页属性 根据每个页的属性单独设置其对应内存页的属性
	//统一设置成一个属性PAGE_EXECUTE_READWRITE
	DWORD dwOldProtect = 0;
	if (VirtualProtect(lpBaseAddress,dwSizeOfImage,PAGE_EXECUTE_READWRITE,&dwOldProtect)==FALSE)
	{
		m_Tip += "VirtualProtect ERROR\r\n";
		return NULL;
	}

	//修改PE文件映像基址ImageBase
	if (SetImageBase(lpBaseAddress) == FALSE)
	{
		m_Tip += "SetImageBase ERROR\r\n";
		return NULL;
	}

	//调用DLL的入口函数DllMain 函数地址即为PE文件的入口点AddressOfEntryPoint
	if (CallDllMain(lpBaseAddress) == FALSE)
	{
		m_Tip += "CallDllMain ERROR\r\n";
		return NULL;
	}
	UpdateData(FALSE);
	return lpBaseAddress;
}


//************************************************************
// 函数名称: GetSizeOfImage 
// 函数说明: 获取内存中的映像大小
// 作	 者: GuiShou
// 时	 间: 2019/1/20
// 参	 数: LPVOID lpData内存中的DLL基址 
// 返 回 值: DWORD PE文件结构中IMAGE_NT_HEADERS.OptionalHeader.SizeOfImage值的大小
//************************************************************
DWORD CStrat::GetSizeOfImage(LPVOID lpData)
{
	DWORD dwSizeOfImage = 0;
	PIMAGE_DOS_HEADER pDosHeader = (PIMAGE_DOS_HEADER)lpData;
	PIMAGE_NT_HEADERS pNtHeader = (PIMAGE_NT_HEADERS)((ULONG32)pDosHeader+pDosHeader->e_lfanew);
	dwSizeOfImage = pNtHeader->OptionalHeader.SizeOfImage;
	return dwSizeOfImage;
}



//************************************************************
// 函数名称: MmMapFile 
// 函数说明: 将内存DLL数据按SectionAlignment大小对齐映射到进程内存中
// 作	 者: GuiShou
// 时	 间: 2019/1/20
// 参	 数: LPVOID lpData 内存DLL文件数据的基址, 
// LPVOID lpBaseAddress 内存DLL数据按SectionAlignment大小对齐映射到进程内存中的内存基址 
// 返 回 值: BOOL 是否成功
//************************************************************
BOOL CStrat::MmMapFile(LPVOID lpData, LPVOID lpBaseAddress)
{
	PIMAGE_DOS_HEADER pDosHeader = (PIMAGE_DOS_HEADER)lpData;
	PIMAGE_NT_HEADERS pNtHeader = (PIMAGE_NT_HEADERS)((ULONG32)pDosHeader+pDosHeader->e_lfanew);

	//获取SizeOfHeaders的值 所有头部的大小
	DWORD dwSizeOfHeaders = pNtHeader->OptionalHeader.SizeOfHeaders;
	//获取区段数量
	WORD wNumberOfSections = pNtHeader->FileHeader.NumberOfSections;
	//获取第一个区段的地址 Nt头的起始位置+整个Nt头的大小=区段头起始地址(也可以用那个宏)
	PIMAGE_SECTION_HEADER pSectionHeader = (PIMAGE_SECTION_HEADER)((DWORD)pNtHeader + sizeof(IMAGE_NT_HEADERS));

	//加载所有头部拷贝到内存
	RtlCopyMemory(lpBaseAddress, lpData, dwSizeOfHeaders);

	//对齐SectionAlignment循环加载所有区段
	WORD i = 0;
	LPVOID lpSrcMem = NULL;
	LPVOID lpDestMem = NULL;
	DWORD dwSizeOfRawData = 0;
	for (i=0;i<wNumberOfSections;i++)
	{
		//过滤掉无效区段
		if ((0==pSectionHeader->VirtualAddress)||(0==pSectionHeader->SizeOfRawData))
		{
			pSectionHeader++;
			continue;
		}
		lpSrcMem = (LPVOID)((DWORD)lpData + pSectionHeader->PointerToRawData);
		lpDestMem = (LPVOID)((DWORD)lpBaseAddress + pSectionHeader->VirtualAddress);
		dwSizeOfRawData = pSectionHeader->SizeOfRawData;
		RtlCopyMemory(lpDestMem, lpSrcMem, dwSizeOfRawData);

		pSectionHeader++;
	}
	return TRUE;
}



//************************************************************
// 函数名称: FixRelocationTable 
// 函数说明: 修复重定位
// 作	 者: GuiShou
// 时	 间: 2019/1/20
// 参	 数: LPVOID lpBaseAddress 内存DLL数据按SectionAlignment大小对齐映射到进程内存中的内存基址 
// 返 回 值: BOOL 是否成功
//************************************************************
BOOL CStrat::FixRelocationTable(LPVOID lpBaseAddress)
{
	/* 重定位表的结构：
	// DWORD sectionAddress, DWORD size (包括本节需要重定位的数据)
	// 例如 1000节需要修正5个重定位数据的话，重定位表的数据是
	// 00 10 00 00   14 00 00 00      xxxx xxxx xxxx xxxx xxxx 0000
	// -----------   -----------      ----
	// 给出节的偏移  总尺寸=8+6*2     需要修正的地址           用于对齐4字节
	// 重定位表是若干个相连，如果address 和 size都是0 表示结束
	// 需要修正的地址是12位的，高4位是形态字，intel cpu下是3
	*/
	//假设NewBase是0x600000,而文件中设置的缺省ImageBase是0x400000,则修正偏移量就是0x200000
	//注意重定位表的位置可能和硬盘文件中的偏移地址不同，应该使用加载后的地址

	PIMAGE_DOS_HEADER pDosHeader = (PIMAGE_DOS_HEADER)lpBaseAddress;
	PIMAGE_NT_HEADERS pNtHeader = (PIMAGE_NT_HEADERS)((ULONG32)pDosHeader + pDosHeader->e_lfanew);
	PIMAGE_BASE_RELOCATION pLoc = (PIMAGE_BASE_RELOCATION)((unsigned long)pDosHeader +
	pNtHeader->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_BASERELOC].VirtualAddress);

	//判断是否有重定位表
	if ((PVOID)pLoc==(PVOID)pDosHeader)
	{
		//重定位表为空
		return TRUE;
	}

	//开始修复重定位
	while ((pLoc->VirtualAddress+pLoc->SizeOfBlock)!=0)
	{
		WORD* pLocData = (WORD*)((PBYTE)pLoc + sizeof(IMAGE_BASE_RELOCATION));

		//计算本区段需要修正的重定位项的数量
		int nNumberOfReloc = (pLoc->SizeOfBlock - sizeof(IMAGE_BASE_RELOCATION)) / sizeof(WORD);

		for (int i=0;i<nNumberOfReloc;i++)
		{
			// 每个WORD由两部分组成。高4位指出了重定位的类型，WINNT.H中的一系列IMAGE_REL_BASED_xxx定义了重定位类型的取值。
			// 低12位是相对于VirtualAddress域的偏移，指出了必须进行重定位的位置
			if ((DWORD)(pLocData[i]& 0x0000F000)== 0x00003000)	//这是一个需要修正的地址
			{
				DWORD* pAddress = (DWORD*)((PBYTE)pDosHeader + pLoc->VirtualAddress + (pLocData[i] & 0x0FFF));
				DWORD dwDelta = (DWORD)pDosHeader - pNtHeader->OptionalHeader.ImageBase;
				*pAddress += dwDelta;
			}
		}

		//下一个重定位块
		pLoc = (PIMAGE_BASE_RELOCATION)((PBYTE)pLoc + pLoc->SizeOfBlock);
	}
	return TRUE;
}



//************************************************************
// 函数名称: FixImportTable 
// 函数说明: 修复导入表
// 作	 者: GuiShou
// 时	 间: 2019/1/20
// 参	 数: LPVOID lpBaseAddress 内存DLL数据按SectionAlignment大小对齐映射到进程内存中的内存基址 
// 返 回 值: BOOL 是否成功
//************************************************************
BOOL CStrat::FixImportTable(LPVOID lpBaseAddress)
{
	PIMAGE_DOS_HEADER pDosHeader = (PIMAGE_DOS_HEADER)lpBaseAddress;
	PIMAGE_NT_HEADERS pNtHeader = (PIMAGE_NT_HEADERS)((ULONG32)pDosHeader + pDosHeader->e_lfanew);
	PIMAGE_IMPORT_DESCRIPTOR pImportable = (PIMAGE_IMPORT_DESCRIPTOR)((DWORD)pDosHeader+
	pNtHeader->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_IMPORT].VirtualAddress);

	//循环遍历DLL导入表中的DLL及获取DLL导入表中的函数地址
	char* lpDllName = nullptr;
	HMODULE hDll = NULL;
	PIMAGE_THUNK_DATA lpImportNameArray = NULL;
	PIMAGE_IMPORT_BY_NAME lpImportByName = NULL;
	PIMAGE_THUNK_DATA lpImportFuncAddrArray = NULL;
	FARPROC	lpFuncAddress = NULL;
	DWORD i = 0;

	while (TRUE)
	{
		if (pImportable->OriginalFirstThunk==0)
		{
			break;
		}
		//获取导入表中的DLL名称并加载DLL
		lpDllName = (char*)((DWORD)pDosHeader + pImportable->Name);
		hDll = GetModuleHandleA(lpDllName);
		if (hDll==NULL)
		{
			hDll = LoadLibraryA(lpDllName);
			if (hDll==NULL)
			{
				pImportable++;
				continue;
			}
		}

		i = 0;
		//获取导入函数名称表首地址
		lpImportNameArray = (PIMAGE_THUNK_DATA)((DWORD)pDosHeader + pImportable->OriginalFirstThunk);
		//获取导入函数地址表首地址
		lpImportFuncAddrArray = (PIMAGE_THUNK_DATA)((DWORD)pDosHeader + pImportable->FirstThunk);

		while (TRUE)
		{
			if (lpImportNameArray[i].u1.AddressOfData==0)
			{
				break;
			}
			//获取IMAGE_IMPORT_BY_NAME结构
			lpImportByName = (PIMAGE_IMPORT_BY_NAME)((DWORD)pDosHeader + lpImportNameArray[i].u1.AddressOfData);

			//判断函数是以名称导入还是以序号导入
			if (0x80000000&lpImportNameArray[i].u1.Ordinal)
			{
				//序号导入
				// 当IMAGE_THUNK_DATA值的最高位为1时，表示函数以序号方式输入，这时，低位被看做是一个函数序号
				lpFuncAddress = GetProcAddress(hDll, (LPCSTR)(lpImportNameArray[i].u1.Ordinal & 0x0000FFFF));
			}
			else
			{
				//名称导入
				lpFuncAddress = GetProcAddress(hDll, (LPCSTR)lpImportByName->Name);
			}

			lpImportFuncAddrArray[i].u1.Function = (DWORD)lpFuncAddress;
			i++;
		}
		pImportable++;
	}
	return TRUE;
}



//************************************************************
// 函数名称: SetImageBase 
// 函数说明: 修改PE文件的映像基址
// 作	 者: GuiShou
// 时	 间: 2019/1/20
// 参	 数: LPVOID lpBaseAddress 内存DLL数据按SectionAlignment大小对齐映射到进程内存中的内存基址 
// 返 回 值: BOOL 是否成功
//************************************************************
BOOL CStrat::SetImageBase(LPVOID lpBaseAddress)
{
	PIMAGE_DOS_HEADER pDosHeader = (PIMAGE_DOS_HEADER)lpBaseAddress;
	PIMAGE_NT_HEADERS pNtHeaders = (PIMAGE_NT_HEADERS)((ULONG32)pDosHeader + pDosHeader->e_lfanew);
	pNtHeaders->OptionalHeader.ImageBase = (ULONG32)lpBaseAddress;

	return TRUE;
}


//************************************************************
// 函数名称: CallDllMain 
// 函数说明: 调用DLL的入口函数DllMain
// 作	 者: GuiShou
// 时	 间: 2019/1/20
// 参	 数: LPVOID lpBaseAddress 内存DLL数据按SectionAlignment大小对齐映射到进程内存中的内存基址 
// 返 回 值: BOOL 是否成功
//************************************************************
BOOL CStrat::CallDllMain(LPVOID lpBaseAddress)
{
	//定义函数指针变量
	typedef_DllMain DllMain = NULL;
	PIMAGE_DOS_HEADER pDosHeader = (PIMAGE_DOS_HEADER)lpBaseAddress;
	PIMAGE_NT_HEADERS pNtHeaders = (PIMAGE_NT_HEADERS)((ULONG32)pDosHeader + pDosHeader->e_lfanew);

	DllMain = (typedef_DllMain)((ULONG32)pDosHeader + pNtHeaders->OptionalHeader.AddressOfEntryPoint);

	//调用入口函数 附加进程DLL_PROCESS_ATTACH
	BOOL bRet = DllMain((HINSTANCE)lpBaseAddress, DLL_PROCESS_ATTACH, NULL);
	if (bRet==FALSE)
	{
		m_Tip += "DllMain ERROR\r\n";
	}
	return bRet;
}



//************************************************************
// 函数名称: MmGetProcAddress 
// 函数说明: 模拟GetProcAddress获取内存DLL的导出函数
// 作	 者: GuiShou
// 时	 间: 2019/1/20
// 参	 数: LPVOID lpBaseAddress 内存DLL文件加载到进程中的加载基址, PCHAR lpszFuncName 导出函数的名字
// 返 回 值: LPVOID 返回导出函数的的地址
//************************************************************
LPVOID CStrat::MmGetProcAddress(LPVOID lpBaseAddress, PCHAR lpszFuncName)
{
	LPVOID lpFunc = NULL;
	// 获取导出表
	PIMAGE_DOS_HEADER pDosHeader = (PIMAGE_DOS_HEADER)lpBaseAddress;
	PIMAGE_NT_HEADERS pNtHeaders = (PIMAGE_NT_HEADERS)((ULONG32)pDosHeader + pDosHeader->e_lfanew);
	PIMAGE_EXPORT_DIRECTORY pExportTable = (PIMAGE_EXPORT_DIRECTORY)((DWORD)pDosHeader + pNtHeaders->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_EXPORT].VirtualAddress);
	// 获取导出表的数据
	PDWORD lpAddressOfNamesArray = (PDWORD)((DWORD)pDosHeader + pExportTable->AddressOfNames);
	PCHAR lpFuncName = NULL;
	PWORD lpAddressOfNameOrdinalsArray = (PWORD)((DWORD)pDosHeader + pExportTable->AddressOfNameOrdinals);
	WORD wHint = 0;
	PDWORD lpAddressOfFunctionsArray = (PDWORD)((DWORD)pDosHeader + pExportTable->AddressOfFunctions);

	DWORD dwNumberOfNames = pExportTable->NumberOfNames;
	DWORD i = 0;
	// 遍历导出表的导出函数的名称, 并进行匹配
	for (i = 0; i < dwNumberOfNames; i++)
	{
		lpFuncName = (PCHAR)((DWORD)pDosHeader + lpAddressOfNamesArray[i]);
		if (0 == ::lstrcmpiA(lpFuncName, lpszFuncName))
		{
			// 获取导出函数地址
			wHint = lpAddressOfNameOrdinalsArray[i];
			lpFunc = (LPVOID)((DWORD)pDosHeader + lpAddressOfFunctionsArray[wHint]);
			break;
		}
	}
	return lpFunc;
}



//************************************************************
// 函数名称: MmFreeLibrary 
// 函数说明: 释放从内存加载的DLL到进程内存的空间
// 作	 者: GuiShou
// 时	 间: 2019/1/20
// 参	 数: LPVOID lpBaseAddress 内存DLL文件加载到进程中的加载基址
// 返 回 值: BOOL是否成功
//************************************************************
BOOL CStrat::MmFreeLibrary(LPVOID lpBaseAddress)
{
	BOOL bRet = FALSE;
	if (lpBaseAddress==NULL)
	{
		return bRet;
	}
	bRet = VirtualFree(lpBaseAddress, 0, MEM_RELEASE);
	lpBaseAddress = NULL;
	return bRet;
}