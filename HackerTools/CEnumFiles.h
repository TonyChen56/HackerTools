#pragma once


// CEnumFiles 对话框

class CEnumFiles : public CDialogEx
{
	DECLARE_DYNAMIC(CEnumFiles)

public:
	CEnumFiles(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CEnumFiles();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_EnumFiles };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CComboBox m_Combox;
	CListCtrl m_ListCtrl;
	virtual BOOL OnInitDialog();

	//初始化Combox
	void InitCombox();

	//初始化ListCtrl
	void InitListCtrl();

	//遍历文件
	BOOL EnumFiles(LPCTSTR FullPath);

	//更新目录
	void UpdateDir(LPCTSTR FullPath);
	afx_msg void OnCbnSelchangeCombo1();
	afx_msg void OnBnClickedCheckdisk();
	afx_msg void OnNMRClickList2(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnPopFileAttr();
	afx_msg void OnLvnItemActivateList2(NMHDR *pNMHDR, LRESULT *pResult);
};
