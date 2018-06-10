#if !defined(AFX_NEWCHARDLG_H__5102B68E_E918_45E7_B521_1980C30B94C1__INCLUDED_)
#define AFX_NEWCHARDLG_H__5102B68E_E918_45E7_B521_1980C30B94C1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// NewCharDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CNewCharDlg dialog

class CNewCharDlg : public CDialog
{
// Construction
public:
	CString GetSafetyName() const;
	int GetAmmoBuyQty() const;
	int GetAmmoType() const;
	int GetCharClass() const;
	CString GetCharName() const;
	CNewCharDlg(LPCTSTR lpName = _T(""), int nClass = 6, LPCTSTR lpSafetyName = _T("Default"), int nAmmoType = 0, int nAmmoBuyQty = 2, CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CNewCharDlg)
	enum { IDD = IDD_NEWCHARDLG };
	CEditEx	m_wndAmmoBuyQty;
	CComboBox	m_wndSafetySchemes;
	CEditEx	m_ctlCharName;
	CString	m_sCharName;
	int		m_nCharClass;
	int		m_nAmmoType;
	int		m_nAmmoBuyQty;
	int		m_nSafetyIdx;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CNewCharDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CNewCharDlg)
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	afx_msg void OnAmmo();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

protected:
	CString m_sSafetySchemeName;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_NEWCHARDLG_H__5102B68E_E918_45E7_B521_1980C30B94C1__INCLUDED_)
