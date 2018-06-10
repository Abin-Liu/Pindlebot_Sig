// ActionListDlg.cpp : implementation file
//

#include "stdafx.h"
#include "pindlebot.h"
#include "ActionListDlg.h"
#include "ActionDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CActionListDlg dialog


CActionListDlg::CActionListDlg(CD2Character* pChar, CArrayEx<CD2Action, const CD2Action&>* pActions, int nActionSitu, CWnd* pParent /*=NULL*/)
	: CDialog(CActionListDlg::IDD, pParent)
{
	m_nCurSel = -1;
	m_nActionSitu = nActionSitu;
	m_pChar = pChar;
	m_pActions = pActions;
	//{{AFX_DATA_INIT(CActionListDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CActionListDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CActionListDlg)
	DDX_Control(pDX, IDC_LIST1, m_list);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CActionListDlg, CDialog)
	//{{AFX_MSG_MAP(CActionListDlg)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST1, OnItemchangedList1)
	ON_BN_CLICKED(IDC_ADDACTION, OnAddaction)
	ON_BN_CLICKED(IDC_REMOVEACTION, OnRemoveaction)
	ON_BN_CLICKED(IDC_EDITACTION, OnEditaction)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST1, OnDblclkList1)
	ON_BN_CLICKED(IDC_MOVEUP, OnMoveup)
	ON_BN_CLICKED(IDC_MOVEDOWN, OnMovedown)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CActionListDlg message handlers

void CActionListDlg::OnItemchangedList1(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	// TODO: Add your control notification handler code here
	if (m_list.GetSelectedCount() != 1)
		m_nCurSel = -1;
	else
	{
		POSITION pos= m_list.GetFirstSelectedItemPosition();
		m_nCurSel = m_list.GetNextSelectedItem(pos);
	}

	CheckStats();

	*pResult = 0;
}

BOOL CActionListDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	CString sTitle;
	CString sSitu;
	switch (m_nActionSitu)
	{
	case DAS_BEFORERED:
		sSitu = _T("进入红门之前");
		break;

	case DAS_AFTERRED:
		sSitu = _T("进入红门之后"); 
		break;

	case DAS_ONPINDLESKIN:
		sSitu = _T("到达Pindleskin面前");
		break;

	case DAS_ONCORPOSE:
		sSitu = _T("地上有尸体出现时");
		break;

	default:
		sSitu = __T("(条件未知)");
		break;
	}

	sTitle.Format("%s (%s) - %s", m_pChar->GetCharName(), m_pChar->GetClassName(), sSitu);
	SetWindowText(sTitle);

	m_list.SetExtendedStyle(LVS_EX_FULLROWSELECT);
	m_list.InsertColumn(0, _T("技能"), LVCFMT_LEFT, 100);
	m_list.InsertColumn(1, _T("左/右"), LVCFMT_LEFT, 50);
	m_list.InsertColumn(2, _T("施放目标"), LVCFMT_LEFT, 150);
	m_list.InsertColumn(3, _T("间隔"), LVCFMT_LEFT, 50);
	m_list.InsertColumn(4, _T("重复"), LVCFMT_LEFT, 50);

	ListActions();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CActionListDlg::ListActions()
{
	m_list.DeleteAllItems();

	const CD2Skill* pSkill = NULL;
	char sz[64] = "";

	for (int i = 0; i < m_pActions->GetSize(); i++)
	{
		pSkill = m_pChar->FindSkill(m_pActions->GetAt(i).GetSkillID());
		if (pSkill != NULL) // skill exists
		{
			char szSkillName[MAX_SKILLNAME_LEN] = "";
			if (pSkill->GetSkillName(szSkillName))
				m_list.InsertItem(i, szSkillName);
		}
		else
		{
			m_list.InsertItem(i, _T("无效技能"));
		}

		m_pActions->GetAt(i).GetSideText(sz);
		m_list.SetItemText(i, 1, sz);

		m_pActions->GetAt(i).GetTargetText(sz);
		m_list.SetItemText(i, 2, sz);

		sz[0] = 0;
		sprintf(sz, "%d", m_pActions->GetAt(i).GetDelay());
		m_list.SetItemText(i, 3, sz);

		sz[0] = 0;
		sprintf(sz, "%d", m_pActions->GetAt(i).GetRepeat());
		m_list.SetItemText(i, 4, sz);		
	}
}

void CActionListDlg::OnAddaction() 
{
	// TODO: Add your control notification handler code here
	CActionDlg dlg(m_pChar, m_nActionSitu);
	if (dlg.DoModal() != IDOK)
		return;

	m_pActions->Add(dlg.GetAction());
	ListActions();
	m_nCurSel = m_pActions->GetSize() - 1;
	m_list.SetItemState(m_nCurSel, LVIS_SELECTED, LVIS_SELECTED);
	CheckStats();
}

void CActionListDlg::OnRemoveaction() 
{
	// TODO: Add your control notification handler code here
	m_pActions->RemoveAt(m_nCurSel);
	if (m_nCurSel >= m_pActions->GetSize())
		m_nCurSel--;
	ListActions();
	m_list.SetItemState(m_nCurSel, LVIS_SELECTED, LVIS_SELECTED);
	CheckStats();
}

void CActionListDlg::OnEditaction() 
{
	// TODO: Add your control notification handler code here
	CActionDlg dlg(m_pChar, m_nActionSitu);
	dlg.SetAction(m_pActions->GetAt(m_nCurSel));
	if (dlg.DoModal() != IDOK)
		return;

	m_pActions->SetAt(m_nCurSel, dlg.GetAction());
	ListActions();
	m_list.SetItemState(m_nCurSel, LVIS_SELECTED, LVIS_SELECTED);
}

void CActionListDlg::OnDblclkList1(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	if (m_nCurSel != -1)
		OnEditaction();
	*pResult = 0;
}

void CActionListDlg::CheckStats()
{
	GetDlgItem(IDC_MOVEUP)->EnableWindow(m_nCurSel != -1);
	GetDlgItem(IDC_MOVEDOWN)->EnableWindow(m_nCurSel != -1);
	GetDlgItem(IDC_REMOVEACTION)->EnableWindow(m_nCurSel != -1);
	GetDlgItem(IDC_EDITACTION)->EnableWindow(m_nCurSel != -1);
	GetDlgItem(IDC_MOVEUP)->EnableWindow(m_nCurSel != -1);
	GetDlgItem(IDC_MOVEUP)->EnableWindow(m_nCurSel != -1);
}

void CActionListDlg::OnMoveup() 
{
	// TODO: Add your control notification handler code here
	if (m_nCurSel <= 0)
		return;
	CD2Action temp = m_pActions->GetAt(m_nCurSel - 1);
	m_pActions->SetAt(m_nCurSel - 1, m_pActions->GetAt(m_nCurSel));
	m_pActions->SetAt(m_nCurSel, temp);
	m_nCurSel--;
	ListActions();
	m_list.SetItemState(m_nCurSel, LVIS_SELECTED, LVIS_SELECTED);
}

void CActionListDlg::OnMovedown() 
{
	// TODO: Add your control notification handler code here
	if (m_nCurSel == -1 || m_nCurSel >= m_pActions->GetSize() - 1)
		return;

	CD2Action temp = m_pActions->GetAt(m_nCurSel + 1);
	m_pActions->SetAt(m_nCurSel + 1, m_pActions->GetAt(m_nCurSel));
	m_pActions->SetAt(m_nCurSel, temp);
	m_nCurSel++;
	ListActions();
	m_list.SetItemState(m_nCurSel, LVIS_SELECTED, LVIS_SELECTED);
}
