// SafeSchemePage.cpp : implementation file
//

#include "stdafx.h"
#include "pindlebot.h"
#include "SafeSchemePage.h"
#include "SafetyDlg.h"
#include "SafetySchemeNameDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSafeSchemePage property page

IMPLEMENT_DYNCREATE(CSafeSchemePage, CPropertyPage)

CSafeSchemePage::CSafeSchemePage() : CPropertyPage(CSafeSchemePage::IDD)
{
	//{{AFX_DATA_INIT(CSafeSchemePage)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_bPageInitialied = FALSE;
}

CSafeSchemePage::~CSafeSchemePage()
{
}

void CSafeSchemePage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSafeSchemePage)
	DDX_Control(pDX, IDC_LIST1, m_wndList);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSafeSchemePage, CPropertyPage)
	//{{AFX_MSG_MAP(CSafeSchemePage)
	ON_BN_CLICKED(IDC_NEW, OnNew)
	ON_BN_CLICKED(IDC_EDIT, OnEdit)
	ON_BN_CLICKED(IDC_DELETE, OnDelete)
	ON_LBN_SELCHANGE(IDC_LIST1, OnSelchangeList1)
	ON_LBN_DBLCLK(IDC_LIST1, OnDblclkList1)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSafeSchemePage message handlers

void CSafeSchemePage::OnNew() 
{
	// TODO: Add your control notification handler code here
	CSafetySchemeNameDlg nameDlg;
	if (nameDlg.DoModal() != IDOK)
		return;

	CSafetyScheme ss;
	ss.m_sName = nameDlg.GetName();

	if (g_aSafetySchemes.Find(ss) != -1)
	{
		CString sMsg;
		sMsg.Format(_T("安全方案\"%s\"已经存在。"), ss.GetName());
		MessageBox(sMsg, _T("命名冲突"), MB_ICONEXCLAMATION);
		return;
	}

	CSafetyDlg safetyDlg(ss);
	if (safetyDlg.DoModal() == IDOK)
	{
		g_aSafetySchemes.Add(safetyDlg.GetScheme());
		m_wndList.AddString(safetyDlg.GetScheme().GetName());
	}
}

void CSafeSchemePage::OnEdit() 
{
	// TODO: Add your control notification handler code here
	const int IDX = m_wndList.GetCurSel();

	if (IDX >= 0 && IDX < g_aSafetySchemes.GetSize())
	{
		CSafetyDlg dlg(g_aSafetySchemes[IDX]);
		if (dlg.DoModal() == IDOK)
		{
			g_aSafetySchemes[IDX] = dlg.GetScheme();
		}
	}	
}

void CSafeSchemePage::OnDelete() 
{
	// TODO: Add your control notification handler code here
	const int IDX = m_wndList.GetCurSel();

	if (IDX >= 0 && IDX < g_aSafetySchemes.GetSize() && g_aSafetySchemes[IDX].GetName().CompareNoCase(_T("Default")) != 0)
	{
		CString sMsg;
		sMsg.Format(_T("安全方案\"%s\"将被删除，如果有一个或多个角色正在使用该安全方案，它们将被迫改用\"Default\"方案。\n\n确定要继续吗？"), g_aSafetySchemes[IDX].GetName());

		if (MessageBox(sMsg, _T("删除安全方案"), MB_ICONEXCLAMATION | MB_YESNO) == IDYES)
		{
			g_aSafetySchemes.RemoveAt(IDX);
			m_wndList.DeleteString(IDX);
		}
	}	
}

BOOL CSafeSchemePage::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	
	// TODO: Add extra initialization here
	m_bPageInitialied = TRUE;
	
	for (int i = 0; i < g_aSafetySchemes.GetSize(); i++)
		m_wndList.AddString(g_aSafetySchemes[i].GetName());

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CSafeSchemePage::OnSelchangeList1() 
{
	// TODO: Add your control notification handler code here
	const int IDX = m_wndList.GetCurSel();
	GetDlgItem(IDC_EDIT)->EnableWindow(IDX != -1);
	GetDlgItem(IDC_DELETE)->EnableWindow(IDX != -1 && g_aSafetySchemes[IDX].GetName().CompareNoCase(_T("Default")) != 0);
}

void CSafeSchemePage::OnDblclkList1() 
{
	// TODO: Add your control notification handler code here
	OnEdit();
}

BOOL CSafeSchemePage::OnSetActive() 
{
	// TODO: Add your specialized code here and/or call the base class
	
	return CPropertyPage::OnSetActive();
}
