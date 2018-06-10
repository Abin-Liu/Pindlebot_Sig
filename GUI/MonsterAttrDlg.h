#if !defined(AFX_MONSTERATTRDLG_H__CE0CCE63_0633_449D_8EA1_9A51DEA28E28__INCLUDED_)
#define AFX_MONSTERATTRDLG_H__CE0CCE63_0633_449D_8EA1_9A51DEA28E28__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MonsterAttrDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CMonsterAttrDlg dialog

class CMonsterAttrDlg : public CDialog
{
// Construction
public:
	WORD GetAttr2() const;
	WORD GetAttr1() const;
	CMonsterAttrDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CMonsterAttrDlg)
	enum { IDD = IDD_MONSTERATTRDLG };
	CComboBox	m_wndAttr2;
	CComboBox	m_wndAttr1;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMonsterAttrDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	void InitAttributes();

	// Generated message map functions
	//{{AFX_MSG(CMonsterAttrDlg)
	afx_msg void OnSelchangeCombo();
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

protected:
	WORD m_wAttr1;
	WORD m_wAttr2;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MONSTERATTRDLG_H__CE0CCE63_0633_449D_8EA1_9A51DEA28E28__INCLUDED_)
