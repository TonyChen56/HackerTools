#pragma once


// CPrivilege 对话框

class CPrivilege : public CDialogEx
{
	DECLARE_DYNAMIC(CPrivilege)

public:
	CPrivilege(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CPrivilege();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_Privilege };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
//	afx_msg void OnEnChangeEdit1();
//	CString m_Tip;
	afx_msg void OnBnClickedDebug();
	afx_msg void OnBnClickedPassuac();

	//提升令牌权限
	BOOL EnablePrivileges(const wchar_t* pszPrivilegesName);
	CListCtrl m_List;
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedCheckprivilege();

	//遍历当前进程权限
	BOOL FindAllPrivileges();
	CString m_Path;
	afx_msg void OnDropFiles(HDROP hDropInfo);
};
