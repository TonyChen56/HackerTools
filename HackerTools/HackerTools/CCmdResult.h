#pragma once


// CCmdResult 对话框

class CCmdResult : public CDialogEx
{
	DECLARE_DYNAMIC(CCmdResult)

public:
	CCmdResult(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CCmdResult();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CmdResult };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CString m_Cmd;
	CString m_Result;


	// 执行 cmd 命令, 并获取执行结果数据
	BOOL PipeCmd(char *pszCmd, char* pszResultBuffer, DWORD dwResultBufferSize);
	afx_msg void OnBnClickedShowcmd();
};
