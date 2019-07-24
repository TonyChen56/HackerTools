#pragma once


// CEnumProcess 对话框

class CEnumProcess : public CDialogEx
{
	DECLARE_DYNAMIC(CEnumProcess)

public:
	CEnumProcess(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CEnumProcess();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_EnumProcess };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	CListCtrl m_List;

	//遍历进程
	BOOL EnumProcess();
};
