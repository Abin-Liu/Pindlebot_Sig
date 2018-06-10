/////////////////////////////////////////////////////////////////////
// Item.h
//
// Definitions of ITEM struct and ITEMCONFIG struct.
//
// Bellow code are UNICODE compliant for future compatibility.
//
// 2004 Abin (abinn32@yahoo.com)
//////////////////////////////////////////////////////////////////////

#ifndef __ITEM_H__
#define __ITEM_H__

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <tchar.h>

// Packet types
#define	ITEM_MESSAGEID_DROP				0x9C // Message ID
#define	ITEM_ACTION_NEW_GROUND			0x00
#define	ITEM_ACTION_PICK_UP				0x01
#define	ITEM_ACTION_DROP				0x02 
#define	ITEM_ACTION_OLD_GROUND			0x03
#define	ITEM_ACTION_PLACE_UNKNOWN		0x04
#define	ITEM_ACTION_TO_STORE			0x0B
#define	ITEM_ACTION_FROM_STORE			0x0C
#define	ITEM_ACTION_PLACE_BELT			0x0E
	
// Item types
#define	ITEM_ITEMTYPE_HELM				0x00
#define	ITEM_ITEMTYPE_ARMOR				0x01
#define	ITEM_ITEMTYPE_WEAPON			0x05
#define	ITEM_ITEMTYPE_BOW				0x06
#define	ITEM_ITEMTYPE_SHIELD			0x07
#define	ITEM_ITEMTYPE_EXPANSION			0x0A
#define	ITEM_ITEMTYPE_OTHER				0x10

// Drop locations
#define	ITEM_LOCAIION_STORED			0x00
#define	ITEM_LOCATION_WORN				0x01
#define	ITEM_LOCATION_BELT				0x02
#define	ITEM_LOCATION_GROUND			0x03
#define	ITEM_LOCATION_TOHAND			0x04

// Item levels
#define	ITEM_LEVEL_INFERIOR				0x01
#define	ITEM_LEVEL_NORMAL				0x02
#define	ITEM_LEVEL_SUPERIOR				0x03
#define	ITEM_LEVEL_MAGIC				0x04
#define	ITEM_LEVEL_SET					0x05
#define	ITEM_LEVEL_RARE					0x06
#define	ITEM_LEVEL_UNIQUE				0x07
#define	ITEM_LEVEL_CRAFT				0x08
#define	ITEM_INFERIORTYPE_CRUDE			0x00
#define	ITEM_INFERIORTYPE_CRACKED		0x01
#define	ITEM_INFERIORTYPE_DAMAGED		0x02
#define	ITEM_INFERIORTYPE_LOWQUALITY	0x04
#define	ITEM_SUPERIOR_AR				0x00
#define	ITEM_SUPERIOR_MAXDMG			0x01
#define	ITEM_SUPERIOR_AC				0x02
#define	ITEM_SUPERIOR_AR_MAXDMG			0x03
#define	ITEM_SUPERIOR_DUR				0x04
#define	ITEM_SUPERIOR_DUR_AR			0x05
#define	ITEM_SUPERIOR_DUR_MAXDMG		0x06
#define	ITEM_SUPERIOR_DUR_AC			0x07

// Gold sizes
#define	ITEM_GOLDSIZE_12BIT				0x00
#define	ITEM_GOLDSIZE_32BIT				0x01
#define	ITEM_UNSET						0xFF

// Miscs
#define	ITEM_CODE_LEN					3
#define	ITEM_DESC_LEN					128

// special item codes
#define ITEM_GOLD						_T("gld")
#define ITEM_REJUV						_T("rvs")
#define ITEM_FULL_REJUV					_T("rvl")

// The ITEM struct
typedef struct tagItem
{
	DWORD dwItemID; // item ID
	BYTE iType;		// type: weapon, armor, etc
	BYTE iIdentified; // identified?
	BYTE iEthereal; // ethereal?
	BYTE iSocketed; // has sockets on it?
	BYTE iLevel; // level: unique, rare, set, superior, etc
	BYTE iLocation; // ground, belt, inventory etc
	WORD wPositionX; // location x
	WORD wPositionY; // location y
	DWORD dwGoldAmount; // Gold amount, "gld" only
	TCHAR szItemCode[ITEM_CODE_LEN + 1]; // item code
} ITEM, *LPITEM;
typedef const tagItem* LPCITEM;

// The ITEMCONFIG struct
typedef struct tagItemConfig
{
	BYTE iPickup; // pick up or ignore?
	BYTE iIdentified; // identified?
	BYTE iEthereal; // ethereal?
	BYTE iSocketed; // has socket?
	BYTE iLevel; // level: unique, rare, set, superior, etc
	BYTE iType; // type: weapon, armor, etc
	TCHAR szItemCode[ITEM_CODE_LEN + 1]; // item code
	TCHAR szDescription[ITEM_DESC_LEN + 1]; // description: "{elite} Unique hydra bow(Windforce) dropped."
} ITEMCONFIG, *LPITEMCONFIG;
typedef const tagItemConfig* LPCITEMCONFIG;

BOOL ParseItemInfo(const BYTE* aPacket, DWORD aLen, ITEM& item); // Parses item info from a packet
BOOL ParseItemDrop(const BYTE* aPacket, DWORD aLen, ITEM& item); // Parses item info from a item-drop packet
BOOL GetItemCode(const BYTE* aPacket, DWORD aLen, LPTSTR szBuffer); // Extracts item code from a packet
BOOL IsItemMatchConfig(const ITEMCONFIG& config, const ITEM& item); // Compares an ITEM with an ITEMCONFIG
BOOL IsGold(LPCTSTR lpszItemCode); // TRUE if is "gld"
BOOL IsRejuv(LPCTSTR lpszItemCode); // TRUE if is "rvs"
BOOL IsFullRejuv(LPCTSTR lpszItemCode); // TRUE if is "rvl"

#endif