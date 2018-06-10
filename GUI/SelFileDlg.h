#if !defined(AFX_SELFILEDLG_H__B13F095D_46BE_48A1_97E7_2348CE900AF6__INCLUDED_)
#define AFX_SELFILEDLG_H__B13F095D_46BE_48A1_97E7_2348CE900AF6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SelFileDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CSelFileDlg dialog

class CSelFileDlg : public CDialog
{
// Construction
public:
	CString GetSelFile() const;
	CSelFileDlg(CString sFile, CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CSelFileDlg)
	enum { IDD = IDD_INIFILEDLG };
	CListBox	m_list;
	CString	m_sFile;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSelFileDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CSelFileDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSelchangeList1();
	afx_msg void OnDblclkList1();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	CString m_sOrigFile;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SELFILEDLG_H__B13F095D_46BE_48A1_97E7_2348CE900AF6__INCLUDED_)
