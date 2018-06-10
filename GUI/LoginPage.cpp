// LoginPage.cpp : implementation file
//

#include "stdafx.h"
#include "Pindlebot.h"
#include "LoginPage.h"
#include "IniFile.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CLoginPage property page

IMPLEMENT_DYNCREATE(CLoginPage, CPropertyPage)

CLoginPage::CLoginPage() : CPropertyPage(CLoginPage::IDD)
{
	m_bActivated = FALSE;

	//{{AFX_DATA_INIT(CLoginPage)
	m_bResMerc = g_ini.GetProfileBool(_T("General"), _T("Res Merc"), TRUE);
	//}}AFX_DATA_INIT

}

CLoginPage::~CLoginPage()
{
}

void CLoginPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CLoginPage)
	DDX_Check(pDX, IDC_RESMERC, m_bResMerc);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CLoginPage, CPropertyPage)
	//{{AFX_MSG_MAP(CLoginPage)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CLoginPage message handlers

BOOL CLoginPage::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	
	// TODO: Add extra initialization here
	m_bActivated = TRUE;
	// CG: The following block was added by the ToolTips component.
	{
		// Create the ToolTip control.
		m_tooltip.Create(this);
		m_tooltip.Activate(TRUE);

		// TODO: Use one of the following forms to add controls:
		// m_tooltip.AddTool(GetDlgItem(IDC_<name>), <string-table-id>);
		// m_tooltip.AddTool(GetDlgItem(IDC_<name>), "<text>");
		m_tooltip.AddTool(GetDlgItem(IDC_RESMERC), _T("允许/禁止自动复活雇佣兵，注意要有足够的金币"));
	}
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

int CLoginPage::GetAutoRes() const
{
	return m_bResMerc ? 1 : 0;
}

void CLoginPage::ApplySettings() const
{
	if (!m_bActivated)
		return;

	//apply Login page
	g_ini.WriteProfileInt(_T("General"), _T("Res Merc"), GetAutoRes());
}

BOOL CLoginPage::PreTranslateMessage(MSG* pMsg)
{
	// CG: The following block was added by the ToolTips component.
	{
		// Let the ToolTip process this message.
		m_tooltip.RelayEvent(pMsg);
	}
	return CPropertyPage::PreTranslateMessage(pMsg);	// CG: This was added by the ToolTips component.
}

BOOL CLoginPage::OnKillActive() 
{
	// TODO: Add your specialized code here and/or call the base class
	
	return CPropertyPage::OnKillActive();
}
