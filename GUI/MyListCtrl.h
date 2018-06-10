#if !defined(AFX_MYLISTCTRL_H__45B27053_2493_4E99_8332_3F73E7976578__INCLUDED_)
#define AFX_MYLISTCTRL_H__45B27053_2493_4E99_8332_3F73E7976578__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MyListCtrl.h : header file

#include "IniFile.h"

// checked-state changes:
#define CC_NONE				0
#define CC_DISABLETOENABLE	1
#define CC_ENABLETODISABLE	2

/////////////////////////////////////////////////////////////////////////////
// CMyListCtrl window

class CMyListCtrl : public CListCtrl
{
// Construction
public:
	CMyListCtrl();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMyListCtrl)
	//}}AFX_VIRTUAL

// Implementation
public:
	void SetIni(CIniFile* pIni);
	virtual ~CMyListCtrl();

	// Generated message map functions
protected:
	//{{AFX_MSG(CMyListCtrl)
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()

protected:
	CIniFile* m_pIni;
	int Select(POINT pt);
	int m_nCurSel;
private:
	void PickUp(BOOL bPickup);
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MYLISTCTRL_H__45B27053_2493_4E99_8332_3F73E7976578__INCLUDED_)
