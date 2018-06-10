// MyListCtrl.cpp : implementation file
//

#include "stdafx.h"
#include "pindlebot.h"
#include "MyListCtrl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMyListCtrl

CMyListCtrl::CMyListCtrl(): m_nCurSel(-1), m_pIni(NULL)
{
}

CMyListCtrl::~CMyListCtrl()
{
}


BEGIN_MESSAGE_MAP(CMyListCtrl, CListCtrl)
	//{{AFX_MSG_MAP(CMyListCtrl)
	ON_WM_LBUTTONDOWN()
	ON_WM_RBUTTONDOWN()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMyListCtrl message handlers

void CMyListCtrl::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	CListCtrl::OnLButtonDown(nFlags, point);
	UINT nFlag = Select(point);

	if (nFlag == 8 && m_nCurSel != -1)
		PickUp(this->GetCheck(m_nCurSel));
}

void CMyListCtrl::OnRButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	CListCtrl::OnRButtonDown(nFlags, point);
	UINT nFlag = Select(point);
	if (nFlag == 8 && m_nCurSel != -1)
		PickUp(this->GetCheck(m_nCurSel));
}

int CMyListCtrl::Select(POINT pt)
{
	UINT nFlag;
	int nIndex = this->HitTest(pt, &nFlag);
	if (nFlag == 8 && nIndex != -1)
	{
		m_nCurSel = nIndex;
		SetItemState(nIndex, LVIS_SELECTED | LVIS_FOCUSED , LVIS_SELECTED | LVIS_FOCUSED);
	}
	return nFlag;
}

void CMyListCtrl::SetIni(CIniFile *pIni)
{
	m_pIni = pIni;
}

void CMyListCtrl::PickUp(BOOL bPickup)
{
	if (bPickup)
	{
		// disable to enable, set pickup=1
		CString sItem = GetItemText(m_nCurSel, 0);
		m_pIni->WriteProfileInt(sItem, _T("Pickup"), 1);
		//MessageBox(CString(_T("已设定物品\"")) + sItem + _T("\"为允许拾取。\n改动将在下一次启动Diablo II时开始生效。"), _T("允许物品拾取"), MB_ICONINFORMATION);
	}
	else
	{
		// enable to disable, set pickup=0
		CString sItem = GetItemText(m_nCurSel, 0);
		m_pIni->WriteProfileInt(sItem, _T("Pickup"), 0);
		//MessageBox(CString(_T("已设定物品\"")) + sItem + _T("\"为禁止拾取。\n改动将在下一次启动Diablo II时开始生效。"), _T("禁止物品拾取"), MB_ICONINFORMATION);
	}
}
