// GoldManager.cpp: implementation of the CGoldManager class.
//
//////////////////////////////////////////////////////////////////////
//#include "StdAfx.h"
#include "GoldManager.h"

// Stuff for picking gold
#define MAXGOLD(char_level)((char_level) * (DWORD)10000)
#define GOLDSPACE(char_level, inventory_gold)(MAXGOLD(char_level) > (inventory_gold) ? MAXGOLD(char_level) - (inventory_gold) : 0)
#define GUESSLEVEL(inventory_gold)(((inventory_gold) % 10000) ? (inventory_gold) / 10000 + 1 : (inventory_gold) / 10000)

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CGoldManager::CGoldManager()
{
	InitAttributes();
}

CGoldManager::~CGoldManager()
{

}

void CGoldManager::OnGameJoin(DWORD dwPlayerID)
{
	InitAttributes();
	m_dwPlayerID = dwPlayerID;
}

void CGoldManager::OnGameLeave()
{
	InitAttributes();
}

void CGoldManager::OnGamePacketBeforeReceived(const BYTE *aPacket, DWORD aLen)
{
	if (!aPacket || !aLen)
		return;

	// inventory gold updates
	if (aPacket[0] == 0x19 && aLen == 2)
	{
		// less than 255 gold update
		m_dwInventoryGold += aPacket[1];
		return;
	}

	if (aPacket[0] == 0x1d && aPacket[1] == 0x0e && aLen >= 3)
	{
		// inventory gold BYTE update
		BYTE gTmp = 0;
		::memcpy(&gTmp, &aPacket[2], sizeof(BYTE));
		m_dwInventoryGold = (DWORD)gTmp;
		m_iCharLevel = max(m_iCharLevel, (BYTE)GUESSLEVEL(m_dwInventoryGold));
		return;
	}

	if (aPacket[0] == 0x1e && aPacket[1] == 0x0e && aLen >= 4)
	{
		// inventory gold WORD update
		WORD gTmp = 0;
		memcpy(&gTmp, &aPacket[2], sizeof(WORD));
		m_dwInventoryGold = (DWORD)gTmp;
		m_iCharLevel = max(m_iCharLevel, (BYTE)GUESSLEVEL(m_dwInventoryGold));
		return;
	}

	if (aPacket[0] == 0x1f && aPacket[1] == 0x0e && aLen >= 6)
	{
		// inventory gold DWORD update
		DWORD gTmp = 0;
		memcpy(&gTmp, &aPacket[2], sizeof(DWORD));
		m_dwInventoryGold = (DWORD)gTmp;
		m_iCharLevel = max(m_iCharLevel, (BYTE)GUESSLEVEL(m_dwInventoryGold));
		return;
	}

	// stash gold update
	if (aPacket[0] == 0x1d && aPacket[1] == 0x0f)
	{	
		// stash gold BYTE update
		BYTE gTmp;
		memcpy(&gTmp, &aPacket[2], sizeof(BYTE));
		m_dwStashGold = (DWORD)gTmp;
		return;
	}
	
	if (aPacket[0] == 0x1e && aPacket[1] == 0x0f)
	{
		// stash gold WORD update
		WORD gTmp;
		memcpy(&gTmp, &aPacket[2], sizeof(WORD));
		m_dwStashGold = (DWORD)gTmp;
		return;
	}
	
	if (aPacket[0] == 0x1f && aPacket[1] == 0x0f)
	{	
		// stash gold DWORD update
		DWORD gTmp;
		memcpy(&gTmp, &aPacket[2], sizeof(DWORD));
		m_dwStashGold = (DWORD)gTmp;
		return;
	}

	// Char level, used for determining gold capacity
	if (aLen > 24
		&& aPacket[0] == 0x5b 
		&& !::memcmp(aPacket + 3, &m_dwPlayerID, 4))
	{
		m_iCharLevel = aPacket[24];
		return;
	}
}

DWORD CGoldManager::GetInventoryGold() const
{
	return m_dwInventoryGold;
}

DWORD CGoldManager::GetStashGold() const
{
	return m_dwStashGold;
}

BYTE CGoldManager::GetCharLevel() const
{
	return m_iCharLevel;
}

DWORD CGoldManager::GetInventoryGoldSpace() const
{
	return GOLDSPACE(m_iCharLevel, m_dwInventoryGold);
}

void CGoldManager::InitAttributes()
{
	CD2Abstract::InitAttributes();

	m_dwPlayerID = 0;
	m_iCharLevel = 1;
	m_dwInventoryGold = 0;
	m_dwStashGold = 0;
}
