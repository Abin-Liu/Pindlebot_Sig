//////////////////////////////////////////////////////////////////////
// D2Player.h: interface of the CD2Player class.
//////////////////////////////////////////////////////////////////////

#ifndef __D2PLAYER_H__
#define __D2PLAYER_H__

#include "D2StaticObject.h"
#include "D2Unit.h"

#define PLAYER_MAX_NAME		15 // Maximum character name length

///////////////////////////////////////////////////////////////////////
// PLAYERCORPSE Struct Definition
///////////////////////////////////////////////////////////////////////
typedef struct tagPlayerCorpse
{
	DWORD dwCorpseID; // Corpse ID
	WORD x;	// Corpse position x
	WORD y; // Corpse position y
} PLAYERCORPSE, *LPPLAYERCORPSE;
typedef const tagPlayerCorpse* LPCPLAYERCORPSE;

///////////////////////////////////////////////////////////////////////
// CD2Player Class Definition
///////////////////////////////////////////////////////////////////////
class CD2Player : public CD2Abstract
{
public:	

	///////////////////////////////////////////////////////////////////////
	// Constructor(s) & destructor
	///////////////////////////////////////////////////////////////////////
	CD2Player();
	virtual ~CD2Player();

	///////////////////////////////////////////////////////////////////////
	// Client Entries
	///////////////////////////////////////////////////////////////////////
	virtual void OnGameJoin(DWORD dwPlayerID, const WORD* pPositionXAddr, const WORD* pPositionYAddr, const BYTE* pActAddr, const DWORD* pLocationAddr, BYTE iCharClass, LPCTSTR lpszCharName);
	virtual void OnGameLeave();
	virtual void OnGamePacketBeforeReceived(const BYTE* aPacket, DWORD aLen);
	virtual void OnGamePacketBeforeSent(const BYTE* aPacket, DWORD aLen);
	virtual void OnGameTimerTick();

	///////////////////////////////////////////////////////////////////////
	// Public Attributes
	///////////////////////////////////////////////////////////////////////
	BOOL IsValidPlayer() const; // Verify player data
	DWORD GetPlayerID() const; // Retrieve player's ID
	BYTE GetAct() const; // Current act
	WORD GetPositionX() const; // Player position X
	WORD GetPositionY() const; // Player position Y
	BYTE GetLevel() const; // Player's level
	DWORD GetLocation() const; // Player location type
	LPCTSTR GetCharName() const; // Character name
	DWORD GetInventoryGold() const; // Inventory gold amount
	DWORD GetStashGold() const; // Stash gold amount
	WORD GetHP() const; // Current HP
	WORD GetMana() const; // Current mana
	WORD GetLeftSpell() const; // Left-hand spell
	WORD GetRightSpell() const; // Right-hand spell
	BOOL HasSpell(WORD wSpellID) const; // Whether the player has this spell
	DWORD GetCorpseCount() const; // Player's nearby corpse count
	DWORD GetHostilerCount() const; // Hostiler count
	WORD GetDistance(WORD x, WORD y) const; // Player's distance from a position
	DWORD GetInventoryGoldLimit() const; // The maximum gold amount that the player can hold in inventory

	///////////////////////////////////////////////////////////////////////
	// Public Operations
	///////////////////////////////////////////////////////////////////////
	void InitAttributes(); // Clean all attributes

	BOOL RunTo(WORD x, WORD y); // Run to a position
	BOOL RunTo(const CD2StaticObject* pStaticObject) const; // Run to a static object
	BOOL RunTo(const CD2Unit* pNpc) const; // Run to an unit (npc, monster, etc)

	BOOL SelectSpell(WORD wSpell, BOOL bLeft) const; // Select a spell
	BOOL CastSpell(WORD wSpell, BOOL bLeft, WORD x = 0, WORD y = 0) const; // Cast spell on a map position
	BOOL CastSpell(WORD wSpell, BOOL bLeft, CD2StaticObject *pTarget) const; // Cast spell on a static object
	BOOL CastSpell(WORD wSpell, BOOL bLeft, const CD2Unit* pTarget) const; // Cast spell on an unit (npc, monster, etc)

	BOOL Say(LPCTSTR lpszMessage) const; // Say a message
	BOOL OverHead(LPCTSTR lpszMessage, BOOL bClientOnly = TRUE) const; // Display an over-head message
	BOOL SendSound(BYTE iSound) const; // Send a sound
	BOOL AntiIdle() const; // Anti-idle

	BOOL OpenStaticObject(const CD2StaticObject* pStaticObject) const; // Interact with a static object
	BOOL LeaveStaticObject(const CD2StaticObject* pStaticObject) const; // Stop interacting with a static object
	BOOL EnterWP(const CD2StaticObject* pWP, DWORD dwDestination) const; // take WP to another area

	BOOL TalkToNpc(const CD2Unit* pNpc) const; // Interact with an npc
	BOOL LeaveNpc(const CD2Unit* pNpc) const; // Stop interacting with an npc
	BOOL ResurrectMerc(const CD2Unit* pNpc) const; // Resurrect merc
	BOOL OpenNpcStore(const CD2Unit* pNpc) const; // Open an npc's store
	BOOL BuyItem(DWORD dwItemID, const CD2Unit* pNpc) const; // Buy an item from npc
	BOOL RepairItem(const CD2Unit* pNpc, DWORD dwItemID = 0) const; // Repair items

	BOOL DepositGoldToStash(DWORD dwAmount = 0) const; // Deposit inventory gold to stash
	BOOL PickupCorpse() const; // Pick up player's corpse
	BOOL RedrawClient(BOOL bFade = TRUE) const; // Redraw client view

protected:		

	///////////////////////////////////////////////////////////////////////
	// Overrides
	///////////////////////////////////////////////////////////////////////
	virtual void OnSkillList(BYTE iSkillCount);
	virtual void OnCharLevel(BYTE iLevel);
	virtual void OnHPUp(WORD wOldHP, WORD wNewHP);
	virtual void OnHPDown(WORD wOldHP, WORD wNewHP);
	virtual void OnManaUp(WORD wOldMana, WORD wNewMana);
	virtual void OnManaDown(WORD wOldMana, WORD wNewMana);
	virtual void OnGoldUpdate(DWORD dwInventoryGold, DWORD dwStashGold);
	virtual void OnHostile(DWORD dwHostilerID, DWORD dwHostilerCount);
	virtual void OnUnhostile(DWORD dwUnhostilerID, DWORD dwHostilerCount);
	virtual void OnPlayerDeath(BYTE iAct, WORD x, WORD y);
	virtual void OnCorpse(DWORD dwCorpseID, WORD x, WORD y);
	virtual void OnCorpsePicked();
	virtual void OnLSpell(WORD wOldSpell, WORD wNewSpell);
	virtual void OnRSpell(WORD wOldSpell, WORD wNewSpell);

	///////////////////////////////////////////////////////////////////////
	// Protected Operations
	///////////////////////////////////////////////////////////////////////
	BOOL CastSpellOnEntity(WORD wSpell, BOOL bLeft, DWORD dwType, DWORD dwEntityID) const;
	BOOL StopInteractWithEntity(DWORD dwType, DWORD dwEntityID) const;
	BOOL PrepareInteractWithNPC(DWORD dwNpcID, WORD  x, WORD y) const;
	BOOL InteractWithEntity(DWORD dwType, DWORD dwEntityID) const;
	BOOL RunToEntity(DWORD dwType, DWORD dwEntityID, WORD x, WORD y) const;

	///////////////////////////////////////////////////////////////////////
	// Protected Member Data
	///////////////////////////////////////////////////////////////////////
	DWORD m_dwPlayerID;
	BYTE m_iCharClass;
	BYTE m_iCharLevel;
	const WORD* m_pPositionXAddr;
	const WORD* m_pPositionYAddr;
	const BYTE* m_pActAddr;
	const DWORD* m_pLocationAddr;
	TCHAR m_szCharName[PLAYER_MAX_NAME + 1];
	CArrayEx<WORD, WORD> m_aSkillList;
	CArrayEx<DWORD, DWORD> m_aHostilerList;
	CArrayEx<PLAYERCORPSE, PLAYERCORPSE> m_aCorpseList;
	WORD m_wHP;
	WORD m_wMana;
	DWORD m_dwInventoryGold;
	DWORD m_dwStashGold;
	WORD m_wLeftSpell;
	WORD m_wRightSpell;

private:
	virtual void OnGameJoin() {} // Blocked

};

#endif // __D2PLAYER_H__