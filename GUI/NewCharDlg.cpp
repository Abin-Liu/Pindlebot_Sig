// NewCharDlg.cpp : implementation file
//

#include "stdafx.h"
#include "pindlebot.h"
#include "NewCharDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CNewCharDlg dialog


CNewCharDlg::CNewCharDlg(LPCTSTR lpName, int nClass, LPCTSTR lpSafetyName, int nAmmoType, int nAmmoBuyQty, CWnd* pParent /*=NULL*/)
	: CDialog(CNewCharDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CNewCharDlg)
	m_sCharName = lpName;
	m_nCharClass = nClass;
	m_nAmmoType = nAmmoType;
	m_nAmmoBuyQty = nAmmoBuyQty;
	m_nSafetyIdx = 0;
	//}}AFX_DATA_INIT

	m_sSafetySchemeName = lpSafetyName;
}


void CNewCharDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CNewCharDlg)
	DDX_Control(pDX, IDC_AMMOBUYQTY, m_wndAmmoBuyQty);
	DDX_Control(pDX, IDC_SAFETYSCHEMES, m_wndSafetySchemes);
	DDX_Control(pDX, IDC_CHARNAME, m_ctlCharName);
	DDX_Text(pDX, IDC_CHARNAME, m_sCharName);
	DDV_MaxChars(pDX, m_sCharName, 15);
	DDX_CBIndex(pDX, IDC_COMBO1, m_nCharClass);
	DDX_Radio(pDX, IDC_AMMO, m_nAmmoType);
	DDX_Text(pDX, IDC_AMMOBUYQTY, m_nAmmoBuyQty);
	DDV_MinMaxInt(pDX, m_nAmmoBuyQty, 0, 5);
	DDX_CBIndex(pDX, IDC_SAFETYSCHEMES, m_nSafetyIdx);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CNewCharDlg, CDialog)
	//{{AFX_MSG_MAP(CNewCharDlg)
	ON_BN_CLICKED(IDC_AMMO, OnAmmo)
	ON_BN_CLICKED(IDC_RADIO1, OnAmmo)
	ON_BN_CLICKED(IDC_RADIO2, OnAmmo)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CNewCharDlg message handlers

void CNewCharDlg::OnOK() 
{
	// TODO: Add extra validation here
	UpdateData(TRUE);
	m_wndSafetySchemes.GetLBText(m_nSafetyIdx, m_sSafetySchemeName);
	if (m_sCharName.IsEmpty() || m_nCharClass < 0 || m_nCharClass >= 7)
		return;
	else
		EndDialog(IDOK);
}

CString CNewCharDlg::GetCharName() const
{
	return m_sCharName;
}

int CNewCharDlg::GetCharClass() const
{
	return m_nCharClass;
}

int CNewCharDlg::GetAmmoType() const
{
	return m_nAmmoType;
}

int CNewCharDlg::GetAmmoBuyQty() const
{
	return m_nAmmoBuyQty;
}

BOOL CNewCharDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	for (int i = 0; i < g_aSafetySchemes.GetSize(); i++)
	{
		m_wndSafetySchemes.AddString(g_aSafetySchemes[i].GetName());
	}

	m_nSafetyIdx = m_wndSafetySchemes.FindStringExact(0, m_sSafetySchemeName);
	m_wndAmmoBuyQty.EnableWindow(m_nAmmoType > 0);
	UpdateData(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

CString CNewCharDlg::GetSafetyName() const
{
	return m_sSafetySchemeName;
}

void CNewCharDlg::OnAmmo() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	m_wndAmmoBuyQty.EnableWindow(m_nAmmoType > 0);
}
