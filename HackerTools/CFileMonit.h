#pragma once


// CFileMonit 对话框

class CFileMonit : public CDialogEx
{
	DECLARE_DYNAMIC(CFileMonit)

public:
	CFileMonit(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CFileMonit();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_FileMonit };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CString m_Tip;
	CString m_FileMonit;
	virtual BOOL OnInitDialog();

	//创建线程创建文件
	void MonitorFiles(char* pszDirectory);
};
