// SafetySchemeNameDlg.cpp : implementation file
//

#include "stdafx.h"
#include "pindlebot.h"
#include "SafetySchemeNameDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSafetySchemeNameDlg dialog


CSafetySchemeNameDlg::CSafetySchemeNameDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSafetySchemeNameDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSafetySchemeNameDlg)
	m_sName = _T("");
	//}}AFX_DATA_INIT
}


void CSafetySchemeNameDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSafetySchemeNameDlg)
	DDX_Control(pDX, IDC_EDIT1, m_ctlName);
	DDX_Text(pDX, IDC_EDIT1, m_sName);
	DDV_MaxChars(pDX, m_sName, 64);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSafetySchemeNameDlg, CDialog)
	//{{AFX_MSG_MAP(CSafetySchemeNameDlg)
	ON_EN_CHANGE(IDC_EDIT1, OnChangeEdit1)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSafetySchemeNameDlg message handlers

void CSafetySchemeNameDlg::OnChangeEdit1() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	
	if (!IsValidName())
	{
		// remove the last char
		m_sName.Delete(m_sName.GetLength() - 1);
		UpdateData(FALSE);
		m_ctlName.SetSel(m_sName.GetLength(), m_sName.GetLength());
	}

	m_sName.TrimLeft();
	m_sName.TrimRight();
	GetDlgItem(IDOK)->EnableWindow(!m_sName.IsEmpty());
}

BOOL CSafetySchemeNameDlg::IsValidName() const
{
	if (m_sName.IsEmpty())
		return TRUE;

	TCHAR c = m_sName.GetAt(m_sName.GetLength() - 1);
	return isdigit(c) || isalpha(c) || c == ' ' || c == '_' || c == '-';
}

CString CSafetySchemeNameDlg::GetName() const
{
	return m_sName;
}

void CSafetySchemeNameDlg::OnOK() 
{
	// TODO: Add extra validation here
	UpdateData(TRUE);
	m_sName.TrimLeft();
	m_sName.TrimRight();
	if (m_sName.IsEmpty())
		return;

	CDialog::EndDialog(IDOK);
}
