#if !defined(AFX_PICKITPAGE_H__A58D7826_5663_4095_82C3_76E77731266A__INCLUDED_)
#define AFX_PICKITPAGE_H__A58D7826_5663_4095_82C3_76E77731266A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PickitPage.h : header file
//

#include "IniFile.h"
#include "MyListCtrl.h"

/////////////////////////////////////////////////////////////////////////////
// CPickitPage dialog

struct D2ITEM
{
	CString sName;
	CString sDesc;
	CString sCode;
	CString sLevel;
	BYTE nPickup;
	BYTE nIdentified;
	BYTE nHasSockets;
	BYTE nIsEthereal;
};

class CPickitPage : public CPropertyPage
{
	DECLARE_DYNCREATE(CPickitPage)

// Construction
public:
	void ApplySettings();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	CPickitPage();
	~CPickitPage();

// Dialog Data
	//{{AFX_DATA(CPickitPage)
	enum { IDD = IDD_PICKITDLG };
	CMyListCtrl	m_list;
	CString	m_sCurFile;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CPickitPage)
	public:
	virtual BOOL OnSetActive();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	CToolTipCtrl m_tooltip;
	// Generated message map functions
	//{{AFX_MSG(CPickitPage)
	virtual BOOL OnInitDialog();
	afx_msg void OnItemchangedList1(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnModifyini();
	afx_msg void OnRefresh();
	afx_msg void OnSelectfile();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	void ParseAndList();
	CIniFile m_ini;
	BOOL m_bActivated;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PICKITPAGE_H__A58D7826_5663_4095_82C3_76E77731266A__INCLUDED_)
