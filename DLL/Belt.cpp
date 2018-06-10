// Belt.cpp: implementation of the CBelt class.
//
//////////////////////////////////////////////////////////////////////
//#include "StdAfx.h"
#include "Belt.h"
#include <tchar.h>
#include "item.h"
#include <stdio.h>

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CBelt::CBelt()
{
	InitAttributes();
}

CBelt::~CBelt()
{
}

BOOL CBelt::GetBeltItemName(const BELTITEM& item, LPTSTR lpBuffer, DWORD dwMaxLen)
{
	if (lpBuffer == NULL)
		return FALSE;

	lpBuffer[0] = _T('\0');

	switch (item.dwType)
	{
		// Misc Potions
	case POTION_STAMINA:
		_tcsncpy(lpBuffer, _T("Stamina Potion"), dwMaxLen);
		break;

	case POTION_ANTIDOTE:
		_tcsncpy(lpBuffer, _T("Antidote Potion"), dwMaxLen);
		break;

	case POTION_THAWING:
		_tcsncpy(lpBuffer, _T("Thawing Potion"), dwMaxLen);
		break;

	// Healing Potions
	case POTION_MINOR_HEALING:
		_tcsncpy(lpBuffer, _T("Minor Healing Potion"), dwMaxLen);
		break;

	case POTION_LIGHT_HEALING:
		_tcsncpy(lpBuffer, _T("Light Healing Potion"), dwMaxLen);
		break;

	case POTION_HEALING:
		_tcsncpy(lpBuffer, _T("Healing Potion"), dwMaxLen);
		break;

	case POTION_GREATER_HEALING:
		_tcsncpy(lpBuffer, _T("Greater Healing Potion"), dwMaxLen);
		break;

	case POTION_SUPER_HEALING:
		_tcsncpy(lpBuffer, _T("Super Healing Potion"), dwMaxLen);
		break;

	// Mana Potions
	case POTION_MINOR_MANA:
		_tcsncpy(lpBuffer, _T("Minor Mana Potion"), dwMaxLen);
		break;

	case POTION_LIGHT_MANA:
		_tcsncpy(lpBuffer, _T("Light Mana Potion"), dwMaxLen);
		break;

	case POTION_MANA:
		_tcsncpy(lpBuffer, _T("Mana Potion"), dwMaxLen);
		break;

	case POTION_GREATER_MANA:
		_tcsncpy(lpBuffer, _T("Greater Mana Potion"), dwMaxLen);
		break;

	case POTION_SUPER_MANA:
		_tcsncpy(lpBuffer, _T("Super Mana Potion"), dwMaxLen);
		break;

	// Rejuvenate Potions
	case POTION_REJUVENATE:
		_tcsncpy(lpBuffer, _T("Rejuvenate Potion"), dwMaxLen);
		break;

	case POTION_FULL_REJUVENATE:
		_tcsncpy(lpBuffer, _T("Full Rejuvenate Potion"), dwMaxLen);
		break;

	// Scrolls
	case SCROLL_IDENTIFY:
		_tcsncpy(lpBuffer, _T("Scroll of Identify"), dwMaxLen);
		break;

	case SCROLL_TOWN_PORTAL:
		_tcsncpy(lpBuffer, _T("Scroll of Town Portal"), dwMaxLen);
		break;

	default:
		break;
	}

	return _tcslen(lpBuffer);
}

void CBelt::OnGamePacketBeforeReceived(const BYTE *aPacket, DWORD aLen)
{
	CD2Abstract::OnGamePacketBeforeReceived(aPacket, aLen);

	if (!aPacket || !aLen)
		return;

	// belt is worn
	if (aLen > 18
		&& aPacket[0] == 0x9d
		&& (aPacket[1] == 0x06 || aPacket[1] == 0x09)
		&& aPacket[18] == 0x04
		&& !::memcmp(&aPacket[9], &m_dwPlayerID, 4))
	{
		// belt type
		TCHAR sz[ITEM_CODE_LEN + 1] = _T("");
		if (GetItemCode(aPacket, aLen, sz))
			m_nBeltRows = GetBeltMaxRow(sz);
		
		return;
	}

	// belt is picked to cursor, that means belt is removed as of now
	if (aLen > 18
		&& aPacket[0] == 0x9d
		&& aPacket[1] == 0x08
		&& aPacket[18] == 0x10
		&& !::memcmp(&aPacket[9], &m_dwPlayerID, 4))
	{
		m_nBeltRows = 1;
		return;
	}

	if (aLen == 20
		&& aPacket[0] == 0x9c
		&& aPacket[2] == 0x14
		&& aPacket[3] == 0x10)
	{
		// items added/removed to/from belt
		
		int nRow = 0, nCol = 0;
		DWORD dwID = 0, dwType = 0;
		int n = int(aPacket[14]);
		
		// determine column & row
		nRow = (((n % 32) - (n % 8)) / 8) % 4;
		nCol = ((n % 8) / 2) % 4;
		m_nBeltRows = max(m_nBeltRows, nRow + 1);
	
		// determine item ID & type
		memcpy(&dwID, aPacket + 4, 4); // item ID
		memcpy(&dwType, aPacket + 15, 4); // item type
	
		if (aPacket[1] == 0x0e || aPacket[1] == 0x10)
		{
			// item dropped to belt
			m_items[nRow][nCol].dwID = dwID;
			m_items[nRow][nCol].dwType = dwType;
			return;
		}

		if (aPacket[1] == 0x0f)
		{
			// item removed from belt
			m_items[nRow][nCol].dwID = 0;
			m_items[nRow][nCol].dwType = 0;
			return;			
		}
		
		return;
	}

	if (aLen == 25
		&& aPacket[0] == 0x9d
		&& aPacket[1] == 0x15
		&& aPacket[2] == 0x19
		&& aPacket[3] == 0x10)
	{
		// items shift down
		int nRow = 0, nCol = 0;
		DWORD dwID = 0, dwType = 0;
		int n = int(aPacket[19]);

		// determine column & row
		nRow = (((n % 32) - (n % 8)) / 8) % 4;
		nCol = ((n % 8) / 2) % 4;
		m_nBeltRows = max(m_nBeltRows, nRow + 1);

		// determine item ID & type
		memcpy(&dwID, aPacket + 4, 4); // item ID
		memcpy(&dwType, aPacket + 20, 4); // item type
		m_items[nRow][nCol].dwID = dwID;
		m_items[nRow][nCol].dwType = dwType;

		// remove the upper item
		if (nRow < 3)
		{
			m_items[nRow + 1][nCol].dwID = 0;
			m_items[nRow + 1][nCol].dwType = 0;
		}
		return;
	}	
}

BOOL CBelt::IsBeltFull() const
{
	for (int row = 0; row < m_nBeltRows; row++)
	{
		for (int col = 0; col < 4; col++)
		{
			if (m_items[row][col].dwID == 0)
				return FALSE;
		}
	}

	return TRUE;
}

LPCBELTITEM CBelt::GetItem(int nRow, int nCol) const
{
	if (nRow < 0 || nRow >= m_nBeltRows || nCol < 0 || nCol > 3)
		return NULL;
	
	return &m_items[nRow][nCol];
}

int CBelt::GetBeltMaxRow(LPCTSTR lpItemCode)
{
	// invalid code, 1 row
	if (lpItemCode == NULL || _tcslen(lpItemCode) < 3)
		return 1;

	// 2 rows: sash, light belt
	else if (_tcsicmp(lpItemCode, _T("lbl")) == 0 || _tcsicmp(lpItemCode, _T("vbl")) == 0)
		return 2;

	// 3 rows: belt, heavy belt
	else if (_tcsicmp(lpItemCode, _T("mbl")) == 0 || _tcsicmp(lpItemCode, _T("tbl")) == 0)
		return 3;

	// the rest should have 4 rows
	else
		return 4;
}

int CBelt::GetBeltRows() const
{
	return m_nBeltRows;
}

void CBelt::OnGameJoin(DWORD dwPlayerID)
{
	CD2Abstract::OnGameJoin();

	InitAttributes();	
	m_dwPlayerID = dwPlayerID;
}

void CBelt::OnGameLeave()
{
	CD2Abstract::OnGameLeave();
	
	InitAttributes();
}

LPCBELTITEM CBelt::FindFirstItem(DWORD dwType) const
{
	for (int row = 0; row < m_nBeltRows; row++)
	{
		for (int col = 0; col < 4; col++)
		{
			if (m_items[row][col].dwID != 0 && m_items[row][col].dwType == dwType)
				return &m_items[row][col];
		}
	}

	return NULL;
}

int CBelt::GetAvailableCol(DWORD dwItemType) const
{
	if (m_nBeltRows < 1)
		return -1;

	int nCata = GetBeltItemCata(dwItemType);
	if (nCata == BELT_CATA_UNKNOWN)
		return -1;

	int col = 0;
	for (col = 0; col < 4; col++)
	{
		if (!IsColFull(col) && nCata == GetBeltItemCata(m_items[0][col].dwType))
			return col;
	}

	// now look for a col whose first slot is empty
	for (col = 0; col < 4; col++)
	{
		if (m_items[0][col].dwID == 0)
			return col;
	}

	return -1;
}

BOOL CBelt::IsColFull(int nCol) const
{
	if (nCol < 0 || nCol > 3)
		return TRUE;

	for (int row = 0; row < m_nBeltRows; row++)
	{
		if (m_items[row][nCol].dwID == 0)
			return FALSE;
	}
	return TRUE;
}

int CBelt::GetBeltItemCata(DWORD dwType)
{
	int nCata = BELT_CATA_UNKNOWN;
	switch (dwType)
	{
	case POTION_STAMINA:
	case POTION_ANTIDOTE:
	case POTION_THAWING:
		nCata = BELT_CATA_JUNKPOTION;
		break;

	case POTION_MINOR_HEALING:
	case POTION_LIGHT_HEALING:
	case POTION_HEALING:
	case POTION_GREATER_HEALING:
	case POTION_SUPER_HEALING:
		nCata = BELT_CATA_HEALING;
		break;

	case POTION_MINOR_MANA:
	case POTION_LIGHT_MANA:
	case POTION_MANA:
	case POTION_GREATER_MANA:
	case POTION_SUPER_MANA:
		nCata = BELT_CATA_MANA;
		break;
		
	case POTION_REJUVENATE:
	case POTION_FULL_REJUVENATE:
		nCata = BELT_CATA_REJUV;
		break;	

	default:
		nCata = BELT_CATA_UNKNOWN;
		break;
	}

	return nCata;
}

BOOL CBelt::HealMe(BOOL bPurpleOnly) const
{
	DWORD dwItemID = FindPurplePotion();
	if (dwItemID == 0 && !bPurpleOnly)
		dwItemID = FindRedPotion();

	return dwItemID ? DrinkPotion(dwItemID, FALSE) : 0;
}

BOOL CBelt::HealMerc(BOOL bPurpleOnly) const
{
	DWORD dwItemID = FindPurplePotion();
	if (dwItemID == 0 && !bPurpleOnly)
		dwItemID = FindRedPotion();

	return dwItemID ? DrinkPotion(dwItemID, TRUE) : 0;
}

BOOL CBelt::DrinkPotion(DWORD dwItemID, BOOL bFeedMerc) const
{
	if (dwItemID == 0)
		return FALSE;

	BYTE aPacket[13] = { 0x26 };
	memcpy(aPacket + 1, &dwItemID, 4);
	aPacket[5] = bFeedMerc ? 0x01 : 0x00;
	memset(aPacket + 6, 0x00, 7);
	return SendPacketToServer(aPacket, 13);	
}

int CBelt::DrinkAllDrunks(BOOL bIncludeRejuv) const
{
	int nCount = 0;
	for (int row = 0; row < m_nBeltRows; row++)
	{
		for (int col = 0; col < 4; col++)
		{
			if (m_items[row][col].dwID == 0
				|| m_items[row][col].dwType == POTION_FULL_REJUVENATE
				|| m_items[row][col].dwType == SCROLL_IDENTIFY
				|| m_items[row][col].dwType == SCROLL_TOWN_PORTAL)
				continue;

			if (!bIncludeRejuv && m_items[row][col].dwType == POTION_REJUVENATE)
				continue;

			// all others are considered junks
			if (DrinkPotion(m_items[row][col].dwID, FALSE))
				nCount++;
		}
	}
	return nCount;
}

DWORD CBelt::FindPurplePotion() const
{
	LPCBELTITEM p = FindFirstItem(POTION_FULL_REJUVENATE);
	if (!p)
		p = FindFirstItem(POTION_REJUVENATE);
	return p ? p->dwID : 0;
}

DWORD CBelt::FindRedPotion() const
{
	LPCBELTITEM p = FindFirstItem(POTION_SUPER_HEALING);

	if (!p)
		p = FindFirstItem(POTION_GREATER_HEALING);

	if (!p)
		p = FindFirstItem(POTION_HEALING);

	if (!p)
		p = FindFirstItem(POTION_LIGHT_HEALING);

	if (!p)
		p = FindFirstItem(POTION_MINOR_HEALING);

	return p ? p->dwID : 0;
}

DWORD CBelt::FindBluePotion() const
{
	LPCBELTITEM p = FindFirstItem(POTION_SUPER_MANA);

	if (!p)
		p = FindFirstItem(POTION_GREATER_MANA);

	if (!p)
		p = FindFirstItem(POTION_MANA);

	if (!p)
		p = FindFirstItem(POTION_LIGHT_MANA);

	if (!p)
		p = FindFirstItem(POTION_MINOR_MANA);

	return p ? p->dwID : 0;
}

BOOL CBelt::SupplyMana(BOOL bDrinkPurpleIfNoBlue) const
{
	DWORD dwItemID = FindBluePotion();
	if (dwItemID == 0 && bDrinkPurpleIfNoBlue)
		dwItemID = FindPurplePotion();

	return dwItemID ? DrinkPotion(dwItemID, FALSE) : 0;
}

BOOL CBelt::DrinkBeltPotion(int nRow, int nCol) const
{
	LPCBELTITEM lpItem = GetItem(nRow, nCol);
	if (lpItem == NULL || lpItem->dwID == 0 || lpItem->dwType == SCROLL_IDENTIFY || lpItem->dwType == SCROLL_TOWN_PORTAL)
		return FALSE;

	return DrinkPotion(lpItem->dwID, FALSE);
}

void CBelt::InitAttributes()
{
	CD2Abstract::InitAttributes();

	m_dwPlayerID = 0;
	::memset(m_items, 0, sizeof(BELTITEM) * 16);
	m_nBeltRows = 0;
}
