// HSListBox.cpp : implementation file
//

#include "stdafx.h"
#include "HSListBox.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CHSListBox

CHSListBox::CHSListBox():m_bAutoScroll(FALSE), m_iMaxWidth(0)
{}

CHSListBox::~CHSListBox()
{}

void CHSListBox::SetAutoScroll(BOOL bAutoScroll)
{
	m_bAutoScroll = bAutoScroll;
}

int CHSListBox::AddString(LPCTSTR lpszItem)
{
	// call parent first
	int nRet = CListBox::AddString(lpszItem);
	m_iMaxWidth = max(m_iMaxWidth, _GetListStringWidth(lpszItem));

	// set scroll range, it should be:
	// string_width + left_margin + right_margin
	// where left_margin and right_margin are usually 3 pixels each
	SetHorizontalExtent(m_iMaxWidth + 3 + 3);

	// if m_bAutoScroll is TRUE, automatically scroll to index
	// of the newest item
	if (m_bAutoScroll)
		SetTopIndex(nRet);
	return nRet;
}

int CHSListBox::InsertString(int nIndex, LPCTSTR lpszItem)
{
	// call parent first
	int nRet = CListBox::InsertString(nIndex, lpszItem);

	m_iMaxWidth = max(m_iMaxWidth, _GetListStringWidth(lpszItem));
	SetHorizontalExtent(m_iMaxWidth + 3 + 3);

	if (m_bAutoScroll)
		SetTopIndex(nRet);
	return nRet;
}

void CHSListBox::ResetContent()
{
   CListBox::ResetContent();

   m_iMaxWidth = 0;
   SetHorizontalExtent(0);
}

int CHSListBox::DeleteString(UINT nIndex)
{
	// check string width of the item that is being deleted.
	int nCurWidth = _GetListStringWidth(nIndex);
	int nRet = CListBox::DeleteString(nIndex);

	// if the string being deleted has the biggest width, need
	// to reset HScroll and assign m_iMaxWidth by another string
	// which has the second-biggest width
	if (nRet != LB_ERR &&  nCurWidth >= m_iMaxWidth)
	{
		// reassign m_iMaxWidth
		m_iMaxWidth = 0;
		for (int i = 0; i < GetCount() - 1; i++)
			m_iMaxWidth = max(m_iMaxWidth, _GetListStringWidth(i));
	
		// reset HScroll bar
		SetHorizontalExtent(m_iMaxWidth + 3 + 3);
	}
	return nRet;
}

// only used to calculate width of a string item in listbox
int CHSListBox::_GetListStringWidth(LPCTSTR lpszString)
{
	SIZE sSize;
	sSize.cx = LB_ERR;
    CClientDC myDC(this);
	CFont* pListBoxFont = GetFont();
    if(pListBoxFont != NULL && lpszString)
    {
        CFont* pOldFont = myDC.SelectObject(pListBoxFont);
		GetTextExtentPoint32(myDC.m_hDC, 
        lpszString, strlen(lpszString), &sSize);
		myDC.SelectObject(pOldFont);
	}

	return sSize.cx;
}

int CHSListBox::_GetListStringWidth(UINT nIndex)
{
	int nLen = GetTextLen(nIndex);
	char* sz = new char[nLen + 1];
	int n = GetText(nIndex, sz);
	int nResult = _GetListStringWidth(sz);
	delete [] sz;
	return nResult;
}

WORD CHSListBox::GetScrollWidth()
{
	// get info of the vertical scroll bar
	SCROLLINFO scrollInfo;
    memset(&scrollInfo, 0, sizeof(SCROLLINFO));
    scrollInfo.cbSize = sizeof(SCROLLINFO);
    scrollInfo.fMask  = SIF_ALL;
    GetScrollInfo(SB_VERT, &scrollInfo, SIF_ALL);
	WORD nScrollWidth = 0;
    if(GetCount() > 1 && ((int)scrollInfo.nMax 
        >= (int)scrollInfo.nPage))
       nScrollWidth = GetSystemMetrics(SM_CXVSCROLL);

	return nScrollWidth;
}

BOOL CHSListBox::IsAutoScroll() const
{
	return m_bAutoScroll;
}