// PindlebotDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Pindlebot.h"
#include "PindlebotDlg.h"
#include "LoginPage.h"
#include "PickitPage.h"
#include "StatisticsDlg.h"
#include "CharClassDlg.h"
#include "ItemLogDlg.h"
#include "D2Monster.h"
#include "SafetyDlg.h"
#include "MySheet.h"
#include "SafeSchemePage.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define WM_ICON_NOTIFY  WM_APP + 10

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	CHyperLink	m_urlHomepage;
	CHyperLink	m_urlCureteam;
	CHyperLink	m_urlAbin;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	CString m_sAppTitle;
	CString m_sAppVer;
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	DDX_Control(pDX, IDC_HOMEPAGE, m_urlHomepage);
	DDX_Control(pDX, IDC_CURETEAM, m_urlCureteam);
	DDX_Control(pDX, IDC_ABIN, m_urlAbin);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPindlebotDlg dialog

CPindlebotDlg::CPindlebotDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CPindlebotDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CPindlebotDlg)
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_pActiveWnd = NULL;
}

void CPindlebotDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPindlebotDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CPindlebotDlg, CDialog)
	//{{AFX_MSG_MAP(CPindlebotDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_COMMAND(IDC_ABOUT, OnAbout)
	ON_BN_CLICKED(IDC_EXIT, OnExit)
	ON_WM_TIMER()
	ON_COMMAND(ID_POPUP_ABOUT, OnPopupAbout)
	ON_COMMAND(ID_POPUP_ITEMLOG, OnPopupItemlog)
	ON_COMMAND(ID_POPUP_STATISTICS, OnPopupStatistics)
	ON_COMMAND(ID_POPUP_SETTINGS, OnPopupSettings)
	ON_COMMAND(ID_POPUP_HELP, OnPopupHelp)
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_ICON_NOTIFY, OnTrayNotification)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPindlebotDlg message handlers

LRESULT CPindlebotDlg::OnTrayNotification(WPARAM wParam, LPARAM lParam)
{
	UINT nMsg = (UINT)lParam;

	if (nMsg == WM_LBUTTONDOWN || nMsg == WM_RBUTTONDOWN || nMsg == WM_RBUTTONDOWN)
	{
		if (m_pActiveWnd != NULL && ::IsWindow(m_pActiveWnd->GetSafeHwnd()))
		{
			::SetForegroundWindow(m_pActiveWnd->GetSafeHwnd());
		}
	}
	else if (nMsg == WM_RBUTTONUP)
	{
		// pop up menu
		PopupMenu();
	}

	return (LRESULT)0;
}

BOOL CPindlebotDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	// TODO: Add extra initialization here
	this->SetWindowText(APP_TITLE);
	this->CenterWindow(NULL);
	m_tray.Create(this, WM_ICON_NOTIFY, APP_TITLE, m_hIcon, IDR_MAINFRAME);
	m_tray.MinimiseToTray(this);

	SetTimer(0, 250, NULL);

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CPindlebotDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CPindlebotDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CPindlebotDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

BOOL CAboutDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	SetDlgItemText(IDC_TITLE, APP_TITLE);
	m_urlAbin.SetURL(MAILME);
	m_urlCureteam.SetURL(CURE_TEAM);
	m_urlHomepage.SetURL(HOMEPAGE);
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CPindlebotDlg::OnAbout() 
{
	// TODO: Add your command handler code here
	CAboutDlg dlg;
	dlg.DoModal();
}

void CPindlebotDlg::PopupMenu()
{
	if (m_pActiveWnd != NULL && ::IsWindow(m_pActiveWnd->GetSafeHwnd()))
	{
		::SetForegroundWindow(m_pActiveWnd->GetSafeHwnd());
		return;
	}

	CMenu menu;
	CMenu* popUp;
	CPoint point;

	::GetCursorPos(&point);
	if (!menu.LoadMenu(IDR_POPUP_MENU))
		return;
	
	popUp = menu.GetSubMenu(0); // Chinese menu

	if (popUp)
	{
		HWND hWnd = this->GetSafeHwnd();
		::SetForegroundWindow(hWnd);
		popUp->TrackPopupMenu(TPM_LEFTALIGN, point.x, point.y, this);
		::PostMessage(hWnd, WM_NULL, 0, 0);
	}
}

void CPindlebotDlg::OnExit() 
{
	// TODO: Add your control notification handler code here
	OnCancel();
}

void CPindlebotDlg::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	if (nIDEvent == 0)
	{
		this->ShowWindow(SW_HIDE);
		KillTimer(0);
	}
	
	CDialog::OnTimer(nIDEvent);
}

void CPindlebotDlg::OnPopupAbout() 
{
	// TODO: Add your command handler code here
	CAboutDlg dlg;
	m_pActiveWnd = &dlg;
	dlg.DoModal();
	m_pActiveWnd = NULL;
}

void CPindlebotDlg::OnPopupItemlog() 
{
	// TODO: Add your command handler code here
	CItemLogDlg dlg;
	m_pActiveWnd = &dlg;
	dlg.DoModal();
	m_pActiveWnd = NULL;
}

void CPindlebotDlg::OnPopupStatistics() 
{
	// TODO: Add your command handler code here
	CStatisticsDlg dlg;
	m_pActiveWnd = &dlg;
	dlg.DoModal();
	m_pActiveWnd = NULL;
}

void CPindlebotDlg::OnPopupSettings() 
{
	// TODO: Add your command handler code here
	CMySheet sheet(APP_TITLE);
	sheet.m_psh.dwFlags |= PSH_NOAPPLYNOW; 
	CLoginPage pageControl;
	CCharClassDlg pageChar;
	CPickitPage pagePickit;
	CSafeSchemePage pageSafety;
	
	sheet.AddPage(&pageControl);	
	sheet.AddPage(&pageSafety);
	sheet.AddPage(&pageChar);
	sheet.AddPage(&pagePickit);
	
	m_pActiveWnd = &sheet;
	const int RES = sheet.DoModal();
	m_pActiveWnd = NULL;

	if (RES == IDOK)
	{
		pageChar.ApplySettings();
		pageControl.ApplySettings();
		pagePickit.ApplySettings();
		::DeleteFile(g_safetyIni.GetPathName());
		for (int i = 0; i < g_aSafetySchemes.GetSize(); i++)
			g_aSafetySchemes[i].Save(g_safetyIni);
	}
}

void CPindlebotDlg::OnPopupHelp() 
{
	// TODO: Add your command handler code here
	AfxGetApp()->WinHelp(0);
}
