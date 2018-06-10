///////////////////////////////////////////////////////////////////
// Pindlebot.cpp
//
// Global stuff and d2hackit template.
//
// 2003 Abin (abinn32@163.com)
///////////////////////////////////////////////////////////////////

#include "Globals.h"
#include "Pindlebot.h"
#include <time.h>
#include "Waypoint.h"
#include "memdef.h"
#include "PickitHelper.h"
#include <math.h>
#include "Algorithm.h"
#include "external\\clientcore.cpp"
#include "D2Lib\\D2StaticObject.h"
#include "D2Lib\\D2Unit.h"
#include "D2Lib\\D2Merc.h"
#include "Pindleskin.h"
#include "KeyBind.h"

BOOL g_bDebug = FALSE;

CPickitHelper g_pickit;
CGameInfo g_gameinfo; // game information
CKeyBind g_keyBind;

CD2Merc g_Merc;
CD2Unit g_Malah;
CD2Unit g_Larzuk;
CD2Unit g_Qual;
CPindleskin g_Pindleskin;
CD2StaticObject g_redPortal;
CD2StaticObject g_stash;
CD2StaticObject g_act1Wp;

// timer
DWORD g_dwTotalTime = 0;

int TeleFail = 0;
int g_nStepFail = 0;
int g_nInitLocFail = 0;
int g_nCurState = MS_NONE;
int g_nPrevState = MS_NONE;
int g_nTotalGame = 0;
BOOL g_bPaused = FALSE;
BOOL g_bCharDead = FALSE;

const DWORD TICK = 100;

BOOL g_bUseSig = FALSE;

//////////////////////////////////////////////////////////////////////
// CLIENTINFO
//////////////////////////////////////////////////////////////////////
DWORD   ModuleVersion=MAKELONG(5, 0);
char    ModuleAuthor[]="Abin";
char    ModuleDescription[]="Pindlebot for SIG";
char    ModuleWebsite[]="http://www.wxjindu.com/abin/";
char    ModuleEmail[]="abinn32@yahoo.com";

THISGAMESTRUCT *thisgame = NULL;

#define PLAYERID	(thisgame->player->PlayerID)
#define PLAYERX		(thisgame->player->PlayerPositionX)
#define PLAYERY		(thisgame->player->PlayerPositionY)

MOTIONSTATE g_state; // current state
CIni g_ini; // ini file accessor
CIni g_CharIni; // character specified data ini
CIni g_SafetyIni; // safety scheme data ini
CIni g_logIni; // log files
CTicker g_ticker; // ticker

CArrayEx<WORD, WORD> g_aSkills; // All available skills for current player
CArrayEx<DWORD, DWORD> g_aCorposes; // Corposes list
CArrayEx<WORD, WORD> g_aAttrs; // Pindleskin attributes + aura

// alterable routes
const LOCATION* g_pTownRoute = TO_PORTAL; // route to red portal(town walk)
const LOCATION* g_pTempleRoute = NULL; // route approach pindleskin

#define HARDDELAY	250 // mandatory spell delay, will be added to every action

#define MAX_LOC_FAIL	10

BOOL g_bStop = TRUE; // stopped?
BOOL g_bStarted = FALSE;
BOOL g_bInTown = TRUE;
BOOL g_bLogPackets = FALSE;
BOOL g_bGameReady = FALSE;
BOOL g_bShowMsg = TRUE;
BOOL g_bUseTeleport = FALSE;
BOOL g_bTeleportOK = FALSE;
BOOL g_bLarzukOK = FALSE; // Talked to Larzuk successfully
BOOL g_bMalahOK = FALSE; // Talked to Malah successfully
BOOL g_bPindleAwaken = FALSE; // Pindleskin has been awaken

// Skill casting results
enum { SCR_SUCCEEDED = 0, SCR_INVALIDSKILL, SCR_OUTOFAMMO };

int g_nUpdateRetry = 0;
int g_nNPCFail = 0;

DWORD g_dwArrowID = 0x00;
DWORD g_dwBoltID = 0x00;
#define ARROWPRICE	0x51
#define BOLTPRICE	0x3a;
BOOL g_nAmmoRetry = 0;
WORD g_wAmmoQuantity = 0;
#define _MAX_AMMORETRY	4

int g_nPortalRetry = 0;
int g_nQuitRetry = 0;
int g_nLocationRetry = 0;
int g_nMercRetry = 0;

int g_nCurHP = 0;
int g_nCurMP = 0;

BOOL g_bGameJoined = FALSE;

MODULECOMMANDSTRUCT ModuleCommands[]=
{
	{
		"help",  OnGameCommandHelp,
		"help ÿc0 List commands available in this module.\n"
		"<command> help ÿc0 Shows detailed help for <command> in this module."
	},

	{
		"pause", OnGameCommandPause,
		"pause ÿc0 Pause/resume the bot."
	},

	{
		"statistics", OnGameCommandStatistics,
		"statistics ÿc0 Display current running results."
	},

	{
		"start", OnGameCommandStart,
		"start ÿc0 Start current run."
	},

	{
		"stop", OnGameCommandStop,
		"stop ÿc0 Stop pindlebot."
	},

	{
		"debug", OnGameCommandDebug,
		"Debug ÿc0 For debug purpose, users should ÿc1notÿc0 use this."
	},

	{NULL}
};

BOOL CALLBACK OnHotkey(BYTE iVkCode, LPCTSTR lpszCommand)
{
	if (lpszCommand && lpszCommand[0] != '\0')
		server->GameCommandLine((LPTSTR)lpszCommand);
	return TRUE;
}

BOOL ExamAttributes()
{
	DWORD dwRes = g_gameinfo.IsComboFind(g_aAttrs);
	if (dwRes == 0)
		return FALSE;

	WORD wAttr1 = LOWORD(dwRes);
	WORD wAttr2 = HIWORD(dwRes);
	
	char szAttr1[64] = "";
	char szAttr2[64] = "";

	BYTE i = 0;
	if (wAttr1 | 0x8000) // aura
	{
		i = (BYTE)(wAttr1 & ~0x8000);
		g_Pindleskin.GetAuraName(i, szAttr1, 63);
	}
	else // bonus
	{
		i = (BYTE)wAttr1;
		g_Pindleskin.GetBonusName(i, szAttr1, 63);
	}

	if (wAttr2 | 0x8000) // aura
	{
		i = (BYTE)(wAttr2 & ~0x8000);
		g_Pindleskin.GetAuraName(i, szAttr2, 63);
	}
	else // bonus
	{
		i = (BYTE)wAttr2;
		g_Pindleskin.GetBonusName(i, szAttr2, 63);
	}

	server->GamePrintInfo("ÿc1Dangerous monster attributes found:");
	char szBonuses[128] = "";
	if (szAttr1[0] != '\0')
		strcat(szBonuses, szAttr1);

	if (szAttr2[0] != '\0')
	{
		strcat(szBonuses, " + ");
		strcat(szBonuses, szAttr2);
	}

	server->GamePrintInfo(szBonuses);
	return TRUE;
}

void CALLBACK DebugMessage(LPCVOID lpObject, LPCTSTR lpszMessage, LPVOID lpData)
{
	if (lpszMessage)
		server->GamePrintInfo(lpszMessage);
}

void CALLBACK OnMercMessage(LPCVOID lpObject, UINT nMessage, WPARAM wParam, LPARAM lParam, LPVOID lpData)
{
	switch (nMessage)
	{
	case MM_HPPERCENTDOWN:
		if (g_gameinfo.NeedHealMerc((int)lParam))
		{
			g_pickit.GetBelt().HealMerc();
			server->GamePrintInfo("ÿc2Fed your merc a potion.");
			g_logIni.IncreaseInt("Log", "Potion Merc");
		}
		break;

	case MM_DEATH:
		server->GamePrintInfo("Merc has been ÿc1killedÿc0.");
		if (g_gameinfo.m_bWatchMerc && !g_bCharDead)
			FinishRun(FR_MERCDEAD, TRUE);
		g_logIni.IncreaseInt("Log", "Merc die");
		break;

	default:
		break;
	}
}

void CALLBACK OnPindleskinMessage(LPCVOID lpObject, UINT nMessage, WPARAM wParam, LPARAM lParam, LPVOID lpData)
{
	char szMsg[256] = "";
	const BYTE* pBonuses = NULL;
	BYTE iAura = MONSTER_AURA_NONE;

	switch (nMessage)
	{
	case PM_BONUSES:
		pBonuses = (const BYTE*)lParam;
		if (pBonuses)
		{
			// check for deadly combos
			g_aAttrs.RemoveAll();
			for (int i = 0; i < PINDLE_MAX_BONUS; i++)
			{
				if (pBonuses[i])
					g_aAttrs.Add((WORD)pBonuses[i]);
			}

			if (g_gameinfo.IsComboFind(g_aAttrs))
			{
				server->GamePrintInfo("ÿc1Dangerous monster attributes found:");

				FinishRun(FR_DEADLYCOMBO, FALSE);
			}
		}
		break;
		
	case MM_AURAACTIVATE:
		iAura = (BYTE)lParam;
		if (iAura)
		{
			// check for deadly combos
			g_aAttrs.Add((WORD)iAura | 0x8000);
			if (g_gameinfo.IsComboFind(g_aAttrs))
			{
				FinishRun(FR_DEADLYCOMBO, FALSE);
			}
		}
		break;

	case MM_CORPSE:
		// pindle dead
		server->GamePrintInfo("ÿc2Pindleskinÿc0 has been killed.");
		g_nCurState = MS_PINDLE_DEAD;
		g_pickit.SetRadius(99);
		g_ticker.ResetTicker(500);
		break;

	default:
		break;
	}
}

void VerifyStep(int nPrevState)
{
	g_nPrevState = nPrevState;
	g_nCurState = MS_VERIFY_STEP;
}

void ApproachWp()
{
	if (g_act1Wp.GetPositionX() < PLAYERX)
	{
		MoveCharTo(PLAYERX, PLAYERY - 16);
	}
	else
	{
		MoveCharTo(PLAYERX + 26, PLAYERY - 6);
	}
}

BOOL MalahNotInTheWay()
{
	return ((g_Malah.GetPositionY() <= APPROACH_MALAH.y - 3)
		|| (g_Malah.GetPositionY() <= PLAYERY - 2));
}

void ShowOverheadMsg(LPCSTR lpMsg)
{
	if (g_bShowMsg)
		OverHeadMsgFake(PLAYERID, lpMsg);
}

BOOL ProcessCorpose()
{
	if (g_gameinfo.m_scheme.m_aOnCorpose.IsEmpty()
		|| g_aCorposes.IsEmpty())
	{
		return FALSE;
	}

	// if there are more than 1 skills in this scheme, randomly pick 1
	int nIndex = rand() % g_gameinfo.m_scheme.m_aOnCorpose.GetSize();
	WORD wSkillID = g_gameinfo.m_scheme.m_aOnCorpose[nIndex].GetSkillID();
	
	// validate skill first, as always...
	if (!IsSkillValid(wSkillID))
	{
		g_gameinfo.m_scheme.m_aOnCorpose.RemoveAt(nIndex);
		return FALSE;
	}

	// cast the skill on the first corpose(first die first service, LOL)
	D2SKILLINFO dsi;
	CD2Skill::GetSkillInfo(wSkillID, &dsi);
	CastOnEntity(g_aCorposes[0], wSkillID, TRUE);

	// remove the corpose from list since it's been "used"
	g_aCorposes.RemoveAt(0);

	return TRUE;
}

BOOL NeedResMerc()
{
	return (g_gameinfo.m_bResMerc
		&& !g_Merc.IsAlive()
		&& (g_pickit.GetGoldManager().GetInventoryGold() + g_pickit.GetGoldManager().GetStashGold() >= 50000));
}

BOOL NeedRefillAmmo()
{
	return (g_gameinfo.GetAmmoBuyQty() > 0 && g_wAmmoQuantity < 5);
}

void RefillAmmo()
{
	if (!g_Larzuk.IsValidObject())
		return;
	
	ShowOverheadMsg("Refilling ammo.");
	OpenStore(g_Larzuk.GetObjectID());
	// refill ammo
	int nAmmoType = g_gameinfo.GetAmmoType();
	//char sz[32] = "";
	//sprintf(sz, "arrow id: %d   bolt id: %d", g_dwArrowID, g_dwBoltID);
	//server->GamePrintInfo(sz);
	if (nAmmoType == AMMO_ARROW)
	{
		server->GamePrintInfo("buy arrow");
		BuyItem(g_dwArrowID, g_Larzuk.GetObjectID());
	}
	else if (nAmmoType == AMMO_BOLT)
	{
		server->GamePrintInfo("buy bolt");
		BuyItem(g_dwBoltID, g_Larzuk.GetObjectID());
	}
	else
	{
	}
}

// perform an action
int PerformAction(const CD2Action& action)
{
	char sMsg[128] = "";
	char sSkillName[MAX_SKILLNAME_LEN] = "";
	CD2Skill d2s(action.GetSkillID());
	d2s.GetSkillName(sSkillName);
	if (!IsSkillValid(action.GetSkillID())) // invalid skill
	{
		sprintf(sMsg, "Skill ÿc7%sÿc0 is unavailable to this player.", sSkillName);
		server->GamePrintError(sMsg);
		return SCR_INVALIDSKILL;
	}

	int nResult = SCR_SUCCEEDED;

	if (g_gameinfo.GetAmmoType() > 0 && g_wAmmoQuantity == 0)
	{
		nResult = SCR_OUTOFAMMO;
	}

	if (g_nCurState == MS_CAST_ONPINDLESKIN)
		ForceClientViewLocation(PLAYERX, PLAYERY, 0);
	
	WORD wSkillID = action.GetSkillID();
	BOOL bRight = (action.GetSide() == SAS_RIGHT);

	sprintf(sMsg, "Casting: \"%s\"(%d)", sSkillName, g_state.nStepOnSkill + 1);
	ShowOverheadMsg(sMsg);
	SIGPostString(sMsg);

	WORD DIFX = (PLAYERX > g_Pindleskin.GetPositionX()) ? (PLAYERX - g_Pindleskin.GetPositionX()) : (g_Pindleskin.GetPositionX() - PLAYERX);
	WORD DIFY = (PLAYERY > g_Pindleskin.GetPositionY()) ? (PLAYERY - g_Pindleskin.GetPositionY()) : (g_Pindleskin.GetPositionY() - PLAYERY);
	WORD wTargetX = 0;
	WORD wTargetY = 0;
	
	if (action.GetTarget() == CAT_NONE)
	{
		CastOnMap(PLAYERX, PLAYERY, wSkillID, bRight);
	}
	else if (action.GetTarget() == CAT_BEFOREPINDLESKIN)
	{
		wTargetX = (PLAYERX > g_Pindleskin.GetPositionX()) ? (g_Pindleskin.GetPositionX() + DIFX / 2) : (g_Pindleskin.GetPositionX() - DIFX / 2);
		wTargetY = (PLAYERY > g_Pindleskin.GetPositionY()) ? (g_Pindleskin.GetPositionY() + DIFY / 2) : (g_Pindleskin.GetPositionY() - DIFY / 2);
		CastOnMap(wTargetX, wTargetY, wSkillID, bRight);
	}
	else if (action.GetTarget() == CAT_BEYONDPINDLESKIN)
	{
		Truncate(DIFX, 5, 15);
		Truncate(DIFY, 5, 15);
		wTargetX = (PLAYERX > g_Pindleskin.GetPositionX()) ? (g_Pindleskin.GetPositionX() - DIFX) : (g_Pindleskin.GetPositionX() + DIFX);
		wTargetY = (PLAYERY > g_Pindleskin.GetPositionY()) ? (g_Pindleskin.GetPositionY() - DIFY) : (g_Pindleskin.GetPositionY() + DIFY);
		CastOnMap(wTargetX, wTargetY, wSkillID, bRight);
	}
	else
	{
		// by default target directly on pindleskin
		CastOnEntity(g_Pindleskin.GetObjectID(), wSkillID, bRight);
	}

	return nResult;
}

void GetSkillList(const BYTE* aPacket, DWORD aLen)
{
	g_aSkills.RemoveAll();

	const BYTE COUNT = aPacket[1];// number of skills
	WORD wSkillID = 0x0000;

	for (int i = 0, j = 6; i < COUNT && j <= aLen - 3; i++, j += 3)
	{
		::memcpy(&wSkillID, aPacket + j, 2);
		g_aSkills.Add(wSkillID);
	}	

	// check to see if Teleport is available to this player,
	// and pick different route, as teleporting to pindleskin
	// requires 3 steps, whereas walk to pindleskin requires 4 steps
	if (g_aSkills.Find(D2S_TELEPORT) != -1)
	{
		g_bUseTeleport = TRUE;
		g_pTempleRoute = g_gameinfo.GetTeleRoute();
	}
	else
	{
		g_bUseTeleport = FALSE;
		g_pTempleRoute = g_gameinfo.GetWalkRoute();
	}
}

BOOL IsSkillValid(WORD wSkillID)
{
	return (g_aSkills.Find(wSkillID) != -1);
}

void ShowCharSet()
{
	char sMsg[128] = "";
	
	server->GamePrintInfo(" ");
	server->GamePrintInfo("ÿc7-------------------------------------------");
	sprintf(sMsg, "Safety Setting ÿc2%sÿc0 loaded for ÿc7%s%ÿc0:", g_gameinfo.GetSafetySchemeName(), thisgame->player->PlayerName);
	server->GamePrintInfo(sMsg);
	server->GamePrintInfo(" ");
	sprintf(sMsg, "Max HP: ÿc2%d", g_gameinfo.m_nMaxHP);
	server->GamePrintInfo(sMsg);

	sprintf(sMsg, "Max Mana: ÿc2%d", g_gameinfo.m_nMaxMP);
	server->GamePrintInfo(sMsg);

	if (g_gameinfo.m_bWatchMalahHP)
	{
		sprintf(sMsg, "Malah HP: ÿc2%d ÿc0(%d%%)", g_gameinfo.m_nMalahHP, g_gameinfo.m_nMalahHPPercent);
		server->GamePrintInfo(sMsg);
	}
	else
	{
		server->GamePrintInfo("Malah HP: ÿc1disabled");
	}	

	if (g_gameinfo.m_bWatchMalahMP)
	{
		sprintf(sMsg, "Malah Mana: ÿc2%d  ÿc0(%d%%)", g_gameinfo.m_nMalahMP, g_gameinfo.m_nMalahMPPercent);
		server->GamePrintInfo(sMsg);
	}
	else
	{
		server->GamePrintInfo("Malah Mana: ÿc1disabled");
	}

	if (g_gameinfo.m_bWatchPotionHP)
	{
		sprintf(sMsg, "Potion HP: ÿc2%d ÿc0(%d%%)", g_gameinfo.m_nPotionHP, g_gameinfo.m_nPotionHPPercent);
		server->GamePrintInfo(sMsg);
	}
	else
	{
		server->GamePrintInfo("Potion HP: ÿc1disabled");
	}

	if (g_gameinfo.m_bWatchPotionMP)
	{
		sprintf(sMsg, "Potion MP: ÿc2%d ÿc0(%d%%)", g_gameinfo.m_nPotionMP, g_gameinfo.m_nPotionMPPercent);
		server->GamePrintInfo(sMsg);
	}
	else
	{
		server->GamePrintInfo("Potion MP: ÿc1disabled");
	}

	if (g_gameinfo.m_bWatchAlertHP)
	{
		sprintf(sMsg, "Retreat HP: ÿc2%d  ÿc0(%d%%)", g_gameinfo.m_nAlertHP, g_gameinfo.m_nAlertHPPercent);
		server->GamePrintInfo(sMsg);
	}
	else
	{
		server->GamePrintInfo("Retreat HP: ÿc1disabled");
	}

	if (g_gameinfo.m_bWatchAlertMP)
	{
		sprintf(sMsg, "Retreat Mana: ÿc2%d  ÿc0(%d%%)", g_gameinfo.m_nAlertMP, g_gameinfo.m_nAlertMPPercent);
		server->GamePrintInfo(sMsg);
	}
	else
	{
		server->GamePrintInfo("Retreat Mana: ÿc1disabled");
	}

	server->GamePrintInfo("ÿc7-------------------------------------------");
	server->GamePrintInfo(" ");
}

BOOL IsInGame()
{
	return (thisgame != NULL
			&& thisgame->player != NULL
			&& thisgame->player->PlayerPositionX > 0
			&& thisgame->player->PlayerPositionY > 0);
}

BOOL IsInRedPortal()
{
	if (thisgame == NULL)
		return FALSE;

	//0x05=in town, 0x01=out of town, 0x03=moving
	return thisgame->player->PlayerLocation == 0x01;
}

void FinishRun(int nReason, BOOL bRunAway)
{	
	// prepare to quit current game
	g_bStarted = FALSE;
	g_bPaused = FALSE;
	g_nCurState = MS_FINISHED;
	
	// ends any previous waiting states
	g_ticker.ResetTicker(0);

	// in case D2ExitGame function fails...
	g_nQuitRetry++;
	if (g_nQuitRetry > 3)
	{
		if (g_nQuitRetry < 6)
		{			
			SIGLeave();
		}
		else // g_nQuitRetry >= 6, get major error here
		{
			SIGQuitD2();
			return;
		}		
	}

	char szMsg[128] = "";
	//g_monster.GetComboString(szMsg, FALSE);

	switch (nReason)
	{
	case FR_SUCCEEDED:
		server->GamePrintInfo("Game finished by reason: ÿc2succeeded.");
		SIGPostString("ÓÎÏ·½áÊø£ºÁÔÉ±³É¹¦");
		g_logIni.IncreaseInt("Log", "Succeed");
		break;

	case FR_INDANGER:
		if (bRunAway)
		{
			// Immediately run away from Pindleskin
			MoveCharTo(ADJ_RETREAT.x, ADJ_RETREAT.y);
		}
		server->GamePrintInfo("Game finished by reason: ÿc1retreating.");
		SIGPostString("ÓÎÏ·½áÊø£º½ÇÉ«³·ÍË");
		g_logIni.IncreaseInt("Log", "Retreat");
	//	g_logIni.IncreaseInt("Retreat", szMsg);
		break;

	case FR_CHARDEAD:

		g_bCharDead = TRUE;
		server->GamePrintInfo("Game finished by reason: ÿc1character slain.");
		SIGPostString("ÓÎÏ·½áÊø£º½ÇÉ«ËÀÍö :(");
		BYTE aPacket[1];
		aPacket[0] = 0x41;
		server->GameSendPacketToServer(aPacket, 1);
		g_logIni.IncreaseInt("Log", "Char die");
	//	g_logIni.IncreaseInt("Char Dead", szMsg);
		break;

	case FR_FAILED:
		server->GamePrintInfo("Game finished by reason: ÿc1failed.");
		SIGPostString("ÓÎÏ·½áÊø£ºÔËÐÐÔÝÊ±Ê§°Ü");
		g_logIni.IncreaseInt("Log", "Fail");
		break;

	case FR_REPAIR:
		server->GamePrintInfo("Game finished by reason: ÿc7repair.");
		SIGPostString("ÓÎÏ·½áÊø£ºÐÞÀíÎïÆ·");
		g_logIni.IncreaseInt("Log", "Repair");
		break;

	case FR_MERCDEAD:
		if (bRunAway)
		{
			// Immediately run away from Pindleskin
			MoveCharTo(ADJ_RETREAT.x, ADJ_RETREAT.y);
		}
		server->GamePrintInfo("Game finished by reason: ÿc1merc death.");
		SIGPostString("ÓÎÏ·½áÊø£º¹ÍÓ¶±øËÀÍö");	
		g_logIni.IncreaseInt("Log", "Retreat");
		break;

	case FR_DEADLYCOMBO:
		//server->GamePrintInfo("Deadly Combo:");
		SIGPostString(szMsg);
		g_logIni.IncreaseInt("Log", "Avoid");
	//	g_logIni.IncreaseInt("Avoid", szMsg);
		break;

	case FR_OUTSKILL:
		server->GamePrintInfo("Game finished by reason: ÿc1insufficient skills.");
		SIGPostString("ÓÎÏ·½áÊø£º¼¼ÄÜ²»×ã");
		g_logIni.IncreaseInt("Log", "Ins Skill");
	//	g_logIni.IncreaseInt("Ins Skill", szMsg);
		break;

	case FR_WARPWP:
		server->GamePrintInfo("Game finished by reason: ÿc2WP warping.");
		SIGPostString("ÓÎÏ·½áÊø£º×ª»»³¡¾°");
		g_logIni.IncreaseInt("Log", "Warp");
		break;

	default:		
		break;
	}

	g_nCurState = MS_PREPARETOQUIT;
	g_ticker.ResetTicker(0);
}

BOOL EXPORT OnClientStart()
{
	g_Malah.SetFingerprinter(&FP_NPC_MALAH);
	g_Larzuk.SetFingerprinter(&FP_NPC_LARZUK);
	g_Qual.SetFingerprinter(&FP_NPC_QUAL);
	g_redPortal.SetFingerprinter(&FP_OBJ_ACT5REDPORTAL);
	g_stash.SetFingerprinter(&FP_OBJ_STASH);
	g_act1Wp.SetFingerprinter(&FP_OBJ_ACT1WP);

	g_Merc.SetDebugMessageProc(DebugMessage, NULL, "merc");
	if (g_bDebug)
	{
		
		g_Malah.SetDebugMessageProc(DebugMessage, NULL, "Malah");
		g_Larzuk.SetDebugMessageProc(DebugMessage, NULL, "larzuk");
		g_Qual.SetDebugMessageProc(DebugMessage, NULL, "Qual");
		g_redPortal.SetDebugMessageProc(DebugMessage, NULL, "RedPortal");
		g_stash.SetDebugMessageProc(DebugMessage, NULL, "Stash");
		g_act1Wp.SetDebugMessageProc(DebugMessage, NULL, "Act1WP");
		g_Pindleskin.SetDebugMessageProc(DebugMessage, NULL, "Pindleskin");
	}
	
	g_Merc.SetD2MessageProc(OnMercMessage);	
	g_Pindleskin.SetD2MessageProc(OnPindleskinMessage);

	g_keyBind.InstallKeyBind();
	g_keyBind.SetKeyBindProc(OnHotkey);
	g_keyBind.SetEntry(VK_F9, 0, "pindlebot statistics");
	g_keyBind.SetEntry(VK_F10, 0, "pindlebot start");
	g_keyBind.SetEntry(VK_F11, 0, "pindlebot stop");
	g_keyBind.SetEntry(VK_F12, 0, "pindlebot pause");

	srand((unsigned)time(NULL));

	// create \scripts\output dir
	char szPath[_MAX_PATH + 1] = "";
	//sprintf(szPath, "%s\\Scripts", GetModuleDirectory());
	//::CreateDirectory(szPath, NULL);

	//sprintf(szPath, "%s\\output", szPath);
	//::CreateDirectory(szPath, NULL);

	sprintf(szPath, "%s\\Pindlebot.txt", GetModuleDirectory());
	g_logIni.SetPathName(szPath);

	// load settings from ini
	sprintf(szPath, "%s\\Pindlebot.ini", GetModuleDirectory());
	g_ini.SetPathName(szPath);

	sprintf(szPath, "%s\\CharSpec.dat", GetModuleDirectory());
	g_CharIni.SetPathName(szPath);

	sprintf(szPath, "%s\\Safety.dat", GetModuleDirectory());
	g_SafetyIni.SetPathName(szPath);

	g_gameinfo.LoadOtherSettings();

	// load pickit config
	sprintf(szPath, "%s\\pindlebot.ini", GetModuleDirectory());

	// load configuration
	char sFile[_MAX_PATH] = "";
	::GetPrivateProfileString("general", "pickit", "pickit.ini", sFile, _MAX_PATH - 1, szPath);
	
	if (strlen(sFile) < 5)
		strcpy(sFile, "pickit.ini");
	sprintf(szPath, "%s\\%s", GetModuleDirectory(), sFile);

	g_pickit.LoadConfig(szPath);

	return TRUE;
}

BOOL EXPORT OnClientStop()
{
	g_keyBind.UninstallKeyBind();
	return TRUE;
}

void EXPORT OnGameJoin(THISGAMESTRUCT* game)
{
	g_pickit.OnGameJoin(game->player->PlayerID, &game->player->PlayerPositionX, &game->player->PlayerPositionY);
	g_Merc.OnGameJoin(game->player->PlayerID, &game->player->PlayerPositionX, &game->player->PlayerPositionY);
	g_Pindleskin.OnGameJoin();
	g_Malah.OnGameJoin();
	g_Larzuk.OnGameJoin();
	g_Qual.OnGameJoin();
	g_redPortal.OnGameJoin();
	g_stash.OnGameJoin();
	g_act1Wp.OnGameJoin();

	g_dwTotalTime = 0;
	g_wAmmoQuantity = 0;
	thisgame = game;
	srand((unsigned)time(NULL));

	g_bPaused = FALSE;		
	g_bGameJoined = TRUE;
	g_nCurState = MS_NONE;
	g_nPrevState = MS_NONE;
	g_nTotalGame++;

	g_pTownRoute = TO_PORTAL;

	memset(&g_state, 0, sizeof(MOTIONSTATE));
	g_nCurHP = 0;
	g_nCurMP = 0;
	g_nNPCFail = 0;
	g_bInTown = TRUE;
	g_nUpdateRetry = 0;
	g_bStarted = FALSE;
	g_nPortalRetry = 0;
	g_nQuitRetry = 0;
	g_nLocationRetry = 0;
	g_nMercRetry = 0;
	g_nStepFail = 0;
	g_bCharDead = FALSE;
	g_bLarzukOK = FALSE;
	g_bMalahOK = FALSE;
	g_bPindleAwaken = FALSE;
	g_bTeleportOK = FALSE;
	g_nInitLocFail = 0;

	g_bStop = TRUE;
	g_aCorposes.RemoveAll();
	g_aAttrs.RemoveAll();

	if (!g_gameinfo.LoadScheme(thisgame->player->PlayerName))
	{
		char sMsg[128] = "";
		sprintf(sMsg, "Cannot find a scheme for ÿc7%sÿc0, game stopped.", thisgame->player->PlayerName);
		server->GamePrintError(sMsg);
		OnGameCommandStop(NULL, 0);
		SIGPostString("ÎÞ·¨ÕÒµ½½ÇÉ«¼¼ÄÜ·½°¸");
		return;
	}
	
	g_logIni.IncreaseInt("Log", "Game");
	g_pickit.SetRadius(0);
	//OnGameCommandStart(NULL, 0);
}

void EXPORT OnGameLeave(THISGAMESTRUCT* game)
{
	g_Merc.OnGameLeave();
	g_Pindleskin.OnGameLeave();
	g_Malah.OnGameLeave();
	g_Larzuk.OnGameLeave();
	g_Qual.OnGameLeave();
	g_redPortal.OnGameLeave();
	g_stash.OnGameLeave();
	g_act1Wp.OnGameLeave();
	g_bGameJoined = FALSE;
	g_pickit.OnGameLeave();
	
	g_bGameReady = FALSE;
	g_nCurState = MS_FINISHED;
	g_bStop = TRUE;	

	g_logIni.WriteBool("Log", "Running", FALSE);
	if (g_dwTotalTime > 0)
	{
		g_logIni.IncreaseInt("Log", "Run Time", g_dwTotalTime / 1000);
		g_dwTotalTime = 0;
	}
}

BOOL PRIVATE OnGameCommandStart(char** argv, int argc)
{
	BOOL bOK = FALSE;
	if (g_nCurState != MS_NONE) // aleady started
	{
		server->GamePrintInfo("This run is started already.");
		return TRUE;
	}

	server->GamePrintInfo("Starting Pindlebot ...");
	SIGPostString("ÕýÔÚÆô¶¯Pindlebot...");
	g_nCurState = MS_INIT_START; // started
	g_bStop = FALSE;
	g_ticker.ResetTicker(0);
	return TRUE;
}

BOOL PRIVATE OnGameCommandStop(char** argv, int argc)
{
	SIGStop(); // stop sig
	g_logIni.WriteBool("Log", "Running", FALSE);
	if (g_dwTotalTime > 0)
	{
		g_logIni.IncreaseInt("Log", "Run Time", g_dwTotalTime / 1000);
		g_dwTotalTime = 0;
	}

	server->GameCommandLine("unload mover");
	g_bStop = TRUE;
	g_bPaused = FALSE;
	g_bStarted = FALSE;

	// show statistics before stopping
	OnGameCommandStatistics(NULL, 0);
	server->GamePrintInfo("Pindlebot ÿc2stopped.");
	SIGPostString("PindlebotÒÑÍ£Ö¹");
	return TRUE;
}

//////////////////////////////////////////////////////////////////////
// OnGameTimerTick
// -------------------------------------------------------------------
// This gets executed apx. every 1/10th of a second when in a game.
//
// You can use this to create custom timers.
//////////////////////////////////////////////////////////////////////
DWORD EXPORT OnGameTimerTick(void)
{
	if (!g_bGameJoined)
		return 0;

	// quit game, highest priority
	if (g_nCurState == MS_PREPARETOQUIT)
	{
		// prepare to quit current game
		g_bStarted = FALSE;
		g_bPaused = FALSE;
		g_nCurState = MS_FINISHED;
		SIGLeave();	
		return 0;
	}

	if (g_bGameJoined)
		g_pickit.OnGameTimerTick();

	if (g_bStop)
		return 0;

	if (g_bGameJoined && g_bGameReady && !g_bPaused)
		g_dwTotalTime += TICK;
	
	g_ticker.TickIt(TICK);

	if (IsInGame())
	{
		if (!g_bGameReady)
		{			
			g_bGameReady = TRUE;
		}		
	}
	else
	{
		if (g_bGameReady)
		{
			// disconencted
			g_bGameReady = FALSE;
			SIGLeave();
			return 0;
		}
		else
		{
			// not started yet
			g_bGameReady = FALSE;
			return 0;
		}
	}

	if (g_nCurState == MS_PICKITEMS_OK && g_pickit.GetQueueItemCount(TRUE) == 0)
	{
		server->GamePrintInfo("No more valuable items queued.");
		FinishRun(FR_SUCCEEDED, FALSE);
		return 0;
	}

	if(g_bPaused
		||g_nCurState == MS_NONE
		|| g_nCurState == MS_FINISHED
		|| !g_ticker.IsTickEnded()
		|| g_bStop)
	{
		return 0;
	}

	// check for states
	switch (g_nCurState)
	{
	case MS_INIT_START:

		if (thisgame->player->Act != 0 && thisgame->player->Act != 4) // neither act5 nor act1...
		{
			SIGStop();
			server->GamePrintError("Character must be at either ÿc2Act5 or act1 initial locationÿc0 to start.");
			OnGameCommandStop(NULL, 0);
			SIGPostString("½ÇÉ«±ØÐëÎ»ÓÚact1»òact5");
			return 0;	
		}
		else
		{
			g_bStarted = TRUE;
		}

		if (g_gameinfo.m_nMaxHP > 0)
		{
			g_gameinfo.UpdateHPMP(g_nCurHP, g_nCurMP, TRUE);
			if (g_gameinfo.m_bShowSettings)
				ShowCharSet();
		}

		server->GamePrintInfo("Pindlebot ÿc2started.");
		SIGPostString("PindlebotÒÑÆô¶¯");

		if (g_bCharDead || g_nCurHP < 2)
		{
			ShowOverheadMsg("Pickup corpose.");
			SIGPostString("¼ñÊ°½ÇÉ«Ê¬Ìå");
			PickCorpose();
		}

		// if this is a Paladin, activate Vigor
		if (g_gameinfo.m_scheme.m_byCharClass == CCD_PALADIN
			&& IsSkillValid(D2S_VIGOR))
		{
			SelectSkill(D2S_VIGOR, TRUE);
		}

		// if this is an Assassin, activate Burst of speed
		if (g_gameinfo.m_scheme.m_byCharClass == CCD_ASSASSIN
			&& IsSkillValid(D2S_BURSTOFSPEED))
		{
			CastOnMap(PLAYERX, PLAYERY, D2S_BURSTOFSPEED, TRUE);
		}

		// if is at act1, goto act5
		if (thisgame->player->Act == 0)
		{
			g_nCurState = MS_APPROACH_WP;
		}

		// if need to repair items or refill ammo, travel to larzuk
		else if (NeedRefillAmmo())
		{
			// walk to Larzuk
			//MoveCharTo(LARZUK_STORE.x, LARZUK_STORE.y, g_gameinfo.GetLocRand());
			//g_ticker.ResetTicker(Randomize(g_gameinfo.m_nWalkDelay, g_gameinfo.GetTimeRand()));
			g_nCurState = MS_TRAVEL_LARZUK;
			g_ticker.ResetTicker(3000);
		}
		// run as normal
		else
		{
			g_nCurState = MS_ADJUST_LOCATION;
		}				
			
		break;

	case MS_APPROACH_WP:
		ShowOverheadMsg("Detecting WP coordinates");
		SIGPostString("ÕýÔÚÑ°ÕÒWPÂ·¾¶");
		ApproachWp();
		g_ticker.ResetTicker(2500);
		
		g_nCurState = MS_WALK_WP;
		break;

	case MS_WALK_WP:
		ShowOverheadMsg("Approaching WP...");
		MoveCharTo(g_act1Wp.GetPositionX(), g_act1Wp.GetPositionY());
		g_ticker.ResetTicker(1500);

		g_nCurState = MS_WARP_ACT5;
		break;

	case MS_WARP_ACT5:

		ShowOverheadMsg("Taking WP to Act5...");
		SIGPostString("ÒÆ¶¯µ½act5...");
		TakeWayPoint(g_act1Wp.GetObjectID(), WAYPOINT_ACT5_1);
		g_ticker.ResetTicker(6000);
		g_nCurState = MS_AFTER_WP;
		break;

	case MS_AFTER_WP:
		ShowOverheadMsg("Warped to Act5");
		SIGPostString("ÒÑ¾­µ½´ïact5");
		FinishRun(FR_WARPWP, FALSE);
		break;

	case MS_ADJUST_LOCATION:
				
		ShowOverheadMsg("Adjusting initial location");
		SIGPostString("È·¶¨ÆðÊ¼Â·¾¶");
		g_state.nStepOnPortal = 0;
		MoveCharTo(APPROACH_MALAH.x, APPROACH_MALAH.y);
		g_ticker.ResetTicker(250);
		g_nCurState = MS_VERYFY_INIT_LOCATION;		
				
		break;

	case MS_VERYFY_INIT_LOCATION:
		// verify the first move results
		if (g_nInitLocFail >= 20 || CheckPosition(APPROACH_MALAH.x, APPROACH_MALAH.y, 3))
		{
			// move on
			// here is a good time to drink junky potions
			int nDrank = g_pickit.GetBelt().DrinkAllDrunks();
			if (nDrank)
			{
				char sz[64] = "";
				sprintf(sz, "Drank %d junky potions.", nDrank);
				ShowOverheadMsg(sz);
				sprintf(sz, "ºÈµô%dÆ¿À¬»øÒ©Ë®", nDrank);
				SIGPostString(sz);
			}
			g_nInitLocFail = 0;
			g_nCurState = (g_gameinfo.NeedMalah(g_nCurHP, g_nCurMP) != 0) ? MS_WALK_MALAH : MS_AFTER_MALAH;
		}
		else
		{
			// do it again
			g_nInitLocFail++;
			g_nCurState = MS_VERYFY_INIT_LOCATION;
		}
		break;

	case MS_WALK_MALAH:
		ShowOverheadMsg("Walking to Malah.");
		SIGPostString("ÕýÔÚ½Ó½üMalah");
		// code here walk to Malah
		//StepToEntity(0x01, g_Malah.GetObjectID());
		g_nCurState = MS_TALK_MALAH;
		g_ticker.ResetTicker(500);
		break;

	case MS_TALK_MALAH:
		ShowOverheadMsg("Interacting with Malah.");
		SIGPostString("ÕýÔÚÓëMalah½»Ì¸");
		// code here talk to malah
		InteractWithEntity(0x01, g_Malah.GetObjectID());
		g_nCurState = MS_LEAVE_MALAH;
		g_ticker.ResetTicker(2000);
		break;

	case MS_LEAVE_MALAH:

		if (g_bMalahOK)
		{
			// Talked to Malah successfully
			ShowOverheadMsg("Leaving Malah.");
			SIGPostString("Àë¿ªMalah");
			CloseStore(g_Malah.GetObjectID());
			if (g_gameinfo.UpdateHPMP(g_nCurHP, g_nCurMP, FALSE))
				ShowCharSet();

			g_nCurState = MS_AFTER_MALAH;
		}
		else
		{
			g_nNPCFail++;

			if (g_nNPCFail < 10)
				g_nCurState = MS_ADJUST_LOCATION;
			else
				FinishRun(FR_FAILED, FALSE);
		}				

		break;

	case MS_AFTER_MALAH:

		if (!MalahNotInTheWay())
		{
			// make a detour around malah			
			ShowOverheadMsg("Making a detour around Malah...");
			SIGPostString("Malah×èµ²Â·¾¶£¬ÕýÔÚÈÆµÀ");
			MoveCharTo(AVOID_MALAH.x, AVOID_MALAH.y);
			g_ticker.ResetTicker(500);
		}
				
		g_state.nStepOnPortal = 0;
		g_nCurState = MS_WALK_PORTAL;

		break;

	case MS_WALK_PORTAL:

		if (g_state.nStepOnPortal == 3 && NeedResMerc())
		{
			// approach to Qual
			ShowOverheadMsg("Need to resurrect merc.");
			SIGPostString("ÐèÒª¸´»î¹ÍÓ¶±ø");
			MoveCharTo(APPROACH_QUAL.x, APPROACH_QUAL.y);
			g_ticker.ResetTicker(1500);
			g_nCurState = MS_WALK_QUAL;
		}
		else if (g_pTownRoute[g_state.nStepOnPortal].x == 0x00) // reached portal already
		{
			// Portal reached
			ShowOverheadMsg("Portal reached.");
			SIGPostString("µ½´ïºìÉ«´«ËÍÃÅ");
			// check HP.MP update here
			if (g_gameinfo.UpdateHPMP(g_nCurHP, g_nCurMP, TRUE))
			{
				server->GamePrintInfo("Character HP/MP updated.");
				ShowCharSet();
			}
					
			g_nCurState = MS_VERIFY_LOCATION;
		}
		else // keep on going to portal
		{
			char sz[32] = "";
			sprintf(sz, "Traveling to red portal (%d)", g_state.nStepOnPortal + 1);
			ShowOverheadMsg(sz);

			sz[0] = '\0';
			sprintf(sz, "×ßÏòºìÉ«´«ËÍÃÅ(%d)...", g_state.nStepOnPortal + 1);
			SIGPostString(sz);
					
			MoveCharTo(g_pTownRoute[g_state.nStepOnPortal].x, g_pTownRoute[g_state.nStepOnPortal].y);
			g_ticker.ResetTicker(250);
			g_nCurState = MS_VERIFY_STEP;
		}

		break;

	case MS_VERIFY_STEP:
				
		//0x05=in town, 0x01=out of town, 0x03=moving
		if (thisgame->player->PlayerLocation == 0x05)
		{
			g_state.nStepOnPortal++;
			g_nCurState = MS_WALK_PORTAL;
		}
				
		break;

	case MS_WALK_QUAL:
		ShowOverheadMsg("Walking to Qual-Kehk.");
		SIGPostString("×ßÏòQual-Kehk...");
		// code here to walk to Qual-Kehk
		//StepToEntity(0x01, g_Qual.GetObjectID());
		g_nCurState = MS_TALK_QUAL;
		g_ticker.ResetTicker(1500);
		break;

	case MS_TALK_QUAL:
		ShowOverheadMsg("Interacting with Qual-Kehk.");
		SIGPostString("ÓëQual-Kehk½»Ì¸");
		// code here to talk to Qual-Kehk
		InteractWithEntity(0x01, g_Qual.GetObjectID());
		//CloseStore(g_dwQualID);
		//InteractWithEntity(0x01, g_dwQualID);
		g_nCurState = MS_RES_MERC;
		g_ticker.ResetTicker(2000);
		break;

	case MS_RES_MERC:
		if (g_Merc.IsValidObject() && !g_Merc.IsAlive() && g_gameinfo.m_bResMerc)
		{
			ShowOverheadMsg("Requesting to resurrect merc.");
			SIGPostString("ÇëÇó¸´»î¹ÍÓ¶±ø");
			ResurrectMerc(g_Qual.GetObjectID());
		}
		g_nCurState = MS_LEAVE_QUAL;
		g_ticker.ResetTicker(1000);
		break;

	case MS_LEAVE_QUAL:
		ShowOverheadMsg("Leaving Qual-Kehk.");
		SIGPostString("Àë¿ªQual-Kehk");
		// code here to leave Qual-Kehk
		CloseStore(g_Qual.GetObjectID());
		MoveCharTo(TO_PORTAL[2].x, TO_PORTAL[2].y);
		g_ticker.ResetTicker(1500);
		g_nCurState = MS_WALK_PORTAL;
		break;

	case MS_TRAVEL_LARZUK:
			
		if (TO_LARZUK[g_state.nStepOnlarzuk].x == 0) // reached Larzuk's store
		{
			ShowOverheadMsg("Walking to Larzuk.");
			SIGPostString("×ßÏòLarzuk");
		//	StepToEntity(0x01, g_Larzuk.GetObjectID());
			g_nCurState = MS_TALK_LARZUK;
		}
		else // traveling to Larzuk's store
		{
			ShowOverheadMsg("Traveling to Larzuk.");
			SIGPostString("×ßÏòLarzuk...");
			MoveCharTo(TO_LARZUK[g_state.nStepOnlarzuk].x, TO_LARZUK[g_state.nStepOnlarzuk].y);
			g_state.nStepOnlarzuk++;
			g_ticker.ResetTicker(1500);
		}

		break;

	case MS_TALK_LARZUK:
		ShowOverheadMsg("Interacting with Larzuk.");
		SIGPostString("ÓëLarzuk½»Ì¸");
		OpenStore(g_Larzuk.GetObjectID());
		g_nCurState = MS_ASK_REPAIR;
		g_ticker.ResetTicker(3000);
		break;

	case MS_ASK_REPAIR:

		if (g_bLarzukOK)
		{
			// reached Larzuk successfully
			// check arrow/bolt
			if (NeedRefillAmmo())
			{						
				SIGPostString("ÕýÔÚ²¹³ä¼ýÖ§");
				// refill ammo
				for (int i = 0; i < g_gameinfo.GetAmmoBuyQty(); i++)
				{
					RefillAmmo();
					Sleep(1000);
				}
						
				g_wAmmoQuantity = 100;
				g_nAmmoRetry = 0;
			}

			g_nCurState = MS_LEAVE_LARZUK;
			g_ticker.ResetTicker(1500);
		}
		else
		{
			// failed to reach Larzuk
			ShowOverheadMsg("Failed to reach Larzuk, retrying...");
			SIGPostString("ÓëLarzuk½»»¥Ê§°Ü£¬ÕýÔÚÖØÊÔ...");

			g_nNPCFail++;

			if (g_nNPCFail < 10)
			{
				g_state.nStepOnlarzuk = 0;
				g_nCurState = MS_TRAVEL_LARZUK;
			}
			else
			{
				FinishRun(FR_FAILED, FALSE);
			}					
		}
				
		break;

	case MS_LEAVE_LARZUK:

		ShowOverheadMsg("Leaving Larzuk");
		CloseStore(g_Larzuk.GetObjectID());
		g_nCurState = MS_AFTER_LARZUK;
	//	g_ticker.ResetTicker(5000);

		break;

	case MS_AFTER_LARZUK:

		if (NeedResMerc() || g_gameinfo.NeedMalah(g_nCurHP, g_nCurMP) != 0)
		{
			FinishRun(FR_REPAIR, FALSE);
		}
		else
		{
			g_state.nStepOnPortal = 0;
			g_pTownRoute = LARZUK_TO_PORTAL;
			g_nCurState = MS_WALK_PORTAL;
		}
		break;

	case MS_VERIFY_LOCATION:

		if (!g_redPortal.IsValidObject())
		{
			server->GamePrintError("Failed to reach red portal.");
			SIGPostString("ÎÞ·¨µ½´ïºìÉ«´«ËÍÃÅ");
			FinishRun(FR_FAILED, FALSE);
			break;
		}

		//g_nLocationRetry++;
		//if (CheckPosition(PLAYERX, PLAYERY, PortalLocation.x, PortalLocation.y, 10))
		//{
			g_nCurState = MS_CAST_BEFORERED;
		//}
			/*
		else
		{
			ShowOverheadMsg("Verifying location.");
			SIGPostString("¼ì²â½ÇÉ«×ø±ê");
			if (g_nLocationRetry < MAX_LOC_FAIL)
			{
				TakeStep(PortalLocation.x, PortalLocation.y);
				g_ticker.ResetTicker(500);
			}
			else
			{
				// no more try
				server->GamePrintError("Failed to adjust location.");
				FinishRun(FR_FAILED, FALSE);
			}
		}*/
		break;

	case MS_CAST_BEFORERED:

		// actions before entering red portal
		if (g_state.nStepOnAction >= g_gameinfo.m_scheme.m_aBeforeRed.GetSize())
		{
			// all actions have been performed, now ready to enter portal
			g_state.nStepOnAction = 0;
			g_nCurState = MS_TALK_PORTAL;
		}
		else
		{
			// perform all actions in array
			if (g_state.nStepOnSkill < g_gameinfo.m_scheme.m_aBeforeRed[g_state.nStepOnAction].GetRepeat())
			{
				int nRes = PerformAction(g_gameinfo.m_scheme.m_aBeforeRed[g_state.nStepOnAction]);
				if (nRes == SCR_SUCCEEDED)
				{
					g_state.nStepOnSkill++;
					g_ticker.ResetTicker(HARDDELAY + g_gameinfo.m_scheme.m_aBeforeRed[g_state.nStepOnAction].GetDelay());
				}
				else if (nRes == SCR_INVALIDSKILL) // invalid skill
				{
					g_state.nStepOnSkill += 65535;
				}
				else // out of ammo
				{
					if (g_nAmmoRetry < _MAX_AMMORETRY)
					{
						g_nAmmoRetry++;
						g_ticker.ResetTicker(750);
					}
					else
					{
						FinishRun(FR_FAILED, FALSE);
					}	
				}
			}
			else
			{
				g_state.nStepOnSkill = 0;
				g_state.nStepOnAction++;
			}
		}
	
		break;

	case MS_TALK_PORTAL:
		ShowOverheadMsg("Entering red portal...");
		SIGPostString("ÕýÔÚ½øÈëºìÉ«´«ËÍÃÅ");
		// code here to talk to portal
		TakePortal(g_redPortal.GetObjectID());
		g_ticker.ResetTicker(1500);
		g_nCurState = MS_CHECK_PORTAL;
		break;

	case MS_CHECK_PORTAL:
		if (IsInRedPortal())
		{
			// entered red successfully
			ShowOverheadMsg("Entered red portal successfully.");
			SIGPostString("ÒÑ³É¹¦½øÈëºìÉ«´«ËÍÃÅ");
			g_bInTown = FALSE;

			if (g_nUpdateRetry > 5 
				|| !CheckPosition(g_redPortal.GetPositionX(), g_redPortal.GetPositionX(), 10))
			{
				g_nCurState = MS_CAST_AFTERRED;
			}
			else
			{
				ShowOverheadMsg("Waiting for location update.");
				SIGPostString("ÕýÔÚ½ÓÊÕÐÂµØÍ¼×ø±êÏµÍ³");
				g_nUpdateRetry++;
				//g_ticker.ResetTicker(500);
			}
		}
		else
		{
			if (g_nPortalRetry >= 5)
			{
				// totally failed, quit
				server->GamePrintError("Failed to enter red portal.");
				SIGPostString("ÎÞ·¨½øÈëºìÉ«´«ËÍÃÅ");
				FinishRun(FR_FAILED, FALSE);
			}
			else
			{
				// try more
				g_nPortalRetry++;
				g_nCurState = MS_TALK_PORTAL;
				g_ticker.ResetTicker(250);
			}
		}
				
		break;

	case MS_CAST_AFTERRED:

		// actions after entering red portal
		if (g_state.nStepOnAction >= g_gameinfo.m_scheme.m_aAfterRed.GetSize())
		{
			// all actions have been performed, now ready to approach pindleskin
			g_state.nStepOnAction = 0;
			g_state.nStepOnTeleport = 0;
			TeleFail = 0;
			g_nCurState = MS_APPROACH_PINDLESKIN;
		}
		else
		{
			// perform all actions in array
			if (g_state.nStepOnSkill < g_gameinfo.m_scheme.m_aAfterRed[g_state.nStepOnAction].GetRepeat())
			{
				int nRes = PerformAction(g_gameinfo.m_scheme.m_aAfterRed[g_state.nStepOnAction]);

				if (nRes == SCR_SUCCEEDED)
				{
					g_ticker.ResetTicker(HARDDELAY + g_gameinfo.m_scheme.m_aAfterRed[g_state.nStepOnAction].GetDelay());
					g_state.nStepOnSkill++;
				}
				else if (nRes == SCR_INVALIDSKILL)
				{
					g_state.nStepOnSkill += 65535;
				}
				else
				{
					if (g_nAmmoRetry < _MAX_AMMORETRY)
					{
						g_nAmmoRetry++;
						g_ticker.ResetTicker(750);
					}
					else
					{
						FinishRun(FR_FAILED, FALSE);
					}
				}
			}
			else
			{
				g_state.nStepOnSkill = 0;
				g_state.nStepOnAction++;
			}
		}

		break;

	case MS_APPROACH_PINDLESKIN:
		ShowOverheadMsg("Approaching Pindleskin.");
		SIGPostString("ÕýÔÚ½Ó½üPindleskin");
		// code here to approach pindleskin
				
		g_bTeleportOK = FALSE;
		if (g_pTempleRoute[g_state.nStepOnTeleport].x == 0) // reached
		{
			g_nCurState = MS_CAST_ONPINDLESKIN;
		}
		else
		{
			if (!g_bUseTeleport)
			{
				// player does not have teleport skill
				if (TeleFail == 0)
				{
					MoveCharTo(g_pTempleRoute[g_state.nStepOnTeleport].x,
						g_pTempleRoute[g_state.nStepOnTeleport].y);
					g_ticker.ResetTicker(1000);
				}
				else 
				{
					TakeStep(g_pTempleRoute[g_state.nStepOnTeleport].x,
						g_pTempleRoute[g_state.nStepOnTeleport].y);
					g_ticker.ResetTicker(500);
				}
			}
			else
			{
				// player has teleport skill
				CastOnMap(g_pTempleRoute[g_state.nStepOnTeleport].x,
						g_pTempleRoute[g_state.nStepOnTeleport].y, D2S_TELEPORT, TRUE);
				
				if (TeleFail == 0)
					g_ticker.ResetTicker(1000);
				else
					g_ticker.ResetTicker(500);
			}
					
			g_nCurState = MS_CHECK_TELEPORT;
		}
		break;

	case MS_CHECK_TELEPORT:

		// check position to see if previous teleport succeeded
		if (//(g_bUseTeleport && g_bTeleportOK) ||
			//g_bPindleAwaken ||
			CheckPosition(g_pTempleRoute[g_state.nStepOnTeleport].x,
						g_pTempleRoute[g_state.nStepOnTeleport].y, 6))
		{
			// succeeded
			TeleFail = 0;
			g_state.nStepOnTeleport++;
			g_nCurState = MS_APPROACH_PINDLESKIN;
		}
		else
		{
			ShowOverheadMsg("Verifying step.");
			SIGPostString("ÕýÔÚ¼ì²é½ÇÉ«×ø±ê");
			// failed
			if (TeleFail < MAX_LOC_FAIL)
			{
				TeleFail++;
				g_nCurState = MS_APPROACH_PINDLESKIN;
			}
			else // max failure reached
			{
				server->GamePrintError("Failed to adjust location.");
				FinishRun(FR_FAILED, FALSE);
			}
		}
		break;

	case MS_CAST_ONPINDLESKIN:

		if (!g_Pindleskin.IsValidObject())
		{
			server->GamePrintError("Failed to reach Pindleskin.");
			SIGPostString("ÎÞ·¨µ½´ïPindleskinÎ»ÖÃ");
			FinishRun(FR_FAILED, FALSE);
			break;
		}

		// actions to kill pindleskin
		// check corpose-related action
		if (ProcessCorpose())
		{
			g_ticker.ResetTicker(HARDDELAY + g_gameinfo.m_scheme.m_aOnCorpose[0].GetDelay());
			break;
		}

		if (g_state.nStepOnAction >= g_gameinfo.m_scheme.m_aOnPindleskin.GetSize())
		{
			// all actions have been performed, and failed to kill pindleskin
			g_state.nStepOnAction = 0;
			FinishRun(FR_OUTSKILL, FALSE);
		}
		else
		{
			// perform all actions in array
			if (g_state.nStepOnSkill < g_gameinfo.m_scheme.m_aOnPindleskin[g_state.nStepOnAction].GetRepeat())
			{
				int nRes = PerformAction(g_gameinfo.m_scheme.m_aOnPindleskin[g_state.nStepOnAction]);

				if (nRes == SCR_SUCCEEDED)
				{
					g_ticker.ResetTicker(HARDDELAY + g_gameinfo.m_scheme.m_aOnPindleskin[g_state.nStepOnAction].GetDelay());
					g_state.nStepOnSkill++;
				}
				else if (nRes == SCR_INVALIDSKILL)
				{
					g_state.nStepOnSkill += 65535;
				}
				else
				{
					if (g_nAmmoRetry < _MAX_AMMORETRY)
					{
						g_nAmmoRetry++;
						g_ticker.ResetTicker(750);
					}
					else
					{
						FinishRun(FR_FAILED, FALSE);
					}
				}
			}
			else
			{
				g_state.nStepOnSkill = 0;
				g_state.nStepOnAction++;
			}
		}

		break;

	case MS_PINDLE_DEAD:

		// pindleskin is dead and his corpose appeared
		// for barbs, use Find item in a second, for others
		// enable pickit		
		if (IsSkillValid(D2S_FINDITEM))
		{
			// give a moment because pindleskin's corpose needes to finish
			// its death blast before being available to be looted
			ShowOverheadMsg("Waiting for Pindleskin's corpose to be available.");
			SIGPostString("µÈ´ýPindlesinÊ¬Ìå³öÏÖ");
			g_ticker.ResetTicker(1000);
			g_nCurState = MS_CAST_FINDITEM;
		}
		else
		{
			g_nCurState = MS_PICK_ITEMS;
		}

		break;

	case MS_CAST_FINDITEM:

		ShowOverheadMsg("Using Find item skill on Pindleskin's corpose.");
		SIGPostString("¶ÔPindleskinÊ¬ÌåÊ¹ÓÃFind Item");
		CastOnEntity(g_Pindleskin.GetObjectID(), D2S_FINDITEM, TRUE);
		g_ticker.ResetTicker(1500);
		g_nCurState = MS_PICK_ITEMS;
		break;

	case MS_PICK_ITEMS:
		ShowOverheadMsg("Picking up items...");
		SIGPostString("ÕýÔÚ¼ì²é»òÊ°È¡Õ½ÀûÆ·");
		g_ticker.ResetTicker(6000); 
		g_nCurState = MS_PICKITEMS_OK;
		break;

	case MS_PICKITEMS_OK:
		FinishRun(FR_SUCCEEDED, FALSE);

	default:
		break;

	} // switch (g_state.nState)

return 0;	
}

DWORD EXPORT OnGamePacketBeforeSent(BYTE* aPacket, DWORD aLen)
{	
	// weapon switch must not be performed during process
	if (aLen == 1 && aPacket[0] == 0x60)
	{
		if (!g_bStop)
		{
			server->GamePrintError("Weapon-switch prohibited while Pindlebot is running.");
			return 0;
		}
	}	

	// resurrect merc packet, verify before send, make sure merc is really dead, 
	// otherwise merc may disappear permanently.
	if (aLen == 5 && aPacket[0] == 0x62)
	{
		// if merc is not dead, block the packet
		if (g_Merc.IsAlive())
			return 0;
	}

	return aLen;
}

DWORD EXPORT OnGamePacketBeforeReceived(BYTE* aPacket, DWORD aLen)
{
	g_Merc.OnGamePacketBeforeReceived(aPacket, aLen);
	g_Pindleskin.OnGamePacketBeforeReceived(aPacket, aLen);
	g_Malah.OnGamePacketBeforeReceived(aPacket, aLen);
	g_Larzuk.OnGamePacketBeforeReceived(aPacket, aLen);
	g_Qual.OnGamePacketBeforeReceived(aPacket, aLen);
	g_redPortal.OnGamePacketBeforeReceived(aPacket, aLen);
	g_stash.OnGamePacketBeforeReceived(aPacket, aLen);
	g_act1Wp.OnGamePacketBeforeReceived(aPacket, aLen);
	g_pickit.OnGamePacketBeforeReceived(aPacket, aLen);

	// item sticking on cursor upon join game(1.10 confirmed)
	if (aPacket[0] == 0x9c && aPacket[1] == 0x12)
	{
		//PrintPacket(aPacket, aLen, 1);
		server->GamePrintInfo("Item stuck on cursor, dropping it.");
		BYTE a[5] = {0x17}; // 1.10 confirmed
		::memcpy(a + 1,  aPacket + 4, 4);
		server->GameSendPacketToServer(a, 5);
		return aLen;
	}

	// Skill list (1.10 confirmed)
	if (aPacket[0] == 0x94)
	{
		GetSkillList(aPacket, aLen);
		return aLen;
	}

	// ammo equipmented packets, to determine wether player has arrow or bolt equipped
	if (aLen == 29
		&& aPacket[0] == 0x9d
		&& (aPacket[1] == 0x06 || aPacket[1] == 0x17)
		&& aPacket[2] == 0x1d
		&& aPacket[3] == 0x10
		&& aPacket[9] == thisgame->player->PlayerID
		) // at the active switch position
	{
		char szItemCode[5] = "";
		GetItemCode(aPacket, aLen, szItemCode);
		if (stricmp(szItemCode, "aqv") == 0) // arrow
		{
			//server->GamePrintInfo("Arrow");
			if (g_gameinfo.GetAmmoType() == AMMO_ARROW)
			{
				g_nAmmoRetry = 0;
				g_wAmmoQuantity = 100;				
			}		
		}
		else if (stricmp(szItemCode, "cqv") == 0) // bolt
		{
			//server->GamePrintInfo("Bolt");
			if (g_gameinfo.GetAmmoType() == AMMO_BOLT)
			{
				g_nAmmoRetry = 0;
				g_wAmmoQuantity = 100;				
			}		
		}	

		return aLen;
	}

	// arrow/bolt/throwing weapon quantity update
	if (aLen == 11
		&& aPacket[0] == 0x3e)
	{
		// arrow/bolt remaining amount
		::memcpy(&g_wAmmoQuantity, aPacket + 7, 2);
		if (g_wAmmoQuantity > 0)
			g_nAmmoRetry = 0;
		return aLen;
	}
	
	// teleport successfully (1.10 confirmed)
	if (aPacket[0] == 0x15 && aPacket[1] == 0x01) 
	{
		DWORD dwID;
		::memcpy(&dwID, aPacket + 2, 4);
		if (dwID == thisgame->player->PlayerID)
		{
			WORD x, y;
			::memcpy(&x, aPacket + 6, 2);
			::memcpy(&y, aPacket + 8, 2);

			if (g_bUseTeleport
				&& x == g_pTempleRoute[g_state.nStepOnTeleport].x
				&& y ==	g_pTempleRoute[g_state.nStepOnTeleport].y)
			{
				//server->GamePrintInfo("teleport OK");
				g_bTeleportOK = TRUE;
			}
		}

		return aLen;
	}
	
	// NPC interaction successfully(1.10 confirmed)
	if (aLen == 40
		&& aPacket[0] == 0x27
		&& aPacket[1] == 0x01)
	{
		DWORD dwID;
		::memcpy(&dwID, aPacket + 2, 4);

		// we talked to Larzuk 
		if (dwID == g_Larzuk.GetObjectID())
		{
			g_bLarzukOK = TRUE;
		}
		else if (dwID == g_Malah.GetObjectID())
		{
			g_bMalahOK = TRUE;
		}
		else
		{
		}
		return aLen;
	}


	// character is killed packet
	if ( aLen >= 6
		&& aPacket[0] == 0x8e
		&& aPacket[1] == 0x01)
	{
		DWORD dwID;
		::memcpy(&dwID, aPacket + 2, 4); // who died

		if (dwID == thisgame->player->PlayerID)
		{
			g_bCharDead = TRUE;
			if (g_bStarted && !g_bInTown && !g_bCharDead)
			{	
				FinishRun(FR_CHARDEAD, FALSE);
			}
		}
		return aLen;
	}

	// Update HP/MP
	if (aPacket[0] == 0x95)
	{
		g_nCurHP = *((WORD *)(aPacket + 1)) & 0x0FFF;
		g_nCurMP = (*((WORD *)(aPacket + 3)) & 0x0FFF) << 1;


		int nWhatLow = g_gameinfo.NeedRetreat(g_nCurHP, g_nCurMP);
		if (g_nCurHP < 1)
		{
			g_bCharDead = TRUE;
			if (g_bStarted && !g_bInTown && !g_bCharDead)
			{
				FinishRun(FR_CHARDEAD, FALSE);
			}
		}
		if (nWhatLow == 1) // HP low
		{
			if (!g_bInTown)
			{
				char msg[65] = "";
				sprintf(msg, "Player is in ÿc1danger ÿc0! HP<ÿc2%d%%.", g_gameinfo.m_nAlertHPPercent);
				server->GamePrintInfo(msg);
				FinishRun(FR_INDANGER, TRUE);
			}
		}
		else if (nWhatLow == 2) // MP low
		{
			if (!g_bInTown)
			{
				char msg[65] = "";
				sprintf(msg, "Player is in ÿc1danger ÿc0! Mana<ÿc2%d%%", g_gameinfo.m_nAlertMPPercent);
				server->GamePrintInfo(msg);
				FinishRun(FR_INDANGER, TRUE);
			}
		}
		else
		{
			// HP/MP not reach retreating level, now see if we need to drink a potion
			if (g_gameinfo.NeedPotionHeal(g_nCurHP, g_nCurMP))
			{
				if (g_pickit.GetBelt().HealMe(FALSE))				{
					server->GamePrintInfo("ÿc2Drank a potion.");
					g_logIni.IncreaseInt("Log", "Potion Drunk");
				}
			}
		}
		return aLen;
	}	

	// corpose picked up packet 8e0001000000********
	if ( aLen >= 6
		&& aPacket[0] == 0x8e
		&& aPacket[1] == 0x00
		&& aPacket[2] == 0x01 
		&& aPacket[3] == 0x00
		&& aPacket[4] == 0x00
		&& aPacket[5] == 0x00)
	{
		DWORD dwID;
		::memcpy(&dwID, aPacket + 2, 4);
		if (dwID == thisgame->player->PlayerID)
		{
			g_bCharDead = FALSE;
			g_bCharDead = FALSE;
			server->GamePrintInfo("Corpose picked up ÿc2OK.");
		}
		return aLen;
	}
	
	// arrow/bolt info from NPC
	if (aLen == 24
		&& aPacket[0] == 0x9c // header
		&& aPacket[1] == 0x0b // in store
		&& aPacket[2] == 0x18 // consuming weapon
		&& aPacket[3] == 0x10
		&& aPacket[23] == 0x3f) // arrow/bolt
	{
		//if (aPacket[21] == 0xe0 && aPacket[22] == 0xf5) // arrow
		if (aPacket[15] == 0x18) // arrow
		{
			::memcpy(&g_dwArrowID, aPacket + 4, 4);
		}
		else if (aPacket[15] == 0x38) // bolt
		{
			::memcpy(&g_dwBoltID, aPacket + 4, 4);	
		}
		return aLen;		
	}


	// monster corpose appears packet
	if (g_nCurState == MS_CAST_ONPINDLESKIN
		&& aLen == 12
		&& aPacket[0] == 0x69
		&& aPacket[5] == 0x09)
	{
		// monster corpose appeared, can be used for corpose related spells, such
		// as Revive, Corpose explosion, etc.
		
		// Obtain the corpose ID and add to array
		DWORD dwCorposeID;
		::memcpy(&dwCorposeID, aPacket + 1, 4);
		if (dwCorposeID != g_Pindleskin.GetObjectID() && dwCorposeID != g_Merc.GetObjectID())
		{
			int n = g_aCorposes.Find(dwCorposeID);
			if (n == -1)
				g_aCorposes.Add(dwCorposeID);
		}
		return aLen;
	}

	//////////////////////////////////////////////////////////////////////
	// Unit Revived by Itself (Act5 Zombies) 
	//////////////////////////////////////////////////////////////////////
	if (aLen == 17
		&& aPacket[0] == 0x4d)
	{
		DWORD dwID = 0;
		::memcpy(&dwID, aPacket + 2, 4);
		int n = g_aCorposes.Find(dwID);
		if (n >= 0)
			g_aCorposes.RemoveAt(n);
		return aLen;
	}

    return aLen;
}

void GetPercent(int n, int nTotal, LPTSTR lpBuffer)
{
	lpBuffer[0] = '\0';
	if (nTotal == 0)
		return;

	float f = (float)n / (float)nTotal * 100.0f;
	if (f > 100.0)
		f = 100.0;
	
	sprintf(lpBuffer, "(%.1f%%)", f);
}

BOOL PRIVATE OnGameCommandStatistics(char** argv, int argc)
{
	char szLine[128] = "";
	char szPerc[32] = "";
	int nData = 0;

	server->GamePrintInfo("ÿc7Pindlebot Statistics:");
	server->GamePrintInfo("ÿc7-------------------------------");

	int nTotalGames = g_logIni.GetInt("Log","Game", 0) - 1;
	DWORD dwRunTime = g_logIni.GetInt("Log", "Run Time", 0);

	sprintf(szLine, "ÿc7Completed Games:ÿc0 %d", nTotalGames);
	server->GamePrintInfo(szLine);

	sprintf(szLine, "ÿc7In Game Time:ÿc0 %d Hours %d Minutes %d Seconds", dwRunTime / 3600, (dwRunTime % 3600) / 60, (dwRunTime % 3600) % 60);
	server->GamePrintInfo(szLine);

	float f = 0.0;

	if (nTotalGames > 0)
		f = (float)dwRunTime / (float)nTotalGames;
	
	sprintf(szLine, "ÿc7Average Speed:ÿc0 %u Seconds/Game", (long)f);
	server->GamePrintInfo(szLine);

	nData = g_logIni.GetInt("Log", "Succeed", 0);
	GetPercent(nData, nTotalGames, szPerc);
	sprintf(szLine, "ÿc7Succeeded:ÿc0 %4d %s", nData, szPerc);
	server->GamePrintInfo(szLine);

	nData = g_logIni.GetInt("Log", "Merc Die", 0);
	GetPercent(nData, nTotalGames, szPerc);
	sprintf(szLine, "ÿc7Merc Died:ÿc0 %4d %s", nData, szPerc);
	server->GamePrintInfo(szLine);

	nData = g_logIni.GetInt("Log", "Char Die", 0);
	GetPercent(nData, nTotalGames, szPerc);
	sprintf(szLine, "ÿc7Char Died:ÿc0 %4d %s", nData, szPerc);
	server->GamePrintInfo(szLine);

	nData = g_logIni.GetInt("Log", "Retreat", 0);
	GetPercent(nData, nTotalGames, szPerc);
	sprintf(szLine, "ÿc7Retreated:ÿc0 %4d %s", nData, szPerc);
	server->GamePrintInfo(szLine);

	nData = g_logIni.GetInt("Log", "Avoid", 0);
	GetPercent(nData, nTotalGames, szPerc);
	sprintf(szLine, "ÿc7Pre-Avoided:ÿc0 %4d %s", nData, szPerc);
	server->GamePrintInfo(szLine);
	
	nData = g_logIni.GetInt("Log", "Ins Skill", 0);
	GetPercent(nData, nTotalGames, szPerc);
	sprintf(szLine, "ÿc7Ins. Skill:ÿc0 %4d %s", nData, szPerc);
	server->GamePrintInfo(szLine);

	nData = g_logIni.GetInt("Log", "Potion Drunk", 0);
	sprintf(szLine, "ÿc7Potion Drunk:ÿc0 %d", nData);
	server->GamePrintInfo(szLine);

	nData = g_logIni.GetInt("Log", "Potion Merc", 0);
	sprintf(szLine, "ÿc7Merc Potion Fed:ÿc0 %d", nData);
	server->GamePrintInfo(szLine);

	server->GamePrintInfo("ÿc7-------------------------------");
	
	return TRUE;
}

BOOL PRIVATE OnGameCommandPause(char** argv, int argc)
{
	if (g_bStop)
	{
		server->GamePrintInfo("Pindlebot is not ÿc2startedÿc0, cannot paused or resume it now.");
		return TRUE;
	}
	
	if (!g_bPaused)
	{
		g_bPaused = TRUE;
		server->GamePrintInfo("Pindlebot ÿc2paused.");
		//SIGPause();
	}
	else
	{
		g_bPaused = FALSE;
		server->GamePrintInfo("Pindlebot ÿc2resumed.");
		//SIGResume();
	}
	return TRUE;
}

BOOL PRIVATE OnGameCommandDebug(char** argv, int argc)
{
	char szMsg[64] = "";
	sprintf(szMsg, "%d skills loaded.", g_aSkills.GetSize());
	server->GamePrintInfo(szMsg);
	return TRUE;
}

// SIG functions
void SIGLeave()
{
	server->GameSaveAndExit();
}

void SIGPause()
{
	if (g_bUseSig)
		server->GameCommandLine("sig pause");
}

void SIGResume()
{
	if (g_bUseSig)
		server->GameCommandLine("sig resume");
}

void SIGStop()
{
	if (g_bUseSig)
		server->GameCommandLine("sig stop");
}

void SIGQuitD2()
{
	if (g_bUseSig)
		server->GameCommandLine("sig quitd2");
}

void SIGPostString(LPCTSTR lpsz)
{
	if (g_bUseSig && lpsz != NULL)
	{
		char sz[256] = "";
		sprintf(sz, "sig poststring %s", lpsz);
		server->GameCommandLine(sz);
	}
}
