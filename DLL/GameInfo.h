#ifndef __GAMEINFO_H__
#define __GAMEINFO_H__

#include <windows.h>
#include "D2lib\\template\\ArrayEx.h"
#include "PathInfo.h"
#include "D2Skill.h"
#include "D2Character.h"
#include "D2Action.h"
#include "D2Scheme.h"

class CIniFile;

#define SP_NONE	0
#define SP_SENT	1
#define SP_RECEIVED	2
#define SP_BOTH	3
#define ID_LEN	4
#define COMBO_START	39

class CGameInfo
{
public:
	BOOL NeedHealMerc(int nMercHPPercent) const;
	BOOL NeedPotionHeal(int nCurHP, int nCurMP) const;
	int GetAmmoType() const;
	int GetAmmoBuyQty() const;
	LPCSTR GetSafetySchemeName() const;
	const LOCATION* GetWalkRoute() const;
	const LOCATION* GetTeleRoute() const;
	BOOL LoadScheme(LPCTSTR lpCharName);
	BOOL UpdateHPMP(int nCurHP, int nCurMP, BOOL bAddOnly);
	int NeedRetreat(int nCurHP, int nCurMP) const;
	int NeedMalah(int nCurHP, int nCurMP) const;
	void CalcLocation(WORD x, WORD y);
	void LoadOtherSettings();
	CGameInfo() { Initialize(); }
	void Initialize();

	CD2Scheme m_scheme;

	BOOL m_bWatchMalahHP;
	BOOL m_bWatchMalahMP;
	BOOL m_bWatchAlertHP;
	BOOL m_bWatchAlertMP;
	BOOL m_bWatchPotionHP;
	BOOL m_bWatchPotionMP;
	BOOL m_bWatchPotionMerc;

	int m_nMalahHPPercent;
	int m_nMalahMPPercent;
	int m_nAlertHPPercent;
	int m_nAlertMPPercent;
	int m_nPotionHPPercent;
	int m_nPotionMPPercent;
	int m_nPotionMercPercent;

	int m_nMalahHP;
	int m_nMalahMP;
	int m_nAlertHP;
	int m_nAlertMP;
	int m_nPotionHP;
	int m_nPotionMP;

	int m_nMaxHP;
	int m_nMaxMP;

	BOOL m_bWatchMerc;
	BOOL m_bShowSettings;

	BOOL m_bResMerc;
	BOOL m_bClientWalk;	

	// Pindleskin attribute combos
	DWORD IsComboFind(const CArrayEx<WORD, WORD>& arr) const;
	CArrayEx<WORD, WORD> m_aAttrs;

private:
	void ValidateCombos();
	BOOL LoadSafety(LPCTSTR lpCharName);
	void LoadCombos(LPCSTR lpSafety);

	int m_nLocationAdj;
//	int m_nLocationRand;
//	int m_nTimeRand;

	int m_nAmmoType;
	int m_nAmmoBuyQty;

	LOCATION m_aWalkToPindle[5];
	LOCATION m_aTeleToPindle[4];
protected:
	static BOOL CALLBACK ParseDNTStrProc(LPCTSTR lpszStr, LPVOID lpData);
};

class CShowPacket
{
public:
	CShowPacket() { Initialize(); }
	void Initialize();
	BOOL IsShow(BYTE* aPacket, DWORD aLen, BOOL bSent) const;
	BYTE show;
	BYTE pckID;
	BYTE pckLen;
	BYTE filter[64];
	BYTE filterLen;
};

#endif