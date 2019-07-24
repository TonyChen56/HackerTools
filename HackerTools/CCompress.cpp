// CCompress.cpp: 实现文件
//

#include "stdafx.h"
#include "HackerTools.h"
#include "CCompress.h"
#include "afxdialogex.h"
#include <winternl.h>





// CCompress 对话框

IMPLEMENT_DYNAMIC(CCompress, CDialogEx)

CCompress::CCompress(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_Compress, pParent)
	, m_Path(_T(""))
	, m_Data(_T(""))
	, m_Tip(_T(""))
{

}

CCompress::~CCompress()
{
}

void CCompress::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_Path);
	DDX_Text(pDX, IDC_EDIT2, m_Data);
	DDX_Text(pDX, IDC_EDIT3, m_Tip);
}


BEGIN_MESSAGE_MAP(CCompress, CDialogEx)
	ON_WM_DROPFILES()
	ON_BN_CLICKED(IDC_DataCompress, &CCompress::OnBnClickedDatacompress)
	ON_BN_CLICKED(IDC_FileCompress, &CCompress::OnBnClickedFilecompress)
END_MESSAGE_MAP()





	//定义必要的函数指针
typedef NTSTATUS(WINAPI *typedef_RtlGetCompressionWorkSpaceSize)(
	_In_  USHORT CompressionFormatAndEngine,
	_Out_ PULONG CompressBufferWorkSpaceSize,
	_Out_ PULONG CompressFragmentWorkSpaceSize
	);

typedef NTSTATUS(WINAPI *typedef_RtlCompressBuffer)(
	_In_  USHORT CompressionFormatAndEngine,
	_In_  PUCHAR UncompressedBuffer,
	_In_  ULONG  UncompressedBufferSize,
	_Out_ PUCHAR CompressedBuffer,
	_In_  ULONG  CompressedBufferSize,
	_In_  ULONG  UncompressedChunkSize,
	_Out_ PULONG FinalCompressedSize,
	_In_  PVOID  WorkSpace
	);

typedef NTSTATUS(WINAPI *typedef_RtlDecompressBuffer)(
	_In_  USHORT CompressionFormat,
	_Out_ PUCHAR UncompressedBuffer,
	_In_  ULONG  UncompressedBufferSize,
	_In_  PUCHAR CompressedBuffer,
	_In_  ULONG  CompressedBufferSize,
	_Out_ PULONG FinalUncompressedSize
	);




BOOL CCompress::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	//管理员模式下取消过滤拖拽消息
	ChangeWindowMessageFilterEx(m_hWnd, WM_DROPFILES, MSGFLT_ALLOW, NULL);
	ChangeWindowMessageFilterEx(m_hWnd, 0x0049, MSGFLT_ALLOW, NULL);

	return TRUE;  
}


//************************************************************
// 函数名称: OnDropFiles
// 函数说明: 响应文件拖拽消息
// 作	 者: GuiShou
// 时	 间: 2019/1/24
// 参	 数: HDROP hDropInfo 文件句柄
// 返 回 值: void
//************************************************************
void CCompress::OnDropFiles(HDROP hDropInfo)
{
	//清空文本框的内容
	m_Path = "";

	//获取文件路径
	TCHAR szPath[MAX_PATH] = { 0 };
	DragQueryFile(hDropInfo, 0, szPath, MAX_PATH);

	//显示到控件上
	m_Path = szPath;

	UpdateData(FALSE);

	CDialogEx::OnDropFiles(hDropInfo);
}


//************************************************************
// 函数名称: OnBnClickedDatacompress
// 函数说明: 数据压缩按钮
// 作	 者: GuiShou
// 时	 间: 2019/1/25
// 参	 数: void
// 返 回 值: void
//************************************************************
void CCompress::OnBnClickedDatacompress()
{
	UpdateData(TRUE);
	//检测数据是否为空
	if (m_Data == "")
	{
		MessageBox(L"数据不能为空");
		return;
	}


	BYTE *pCompressData = NULL;		//压缩的数据
	DWORD dwCompressDataLength = 0;	//压缩数据的长度
	BYTE* pUnCompressData = NULL;	//未压缩的数据
	DWORD dwUnCompressDataLength;	//未压缩的数据长度

	//MFC的问题 需要把压缩后的数据的十六进制显示到界面

	//转换数据
	USES_CONVERSION;
	pUnCompressData = (BYTE*)W2A(m_Data);
	dwUnCompressDataLength = ::lstrlenA((LPCSTR)pUnCompressData);

	//压缩数据
	DataCompress(pUnCompressData, dwUnCompressDataLength, &pCompressData, &dwCompressDataLength);

	//解压缩数据
	DataDeCompress(pCompressData, dwCompressDataLength, &pUnCompressData, &dwUnCompressDataLength);

	CString temp;
	//清空控件上显示的内容
	m_Data = "";
	
	//显示原数据
	m_Data += "原数据为:\r\n";
	temp = ByteToCString(pUnCompressData, dwUnCompressDataLength);
	m_Data += temp;
	m_Data += "\r\n";


	//显示压缩后的数据
	m_Data += "压缩后的数据为:\r\n";
	temp = ByteToCString(pCompressData, dwCompressDataLength);
	m_Data += temp;
	m_Data += "\r\n";


	//显示解压缩后的数据
	m_Data += "解压缩后的数据为:\r\n";
	temp = ByteToCString(pUnCompressData, dwUnCompressDataLength);
	m_Data += temp;
	m_Data += "\r\n";

	 //显示到控件
	UpdateData(FALSE);
}



//************************************************************
// 函数名称: OnBnClickedFilecompress
// 函数说明: 文件解压缩按钮
// 作	 者: GuiShou
// 时	 间: 2019/1/25
// 参	 数: void
// 返 回 值: void
//************************************************************
void CCompress::OnBnClickedFilecompress()
{
	//检查文件是否存在
	if (GetFileAttributes(m_Path)==INVALID_FILE_ATTRIBUTES)
	{
		MessageBox(L"文件不存在！请重试!");
		return;
	}

	BOOL bRet = FALSE;
	BYTE* pCompressData = NULL;
	DWORD dwCompressDataSize = 0;
	BYTE* pUnCompressData = NULL;
	DWORD dwUnCompressDataSize = 0;
	USES_CONVERSION;

	//将文件压缩为数据
	bRet = Zlib_CompressData(W2A(m_Path), &pCompressData, &dwCompressDataSize);
	if (bRet==FALSE)
	{
		m_Tip += "将文件压缩为数据失败\r\n";
		return;
	}
	else
	{
		m_Tip += "将文件压缩为数据成功\r\n";
	}
	//保存压缩数据为文件
	bRet = SaveToFile("CompressFile.GuiShou", pCompressData, dwCompressDataSize);
	if (bRet == FALSE)
	{
		m_Tip += "保存压缩文件失败\r\n";
		return;
	}
	else
	{
		m_Tip += "保存压缩文件成功\r\n";
	}

	//将文件解压缩为数据
	bRet = Zlib_UnCompressData("CompressFile.GuiShou", &pUnCompressData, &dwUnCompressDataSize);
	if (bRet == FALSE)
	{
		m_Tip += "将文件解压缩为数据失败\r\n";
		return;
	}
	else
	{
		m_Tip += "将文件解压缩为数据成功\r\n";
	}
	//保存解压缩数据为文件
	bRet = SaveToFile("GuiShou.exe",pUnCompressData, dwUnCompressDataSize);
	if (bRet == FALSE)
	{
		m_Tip += "保存解压缩数据为文件失败\r\n";
		return;
	}
	else
	{
		m_Tip += "保存解压缩数据为文件成功\r\n";
	}
	//释放内存
	delete[] pUnCompressData;
	delete[] pCompressData;
	UpdateData(FALSE);
}



//************************************************************
// 函数名称: DataCompress
// 函数说明: 数据压缩
// 作	 者: GuiShou
// 时	 间: 2019/1/26
// 参	 数: BYTE*  pUncompressData 未压缩的数据 DWORD dwUncompressDataLength 未压缩的数据的数据大小
// 参	 数: BYTE** ppCompressData  压缩后的数据 DWORD * pdwCompressDataLength 压缩后的数据大小
// 返 回 值: BOOL 是否成功
//************************************************************
BOOL CCompress::DataCompress(BYTE* pUncompressData, DWORD dwUncompressDataLength, BYTE** ppCompressData, DWORD* pdwCompressDataLength)
{


	BOOL bRet = FALSE;
	NTSTATUS status = 0;
	HMODULE hModule = NULL;
	//定义函数指针变量
	typedef_RtlGetCompressionWorkSpaceSize RtlGetCompressionWorkSpaceSize = NULL;
	typedef_RtlCompressBuffer RtlCompressBuffer = NULL;
	DWORD dwWorkSpaceSize = 0;
	DWORD dwFragmentWorkSpaceSize = 0;
	BYTE* pWorkSpace = NULL;
	BYTE* pCompressData = NULL;
	DWORD dwCompressDataLength = 4096;
	DWORD dwFinalCompressSize = 0;

	do 
	{
		//加载ntdll.dll
		hModule = LoadLibrary(L"ntdll.dll");
		if (hModule==NULL)
		{
			m_Tip += L"LoadLibrary Error\r\n";
			break;
		}

		//获取函数地址
		RtlGetCompressionWorkSpaceSize = (typedef_RtlGetCompressionWorkSpaceSize)::GetProcAddress(hModule, "RtlGetCompressionWorkSpaceSize");
		if (NULL == RtlGetCompressionWorkSpaceSize)
		{
			m_Tip += L"GetProcAddress Error\r\n";
			break;
		}
		RtlCompressBuffer = (typedef_RtlCompressBuffer)::GetProcAddress(hModule, "RtlCompressBuffer");
		if (NULL == RtlCompressBuffer)
		{
			m_Tip += L"GetProcAddress Error\r\n";
			break;
		}

		//获取WorkSpace大小
		status = RtlGetCompressionWorkSpaceSize(COMPRESSION_FORMAT_LZNT1 | COMPRESSION_ENGINE_STANDARD, &dwWorkSpaceSize, &dwFragmentWorkSpaceSize);
		if (0!=status)
		{
			m_Tip += L"RtlGetCompressionWorkSpaceSize Error\r\n";
			break;
		}

		//申请动态内存
		pWorkSpace = new BYTE[dwWorkSpaceSize]{ 0 };
		if (pWorkSpace==NULL)
		{
			m_Tip += L"new Error\r\n";
			break;
		}

		while (TRUE)
		{
			//申请动态内存存储压缩后的数据
			pCompressData = new BYTE[dwCompressDataLength];
			if (pCompressData == NULL)
			{
				m_Tip += L"new Error\r\n";
				break;
			}


			//开始压缩数据 
			RtlCompressBuffer(COMPRESSION_FORMAT_LZNT1,pUncompressData,dwUncompressDataLength,(PUCHAR)pCompressData,dwCompressDataLength,4096,&dwFinalCompressSize,(PVOID)pWorkSpace);

			//如果压缩数据的缓冲区小于最终的压缩大小 说明压缩失败 需要重新压缩
			if (dwCompressDataLength<dwFinalCompressSize)
			{
				//释放内存
				if (pCompressData)
				{
					delete[] pCompressData;
				}
				dwCompressDataLength = dwFinalCompressSize;
			}
			else
			{
				//否则说明压缩成功 退出循环
				break;
			}
		}

		//返回压缩后的数据和长度
		*ppCompressData = pCompressData;
		*pdwCompressDataLength = dwFinalCompressSize;
		bRet = TRUE;
		UpdateData(FALSE);

	} while (FALSE);

	//释放资源
	if (pWorkSpace)
	{
		delete[] pWorkSpace;
	}
	if (hModule)
	{
		FreeLibrary(hModule);
	}
	return bRet;
}



//************************************************************
// 函数名称: DataDeCompress
// 函数说明: 数据解压缩
// 作	 者: GuiShou
// 时	 间: 2019/1/26
// 参	 数: BYTE* pCompressData 压缩的数据 DWORD dwCompressDataLength 压缩的数据长度
// 参	 数: BYTE** ppUncompressData 解压缩的数据 DWORD* pdwUncompressDataLength 解压缩的数据长度
// 返 回 值: BOOL 是否成功
//************************************************************

BOOL CCompress::DataDeCompress(BYTE* pCompressData, DWORD dwCompressDataLength, BYTE** ppUncompressData, DWORD* pdwUncompressDataLength)
{
	BOOL bRet = FALSE;
	HMODULE hModule = NULL;
	typedef_RtlDecompressBuffer RtlDecompressBuffer = NULL;
	BYTE *pUncompressData = NULL;
	DWORD dwUncompressDataLength = 4096;
	DWORD dwFinalUncompressSize = 0;
	do
	{
		// 加载 ntdll.dll 
		hModule = ::LoadLibraryA("ntdll.dll");
		if (NULL == hModule)
		{
			m_Tip += " LoadLibrary Error\r\n";
			break;
		}

		// 获取 RtlDecompressBuffer 函数地址
		RtlDecompressBuffer = (typedef_RtlDecompressBuffer)::GetProcAddress(hModule, "RtlDecompressBuffer");
		if (NULL == RtlDecompressBuffer)
		{
			m_Tip += " GetProcAddress Error\r\n";
			break;
		}

		while (TRUE)
		{
			// 申请动态内存
			pUncompressData = new BYTE[dwUncompressDataLength];
			if (NULL == pUncompressData)
			{
				m_Tip += " new Error\r\n";
				break;
			}
			::RtlZeroMemory(pUncompressData, dwUncompressDataLength);

			// 调用RtlCompressBuffer压缩数据
			RtlDecompressBuffer(COMPRESSION_FORMAT_LZNT1, pUncompressData, dwUncompressDataLength, pCompressData, dwCompressDataLength, &dwFinalUncompressSize);
			if (dwUncompressDataLength < dwFinalUncompressSize)
			{
				// 释放内存
				if (pUncompressData)
				{
					delete[]pUncompressData;
					pUncompressData = NULL;
				}
				dwUncompressDataLength = dwFinalUncompressSize;
			}
			else
			{
				break;
			}
		}

		// 返回
		*ppUncompressData = pUncompressData;
		*pdwUncompressDataLength = dwFinalUncompressSize;
		bRet = TRUE;

	} while (FALSE);

	// 释放
	if (hModule)
	{
		::FreeLibrary(hModule);
	}

	return bRet;
}


//************************************************************
// 函数名称: ByteToCString
// 函数说明: 把BYTE* 转为十六进制显示的CString
// 作	 者: GuiShou
// 时	 间: 2019/1/27
// 参	 数: BYTE * str需要转换的BYTE* DWORD dwLength 长度
//************************************************************
CString CCompress::ByteToCString(BYTE* str,DWORD dwLength)
{
	//拷贝压缩之后的数据到数组
	BYTE tempStr[4096] = { 0 };
	memcpy(tempStr, str, dwLength);

	//将解压缩的数据转为十六进制 显示到界面
	CString cStr;
	CString data;
	DWORD i = 0;

	for (i = 0; i < dwLength; i++)
	{
		cStr.Format(L"%02x", tempStr[i]);
		data += cStr;

	}

	return data;
}


//************************************************************
// 函数名称: Zlib_CompressData
// 函数说明: 将文件压缩为数据
// 作	 者: GuiShou
// 时	 间: 2019/1/27
// 参	 数: char * pszCompressFileName 需要压缩的文件完整路径
// 参	 数: BYTE ** ppCompressData 压缩后的数据
// 参	 数: DWORD* pdwCompressDataSize 压缩后的数据大小
//************************************************************
BOOL CCompress::Zlib_CompressData(char* pszCompressFileName, BYTE** ppCompressData, DWORD* pdwCompressDataSize)
{
	//注意可能出现压缩后的文件比压缩前的文件大的现象
	//打开文件 获取数据
	HANDLE hFile = CreateFileA(pszCompressFileName,GENERIC_READ,
	FILE_SHARE_READ|FILE_SHARE_WRITE,NULL,OPEN_EXISTING,FILE_ATTRIBUTE_ARCHIVE,NULL);
	if (hFile==INVALID_HANDLE_VALUE)
	{
		m_Tip += "CreateFileA Error\r\n";
		return FALSE;
	}

	//获取文件大小
	DWORD dwFileSize = GetFileSize(hFile, NULL);
	if (MAX_SRC_FILE_SIZE<dwFileSize)
	{
		m_Tip += "单个文件大小限制为100M！\r\n";
		CloseHandle(hFile);
		return FALSE;
	}

	DWORD dwDestDataSize = dwFileSize;	

	//申请空间存放源文件
	BYTE* pSrcData = new BYTE[dwFileSize]{ 0 };
	if (pSrcData==NULL)
	{
		m_Tip += "new Error\r\n";
		CloseHandle(hFile);
		return FALSE;
	}
	//申请空间存放压缩后的目标文件
	BYTE* pDestData = new BYTE[dwDestDataSize]{ 0 };
	if (pDestData == NULL)
	{
		m_Tip += "new Error\r\n";
		CloseHandle(hFile);
		return FALSE;
	}

	DWORD dwRet = 0;
	//读取源文件到内存
	ReadFile(hFile, pSrcData, dwFileSize, &dwRet, NULL);
	if ((0>=dwRet)||dwRet!=dwFileSize)
	{
		delete[] pSrcData;
		delete[] pDestData;
		CloseHandle(hFile);
		return FALSE;
	}

	//开始压缩数据
	int iRet = 0;
	do 
	{
		iRet = compress(pDestData, &dwDestDataSize, pSrcData, dwFileSize);
		if (0==iRet)
		{
			//成功
			break;
		}
		else if(-5==iRet)
		{
			//输出缓冲区不够大 以100KB大小递增 
			delete[] pDestData;
			dwDestDataSize = dwDestDataSize + (100 * 1024);
			pDestData = new BYTE[dwDestDataSize];
			if (pDestData==NULL)
			{
				delete[] pDestData;
				CloseHandle(hFile);
				return FALSE;
			} 
		}
		else
		{
			//没有足够的内存或者其他情况
			delete[] pDestData;
			delete[] pSrcData;
			CloseHandle(hFile);
			return FALSE;
		}
	} while (TRUE);

	//返回数据
	*ppCompressData = pDestData;
	*pdwCompressDataSize = dwDestDataSize;

	//释放内存
	delete[] pSrcData;
	CloseHandle(hFile);
	return TRUE;
}



//************************************************************
// 函数名称: SaveToFile
// 函数说明: 将数据保存为文件
// 作	 者: GuiShou
// 时	 间: 2019/1/27
// 参	 数: char * pszFileName 文件名
// 参	 数: BYTE * pData 数据
// 参	 数: DWORD dwDataSize 数据大小
//************************************************************
BOOL CCompress::SaveToFile(char * pszFileName, BYTE * pData, DWORD dwDataSize)
{
	char szSaveName[MAX_PATH] = { 0 };
	lstrcpyA(szSaveName,pszFileName);
	//去掉路径前缀
	PathStripPathA(szSaveName);

	//创建文件
	HANDLE hFile = CreateFileA(szSaveName, GENERIC_READ | GENERIC_WRITE,
		FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_ARCHIVE, NULL);
	if (hFile==INVALID_HANDLE_VALUE)
	{
		m_Tip += "CreateFileA\r\n";
		return FALSE;
	}
	//写入数据
	DWORD dwRet = 0;
	WriteFile(hFile, pData, dwDataSize, &dwRet, NULL);
	//关闭句柄
	CloseHandle(hFile);
	return TRUE;

}


//************************************************************
// 函数名称: Zlib_UnCompressData
// 函数说明: 将文件解压缩为数据
// 作	 者: GuiShou
// 时	 间: 2019/1/27
// 参	 数: char * pszUncompressFileName 需要解压缩的文件名
// 参	 数: BYTE ** ppUnCompressData 解压缩后的数据
// 参	 数: pdwUnCompressDataSizee  解压缩后的数据大小
//************************************************************
BOOL CCompress::Zlib_UnCompressData(char * pszUncompressFileName, BYTE ** ppUnCompressData, DWORD* pdwUnCompressDataSize)
{
	//打开文件并获取数据
	HANDLE hFile = CreateFileA(pszUncompressFileName, GENERIC_READ,
		FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_ARCHIVE, NULL);
	if (hFile == INVALID_HANDLE_VALUE)
	{
		m_Tip += "CreateFileA Error\r\n";
		return FALSE;
	}

	//获取文件大小
	DWORD dwFileSize = GetFileSize(hFile, NULL);
	DWORD dwDestDataSize = MAX_SRC_FILE_SIZE;	//目标文件大小

	//根据源文件大小申请空间
	BYTE* pSrcData = new BYTE[dwFileSize]{ 0 };
	if (pSrcData==NULL)
	{
		CloseHandle(hFile);
		return FALSE;
	}

	//根据目标文件大小申请空间
	BYTE* pDestData = new BYTE[dwDestDataSize]{ 0 };
	if (pDestData == NULL)
	{
		CloseHandle(hFile);
		return FALSE;
	}

	DWORD dwRet = 0;
	//读取需要解压缩的文件的到内存
	ReadFile(hFile, pSrcData, dwFileSize, &dwRet, NULL);
	if ((0>=dwRet)||dwRet!=dwFileSize)
	{
		delete[]pDestData;
		delete[]pSrcData;
		::CloseHandle(hFile);
		return FALSE;
	}
	
	//解压缩数据
	int iRet = 0;
	do 
	{
		iRet = uncompress(pDestData, &dwDestDataSize, pSrcData, dwFileSize);
		if (0==iRet)
		{
			//成功
			break;
		}
		else if (-5==iRet)
		{
			//输出缓冲区不够大 以100KB大小递增
			delete[]pDestData;
			pDestData = NULL;
			dwDestDataSize = dwDestDataSize + (100 * 1024);
			pDestData = new BYTE[dwDestDataSize];
			if (NULL == pDestData)
			{
				delete[]pSrcData;
				pSrcData = NULL;
				::CloseHandle(hFile);
				return FALSE;
			}
		}
		else
		{
			//没有足够的内存 或其他情况
			delete[]pDestData;
			pDestData = NULL;
			delete[]pSrcData;
			pSrcData = NULL;
			::CloseHandle(hFile);
			return FALSE;
		}
	} while (TRUE);

	//返回数据
	*ppUnCompressData = pDestData;
	*pdwUnCompressDataSize = dwDestDataSize;

	//释放内存
	delete[]pSrcData;
	pSrcData = NULL;
	::CloseHandle(hFile);

	return TRUE;
}


