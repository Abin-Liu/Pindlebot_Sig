// Pindlebot.h : main header file for the PINDLEBOT application
//

#if !defined(AFX_PINDLEBOT_H__46FB1288_979C_4834_A94A_0CB9006836D6__INCLUDED_)
#define AFX_PINDLEBOT_H__46FB1288_979C_4834_A94A_0CB9006836D6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols
#include "Singleton.h"

/////////////////////////////////////////////////////////////////////////////
// CPindlebotApp:
// See Pindlebot.cpp for the implementation of this class
//
//CString CreateGUID();

BOOL IsValidD2Char(TCHAR ch);

class CPindlebotApp : public CWinApp
{
public:
	CPindlebotApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPindlebotApp)
	public:
	virtual BOOL InitInstance();
	virtual void WinHelp(DWORD dwData, UINT nCmd = HELP_CONTEXT);
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CPindlebotApp)
	afx_msg void OnAppHelp();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()	
protected:
	void LoadSafetySchemes() const;
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PINDLEBOT_H__46FB1288_979C_4834_A94A_0CB9006836D6__INCLUDED_)
