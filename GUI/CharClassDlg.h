#if !defined(AFX_CHARCLASSDLG_H__658C7448_36E3_4E2B_8901_ED44569987D7__INCLUDED_)
#define AFX_CHARCLASSDLG_H__658C7448_36E3_4E2B_8901_ED44569987D7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CharClassDlg.h : header file
//

#include "D2Skill.h"
#include "D2Scheme.h"
#include "ArrayEx.h"
#include "ReportCtrl.h"

/////////////////////////////////////////////////////////////////////////////
// CCharClassDlg dialog

class CCharClassDlg : public CPropertyPage
{
	DECLARE_DYNCREATE(CCharClassDlg)

// Construction
public:
	
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	void ApplySettings();
	CCharClassDlg();   // standard constructor

// Dialog Data
	//{{AFX_DATA(CCharClassDlg)
	enum { IDD = IDD_CHARCLASSDLG };
	CButton	m_ctlUseFindItem;
	CReportCtrl	m_list;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCharClassDlg)
	public:
	protected:
	virtual BOOL OnSetActive();
	virtual BOOL OnKillActive();
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	void CheckChangeInvalidSafetyScheme();
	BOOL VerifySafetyScheme(LPCTSTR lpName) const;
	CToolTipCtrl m_tooltip;

	// Generated message map functions
	//{{AFX_MSG(CCharClassDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnItemchangedCharlist(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnNewchar();
	afx_msg void OnDelchar();
	afx_msg void OnBeforered();
	afx_msg void OnAfterred();
	afx_msg void OnOnpindleskin();
	afx_msg void OnOncorpose();
	afx_msg void OnEditchar();
	afx_msg void OnDblclkCharlist(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:

	void CheckStats();
	void InitSchemes();
	CIniFile m_ini;
	CArrayEx<CD2Scheme, const CD2Scheme&> m_aSchemes;

	void ListSchemes();
	BOOL m_bActivated;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CHARCLASSDLG_H__658C7448_36E3_4E2B_8901_ED44569987D7__INCLUDED_)
