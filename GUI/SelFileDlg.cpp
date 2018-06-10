// SelFileDlg.cpp : implementation file
//

#include "stdafx.h"
#include "pindlebot.h"
#include "SelFileDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSelFileDlg dialog

// exclude list
const TCHAR* const EXCLUDE[] = {
	_T("Pindlebot.ini"),
	_T("Bind.ini"),
	_T("irunin.ini"),
	_T("D2HackIt.ini"),
	NULL
};


CSelFileDlg::CSelFileDlg(CString sFile, CWnd* pParent /*=NULL*/)
	: CDialog(CSelFileDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSelFileDlg)
	m_sFile = _T("");
	//}}AFX_DATA_INIT

	m_sOrigFile = sFile;
}


void CSelFileDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSelFileDlg)
	DDX_Control(pDX, IDC_LIST1, m_list);
	DDX_LBString(pDX, IDC_LIST1, m_sFile);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSelFileDlg, CDialog)
	//{{AFX_MSG_MAP(CSelFileDlg)
	ON_LBN_SELCHANGE(IDC_LIST1, OnSelchangeList1)
	ON_LBN_DBLCLK(IDC_LIST1, OnDblclkList1)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSelFileDlg message handlers

BOOL CSelFileDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	CFileFind ff;
	BOOL bFind = ff.FindFile(g_sAppDir + _T("\\*.ini"));
	CString sFile;
	BOOL bExclude = FALSE;

	while (bFind)
	{
		bFind = ff.FindNextFile();
		sFile = ff.GetFileName();

		bExclude = FALSE;
		for (int i = 0; EXCLUDE[i] != NULL; i++)
		{
			if (sFile.CompareNoCase(EXCLUDE[i]) == 0)
			{
				bExclude = TRUE;
				break;
			}
		}

		if (!bExclude)
			m_list.AddString(sFile);
	}

	int nPos = m_list.FindString(-1, m_sOrigFile);
	m_list.SetCurSel(nPos);
	GetDlgItem(IDOK)->EnableWindow(m_list.GetCurSel() != LB_ERR);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

CString CSelFileDlg::GetSelFile() const
{
	return m_sFile;
}

void CSelFileDlg::OnSelchangeList1() 
{
	// TODO: Add your control notification handler code here
	GetDlgItem(IDOK)->EnableWindow(m_list.GetCurSel() != LB_ERR);
}

void CSelFileDlg::OnDblclkList1() 
{
	// TODO: Add your control notification handler code here
	if (m_list.GetCurSel() != LB_ERR)
		OnOK();
}
