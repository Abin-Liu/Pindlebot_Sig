// ActionDlg.cpp : implementation file
//

#include "stdafx.h"
#include "pindlebot.h"
#include "ActionDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CActionDlg dialog


CActionDlg::CActionDlg(const CD2Character* pChar, int nSituation, CWnd* pParent /*=NULL*/)
	: CDialog(CActionDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CActionDlg)
	m_nTarget = CAT_NONE;
	m_nCastDelay = 0;
	m_nSide = SAS_RIGHT;
	m_nRepeat = 1;
	m_sCoolDown = _T("该技能冷却时间为：0 毫秒。");
	//}}AFX_DATA_INIT

	m_pChar = pChar;
	m_nSituation = nSituation;
	m_pSelSkill = NULL;
}


void CActionDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CActionDlg)
	DDX_Control(pDX, IDC_REPEAT, m_ctlRepeat);
	DDX_Control(pDX, IDC_CASTDELAY, m_ctlDelay);
	DDX_Control(pDX, IDC_SKILLLIST, m_ctlSkillList);
	DDX_CBIndex(pDX, IDC_TARGETLIST, m_nTarget);
	DDX_Text(pDX, IDC_CASTDELAY, m_nCastDelay);
	DDV_MinMaxInt(pDX, m_nCastDelay, 0, 2147483647);
	DDX_Radio(pDX, IDC_LEFT, m_nSide);
	DDX_Text(pDX, IDC_REPEAT, m_nRepeat);
	DDV_MinMaxInt(pDX, m_nRepeat, 1, 2147483647);
	DDX_Text(pDX, IDC_COOLDOWN, m_sCoolDown);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CActionDlg, CDialog)
	//{{AFX_MSG_MAP(CActionDlg)
	ON_CBN_SELCHANGE(IDC_SKILLLIST, OnSelchangeSkilllist)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CActionDlg message handlers

void CActionDlg::SetAction(const CD2Action &action)
{
	m_pSelSkill = m_pChar->FindSkill(action.GetSkillID());
	m_nTarget = action.GetTarget();
	m_nCastDelay = action.GetDelay();
	m_nSide = action.GetSide();
	m_nRepeat = action.GetRepeat();
	m_sCoolDown.Format(_T("该技能冷却时间为：%d 毫秒。"), m_pSelSkill->GetCoolDown());
}

CD2Action CActionDlg::GetAction() const
{
	return CD2Action(m_pSelSkill->GetSkillID(), m_nSide, m_nTarget, m_nCastDelay, m_nRepeat);
}

BOOL CActionDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	CString sTitle;
	CString sSitu;
	switch (m_nSituation)
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

	InitSkills();

	if (m_pSelSkill != NULL)
	{
		char szSkillName[MAX_SKILLNAME_LEN] = "";
		if (m_pSelSkill->GetSkillName(szSkillName))
			m_ctlSkillList.SelectString(-1, szSkillName);
		//CheckStats();
	}

	GetDlgItem(IDOK)->EnableWindow(m_pSelSkill != NULL);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CActionDlg::InitSkills()
{
	const CD2Skill* aSkills = m_pChar->GetSkillArray();

	for (int i = 0; aSkills[i].GetSkillID() != D2S_INVALID; i++)
	{
		if (aSkills[i].IsPassive())
			continue;

		// skills that can be cast in town
		if (m_nSituation == DAS_BEFORERED && !aSkills[i].IsEnableInTown())
			continue;

		// Skills that require corposes
		if (m_nSituation == DAS_ONCORPOSE && !aSkills[i].NeedCorpse())
			continue;

		// Skills that are available in this situation
		char szSkillName[MAX_SKILLNAME_LEN] = "";
		if (aSkills[i].GetSkillName(szSkillName))
			m_ctlSkillList.AddString(szSkillName);
	}
}

void CActionDlg::OnSelchangeSkilllist() 
{
	// TODO: Add your control notification handler code here
	CString str;
	m_ctlSkillList.GetWindowText(str);
	m_pSelSkill = m_pChar->FindSkill(str);
	CheckStats();
}

void CActionDlg::CheckStats()
{
	GetDlgItem(IDOK)->EnableWindow(m_pSelSkill != NULL);
	if (m_pSelSkill == NULL)
		return;

	UpdateData(TRUE);
	m_nSide = (m_pSelSkill->IsRightOnly()) ? SAS_RIGHT : m_nSide;
	GetDlgItem(IDC_LEFT)->EnableWindow(!m_pSelSkill->IsRightOnly());
	m_nTarget = (m_pSelSkill->IsTargetable()) ? m_nTarget : CAT_NONE;
	GetDlgItem(IDC_TARGETLIST)->EnableWindow(m_pSelSkill->IsTargetable());
	m_sCoolDown.Format(_T("该技能冷却时间为：%d 毫秒。"), m_pSelSkill->GetCoolDown());
	m_nCastDelay = max(m_nCastDelay, 250);
	UpdateData(FALSE);
}

void CActionDlg::OnOK() 
{
	// TODO: Add extra validation here
	if (m_pSelSkill == NULL)
		return;

	UpdateData(TRUE);
	/*
	if (m_nCastDelay < m_pSelSkill->GetCoolDown())
	{
		CString sMsg;
		sMsg.Format(_T("技能“%s”的施放间隔不可能低于%d毫秒。"), m_pSelSkill->GetSkillName(), m_pSelSkill->GetCoolDown());
		MessageBox(sMsg, _T("参数错误"), MB_ICONEXCLAMATION);
		m_ctlDelay.SetFocus();
		m_ctlDelay.SetSel(0, -1);
		return;
	}
	*/

	EndDialog(IDOK);
}
