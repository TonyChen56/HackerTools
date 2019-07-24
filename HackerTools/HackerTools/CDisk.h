#pragma once


// CDisk 对话框

class CDisk : public CDialogEx
{
	DECLARE_DYNAMIC(CDisk)

public:
	CDisk(LPCTSTR name, CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CDisk();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_Disk };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	CString m_Disk;
	CString m_DiskName;
	CString m_System;
	CString m_FreeSpace;
	CString m_TotalSpace;
	CString m_Info;

	CString m_path;

	//查看磁盘
	void CheckDisk();
};
