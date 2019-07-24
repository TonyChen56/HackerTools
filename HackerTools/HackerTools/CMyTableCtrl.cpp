// CMyTableCtrl.cpp: 实现文件
//

#include "stdafx.h"
#include "HackerTools.h"
#include "CMyTableCtrl.h"


// CMyTableCtrl

IMPLEMENT_DYNAMIC(CMyTableCtrl, CTabCtrl)

CMyTableCtrl::CMyTableCtrl()
{

}

CMyTableCtrl::~CMyTableCtrl()
{
}


BEGIN_MESSAGE_MAP(CMyTableCtrl, CTabCtrl)
	ON_NOTIFY_REFLECT(TCN_SELCHANGE, &CMyTableCtrl::OnTcnSelchange)
END_MESSAGE_MAP()



// CMyTableCtrl 消息处理程序




void CMyTableCtrl::OnTcnSelchange(NMHDR *pNMHDR, LRESULT *pResult)
{
	//当前选项卡的序号
	int nNum = GetCurSel();
	switch (nNum)
	{
	case 0:
	{
		m_Dia[0]->ShowWindow(SW_SHOW);
		m_Dia[1]->ShowWindow(SW_HIDE);
	}
	break;
	case 1:
	{
		m_Dia[0]->ShowWindow(SW_HIDE);
		m_Dia[1]->ShowWindow(SW_SHOW);
	}
	break;
	default:
		break;
	}
	*pResult = 0;
}
