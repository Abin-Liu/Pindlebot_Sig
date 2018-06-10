// MonsterAttrDlg.cpp : implementation file
//

#include "stdafx.h"
#include "pindlebot.h"
#include "MonsterAttrDlg.h"
#include "d2monster.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMonsterAttrDlg dialog


CMonsterAttrDlg::CMonsterAttrDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CMonsterAttrDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CMonsterAttrDlg)
	//}}AFX_DATA_INIT
	m_wAttr1 = PINDLE_BONUS_NONE;
	m_wAttr2 = PINDLE_BONUS_NONE;
}


void CMonsterAttrDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMonsterAttrDlg)
	DDX_Control(pDX, IDC_COMBO2, m_wndAttr2);
	DDX_Control(pDX, IDC_COMBO1, m_wndAttr1);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CMonsterAttrDlg, CDialog)
	//{{AFX_MSG_MAP(CMonsterAttrDlg)
	ON_CBN_SELCHANGE(IDC_COMBO1, OnSelchangeCombo)
	ON_CBN_SELCHANGE(IDC_COMBO2, OnSelchangeCombo)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMonsterAttrDlg message handlers


void CMonsterAttrDlg::OnSelchangeCombo() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	GetDlgItem(IDOK)->EnableWindow(m_wndAttr1.GetCurSel() != m_wndAttr2.GetCurSel());
}

BOOL CMonsterAttrDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	InitAttributes();
	m_wndAttr1.SetCurSel(0);
	m_wndAttr2.SetCurSel(0);
	// TODO: Add extra initialization here
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CMonsterAttrDlg::InitAttributes()
{
	int n = -1;
	n = m_wndAttr1.AddString(_T("�κ�-Any"));
	m_wndAttr1.SetItemData(n, PINDLE_BONUS_NONE);

	n = m_wndAttr1.AddString(_T("������ǿ-Cold Enchanted"));
	m_wndAttr1.SetItemData(n, PINDLE_BONUS_COLD_ENCHANTED);

	n = m_wndAttr1.AddString(_T("����-Cursed"));
	m_wndAttr1.SetItemData(n, PINDLE_BONUS_CURSED);

	n = m_wndAttr1.AddString(_T("��������-Extra Fast"));
	m_wndAttr1.SetItemData(n, PINDLE_BONUS_EXTRA_FAST);

	n = m_wndAttr1.AddString(_T("����ǿ׳-Extra Strong"));
	m_wndAttr1.SetItemData(n, PINDLE_BONUS_EXTRA_STRONG);

	n = m_wndAttr1.AddString(_T("������ǿ-Lightning Enchanted"));
	m_wndAttr1.SetItemData(n, PINDLE_BONUS_LIGHTNING_ENCHANTED);

	n = m_wndAttr1.AddString(_T("ħ���ֿ�-Magic Resistant"));
	m_wndAttr1.SetItemData(n, PINDLE_BONUS_MAGIC_RESISTANT);

	n = m_wndAttr1.AddString(_T("����ȼ��-Mana Burn"));
	m_wndAttr1.SetItemData(n, PINDLE_BONUS_MANA_BURN);

	n = m_wndAttr1.AddString(_T("�������-Multishot"));
	m_wndAttr1.SetItemData(n, PINDLE_BONUS_MULTISHOT);

	n = m_wndAttr1.AddString(_T("������-Spectral Hit"));
	m_wndAttr1.SetItemData(n, PINDLE_BONUS_SPECTRAL_HIT);

	n = m_wndAttr1.AddString(_T("ʯͷƤ��-Stone Skin"));
	m_wndAttr1.SetItemData(n, PINDLE_BONUS_STONE_SKIN);

	n = m_wndAttr1.AddString(_T("˲��ת��-Teleport"));
	m_wndAttr1.SetItemData(n, PINDLE_BONUS_TELEPORT);

	n = m_wndAttr1.AddString(_T("��׼�⻷-Blessed Aim Aura"));
	m_wndAttr1.SetItemData(n, UNIT_AURA_BLESSEDAIM);

	n = m_wndAttr1.AddString(_T("���й⻷-Conviction Aura"));
	m_wndAttr1.SetItemData(n, UNIT_AURA_CONVICTION);

	n = m_wndAttr1.AddString(_T("���ȹ⻷-Fanaticism Aura"));
	m_wndAttr1.SetItemData(n, UNIT_AURA_FANATICISM);

	n = m_wndAttr1.AddString(_T("ʥ��⻷-Holy Fire Aura"));
	m_wndAttr1.SetItemData(n, UNIT_AURA_HOLYFIRE);

	n = m_wndAttr1.AddString(_T("ʥ���⻷-Holy Freeze Aura"));
	m_wndAttr1.SetItemData(n, UNIT_AURA_HOLYFREEZE);

	n = m_wndAttr1.AddString(_T("ʥ��⻷-Holy Shock Aura"));
	m_wndAttr1.SetItemData(n, UNIT_AURA_HOLYSHOCK);

	n = m_wndAttr1.AddString(_T("�����⻷-Might Aura"));
	m_wndAttr1.SetItemData(n, UNIT_AURA_MIGHT);


	n = m_wndAttr2.AddString(_T("�κ�-Any"));
	m_wndAttr2.SetItemData(n, PINDLE_BONUS_NONE);

	n = m_wndAttr2.AddString(_T("������ǿ-Cold Enchanted"));
	m_wndAttr2.SetItemData(n, PINDLE_BONUS_COLD_ENCHANTED);

	n = m_wndAttr2.AddString(_T("����-Cursed"));
	m_wndAttr2.SetItemData(n, PINDLE_BONUS_CURSED);

	n = m_wndAttr2.AddString(_T("��������-Extra Fast"));
	m_wndAttr2.SetItemData(n, PINDLE_BONUS_EXTRA_FAST);

	n = m_wndAttr2.AddString(_T("����ǿ׳-Extra Strong"));
	m_wndAttr2.SetItemData(n, PINDLE_BONUS_EXTRA_STRONG);

	n = m_wndAttr2.AddString(_T("������ǿ-Lightning Enchanted"));
	m_wndAttr2.SetItemData(n, PINDLE_BONUS_LIGHTNING_ENCHANTED);

	n = m_wndAttr2.AddString(_T("ħ���ֿ�-Magic Resistant"));
	m_wndAttr2.SetItemData(n, PINDLE_BONUS_MAGIC_RESISTANT);

	n = m_wndAttr2.AddString(_T("����ȼ��-Mana Burn"));
	m_wndAttr2.SetItemData(n, PINDLE_BONUS_MANA_BURN);

	n = m_wndAttr2.AddString(_T("�������-Multishot"));
	m_wndAttr2.SetItemData(n, PINDLE_BONUS_MULTISHOT);

	n = m_wndAttr2.AddString(_T("������-Spectral Hit"));
	m_wndAttr2.SetItemData(n, PINDLE_BONUS_SPECTRAL_HIT);

	n = m_wndAttr2.AddString(_T("ʯͷƤ��-Stone Skin"));
	m_wndAttr2.SetItemData(n, PINDLE_BONUS_STONE_SKIN);

	n = m_wndAttr2.AddString(_T("˲��ת��-Teleport"));
	m_wndAttr2.SetItemData(n, PINDLE_BONUS_TELEPORT);

	n = m_wndAttr2.AddString(_T("��׼�⻷-Blessed Aim Aura"));
	m_wndAttr2.SetItemData(n, UNIT_AURA_BLESSEDAIM);

	n = m_wndAttr2.AddString(_T("���й⻷-Conviction Aura"));
	m_wndAttr2.SetItemData(n, UNIT_AURA_CONVICTION);

	n = m_wndAttr2.AddString(_T("���ȹ⻷-Fanaticism Aura"));
	m_wndAttr2.SetItemData(n, UNIT_AURA_FANATICISM);

	n = m_wndAttr2.AddString(_T("ʥ��⻷-Holy Fire Aura"));
	m_wndAttr2.SetItemData(n, UNIT_AURA_HOLYFIRE);

	n = m_wndAttr2.AddString(_T("ʥ���⻷-Holy Freeze Aura"));
	m_wndAttr2.SetItemData(n, UNIT_AURA_HOLYFREEZE);

	n = m_wndAttr2.AddString(_T("ʥ��⻷-Holy Shock Aura"));
	m_wndAttr2.SetItemData(n, UNIT_AURA_HOLYSHOCK);

	n = m_wndAttr2.AddString(_T("�����⻷-Might Aura"));
	m_wndAttr2.SetItemData(n, UNIT_AURA_MIGHT);
}

void CMonsterAttrDlg::OnOK() 
{
	// TODO: Add extra validation here
	m_wAttr1 = WORD(m_wndAttr1.GetItemData(m_wndAttr1.GetCurSel()));
	m_wAttr2 = WORD(m_wndAttr2.GetItemData(m_wndAttr2.GetCurSel()));
	CDialog::OnOK();
}

WORD CMonsterAttrDlg::GetAttr1() const
{
	return m_wAttr1;
}

WORD CMonsterAttrDlg::GetAttr2() const
{
	return m_wAttr2;
}
