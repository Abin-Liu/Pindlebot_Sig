#if !defined(AFX_SAFETYSCHEMENAMEDLG_H__FCA327BF_2F7B_40FF_B9A3_1F7AB8A9451F__INCLUDED_)
#define AFX_SAFETYSCHEMENAMEDLG_H__FCA327BF_2F7B_40FF_B9A3_1F7AB8A9451F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SafetySchemeNameDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CSafetySchemeNameDlg dialog

class CSafetySchemeNameDlg : public CDialog
{
// Construction
public:
	CString GetName() const;
	CSafetySchemeNameDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CSafetySchemeNameDlg)
	enum { IDD = IDD_SAFETYSCHEMENAME };
	CEditEx	m_ctlName;
	CString	m_sName;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSafetySchemeNameDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CSafetySchemeNameDlg)
	afx_msg void OnChangeEdit1();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	BOOL IsValidName() const;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SAFETYSCHEMENAMEDLG_H__FCA327BF_2F7B_40FF_B9A3_1F7AB8A9451F__INCLUDED_)
