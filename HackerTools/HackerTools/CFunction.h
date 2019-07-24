#pragma once
#include <Winuser.h>
#include <string>
#include <fstream>
#include <iostream>

using namespace std;

// CFunction 对话框

class CFunction : public CDialogEx
{
	DECLARE_DYNAMIC(CFunction)

public:
	CFunction(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CFunction();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_Function };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedEnumprocess();
	afx_msg void OnBnClickedEnumfiles();
	afx_msg void OnBnClickedScreenshot();

	//捕获桌面截屏
	BOOL ScreenCapture();

	//绘制鼠标
	BOOL PaintMouse(HDC hdc);

	//保存为图片
	BOOL SaveBmp(HBITMAP hBmp);
	afx_msg void OnBnClickedKeyrecord();
	
	//注册原始输入设备
	BOOL RegisterDevice();

	//获取原始输入数据
	BOOL GetInputData();
	afx_msg void OnBnClickedShowcmd();

	//响应WM_DeviceChange消息监控U盘
	LRESULT OnDeviceChange(WPARAM wParam, LPARAM lParam);
	afx_msg void OnBnClickedDevicemonit();
	afx_msg void OnBnClickedFilemonit();

	//构造批处理文件
	BOOL CreateBat(char* pszFileName);

	//自删除
	BOOL DeleteSelf();
	afx_msg void OnBnClickedSelfdelete();

	//拷贝文件
	BOOL CFunction::CopyFiles(char* FullPath, char* szPostFix);
};
