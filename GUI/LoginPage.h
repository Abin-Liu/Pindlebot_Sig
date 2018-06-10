#if !defined(AFX_LOGINPAGE_H__F0CDAB34_47CC_43E6_8B35_2BB45C876CD7__INCLUDED_)
#define AFX_LOGINPAGE_H__F0CDAB34_47CC_43E6_8B35_2BB45C876CD7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// LoginPage.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CLoginPage dialog

class CLoginPage : public CPropertyPage
{
	DECLARE_DYNCREATE(CLoginPage)

// Construction
public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	void ApplySettings() const;
	int GetAutoRes() const;
	CLoginPage();
	~CLoginPage();

// Dialog Data
	//{{AFX_DATA(CLoginPage)
	enum { IDD = IDD_LOGINDLG };
	BOOL	m_bResMerc;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CLoginPage)
	public:
	virtual BOOL OnKillActive();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	CToolTipCtrl m_tooltip;
	// Generated message map functions
	//{{AFX_MSG(CLoginPage)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:

	BOOL m_bActivated;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_LOGINPAGE_H__F0CDAB34_47CC_43E6_8B35_2BB45C876CD7__INCLUDED_)
