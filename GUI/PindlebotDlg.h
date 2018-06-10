// PindlebotDlg.h : header file
//

#if !defined(AFX_PINDLEBOTDLG_H__A1A7EF35_A357_4B72_983E_87A95FC36F72__INCLUDED_)
#define AFX_PINDLEBOTDLG_H__A1A7EF35_A357_4B72_983E_87A95FC36F72__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CPindlebotDlg dialog

#include "HyperLink.h"
#include "SystemTray.h"

class CPindlebotDlg : public CDialog
{
// Construction
public:
	CPindlebotDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CPindlebotDlg)
	enum { IDD = IDD_PINDLEBOT_DIALOG };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPindlebotDlg)
	public:
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	CToolTipCtrl m_tooltip;
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CPindlebotDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnAbout();
	afx_msg void OnExit();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnPopupAbout();
	afx_msg void OnPopupItemlog();
	afx_msg void OnPopupStatistics();
	afx_msg void OnPopupSettings();
	afx_msg void OnPopupHelp();
	//}}AFX_MSG

	afx_msg LRESULT OnTrayNotification(WPARAM wParam, LPARAM lParam);

	DECLARE_MESSAGE_MAP()


protected:
	void PopupMenu();
	CSystemTray m_tray;
	CWnd* m_pActiveWnd;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PINDLEBOTDLG_H__A1A7EF35_A357_4B72_983E_87A95FC36F72__INCLUDED_)
