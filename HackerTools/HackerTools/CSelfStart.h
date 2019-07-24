#ifndef _SERVICE_OPERATE_H_
#define _SERVICE_OPERATE_H_
#pragma once
#include <Shlwapi.h>
#pragma comment(lib, "Shlwapi.lib")

// CSelfStart 对话框

class CSelfStart : public CDialogEx
{
	DECLARE_DYNAMIC(CSelfStart)

public:
	CSelfStart(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CSelfStart();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_SelfStart };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnDropFiles(HDROP hDropInfo);
	CString m_Path;
	CString m_Tips;
	afx_msg void OnBnClickedRegedit();
	afx_msg void OnBnClickedStartdir();
	afx_msg void OnBnClickedTask();
	afx_msg void OnBnClickedService();

	//服务操作函数
	BOOL SystemServiceOperate(char* csPath, int iOperateType);


	//服务相关操作
	enum MyEnum
	{
		Load_Service = 0,
		Start_Service = 1,
		Stop_Service = 2,
		Delete_Service = 3
	}m_OperateType;

	//设置注册表使程序自启动
	BOOL SetRegAutoRun(const char* lpszFileName, const char* lpszValueName);

	//使用快速启动目录实现自启动
	BOOL FastStartDirectory(const char* lpszSrcFilePath);

private:
	CString m_FastStartDir;		//用于保存快速启动目录
};

#endif