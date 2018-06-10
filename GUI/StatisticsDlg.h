#if !defined(AFX_STATISTICSDLG_H__E4D34D78_890D_407E_8DDF_EEC01DB04A88__INCLUDED_)
#define AFX_STATISTICSDLG_H__E4D34D78_890D_407E_8DDF_EEC01DB04A88__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// StatisticsDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CStatisticsDlg dialog
#include "IniFile.h"

class CStatisticsDlg : public CDialog
{
// Construction
public:
	CString GetPercent(int n, int nTotal) const;
	CStatisticsDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CStatisticsDlg)
	enum { IDD = IDD_STATISTICSDLG };
	CString	m_sCharDied;
	CString	m_sMercDied;
	CString	m_sPindleKilled;
	CString	m_sRetreated;
	CString	m_sDeadlyCombo;
	CString	m_sInsufficientSkill;
	CString	m_sNetEffect;
	CString	m_sTotalTime;
	CString	m_sTotalGames;
	CString	m_sPotionDrunk;
	CString	m_sPotionMerc;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CStatisticsDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CStatisticsDlg)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	CIniFile m_ini;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STATISTICSDLG_H__E4D34D78_890D_407E_8DDF_EEC01DB04A88__INCLUDED_)
