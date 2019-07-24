#pragma once


// CHide 对话框

class CHide : public CDialogEx
{
	DECLARE_DYNAMIC(CHide)

public:
	CHide(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CHide();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_Hide };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedPretend();

	BOOL IsSystem32();
//	afx_msg void OnBnClickedButton3();
	CString m_Path;
	afx_msg void OnDropFiles(HDROP hDropInfo);
	virtual BOOL OnInitDialog();

	//修改本进程的PEB中的路径和命令行信息 实现进程伪装
	BOOL DisguiseProcess(const wchar_t* lpwszPath, const wchar_t* lpwszCmd);


	CString m_Tip;
	afx_msg void OnBnClickedPuppet();

	//使用ShellCode替换目标进程
	BOOL ReplaceProcess(const wchar_t* pszFilePath, PVOID pReplaceData, DWORD dwReplaceDataSize, DWORD dwRunOffset);

	afx_msg void OnBnClickedHijack();
};
