//////////////////////////////////////////////////////////////////////
// D2Monster.h: interface of the CD2Monster class.
//////////////////////////////////////////////////////////////////////

#ifndef __D2MONSTER_H__
#define __D2MONSTER_H__

#include "D2Unit.h"

///////////////////////////////////////////////////////////////////////
// Monster Aura Type Definition
///////////////////////////////////////////////////////////////////////
#define MONSTER_AURA_NONE				0x00 // none
#define MONSTER_AURA_HOLYFREEZE			0x2b // holy freeze
#define MONSTER_AURA_BLESSEDAIM			0x28 // blessed aim
#define MONSTER_AURA_MIGHT				0x21 // might
#define MONSTER_AURA_HOLYFIRE			0x23 // holy fire
#define MONSTER_AURA_HOLYSHOCK			0x2e // holy shock
#define MONSTER_AURA_CONVICTION			0x1c // conviction
#define MONSTER_AURA_FANATICISM			0x31 // fanaticism
#define MONSTER_AURA_SALVATION			0x08 // Salvation
#define MONSTER_AURA_REDEMPTION			0x32 // Redemption
#define MONSTER_AURA_MEDITATION			0x30 // Meditation
#define MONSTER_AURA_VIGOR				0x29 // Vigor
#define MONSTER_AURA_RESISTLIGHTNING	0x05 // Resist lightning
#define MONSTER_AURA_CLEANSING			0x2d // Cleansing
#define MONSTER_AURA_RESISTCOLD			0x04 // Resist cold
#define MONSTER_AURA_DEFIANCE			0x25 // Defiance
#define MONSTER_AURA_RESISTFIRE			0x03 // Resist fire
#define MONSTER_AURA_PRAYER				0x22 // Prayer
#define MONSTER_AURA_SANCTUARY			0x2f // Sanctuary
#define MONSTER_AURA_CONCENTRATION		0x2a // Concentration
#define MONSTER_AURA_THORNS				0x24 // Thorns


///////////////////////////////////////////////////////////////////////
// CD2Monster Class Definition
///////////////////////////////////////////////////////////////////////
class CD2Monster : public CD2Unit
{
public:

	///////////////////////////////////////////////////////////////////////
	// Constructor(s) & destructor
	///////////////////////////////////////////////////////////////////////
	CD2Monster();
	virtual ~CD2Monster();

	///////////////////////////////////////////////////////////////////////
	// Client Entries
	///////////////////////////////////////////////////////////////////////
	virtual void OnGameJoin();
	virtual void OnGameLeave();
	virtual void OnGamePacketBeforeReceived(const BYTE* aPacket, DWORD aLen);
	virtual void OnGamePacketBeforeSent(const BYTE* aPacket, DWORD aLen);
	virtual void OnGameTimerTick();

	///////////////////////////////////////////////////////////////////////
	// Public Attributes
	///////////////////////////////////////////////////////////////////////
	BYTE GetHPPercent() const; // Monster's current HP percentage (0 to 100)
	BOOL IsAlive() const; // Whether the object is alive
	BOOL IsCorpse() const; // Whether the object's corpse has appeared	
	BYTE GetActivatedAura() const; // Retrieves Monster's activated aura type
	BYTE GetAffectedAura() const; // Retrieves Monster's affected aura type
	static DWORD GetAuraName(BYTE iAura, LPTSTR lpszBuffer, DWORD dwMaxChars); // Get aura name text

	///////////////////////////////////////////////////////////////////////
	// Public Operations
	///////////////////////////////////////////////////////////////////////
	void InitAttributes(); // Clean all attributes

protected:

	///////////////////////////////////////////////////////////////////////
	// Overrides
	///////////////////////////////////////////////////////////////////////
	virtual void OnMonsterSpawn(DWORD dwObjectID, WORD x, WORD y); // Monster spawned
	virtual void OnHPPercentUp(BYTE iOldPercent, BYTE iNewPercent); // Monster's HP percent increased
	virtual void OnHPPercentDown(BYTE iOldPercent, BYTE iNewPercent); // Monster's HP percent decreased
	virtual void OnAuraActivated(BYTE iAura); // Monster activated an aura
	virtual void OnAuraAffected(BYTE iAura); // Monster is affected by an aura
	virtual void OnStartMove(WORD wFromX, WORD wFromY, WORD wToX, WORD wToY); // Monster started to move
	virtual void OnEndMove(WORD wFromX, WORD wFromY, WORD wToX, WORD wToY); // Monster ended moving
	virtual void OnAttack(WORD x, WORD y, DWORD dwVictimID); // Monster attacked some targets
	virtual void OnUnderAttack(WORD x, WORD y, DWORD dwAttackerID); // Monster is under attack
	virtual void OnDeath(); // Monster died
	virtual void OnCorpseAppeared(WORD x, WORD y); // Monster's corpse appeared
	virtual void OnRevived(WORD x, WORD y);
	virtual void OnDestroy(); // Monster is gone from player's vision

	///////////////////////////////////////////////////////////////////////
	// Protected Operations
	///////////////////////////////////////////////////////////////////////

	///////////////////////////////////////////////////////////////////////
	// Protected Member Data
	///////////////////////////////////////////////////////////////////////
	BOOL m_bIsAlive; // Whether the Monster is alive
	BOOL m_bIsCorpse; // Whether the Monster's corpse has appeared
	BYTE m_iHPPercent; // Monster HP remaining percent
	BYTE m_iAuraActivated; // Monster actives aura
	BYTE m_iAuraAffected; // Monster is affected by aura
};

#endif // __D2MONSTER_H__