//////////////////////////////////////////////////////////////////////
// Pindleskin.h: interface for the CPindleskin class.
//
// A class derived from CD2Unit, used for maintaining Pindleskin 
// states and attributes.
//
// Written by Abin (abinn32@yahoo.com)
// http://www.wxjindu.com/abin/
//
// July 16th, 2004
//////////////////////////////////////////////////////////////////////

#ifndef __PINDLESKIN_H__
#define __PINDLESKIN_H__

///////////////////////////////////////////////////////////////////////
// Pindleskin Bonus Definition
///////////////////////////////////////////////////////////////////////
#define PINDLE_MAX_BONUS					3 // An unit can have up to 3 bonuses
#define PINDLE_BONUS_NONE					0x00 // none
#define PINDLE_BONUS_AURA_ENCHANTED			0xf0 // aura enchanted
#define PINDLE_BONUS_COLD_ENCHANTED			0x90 // cold enchanted
#define PINDLE_BONUS_CURSED					0x38 // cursed
#define PINDLE_BONUS_EXTRA_FAST				0x30 // extra fast
#define PINDLE_BONUS_EXTRA_STRONG			0x28 // extra strong
#define PINDLE_BONUS_LIGHTNING_ENCHANTED	0x88 // lightning enchanted
#define PINDLE_BONUS_FIRE_ENCHANTED			0xb0 // fire enchanted
#define PINDLE_BONUS_MAGIC_RESISTANT		0x40 // magic resistant
#define PINDLE_BONUS_MANA_BURN				0xc8 // mana burn
#define PINDLE_BONUS_MULTISHOT				0xe8 // multishot
#define PINDLE_BONUS_SPECTRAL_HIT			0xd8 // spectral hit
#define PINDLE_BONUS_STONE_SKIN				0xe0 // stone skin
#define PINDLE_BONUS_TELEPORT				0xd0 // teleport


///////////////////////////////////////////////////////////////////////
// Pindleskin Message Definition
///////////////////////////////////////////////////////////////////////
#define PM_BONUSES		DM_APP + 120 // Pindleskin's bonuses just revealed

#include "D2Lib\\D2Monster.h"

class CPindleskin : public CD2Monster  
{
public:
	
	CPindleskin();
	virtual ~CPindleskin();

	///////////////////////////////////////////////////////////////////////
	// Client Entries
	///////////////////////////////////////////////////////////////////////
	virtual void OnGameJoin();
	virtual void OnGameLeave();
	virtual void OnGamePacketBeforeReceived(const BYTE* aPacket, DWORD aLen);

	static DWORD GetBonusName(BYTE iBonus, LPTSTR lpszBuffer, DWORD dwMaxChars);
	const BYTE* GetBonuses() const;
	
	// Blocked
	virtual BOOL SetFingerprinter(LPCD2OBJFINGERPRINTER lpFingerprinter) { return FALSE; }

protected:

	virtual void OnBonuses(const BYTE* aBonuses);
	virtual void OnDestroy();

	///////////////////////////////////////////////////////////////////////
	// Operations
	///////////////////////////////////////////////////////////////////////
	void InitAttributes(); // Clean all attributes

	///////////////////////////////////////////////////////////////////////
	// Protected Member Data
	///////////////////////////////////////////////////////////////////////
	BYTE m_aBonuses[PINDLE_MAX_BONUS]; // Boss bonuses
};

#endif // __PINDLESKIN_H__
