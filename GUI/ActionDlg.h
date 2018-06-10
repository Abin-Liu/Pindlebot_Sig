#if !defined(AFX_ACTIONDLG_H__0043F9AF_645A_4E3E_AA4F_F396D88C1E0A__INCLUDED_)
#define AFX_ACTIONDLG_H__0043F9AF_645A_4E3E_AA4F_F396D88C1E0A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ActionDlg.h : header file
//

#include "D2Skill.h"	// Added by ClassView
#include "D2Scheme.h"

/////////////////////////////////////////////////////////////////////////////
// CActionDlg dialog

class CActionDlg : public CDialog
{
// Construction
public:
	CD2Action GetAction() const;
	void SetAction(const CD2Action& action);
	CActionDlg(const CD2Character* pChar, int nSituation, CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CActionDlg)
	enum { IDD = IDD_ACTIONDLG };
	CEditEx	m_ctlRepeat;
	CEditEx	m_ctlDelay;
	CComboBox	m_ctlSkillList;
	int		m_nTarget;
	int		m_nCastDelay;
	int		m_nSide;
	int		m_nRepeat;
	CString	m_sCoolDown;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CActionDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CActionDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSelchangeSkilllist();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	void CheckStats();
	void InitSkills();
	const CD2Character* m_pChar;
	int m_nSituation;
	const CD2Skill* m_pSelSkill;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ACTIONDLG_H__0043F9AF_645A_4E3E_AA4F_F396D88C1E0A__INCLUDED_)
