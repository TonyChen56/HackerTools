#pragma once


// CDia_Core 对话框

class CDia_Core : public CDialogEx
{
	DECLARE_DYNAMIC(CDia_Core)

public:
	CDia_Core(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CDia_Core();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CORE };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
};
