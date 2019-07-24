
// HackerToolsDlg.h: 头文件
//

#pragma once
#include "CMyTableCtrl.h"

// CHackerToolsDlg 对话框
class CHackerToolsDlg : public CDialogEx
{
// 构造
public:
	CHackerToolsDlg(CWnd* pParent = nullptr);	// 标准构造函数

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_HACKERTOOLS_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnHelp();
	afx_msg void OnIntroductions();
	CMyTableCtrl m_MyTable;
};
