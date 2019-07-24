#pragma once
#include <Wincrypt.h>


// CEncrypt 对话框

class CEncrypt : public CDialogEx
{
	DECLARE_DYNAMIC(CEncrypt)

public:
	CEncrypt(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CEncrypt();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_Encrypt };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnDropFiles(HDROP hDropInfo);
	virtual BOOL OnInitDialog();
	CString m_Tip;
	CString m_Data;
	CString m_Path;
	afx_msg void OnBnClickedHash();

	//获取文件数据
	BOOL GetFileData(char* pszFilePath, BYTE** ppFileData, DWORD* pdwFileDataLength);

	//计算HASH值
	BOOL CalcHash(BYTE* pData,DWORD dwDataLength,ALG_ID algHashType,BYTE** ppHashData,DWORD* pdwHashDataLength);

	//BYTE*转CString
	CString ByteToCString(BYTE* str, DWORD dwLength);
	afx_msg void OnBnClickedAes();

	//AES加密
	BOOL AesEncrypt(BYTE* pPassword, DWORD dwPasswordLength, BYTE* pData, DWORD& dwDataLength, DWORD dwBufferLength);

	//AES解密
	BOOL AesDecrypt(BYTE* pPassword, DWORD dwPasswordLength, BYTE* pData, DWORD& dwDataLength, DWORD dwBufferLength);

	afx_msg void OnBnClickedRsa();

	//生成公钥和私钥
	BOOL GenerateKey(BYTE** ppPublicKey, DWORD* pdwPublicKeyLength, BYTE** ppPrivateKey, DWORD* pdwPrivateKeyLength);

	//公钥加密数据
	BOOL RsaEncrypt(BYTE* pPublicKey, DWORD dwPublicKeyLength, BYTE* pData, DWORD& dwDataLength, DWORD dwBufferLength);

	//私钥解密数据
	BOOL RsaDecrypt(BYTE* pPrivateKey,DWORD dwPrivateKeyLength,BYTE* pData,DWORD& dwDataLength);
};
