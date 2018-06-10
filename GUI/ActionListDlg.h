#if !defined(AFX_ACTIONLISTDLG_H__395ACC57_2E3D_43C3_81E7_513FB8A305B0__INCLUDED_)
#define AFX_ACTIONLISTDLG_H__395ACC57_2E3D_43C3_81E7_513FB8A305B0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ActionListDlg.h : header file
//

#include "D2Skill.h"
#include "D2Character.h"
#include "D2Action.h"

/////////////////////////////////////////////////////////////////////////////
// CActionListDlg dialog

class CActionListDlg : public CDialog
{
// Construction
public:
	CActionListDlg(CD2Character* pChar, CArrayEx<CD2Action, const CD2Action&>* pActions, int nActionSitu, CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CActionListDlg)
	enum { IDD = IDD_ACTIONLISTDLG };
	CListCtrl	m_list;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CActionListDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CActionListDlg)
	afx_msg void OnItemchangedList1(NMHDR* pNMHDR, LRESULT* pResult);
	virtual BOOL OnInitDialog();
	afx_msg void OnAddaction();
	afx_msg void OnRemoveaction();
	afx_msg void OnEditaction();
	afx_msg void OnDblclkList1(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnMoveup();
	afx_msg void OnMovedown();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	void CheckStats();
	void ListActions();
	int m_nCurSel;
	int m_nActionSitu;
	CD2Character* m_pChar;	
	CArrayEx<CD2Action, const CD2Action&>* m_pActions;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ACTIONLISTDLG_H__395ACC57_2E3D_43C3_81E7_513FB8A305B0__INCLUDED_)
