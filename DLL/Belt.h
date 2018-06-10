//////////////////////////////////////////////////////////////////////
// Belt.h
//
// Class definition for BELTITEM and CBelt
//
// BELTITEM: Represents items those can be put into belt slots.
//
// CBelt: Maintains character's belt and potion information.
//
// The code is UNICODE compliant for future compatibility.
//
// 2004 Abin (abinn32@163.com)
//////////////////////////////////////////////////////////////////////

#ifndef __BELT_H__
#define __BELT_H__

#include "D2Lib\\D2Abstract.h"

/////////////////////////////////////////////////////////////////
// belt item type definitions
/////////////////////////////////////////////////////////////////

// Misc Potions
#define POTION_STAMINA				0x07370760
#define POTION_ANTIDOTE				0x07370790
#define POTION_THAWING				0x0736D770

// Healing Potions
#define POTION_MINOR_HEALING		0x03170680
#define POTION_LIGHT_HEALING		0x03270680
#define POTION_HEALING				0x03370680
#define POTION_GREATER_HEALING		0x03470680
#define POTION_SUPER_HEALING		0x03570680

// Mana Potions
#define POTION_MINOR_MANA			0x031706D0
#define POTION_LIGHT_MANA			0x032706D0
#define POTION_MANA					0x033706D0
#define POTION_GREATER_MANA			0x034706D0
#define POTION_SUPER_MANA			0x035706D0

// Rejuvenate Potions
#define POTION_REJUVENATE			0x07376720
#define POTION_FULL_REJUVENATE		0x06C76720

// Scrolls
#define SCROLL_IDENTIFY				0x06373690
#define SCROLL_TOWN_PORTAL			0x06373740

// Belt item categories
enum {  BELT_CATA_UNKNOWN = 0,  // None drinable items, scrolls or invalid items
		BELT_CATA_JUNKPOTION,	// Junky potions
		BELT_CATA_HEALING,		// Healing potions
		BELT_CATA_MANA,			// Mana potions
		BELT_CATA_REJUV };		// Rejuvenation potions

// Belt event types
enum {  BELT_EVENT_BELT = 0,	// The belt itself updated
		BELT_EVENT_ADD,			// Item added to the belt
		BELT_EVENT_REMOVE,		// Item removed from the belt
		BELT_EVENT_SHIFTDOWN };	// Belt rows shifts down(happens when player drank a lower-row item)

// Items that can be put on belt
typedef struct tagBeltItem
{	
	DWORD dwID; // item id
	DWORD dwType; // type
} BELTITEM, *LPBELTITEM;
typedef const tagBeltItem* LPCBELTITEM;

// The CVelt class definition
class CBelt : public CD2Abstract
{
public:		

	// Constructor & Destructor
	CBelt();
	virtual ~CBelt();
	
	// Static methods
	static int GetBeltItemCata(DWORD dwType);
	static BOOL GetBeltItemName(const BELTITEM& item, LPTSTR lpBuffer, DWORD dwMaxLen);
	
	// Item Access
	LPCBELTITEM GetItem(int nRow, int nCol) const; // Access an item by giving its row and column	
	LPCBELTITEM FindFirstItem(DWORD dwType) const; // Find the first item in the belt with the specified type

	// Belt Access
	int GetAvailableCol(DWORD dwItemType) const; // Which row can hold this type of item?
	int GetBeltRows() const; // The maximum row number of the players belt
	BOOL IsBeltFull() const; // Wether the player's belt is full	
	
	// Belt item utilizing
	BOOL HealMe(BOOL bPurpleOnly = TRUE) const; // Drink a potion(player itself)
	BOOL HealMerc(BOOL bPurpleOnly = TRUE) const; // Feed the merc a potion
	BOOL SupplyMana(BOOL bDrinkPurpleIfNoBlue = TRUE) const; // Drink blue potion to get mana
	BOOL DrinkBeltPotion(int nRow, int nCol) const; // Drink a belt potion, which is located at slot(nRow,nCol)
	int DrinkAllDrunks(BOOL bIncludeRejuv = FALSE) const; // Remove all junky potions from the belt

	// Client Entries
	virtual void OnGameJoin(DWORD dwPlayerID); // Must be called in "OnGameJoin"
	virtual void OnGameLeave(); // Must be called in "OnGameLeave"
	virtual void OnGamePacketBeforeReceived(const BYTE* aPacket, DWORD aLen); // Must be called in "OnGamePacketBeforeReceived"
	virtual void OnGamePacketBeforeSent(const BYTE* aPacket, DWORD aLen) {} // Must be called in "OnGamePacketBeforeReceived"
	virtual void OnGameTimerTick() {}

protected:
	virtual void InitAttributes();

	DWORD FindBluePotion() const;
	DWORD FindRedPotion() const;
	DWORD FindPurplePotion() const;
	BOOL DrinkPotion(DWORD dwItemID, BOOL bFeedMerc) const; // Drink a belt potion
	static int GetBeltMaxRow(LPCTSTR lpItemCode); // Maximu row number of a belt whose item code is lpItemCode
	BOOL IsColFull(int nCol) const; // Wether a column is full

	BELTITEM m_items[4][4];	// Items in belt
	int m_nBeltRows; // Current belt row number
	DWORD m_dwPlayerID; // Player's ID	

private:
	virtual void OnGameJoin() {} // blocked
};

#endif // __BELT_H__
