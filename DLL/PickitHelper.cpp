// PickitHelper.cpp: implementation of the CPickit class.
//
//////////////////////////////////////////////////////////////////////
//#include "StdAfx.h"
#include "PickitHelper.h"
#include "Ini.h"
#include <stdio.h>
#include <time.h>

#define INVENTORYFULL_RETRY		3

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CPickitHelper::CPickitHelper()
{
	m_pItemDropProc = NULL;
	m_pInventoryFullProc = NULL;
	m_lItemDropParam = (LPARAM)0;
	m_lInventoryFullParam = (LPARAM)0;
	m_nGoldMethod = PICKIT_PICK_AUTO;
	m_nRejuvMethod = PICKIT_PICK_NO;
	m_nFullRejuvMethod = PICKIT_PICK_AUTO;
	m_szConfigFile[0] = _T('\0');
	m_dwDelay = PICKIT_DELAY_DEFAULT;
	m_dwRadius = PICKIT_RADIUS_DEFAULT;

	InitAttributes();
	CleanConfig();
}

CPickitHelper::~CPickitHelper()
{
}

BOOL CPickitHelper::LoadConfig(LPCTSTR lpszFile)
{
	if (lpszFile == NULL || *lpszFile == 0)
		return FALSE; // invalid params

	DWORD dwRes = ::GetFileAttributes(lpszFile);
	if (dwRes == -1 || dwRes == FILE_ATTRIBUTE_DIRECTORY)
		return FALSE; // file not exists

	CleanConfig();
	CleanQueue();
	
	m_bBusy = TRUE;	
	m_curPos = NULL;
	m_nInventoryFullState = 0;

	_tcsncpy(m_szConfigFile, lpszFile, MAX_PATH);	
	CIni ini(lpszFile);

	// keey enlarging the buffer unless we make sure all section names
	// are loaded.
	DWORD dwBufSize = 1024;
	TCHAR *pszBuffer = new TCHAR[dwBufSize];
	DWORD dwCopied = ini.GetSectionNames(pszBuffer, dwBufSize - 1);
	while (dwCopied + 128 >= dwBufSize)
	{
		dwBufSize += 1024;
		delete [] pszBuffer;
		pszBuffer = new TCHAR[dwBufSize];
		dwCopied = ini.GetSectionNames(pszBuffer, dwBufSize - 1);
	}	
	
	// iterate through all sections
	ITEMCONFIG cfg = { 0 };
	for (TCHAR* pSection = pszBuffer; pSection[0] != _T('\0'); pSection = &pSection[_tcslen(pSection) + 1])
	{	
		// now query the section and fill in data

		::memset(&cfg, 0, sizeof(ITEMCONFIG));
		TCHAR sz[256] = _T("");
				
		// ItemCode should be 3 chars long
		// Code="xxx" or Code=xxx
		ini.GetString(pSection, _T("Code"), sz, ITEM_CODE_LEN + 3);
		if (sz[0] == _T('\"'))
			_tcsncpy(cfg.szItemCode, &sz[1], ITEM_CODE_LEN);
		else
			_tcsncpy(cfg.szItemCode, sz, ITEM_CODE_LEN);
		
		if (_tcslen(cfg.szItemCode) != ITEM_CODE_LEN)
			continue;

		// Item Description
		ini.GetString(pSection, _T("Description"), cfg.szDescription, ITEM_DESC_LEN);

		// replace "%c" to "ÿc" so users do not need to put that character in their ini files
		// they can use "%c" instead.
		for (int i = 0; cfg.szDescription[i] != '\0'; i++)
		{
			if (cfg.szDescription[i] == '%' && cfg.szDescription[i + 1] == 'c')
				cfg.szDescription[i] = 'ÿ';
		}

		// identified
		cfg.iIdentified = (BYTE)ini.GetInt(pSection, _T("Identified"), ITEM_UNSET);

		// ethereal
		cfg.iEthereal = (BYTE)ini.GetInt(pSection, _T("isEthereal"), ITEM_UNSET);

		// item should have sockets, doesnt matter how many unlesss NumSockets is used
		cfg.iSocketed = (BYTE)ini.GetInt(pSection, _T("HasSockets"), ITEM_UNSET);

		// dont pickup item.  useful if you only want to be told of the item drop
		cfg.iPickup = (BYTE)ini.GetInt(pSection, _T("Pickup"), 1);

		if (IsGold(cfg.szItemCode)
			|| IsRejuv(cfg.szItemCode)
			|| IsFullRejuv(cfg.szItemCode))
		{
			// these items are special, we do not load them by the normal way...
			continue;
		}

		if (!cfg.iPickup && _tcslen(cfg.szDescription) == 0)
			continue; // no pickup no announce

		// item level
		ini.GetString(pSection, _T("Level"), sz, 64);

		if(_tcsicmp(sz, _T("rare")) == 0)
			cfg.iLevel = ITEM_LEVEL_RARE;
		else if(_tcsicmp(sz, _T("unique")) == 0)
			cfg.iLevel = ITEM_LEVEL_UNIQUE;
		else if(_tcsicmp(sz, _T("set")) == 0)
			cfg.iLevel = ITEM_LEVEL_SET;
		else if(_tcsicmp(sz, _T("magic")) == 0)
			cfg.iLevel = ITEM_LEVEL_MAGIC;
		else if(_tcsicmp(sz, _T("craft")) == 0)
			cfg.iLevel = ITEM_LEVEL_CRAFT;
		else if(_tcsicmp(sz, _T("normal")) == 0)
			cfg.iLevel = ITEM_LEVEL_NORMAL;
		else if(_tcsicmp(sz, _T("superior")) == 0)
			cfg.iLevel = ITEM_LEVEL_SUPERIOR;
		else if(_tcsicmp(sz, _T("lowquality")) == 0)
			cfg.iLevel = ITEM_LEVEL_INFERIOR;
		else
			cfg.iLevel = ITEM_UNSET;
	
		// item types
		ini.GetString(pSection, _T("Type"), sz, 64);
		if(_tcsicmp(sz, _T("Helm")) == 0)
			cfg.iType = ITEM_ITEMTYPE_HELM;
		else if(_tcsicmp(sz, _T("Armor")) == 0)
			cfg.iType = ITEM_ITEMTYPE_ARMOR;
		else if(_tcsicmp(sz, _T("Weapon")) == 0)
			cfg.iType = ITEM_ITEMTYPE_WEAPON;
		else if(_tcsicmp(sz, _T("Bow")) == 0)
			cfg.iType = ITEM_ITEMTYPE_BOW;
		else if(_tcsicmp(sz, _T("Shield")) == 0)
			cfg.iType = ITEM_ITEMTYPE_SHIELD;
		else if(_tcsicmp(sz, _T("Other")) == 0)
			cfg.iType = ITEM_ITEMTYPE_OTHER;
		else 
			cfg.iType = ITEM_UNSET;

		// insert this entry to the config queue
		m_aConfig.InsertRear(cfg); // add this entry
	}
	
	// the large buffer is no longer needed
	delete [] pszBuffer;
	pszBuffer = NULL;	
	m_bBusy = FALSE;

	return TRUE;
}

void CPickitHelper::OnGamePacketBeforeReceived(const BYTE *aPacket, DWORD aLen)
{
	CD2Abstract::OnGamePacketBeforeReceived(aPacket, aLen);

	m_belt.OnGamePacketBeforeReceived(aPacket, aLen);
	m_gold.OnGamePacketBeforeReceived(aPacket, aLen);

	if (!aPacket || !aLen)
		return;

	// inventory full packet
	if (aLen == 8
		&& aPacket[0] == 0x2c
		&& aPacket[1] == 0x00
		&& !::memcmp(aPacket + 2, &m_dwPlayerID, 4)
		&& aPacket[6] == 0x17
		&& aPacket[7] == 0x00)
	{
		m_nInventoryFullState++; // we shall retry a few more times	
		if (m_nInventoryFullState >= INVENTORYFULL_RETRY)
		{
			m_bPaused = TRUE;
			// disable item pick immediately if inventory is full
			if (m_nInventoryFullState == INVENTORYFULL_RETRY && m_pInventoryFullProc != NULL)
				m_pInventoryFullProc(m_lInventoryFullParam); // notify the client
		}

		return;
	}

	// item was picked up or is gone
	if(aPacket[0] == 0x0a && aPacket[1] == 0x04)
	{	
		// remove it from our item queue if it was there
		m_bBusy = TRUE;
		DWORD dwID = 0;
		::memcpy(&dwID, aPacket + 2, 4);
		RemoveFromQueue(dwID);
		m_bBusy = FALSE;
		return;
	}	

	// is a new item dropped?
	ITEM item = { 0 };
	if (ParseItemDrop(aPacket, aLen, item))
	{
		const ITEMCONFIG* pCfg = FindItem(item);
		if (pCfg == NULL)
			return;	
		
		// the user wanted this item, so add it into our item queue
		m_aItemQueue.InsertRear(item);
		if (m_pItemDropProc != NULL && pCfg->szDescription[0] != _T('\0'))
			m_pItemDropProc(item, pCfg->szDescription, m_lItemDropParam); // notify the client

		return;
	}
}

void CPickitHelper::CleanQueue()
{
	m_bBusy = TRUE;
	m_curPos = NULL;
	m_aItemQueue.RemoveAll();
	m_bBusy = FALSE;
}

void CPickitHelper::OnGameTimerTick()
{
	CD2Abstract::OnGameTimerTick();
	
	static DWORD dwTick = 0; // For far items
	dwTick++;
	dwTick %= 5;

	if (m_bBusy || m_dwRadius == 0 || m_bPaused)
		return;	

	// only send packet if delay is elapsed
	DWORD dwNow = ::GetTickCount();
	if (m_dwLastProc + m_dwDelay > dwNow)
		return; // Delay not elapsed yet, must wait

	int nCount = 0;
	POSITION pos = m_aItemQueue.GetHeadPosition();
	while (pos)
	{
		const ITEM& item = m_aItemQueue.GetNext(pos);		
		if (IsNeedPickup(item, TRUE) && SendPickPacket(item, dwTick))
			nCount++;
	}

	if (nCount)
		m_dwLastProc = dwNow;

	return;
}

DWORD CPickitHelper::SetRadius(DWORD dwRadius)
{
	m_dwRadius = min(PICKIT_RADIUS_MAX, dwRadius);
	return m_dwRadius;
}

DWORD CPickitHelper::GetRadius() const
{
	return m_dwRadius;
}

BOOL CPickitHelper::SendPickPacket(const ITEM& item, DWORD dwTicker)
{
	if (m_bBusy
		||item.dwItemID == 0 
		|| m_pPlayerX == NULL
		|| m_pPlayerY == NULL
		|| *m_pPlayerX == 0
		|| *m_pPlayerY == 0
		|| m_bBusy
		|| m_dwRadius == 0
		|| m_bPaused)
	{
		return FALSE;
	}

	const DWORD DistanceSquared = (*m_pPlayerX - item.wPositionX) * (*m_pPlayerX - item.wPositionX)
				+ (*m_pPlayerY - item.wPositionY) * (*m_pPlayerY - item.wPositionY);
	const DWORD dwCfgSquared = m_dwRadius * m_dwRadius;

	// if item is less then 1/2 pickupradius away send pickup request everytime
	// if item is greater then 1/2 pickupradius away send pickup reqest every 5 function calls
	// ignore items outside pickup radius
	if (DistanceSquared <= dwCfgSquared)
	{
		if (DistanceSquared <= dwCfgSquared / 4 || (dwTicker % 5) == 0)
		{		
			// prepare and send the packet, originally Ackmed's code
			BYTE PickupPacket[13];
			PickupPacket[0] = 0x16;
			PickupPacket[1] = 0x04;
			PickupPacket[2] = PickupPacket[3] = PickupPacket[4] = 0;			
			memcpy(&PickupPacket[5], &(item.dwItemID), 4);
			PickupPacket[9] = PickupPacket[10] = PickupPacket[11] = PickupPacket[12] = 0;
			return SendPacketToServer(PickupPacket, 13);
		}
	}	

	return FALSE;
}

void CPickitHelper::OnGameJoin(DWORD dwPlayerID, const WORD *pPlayerXAddr, const WORD *pPlayerYAddr)
{
	// we will be reading player location from these two memory address
	CD2Abstract::OnGameJoin();

	InitAttributes();
	m_pPlayerX = pPlayerXAddr;
	m_pPlayerY = pPlayerYAddr;
	m_dwPlayerID = dwPlayerID;	
	m_belt.OnGameJoin(dwPlayerID);
	m_gold.OnGameJoin(dwPlayerID);
}

void CPickitHelper::OnGameLeave()
{	
	CD2Abstract::OnGameLeave();
	
	InitAttributes();
	m_belt.OnGameLeave();
	m_gold.OnGameLeave();
}

void CPickitHelper::SetItemDropProc(ItemDropProc pFunc, LPARAM lParam)
{
	m_pItemDropProc = pFunc;
	m_lItemDropParam = lParam;
}

void CPickitHelper::SetInventoryFullProc(InventoryFullProc pFunc, LPARAM lParam)
{
	m_pInventoryFullProc = pFunc;
	m_lInventoryFullParam = lParam;
}

const ITEMCONFIG* CPickitHelper::FindItem(const ITEM &item) const
{
	POSITION pos = m_aConfig.GetHeadPosition();

	while (pos)
	{
		const ITEMCONFIG& cfg = m_aConfig.GetNext(pos);
		if (IsItemMatchConfig(cfg, item))
			return &cfg;
	}

	return NULL;
}

void CPickitHelper::Enable(BOOL bEnable)
{
	m_bPaused = !bEnable;
	if (bEnable)
		m_nInventoryFullState = 0; // Player may have rearranged his inventory, so reset inventory state
}

BOOL CPickitHelper::IsEnabled() const
{
	return !m_bPaused;
}	

BOOL CPickitHelper::Toggle()
{
	Enable(!IsEnabled());
	return IsEnabled();
}

BOOL CPickitHelper::WriteLog(LPCTSTR lpszFilePath, LPCTSTR lpszString, BOOL bAppend)
{
	if (lpszFilePath == NULL || lpszString == NULL || lpszString[0] == 0)
		return FALSE;	

	FILE *stream = _tfopen(lpszFilePath, bAppend ? _T("a+") : _T("w"));
	if (stream == NULL)
		return FALSE;

	// get time stamp
	TCHAR szTimeString[33] = _T("");
	struct tm *when;
    time_t now;
    ::time(&now);
    when = ::localtime(&now);
	_tcsftime(szTimeString, 32, _T("%m/%d %H:%M:%S"), when);
	if (szTimeString[0] == _T('0'))
		szTimeString[0] = _T(' ');

	const int LEN = _tcslen(szTimeString) + _tcslen(lpszString) + 10;
	TCHAR* pszString = new TCHAR[LEN];
	_stprintf(pszString, _T("%s | %s\n"), szTimeString, lpszString);

	// replace the special color char to '%' so the log file can be read by
	// MFC CStdioFile
	for (int i = 0; pszString[i] != _T('\0'); i++)
	{
		if (pszString[i] == _T('ÿ'))
			pszString[i] = _T('%');
	}

	// write to file
	_ftprintf(stream, _T("%s"), pszString);

	delete [] pszString;
	::fclose(stream);
	return TRUE;
}

LPCTSTR CPickitHelper::GetConfigFile() const
{
	return m_szConfigFile;
}

int CPickitHelper::GetQueueItemCount(BOOL bValuableOnly) const
{
	if (!bValuableOnly)
		return m_aItemQueue.GetItemCount();

	int nCount = 0;
	POSITION pos = m_aItemQueue.GetHeadPosition();
	while (pos)
	{
		const ITEM& item = m_aItemQueue.GetNext(pos);
		if (!IsGold(item.szItemCode)
			&& !IsFullRejuv(item.szItemCode)
			&& !IsRejuv(item.szItemCode))
		{
			nCount++;
		}
	}

	return nCount;
}

int CPickitHelper::GetConfigCount() const
{
	int nCount = m_aConfig.GetItemCount() - 3; // minus 3 special items
	return max(0, nCount);
}

BOOL CPickitHelper::IsNeedPickup(const ITEM& item, BOOL bCfgPickup) const
{
	// For special items check for m_bAllowxxx
	if (IsGold(item.szItemCode))
		return m_nGoldMethod == PICKIT_PICK_YES || (m_nGoldMethod == PICKIT_PICK_AUTO && item.dwGoldAmount <= m_gold.GetInventoryGoldSpace());

	if (IsRejuv(item.szItemCode))
		return m_nRejuvMethod == PICKIT_PICK_YES || (m_nRejuvMethod == PICKIT_PICK_AUTO && m_belt.GetAvailableCol(POTION_REJUVENATE) >= 0);

	if (IsFullRejuv(item.szItemCode))
		return m_nFullRejuvMethod == PICKIT_PICK_YES || (m_nFullRejuvMethod == PICKIT_PICK_AUTO && m_belt.GetAvailableCol(POTION_FULL_REJUVENATE) >= 0);

	return bCfgPickup;
}

int CPickitHelper::RemoveFromQueue(DWORD dwItemID)
{
	int nCount = 0;
	POSITION pos = m_aItemQueue.GetHeadPosition();
	while (pos)
	{
		POSITION temp = pos;
		const ITEM& item = m_aItemQueue.GetNext(pos);
		if (item.dwItemID == dwItemID)
		{
			nCount++;
			m_aItemQueue.RemoveAt(temp);
			if (m_curPos == temp)
				m_curPos = pos; // Update active-item pointer
		}
	}
	return nCount;
}

DWORD CPickitHelper::SetDelay(DWORD dwDelay)
{
	// must not be lower than 100 ms, otherwise connection might be interrupted
	m_dwDelay = max(PICKIT_DELAY_MIN, dwDelay);
	return m_dwDelay;
}

DWORD CPickitHelper::GetDelay() const
{
	return m_dwDelay;
}

BOOL CPickitHelper::SetSpecialItemMethod(int nSpecialItem, int nMethod)
{
	if (nMethod < PICKIT_PICK_NO || nMethod > PICKIT_PICK_AUTO)
		return FALSE;

	switch (nSpecialItem)
	{
	case PICKIT_SPECIAL_GOLD:
		m_nGoldMethod = nMethod;
		break;

	case PICKIT_SPECIAL_REJUV:
		m_nRejuvMethod = nMethod;
		break;

	case PICKIT_SPECIAL_FULLREJUV:
		m_nFullRejuvMethod = nMethod;
		break;

	default:
		return FALSE;
	}

	return TRUE;
}

int CPickitHelper::GetSpecialItemMethod(int nSpecialItem) const
{
	int nMethod = PICKIT_PICK_UNKNWON;
	switch (nSpecialItem)
	{
	case PICKIT_SPECIAL_GOLD:
		return m_nGoldMethod;
		break;

	case PICKIT_SPECIAL_REJUV:
		return m_nRejuvMethod;
		break;

	case PICKIT_SPECIAL_FULLREJUV:
		return m_nFullRejuvMethod;
		break;

	default:
		nMethod = PICKIT_PICK_UNKNWON;
		break;
	}

	return nMethod;
}

const CBelt& CPickitHelper::GetBelt() const
{
	return m_belt;
}

const CGoldManager& CPickitHelper::GetGoldManager() const
{
	return m_gold;
}

void CPickitHelper::CleanConfig()
{
	m_bBusy = TRUE;
	m_aConfig.RemoveAll();

	// Always add special items
	ITEMCONFIG cfg = { 0 };
	cfg.iEthereal = ITEM_UNSET;
	cfg.iIdentified = ITEM_UNSET;
	cfg.iLevel = ITEM_UNSET;
	cfg.iPickup = 1;
	cfg.iSocketed = ITEM_UNSET;
	cfg.iType = ITEM_UNSET;	

	_tcsncpy(cfg.szItemCode, ITEM_GOLD, ITEM_CODE_LEN);
	m_aConfig.InsertRear(cfg); // Gold

	_tcsncpy(cfg.szItemCode, ITEM_FULL_REJUV, ITEM_CODE_LEN);
	m_aConfig.InsertRear(cfg); // Full rejuv

	_tcsncpy(cfg.szItemCode, ITEM_REJUV, ITEM_CODE_LEN);
	m_aConfig.InsertRear(cfg); // Rejuv
	
	m_bBusy = FALSE;
}

void CPickitHelper::InitAttributes()
{
	m_pPlayerX = NULL;
	m_pPlayerY = NULL;
	m_dwPlayerID = 0;
	m_nInventoryFullState = 0;
	m_bPaused = FALSE;
	m_curPos = NULL;
	m_bBusy = FALSE;
	m_dwLastProc = 0;
	CleanQueue();
}
