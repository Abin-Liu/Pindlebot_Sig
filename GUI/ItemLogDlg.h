#if !defined(AFX_ITEMLOGDLG_H__4CA333CA_1FBC_4E41_98BE_91FBB1A4DA24__INCLUDED_)
#define AFX_ITEMLOGDLG_H__4CA333CA_1FBC_4E41_98BE_91FBB1A4DA24__INCLUDED_

#include "HSListBox.h"

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ItemLogDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CItemLogDlg dialog

class CItemLogDlg : public CDialog
{
// Construction
public:
	CItemLogDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CItemLogDlg)
	enum { IDD = IDD_ITEMLOGDLG };
	CHSListBox	m_list;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CItemLogDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CItemLogDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnClear();
	afx_msg void OnCopytoclipboard();
	afx_msg void OnSelchangeList1();
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ITEMLOGDLG_H__4CA333CA_1FBC_4E41_98BE_91FBB1A4DA24__INCLUDED_)
