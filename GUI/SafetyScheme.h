// SafetyScheme.h: interface for the CSafetyScheme class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SAFETYSCHEME_H__9BB2EC89_D468_4285_9F72_A7A1041DB2AC__INCLUDED_)
#define AFX_SAFETYSCHEME_H__9BB2EC89_D468_4285_9F72_A7A1041DB2AC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "inifile.h"
#include "reportCtrl.h"

class CSafetyScheme  
{
public:
	CString GetName() const;
	BOOL operator==(const CSafetyScheme &rhs) const;
	const CSafetyScheme& operator=(const CSafetyScheme& rhs);
	CSafetyScheme(const CSafetyScheme& rhs);
	void ListCombos(CReportCtrl& lc) const;
	void Load(CIniFile& ini, LPCTSTR lpName);
	void Save(CIniFile& ini) const;
	CSafetyScheme();
	virtual ~CSafetyScheme();

	CString m_sName;
	BOOL m_bMalahHP;
	int m_nMalahHP;
	BOOL m_bMalahMP;
	int m_nMalahMP;
	BOOL m_bAlertHP;
	int m_nAlertHP;
	BOOL m_bAlertMP;
	int m_nAlertMP;
	BOOL m_bAlertMerc;
	int m_nDistance;
	BOOL m_bPotionHP;
	int m_nPotionHP;
	BOOL m_bPotionMP;
	int m_nPotionMP;
	BOOL m_bPotionMerc;
	int m_nPotionMerc;
	CWordArray m_aAttr1;
	CWordArray m_aAttr2;
};

#endif // !defined(AFX_SAFETYSCHEME_H__9BB2EC89_D468_4285_9F72_A7A1041DB2AC__INCLUDED_)
