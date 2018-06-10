////////////////////////////////////////////////////////////////////// 
//        HSListBox.h (Interface for the CHSListBox class)
//  
//  October 11, 2001    Liu, Bin
// 
//  Description:
//  In current release of Microsoft Visual C++ there exists a flaw that
//  prevents horizontal scroll bar from functioning properly with
//  CListBox control, as a result, horizontal scroll bar will not
//  appear on a CListBox control in most case, even though WS_HSCROLL
//  flag has been set in the Create method of ClistBox.
//
//  This class is derived from CListBox and over-rode the following
//  virtual member functons of base class CListBox:
//  
//  AddString, InsertString, DeleteString, ResetContent
//
//  And makes functionalities of horizontal scroll bar work the way as
//  they were supposed to.
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_HSLISTBOX_H__48CAF3DC_E0A3_4586_AEC1_CE791E8D6A63__INCLUDED_)
#define AFX_HSLISTBOX_H__48CAF3DC_E0A3_4586_AEC1_CE791E8D6A63__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CHSListBox : public CListBox  
{
public:
	CHSListBox();
	virtual ~CHSListBox();

	WORD GetScrollWidth();
	virtual int DeleteString(UINT nIndex);
	virtual void ResetContent();
	virtual int InsertString(int nIndex, LPCTSTR lpszItem);
	virtual int AddString(LPCTSTR lpszItem);
	void SetAutoScroll(BOOL bAutoScroll = TRUE);
	BOOL IsAutoScroll() const;
	int GetMaxWidth() const;

protected:
	virtual int _GetListStringWidth(UINT nIndex);
	virtual int _GetListStringWidth(LPCTSTR lpszString);

private:
	int m_iMaxWidth;
	BOOL m_bAutoScroll;
};

#endif // !defined(AFX_HSLISTBOX_H__48CAF3DC_E0A3_4586_AEC1_CE791E8D6A63__INCLUDED_)
