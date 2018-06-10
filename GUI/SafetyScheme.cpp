// SafetyScheme.cpp: implementation of the CSafetyScheme class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "pindlebot.h"
#include "SafetyScheme.h"
#include "StrSplit.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

#include "d2monster.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CSafetyScheme::CSafetyScheme()
{
	m_sName = _T("Default");
	m_bMalahHP = TRUE;
	m_bMalahMP = TRUE;
	m_bAlertHP = TRUE;
	m_bAlertMP = TRUE;
	m_nMalahHP = 90;
	m_nMalahMP = 75;
	m_nAlertHP = 50;
	m_nAlertMP = 15;
	m_bAlertMerc = TRUE;
	m_nDistance = 0;

	m_bPotionHP = TRUE;
	m_nPotionHP = 80;
	m_bPotionMP = TRUE;
	m_nPotionMP = 50;
	m_bPotionMerc = TRUE;
	m_nPotionMerc = 75;

	m_aAttr1.Add(PINDLE_BONUS_LIGHTNING_ENCHANTED);
	m_aAttr2.Add(PINDLE_BONUS_MULTISHOT);

	m_aAttr1.Add(PINDLE_BONUS_LIGHTNING_ENCHANTED);
	m_aAttr2.Add(PINDLE_BONUS_COLD_ENCHANTED);

	m_aAttr1.Add(PINDLE_BONUS_LIGHTNING_ENCHANTED);
	m_aAttr2.Add(PINDLE_BONUS_SPECTRAL_HIT);

	m_aAttr1.Add(PINDLE_BONUS_LIGHTNING_ENCHANTED);
	m_aAttr2.Add(UNIT_AURA_CONVICTION);

	m_aAttr1.Add(PINDLE_BONUS_COLD_ENCHANTED);
	m_aAttr2.Add(UNIT_AURA_CONVICTION);

	m_aAttr1.Add(PINDLE_BONUS_CURSED);
	m_aAttr2.Add(UNIT_AURA_MIGHT);

	m_aAttr1.Add(PINDLE_BONUS_CURSED);
	m_aAttr2.Add(UNIT_AURA_FANATICISM);

	m_aAttr1.Add(PINDLE_BONUS_EXTRA_FAST);
	m_aAttr2.Add(UNIT_AURA_MIGHT);

	m_aAttr1.Add(PINDLE_BONUS_EXTRA_FAST);
	m_aAttr2.Add(UNIT_AURA_FANATICISM);
}

CSafetyScheme::~CSafetyScheme()
{

}

void CSafetyScheme::Save(CIniFile &ini) const
{
	ini.WriteProfileInt(m_sName, _T("Location Adjust"), m_nDistance);
	ini.WriteProfileBool(m_sName, _T("Watch Malah HP"), m_bMalahHP);
	ini.WriteProfileBool(m_sName, _T("Watch Malah MP"), m_bMalahMP);
	ini.WriteProfileBool(m_sName, _T("Watch Alert HP"), m_bAlertHP);
	ini.WriteProfileBool(m_sName, _T("Watch Alert MP"), m_bAlertMP);

	ini.WriteProfileBool(m_sName, _T("Watch Potion HP"), m_bPotionHP);
	ini.WriteProfileBool(m_sName, _T("Watch Potion MP"), m_bPotionMP);
	ini.WriteProfileBool(m_sName, _T("Watch Potion Merc"), m_bPotionMerc);

	ini.WriteProfileInt(m_sName, _T("Malah HP"), m_nMalahHP);
	ini.WriteProfileInt(m_sName, _T("Malah MP"), m_nMalahMP);
	ini.WriteProfileInt(m_sName, _T("Alert HP"), m_nAlertHP);
	ini.WriteProfileInt(m_sName, _T("Alert MP"), m_nAlertMP);

	ini.WriteProfileInt(m_sName, _T("Potion HP"), m_nPotionHP);
	ini.WriteProfileInt(m_sName, _T("Potion MP"), m_nPotionMP);
	ini.WriteProfileInt(m_sName, _T("Potion Merc"), m_nPotionMerc);

	ini.WriteProfileBool(m_sName, _T("Watch Merc"), m_bAlertMerc);
	
	CString sAttrs, str;
	for (int i = 0; i < m_aAttr1.GetSize(); i++)
	{
		str.Format(_T("%04X %04X "), m_aAttr1[i], m_aAttr2[i]);
		sAttrs += str;
	}

	ini.WriteProfileString(m_sName, _T("Avoid Combo"), sAttrs);
}

void CSafetyScheme::Load(CIniFile &ini, LPCTSTR lpName)
{
	m_sName = lpName;
	m_aAttr1.RemoveAll();
	m_aAttr2.RemoveAll();

	m_nDistance = ini.GetProfileInt(lpName, _T("Location Adjust"), 0);
	m_bMalahHP = ini.GetProfileBool(lpName, _T("Watch Malah HP"), TRUE);
	m_bMalahMP = ini.GetProfileBool(lpName, _T("Watch Malah MP"), TRUE);
	m_bAlertHP = ini.GetProfileBool(lpName, _T("Watch Alert HP"), TRUE);
	m_bAlertMP = ini.GetProfileBool(lpName, _T("Watch Alert MP"), TRUE);

	m_bPotionHP = ini.GetProfileBool(m_sName, _T("Watch Potion HP"), TRUE);
	m_bPotionMP = ini.GetProfileBool(m_sName, _T("Watch Potion MP"), TRUE);
	m_bPotionMerc = ini.GetProfileBool(m_sName, _T("Watch Potion Merc"), TRUE);

	m_nPotionHP = ini.GetProfileInt(m_sName, _T("Potion HP"), 80);
	m_nPotionMP = ini.GetProfileInt(m_sName, _T("Potion MP"), 50);
	m_nPotionMerc = ini.GetProfileInt(m_sName, _T("Potion Merc"), 75);

	m_nMalahHP = ini.GetProfileInt(lpName, _T("Malah HP"), 90);
	m_nMalahMP = ini.GetProfileInt(lpName, _T("Malah MP"), 75);
	m_nAlertHP = ini.GetProfileInt(lpName, _T("Alert HP"), 50);
	m_nAlertMP = ini.GetProfileInt(lpName, _T("Alert MP"), 15);

	m_bAlertMerc = ini.GetProfileBool(lpName, _T("Watch Merc"), TRUE);
	
	CString sAttrs = ini.GetProfileString(lpName, _T("Avoid Combo"), DEF_AVOID_COMBOS);
	CStringArray a;
	StrSplit(sAttrs, a, _T(','));

	if ((a.GetSize() % 2) != 0)
		a.RemoveAt(a.GetUpperBound());

	for (int i = 0; i < a.GetSize() - 1; i += 2)
	{
		m_aAttr1.Add((WORD)_tcstoul(a[i], NULL, 16));
		m_aAttr2.Add((WORD)_tcstoul(a[i + 1], NULL, 16));
	}
}

void CSafetyScheme::ListCombos(CReportCtrl &lc) const
{
	lc.DeleteAllItems();
	for (int i = 0; i < m_aAttr1.GetSize(); i++)
	{
		const int IDX = lc.InsertItem(INT_MAX, GetMonsterAttribute(m_aAttr1[i]), GetMonsterAttribute(m_aAttr2[i]));
		lc.SetItemData(IDX, MAKELONG(m_aAttr1[i], m_aAttr2[i]));
	}	
}

CSafetyScheme::CSafetyScheme(const CSafetyScheme &rhs)
{
	m_sName = rhs.m_sName;
	m_bMalahHP = rhs.m_bMalahHP;
	m_bMalahMP = rhs.m_bMalahMP;
	m_bAlertHP = rhs.m_bAlertHP;
	m_bAlertMP = rhs.m_bAlertMP;
	m_nMalahHP = rhs.m_nMalahHP;
	m_nMalahMP = rhs.m_nMalahMP;
	m_nAlertHP = rhs.m_nAlertHP;
	m_nAlertMP = rhs.m_nAlertMP;
	m_bAlertMerc = rhs.m_bAlertMerc;
	m_nDistance = rhs.m_nDistance;

	m_bPotionHP = rhs.m_bPotionHP;
	m_nPotionHP = rhs.m_nPotionHP;
	m_bPotionMP = rhs.m_bPotionMP;
	m_nPotionMP = rhs.m_nPotionMP;
	m_bPotionMerc = rhs.m_bPotionMerc;
	m_nPotionMerc = rhs.m_nPotionMerc;

	m_aAttr1.Copy(rhs.m_aAttr1);
	m_aAttr2.Copy(rhs.m_aAttr2);
}

const CSafetyScheme& CSafetyScheme::operator=(const CSafetyScheme &rhs)
{
	m_sName = rhs.m_sName;
	m_bMalahHP = rhs.m_bMalahHP;
	m_bMalahMP = rhs.m_bMalahMP;
	m_bAlertHP = rhs.m_bAlertHP;
	m_bAlertMP = rhs.m_bAlertMP;
	m_nMalahHP = rhs.m_nMalahHP;
	m_nMalahMP = rhs.m_nMalahMP;
	m_nAlertHP = rhs.m_nAlertHP;
	m_nAlertMP = rhs.m_nAlertMP;
	m_bAlertMerc = rhs.m_bAlertMerc;
	m_nDistance = rhs.m_nDistance;

	m_bPotionHP = rhs.m_bPotionHP;
	m_nPotionHP = rhs.m_nPotionHP;
	m_bPotionMP = rhs.m_bPotionMP;
	m_nPotionMP = rhs.m_nPotionMP;
	m_bPotionMerc = rhs.m_bPotionMerc;
	m_nPotionMerc = rhs.m_nPotionMerc;

	m_aAttr1.Copy(rhs.m_aAttr1);
	m_aAttr2.Copy(rhs.m_aAttr2);
	return *this;
}

BOOL CSafetyScheme::operator==(const CSafetyScheme &rhs) const
{
	return m_sName.CompareNoCase(rhs.m_sName) == 0;
}

CString CSafetyScheme::GetName() const
{
	return m_sName;
}
