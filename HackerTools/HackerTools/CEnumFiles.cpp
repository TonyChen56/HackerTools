// CEnumFiles.cpp: 实现文件
//

#include "stdafx.h"
#include "HackerTools.h"
#include "CEnumFiles.h"
#include "afxdialogex.h"
#include "CDisk.h"
#include "CFileAttr.h"


TCHAR CurrentDir[MAX_PATH] = { 0 };		//当前目录
TCHAR CurrentFile[MAX_PATH] = { 0 };	//当前List选中的文件


// CEnumFiles 对话框

IMPLEMENT_DYNAMIC(CEnumFiles, CDialogEx)

CEnumFiles::CEnumFiles(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_EnumFiles, pParent)
{

}

CEnumFiles::~CEnumFiles()
{
}

void CEnumFiles::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO1, m_Combox);
	DDX_Control(pDX, IDC_LIST2, m_ListCtrl);
}


BEGIN_MESSAGE_MAP(CEnumFiles, CDialogEx)
	ON_CBN_SELCHANGE(IDC_COMBO1, &CEnumFiles::OnCbnSelchangeCombo1)
	ON_BN_CLICKED(IDC_CheckDisk, &CEnumFiles::OnBnClickedCheckdisk)
	ON_NOTIFY(NM_RCLICK, IDC_LIST2, &CEnumFiles::OnNMRClickList2)
	ON_COMMAND(ID_32773, &CEnumFiles::OnPopFileAttr)
	ON_NOTIFY(LVN_ITEMACTIVATE, IDC_LIST2, &CEnumFiles::OnLvnItemActivateList2)
END_MESSAGE_MAP()


// CEnumFiles 消息处理程序


BOOL CEnumFiles::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	//初始化Combox
	InitCombox();
	//初始化ListCtrl
	InitListCtrl();
	return TRUE;  
}



//************************************************************
// 函数名称: InitCombox
// 函数说明: 初始化Combox
// 作	 者: GuiShou
// 时	 间: 2019/1/29
// 参	 数: void
// 返 回 值: void
//************************************************************
void CEnumFiles::InitCombox()
{
	//初始化下拉列表相关信息 获取盘符
	TCHAR NameBuffer[MAX_PATH] = { 0 };
	//获取所有逻辑驱动器，放到NameBuffer里
	GetLogicalDriveStrings(MAX_PATH, NameBuffer);

	//循环所有字符 每个字符以\0结尾 循环一次只能拿到一个字符 循环完了之后需要加上字符长度
	for (int i = 0; NameBuffer[i] != 0; i++)
	{
		//显示到Combox
		m_Combox.AddString(&NameBuffer[i]);
		i += _tcslen(&NameBuffer[i]);
	}

	//设置Combox的默认选项
	m_Combox.SetCurSel(0);
}


//************************************************************
// 函数名称: InitListCtrl
// 函数说明: 初始化ListCtrl
// 作	 者: GuiShou
// 时	 间: 2019/1/29
// 参	 数: void
// 返 回 值: void
//************************************************************
void CEnumFiles::InitListCtrl()
{
	m_ListCtrl.SetExtendedStyle(LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT);
	m_ListCtrl.InsertColumn(0, L"名称", 0, 100);
	m_ListCtrl.InsertColumn(1, L"创建时间", 0, 200);
	m_ListCtrl.InsertColumn(2, L"类型", 0, 100);
	m_ListCtrl.InsertColumn(3, L"大小", 0, 100);

	//遍历文件(默认遍历C盘下的文件)

	BOOL bRet = EnumFiles(L"C:\\");
	if (bRet=FALSE)
	{
		MessageBox(L"文件遍历失败！");
		return;
	}
}


//************************************************************
// 函数名称: EnumFiles
// 函数说明: 遍历文件
// 作	 者: GuiShou
// 时	 间: 2019/1/29
// 参	 数: LPCTSTR FullPath 需要遍历文件的完整路径
// 返 回 值: BOOL 是否成功
//************************************************************
BOOL CEnumFiles::EnumFiles(LPCTSTR FullPath)
{
	//拷贝当前完成路径
	_tcscpy_s(CurrentDir, MAX_PATH, FullPath);
	//清空ListCtrl
	m_ListCtrl.DeleteAllItems();
	TCHAR FindPath[MAX_PATH] = { 0 };
	//拼接路径->当前目录下所有文件
	_tcscat_s(FindPath, MAX_PATH, FullPath);
	_tcscat_s(FindPath, MAX_PATH, _T("\\*"));	

	WIN32_FIND_DATA FindData = { 0 };
	//查找第一个文件
	HANDLE hFile = FindFirstFile(FindPath, &FindData);
	if (hFile==INVALID_HANDLE_VALUE)
	{
		MessageBox(L"FindFirstFile Error");
		return FALSE;
	}
	int index = 0;
	LVITEM LvItem = { 0 };	//列表视图控件
	do 
	{
		//过滤当前目录
		if (_tcscmp(FindData.cFileName,TEXT("."))!=0)
		{
			LvItem.iItem = index;		//List的索引值
			m_ListCtrl.InsertItem(&LvItem);	
			//显示文件名
			m_ListCtrl.SetItemText(index, 0, FindData.cFileName);

			SYSTEMTIME SystemTime = { 0 };
			//将文件时间转为系统时间
			FileTimeToSystemTime(&FindData.ftCreationTime, &SystemTime);
			TCHAR FileContent[MAX_PATH] = {0};
			//拼接文件创建时间
			_stprintf_s(FileContent,MAX_PATH,TEXT("%d/%d/%d/ %d:%d:%d"),
				SystemTime.wYear,SystemTime.wMonth,SystemTime.wDay,
				SystemTime.wHour,SystemTime.wMinute,SystemTime.wSecond);
			//显示创建时间
			m_ListCtrl.SetItemText(index, 1, FileContent);

			FileContent[0] = 0;
			//判断文件属性
			if (FindData.dwFileAttributes&FILE_ATTRIBUTE_DIRECTORY)
			{
				_tcscat_s(FileContent, MAX_PATH, TEXT("目录"));
			}
			else
			{
				_tcscat_s(FileContent, MAX_PATH, TEXT("文件"));
			}
			if (FindData.dwFileAttributes&FILE_ATTRIBUTE_HIDDEN)
			{
				_tcscat_s(FileContent, MAX_PATH, TEXT("隐藏"));
			}
			if (FindData.dwFileAttributes&FILE_ATTRIBUTE_READONLY)
			{
				_tcscat_s(FileContent, MAX_PATH, TEXT("只读"));

			}
			//显示文件属性
			m_ListCtrl.SetItemText(index, 2, FileContent);

			FileContent[0] = 0;
			if (FindData.dwFileAttributes&FILE_ATTRIBUTE_DIRECTORY)
			{
				_tcscat_s(FileContent, MAX_PATH, TEXT("-"));
			}
			else
			{
				if (FindData.nFileSizeLow> 1073741824)
				{
					_stprintf_s(FileContent, MAX_PATH, TEXT("%.2lfGB"), FindData.nFileSizeLow / 1024.0 / 1024.0 / 1024.0);
				}
				else if (FindData.nFileSizeLow > 1048576)
				{
					_stprintf_s(FileContent, MAX_PATH, TEXT("%.2lfMB"), FindData.nFileSizeLow / 1024.0 / 1024.0);

				}
				else
				{
					_stprintf_s(FileContent, MAX_PATH, TEXT("%.2lfKB"), FindData.nFileSizeLow / 1024.0 / 1024.0);

				}	
			}
			//显示文件大小
			m_ListCtrl.SetItemText(index, 3, FileContent);
			index++;
		}
	} while (FindNextFile(hFile,&FindData));

	return TRUE;
}

//************************************************************
// 函数名称: UpdateDir
// 函数说明: 更新目录
// 作	 者: GuiShou
// 时	 间: 2019/1/29
// 参	 数: LPCTSTR FullPath 文件完整路径
// 返 回 值: void
//************************************************************
void CEnumFiles::UpdateDir(LPCTSTR FullPath)
{
	TCHAR Path[MAX_PATH] = { 0 };
	_stprintf_s(Path, MAX_PATH, TEXT("%s\\%s"), CurrentDir, FullPath);

	if (GetFileAttributes(Path) & FILE_ATTRIBUTE_DIRECTORY)
	{
		EnumFiles(Path);
	}
}

//************************************************************
// 函数名称: OnCbnSelchangeCombo1
// 函数说明: 响应切换下拉框消息(选中复选框 右键添加事件处理程序)
// 作	 者: GuiShou
// 时	 间: 2019/1/29
// 参	 数: void
// 返 回 值: void
//************************************************************
void CEnumFiles::OnCbnSelchangeCombo1()
{
	TCHAR temp[MAX_PATH] = { 0 };
	//获取Combobox的选中内容
	m_Combox.GetLBText(m_Combox.GetCurSel(), temp);
	//根据选中的盘符遍历文件
	EnumFiles(temp);
}



//************************************************************
// 函数名称: OnBnClickedCheckdisk
// 函数说明: 查看按钮
// 作	 者: GuiShou
// 时	 间: 2019/1/30
// 参	 数: void
// 返 回 值: void
//************************************************************
void CEnumFiles::OnBnClickedCheckdisk()
{
	TCHAR temp[20] = { 0 };
	//将下拉框的字符串以构造函数的形式传给磁盘查看对话框
	m_Combox.GetWindowText(temp, 20);
	CDisk *pCD = new CDisk(temp);
	pCD->DoModal();
	delete pCD;
}



//************************************************************
// 函数名称: OnNMRClickList2
// 函数说明: 响应List控件的右键消息
// 作	 者: GuiShou
// 时	 间: 2019/1/30
// 参	 数: NMHDR *pNMHDR, LRESULT *pResult
// 返 回 值: void
//************************************************************
void CEnumFiles::OnNMRClickList2(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	CMenu menu, *pmenu;
	menu.LoadMenu(IDR_MENU2);
	pmenu = menu.GetSubMenu(0);
	//定义一个用于确定光标位置的位置 
	CPoint point1;
	//获取当前光标的位置，以便使得菜单可以跟随光标
	GetCursorPos(&point1);
	//在指定位置显示弹出菜单
	pmenu->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, point1.x, point1.y, this);
	*pResult = 0;
}


//************************************************************
// 函数名称: OnPopFileAttr
// 函数说明: 响应属性菜单的点击
// 作	 者: GuiShou
// 时	 间: 2019/1/30
// 参	 数: void
// 返 回 值: void
//************************************************************
void CEnumFiles::OnPopFileAttr()
{
	//获取选中的行号
	int i = m_ListCtrl.GetSelectionMark();
	//根据选中的行号获取对应的文件名
	CString FileName = m_ListCtrl.GetItemText(i, 0);
	//拼接出当前选中文件的完整路径
	_stprintf_s(CurrentFile, MAX_PATH, L"%s\\%s", CurrentDir, FileName);
	//弹出对话框 把文件完整路径通过构造函数的参数传过去
	CFileAttr *pFaD = new CFileAttr(CurrentFile);
	pFaD->DoModal();
	delete pFaD;
}



//************************************************************
// 函数名称: OnLvnItemActivateList2
// 函数说明: 响应List控件的双击消息
// 作	 者: GuiShou
// 时	 间: 2019/1/30
// 参	 数: NMHDR *pNMHDR, LRESULT *pResult
// 返 回 值: void
//************************************************************
void CEnumFiles::OnLvnItemActivateList2(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMIA = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	//获取选中的行号
	int i = m_ListCtrl.GetSelectionMark();
	//获取对应的文件名
	CString FileName = m_ListCtrl.GetItemText(i, 0);
	//更新目录
	UpdateDir(FileName);

	*pResult = 0;
}
