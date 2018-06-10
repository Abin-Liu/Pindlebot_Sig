//////////////////////////////////////////////////////////////////////
// GoldManager.h
//
// Class definition for CGoldManager
//
// Maintains character's inventory & stash gold information.
//
// The code is UNICODE compliant for future compatibility.
//
// 2004 Abin (abinn32@163.com)
//////////////////////////////////////////////////////////////////////

#ifndef __GOLDMANAGER_H__
#define __GOLDMANAGER_H__

#include "D2Lib\\D2Abstract.h"

class CGoldManager : public CD2Abstract
{
public:	
	
	CGoldManager();
	virtual ~CGoldManager();

	DWORD GetInventoryGold() const;
	DWORD GetStashGold() const;
	DWORD GetInventoryGoldSpace() const; // How much more gold the player's inventory can hold?
	BYTE GetCharLevel() const;

	// Client related
	virtual void OnGameJoin(DWORD dwPlayerID); // Must be called in "OnGameJoin"
	virtual void OnGameLeave(); // Must be called in "OnGameLeave"
	virtual void OnGamePacketBeforeReceived(const BYTE* aPacket, DWORD aLen); // Must be called in "OnGamePacketBeforeReceived"
	virtual void OnGamePacketBeforeSent(const BYTE* aPacket, DWORD aLen) {}
	virtual void OnGameTimerTick() {}

protected:
	virtual void InitAttributes();
	DWORD m_dwInventoryGold; // Amount of gold in inventory
	DWORD m_dwStashGold; // Amount of gold in stash
	BYTE m_iCharLevel; // Player's level
	DWORD m_dwPlayerID; // Player's ID

private:
	virtual void OnGameJoin() {} // blocked
};

#endif // __GOLDMANAGER_H__
