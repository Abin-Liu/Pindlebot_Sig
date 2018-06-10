// PickitPage.cpp : implementation file
//

#include "stdafx.h"
#include "pindlebot.h"
#include "PickitPage.h"
#include "ProgDlg.h"
#include "SelFileDlg.h"
#include "ArrayEx.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPickitPage property page

IMPLEMENT_DYNCREATE(CPickitPage, CPropertyPage)

CPickitPage::CPickitPage() : CPropertyPage(CPickitPage::IDD)
{
	//{{AFX_DATA_INIT(CPickitPage)
	m_sCurFile = _T("");
	//}}AFX_DATA_INIT
	m_bActivated = FALSE;	
	m_sCurFile = g_ini.GetProfileString(_T("General"), _T("Pickit"), _T("Pickit.ini"));
}

CPickitPage::~CPickitPage()
{
}

void CPickitPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPickitPage)
	DDX_Control(pDX, IDC_LIST1, m_list);
	DDX_Text(pDX, IDC_FILE, m_sCurFile);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPickitPage, CPropertyPage)
	//{{AFX_MSG_MAP(CPickitPage)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST1, OnItemchangedList1)
	ON_BN_CLICKED(IDC_MODIFYINI, OnModifyini)
	ON_BN_CLICKED(IDC_REFRESH, OnRefresh)
	ON_BN_CLICKED(IDC_SELECTFILE, OnSelectfile)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPickitPage message handlers

BOOL CPickitPage::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	
	// TODO: Add extra initialization here
	m_bActivated = TRUE;
	m_ini.SetPathName(g_sAppDir + _T("\\") + m_sCurFile);
	m_list.SetIni(&m_ini);
	m_list.InsertColumn(0, _T("     物品种类"), LVCFMT_LEFT, 270);
	m_list.SetExtendedStyle(LVS_EX_CHECKBOXES);

	// CG: The following block was added by the ToolTips component.
	{
		// Create the ToolTip control.
		m_tooltip.Create(this);
		m_tooltip.Activate(TRUE);

		// TODO: Use one of the following forms to add controls:
		// m_tooltip.AddTool(GetDlgItem(IDC_<name>), <string-table-id>);
		// m_tooltip.AddTool(GetDlgItem(IDC_<name>), "<text>");
		m_tooltip.AddTool(GetDlgItem(IDC_LIST1), _T("当前设置的物品类别，打勾的物品会被拾取，未打勾的则会被忽略"));
		m_tooltip.AddTool(GetDlgItem(IDC_MODIFYINI), _T("手动修改ini文件，不推荐。如果修改不当会引起拾取物品的错乱"));
		m_tooltip.AddTool(GetDlgItem(IDC_SELECTFILE), _T("重新选择ini文件"));
	}
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CPickitPage::OnItemchangedList1(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	// TODO: Add your control notification handler code here
	
	*pResult = 0;
}

void CPickitPage::OnModifyini() 
{
	// TODO: Add your control notification handler code here
	GetDlgItem(IDC_REFRESH)->ShowWindow(TRUE);
	m_list.ShowWindow(FALSE);
	CString sIni = g_sAppDir + _T("\\") + m_sCurFile;
	::ShellExecute(NULL, "open", _T("notepad.exe"), sIni, g_sAppDir, SW_SHOW);
}

void CPickitPage::ParseAndList()
{
	m_list.DeleteAllItems();

	GetDlgItem(IDC_REFRESH)->ShowWindow(FALSE);
	m_list.ShowWindow(TRUE);

	CProgressDlg prog;
	prog.Create();

	CStringArray aItemsStr;
	m_ini.GetProfileSectionNames(aItemsStr);
	CString sItem;
	CArrayEx<CString, LPCTSTR> aItems;
	//CString sItem;
	for (int i = 0; i < aItemsStr.GetSize(); i++)
	{
		sItem = aItemsStr[i];
		sItem.MakeLower();
		if (sItem != _T("configuration") && sItem.Find(_T(" other")) == -1 && aItems.Find(aItemsStr[i]) == -1)
			aItems.Add(aItemsStr[i]);
	}

	aItems.Sort();
	prog.SetRange(0, aItems.GetSize());
	prog.SetStep(1);
	int nPickup = 1;

	for (int j = 0; j < aItems.GetSize(); j++)
	{
		m_list.InsertItem(j, aItems[j]);
		nPickup = m_ini.GetProfileInt(aItems[j], _T("Pickup"), 1);
		m_list.SetCheck(j, nPickup != 0);
		prog.SetStatus(aItems[j]);
		prog.StepIt();
		if (prog.CheckCancelButton())
			break;
	}
}

BOOL CPickitPage::OnSetActive() 
{
	// TODO: Add your specialized code here and/or call the base class
	ParseAndList();
	return CPropertyPage::OnSetActive();
}

void CPickitPage::OnRefresh() 
{
	// TODO: Add your control notification handler code here
	ParseAndList();
}

BOOL CPickitPage::PreTranslateMessage(MSG* pMsg)
{
	// CG: The following block was added by the ToolTips component.
	{
		// Let the ToolTip process this message.
		m_tooltip.RelayEvent(pMsg);
	}
	return CPropertyPage::PreTranslateMessage(pMsg);	// CG: This was added by the ToolTips component.
}

void CPickitPage::OnSelectfile() 
{
	// TODO: Add your control notification handler code here
	CSelFileDlg dlg(m_sCurFile);
	if (dlg.DoModal() == IDOK && m_sCurFile != dlg.GetSelFile())
	{
		m_sCurFile = dlg.GetSelFile();
		if (m_sCurFile.IsEmpty())
			m_sCurFile = _T("Pickit.ini");
		UpdateData(FALSE);

		m_ini.SetPathName(g_sAppDir + _T("\\") + m_sCurFile);
		ParseAndList();
	}
}

void CPickitPage::ApplySettings()
{
	if (!m_bActivated)
		return;

	g_ini.WriteProfileString(_T("General"), _T("Pickit"), m_sCurFile);
}
