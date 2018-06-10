// SafetyDlg.cpp : implementation file
//

#include "stdafx.h"
#include "pindlebot.h"
#include "SafetyDlg.h"
#include "MonsterAttrDlg.h"
#include "d2monster.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSafetyDlg dialog


CSafetyDlg::CSafetyDlg(const CSafetyScheme& ss, CWnd* pParent /*=NULL*/)
	: CDialog(CSafetyDlg::IDD, pParent)
{
	m_ss = ss;
	//{{AFX_DATA_INIT(CSafetyDlg)
	m_nAlertHP = m_ss.m_nAlertHP;
	m_nAlertMP = m_ss.m_nAlertMP;
	m_bMalahHP = m_ss.m_bMalahHP;
	m_bMalahMP = m_ss.m_bMalahMP;
	m_bWatchHP = m_ss.m_bAlertHP;
	m_bWatchMerc = m_ss.m_bAlertMerc;
	m_bWatchMP = m_ss.m_bAlertMP;
	m_nMalahHP = m_ss.m_nMalahHP;
	m_nMalahMP = m_ss.m_nMalahMP;
	m_nAttackDistance = m_ss.m_nDistance;
	m_bWatchPotionHP = ss.m_bPotionHP;
	m_bWatchPotionMP = ss.m_bPotionMP;
	m_bWatchPotionMerc = ss.m_bPotionMerc;
	m_nPotionHP = ss.m_nPotionHP;
	m_nPotionMerc = ss.m_nPotionMerc;
	m_nPotionMP = ss.m_nPotionMP;
	//}}AFX_DATA_INIT
}

void CSafetyDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSafetyDlg)
	DDX_Control(pDX, IDC_POTIONMP, m_wndPotionMP);
	DDX_Control(pDX, IDC_POTIONMERC, m_wndPotionMerc);
	DDX_Control(pDX, IDC_POTIONHP, m_wndPotionHP);
	DDX_Control(pDX, IDC_SPIN_POTIONMP, m_spinPotionMP);
	DDX_Control(pDX, IDC_SPIN_POTIONMERC, m_spinPotionMerc);
	DDX_Control(pDX, IDC_SPIN_POTIONHP, m_spinPotionHP);
	DDX_Control(pDX, IDC_MALAHMP, m_ctlMalahMP);
	DDX_Control(pDX, IDC_MALAHHP, m_ctlMalahHP);
	DDX_Control(pDX, IDC_ALERTMP, m_ctlAlertMP);
	DDX_Control(pDX, IDC_ALERTHP, m_ctlAlertHP);
	DDX_Control(pDX, IDC_SPIN_MALAHMP, m_spinMalahMP);
	DDX_Control(pDX, IDC_SPIN_MALAHHP, m_spinMalahHP);
	DDX_Control(pDX, IDC_SPIN_ALERTMP, m_spinALertMP);
	DDX_Control(pDX, IDC_SPIN_ALERTHP, m_spinAlertHP);
	DDX_Control(pDX, IDC_ATTRLIST, m_AttrList);
	DDX_Text(pDX, IDC_ALERTHP, m_nAlertHP);
	DDV_MinMaxInt(pDX, m_nAlertHP, 0, 100);
	DDX_Text(pDX, IDC_ALERTMP, m_nAlertMP);
	DDV_MinMaxInt(pDX, m_nAlertMP, 0, 100);
	DDX_Check(pDX, IDC_BMALAHHP, m_bMalahHP);
	DDX_Check(pDX, IDC_BMALAHMP, m_bMalahMP);
	DDX_Check(pDX, IDC_BWATCHHP, m_bWatchHP);
	DDX_Check(pDX, IDC_BWATCHMERC, m_bWatchMerc);
	DDX_Check(pDX, IDC_BWATCHMP, m_bWatchMP);
	DDX_Text(pDX, IDC_MALAHHP, m_nMalahHP);
	DDV_MinMaxInt(pDX, m_nMalahHP, 0, 100);
	DDX_Text(pDX, IDC_MALAHMP, m_nMalahMP);
	DDV_MinMaxInt(pDX, m_nMalahMP, 0, 100);
	DDX_Radio(pDX, IDC_RADIO1, m_nAttackDistance);
	DDX_Check(pDX, IDC_WATCHPOTIONHP, m_bWatchPotionHP);
	DDX_Check(pDX, IDC_WATCHPOTIONMP, m_bWatchPotionMP);
	DDX_Check(pDX, IDC_WATCHPOTIONMERC, m_bWatchPotionMerc);
	DDX_Text(pDX, IDC_POTIONHP, m_nPotionHP);
	DDV_MinMaxInt(pDX, m_nPotionHP, 0, 100);
	DDX_Text(pDX, IDC_POTIONMERC, m_nPotionMerc);
	DDV_MinMaxInt(pDX, m_nPotionMerc, 0, 100);
	DDX_Text(pDX, IDC_POTIONMP, m_nPotionMP);
	DDV_MinMaxInt(pDX, m_nPotionMP, 0, 100);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSafetyDlg, CDialog)
	//{{AFX_MSG_MAP(CSafetyDlg)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_ATTRLIST, OnItemchangedAttrlist)
	ON_BN_CLICKED(IDC_ADDCOMBO, OnAddcombo)
	ON_BN_CLICKED(IDC_REMOVECOMBO, OnRemovecombo)
	ON_BN_CLICKED(IDC_IMMUNITY, OnImmunity)
	ON_BN_CLICKED(IDC_BMALAHHP, CheckStats)
	ON_BN_CLICKED(IDC_BMALAHMP, CheckStats)
	ON_BN_CLICKED(IDC_BWATCHHP, CheckStats)
	ON_BN_CLICKED(IDC_BWATCHMP, CheckStats)
	ON_BN_CLICKED(IDC_WATCHPOTIONHP, CheckStats)
	ON_BN_CLICKED(IDC_WATCHPOTIONMP, CheckStats)
	ON_BN_CLICKED(IDC_WATCHPOTIONMERC, CheckStats)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSafetyDlg message handlers

BOOL CSafetyDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	CString sTitle;
	sTitle.Format(_T("角色安全方案 - <%s>"), m_ss.GetName());
	this->SetWindowText(sTitle);
	
	const int WIDTH = 167;
	m_AttrList.InsertColumn(0, _T("属性1"), LVCFMT_LEFT, WIDTH);
	m_AttrList.InsertColumn(1, _T("属性2"), LVCFMT_LEFT, WIDTH);

	m_spinAlertHP.SetRange(0, 100);
	m_spinALertMP.SetRange(0, 100);
	m_spinMalahHP.SetRange(0, 100);
	m_spinMalahMP.SetRange(0, 100);
	m_spinPotionHP.SetRange(0, 100);
	m_spinPotionMP.SetRange(0, 100);
	m_spinPotionMerc.SetRange(0, 100);

	m_ss.ListCombos(m_AttrList);

	CheckStats();

	UpdateData(FALSE);

	// CG: The following block was added by the ToolTips component.
	{
		// Create the ToolTip control.
		m_tooltip.Create(this);
		m_tooltip.Activate(TRUE);

		// TODO: Use one of the following forms to add controls:
		// m_tooltip.AddTool(GetDlgItem(IDC_<name>), <string-table-id>);
		// m_tooltip.AddTool(GetDlgItem(IDC_<name>), "<text>");
		m_tooltip.AddTool(GetDlgItem(IDC_ATTRLIST), _T("怪物属性组合列表。如果怪物属性符合列表中的任一横列，角色将立即退出游戏。"));
		m_tooltip.AddTool(GetDlgItem(IDC_ADDCOMBO), _T("添加一组属性组合"));
		m_tooltip.AddTool(GetDlgItem(IDC_REMOVECOMBO), _T("删除一组已定义的属性组合"));
		m_tooltip.AddTool(GetDlgItem(IDC_RADIO1), _T("正常距离，适合绝大多数角色"));
		m_tooltip.AddTool(GetDlgItem(IDC_RADIO2), _T("稍近于正常距离，适合肉搏型角色以及短射程魔法（如Nova）角色"));
		m_tooltip.AddTool(GetDlgItem(IDC_RADIO3), _T("稍远于正常距离，适合远程攻击型角色，安全性最高"));
		m_tooltip.AddTool(GetDlgItem(IDC_IMMUNITY), _T("查询地狱难度Pindleskin成为各种免疫的条件"));
	}
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CSafetyDlg::OnItemchangedAttrlist(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	// TODO: Add your control notification handler code here
	GetDlgItem(IDC_REMOVECOMBO)->EnableWindow(m_AttrList.GetSelectedCount() > 0);
	*pResult = 0;
}

void CSafetyDlg::OnOK() 
{
	// TODO: Add extra validation here
	UpdateData(TRUE);
	if (!ValidatePercentage())
		return;

	m_ss.m_nAlertHP = m_nAlertHP;
	m_ss.m_nAlertMP = m_nAlertMP;
	m_ss.m_bMalahHP = m_bMalahHP;
	m_ss.m_bMalahMP = m_bMalahMP;
	m_ss.m_bAlertHP = m_bWatchHP;
	m_ss.m_bAlertMerc = m_bWatchMerc;
	m_ss.m_bAlertMP = m_bWatchMP;
	m_ss.m_nMalahHP = m_nMalahHP;
	m_ss.m_nMalahMP = m_nMalahMP;
	m_ss.m_nDistance = m_nAttackDistance;

	m_ss.m_bPotionHP = m_bWatchPotionHP;
	m_ss.m_bPotionMP = m_bWatchPotionMP;
	m_ss.m_bPotionMerc = m_bWatchPotionMerc;
	m_ss.m_nPotionHP = m_nPotionHP;
	m_ss.m_nPotionMerc = m_nPotionMerc;
	m_ss.m_nPotionMP = m_nPotionMP;

	EndDialog(IDOK);
}

void CSafetyDlg::OnAddcombo() 
{
	// TODO: Add your control notification handler code here
	CMonsterAttrDlg dlg;
	if (dlg.DoModal() == IDOK)
	{
		m_ss.m_aAttr1.Add(dlg.GetAttr1());
		m_ss.m_aAttr2.Add(dlg.GetAttr2());
		m_ss.ListCombos(m_AttrList);
	}
}

void CSafetyDlg::OnRemovecombo() 
{
	// TODO: Add your control notification handler code here
	const int IDX = m_AttrList.GetFirstSelectedItem();
	if (IDX >= 0 && IDX < m_AttrList.GetItemCount())
	{
		m_AttrList.DeleteItem(IDX, TRUE);
		m_ss.m_aAttr1.RemoveAt(IDX);
		m_ss.m_aAttr2.RemoveAt(IDX);
	}
}

void CSafetyDlg::OnImmunity() 
{
	// TODO: Add your control notification handler code here
	MessageBox(_T("物理免疫：石头皮肤-Stone Skin + 任何-Any\n火系免役：魔法抵抗-Magic Resistant + 任何-Any\n冰系免疫：魔法抵抗-Magic Resistant + 冰冻增强-Cold Enchanted\n电系免疫：魔法抵抗-Magic Resistant + 闪电增强-Lightning Enchanted\n毒系免疫：永远"), _T("地狱难度Pindleskin各项免疫充要条件"));
}

BOOL CSafetyDlg::PreTranslateMessage(MSG* pMsg)
{
	// CG: The following block was added by the ToolTips component.
	{
		// Let the ToolTip process this message.
		m_tooltip.RelayEvent(pMsg);
	}
	return CDialog::PreTranslateMessage(pMsg);	// CG: This was added by the ToolTips component.
}

void CSafetyDlg::CheckStats()
{
	UpdateData(TRUE);
	m_ctlMalahHP.EnableWindow(m_bMalahHP);
	m_ctlMalahMP.EnableWindow(m_bMalahMP);
	m_ctlAlertHP.EnableWindow(m_bWatchHP);
	m_ctlAlertMP.EnableWindow(m_bWatchMP);
	m_wndPotionHP.EnableWindow(m_bWatchPotionHP);
	m_wndPotionMP.EnableWindow(m_bWatchPotionMP);
	m_wndPotionMerc.EnableWindow(m_bWatchPotionMerc);
}

CSafetyScheme CSafetyDlg::GetScheme() const
{
	return m_ss;
}

BOOL CSafetyDlg::ValidatePercentage() const
{
	return IsPercentageValid(m_nMalahHP)
		&& IsPercentageValid(m_nMalahMP)
		&& IsPercentageValid(m_nAlertHP)
		&& IsPercentageValid(m_nAlertMP)
		&& IsPercentageValid(m_nPotionHP)
		&& IsPercentageValid(m_nPotionMP)
		&& IsPercentageValid(m_nPotionMerc);
}

BOOL CSafetyDlg::IsPercentageValid(int n) const
{
	return n >= 0 && n <= 100;
}
