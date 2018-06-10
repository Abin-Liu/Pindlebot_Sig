#if !defined(AFX_SAFETYDLG_H__72884377_BA2D_4F08_AF9F_DA4680EDA635__INCLUDED_)
#define AFX_SAFETYDLG_H__72884377_BA2D_4F08_AF9F_DA4680EDA635__INCLUDED_

#include "IniFile.h"	// Added by ClassView
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SafetyDlg.h : header file
//

// location adjustment
#define LA_NORMAL	0
#define LA_CLOSER	1
#define LA_FARTHER	2

// default safety settings
#define DEF_ADJ_LOCATION	LA_NORMAL
#define DEF_MALAH_HP		90
#define DEF_MALAH_MP		60
#define DEF_ALERT_HP		50
#define DEF_ALERT_MP		15

#include "SafetyScheme.h"
#include "ReportCtrl.h"

/////////////////////////////////////////////////////////////////////////////
// CSafetyDlg dialog

class CSafetyDlg : public CDialog
{
// Construction
public:
	CSafetyScheme GetScheme() const;
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	CSafetyDlg(const CSafetyScheme& ss, CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CSafetyDlg)
	enum { IDD = IDD_SAFETYDLG };
	CEditEx	m_wndPotionMP;
	CEditEx	m_wndPotionMerc;
	CEditEx	m_wndPotionHP;
	CSpinButtonCtrl	m_spinPotionMP;
	CSpinButtonCtrl	m_spinPotionMerc;
	CSpinButtonCtrl	m_spinPotionHP;
	CEditEx	m_ctlMalahMP;
	CEditEx	m_ctlMalahHP;
	CEditEx	m_ctlAlertMP;
	CEditEx	m_ctlAlertHP;
	CSpinButtonCtrl	m_spinMalahMP;
	CSpinButtonCtrl	m_spinMalahHP;
	CSpinButtonCtrl	m_spinALertMP;
	CSpinButtonCtrl	m_spinAlertHP;
	CReportCtrl	m_AttrList;
	int		m_nAlertHP;
	int		m_nAlertMP;
	BOOL	m_bMalahHP;
	BOOL	m_bMalahMP;
	BOOL	m_bWatchHP;
	BOOL	m_bWatchMerc;
	BOOL	m_bWatchMP;
	int		m_nMalahHP;
	int		m_nMalahMP;
	int		m_nAttackDistance;
	BOOL	m_bWatchPotionHP;
	BOOL	m_bWatchPotionMP;
	BOOL	m_bWatchPotionMerc;
	int		m_nPotionHP;
	int		m_nPotionMerc;
	int		m_nPotionMP;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSafetyDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	BOOL IsPercentageValid(int n) const;
	BOOL ValidatePercentage() const;
	CToolTipCtrl m_tooltip;

	// Generated message map functions
	//{{AFX_MSG(CSafetyDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnItemchangedAttrlist(NMHDR* pNMHDR, LRESULT* pResult);
	virtual void OnOK();
	afx_msg void OnAddcombo();
	afx_msg void OnRemovecombo();
	afx_msg void OnImmunity();
	afx_msg void CheckStats();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
		
private:
	CSafetyScheme m_ss;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SAFETYDLG_H__72884377_BA2D_4F08_AF9F_DA4680EDA635__INCLUDED_)
