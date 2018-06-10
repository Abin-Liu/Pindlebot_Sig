// stdafx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//      are changed infrequently
//

#if !defined(AFX_STDAFX_H__92AF8A43_04FC_4A9A_A070_78E46FD858BB__INCLUDED_)
#define AFX_STDAFX_H__92AF8A43_04FC_4A9A_A070_78E46FD858BB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define VC_EXTRALEAN		// Exclude rarely-used stuff from Windows headers

#include <afxwin.h>         // MFC core and standard components
#include <afxext.h>         // MFC extensions
#include <afxdtctl.h>		// MFC support for Internet Explorer 4 Common Controls
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>			// MFC support for Windows Common Controls
#endif // _AFX_NO_AFXCMN_SUPPORT

#include "EditEx.h"
#include "IniFile.h"
#include "ArrayEx.h"
#include "SafetyScheme.h"

#define APP_TITLE	_T("Pindlebot SIG°æ v3.52")
#define INI_FILE	_T("Pindlebot.ini")
#define SAFETY_INI _T("Safety.dat")
#define CURE_TEAM	_T("http://www.cure-team.com/bbs/")
#define HOMEPAGE	_T("http://www.wxjindu.com/abin/")
#define MAILME	_T("mailto:abinn32@163.com")
#define RETREAT_LOG _T("Retreat.log")
#define DEF_AVOID_COMBOS	_T("17,29,17,18,17,27,17,156,7,177,7,161,18,156,28,8,6,177,6,161,")

extern CString g_sAppDir;
extern CIniFile g_ini;
extern CIniFile g_analysis;
extern CIniFile g_safetyIni;
extern CArrayEx<CSafetyScheme, const CSafetyScheme&> g_aSafetySchemes;

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STDAFX_H__92AF8A43_04FC_4A9A_A070_78E46FD858BB__INCLUDED_)
