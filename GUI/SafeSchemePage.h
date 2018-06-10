#if !defined(AFX_SAFESCHEMEPAGE_H__0814B031_CCEB_4990_AF12_62AB473ACBEF__INCLUDED_)
#define AFX_SAFESCHEMEPAGE_H__0814B031_CCEB_4990_AF12_62AB473ACBEF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SafeSchemePage.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CSafeSchemePage dialog
class CSafeSchemePage : public CPropertyPage
{
	DECLARE_DYNCREATE(CSafeSchemePage)

// Construction
public:
	CSafeSchemePage();
	~CSafeSchemePage();

// Dialog Data
	//{{AFX_DATA(CSafeSchemePage)
	enum { IDD = IDD_SAFETYPAGE };
	CListBox	m_wndList;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CSafeSchemePage)
	public:
	virtual BOOL OnSetActive();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CSafeSchemePage)
	afx_msg void OnNew();
	afx_msg void OnEdit();
	afx_msg void OnDelete();
	virtual BOOL OnInitDialog();
	afx_msg void OnSelchangeList1();
	afx_msg void OnDblclkList1();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

protected:
	BOOL m_bPageInitialied;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SAFESCHEMEPAGE_H__0814B031_CCEB_4990_AF12_62AB473ACBEF__INCLUDED_)
