// ItemLogDlg.cpp : implementation file
//

#include "stdafx.h"
#include "pindlebot.h"
#include "ItemLogDlg.h"
#include "Clipboard.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CItemLogDlg dialog


CItemLogDlg::CItemLogDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CItemLogDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CItemLogDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CItemLogDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CItemLogDlg)
	DDX_Control(pDX, IDC_LIST1, m_list);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CItemLogDlg, CDialog)
	//{{AFX_MSG_MAP(CItemLogDlg)
	ON_BN_CLICKED(IDC_CLEAR, OnClear)
	ON_BN_CLICKED(IDC_COPYTOCLIPBOARD, OnCopytoclipboard)
	ON_LBN_SELCHANGE(IDC_LIST1, OnSelchangeList1)
	ON_WM_CONTEXTMENU()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CItemLogDlg message handlers

BOOL CItemLogDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	m_list.SetAutoScroll();
	CString sLogFile = g_sAppDir + _T("\\Pickit.log");
	CStdioFile file;
	if (!file.Open(sLogFile, CFile::modeRead))
		return TRUE;
	
	CString sLine;
	while (file.ReadString(sLine))
	{
		//if (!isalpha(sLine[0]) && sLine[1] == 'c' && isdigit(sLine[2]))
			//sLine.Delete(1, 3);

		sLine.TrimLeft();
		sLine.TrimRight();
		if (!sLine.IsEmpty())
		{
			m_list.AddString(sLine);
		}
	}
	file.Close();
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CItemLogDlg::OnClear() 
{
	// TODO: Add your control notification handler code here
	if (m_list.GetCount() == 0 || MessageBox(_T("确定要全部清除吗？"), _T("清除数据"), MB_ICONQUESTION | MB_YESNO) != IDYES)
		return;

	CString sLogFile = g_sAppDir + _T("\\Pickit.log");
	CStdioFile file;
	file.Open(sLogFile, CFile::modeCreate);
	file.Close();
	m_list.ResetContent();
}

void CItemLogDlg::OnCopytoclipboard() 
{
	// TODO: Add your control notification handler code here
	const int COUNT = m_list.GetSelCount();
	if (COUNT < 1)
		return;

	// Get the indexes of all the selected items.
	int * p = new int[COUNT];
	m_list.GetSelItems(COUNT, p);
	CString sAll, s;

	for (int i = 0; i < COUNT; i++)
	{
		int n = m_list.GetTextLen(p[i]);
		m_list.GetText(p[i], s.GetBuffer(n));
		s.ReleaseBuffer();
		sAll += s + _T("\r\n");
	}

	CClipboard::SetText(sAll);
	delete [] p;	
}

void CItemLogDlg::OnSelchangeList1() 
{
	// TODO: Add your control notification handler code here
	GetDlgItem(IDC_COPYTOCLIPBOARD)->EnableWindow(m_list.GetSelCount() > 0);
}

void CItemLogDlg::OnContextMenu(CWnd* pWnd, CPoint point) 
{
	// TODO: Add your message handler code here
	if (m_list.GetSelCount() < 1)
		return;

	CRect rect;
	m_list.GetWindowRect(&rect);
	if (!rect.PtInRect(point))
		return;
		
	CMenu menu;
	CMenu* popUp;

	if (!menu.LoadMenu(IDR_ITEMLOG))
		return ;
	
	popUp = menu.GetSubMenu(0);

	if (popUp)
	{
		popUp->TrackPopupMenu(TPM_LEFTALIGN, point.x, point.y, this);
	}
}
