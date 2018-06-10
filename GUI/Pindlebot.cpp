// Pindlebot.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "Pindlebot.h"
#include "PindlebotDlg.h"
#include "Singleton.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CString g_sAppDir;
CIniFile g_ini;
CIniFile g_analysis;
CIniFile g_safetyIni;
CArrayEx<CSafetyScheme, const CSafetyScheme&> g_aSafetySchemes;

/////////////////////////////////////////////////////////////////////////////
// CPindlebotApp

BEGIN_MESSAGE_MAP(CPindlebotApp, CWinApp)
	//{{AFX_MSG_MAP(CPindlebotApp)
	ON_COMMAND(ID_APP_HELP, OnAppHelp)
	//}}AFX_MSG_MAP
	ON_COMMAND(ID_HELP, CWinApp::OnHelp)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPindlebotApp construction

CPindlebotApp::CPindlebotApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CPindlebotApp object

CPindlebotApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CPindlebotApp initialization

BOOL CPindlebotApp::InitInstance()
{
	// Standard initialization
	// If you are not using these features and wish to reduce the size
	//  of your final executable, you should remove from the following
	//  the specific initialization routines you do not need.

#ifdef _AFXDLL
	Enable3dControls();			// Call this when using MFC in a shared DLL
#else
	Enable3dControlsStatic();	// Call this when linking to MFC statically
#endif

	TCHAR szPath[MAX_PATH] = _T("");
	::GetModuleFileName(NULL, szPath, MAX_PATH);

	int nLen = strlen(szPath);
	for (int i = nLen - 1; i >= 0; i--)
	{
		if (szPath[i] == '\\')
		{
			szPath[i] = '\0';
			break;
		}
	}
	
	g_sAppDir = szPath;

	CString sMutex(_T("{D0429C5B-CAD0-4BE8-B2C2-52BB5EE503D1}"));
	sMutex += g_sAppDir;
	sMutex.Replace(_T('\\'),  _T('/'));

	CSingleton st(sMutex);
	if (!st.IsInstanceUnique())
	{
		CString sMsg;
		sMsg.Format(_T("位于\"%s\"的Pindlebot设置程序已经在运行中，请右击系统托盘区的Pindlebot图标打开功能菜单。"), g_sAppDir);
		AfxMessageBox(sMsg);
		return FALSE;
	}

	g_ini.SetPathName(g_sAppDir + _T("\\") + INI_FILE);
	g_analysis.SetPathName(g_sAppDir + _T("\\") + RETREAT_LOG);
	g_safetyIni.SetPathName(g_sAppDir + _T("\\") + SAFETY_INI);

	LoadSafetySchemes();

	CPindlebotDlg dlg;
	m_pMainWnd = &dlg;
	dlg.DoModal();
	
	// Since the dialog has been closed, return FALSE so that we exit the
	//  application, rather than start the application's message pump.
	return FALSE;
}

void CPindlebotApp::WinHelp(DWORD dwData, UINT nCmd) 
{
	// TODO: Add your specialized code here and/or call the base class
	OnAppHelp();
}

void CPindlebotApp::OnAppHelp() 
{
	// TODO: Add your command handler code here
	ShellExecute(NULL, _T("open"), _T("hh.exe"), g_sAppDir + _T("\\pindlebot.chm"), g_sAppDir, SW_SHOW);
}

void CPindlebotApp::LoadSafetySchemes() const
{
	CStringArray a;
	g_safetyIni.GetProfileSectionNames(a);
	g_aSafetySchemes.SetSize(a.GetSize());

	for (int i = 0; i < a.GetSize(); i++)
	{	
		CString s = a[i];
		g_aSafetySchemes[i].Load(g_safetyIni, a[i]);
	}

	CSafetyScheme ss;
	if (g_aSafetySchemes.Find(ss) == -1)
	{
		g_aSafetySchemes.InsertAt(0, ss);
		ss.Save(g_safetyIni);
	}
}
