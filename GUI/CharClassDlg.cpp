// CharClassDlg.cpp : implementation file
//

#include "stdafx.h"
#include "pindlebot.h"
#include "CharClassDlg.h"
#include "NewCharDlg.h"
#include "ActionListDlg.h"
#include "SafetyDlg.h"
#include "SafetySchemeNameDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCharClassDlg dialog

IMPLEMENT_DYNCREATE(CCharClassDlg, CPropertyPage)

CCharClassDlg::CCharClassDlg() : CPropertyPage (CCharClassDlg::IDD)
{
	//{{AFX_DATA_INIT(CCharClassDlg)
	//}}AFX_DATA_INIT

	m_bActivated = FALSE;
	m_ini.SetPathName(g_sAppDir + _T("\\CharSpec.dat"));
}


void CCharClassDlg::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCharClassDlg)
	DDX_Control(pDX, IDC_CHARLIST, m_list);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCharClassDlg, CPropertyPage)
	//{{AFX_MSG_MAP(CCharClassDlg)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_CHARLIST, OnItemchangedCharlist)
	ON_BN_CLICKED(IDC_NEWCHAR, OnNewchar)
	ON_BN_CLICKED(IDC_DELCHAR, OnDelchar)
	ON_BN_CLICKED(IDC_BEFORERED, OnBeforered)
	ON_BN_CLICKED(IDC_AFTERRED, OnAfterred)
	ON_BN_CLICKED(IDC_ONPINDLESKIN, OnOnpindleskin)
	ON_BN_CLICKED(IDC_ONCORPOSE, OnOncorpose)
	ON_BN_CLICKED(IDC_EDITCHAR, OnEditchar)
	ON_NOTIFY(NM_DBLCLK, IDC_CHARLIST, OnDblclkCharlist)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCharClassDlg message handlers

BOOL CCharClassDlg::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	// TODO: Add extra initialization here	
	InitSchemes();
	m_list.InsertColumn(0, _T("角色名"), LVCFMT_LEFT, 90);
	m_list.InsertColumn(1, _T("职业"), LVCFMT_LEFT, 70);
	m_list.InsertColumn(2, _T("安全方案"), LVCFMT_LEFT, 70);
	m_list.InsertColumn(3, _T("消耗"), LVCFMT_LEFT, 50);
	m_list.InsertColumn(4, _T("购买"), LVCFMT_LEFT, 40);
	ListSchemes();
	CheckStats();
	// CG: The following block was added by the ToolTips component.
	{
		// Create the ToolTip control.
		m_tooltip.Create(this);
		m_tooltip.Activate(TRUE);

		// TODO: Use one of the following forms to add controls:
		// m_tooltip.AddTool(GetDlgItem(IDC_<name>), <string-table-id>);
		// m_tooltip.AddTool(GetDlgItem(IDC_<name>), "<text>");
		
		m_tooltip.AddTool(GetDlgItem(IDC_NEWCHAR), _T("增添新角色"));
		m_tooltip.AddTool(GetDlgItem(IDC_DELCHAR), _T("删除为选定角色所设置的所有技能方案"));
		m_tooltip.AddTool(GetDlgItem(IDC_BEFORERED), _T("城内准备阶段，适用于允许在城中施放的辅助性技能"));
		m_tooltip.AddTool(GetDlgItem(IDC_AFTERRED), _T("城外准备阶段，适用于不允许在城中施放的辅助性技能"));
		m_tooltip.AddTool(GetDlgItem(IDC_ONPINDLESKIN), _T("正式攻击阶段"));
		m_tooltip.AddTool(GetDlgItem(IDC_ONCORPOSE), _T("尸体相关技能，仅限于Necromancer和Barbarian"));
	}
	m_bActivated = TRUE;
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CCharClassDlg::ListSchemes()
{
	m_list.DeleteAllItems();
	int i = 0;

	for (i = 0; i < m_aSchemes.GetSize(); i++)
	{
		m_list.InsertItem(i, m_aSchemes[i].m_szCharName);
		m_list.SetItemText(i, 1, CHARCLASSNAME[m_aSchemes[i].m_byCharClass]);
		m_list.SetItemText(i, 2, m_aSchemes[i].m_szSafetyScheme);
		CString sAmmo(_T("None"));
		if (m_aSchemes[i].m_nAmmoType == AMMO_ARROW)
			sAmmo = _T("Arrow");
		else if (m_aSchemes[i].m_nAmmoType == AMMO_BOLT)
			sAmmo = _T("Bolt");
		m_list.SetItemText(i, 3, sAmmo);
		m_list.SetItemText(i, 4, m_aSchemes[i].m_nAmmoBuyQty);
	}
}

void CCharClassDlg::InitSchemes()
{
	m_aSchemes.RemoveAll();
	
	CStringArray aCharNames;
	m_ini.GetProfileSectionNames(aCharNames);

	BYTE byClass = CCD_UNKNOWN;
	int nAmmoType = AMMO_NONE;
	int nAmmoBuyQty = 2;
	CString sSafetyScheme;
	int nCountBeforeRed = 0;
	int nCountAfterRed = 0;
	int nCountOnPindleskin = 0;
	int nCountOnCorpose = 0;
	CString sEntry;
	CString sValue;

	int i = 0;
	int j = 0;

	// Read all schemes from file
	for (i = 0; i < aCharNames.GetSize(); i++)
	{
		byClass = m_ini.GetProfileInt(aCharNames[i], _T("Class"), CCD_UNKNOWN);
		sSafetyScheme = m_ini.GetProfileString(aCharNames[i], _T("Safety Scheme"), _T("Default"));
		
		if (!VerifySafetyScheme(sSafetyScheme))
			sSafetyScheme = _T("Default");

		nAmmoType = m_ini.GetProfileInt(aCharNames[i], _T("Ammo Type"), AMMO_NONE);
		nAmmoBuyQty = m_ini.GetProfileInt(aCharNames[i], _T("Ammo Buy Qty"), 2);
		if (byClass < CCD_UNKNOWN)
			m_aSchemes.Add(CD2Scheme(aCharNames[i], byClass, sSafetyScheme, nAmmoType, nAmmoBuyQty));
	}

	// populate valid schemes
	for (i = 0; i < m_aSchemes.GetSize(); i++)
	{
		// Read counters for each action list
		nCountBeforeRed = m_ini.GetProfileInt(m_aSchemes[i].m_szCharName, _T("Action0"), 0);
		nCountAfterRed = m_ini.GetProfileInt(m_aSchemes[i].m_szCharName, _T("Action1"), 0);
		nCountOnPindleskin = m_ini.GetProfileInt(m_aSchemes[i].m_szCharName, _T("Action2"), 0);
		nCountOnCorpose = m_ini.GetProfileInt(m_aSchemes[i].m_szCharName, _T("Action3"), 0);

		m_aSchemes[i].m_aBeforeRed.SetSize(nCountBeforeRed);
		m_aSchemes[i].m_aAfterRed.SetSize(nCountAfterRed);
		m_aSchemes[i].m_aOnPindleskin.SetSize(nCountOnPindleskin);
		m_aSchemes[i].m_aOnCorpose.SetSize(nCountOnCorpose);

		// populate "before red" actions
		for (j = 0; j < nCountBeforeRed; j++)
		{
			// specified action entries: "action0x"
			sEntry.Format("Action0%d", j);
			sValue = m_ini.GetProfileString(m_aSchemes[i].m_szCharName, sEntry, _T(""));
			m_aSchemes[i].m_aBeforeRed[j].ParseString(sValue);
		}

		// populate "after red" actions
		for (j = 0; j < nCountAfterRed; j++)
		{
			// specified action entries: "action1x"
			sEntry.Format("Action1%d", j);
			sValue = m_ini.GetProfileString(m_aSchemes[i].m_szCharName, sEntry, _T(""));
			m_aSchemes[i].m_aAfterRed[j].ParseString(sValue);
		}

		// populate "on pindleskin" actions
		for (j = 0; j < nCountOnPindleskin; j++)
		{
			// specified action entries: "action2x"
			sEntry.Format("Action2%d", j);
			sValue = m_ini.GetProfileString(m_aSchemes[i].m_szCharName, sEntry, _T(""));
			m_aSchemes[i].m_aOnPindleskin[j].ParseString(sValue);
		}

		// populate "on corpose" actions
		for (j = 0; j < nCountOnCorpose; j++)
		{
			// specified action entries: "action3x"
			sEntry.Format("Action3%d", j);
			sValue = m_ini.GetProfileString(m_aSchemes[i].m_szCharName, sEntry, _T(""));
			m_aSchemes[i].m_aOnCorpose[j].ParseString(sValue);
		}
	}
}

void CCharClassDlg::OnItemchangedCharlist(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	// TODO: Add your control notification handler code here
	CString sName;
	const int IDX = m_list.GetFirstSelectedItem();
	if (IDX != -1)
		sName = m_list.GetItemText(IDX, 0);

	CString str;
	str.Format(_T("技能方案<%s>"), sName);

	SetDlgItemText(IDC_SKILLSCHEME, str);
	CheckStats();
	*pResult = 0;
}

void CCharClassDlg::CheckStats()
{
	const int IDX = m_list.GetFirstSelectedItem();
	const BOOL VALID = IDX >= 0 && IDX < m_aSchemes.GetSize();
	GetDlgItem(IDC_DELCHAR)->EnableWindow(VALID);
	GetDlgItem(IDC_EDITCHAR)->EnableWindow(VALID);
	GetDlgItem(IDC_BEFORERED)->EnableWindow(VALID && m_aSchemes[IDX].m_byCharClass != CCD_BARBARIAN);
	GetDlgItem(IDC_AFTERRED)->EnableWindow(VALID);
	GetDlgItem(IDC_ONCORPOSE)->EnableWindow(VALID && (m_aSchemes[IDX].m_byCharClass == CCD_NECROMANCER || m_aSchemes[IDX].m_byCharClass == CCD_BARBARIAN));
	GetDlgItem(IDC_ONPINDLESKIN)->EnableWindow(VALID);
}

void CCharClassDlg::OnNewchar() 
{
	// TODO: Add your control notification handler code here
	CNewCharDlg dlg;
	if (dlg.DoModal() != IDOK)
		return;

	for (int i = 0; i < m_aSchemes.GetSize(); i++)
	{
		if (dlg.GetCharName().CompareNoCase(m_aSchemes[i].m_szCharName) == 0)
		{
			CString sMsg;
			sMsg.Format(_T("角色名\"%s\"已存在，不能添加同名角色。"), dlg.GetCharName());
			MessageBox(sMsg, _T("命名冲突"), MB_ICONEXCLAMATION);
			return;
		}
	}

	m_aSchemes.Add(CD2Scheme(dlg.GetCharName(), dlg.GetCharClass(), dlg.GetSafetyName(), dlg.GetAmmoType(), dlg.GetAmmoBuyQty()));
	ListSchemes();
	CheckStats();
}

void CCharClassDlg::OnDelchar() 
{
	// TODO: Add your control notification handler code here]
	const int IDX = m_list.GetFirstSelectedItem();
	if (IDX < 0 || IDX >= m_aSchemes.GetSize())
		return;

	CString sMsg;
	sMsg.Format("该操作将删除所有与角色“%s”有关的设置，是否继续？", m_aSchemes[IDX].m_szCharName);
	int nRes = MessageBox(sMsg, _T("删除设置"), MB_ICONQUESTION | MB_YESNO);
	if (nRes != IDYES)
		return;

	m_aSchemes.RemoveAt(IDX);
	ListSchemes();
}

void CCharClassDlg::OnBeforered() 
{
	// TODO: Add your control notification handler code here
	const int IDX = m_list.GetFirstSelectedItem();
	if (IDX < 0 || IDX >= m_aSchemes.GetSize())
		return;

	CD2Character c(m_aSchemes[IDX].m_szCharName, m_aSchemes[IDX].m_byCharClass);
	CActionListDlg dlg(&c, &m_aSchemes[IDX].m_aBeforeRed, DAS_BEFORERED);
	dlg.DoModal();
}

void CCharClassDlg::OnAfterred() 
{
	// TODO: Add your control notification handler code here
	const int IDX = m_list.GetFirstSelectedItem();
	if (IDX < 0 || IDX >= m_aSchemes.GetSize())
		return;

	CD2Character c(m_aSchemes[IDX].m_szCharName, m_aSchemes[IDX].m_byCharClass);
	CActionListDlg dlg(&c, &m_aSchemes[IDX].m_aAfterRed, DAS_AFTERRED);
	dlg.DoModal();
}

void CCharClassDlg::OnOnpindleskin() 
{
	// TODO: Add your control notification handler code here
	const int IDX = m_list.GetFirstSelectedItem();
	if (IDX < 0 || IDX >= m_aSchemes.GetSize())
		return;

	CD2Character c(m_aSchemes[IDX].m_szCharName, m_aSchemes[IDX].m_byCharClass);
	CActionListDlg dlg(&c, &m_aSchemes[IDX].m_aOnPindleskin, DAS_ONPINDLESKIN);
	dlg.DoModal();
}

void CCharClassDlg::OnOncorpose() 
{
	// TODO: Add your control notification handler code here
	const int IDX = m_list.GetFirstSelectedItem();
	if (IDX < 0 || IDX >= m_aSchemes.GetSize())
		return;

	CD2Character c(m_aSchemes[IDX].m_szCharName, m_aSchemes[IDX].m_byCharClass);
	CActionListDlg dlg(&c, &m_aSchemes[IDX].m_aOnCorpose, DAS_ONCORPOSE);
	dlg.DoModal();
}

void CCharClassDlg::ApplySettings()
{
	if (!m_bActivated)
		return;

	::DeleteFile(m_ini.GetPathName());

	// Loop through all schemes in scheme array and write data into file
	// Remove deleted schemes first
	int i = 0;
	int j = 0;
	
	// Apply other schemes
	LPCSTR lpSect = NULL; // Section name
	int nActCount = 0; // Action Counter
	CString sEntry;
	char szBuffer[128] = _T("");

	for (i = 0; i < m_aSchemes.GetSize(); i++)
	{
		lpSect = m_aSchemes[i].m_szCharName;

		// Write character class value
		m_ini.WriteProfileInt(lpSect, _T("Class"), m_aSchemes[i].m_byCharClass);

		// safety scheme name
		m_ini.WriteProfileString(lpSect, _T("Safety Scheme"), m_aSchemes[i].GetSafetyScheme());

		// ammo type & buy qty
		m_ini.WriteProfileInt(lpSect, _T("Ammo Type"), m_aSchemes[i].m_nAmmoType);
		m_ini.WriteProfileInt(lpSect, _T("Ammo Buy Qty"), m_aSchemes[i].m_nAmmoBuyQty);

		// Apply "before red" actions

		// Action counter
		nActCount = m_aSchemes[i].m_aBeforeRed.GetSize();
		m_ini.WriteProfileInt(lpSect, _T("Action0"), nActCount);

		// Apply each action in array
		for (j = 0; j < nActCount; j++)
		{
			m_aSchemes[i].m_aBeforeRed[j].ToString(szBuffer);
			sEntry.Format("Action0%d", j);
			m_ini.WriteProfileString(lpSect, sEntry, szBuffer);
		}

		// Apply "after red" actions

		// Action counter
		nActCount = m_aSchemes[i].m_aAfterRed.GetSize();
		m_ini.WriteProfileInt(lpSect, _T("Action1"), nActCount);

		// Apply each action in array
		for (j = 0; j < nActCount; j++)
		{
			m_aSchemes[i].m_aAfterRed[j].ToString(szBuffer);
			sEntry.Format("Action1%d", j);
			m_ini.WriteProfileString(lpSect, sEntry, szBuffer);
		}

		// Apply "On pindleskin" actions

		// Action counter
		nActCount = m_aSchemes[i].m_aOnPindleskin.GetSize();
		m_ini.WriteProfileInt(lpSect, _T("Action2"), nActCount);

		// Apply each action in array
		for (j = 0; j < nActCount; j++)
		{
			m_aSchemes[i].m_aOnPindleskin[j].ToString(szBuffer);
			sEntry.Format("Action2%d", j);
			m_ini.WriteProfileString(lpSect, sEntry, szBuffer);
		}

		// Apply "on corpose" actions

		// Action counter
		nActCount = m_aSchemes[i].m_aOnCorpose.GetSize();
		m_ini.WriteProfileInt(lpSect, _T("Action3"), nActCount);

		// Apply each action in array
		for (j = 0; j < nActCount; j++)
		{
			m_aSchemes[i].m_aOnCorpose[j].ToString(szBuffer);
			sEntry.Format("Action3%d", j);
			m_ini.WriteProfileString(lpSect, sEntry, szBuffer);
		}
	}
}

BOOL CCharClassDlg::PreTranslateMessage(MSG* pMsg)
{
	// CG: The following block was added by the ToolTips component.
	{
		// Let the ToolTip process this message.
		m_tooltip.RelayEvent(pMsg);
	}
	return CPropertyPage::PreTranslateMessage(pMsg);	// CG: This was added by the ToolTips component.
}

void CCharClassDlg::OnEditchar() 
{
	// TODO: Add your control notification handler code here
	const int IDX = m_list.GetFirstSelectedItem();
	if (IDX < 0 || IDX >= m_aSchemes.GetSize())
		return;

	CNewCharDlg dlg(m_aSchemes[IDX].m_szCharName, m_aSchemes[IDX].m_byCharClass, m_aSchemes[IDX].m_szSafetyScheme, m_aSchemes[IDX].m_nAmmoType, m_aSchemes[IDX].m_nAmmoBuyQty);
	if (dlg.DoModal() != IDOK)
		return;

	if (dlg.GetCharName().CompareNoCase(m_aSchemes[IDX].m_szCharName) != 0)
	{
		for (int i = 0; i < m_aSchemes.GetSize(); i++)
		{
			if (dlg.GetCharName().CompareNoCase(m_aSchemes[i].m_szCharName) == 0)
			{
				CString sMsg;
				sMsg.Format(_T("角色名\"%s\"已存在，不能添加同名角色。"), dlg.GetCharName());
				MessageBox(sMsg, _T("命名冲突"), MB_ICONEXCLAMATION);
				return;
			}
		}
	}

	m_aSchemes[IDX].SetScheme(dlg.GetCharName(), dlg.GetCharClass(), dlg.GetSafetyName(), dlg.GetAmmoType(), dlg.GetAmmoBuyQty());
	ListSchemes();
	CheckStats();
}

void CCharClassDlg::OnDblclkCharlist(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	OnEditchar();
	*pResult = 0;
}

BOOL CCharClassDlg::OnKillActive()
{
	// TODO: Add your specialized code here and/or call the base class
	for (int i = 0; i < m_aSchemes.GetSize(); i++)
	{
		if (m_aSchemes[i].IsSkillEmpty())
		{
			CString sMsg;
			sMsg.Format(_T("你没有为角色\"%s\"设置任何技能，这样角色在游戏中将无法杀死怪物。确定要继续吗？"), m_aSchemes[i].m_szCharName);
			if (MessageBox(sMsg, _T("空白技能方案"), MB_ICONQUESTION | MB_YESNO) == IDNO)
				return FALSE;
			else
				return CPropertyPage::OnKillActive();
		}
	}
	
	return CPropertyPage::OnKillActive();
}

BOOL CCharClassDlg::VerifySafetyScheme(LPCTSTR lpName) const
{
	for (int i = 0; i < g_aSafetySchemes.GetSize(); i++)
	{
		if (g_aSafetySchemes[i].GetName().CompareNoCase(lpName) == 0)
			return TRUE;
	}

	return FALSE;
}

void CCharClassDlg::CheckChangeInvalidSafetyScheme()
{
	if (m_bActivated)
	{
		for (int i = 0; i < m_aSchemes.GetSize(); i++)
		{
			if (!VerifySafetyScheme(m_aSchemes[i].m_szSafetyScheme))
			{
				_tcscpy(m_aSchemes[i].m_szSafetyScheme, _T("Default"));
				m_list.SetItemText(i, 2, _T("Default"));
			}
		}
	}
}

BOOL CCharClassDlg::OnSetActive()
{
	CheckChangeInvalidSafetyScheme();
	return CPropertyPage::OnSetActive();
}
