#pragma once


// CBase 对话框

class CBase : public CDialogEx
{
	DECLARE_DYNAMIC(CBase)

public:
	CBase(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CBase();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_Base };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedRunsingle();
	afx_msg void OnBnClickedReleaseres();
	CString m_Edit;
};
