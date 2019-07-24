#include "stdafx.h"
#include "HackerTools.h"
#include "CEncrypt.h"
#include "afxdialogex.h"


// CEncrypt 对话框

IMPLEMENT_DYNAMIC(CEncrypt, CDialogEx)

CEncrypt::CEncrypt(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_Encrypt, pParent)
	, m_Tip(_T(""))
	, m_Data(_T(""))
	, m_Path(_T(""))
{

}

CEncrypt::~CEncrypt()
{
}

void CEncrypt::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT3, m_Tip);
	DDX_Text(pDX, IDC_EDIT2, m_Data);
	DDX_Text(pDX, IDC_EDIT1, m_Path);
}


BEGIN_MESSAGE_MAP(CEncrypt, CDialogEx)
	ON_WM_DROPFILES()
	ON_BN_CLICKED(IDC_HASH, &CEncrypt::OnBnClickedHash)
	ON_BN_CLICKED(IDC_AES, &CEncrypt::OnBnClickedAes)
	ON_BN_CLICKED(IDC_RSA, &CEncrypt::OnBnClickedRsa)
END_MESSAGE_MAP()


// CEncrypt 消息处理程序


void CEncrypt::OnDropFiles(HDROP hDropInfo)
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


BOOL CEncrypt::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	//管理员模式下取消过滤拖拽消息
	ChangeWindowMessageFilterEx(m_hWnd, WM_DROPFILES, MSGFLT_ALLOW, NULL);
	ChangeWindowMessageFilterEx(m_hWnd, 0x0049, MSGFLT_ALLOW, NULL);
	

	return TRUE;  
}


//************************************************************
// 函数名称: OnBnClickedHash
// 函数说明: HASH按钮
// 作	 者: GuiShou
// 时	 间: 2019/1/27
// 参	 数: void
// 返 回 值: void
//************************************************************
void CEncrypt::OnBnClickedHash()
{
	//检测文件是否存在
	if (GetFileAttributes(m_Path)==INVALID_FILE_ATTRIBUTES)
	{
		MessageBox(L"文件不存在！");
		return;
	}
	//清空数据
	m_Data = "";

	BYTE* pData = NULL;
	DWORD dwDataLength = 0;
	BYTE* pHashData = NULL;
	DWORD dwHashLength = 0;

	//读取文件数据
	USES_CONVERSION;
	GetFileData(W2A(m_Path), &pData, &dwDataLength);

	//计算MD5值
	CalcHash(pData, dwDataLength, CALG_MD5, &pHashData, &dwHashLength);
	//显示到界面
	CString Temp;
	Temp = ByteToCString(pHashData, dwHashLength);
	m_Data += "MD5:\r\n";
	m_Data += Temp;
	m_Data += "\r\n";

	//计算SHA1值
	CalcHash(pData, dwDataLength, CALG_SHA1, &pHashData, &dwHashLength);
	//显示到界面
	Temp = ByteToCString(pHashData, dwHashLength);
	m_Data += "SHA1:\r\n";
	m_Data += Temp;
	m_Data += "\r\n";
	//计算SHA256值
	CalcHash(pData, dwDataLength, CALG_SHA_256, &pHashData, &dwHashLength);
	//显示到界面
	Temp = ByteToCString(pHashData, dwHashLength);
	m_Data += "SHA256:\r\n";
	m_Data += Temp;
	m_Data += "\r\n";
	UpdateData(FALSE);
}


//************************************************************
// 函数名称: GetFileData
// 函数说明: 获取文件数据
// 作	 者: GuiShou
// 时	 间: 2019/1/27
// 参	 数: char * pszFilePath 文件路径 BYTE** pFileData 文件数据首地址
// 参	 数: DWORD * pdwFileDataLength 文件数据长度
// 返 回 值: BOOL 是否成功
//************************************************************
BOOL CEncrypt::GetFileData(char * pszFilePath, BYTE** ppFileData, DWORD * pdwFileDataLength)
{
	BOOL bRet = TRUE;
	BYTE* pFileData = NULL;
	DWORD dwFileDataLength = 0;
	HANDLE hFile = NULL;
	DWORD dwTemp = 0;

	do 
	{
		//打开文件
		hFile = CreateFileA(pszFilePath, GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE,
			NULL, OPEN_EXISTING, FILE_ATTRIBUTE_ARCHIVE, NULL);
		if (hFile==INVALID_HANDLE_VALUE)
		{
			bRet = FALSE;
			m_Tip += "CreateFileA Error\r\n";
			break;
		}

		//获取文件大小
		dwFileDataLength = GetFileSize(hFile, NULL);
		//根据文件大小申请空间
		pFileData = new BYTE[dwFileDataLength]{ 0 };
		if (pFileData==NULL)
		{
			bRet = FALSE;
			m_Tip += "new Error\r\n";
			break;
		}

		//读取文件到申请的空间
		ReadFile(hFile, pFileData, dwFileDataLength, &dwTemp, NULL);

		//返回数据
		*ppFileData = pFileData;
		*pdwFileDataLength = dwFileDataLength;

	} while (FALSE);

	if (hFile)
	{
		CloseHandle(hFile);
	}
	return bRet;
}


//************************************************************
// 函数名称: CalcHash
// 函数说明: 计算哈希值
// 作	 者: GuiShou
// 时	 间: 2019/1/27
// 参	 数: BYTE * pData,需要计算哈希值的数据  DWORD dwDataLength ,需要计算哈希值的数据长度
// 参	 数: ALG_ID algHashType 需要计算哈希值的类型 BYTE** ppHashData,计算出来的哈希数据  
// 参	 数: DWORD * pdwHashDataLength 计算出来的哈希数据长度
// 返 回 值: BOOL 是否成功
//************************************************************
BOOL CEncrypt::CalcHash(BYTE* pData, DWORD dwDataLength, ALG_ID algHashType, BYTE ** ppHashData, DWORD * pdwHashDataLength)
{
	HCRYPTPROV hCryptProv = NULL;
	HCRYPTHASH hCryptHash = NULL;
	BYTE* pHashData = NULL;
	DWORD dwHashDataLength = 0;
	DWORD dwTemp = 0;
	BOOL bRet = FALSE;

	do 
	{
		//获取指定CSP密钥容器的句柄
		bRet = CryptAcquireContext(&hCryptProv, NULL, NULL, PROV_RSA_AES, CRYPT_VERIFYCONTEXT);
		if (bRet==FALSE)
		{
			m_Tip += "CryptAcquireContext Error\r\n";
			break;
		}

		//创建一个HASH对象 指定HASH算法
		bRet = CryptCreateHash(hCryptProv, algHashType, NULL, NULL, &hCryptHash);
		if (bRet == FALSE)
		{
			m_Tip += "CryptCreateHash Error\r\n";
			break;
		}

		//计算HASH数据
		bRet = CryptHashData(hCryptHash, pData, dwDataLength, 0);
		if (bRet == FALSE)
		{
			m_Tip += "CryptHashData Error\r\n";
			break;
		}

		//获取HASH结果的大小
		dwTemp = sizeof(dwHashDataLength);
		bRet = CryptGetHashParam(hCryptHash, HP_HASHSIZE, (BYTE*)(&dwHashDataLength), &dwTemp, 0);
		if (bRet == FALSE)
		{
			m_Tip += "CryptGetHashParam Error\r\n";
			break;
		}

		//根据获取的大小申请内存 存放哈希数据
		pHashData = new BYTE[dwHashDataLength]{ 0 };
		if (pHashData==NULL)
		{
			bRet = FALSE;
			m_Tip += "new Error\r\n";
			break;
		}

		//获取哈希结果数据
		bRet = CryptGetHashParam(hCryptHash, HP_HASHVAL, pHashData, &dwHashDataLength, 0);
		if (bRet == FALSE)
		{
			m_Tip += "CryptGetHashParam Error\r\n";
			break;
		}

		//返回数据
		*ppHashData = pHashData;
		*pdwHashDataLength = dwHashDataLength;
	} while (FALSE);

	//关闭句柄
	if (bRet==FALSE)
	{
		if (pHashData)
		{
			delete[] pHashData;
		}
	}
	if (hCryptHash)
	{
		CryptDestroyHash(hCryptHash);
	}
	if (hCryptProv)
	{
		CryptReleaseContext(hCryptProv, 0);
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
CString CEncrypt::ByteToCString(BYTE * str, DWORD dwLength)
{
	//拷贝压缩之后的数据到数组
	BYTE tempStr[4096] = { 0 };
	memcpy(tempStr, str, dwLength);

	//将解压缩的数据转为十六进制 显示到界面
	CString cStr;
	CString data;
	DWORD i = 0;

	for (i=0;i< dwLength;i++)
	{
		cStr.Format(L"%02x", tempStr[i]);
		data += cStr;

	}
	return data;
}


//************************************************************
// 函数名称: OnBnClickedAes
// 函数说明: AES加解密按钮
// 作	 者: GuiShou
// 时	 间: 2019/1/27
// 参	 数: void
// 返 回 值: void
//************************************************************
void CEncrypt::OnBnClickedAes()
{
	CString temp;
	BYTE *pData = NULL;
	DWORD dwDataLength = 0;
	DWORD dwBufferLength = 4096;
	UpdateData(TRUE);

	//将CString转为BYTE*
	USES_CONVERSION;
	pData = (BYTE*)W2A(m_Data);
	dwDataLength = ::lstrlenA((LPCSTR)pData);

	m_Data = "";
	//显示原数据
	temp = ByteToCString(pData, dwDataLength);
	m_Data += "原数据为:\r\n";
	m_Data += temp;
	m_Data += "\r\n";

	//AES加密
	AesEncrypt((BYTE*)"GuiShou", 6, pData, dwDataLength, dwBufferLength);
	//显示加密的数据
	temp = ByteToCString(pData, dwDataLength);
	m_Data += "AES加密后的数据为:\r\n";
	m_Data += temp;
	m_Data += "\r\n";

	//AES解密
	AesDecrypt((BYTE*)"GuiShou", 6, pData, dwDataLength, dwBufferLength);
	//显示解密的数据
	temp = ByteToCString(pData, dwDataLength);
	m_Data += "AES解密后的数据为:\r\n";
	m_Data += temp;
	m_Data += "\r\n";

	UpdateData(FALSE);
}



//************************************************************
// 函数名称: AesEncrypt
// 函数说明: AES加密
// 作	 者: GuiShou
// 时	 间: 2019/1/27
// 参	 数: BYTE * pPassword 密钥 DWORD dwPasswordLength 密钥长度
// 参	 数: BYTE* pData 需要AES加密的数据 DWORD& dwDataLength 需要AES加密的数据长度
// 参	 数: DWORD dwBufferLength 缓冲区长度
// 返 回 值: BOOL是否成功
//************************************************************
BOOL CEncrypt::AesEncrypt(BYTE * pPassword, DWORD dwPasswordLength, BYTE * pData, DWORD & dwDataLength, DWORD dwBufferLength)
{
	BOOL bRet = TRUE;
	HCRYPTPROV hCryptProc = NULL;
	HCRYPTHASH hCryptHash = NULL;
	HCRYPTKEY hCryptKey = NULL;

	do 
	{
		//获取CSP句柄
		bRet = CryptAcquireContext(&hCryptProc, NULL, NULL, PROV_RSA_AES, CRYPT_VERIFYCONTEXT);
		if (bRet==FALSE)
		{
			m_Tip += "CryptAcquireContext Error\r\n";
			break;
		}

		//创建HASH对象
		bRet = CryptCreateHash(hCryptProc, CALG_MD5, NULL, 0, &hCryptHash);
		if (bRet==FALSE)
		{
			m_Tip += "CryptCreateHash Error\r\n";
			break;
		}

		//对密钥进行HASH计算 计算出密钥的MD5值
		bRet = CryptHashData(hCryptHash, pPassword, dwPasswordLength, 0);
		if (bRet == FALSE)
		{
			m_Tip += "CryptHashData Error\r\n";
			break;
		}

		//使用HASH来生成密钥 返回密钥句柄
		bRet = CryptDeriveKey(hCryptProc, CALG_AES_128, hCryptHash, CRYPT_EXPORTABLE, &hCryptKey);
		if (bRet == FALSE)
		{
			m_Tip += "CryptDeriveKey Error\r\n";
			break;
		}

		//加密数据
		bRet = CryptEncrypt(hCryptKey, NULL, TRUE, 0, pData, &dwDataLength, dwBufferLength);
		if (bRet == FALSE)
		{
			m_Tip += "CryptEncrypt Error\r\n";
			break;
		}
	} while (FALSE);

	//关闭句柄 释放资源
	if (hCryptKey)
	{
		CryptDestroyKey(hCryptKey);
	}
	if (hCryptHash)
	{
		CryptDestroyHash(hCryptHash);
	}
	if (hCryptProc)
	{
		CryptReleaseContext(hCryptProc, 0);
	}
	return bRet;
}


//************************************************************
// 函数名称: AesDecrypt
// 函数说明: AES解密
// 作	 者: GuiShou
// 时	 间: 2019/1/27
// 参	 数: BYTE * pPassword 密钥 DWORD dwPasswordLength 密钥长度
// 参	 数: BYTE* pData 需要AES加密的数据 DWORD& dwDataLength 需要AES加密的数据长度
// 参	 数: DWORD dwBufferLength 缓冲区长度
// 返 回 值: BOOL是否成功
//************************************************************
BOOL CEncrypt::AesDecrypt(BYTE * pPassword, DWORD dwPasswordLength, BYTE * pData, DWORD & dwDataLength, DWORD dwBufferLength)
{
	BOOL bRet = TRUE;
	HCRYPTPROV hCryptProc = NULL;
	HCRYPTHASH hCryptHash = NULL;
	HCRYPTKEY hCryptKey = NULL;

	do
	{
		//获取CSP句柄
		bRet = CryptAcquireContext(&hCryptProc, NULL, NULL, PROV_RSA_AES, CRYPT_VERIFYCONTEXT);
		if (bRet == FALSE)
		{
			m_Tip += "CryptAcquireContext Error\r\n";
			break;
		}

		//创建HASH对象
		bRet = CryptCreateHash(hCryptProc, CALG_MD5, NULL, 0, &hCryptHash);
		if (bRet == FALSE)
		{
			m_Tip += "CryptCreateHash Error\r\n";
			break;
		}

		//对密钥进行HASH计算 计算出密钥的MD5值
		bRet = CryptHashData(hCryptHash, pPassword, dwPasswordLength, 0);
		if (bRet == FALSE)
		{
			m_Tip += "CryptHashData Error\r\n";
			break;
		}

		//使用HASH来生成密钥 返回密钥句柄
		bRet = CryptDeriveKey(hCryptProc, CALG_AES_128, hCryptHash, CRYPT_EXPORTABLE, &hCryptKey);
		if (bRet == FALSE)
		{
			m_Tip += "CryptDeriveKey Error\r\n";
			break;
		}

		//解密数据
		bRet = CryptDecrypt(hCryptKey, NULL, TRUE, 0, pData, &dwDataLength);
		if (bRet == FALSE)
		{
			m_Tip += "CryptDecrypt Error\r\n";
			break;
		}
	} while (FALSE);

	//关闭句柄 释放资源
	if (hCryptKey)
	{
		CryptDestroyKey(hCryptKey);
	}
	if (hCryptHash)
	{
		CryptDestroyHash(hCryptHash);
	}
	if (hCryptProc)
	{
		CryptReleaseContext(hCryptProc, 0);
	}
	return bRet;
}


//************************************************************
// 函数名称: OnBnClickedRsa
// 函数说明: RSA加解密按钮
// 作	 者: GuiShou
// 时	 间: 2019/1/27
// 参	 数: void
// 返 回 值: void
//************************************************************
void CEncrypt::OnBnClickedRsa()
{
	UpdateData(TRUE);
	CString temp;
	BYTE* pPublicKey = NULL;		//公钥
	DWORD dwPublicKeyLength = 0;	//公钥长度
	BYTE* pPrivateKey = NULL;		//私钥
	DWORD dwPrivateKeyLength = 0;	//私钥长度
	BYTE* pData = NULL;				//需要加解密的数据
	BYTE* pTempData = NULL;
	DWORD dwDataLength = 0;			//需要加解密的数据长度
	DWORD dwBufferLength = 4096;	//缓冲区长度
	
	//将CString转为BYTE*
	USES_CONVERSION;
	pTempData = (BYTE*)W2A(m_Data);
	dwDataLength = ::lstrlenA((LPCSTR)pTempData);

	//将数据拷贝到堆空间 否则会导致堆栈破坏
	pData = new BYTE[4096];
	memset(pData, 0, 4096);
	memcpy(pData, pTempData, dwDataLength);

	//清空显示的数据
	m_Data = "";

	//显示原数据
	temp = ByteToCString(pTempData, dwDataLength);
	m_Data += "原数据为:\r\n";
	m_Data += temp;
	m_Data += "\r\n";

	//生成公钥和私钥
	GenerateKey(&pPublicKey, &dwPublicKeyLength, &pPrivateKey, &dwPrivateKeyLength);
	//显示公钥
	temp = ByteToCString(pPublicKey, dwPublicKeyLength);
	m_Data += "公钥:\r\n";
	m_Data += temp;
	m_Data += "\r\n";
	//显示私钥
	temp = ByteToCString(pPrivateKey, dwPrivateKeyLength);
	m_Data += "私钥:\r\n";
	m_Data += temp;
	m_Data += "\r\n";
	
	//公钥加密
	RsaEncrypt(pPublicKey, dwPublicKeyLength, pData, dwDataLength, dwBufferLength);
	//显示加密后的数据
	temp = ByteToCString(pData, dwDataLength);
	m_Data += "RSA加密后的数据为:\r\n";
	m_Data += temp;
	m_Data += "\r\n";
	
	//私钥解密
	RsaDecrypt(pPrivateKey, dwPrivateKeyLength, pData, dwDataLength);
	//显示解密后的数据
	temp = ByteToCString(pData, dwDataLength);
	m_Data += "RSA解密后的数据为:\r\n";
	m_Data += temp;
	m_Data += "\r\n";
	

	UpdateData(FALSE);
}



//************************************************************
// 函数名称: GenerateKey
// 函数说明: 生成公钥和私钥
// 作	 者: GuiShou
// 时	 间: 2019/1/27
// 参	 数: BYTE** ppPublicKey 公钥 DWORD* pdwPublicKeyLength 公钥长度
// 参	 数: BYTE** ppPrivateKey 私钥 DWORD* pdwPrivateKeyLength 私钥长度
// 返 回 值: BOOL 是否成功
//************************************************************
BOOL CEncrypt::GenerateKey(BYTE** ppPublicKey, DWORD* pdwPublicKeyLength, BYTE** ppPrivateKey, DWORD* pdwPrivateKeyLength)
{
	BOOL bRet = TRUE;
	HCRYPTPROV hCryptProv = NULL;
	HCRYPTKEY hCryptKey = NULL;
	BYTE* pPublicKey = NULL;
	DWORD dwPublicKeyLength = 0;
	BYTE* pPrivateKey = NULL;
	DWORD dwPrivateKeyLeng = 0;

	do 
	{
		//获取CSP密钥句柄
		bRet = CryptAcquireContext(&hCryptProv, NULL, NULL, PROV_RSA_FULL, 0);
		if (bRet==FALSE)
		{
			m_Tip += "CryptAcquireContext Error\r\n";
			break;
		}

		//生成公私密钥对
		bRet = CryptGenKey(hCryptProv, AT_KEYEXCHANGE, CRYPT_EXPORTABLE, &hCryptKey);
		if (bRet == FALSE)
		{
			m_Tip += "CryptGenKey Error\r\n";
			break;
		}

		//获取公钥长度
		bRet = CryptExportKey(hCryptKey, NULL, PUBLICKEYBLOB, 0, NULL, &dwPublicKeyLength);
		if (bRet == FALSE)
		{
			m_Tip += "CryptExportKey Error\r\n";
			break;
		}
		//根据公钥长度申请空间
		pPublicKey = new BYTE[dwPublicKeyLength]{ 0 };
		//获取公钥
		bRet = CryptExportKey(hCryptKey, NULL, PUBLICKEYBLOB, 0, pPublicKey, &dwPublicKeyLength);
		if (bRet == FALSE)
		{
			m_Tip += "CryptExportKey Error\r\n";
			break;
		}

		//获取私钥的长度
		bRet = CryptExportKey(hCryptKey, NULL, PRIVATEKEYBLOB, 0, NULL, &dwPrivateKeyLeng);
		if (bRet == FALSE)
		{
			m_Tip += "CryptExportKey Error\r\n";
			break;
		}
		//根据私钥长度申请空间
		pPrivateKey = new BYTE[dwPrivateKeyLeng]{ 0 };
		//获取私钥
		bRet = CryptExportKey(hCryptKey, NULL, PRIVATEKEYBLOB, 0, pPrivateKey, &dwPrivateKeyLeng);
		if (bRet == FALSE)
		{
			m_Tip += "CryptExportKey Error\r\n";
			break;
		}

		//返回数据
		*ppPublicKey = pPublicKey;
		*pdwPublicKeyLength = dwPublicKeyLength;
		*ppPrivateKey = pPrivateKey;
		*pdwPrivateKeyLength = dwPrivateKeyLeng;
	} while (FALSE);

	//关闭句柄
	if (hCryptKey)
	{
		CryptDestroyKey(hCryptKey);
	}
	if (hCryptProv)
	{
		CryptReleaseContext(hCryptProv, 0);
	}
	return bRet;
}



//************************************************************
// 函数名称: RsaEncrypt
// 函数说明: RAS加密
// 作	 者: GuiShou
// 时	 间: 2019/1/27
// 参	 数: BYTE* pPublicKey,公钥 DWORD dwPublicKeyLength 公钥长度
// 参	 数: BYTE* pData 需要加密的数据 DWORD& dwDataLength 需要加密的数据长度 
// 参	 数: DWORD dwBufferLength 缓冲区长度
// 返 回 值: BOOL 是否成功
//************************************************************
BOOL CEncrypt::RsaEncrypt(BYTE* pPublicKey, DWORD dwPublicKeyLength, BYTE* pData, DWORD& dwDataLength, DWORD dwBufferLength)
{
	BOOL bRet = TRUE;
	HCRYPTPROV hCryptProv = NULL;
	HCRYPTKEY hCryptKey = NULL;

	do 
	{
		//获取CSP句柄
		bRet = CryptAcquireContext(&hCryptProv, NULL, NULL, PROV_RSA_FULL, 0);
		if (bRet == FALSE)
		{
			m_Tip += "CryptAcquireContext Error\r\n";
			break;
		}

		//导入公钥
		bRet = CryptImportKey(hCryptProv, pPublicKey, dwPublicKeyLength, NULL, 0, &hCryptKey);
		if (bRet == FALSE)
		{
			m_Tip += "CryptImportKey Error\r\n";
			break;
		}

		//加密数据
		bRet = CryptEncrypt(hCryptKey, NULL, TRUE, 0, pData, &dwDataLength, dwBufferLength);
		if (bRet == FALSE)
		{
			m_Tip += "CryptEncrypt Error\r\n";
			break;
		}

	} while (FALSE);

	//关闭句柄
	if (hCryptKey)
	{
		CryptDestroyKey(hCryptKey);
	}
	if (hCryptProv)
	{
		CryptReleaseContext(hCryptProv, 0);
	}
	return bRet;
}


//************************************************************
// 函数名称: RsaDecrypt
// 函数说明: RAS解密
// 作	 者: GuiShou
// 时	 间: 2019/1/27
// 参	 数: BYTE* pPrivateKey 私钥 DWORD dwPrivateKeyLength 私钥长度
// 参	 数: BYTE* pData 需要解密的数据 DWORD& dwDataLength 需要解密的数据长度 
// 返 回 值: BOOL 是否成功
BOOL CEncrypt::RsaDecrypt(BYTE* pPrivateKey, DWORD dwPrivateKeyLength, BYTE * pData, DWORD & dwDataLength)
{
	BOOL bRet = TRUE;
	HCRYPTPROV hCryptProv = NULL;
	HCRYPTKEY hCryptKey = NULL;

	do
	{
		//获取CSP句柄
		bRet = CryptAcquireContext(&hCryptProv, NULL, NULL, PROV_RSA_FULL, 0);
		if (bRet == FALSE)
		{
			m_Tip += "CryptAcquireContext Error\r\n";
			break;
		}

		//导入私钥
		bRet = CryptImportKey(hCryptProv, pPrivateKey, dwPrivateKeyLength, NULL, 0, &hCryptKey);
		if (bRet == FALSE)
		{
			m_Tip += "CryptImportKey Error\r\n";
			break;
		}

		//解密数据
		bRet = CryptDecrypt(hCryptKey, NULL, TRUE, 0, pData, &dwDataLength);
		if (bRet == FALSE)
		{
			m_Tip += "CryptDecrypt Error\r\n";
			break;
		}

	} while (FALSE);

	//关闭句柄
	if (hCryptKey)
	{
		CryptDestroyKey(hCryptKey);
	}
	if (hCryptProv)
	{
		CryptReleaseContext(hCryptProv, 0);
	}
	return bRet;
}
