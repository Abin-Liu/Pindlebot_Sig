/////////////////////////////////////////////////////////////////////
// PickitHelper.h
//
// CPickitHelper, a class that encapsulates all essential features of the
// famous "pickit" module and enhances it by some new stuff such like:
// OOP implementation, item queue export, gold & rejuv processing, etc.
//
// This class is UNICODE compliant for future compatibility.
//
// 2004 Abin (abinn32@163.com)
//////////////////////////////////////////////////////////////////////

#ifndef __PICKITHELPER_H__
#define __PICKITHELPER_H__

#include "D2Lib\\D2Abstract.h"
#include "item.h"
#include "D2lib\\template\\linkedlist.h" // This is a type-safe LinkedList
#include "Belt.h"
#include "GoldManager.h"

// To announce an item drop
typedef void (CALLBACK *ItemDropProc)(const ITEM& item, LPCTSTR lpszItemDescription, LPARAM lParam);

// To notify the client that player's inventory is full
typedef void (CALLBACK *InventoryFullProc)(LPARAM lParam);

#define PICKIT_DELAY_MIN		100 // Minimal pickup delay
#define PICKIT_DELAY_DEFAULT	200	// Default pickup delay 
#define PICKIT_RADIUS_MAX		50 // Maximal pickup radius
#define PICKIT_RADIUS_DEFAULT	10 // Default pickup radius

// Special items (gold, rejuvenation potions, full rejuvenation potions)
enum { PICKIT_SPECIAL_UNKNOWN = 0, PICKIT_SPECIAL_GOLD, PICKIT_SPECIAL_REJUV, PICKIT_SPECIAL_FULLREJUV};

// Pick up methods for special items (gold, rejuvenation potions, full rejuvenation potions)
enum { PICKIT_PICK_UNKNWON = 0, // Error 
	   PICKIT_PICK_NO,			// Never pick up
	   PICKIT_PICK_YES,			// Always pick up
	   PICKIT_PICK_AUTO };		// Automatic - Pick up when the player can hold more(gold)
								// or the player's belt has empty slots(potions), and disable
								// automatically otherwise.

// The CPickitHelper Class Definition
class CPickitHelper : public CD2Abstract
{
public:		
	void CleanConfig();
			
	// Constructor(s) & Destructor
	CPickitHelper();
	virtual ~CPickitHelper();

	// Enable/disable pickup
	void Enable(BOOL bEnable);
	BOOL IsEnabled() const;
	BOOL Toggle(); // Switch mode(on/off)

	// Pickup radius access
	DWORD GetRadius() const;
	DWORD SetRadius(DWORD dwRadius);	
	
	// Load configurations from *.ini, usually from pickit.ini
	BOOL LoadConfig(LPCTSTR lpszFile);
	LPCTSTR GetConfigFile() const;
	int GetConfigCount() const; // Entries loaded from *.ini

	// write log file
	static BOOL WriteLog(LPCTSTR lpszFilePath, LPCTSTR lpszString, BOOL bAppend = TRUE);

	// Set callback function pointers
	// Should be called in "OnClientStart"
	void SetItemDropProc(ItemDropProc pFunc, LPARAM lParam);
	void SetInventoryFullProc(InventoryFullProc pFunc, LPARAM lParam);

	// Pickup delay: the delay between two pickup packets are sent, in milliseconds
	DWORD SetDelay(DWORD dwDelay);
	DWORD GetDelay() const;

	// Character level and inventory gold access

	// Special item pickup methods
	int GetSpecialItemMethod(int nSpecialItem) const;
	BOOL SetSpecialItemMethod(int nSpecialItem, int nMethod);

	// Item queue access
	void CleanQueue(); // Should be called in "OnGameJoin"
	int GetQueueItemCount(BOOL bValuableOnly = FALSE) const; // Items in queue
	int RemoveFromQueue(DWORD dwItemID); // Removes a particular item from the queue

	// Member object access
	const CBelt& GetBelt() const;
	const CGoldManager& GetGoldManager() const;

	virtual void OnGameJoin(DWORD dwPlayerID, const WORD* pPlayerXAddr, const WORD* pPlayerYAddr);
	virtual void OnGameLeave();
	virtual void OnGamePacketBeforeReceived(const BYTE* aPacket, DWORD aLen);
	virtual void OnGamePacketBeforeSent(const BYTE* aPacket, DWORD aLen) {}
	virtual void OnGameTimerTick();

protected:	
	
	// Internal methods
	const ITEMCONFIG* FindItem(const ITEM& item) const;
	BOOL SendPickPacket(const ITEM& item, DWORD dwTicker);
	BOOL IsNeedPickup(const ITEM& item, BOOL bCfgPickup) const;	

protected:
	void InitAttributes();

	// Member data:

	// Callback function addresses and callback parameters
	ItemDropProc m_pItemDropProc;
	InventoryFullProc m_pInventoryFullProc;
	LPARAM m_lItemDropParam;
	LPARAM m_lInventoryFullParam;

	int m_nInventoryFullState; // Current inventory state

	// Memory addresses where we will be reading the player's location
	const WORD* m_pPlayerX;
	const WORD* m_pPlayerY;	
	
	DWORD m_dwPlayerID; // Player's ID

	// Pickup methods for special items
	int m_nGoldMethod; // Gold
	int m_nRejuvMethod; // Small rejuvenation potions
	int m_nFullRejuvMethod; // Full rejuvenation potions

	CLinkedList<ITEM, const ITEM&, NULL> m_aItemQueue; // The item queue	
	CLinkedList<ITEMCONFIG, const ITEMCONFIG&, NULL> m_aConfig; // The configuration queue
	TCHAR m_szConfigFile[MAX_PATH + 1]; // Path name of the *.ini, usually "pickit.ini"
	BOOL m_bBusy; // A flag to avoid possible access conflicts
	POSITION m_curPos; // Position of current item in the item queue
	DWORD m_dwDelay; // pickup delay, in milliseconds
	DWORD m_dwLastProc; // Time when last pickup packet was sent
	BOOL m_bPaused; // Enabled/disabled flag
	DWORD m_dwRadius; // Pickup radius
	CBelt m_belt; // Player's belt
	CGoldManager m_gold;

private:
	virtual void OnGameJoin() {} // blocked
};

#endif // __PICKITHELPER_H__
