// EditEx.cpp : implementation file
//

#include "stdafx.h"
#include "EditEx.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CEditEx

CEditEx::CEditEx(): m_bAutoSel(TRUE), m_bAlphaDigitOnly(FALSE)
{}

CEditEx::~CEditEx()
{}


BEGIN_MESSAGE_MAP(CEditEx, CEdit)
	//{{AFX_MSG_MAP(CEditEx)
	ON_WM_LBUTTONDOWN()
	ON_WM_RBUTTONDOWN()
	ON_WM_CHAR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CEditEx message handlers

void CEditEx::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	BOOL bAutoSel = m_bAutoSel && GetFocus() != this;
	CEdit::OnLButtonDown(nFlags, point);

	if (bAutoSel)
		SetSel(0, -1); // highlights all texts
}

void CEditEx::OnRButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	BOOL bAutoSel = m_bAutoSel && GetFocus() != this;
	CEdit::OnRButtonDown(nFlags, point);

	if (bAutoSel)
		SetSel(0, -1); // highlights all texts	
}

void CEditEx::EnableAutoSel(BOOL bEnable)
{
	m_bAutoSel = bEnable;
}

void CEditEx::SetAlphaDigitOnly(BOOL bAlphaDigitOnly)
{
	m_bAlphaDigitOnly = bAlphaDigitOnly;
}

void CEditEx::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	// TODO: Add your message handler code here and/or call default
	if (m_bAlphaDigitOnly)
	{
		if (nChar == '.' || (!isdigit(nChar) && !isalpha(nChar)
			&& nChar != VK_BACK && nChar != VK_DELETE))
			return;
	}
	CEdit::OnChar(nChar, nRepCnt, nFlags);
}
