#pragma once


// CInject 对话框

class CInject : public CDialogEx
{
	DECLARE_DYNAMIC(CInject)

public:
	CInject(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CInject();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_Inject };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	int m_Radio;			//绑定单选框的变量
	afx_msg void OnBnClickedRadio1();
	afx_msg void OnDropFiles(HDROP hDropInfo);
	afx_msg void OnBnClickedInject();
	

	CString m_Edit;		//存放dll路径的编辑框
	CString m_TipMsg;	//注入时的提示信息

	void SetGlobalHook();		//设置全局钩子
	BOOL ZwCreateThreadExInjectDll(DWORD dwProcessId, char* pszDllFileName);	//远程线程注入
	CString m_PidAndName;
	virtual BOOL OnInitDialog();
	BOOL EnableDebugPrivilege(BOOL fEnable);	//提权
	BOOL APCInject(char *pszProcessName, char *pszDllName);	//APC注入
	DWORD GetPidByPname(char* pszProcessName);		//根据进程名获取进程ID
	BOOL GetAllTidByPid(DWORD dwProcessId,DWORD** ppThreadId,DWORD* pdwThreadIdLength);	//根据进程ID获取所有的线程ID

	CEdit m_objEdit;
};
