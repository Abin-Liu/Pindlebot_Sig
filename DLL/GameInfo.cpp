#include <stdio.h>
#include "Algorithm.h"
#include "GameInfo.h"
#include "Ini.h"
#include "Globals.h"

void CGameInfo::Initialize()
{
	m_bClientWalk = TRUE;

	m_nLocationAdj = LA_NORMAL;
	m_bResMerc = TRUE;

	m_bWatchMalahHP = FALSE;
	m_bWatchMalahMP = FALSE;
	m_bWatchAlertHP = FALSE;
	m_bWatchAlertMP = FALSE;
	m_bWatchMerc = FALSE;
	m_bWatchPotionHP = FALSE;
	m_bWatchPotionMP = FALSE;
	m_bWatchPotionMerc = FALSE;

	m_nMalahHPPercent = 0;
	m_nMalahMPPercent = 0;
	m_nAlertHPPercent = 0;
	m_nAlertMPPercent = 0;
	m_nPotionHPPercent = 0;
	m_nPotionMPPercent = 0;
	m_nPotionMercPercent = 0;	

	m_nMalahHP = 0;
	m_nMalahMP = 0;
	m_nAlertHP = 0;
	m_nAlertMP = 0;
	m_nPotionHP = 0;
	m_nPotionMP = 0;

	m_nMaxHP = 0;
	m_nMaxMP = 0;

	m_bShowSettings = TRUE;
	m_nAmmoType = 0;
	m_nAmmoBuyQty = 2;
}

void CGameInfo::LoadCombos(LPCSTR lpSafety)
{
	m_aAttrs.RemoveAll();
	char sAll[1026] = ""; // should be enough
	g_SafetyIni.GetArray(lpSafety, "Avoid Combo", sAll, 1024, " ");
	g_SafetyIni.ParseDNTString(sAll, ParseDNTStrProc, (LPVOID)&m_aAttrs);
	ValidateCombos();
}

DWORD CGameInfo::IsComboFind(const CArrayEx<WORD, WORD>& arr) const
{
	WORD wAttr1 = 0;
	WORD wAttr2 = 0;
	
	for (int i = 0; i < m_aAttrs.GetSize() - 1; i += 2)
	{
		WORD wAttr1 = m_aAttrs[i];
		WORD wAttr2 = m_aAttrs[i + 1];
		BOOL bFind1 = wAttr1 ? arr.Find(wAttr1) != -1 : TRUE;
		BOOL bFind2 = wAttr2 ? arr.Find(wAttr2) != -1 : TRUE;
		
		if (bFind1 && bFind2)
			return MAKELONG(wAttr1, wAttr2);
	}

	return 0;
}

void CShowPacket::Initialize()
{
	show = SP_NONE;
	pckID = 0;
	pckLen = 0;
	filter[64];
	filterLen = 0;
}

BOOL CShowPacket::IsShow(BYTE* aPacket, DWORD aLen, BOOL bSent) const
{
	if (show == SP_NONE)
		return FALSE;

	if (show == SP_SENT && !bSent)
		return FALSE;

	if (show == SP_RECEIVED && bSent)
		return FALSE;

	if (pckID > 0 && pckID != aPacket[0])
		return FALSE;

	if (filterLen > 0 && -1 == FindBytes(aPacket, aLen, filter, filterLen, 0))
		return FALSE;

	if (pckLen > 0 && pckLen != aLen)
		return FALSE;
	

	return TRUE;
}

void CGameInfo::LoadOtherSettings()
{
	m_bShowSettings = TRUE;//g_ini.GetBool("Safety", "Show Settings", TRUE);
	m_bResMerc = g_ini.GetBool("general", "res merc", FALSE);
}

void CGameInfo::CalcLocation(WORD x, WORD y)
{
	// copy locations, including tail null
	::memcpy(m_aWalkToPindle, WALK_TO_PINDLE, 5 * sizeof(LOCATION));
	::memcpy(m_aTeleToPindle, TELE_TO_PINDLE, 4 * sizeof(LOCATION));

	// adjust the last step
	if (m_nLocationAdj == LA_CLOSER)
	{
		m_aTeleToPindle[2] = ADJ_CLOSER;
		m_aWalkToPindle[3] = ADJ_CLOSER;
	}
	else if (m_nLocationAdj == LA_FARTHER)
	{
		m_aTeleToPindle[2] = ADJ_FARTHER;
		m_aWalkToPindle[3] = ADJ_FARTHER;
	}
	else // normal
	{
		m_aTeleToPindle[2] = ADJ_NORMAL;
		m_aWalkToPindle[3] = ADJ_NORMAL;
	}

	// apply location offsets
	int nOffsetX = x - INRED_LOC.x;
	int nOffsetY = y - INRED_LOC.y;

	if (nOffsetX < 10)
		nOffsetX = 0;

	if (nOffsetY < 10)
		nOffsetY = 0;

	for (int i = 0; i < 4; i++)
	{
		if (i < 3)
		{
			m_aTeleToPindle[i].x += nOffsetX;
			m_aTeleToPindle[i].y += nOffsetY;
		}
		
		m_aWalkToPindle[i].x += nOffsetX;
		m_aWalkToPindle[i].y += nOffsetY;
	}
}

int CGameInfo::NeedMalah(int nCurHP, int nCurMP) const
{
	if (m_nMaxHP == 0 || m_nMaxMP == 0)
		return 3;
	else if (m_bWatchMalahHP && nCurHP < m_nMalahHP)
		return 1;
	else if (m_bWatchMalahMP && nCurMP < m_nMalahMP)
		return 2;
	else
		return 0;
}

int CGameInfo::NeedRetreat(int nCurHP, int nCurMP) const
{
	if (m_bWatchAlertHP && nCurHP < m_nAlertHP)
		return 1;
	else if (m_bWatchAlertMP && nCurMP < m_nAlertMP)
		return 2;
	else
		return 0;
}

BOOL CGameInfo::UpdateHPMP(int nCurHP, int nCurMP, BOOL bAddOnly)
{
	BOOL bRes = TRUE;

	if (bAddOnly)
	{
		bRes = (nCurHP > m_nMaxHP || nCurMP > m_nMaxMP);
		m_nMaxHP = max(nCurHP, m_nMaxHP);
		m_nMaxMP = max(nCurMP, m_nMaxMP);
	}
	else
	{
		bRes = (nCurHP != m_nMaxHP || nCurMP != m_nMaxMP);
		m_nMaxHP = nCurHP;
		m_nMaxMP = nCurMP;
	}

	if (bRes)
	{
		// calculate new ratios
		m_nMalahHP = int((float)m_nMaxHP * (float)m_nMalahHPPercent / 100.0);
		m_nMalahMP = int((float)m_nMaxMP * (float)m_nMalahMPPercent / 100.0);
		m_nAlertHP = int((float)m_nMaxHP * (float)m_nAlertHPPercent / 100.0);
		m_nAlertMP = int((float)m_nMaxMP * (float)m_nAlertMPPercent / 100.0);
		m_nPotionHP = int((float)m_nMaxHP * (float)m_nPotionHPPercent / 100.0);
		m_nPotionMP = int((float)m_nMaxMP * (float)m_nPotionMPPercent / 100.0);
	}
	
	return bRes;
}

BOOL CGameInfo::LoadScheme(LPCTSTR lpCharName)
{
	// Check if this char is already loaded
	if (stricmp(m_scheme.m_szCharName, lpCharName) == 0)
		return TRUE;
	
	// Check if scheme settings for this character exists
	int nClass = g_CharIni.GetInt(lpCharName, "Class", CCD_UNKNOWN);
	if (nClass < CCD_AMAZON || nClass > CCD_SORCERESS)
		return FALSE;

	// ammo settings
	m_nAmmoType = g_CharIni.GetInt(lpCharName, "Ammo Type", 0);
	m_nAmmoBuyQty = g_CharIni.GetInt(lpCharName, "Ammo Buy Qty", 2);

	char szSafetyScheme[64] = "";
	g_CharIni.GetString(lpCharName, "Safety Scheme", szSafetyScheme, 63, "Default");

	m_scheme.SetScheme(lpCharName, nClass, szSafetyScheme);
	char szEntry[32] = "";
	char szValue[128] = "";

	// Read counters for each action list
	int nCountBeforeRed = g_CharIni.GetInt(lpCharName, "Action0", 0);
	int nCountAfterRed = g_CharIni.GetInt(lpCharName, "Action1", 0);
	int nCountOnPindleskin = g_CharIni.GetInt(lpCharName, "Action2", 0);
	int nCountOnCorpose = g_CharIni.GetInt(lpCharName, "Action3", 0);

	m_scheme.m_aBeforeRed.SetSize(nCountBeforeRed);
	m_scheme.m_aAfterRed.SetSize(nCountAfterRed);
	m_scheme.m_aOnPindleskin.SetSize(nCountOnPindleskin);
	m_scheme.m_aOnCorpose.SetSize(nCountOnCorpose);

	int j = 0;

	// populate "before red" actions
	for (j = 0; j < nCountBeforeRed; j++)
	{
		// specified action entries: "action0x"
		sprintf(szEntry, "Action0%d", j);
		g_CharIni.GetString(m_scheme.m_szCharName, szEntry, szValue, 127);
		m_scheme.m_aBeforeRed[j].ParseString(szValue);
	}

	// populate "after red" actions
	for (j = 0; j < nCountAfterRed; j++)
	{
		// specified action entries: "action1x"
		sprintf(szEntry, "Action1%d", j);
		g_CharIni.GetString(m_scheme.m_szCharName, szEntry, szValue, 127);
		m_scheme.m_aAfterRed[j].ParseString(szValue);
	}

	// populate "on pindleskin" actions
	for (j = 0; j < nCountOnPindleskin; j++)
	{
		// specified action entries: "action2x"
		sprintf(szEntry, "Action2%d", j);
		g_CharIni.GetString(m_scheme.m_szCharName, szEntry, szValue, 127);
		m_scheme.m_aOnPindleskin[j].ParseString(szValue);
	}

	// populate "on corpose" actions
	for (j = 0; j < nCountOnCorpose; j++)
	{
		// specified action entries: "action3x"
		sprintf(szEntry, "Action3%d", j);
		g_CharIni.GetString(m_scheme.m_szCharName, szEntry, szValue, 127);
		m_scheme.m_aOnCorpose[j].ParseString(szValue);
	}

	// verify minimum action delays
	m_scheme.LimitMinDelays(250); // no action delay should smaller than 250ms

	LoadSafety(m_scheme.GetSafetyScheme());
	return TRUE;
}

const LOCATION* CGameInfo::GetTeleRoute() const
{
	return m_aTeleToPindle;
}

const LOCATION* CGameInfo::GetWalkRoute() const
{
	return m_aWalkToPindle;
}

void CGameInfo::ValidateCombos()
{
	if ((m_aAttrs.GetSize() % 2) != 0) // odd number
		m_aAttrs.RemoveAt(m_aAttrs.GetSize() - 1);
}

BOOL CGameInfo::LoadSafety(LPCTSTR lpSafetyName)
{
	BOOL bRes = TRUE;

	// check if the scheme exists
	if (g_SafetyIni.GetInt(lpSafetyName, "Alert HP", -255) == -255)
	{
		m_scheme.SetSafety("default"); // force using "default scheme"
		bRes = FALSE;
	}

	LPCSTR lpSafety = m_scheme.GetSafetyScheme();

	m_bWatchMalahHP = g_SafetyIni.GetBool(lpSafety, "Watch Malah HP", TRUE);
	m_bWatchMalahMP = g_SafetyIni.GetBool(lpSafety, "Watch Malah MP", TRUE);
	m_bWatchAlertHP = g_SafetyIni.GetBool(lpSafety, "Watch Alert HP", TRUE);
	m_bWatchAlertMP = g_SafetyIni.GetBool(lpSafety, "Watch Alert MP", TRUE);
	m_bWatchPotionHP = g_SafetyIni.GetBool(lpSafety, "Watch Potion HP", TRUE);
	m_bWatchPotionMP = g_SafetyIni.GetBool(lpSafety, "Watch Potion MP", TRUE);

	m_bWatchMerc = g_SafetyIni.GetBool(lpSafety, "Watch Merc", FALSE);
	m_bWatchPotionMerc = g_SafetyIni.GetBool(lpSafety, "Watch Potion Merc", FALSE);

	m_nMalahHPPercent = g_SafetyIni.GetInt(lpSafety, "Malah HP", 90);
	m_nMalahMPPercent = g_SafetyIni.GetInt(lpSafety, "Malah MP", 75);
	m_nAlertHPPercent = g_SafetyIni.GetInt(lpSafety, "Alert HP", 50);
	m_nAlertMPPercent = g_SafetyIni.GetInt(lpSafety, "Alert MP", 5);
	m_nPotionHPPercent = g_SafetyIni.GetInt(lpSafety, "Potion HP", 80);
	m_nPotionMPPercent = g_SafetyIni.GetInt(lpSafety, "Potion MP", 50);
	m_nPotionMercPercent = g_SafetyIni.GetInt(lpSafety, "Potion Merc", 75);

	m_nLocationAdj = g_SafetyIni.GetInt(lpSafety, "Location Adjust", LA_NORMAL);

	LoadCombos(m_scheme.GetSafetyScheme());

	return bRes;
}

LPCSTR CGameInfo::GetSafetySchemeName() const
{
	return m_scheme.GetSafetyScheme();
}

int CGameInfo::GetAmmoBuyQty() const
{
	return (m_nAmmoType == 1 || m_nAmmoType == 2) ? m_nAmmoBuyQty : 0;
}

int CGameInfo::GetAmmoType() const
{
	return m_nAmmoType;
}

BOOL CGameInfo::NeedPotionHeal(int nCurHP, int nCurMP) const
{
	return ((m_bWatchPotionHP && nCurHP < m_nPotionHP)
		|| (m_bWatchAlertMP && nCurMP < m_nAlertMP));
}

BOOL CGameInfo::NeedHealMerc(int nMercHPPercent) const
{
	return m_bWatchPotionMerc && nMercHPPercent < m_nPotionMercPercent;
}

BOOL CGameInfo::ParseDNTStrProc(LPCTSTR lpszStr, LPVOID lpData)
{
	CArrayEx<WORD, WORD>* pArray = (CArrayEx<WORD, WORD>*)lpData;
	if (pArray == NULL || lpszStr == NULL)
		return FALSE;

	pArray->Add((WORD)strtoul(lpszStr, NULL, 16));
	return TRUE;
}
