#pragma once


// CStrat 对话框

class CStrat : public CDialogEx
{
	DECLARE_DYNAMIC(CStrat)

public:
	CStrat(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CStrat();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_Start };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CString m_Path;			//保存文件路径的字符串
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedCreateprocess();
	afx_msg void OnBnClickedMemloaddll();
	afx_msg void OnBnClickedCreateuserprocess();
	afx_msg void OnDropFiles(HDROP hDropInfo);
	CString m_Tip;		//提示信息

	//以WinExec的方式启动进程
	BOOL WinExecStart(char* pszPath, UINT icmdshow);

	//以ShellExecute的方式启动进程
	BOOL ShellExecuteStart(CString pszPath, UINT icmdshow);

	//以CreateProcess的方式启动进程
	BOOL CreateProcessStart(char* pszPath, UINT icmdshow);

	//在内存中加载运行dll
	BOOL MmRunDll(CString path);

	//将内存中的DLL按照内存对齐的方式加载到程序中
	LPVOID MmLoadLibrary(LPVOID lpData, DWORD dwSize);

	//获取内存中的映像大小
	DWORD GetSizeOfImage(LPVOID lpData);

	//将内存DLL数据按SectionAlignment大小对齐映射到进程内存中
	BOOL MmMapFile(LPVOID lpData, LPVOID lpBaseAddress);

	//修复重定位表
	BOOL FixRelocationTable(LPVOID lpBaseAddress);

	//修复导入表
	BOOL FixImportTable(LPVOID lpBaseAddress);

	//修改映像基址
	BOOL SetImageBase(LPVOID lpBaseAddress);

	//调用DLL的入口函数DllMain
	BOOL CallDllMain(LPVOID lpBaseAddress);

	//模拟GetProcAddress获取内存DLL的导出函数
	LPVOID MmGetProcAddress(LPVOID lpBaseAddress, PCHAR lpszFuncName);

	//释放从内存加载的DLL到进程内存的空间
	BOOL MmFreeLibrary(LPVOID lpBaseAddress);
};
