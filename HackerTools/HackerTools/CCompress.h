#pragma once
#include <Shlwapi.h>
#pragma comment(lib, "Shlwapi.lib")
//*************************************************
//         zlib压缩库的头文件和静态库
//*************************************************
#include "zlib\\zconf.h"
#include "zlib\\zlib.h"
#ifdef _DEBUG
#ifdef _WIN64
#pragma comment(lib, "zlib\\x64\\debug\\zlibstat.lib")
#else
#pragma comment(lib, "zlib\\x86\\debug\\zlibstat.lib")
#endif
#else
#ifdef _WIN64
#pragma comment(lib, "zlib\\x64\\release\\zlibstat.lib")
#else
#pragma comment(lib, "zlib\\x86\\release\\zlibstat.lib")
#endif
#endif
//*************************************************

#define MAX_SRC_FILE_SIZE (100*1024*1024)			// 单个文件限制大小为 100M 

// CCompress 对话框

class CCompress : public CDialogEx
{
	DECLARE_DYNAMIC(CCompress)

public:
	CCompress(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CCompress();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_Compress };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnDropFiles(HDROP hDropInfo);
	CString m_Data;
	CString m_Path;
	afx_msg void OnBnClickedDatacompress();
	afx_msg void OnBnClickedFilecompress();
	CString m_Tip;


	//数据压缩
	BOOL DataCompress(BYTE* pUncompressData, DWORD dwUncompressDataLength, BYTE** ppCompressData, DWORD* pdwCompressDataLength);

	//数据解压缩
	BOOL DataDeCompress(BYTE *pCompressData, DWORD dwCompressDataLength, BYTE **ppUncompressData, DWORD *pdwUncompressDataLength);

	//BYTE*转CString
	CString ByteToCString(BYTE* str,DWORD dwLength);

	//将文件压缩为数据
	BOOL Zlib_CompressData(char* pszCompressFileName, BYTE** ppCompressData, DWORD* pdwCompressDataSize);

	//将数据保存为文件
	BOOL SaveToFile(char* pszFileName, BYTE* pData, DWORD dwDataSize);

	//将文件解压缩为数据
	BOOL Zlib_UnCompressData(char* pszUncompressFileName,BYTE** ppUnCompressData,DWORD* pdwUnCompressDataSize);
};
