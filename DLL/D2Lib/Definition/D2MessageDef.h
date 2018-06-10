//////////////////////////////////////////////////////////////////////
// D2MessageDef.h
//
// This file defines common Diablo II event messages. Messages are generated
// when something meaningful happens in a Diablo II game, and are sent to the
// applications via callback functions, along with a LPCVOID, a WPARAM,
// a LPARAM, and a 32-bit application defined data LPVOID.
//
// Value of the LPCVOID is always the address of the CD2Abstract object which
// sends the messages, value of the WPARAM and the LPARAM depends on the
// message types.
// 
// This mechanism provides an "event-driving" interface to your applications,
// and your applications should handle the messages sent to them appropriately.
//
// Written by Abin (abinn32@yahoo.com)
// http://www.wxjindu.com/abin/
//
// July 14th, 2004
//
//////////////////////////////////////////////////////////////////////

#ifndef __D2MESSAGEDEF_H__
#define __D2MESSAGEDEF_H__

enum {

	///////////////////////////////////////////////////////////////////////
	// CD2Abstract Messages
	///////////////////////////////////////////////////////////////////////
	DM_VOID	= 0,		// Placeholder

	///////////////////////////////////////////////////////////////////////
	// CD2Object Messages
	///////////////////////////////////////////////////////////////////////
	OM_DESTROY,			// Object is gone from current player's vision

	///////////////////////////////////////////////////////////////////////
	// CD2StaticObject Messages
	///////////////////////////////////////////////////////////////////////
	SM_SPAWN,			// Static Object spawns

	///////////////////////////////////////////////////////////////////////
	// CD2Uint Message Definition
	///////////////////////////////////////////////////////////////////////
	UM_SPAWN,			// Unit spawns
	UM_STARTMOVE,		// Unit starts to move
	UM_ENDMOVE,			// Unit ends movind

	///////////////////////////////////////////////////////////////////////
	// CD2Monster Message Definition
	///////////////////////////////////////////////////////////////////////
	MM_SPAWN,			// Monster spawns
	MM_HPPERCENTUP,		// Monster HP percentage increases
	MM_HPPERCENTDOWN,	// Monster HP percentage decreases
	MM_STARTMOVE,		// Monster starts to move
	MM_ENDMOVE,			// Monster ends movind
	MM_ATTACK,			// Monster attacks
	MM_UNDERATTACK,		// Monster under attack
	MM_DEATH,			// Monster dies
	MM_CORPSE,			// Monster corpse appears
	MM_AURAACTIVATE,	// Monster activates an aura
	MM_AURAAFFECTED,	// Monster is affected by an aura
	MM_REVIVED,			// Monster is revived(e.g. fallen by shaman)

	///////////////////////////////////////////////////////////////////////
	// CD2Player Message Definition
	///////////////////////////////////////////////////////////////////////
	PM_SKILLLIST,		// Player's skill list retrieved
	PM_LSPELL,			// Player's left-hand spell changed
	PM_RSPELL,			// Player's right-hand spell changed
	PM_LEVEL,			// Player's character level retrieved
	PM_HPUP,			// Player HP increases
	PM_HPDOWN,			// Player HP decreases
	PM_MANAUP,			// Player mana increases
	PM_MANADOWN,		// Player mana decreases
	PM_DEATH,			// Player dies
	PM_CORPSE,			// Player's corpse appears
	PM_CORPSEPICKEDUP,	// Player's corpse picked up
	PM_HOSTILE,			// Player is hostiled by other players
	PM_UNHOSTILE,		// Player is unhostiled by other players
	PM_GOLD,			// Player gold amount update

	///////////////////////////////////////////////////////////////////////
	// Available for Application Defined Messages
	///////////////////////////////////////////////////////////////////////
	DM_APP
};

#endif