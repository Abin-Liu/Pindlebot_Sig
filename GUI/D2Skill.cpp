//////////////////////////////////////////////////////////////////
//   D2Skills.cpp
//
// Implementation of CD2Skill class and CD2Character class.
//
// Copyright Abin (Bin Liu)
// All rights reserved.
// 9/19/2002
//
//////////////////////////////////////////////////////////////////

#include "stdafx.h" // only include if used in MFC applications
#include "D2Skill.h"
#include <assert.h>
#include <string.h>
#include <stdio.h>

//////////////////////////////////////////////////////////////////
// Implementation of CD2Skill class
//////////////////////////////////////////////////////////////////

BOOL CD2Skill::GetSkillInfo(WORD wSkillID, LPD2SKILLINFO lpDsi)
{
	BOOL bSkillExists = TRUE;
	::memset(lpDsi, 0, sizeof(D2SKILLINFO));
	
	switch (wSkillID)
	{
		// Common Skills 
		case D2S_ATTACK:			
			
			lpDsi->dwSkillInfoFlag ^= DSI_TARGETABLE;
			lpDsi->dwSkillInfoFlag ^= DSI_PHYSICAL;
			lpDsi->dwSkillInfoFlag ^= DSI_MELEE;
			
			::strncpy(lpDsi->szSkillName, "Attack", MAX_SKILLNAME_LEN);
			break;

		case D2S_THROW:			
			
			lpDsi->dwSkillInfoFlag ^= DSI_TARGETABLE;
			lpDsi->dwSkillInfoFlag ^= DSI_PHYSICAL;
						
			::strncpy(lpDsi->szSkillName, "Throw", MAX_SKILLNAME_LEN);
			break;

		case D2S_UNSUMMON:			
			
			lpDsi->dwSkillInfoFlag ^= DSI_RIGHTONLY;
			lpDsi->dwSkillInfoFlag ^= DSI_TARGETABLE;			
			lpDsi->dwSkillInfoFlag ^= DSI_ENABLEINTOWN;	
			
			::strncpy(lpDsi->szSkillName, "Unsummon", MAX_SKILLNAME_LEN);
			break;

		case D2S_TOMBOFIDENTIFY:			
			
			lpDsi->dwSkillInfoFlag ^= DSI_RIGHTONLY;
			lpDsi->dwSkillInfoFlag ^= DSI_TARGETABLE;			
			lpDsi->dwSkillInfoFlag ^= DSI_ENABLEINTOWN;	
			
			::strncpy(lpDsi->szSkillName, "Tomb of Identify", MAX_SKILLNAME_LEN);
			break;

		case D2S_SCROLLOFIDENTIFY:
			
			
			lpDsi->dwSkillInfoFlag ^= DSI_RIGHTONLY;
			lpDsi->dwSkillInfoFlag ^= DSI_TARGETABLE;			
			lpDsi->dwSkillInfoFlag ^= DSI_ENABLEINTOWN;	
			
			::strncpy(lpDsi->szSkillName, "Scroll of Identify", MAX_SKILLNAME_LEN);
			break;

		case D2S_TOMBOFTOWNPORTAL:
						
			lpDsi->dwSkillInfoFlag ^= DSI_RIGHTONLY;			
						
			::strncpy(lpDsi->szSkillName, "Tomb of Town Portal", MAX_SKILLNAME_LEN);
			break;

		case D2S_SCROLLOFTOWNPORTAL:			
			
			lpDsi->dwSkillInfoFlag ^= DSI_RIGHTONLY;			
						
			::strncpy(lpDsi->szSkillName, "Scroll of Town Portal", MAX_SKILLNAME_LEN);
			break;

///////////////////////////////////////////////////////////////
//    Amazon Skills
///////////////////////////////////////////////////////////////

// Javelin and Spear Skills
		case D2S_JAB:			
			
			lpDsi->dwSkillInfoFlag ^= DSI_TARGETABLE;
			lpDsi->dwSkillInfoFlag ^= DSI_PHYSICAL;
			lpDsi->dwSkillInfoFlag ^= DSI_MELEE;
						
			::strncpy(lpDsi->szSkillName, "Jab", MAX_SKILLNAME_LEN);
			break;

		case D2S_IMPALE:			
			
			lpDsi->dwSkillInfoFlag ^= DSI_TARGETABLE;
			lpDsi->dwSkillInfoFlag ^= DSI_PHYSICAL;
			lpDsi->dwSkillInfoFlag ^= DSI_MELEE;
						
			::strncpy(lpDsi->szSkillName, "Impale", MAX_SKILLNAME_LEN);
			break;

		case D2S_FEND:			
			
			lpDsi->dwSkillInfoFlag ^= DSI_AFFECTGROUP;
			lpDsi->dwSkillInfoFlag ^= DSI_TARGETABLE;
			lpDsi->dwSkillInfoFlag ^= DSI_PHYSICAL;
			lpDsi->dwSkillInfoFlag ^= DSI_MELEE;			
						
			::strncpy(lpDsi->szSkillName, "Fend", MAX_SKILLNAME_LEN);
			break;

		case D2S_POWERSTRIKE:			
			
			lpDsi->dwSkillInfoFlag ^= DSI_LIGHTNING;
			lpDsi->dwSkillInfoFlag ^= DSI_TARGETABLE;
			lpDsi->dwSkillInfoFlag ^= DSI_PHYSICAL;
			lpDsi->dwSkillInfoFlag ^= DSI_MELEE;
			
			::strncpy(lpDsi->szSkillName, "Power Strike", MAX_SKILLNAME_LEN);
			break;

		case D2S_CHARGEDSTRIKE:			
			
			lpDsi->dwSkillInfoFlag ^= DSI_LIGHTNING;
			lpDsi->dwSkillInfoFlag ^= DSI_AFFECTGROUP;
			lpDsi->dwSkillInfoFlag ^= DSI_TARGETABLE;
			lpDsi->dwSkillInfoFlag ^= DSI_PHYSICAL;
			lpDsi->dwSkillInfoFlag ^= DSI_MELEE;			
			
			::strncpy(lpDsi->szSkillName, "Charged Strike", MAX_SKILLNAME_LEN);
			break;

		case D2S_LIGHTNINGSTRIKE:			
			
			lpDsi->dwSkillInfoFlag ^= DSI_LIGHTNING;
			lpDsi->dwSkillInfoFlag ^= DSI_AFFECTGROUP;
			lpDsi->dwSkillInfoFlag ^= DSI_TARGETABLE;
			lpDsi->dwSkillInfoFlag ^= DSI_PHYSICAL;
			lpDsi->dwSkillInfoFlag ^= DSI_MELEE;	
			
			::strncpy(lpDsi->szSkillName, "Lightning Strike", MAX_SKILLNAME_LEN);
			break;

		case D2S_POISONJAVELIN:
			
			lpDsi->dwSkillInfoFlag ^= DSI_POISON;
			lpDsi->dwSkillInfoFlag ^= DSI_TARGETABLE;
			lpDsi->dwSkillInfoFlag ^= DSI_PHYSICAL;
			
			::strncpy(lpDsi->szSkillName, "Poison Javelin", MAX_SKILLNAME_LEN);
			break;

		case D2S_LIGHTNINGBOLT:
			
			lpDsi->dwSkillInfoFlag ^= DSI_LIGHTNING;
			lpDsi->dwSkillInfoFlag ^= DSI_TARGETABLE;
			lpDsi->dwSkillInfoFlag ^= DSI_PHYSICAL;
			
			::strncpy(lpDsi->szSkillName, "Lightning Bolt", MAX_SKILLNAME_LEN);
			break;

		case D2S_PLAGUEJAVELIN:
			
			lpDsi->dwSkillInfoFlag ^= DSI_POISON;
			lpDsi->dwSkillInfoFlag ^= DSI_AFFECTGROUP;
			lpDsi->dwSkillInfoFlag ^= DSI_TARGETABLE;
			lpDsi->dwSkillInfoFlag ^= DSI_PHYSICAL;
			
			::strncpy(lpDsi->szSkillName, "Plague Javelin", MAX_SKILLNAME_LEN);
			break;
			
		case D2S_LIGHTNINGFURY:
			
			lpDsi->dwSkillInfoFlag ^= DSI_LIGHTNING;
			lpDsi->dwSkillInfoFlag ^= DSI_AFFECTGROUP;
			lpDsi->dwSkillInfoFlag ^= DSI_TARGETABLE;
			lpDsi->dwSkillInfoFlag ^= DSI_PHYSICAL;
			
			::strncpy(lpDsi->szSkillName, "Lightning Fury", MAX_SKILLNAME_LEN);
			break;


// Passive and Magic Skills
		case D2S_INNERSIGHT:			
			
			lpDsi->dwSkillInfoFlag ^= DSI_RIGHTONLY;			
			
			::strncpy(lpDsi->szSkillName, "Inner Sight", MAX_SKILLNAME_LEN);
			break;

		case D2S_SLOWMISSILES:			
			
			lpDsi->dwSkillInfoFlag ^= DSI_RIGHTONLY;			
			
			::strncpy(lpDsi->szSkillName, "Slow Missiles", MAX_SKILLNAME_LEN);
			break;

		case D2S_DECOY:			
			
			lpDsi->dwSkillInfoFlag ^= DSI_RIGHTONLY;
			lpDsi->dwSkillInfoFlag ^= DSI_TARGETABLE;			
			lpDsi->dwSkillInfoFlag ^= DSI_ENABLEINTOWN;
			lpDsi->dwSkillInfoFlag ^= DSI_SUMMON;

			::strncpy(lpDsi->szSkillName, "Decoy", MAX_SKILLNAME_LEN);
			break;

		case D2S_VALKYRIE:
			
			lpDsi->dwSkillInfoFlag ^= DSI_RIGHTONLY;
			lpDsi->dwSkillInfoFlag ^= DSI_TARGETABLE;			
			lpDsi->dwSkillInfoFlag ^= DSI_ENABLEINTOWN;
			lpDsi->dwSkillInfoFlag ^= DSI_SUMMON;

			::strncpy(lpDsi->szSkillName, "Valkyrie", MAX_SKILLNAME_LEN);
			break;

		case D2S_DODGE:
			
			lpDsi->dwSkillInfoFlag ^= DSI_PASSIVE;
			
			::strncpy(lpDsi->szSkillName, "Dodge", MAX_SKILLNAME_LEN);
			break;

		case D2S_AVOID:
			
			lpDsi->dwSkillInfoFlag ^= DSI_PASSIVE;
			
			::strncpy(lpDsi->szSkillName, "Avoid", MAX_SKILLNAME_LEN);
			break;

		case D2S_EVADE:
			
			lpDsi->dwSkillInfoFlag ^= DSI_PASSIVE;
			
			::strncpy(lpDsi->szSkillName, "Evade", MAX_SKILLNAME_LEN);
			break;

		case D2S_CRITICALSTRIKE:
			
			lpDsi->dwSkillInfoFlag ^= DSI_PASSIVE;
			
			::strncpy(lpDsi->szSkillName, "Critical Strike", MAX_SKILLNAME_LEN);
			break;

		case D2S_PENETRATE:
			
			lpDsi->dwSkillInfoFlag ^= DSI_PASSIVE;
			
			::strncpy(lpDsi->szSkillName, "Penetrate", MAX_SKILLNAME_LEN);
			break;

		case D2S_PIERCE:
			
			lpDsi->dwSkillInfoFlag ^= DSI_PASSIVE;
			
			::strncpy(lpDsi->szSkillName, "Pierce", MAX_SKILLNAME_LEN);
			break;

// Bow and Crossbow Skills
		case D2S_COLDARROW:			
			
			lpDsi->dwSkillInfoFlag ^= DSI_COLD;
			lpDsi->dwSkillInfoFlag ^= DSI_TARGETABLE;
			lpDsi->dwSkillInfoFlag ^= DSI_PHYSICAL;			
			
			::strncpy(lpDsi->szSkillName, "Cold Arrow", MAX_SKILLNAME_LEN);
			break;

		case D2S_ICEARROW:			
			
			lpDsi->dwSkillInfoFlag ^= DSI_COLD;
			lpDsi->dwSkillInfoFlag ^= DSI_TARGETABLE;
			lpDsi->dwSkillInfoFlag ^= DSI_PHYSICAL;				
			
			::strncpy(lpDsi->szSkillName, "Ice Arrow", MAX_SKILLNAME_LEN);
			break;

		case D2S_FREEZINGARROW:
			
			lpDsi->dwSkillInfoFlag ^= DSI_COLD;
			lpDsi->dwSkillInfoFlag ^= DSI_TARGETABLE;
			lpDsi->dwSkillInfoFlag ^= DSI_PHYSICAL;	
			lpDsi->dwSkillInfoFlag ^= DSI_AFFECTGROUP;
			
			::strncpy(lpDsi->szSkillName, "Freezing Arrow", MAX_SKILLNAME_LEN);
			break;

		case D2S_MAGICARROW:
			
			lpDsi->dwSkillInfoFlag ^= DSI_TARGETABLE;
			lpDsi->dwSkillInfoFlag ^= DSI_PHYSICAL;	
			
			::strncpy(lpDsi->szSkillName, "Magic Arrow", MAX_SKILLNAME_LEN);
			break;

		case D2S_MULTIPLESHOT:
			
			lpDsi->dwSkillInfoFlag ^= DSI_TARGETABLE;
			lpDsi->dwSkillInfoFlag ^= DSI_PHYSICAL;	
			lpDsi->dwSkillInfoFlag ^= DSI_AFFECTGROUP;
			
			::strncpy(lpDsi->szSkillName, "Multiple Shot", MAX_SKILLNAME_LEN);
			break;

		case D2S_GUIDEDARROW:
			
			lpDsi->dwSkillInfoFlag ^= DSI_TARGETABLE;
			lpDsi->dwSkillInfoFlag ^= DSI_PHYSICAL;	
			
			::strncpy(lpDsi->szSkillName, "Guided Arrow", MAX_SKILLNAME_LEN);
			break;

		case D2S_STRAFE:
			
			lpDsi->dwSkillInfoFlag ^= DSI_TARGETABLE;
			lpDsi->dwSkillInfoFlag ^= DSI_PHYSICAL;	
			lpDsi->dwSkillInfoFlag ^= DSI_AFFECTGROUP;
			
			::strncpy(lpDsi->szSkillName, "Strafe", MAX_SKILLNAME_LEN);
			break;

		case D2S_FIREARROW:
			
			lpDsi->dwSkillInfoFlag ^= DSI_FIRE;
			lpDsi->dwSkillInfoFlag ^= DSI_TARGETABLE;
			
			::strncpy(lpDsi->szSkillName, "Fire Arrow", MAX_SKILLNAME_LEN);
			break;

		case D2S_EXPLODINGARROW:
			
			lpDsi->dwSkillInfoFlag ^= DSI_FIRE;
			lpDsi->dwSkillInfoFlag ^= DSI_TARGETABLE;
			lpDsi->dwSkillInfoFlag ^= DSI_AFFECTGROUP;
			
			::strncpy(lpDsi->szSkillName, "Exploding Arrow", MAX_SKILLNAME_LEN);
			break;

		case D2S_IMMOLATIONARROW:
			lpDsi->dwCoolDown = 1000;
			lpDsi->dwSkillInfoFlag ^= DSI_FIRE;
			lpDsi->dwSkillInfoFlag ^= DSI_TARGETABLE;
			lpDsi->dwSkillInfoFlag ^= DSI_AFFECTGROUP;
			
			::strncpy(lpDsi->szSkillName, "Immolation Arrow", MAX_SKILLNAME_LEN);
			break;


///////////////////////////////////////////////////////////////
//    Assassin Skills
///////////////////////////////////////////////////////////////

// Martial Arts
		case D2S_FISTSOFFIRE:
			
			//lpDsi->dwSkillInfoFlag ^= DSI_FIRE;
			lpDsi->dwSkillInfoFlag ^= DSI_TARGETABLE;
			lpDsi->dwSkillInfoFlag ^= DSI_PHYSICAL;	
			//lpDsi->dwSkillInfoFlag ^= DSI_AFFECTGROUP;
			lpDsi->dwSkillInfoFlag ^= DSI_MELEE;
			
			::strncpy(lpDsi->szSkillName, "Fists of Fire", MAX_SKILLNAME_LEN);
			break;

		case D2S_CLAWSOFTHUNDER:
			
			//lpDsi->dwSkillInfoFlag ^= DSI_LIGHTNING;
			lpDsi->dwSkillInfoFlag ^= DSI_TARGETABLE;
			lpDsi->dwSkillInfoFlag ^= DSI_PHYSICAL;	
			//lpDsi->dwSkillInfoFlag ^= DSI_AFFECTGROUP;
			lpDsi->dwSkillInfoFlag ^= DSI_MELEE;
			
			::strncpy(lpDsi->szSkillName, "Claws of Thunder", MAX_SKILLNAME_LEN);
			break;

		case D2S_BLADESOFICE:
			
			//lpDsi->dwSkillInfoFlag ^= DSI_COLD;
			lpDsi->dwSkillInfoFlag ^= DSI_TARGETABLE;
			lpDsi->dwSkillInfoFlag ^= DSI_PHYSICAL;	
			//lpDsi->dwSkillInfoFlag ^= DSI_AFFECTGROUP;
			lpDsi->dwSkillInfoFlag ^= DSI_MELEE;
			
			::strncpy(lpDsi->szSkillName, "Blades of Ice", MAX_SKILLNAME_LEN);
			break;

		case D2S_TIGERSTRIKE:
			
			lpDsi->dwSkillInfoFlag ^= DSI_TARGETABLE;
			lpDsi->dwSkillInfoFlag ^= DSI_PHYSICAL;	
			lpDsi->dwSkillInfoFlag ^= DSI_MELEE;
			
			::strncpy(lpDsi->szSkillName, "Tiger Strike", MAX_SKILLNAME_LEN);
			break;

		case D2S_COBRASTRIKE:

			lpDsi->dwSkillInfoFlag ^= DSI_TARGETABLE;
			lpDsi->dwSkillInfoFlag ^= DSI_PHYSICAL;	
			lpDsi->dwSkillInfoFlag ^= DSI_MELEE;
			
			::strncpy(lpDsi->szSkillName, "Cobra Strike", MAX_SKILLNAME_LEN);
			break;

		case D2S_PHOENIXSTRIKE:
			
			lpDsi->dwSkillInfoFlag ^= DSI_TARGETABLE;
			lpDsi->dwSkillInfoFlag ^= DSI_PHYSICAL;	
			lpDsi->dwSkillInfoFlag ^= DSI_MELEE;
			
			::strncpy(lpDsi->szSkillName, "Phoenix Strike", MAX_SKILLNAME_LEN);
			break;

		case D2S_DRAGONTALON:
			
			lpDsi->dwSkillInfoFlag ^= DSI_TARGETABLE;
			lpDsi->dwSkillInfoFlag ^= DSI_PHYSICAL;	
			lpDsi->dwSkillInfoFlag ^= DSI_MELEE;
			
			::strncpy(lpDsi->szSkillName, "Dragon Talon", MAX_SKILLNAME_LEN);
			break;

		case D2S_DRAGONCLAW:
			
			lpDsi->dwSkillInfoFlag ^= DSI_TARGETABLE;
			lpDsi->dwSkillInfoFlag ^= DSI_PHYSICAL;	
			lpDsi->dwSkillInfoFlag ^= DSI_MELEE;
			
			::strncpy(lpDsi->szSkillName, "Dragon Claw", MAX_SKILLNAME_LEN);
			break;

		case D2S_DRAGONTAIL:
			
			lpDsi->dwSkillInfoFlag ^= DSI_FIRE;
			lpDsi->dwSkillInfoFlag ^= DSI_TARGETABLE;
			lpDsi->dwSkillInfoFlag ^= DSI_PHYSICAL;	
			lpDsi->dwSkillInfoFlag ^= DSI_MELEE;
			lpDsi->dwSkillInfoFlag ^= DSI_AFFECTGROUP;
			
			::strncpy(lpDsi->szSkillName, "Dragon Tail", MAX_SKILLNAME_LEN);
			break;

		case D2S_DRAGONFLIGHT:
			lpDsi->dwCoolDown = 2000;
			lpDsi->dwSkillInfoFlag ^= DSI_TARGETABLE;
			lpDsi->dwSkillInfoFlag ^= DSI_PHYSICAL;	
			//lpDsi->dwSkillInfoFlag ^= DSI_MELEE;
			
			::strncpy(lpDsi->szSkillName, "Dragon Flight", MAX_SKILLNAME_LEN);
			break;


// Shadow Disciplines
		case D2S_BURSTOFSPEED:			
			
			lpDsi->dwSkillInfoFlag ^= DSI_RIGHTONLY;			
			lpDsi->dwSkillInfoFlag ^= DSI_ENABLEINTOWN;

			::strncpy(lpDsi->szSkillName, "Burst of Speed", MAX_SKILLNAME_LEN);
			break;

		case D2S_FADE:			
			
			lpDsi->dwSkillInfoFlag ^= DSI_RIGHTONLY;			
			lpDsi->dwSkillInfoFlag ^= DSI_ENABLEINTOWN;

			::strncpy(lpDsi->szSkillName, "Fade", MAX_SKILLNAME_LEN);
			break;

		case D2S_VENOM:
						
			lpDsi->dwSkillInfoFlag ^= DSI_RIGHTONLY;
			lpDsi->dwSkillInfoFlag ^= DSI_ENABLEINTOWN;

			::strncpy(lpDsi->szSkillName, "Venom", MAX_SKILLNAME_LEN);
			break;

		case D2S_CLAWMASTERY:
			
			lpDsi->dwSkillInfoFlag ^= DSI_PASSIVE;
	
			::strncpy(lpDsi->szSkillName, "Claw Mastery", MAX_SKILLNAME_LEN);
			break;

		case D2S_WEAPONBLOCK:
			
			lpDsi->dwSkillInfoFlag ^= DSI_PASSIVE;
			
			::strncpy(lpDsi->szSkillName, "Weapon Block", MAX_SKILLNAME_LEN);
			break;

		case D2S_SHADOWWARRIOR:
			
			lpDsi->dwSkillInfoFlag ^= DSI_RIGHTONLY;
			lpDsi->dwSkillInfoFlag ^= DSI_TARGETABLE;			
			lpDsi->dwSkillInfoFlag ^= DSI_ENABLEINTOWN;
			lpDsi->dwSkillInfoFlag ^= DSI_SUMMON;

			::strncpy(lpDsi->szSkillName, "Shadow Warrior", MAX_SKILLNAME_LEN);
			break;

		case D2S_SHADOWMASTER:
			
			lpDsi->dwSkillInfoFlag ^= DSI_RIGHTONLY;
			lpDsi->dwSkillInfoFlag ^= DSI_TARGETABLE;			
			lpDsi->dwSkillInfoFlag ^= DSI_ENABLEINTOWN;
			lpDsi->dwSkillInfoFlag ^= DSI_SUMMON;
			
			::strncpy(lpDsi->szSkillName, "Shadow Master", MAX_SKILLNAME_LEN);
			break;

		case D2S_PSYCHICHAMMER:			
			
			lpDsi->dwSkillInfoFlag ^= DSI_TARGETABLE;
			lpDsi->dwSkillInfoFlag ^= DSI_PHYSICAL;
			lpDsi->dwSkillInfoFlag ^= DSI_KNOCKBACK;
			
			::strncpy(lpDsi->szSkillName, "Psychic Hammer", MAX_SKILLNAME_LEN);
			break;

		case D2S_CLOAKOFSHADOWS:			
			
			lpDsi->dwSkillInfoFlag ^= DSI_RIGHTONLY;
			lpDsi->dwSkillInfoFlag ^= DSI_AFFECTGROUP;
			
			::strncpy(lpDsi->szSkillName, "Cloak of Shadows", MAX_SKILLNAME_LEN);
			break;

		case D2S_MINDBLAST:			
			
			lpDsi->dwSkillInfoFlag ^= DSI_RIGHTONLY;
			lpDsi->dwSkillInfoFlag ^= DSI_TARGETABLE;
			lpDsi->dwSkillInfoFlag ^= DSI_PHYSICAL;
			lpDsi->dwSkillInfoFlag ^= DSI_KNOCKBACK;
			lpDsi->dwSkillInfoFlag ^= DSI_STUN;
			lpDsi->dwSkillInfoFlag ^= DSI_AFFECTGROUP;
			
			::strncpy(lpDsi->szSkillName, "Mind Blast", MAX_SKILLNAME_LEN);
			break;

// Traps
		case D2S_SHOCKWEB:			
			
			lpDsi->dwSkillInfoFlag ^= DSI_TARGETABLE;
			lpDsi->dwSkillInfoFlag ^= DSI_LIGHTNING;			
			
			::strncpy(lpDsi->szSkillName, "Shock Web", MAX_SKILLNAME_LEN);
			break;

		case D2S_CHARGEDBOLTSENTRY:
			
			
			lpDsi->dwSkillInfoFlag ^= DSI_RIGHTONLY;
			lpDsi->dwSkillInfoFlag ^= DSI_TARGETABLE;
			lpDsi->dwSkillInfoFlag ^= DSI_LIGHTNING;
			lpDsi->dwSkillInfoFlag ^= DSI_AFFECTGROUP;
			
			::strncpy(lpDsi->szSkillName, "Charged Bolt Sentry", MAX_SKILLNAME_LEN);
			break;

		case D2S_LIGHTNINGSENTRY:
			
			lpDsi->dwSkillInfoFlag ^= DSI_RIGHTONLY;
			lpDsi->dwSkillInfoFlag ^= DSI_TARGETABLE;
			lpDsi->dwSkillInfoFlag ^= DSI_LIGHTNING;
			lpDsi->dwSkillInfoFlag ^= DSI_AFFECTGROUP;
			
			::strncpy(lpDsi->szSkillName, "Lightning Sentry", MAX_SKILLNAME_LEN);
			break;

		case D2S_DEATHSENTRY:
			
			lpDsi->dwSkillInfoFlag ^= DSI_RIGHTONLY;
			lpDsi->dwSkillInfoFlag ^= DSI_TARGETABLE;
			lpDsi->dwSkillInfoFlag ^= DSI_LIGHTNING;
			lpDsi->dwSkillInfoFlag ^= DSI_AFFECTGROUP;
			lpDsi->dwSkillInfoFlag ^= DSI_FIRE;
			lpDsi->dwSkillInfoFlag ^= DSI_PHYSICAL;
			
			::strncpy(lpDsi->szSkillName, "Death Sentry", MAX_SKILLNAME_LEN);
			break;

		case D2S_FIREBLAST:			
			
			lpDsi->dwSkillInfoFlag ^= DSI_TARGETABLE;
			lpDsi->dwSkillInfoFlag ^= DSI_FIRE;
			lpDsi->dwSkillInfoFlag ^= DSI_AFFECTGROUP;
			
			::strncpy(lpDsi->szSkillName, "Fire Blast", MAX_SKILLNAME_LEN);
			break;

		case D2S_WAKEOFFIRE:
			
			lpDsi->dwSkillInfoFlag ^= DSI_RIGHTONLY;
			lpDsi->dwSkillInfoFlag ^= DSI_TARGETABLE;
			lpDsi->dwSkillInfoFlag ^= DSI_FIRE;
			lpDsi->dwSkillInfoFlag ^= DSI_AFFECTGROUP;
			
			::strncpy(lpDsi->szSkillName, "Wake of Fire", MAX_SKILLNAME_LEN);
			break;

		case D2S_WAKEOFINFERNO:
			
			lpDsi->dwSkillInfoFlag ^= DSI_RIGHTONLY;
			lpDsi->dwSkillInfoFlag ^= DSI_TARGETABLE;
			lpDsi->dwSkillInfoFlag ^= DSI_FIRE;
			lpDsi->dwSkillInfoFlag ^= DSI_AFFECTGROUP;
			
			::strncpy(lpDsi->szSkillName, "Wake of Inferno", MAX_SKILLNAME_LEN);
			break;

		case D2S_BLADESENTINEL:			
			
			lpDsi->dwSkillInfoFlag ^= DSI_TARGETABLE;
			lpDsi->dwSkillInfoFlag ^= DSI_PHYSICAL;			
			
			::strncpy(lpDsi->szSkillName, "Blade Sentinel", MAX_SKILLNAME_LEN);
			break;

		case D2S_BLADEFURY:
			
			lpDsi->dwSkillInfoFlag ^= DSI_TARGETABLE;
			lpDsi->dwSkillInfoFlag ^= DSI_PHYSICAL;	
			
			::strncpy(lpDsi->szSkillName, "Blade Fury", MAX_SKILLNAME_LEN);
			break;

		case D2S_BLADESHIELD:			
			
			lpDsi->dwSkillInfoFlag ^= DSI_RIGHTONLY;
			lpDsi->dwSkillInfoFlag ^= DSI_MELEE;
			lpDsi->dwSkillInfoFlag ^= DSI_PHYSICAL;				
			lpDsi->dwSkillInfoFlag ^= DSI_ENABLEINTOWN;

			::strncpy(lpDsi->szSkillName, "Blade Shield", MAX_SKILLNAME_LEN);
			break;


///////////////////////////////////////////////////////////////
//    Barbarian Skills
///////////////////////////////////////////////////////////////

// Warcries
		case D2S_HOWL:			
			
			lpDsi->dwSkillInfoFlag ^= DSI_RIGHTONLY;			
			
			::strncpy(lpDsi->szSkillName, "Howl", MAX_SKILLNAME_LEN);
			break;

		case D2S_TAUNT:			
			
			lpDsi->dwSkillInfoFlag ^= DSI_RIGHTONLY;
			lpDsi->dwSkillInfoFlag ^= DSI_TARGETABLE;			
			
			::strncpy(lpDsi->szSkillName, "Taunt", MAX_SKILLNAME_LEN);
			break;

		case D2S_BATTLECRY:			
			
			lpDsi->dwSkillInfoFlag ^= DSI_RIGHTONLY;
			
			::strncpy(lpDsi->szSkillName, "Battle Cry", MAX_SKILLNAME_LEN);
			break;

		case D2S_WARCRY:			
			
			lpDsi->dwSkillInfoFlag ^= DSI_RIGHTONLY;
			lpDsi->dwSkillInfoFlag ^= DSI_PHYSICAL;
			lpDsi->dwSkillInfoFlag ^= DSI_AFFECTGROUP;
			lpDsi->dwSkillInfoFlag ^= DSI_STUN;

			::strncpy(lpDsi->szSkillName, "War Cry", MAX_SKILLNAME_LEN);
			break;

		case D2S_SHOUT:			
			
			lpDsi->dwSkillInfoFlag ^= DSI_RIGHTONLY;
			
			::strncpy(lpDsi->szSkillName, "Shout", MAX_SKILLNAME_LEN);
			break;

		case D2S_BATTLEORDERS:			
			
			lpDsi->dwSkillInfoFlag ^= DSI_RIGHTONLY;
			
			::strncpy(lpDsi->szSkillName, "Battle Orders", MAX_SKILLNAME_LEN);
			break;

		case D2S_BATTLECOMMAND:
			
			lpDsi->dwSkillInfoFlag ^= DSI_RIGHTONLY;
			
			::strncpy(lpDsi->szSkillName, "Battle Command", MAX_SKILLNAME_LEN);
			break;

		case D2S_FINDPOTION:
			
			lpDsi->dwSkillInfoFlag ^= DSI_RIGHTONLY;
			lpDsi->dwSkillInfoFlag ^= DSI_TARGETABLE;
			lpDsi->dwSkillInfoFlag ^= DSI_NEEDCORPSE;
			
			::strncpy(lpDsi->szSkillName, "Find Potion", MAX_SKILLNAME_LEN);
			break;

		case D2S_FINDITEM:
			
			lpDsi->dwSkillInfoFlag ^= DSI_RIGHTONLY;
			lpDsi->dwSkillInfoFlag ^= DSI_TARGETABLE;
			lpDsi->dwSkillInfoFlag ^= DSI_NEEDCORPSE;
			
			::strncpy(lpDsi->szSkillName, "Find Item", MAX_SKILLNAME_LEN);
			break;

		case D2S_GRIMWARD:
			
			lpDsi->dwSkillInfoFlag ^= DSI_RIGHTONLY;
			lpDsi->dwSkillInfoFlag ^= DSI_TARGETABLE;
			lpDsi->dwSkillInfoFlag ^= DSI_NEEDCORPSE;
			
			::strncpy(lpDsi->szSkillName, "Grim Ward", MAX_SKILLNAME_LEN);
			break;


// Combat Masteries
		case D2S_SWORDMASTERY:
			
			lpDsi->dwSkillInfoFlag ^= DSI_PASSIVE;
			
			::strncpy(lpDsi->szSkillName, "Sword Mastery", MAX_SKILLNAME_LEN);
			break;

		case D2S_POLEARMMASTERY:
			
			lpDsi->dwSkillInfoFlag ^= DSI_PASSIVE;
			
			::strncpy(lpDsi->szSkillName, "Pole Arm Mastery", MAX_SKILLNAME_LEN);
			break;

		case D2S_INCREASEDSTAMINA:
			
			lpDsi->dwSkillInfoFlag ^= DSI_PASSIVE;
			
			::strncpy(lpDsi->szSkillName, "Increased Stamin", MAX_SKILLNAME_LEN);
			break;

		case D2S_INCREASEDSPEED:
			
			lpDsi->dwSkillInfoFlag ^= DSI_PASSIVE;
			
			::strncpy(lpDsi->szSkillName, "Increased Speed", MAX_SKILLNAME_LEN);
			break;

		case D2S_AXEMASTERY:
			
			lpDsi->dwSkillInfoFlag ^= DSI_PASSIVE;
			
			::strncpy(lpDsi->szSkillName, "Axe Mastery", MAX_SKILLNAME_LEN);
			break;

		case D2S_THROWINGMASTERY:
			
			lpDsi->dwSkillInfoFlag ^= DSI_PASSIVE;
			
			::strncpy(lpDsi->szSkillName, "Throwing Mastery", MAX_SKILLNAME_LEN);
			break;

		case D2S_MACEMASTERY:
			
			lpDsi->dwSkillInfoFlag ^= DSI_PASSIVE;

			::strncpy(lpDsi->szSkillName, "Mace Mastery", MAX_SKILLNAME_LEN);
			break;

		case D2S_SPEARMASTERY:
			
			lpDsi->dwSkillInfoFlag ^= DSI_PASSIVE;
			
			::strncpy(lpDsi->szSkillName, "Spear Mastery", MAX_SKILLNAME_LEN);
			break;

		case D2S_IRONSKIN:
			
			lpDsi->dwSkillInfoFlag ^= DSI_PASSIVE;
			
			::strncpy(lpDsi->szSkillName, "Iron Skin", MAX_SKILLNAME_LEN);
			break;

		case D2S_NATURALRESISTANCE:
			
			lpDsi->dwSkillInfoFlag ^= DSI_PASSIVE;
			
			::strncpy(lpDsi->szSkillName, "Natural Resistance", MAX_SKILLNAME_LEN);
			break;

// Combat Skills
		case D2S_LEAP:
			
			lpDsi->dwSkillInfoFlag ^= DSI_TARGETABLE;
			lpDsi->dwSkillInfoFlag ^= DSI_AFFECTGROUP;
			lpDsi->dwSkillInfoFlag ^= DSI_KNOCKBACK;	
			
			::strncpy(lpDsi->szSkillName, "Leap", MAX_SKILLNAME_LEN);
			break;

		case D2S_LEAPATTACK:
			
			lpDsi->dwSkillInfoFlag ^= DSI_TARGETABLE;
			lpDsi->dwSkillInfoFlag ^= DSI_PHYSICAL;
			lpDsi->dwSkillInfoFlag ^= DSI_KNOCKBACK;
			
			::strncpy(lpDsi->szSkillName, "Leap Attack", MAX_SKILLNAME_LEN);
			break;

		case D2S_WHIRLWIND:
			
			lpDsi->dwSkillInfoFlag ^= DSI_TARGETABLE;
			lpDsi->dwSkillInfoFlag ^= DSI_PHYSICAL;
			lpDsi->dwSkillInfoFlag ^= DSI_AFFECTGROUP;
			
			::strncpy(lpDsi->szSkillName, "Whirlwind", MAX_SKILLNAME_LEN);
			break;

		case D2S_BASH:
			
			lpDsi->dwSkillInfoFlag ^= DSI_TARGETABLE;
			lpDsi->dwSkillInfoFlag ^= DSI_PHYSICAL;
			lpDsi->dwSkillInfoFlag ^= DSI_KNOCKBACK;
			lpDsi->dwSkillInfoFlag ^= DSI_MELEE;
			
			::strncpy(lpDsi->szSkillName, "Bash", MAX_SKILLNAME_LEN);
			break;

		case D2S_STUN:
			
			lpDsi->dwSkillInfoFlag ^= DSI_TARGETABLE;
			lpDsi->dwSkillInfoFlag ^= DSI_PHYSICAL;
			lpDsi->dwSkillInfoFlag ^= DSI_STUN;
			lpDsi->dwSkillInfoFlag ^= DSI_MELEE;
			
			::strncpy(lpDsi->szSkillName, "Stun", MAX_SKILLNAME_LEN);
			break;

		case D2S_CONCENTRATE:
			
			lpDsi->dwSkillInfoFlag ^= DSI_TARGETABLE;
			lpDsi->dwSkillInfoFlag ^= DSI_PHYSICAL;
			lpDsi->dwSkillInfoFlag ^= DSI_MELEE;
			
			::strncpy(lpDsi->szSkillName, "Concentrate", MAX_SKILLNAME_LEN);
			break;

		case D2S_BERSERK:
			
			lpDsi->dwSkillInfoFlag ^= DSI_TARGETABLE;
			lpDsi->dwSkillInfoFlag ^= DSI_MAGICAL;
			lpDsi->dwSkillInfoFlag ^= DSI_MELEE;
			
			::strncpy(lpDsi->szSkillName, "Berserk", MAX_SKILLNAME_LEN);
			break;

		case D2S_DOUBLESWING:
			
			lpDsi->dwSkillInfoFlag ^= DSI_TARGETABLE;
			lpDsi->dwSkillInfoFlag ^= DSI_MELEE;
			lpDsi->dwSkillInfoFlag ^= DSI_AFFECTGROUP;
			lpDsi->dwSkillInfoFlag ^= DSI_PHYSICAL;
			
			::strncpy(lpDsi->szSkillName, "Double Swing", MAX_SKILLNAME_LEN);
			break;

		case D2S_DOUBLETHROW:			
			
			lpDsi->dwSkillInfoFlag ^= DSI_TARGETABLE;
			lpDsi->dwSkillInfoFlag ^= DSI_AFFECTGROUP;
			lpDsi->dwSkillInfoFlag ^= DSI_PHYSICAL;
			
			::strncpy(lpDsi->szSkillName, "Double Throw", MAX_SKILLNAME_LEN);
			break;

		case D2S_FRENZY:
			
			lpDsi->dwSkillInfoFlag ^= DSI_TARGETABLE;
			lpDsi->dwSkillInfoFlag ^= DSI_MELEE;
			lpDsi->dwSkillInfoFlag ^= DSI_AFFECTGROUP;
			lpDsi->dwSkillInfoFlag ^= DSI_PHYSICAL;
			
			::strncpy(lpDsi->szSkillName, "Frenzy", MAX_SKILLNAME_LEN);
			break;


///////////////////////////////////////////////////////////////
//    Druid Skills
///////////////////////////////////////////////////////////////

// Elemental
		case D2S_FIRESTORM:
			
			lpDsi->dwSkillInfoFlag ^= DSI_RIGHTONLY;
			lpDsi->dwSkillInfoFlag ^= DSI_TARGETABLE;
			lpDsi->dwSkillInfoFlag ^= DSI_FIRE;
			lpDsi->dwSkillInfoFlag ^= DSI_AFFECTGROUP;			
			
			::strncpy(lpDsi->szSkillName, "Firestorm", MAX_SKILLNAME_LEN);
			break;

		case D2S_MOLTENBOULDER:
			
			lpDsi->dwSkillInfoFlag ^= DSI_KNOCKBACK;
			lpDsi->dwSkillInfoFlag ^= DSI_TARGETABLE;
			lpDsi->dwSkillInfoFlag ^= DSI_FIRE;
			lpDsi->dwSkillInfoFlag ^= DSI_AFFECTGROUP;
			lpDsi->dwCoolDown = 2000;
			
			::strncpy(lpDsi->szSkillName, "Molten Boulder", MAX_SKILLNAME_LEN);
			break;

		case D2S_FISSURE:			
			
			lpDsi->dwSkillInfoFlag ^= DSI_TARGETABLE;
			lpDsi->dwSkillInfoFlag ^= DSI_FIRE;
			lpDsi->dwSkillInfoFlag ^= DSI_AFFECTGROUP;
			lpDsi->dwSkillInfoFlag ^= DSI_RIGHTONLY;
			lpDsi->dwCoolDown = 2000;
			
			::strncpy(lpDsi->szSkillName, "Fissure", MAX_SKILLNAME_LEN);
			break;

		case D2S_VOLCANO:
			
			lpDsi->dwSkillInfoFlag ^= DSI_TARGETABLE;
			lpDsi->dwSkillInfoFlag ^= DSI_FIRE;
			lpDsi->dwSkillInfoFlag ^= DSI_AFFECTGROUP;
			lpDsi->dwSkillInfoFlag ^= DSI_RIGHTONLY;
			lpDsi->dwCoolDown = 4000;
			
			::strncpy(lpDsi->szSkillName, "Volcano", MAX_SKILLNAME_LEN);
			break;

		case D2S_ARMAGEDDON:
			
			lpDsi->dwSkillInfoFlag ^= DSI_FIRE;
			lpDsi->dwSkillInfoFlag ^= DSI_AFFECTGROUP;
			lpDsi->dwSkillInfoFlag ^= DSI_RIGHTONLY;
			lpDsi->dwCoolDown = 6000;
			
			::strncpy(lpDsi->szSkillName, "Armageddon", MAX_SKILLNAME_LEN);
			break;

		case D2S_TWISTER:
			
			lpDsi->dwSkillInfoFlag ^= DSI_PHYSICAL;
			lpDsi->dwSkillInfoFlag ^= DSI_AFFECTGROUP;
			lpDsi->dwSkillInfoFlag ^= DSI_TARGETABLE;
			lpDsi->dwSkillInfoFlag ^= DSI_STUN;
			
			::strncpy(lpDsi->szSkillName, "Twister", MAX_SKILLNAME_LEN);
			break;

		case D2S_TORNADO:
			
			lpDsi->dwSkillInfoFlag ^= DSI_PHYSICAL;
			lpDsi->dwSkillInfoFlag ^= DSI_AFFECTGROUP;
			lpDsi->dwSkillInfoFlag ^= DSI_TARGETABLE;
			
			::strncpy(lpDsi->szSkillName, "Tornado", MAX_SKILLNAME_LEN);
			break;

		case D2S_HURRICANE:
			
			lpDsi->dwSkillInfoFlag ^= DSI_COLD;
			lpDsi->dwSkillInfoFlag ^= DSI_AFFECTGROUP;
			lpDsi->dwSkillInfoFlag ^= DSI_RIGHTONLY;
			lpDsi->dwCoolDown = 6000;
			
			::strncpy(lpDsi->szSkillName, "Hurricane", MAX_SKILLNAME_LEN);
			break;

		case D2S_ARCTICBLAST:
			
			lpDsi->dwSkillInfoFlag ^= DSI_COLD;
			lpDsi->dwSkillInfoFlag ^= DSI_AFFECTGROUP;
			lpDsi->dwSkillInfoFlag ^= DSI_RIGHTONLY;
			lpDsi->dwSkillInfoFlag ^= DSI_TARGETABLE;
			
			::strncpy(lpDsi->szSkillName, "Arctic Blast", MAX_SKILLNAME_LEN);
			break;

		case D2S_CYCLONEARMOR:
			
			lpDsi->dwSkillInfoFlag ^= DSI_RIGHTONLY;
			lpDsi->dwSkillInfoFlag ^= DSI_ENABLEINTOWN;
			
			::strncpy(lpDsi->szSkillName, "Cyclone Armor", MAX_SKILLNAME_LEN);
			break;


// Shape Shifting
		case D2S_WEREWOLF:			
			
			lpDsi->dwSkillInfoFlag ^= DSI_RIGHTONLY;
			lpDsi->dwSkillInfoFlag ^= DSI_ENABLEINTOWN;	
			lpDsi->dwCoolDown = 1000;
			
			::strncpy(lpDsi->szSkillName, "Werewolf", MAX_SKILLNAME_LEN);
			break;

		case D2S_FERALRAGE:			
			
			lpDsi->dwSkillInfoFlag ^= DSI_TARGETABLE;
			lpDsi->dwSkillInfoFlag ^= DSI_PHYSICAL;	
			lpDsi->dwSkillInfoFlag ^= DSI_MELEE;
			
			::strncpy(lpDsi->szSkillName, "Feral Rage", MAX_SKILLNAME_LEN);
			break;

		case D2S_RABIES:
			
			lpDsi->dwSkillInfoFlag ^= DSI_TARGETABLE;
			lpDsi->dwSkillInfoFlag ^= DSI_POISON;	
			lpDsi->dwSkillInfoFlag ^= DSI_MELEE;
			lpDsi->dwSkillInfoFlag ^= DSI_AFFECTGROUP;
			
			::strncpy(lpDsi->szSkillName, "Rabies", MAX_SKILLNAME_LEN);
			break;

		case D2S_FURY:			
			
			lpDsi->dwSkillInfoFlag ^= DSI_TARGETABLE;
			lpDsi->dwSkillInfoFlag ^= DSI_PHYSICAL;	
			lpDsi->dwSkillInfoFlag ^= DSI_MELEE;
			lpDsi->dwSkillInfoFlag ^= DSI_AFFECTGROUP;
			
			::strncpy(lpDsi->szSkillName, "Fury", MAX_SKILLNAME_LEN);
			break;

		case D2S_LYCANTHROPY:
			
			lpDsi->dwSkillInfoFlag ^= DSI_PASSIVE;
			
			::strncpy(lpDsi->szSkillName, "Lycanthropy", MAX_SKILLNAME_LEN);
			break;

		case D2S_FIRECLAWS:			
			
			lpDsi->dwSkillInfoFlag ^= DSI_FIRE;
			lpDsi->dwSkillInfoFlag ^= DSI_TARGETABLE;
			lpDsi->dwSkillInfoFlag ^= DSI_PHYSICAL;	
			lpDsi->dwSkillInfoFlag ^= DSI_MELEE;
			
			::strncpy(lpDsi->szSkillName, "Fire Claws", MAX_SKILLNAME_LEN);
			break;

		case D2S_HUNGER:
			
			lpDsi->dwSkillInfoFlag ^= DSI_TARGETABLE;
			lpDsi->dwSkillInfoFlag ^= DSI_PHYSICAL;	
			lpDsi->dwSkillInfoFlag ^= DSI_MELEE;
			
			::strncpy(lpDsi->szSkillName, "Hunger", MAX_SKILLNAME_LEN);
			break;

		case D2S_WEREBEAR:			
			
			lpDsi->dwSkillInfoFlag ^= DSI_RIGHTONLY;
			lpDsi->dwSkillInfoFlag ^= DSI_ENABLEINTOWN;	
			lpDsi->dwCoolDown = 1000;		
			
			::strncpy(lpDsi->szSkillName, "Werebear", MAX_SKILLNAME_LEN);
			break;

		case D2S_MAUL:
			
			lpDsi->dwSkillInfoFlag ^= DSI_TARGETABLE;
			lpDsi->dwSkillInfoFlag ^= DSI_PHYSICAL;	
			lpDsi->dwSkillInfoFlag ^= DSI_MELEE;
			lpDsi->dwSkillInfoFlag ^= DSI_STUN;
			
			
			::strncpy(lpDsi->szSkillName, "Maul", MAX_SKILLNAME_LEN);
			break;

		case D2S_SHOCKWAVE:
			
			lpDsi->dwSkillInfoFlag ^= DSI_TARGETABLE;
			lpDsi->dwSkillInfoFlag ^= DSI_PHYSICAL;	
			lpDsi->dwSkillInfoFlag ^= DSI_AFFECTGROUP;
			lpDsi->dwSkillInfoFlag ^= DSI_STUN;
			
			::strncpy(lpDsi->szSkillName, "Shock Wave", MAX_SKILLNAME_LEN);
			break;


// Summoning
		case D2S_OAKSAGE:
			
			lpDsi->dwSkillInfoFlag ^= DSI_RIGHTONLY;
			lpDsi->dwSkillInfoFlag ^= DSI_TARGETABLE;
			lpDsi->dwSkillInfoFlag ^= DSI_ENABLEINTOWN;
			lpDsi->dwSkillInfoFlag ^= DSI_SUMMON;
			
			::strncpy(lpDsi->szSkillName, "Oak Sage", MAX_SKILLNAME_LEN);
			break;

		case D2S_HEARTOFWOLVERINE:
			
			lpDsi->dwSkillInfoFlag ^= DSI_RIGHTONLY;
			lpDsi->dwSkillInfoFlag ^= DSI_TARGETABLE;
			lpDsi->dwSkillInfoFlag ^= DSI_ENABLEINTOWN;
			lpDsi->dwSkillInfoFlag ^= DSI_SUMMON;
			
			::strncpy(lpDsi->szSkillName, "Heart of Wolverine", MAX_SKILLNAME_LEN);
			break;

		case D2S_SPIRITOFBARBS:
			
			lpDsi->dwSkillInfoFlag ^= DSI_RIGHTONLY;
			lpDsi->dwSkillInfoFlag ^= DSI_TARGETABLE;
			lpDsi->dwSkillInfoFlag ^= DSI_ENABLEINTOWN;
			lpDsi->dwSkillInfoFlag ^= DSI_SUMMON;
			
			::strncpy(lpDsi->szSkillName, "Spirit of Barbs", MAX_SKILLNAME_LEN);
			break;

		case D2S_RAVEN:
			
			lpDsi->dwSkillInfoFlag ^= DSI_RIGHTONLY;
			lpDsi->dwSkillInfoFlag ^= DSI_TARGETABLE;
			lpDsi->dwSkillInfoFlag ^= DSI_ENABLEINTOWN;
			lpDsi->dwSkillInfoFlag ^= DSI_SUMMON;
			
			::strncpy(lpDsi->szSkillName, "Raven", MAX_SKILLNAME_LEN);
			break;

		case D2S_SUMMONSPIRITWOLF:
			
			lpDsi->dwSkillInfoFlag ^= DSI_RIGHTONLY;
			lpDsi->dwSkillInfoFlag ^= DSI_TARGETABLE;
			lpDsi->dwSkillInfoFlag ^= DSI_ENABLEINTOWN;
			lpDsi->dwSkillInfoFlag ^= DSI_SUMMON;
			
			::strncpy(lpDsi->szSkillName, "Summon Spirit Wolf", MAX_SKILLNAME_LEN);
			break;

		case D2S_SUMMONDIREWOLF:
			
			lpDsi->dwSkillInfoFlag ^= DSI_RIGHTONLY;
			lpDsi->dwSkillInfoFlag ^= DSI_TARGETABLE;
			lpDsi->dwSkillInfoFlag ^= DSI_ENABLEINTOWN;
			lpDsi->dwSkillInfoFlag ^= DSI_SUMMON;
			
			::strncpy(lpDsi->szSkillName, "Summon Dire Wolf", MAX_SKILLNAME_LEN);
			break;

		case D2S_SUMMONGRIZZLY:
			
			lpDsi->dwSkillInfoFlag ^= DSI_RIGHTONLY;
			lpDsi->dwSkillInfoFlag ^= DSI_TARGETABLE;
			lpDsi->dwSkillInfoFlag ^= DSI_ENABLEINTOWN;
			lpDsi->dwSkillInfoFlag ^= DSI_SUMMON;
			
			::strncpy(lpDsi->szSkillName, "Summon Grizzly", MAX_SKILLNAME_LEN);
			break;

		case D2S_POISONCREEPER:
			
			lpDsi->dwSkillInfoFlag ^= DSI_RIGHTONLY;
			lpDsi->dwSkillInfoFlag ^= DSI_TARGETABLE;
			lpDsi->dwSkillInfoFlag ^= DSI_ENABLEINTOWN;
			lpDsi->dwSkillInfoFlag ^= DSI_SUMMON;
			
			::strncpy(lpDsi->szSkillName, "Poison Creeper", MAX_SKILLNAME_LEN);
			break;

		case D2S_CARRIONVINE:
			
			lpDsi->dwSkillInfoFlag ^= DSI_RIGHTONLY;
			lpDsi->dwSkillInfoFlag ^= DSI_TARGETABLE;
			lpDsi->dwSkillInfoFlag ^= DSI_ENABLEINTOWN;
			lpDsi->dwSkillInfoFlag ^= DSI_SUMMON;
			
			::strncpy(lpDsi->szSkillName, "Carrion Vine", MAX_SKILLNAME_LEN);
			break;

		case D2S_SOLARCREEPER:
			
			lpDsi->dwSkillInfoFlag ^= DSI_RIGHTONLY;
			lpDsi->dwSkillInfoFlag ^= DSI_TARGETABLE;
			lpDsi->dwSkillInfoFlag ^= DSI_ENABLEINTOWN;
			lpDsi->dwSkillInfoFlag ^= DSI_SUMMON;
			
			::strncpy(lpDsi->szSkillName, "Solar Creeper", MAX_SKILLNAME_LEN);
			break;


///////////////////////////////////////////////////////////////
//    Necromaner Skills
///////////////////////////////////////////////////////////////

// Summoning Spells
		case D2S_SKELETONMASTERY:
			
			lpDsi->dwSkillInfoFlag ^= DSI_PASSIVE;
			
			::strncpy(lpDsi->szSkillName, "Skeleton Mastery", MAX_SKILLNAME_LEN);
			break;

		case D2S_GOLEMMASTERY:
			
			lpDsi->dwSkillInfoFlag ^= DSI_PASSIVE;
			
			::strncpy(lpDsi->szSkillName, "Golem Mastery", MAX_SKILLNAME_LEN);
			break;

		case D2S_SUMMONRESIST:
			
			lpDsi->dwSkillInfoFlag ^= DSI_PASSIVE;
			
			::strncpy(lpDsi->szSkillName, "Summon Resist", MAX_SKILLNAME_LEN);
			break;

		case D2S_CLAYGOLEM:
			
			lpDsi->dwSkillInfoFlag ^= DSI_RIGHTONLY;
			lpDsi->dwSkillInfoFlag ^= DSI_TARGETABLE;
			lpDsi->dwSkillInfoFlag ^= DSI_ENABLEINTOWN;
			lpDsi->dwSkillInfoFlag ^= DSI_SUMMON;
			
			::strncpy(lpDsi->szSkillName, "Clay Golem", MAX_SKILLNAME_LEN);
			break;

		case D2S_BLOODGOLEM:
			
			lpDsi->dwSkillInfoFlag ^= DSI_RIGHTONLY;
			lpDsi->dwSkillInfoFlag ^= DSI_TARGETABLE;
			lpDsi->dwSkillInfoFlag ^= DSI_ENABLEINTOWN;
			lpDsi->dwSkillInfoFlag ^= DSI_SUMMON;
			
			::strncpy(lpDsi->szSkillName, "Blood Golem", MAX_SKILLNAME_LEN);
			break;

		case D2S_IRONGOLEM:
			
			lpDsi->dwSkillInfoFlag ^= DSI_RIGHTONLY;
			lpDsi->dwSkillInfoFlag ^= DSI_TARGETABLE;
			lpDsi->dwSkillInfoFlag ^= DSI_ENABLEINTOWN;
			lpDsi->dwSkillInfoFlag ^= DSI_SUMMON;
			
			::strncpy(lpDsi->szSkillName, "Iron Golem", MAX_SKILLNAME_LEN);
			break;

		case D2S_FIREGOLEM:
			
			lpDsi->dwSkillInfoFlag ^= DSI_RIGHTONLY;
			lpDsi->dwSkillInfoFlag ^= DSI_TARGETABLE;
			lpDsi->dwSkillInfoFlag ^= DSI_ENABLEINTOWN;
			lpDsi->dwSkillInfoFlag ^= DSI_SUMMON;
			
			::strncpy(lpDsi->szSkillName, "Fire Golem", MAX_SKILLNAME_LEN);
			break;

		case D2S_RAISESKELETON:
			
			lpDsi->dwSkillInfoFlag ^= DSI_RIGHTONLY;
			lpDsi->dwSkillInfoFlag ^= DSI_NEEDCORPSE;
			lpDsi->dwSkillInfoFlag ^= DSI_SUMMON;
			
			::strncpy(lpDsi->szSkillName, "Raise Skeleton", MAX_SKILLNAME_LEN);
			break;

		case D2S_RAISESKELETALMAGE:
			
			lpDsi->dwSkillInfoFlag ^= DSI_RIGHTONLY;
			lpDsi->dwSkillInfoFlag ^= DSI_NEEDCORPSE;
			lpDsi->dwSkillInfoFlag ^= DSI_SUMMON;
			
			::strncpy(lpDsi->szSkillName, "Raise Skeletal Mage", MAX_SKILLNAME_LEN);
			break;

		case D2S_REVIVE:
			
			lpDsi->dwSkillInfoFlag ^= DSI_RIGHTONLY;
			lpDsi->dwSkillInfoFlag ^= DSI_NEEDCORPSE;
			lpDsi->dwSkillInfoFlag ^= DSI_SUMMON;
			
			::strncpy(lpDsi->szSkillName, "Revive", MAX_SKILLNAME_LEN);
			break;


// Poison and Bone Spells
		case D2S_POISONDAGGER:
			
			lpDsi->dwSkillInfoFlag ^= DSI_MELEE;
			lpDsi->dwSkillInfoFlag ^= DSI_TARGETABLE;
			lpDsi->dwSkillInfoFlag ^= DSI_POISON;
			lpDsi->dwSkillInfoFlag ^= DSI_PHYSICAL;
			
			::strncpy(lpDsi->szSkillName, "Poison Dagger", MAX_SKILLNAME_LEN);
			break;

		case D2S_POISONEXPLOSION:
			
			lpDsi->dwSkillInfoFlag ^= DSI_RIGHTONLY;
			lpDsi->dwSkillInfoFlag ^= DSI_NEEDCORPSE;
			lpDsi->dwSkillInfoFlag ^= DSI_POISON;
			lpDsi->dwSkillInfoFlag ^= DSI_AFFECTGROUP;
			
			::strncpy(lpDsi->szSkillName, "Poison Explosion", MAX_SKILLNAME_LEN);
			break;

		case D2S_POISONNOVA:
			
			lpDsi->dwSkillInfoFlag ^= DSI_RIGHTONLY;
			lpDsi->dwSkillInfoFlag ^= DSI_POISON;
			lpDsi->dwSkillInfoFlag ^= DSI_AFFECTGROUP;
			
			::strncpy(lpDsi->szSkillName, "Poison Nova", MAX_SKILLNAME_LEN);
			break;

		case D2S_TEETH:
			
			lpDsi->dwSkillInfoFlag ^= DSI_TARGETABLE;
			lpDsi->dwSkillInfoFlag ^= DSI_MAGICAL;
			lpDsi->dwSkillInfoFlag ^= DSI_AFFECTGROUP;
			
			::strncpy(lpDsi->szSkillName, "Teeth", MAX_SKILLNAME_LEN);
			break;

		case D2S_CORPOSEEXPLOSION:
			
			lpDsi->dwSkillInfoFlag ^= DSI_NEEDCORPSE;
			lpDsi->dwSkillInfoFlag ^= DSI_PHYSICAL;
			lpDsi->dwSkillInfoFlag ^= DSI_FIRE;
			lpDsi->dwSkillInfoFlag ^= DSI_RIGHTONLY;
			lpDsi->dwSkillInfoFlag ^= DSI_AFFECTGROUP;
			
			::strncpy(lpDsi->szSkillName, "Corpose Explosion", MAX_SKILLNAME_LEN);
			break;

		case D2S_BONESPEAR:
			
			lpDsi->dwSkillInfoFlag ^= DSI_TARGETABLE;
			lpDsi->dwSkillInfoFlag ^= DSI_MAGICAL;
			lpDsi->dwSkillInfoFlag ^= DSI_AFFECTGROUP;
			
			::strncpy(lpDsi->szSkillName, "Bone Spear", MAX_SKILLNAME_LEN);
			break;

		case D2S_BONESPIRIT:
			
			lpDsi->dwSkillInfoFlag ^= DSI_TARGETABLE;
			lpDsi->dwSkillInfoFlag ^= DSI_MAGICAL;
			
			::strncpy(lpDsi->szSkillName, "Bone Spirit", MAX_SKILLNAME_LEN);
			break;

		case D2S_BONEARMOR:
			
			lpDsi->dwSkillInfoFlag ^= DSI_RIGHTONLY;
			lpDsi->dwSkillInfoFlag ^= DSI_ENABLEINTOWN;
			
			::strncpy(lpDsi->szSkillName, "Bone Armor", MAX_SKILLNAME_LEN);
			break;

		case D2S_BONEWALL:
			
			lpDsi->dwSkillInfoFlag ^= DSI_RIGHTONLY;
			lpDsi->dwSkillInfoFlag ^= DSI_TARGETABLE;
			
			::strncpy(lpDsi->szSkillName, "Bone Wall", MAX_SKILLNAME_LEN);
			break;

		case D2S_BONEPRISON:
			
			lpDsi->dwSkillInfoFlag ^= DSI_RIGHTONLY;
			lpDsi->dwSkillInfoFlag ^= DSI_TARGETABLE;
			
			::strncpy(lpDsi->szSkillName, "Bone Prison", MAX_SKILLNAME_LEN);
			break;


// Curses
		case D2S_DIMVISION:
			
			lpDsi->dwSkillInfoFlag ^= DSI_AFFECTGROUP;
			lpDsi->dwSkillInfoFlag ^= DSI_RIGHTONLY;
			lpDsi->dwSkillInfoFlag ^= DSI_TARGETABLE;
			
			::strncpy(lpDsi->szSkillName, "Dim Vision", MAX_SKILLNAME_LEN);
			break;

		case D2S_CONFUSE:
			
			lpDsi->dwSkillInfoFlag ^= DSI_AFFECTGROUP;
			lpDsi->dwSkillInfoFlag ^= DSI_RIGHTONLY;
			lpDsi->dwSkillInfoFlag ^= DSI_TARGETABLE;
			
			::strncpy(lpDsi->szSkillName, "Confuse", MAX_SKILLNAME_LEN);
			break;

		case D2S_ATTRACT:
			
			lpDsi->dwSkillInfoFlag ^= DSI_RIGHTONLY;
			lpDsi->dwSkillInfoFlag ^= DSI_TARGETABLE;
			
			::strncpy(lpDsi->szSkillName, "Attract", MAX_SKILLNAME_LEN);
			break;

		case D2S_AMPLIFYDAMAGE:
			
			lpDsi->dwSkillInfoFlag ^= DSI_AFFECTGROUP;
			lpDsi->dwSkillInfoFlag ^= DSI_RIGHTONLY;
			lpDsi->dwSkillInfoFlag ^= DSI_TARGETABLE;
			
			::strncpy(lpDsi->szSkillName, "Amplify Damage", MAX_SKILLNAME_LEN);
			break;

		case D2S_IRONMAIDEN:
			
			lpDsi->dwSkillInfoFlag ^= DSI_AFFECTGROUP;
			lpDsi->dwSkillInfoFlag ^= DSI_RIGHTONLY;
			lpDsi->dwSkillInfoFlag ^= DSI_TARGETABLE;
			
			::strncpy(lpDsi->szSkillName, "Iron Maiden", MAX_SKILLNAME_LEN);
			break;

		case D2S_LIFETAP:
			
			lpDsi->dwSkillInfoFlag ^= DSI_AFFECTGROUP;
			lpDsi->dwSkillInfoFlag ^= DSI_RIGHTONLY;
			lpDsi->dwSkillInfoFlag ^= DSI_TARGETABLE;
			
			::strncpy(lpDsi->szSkillName, "Life Tap", MAX_SKILLNAME_LEN);
			break;

		case D2S_LOWERRESIST:
			
			lpDsi->dwSkillInfoFlag ^= DSI_AFFECTGROUP;
			lpDsi->dwSkillInfoFlag ^= DSI_RIGHTONLY;
			lpDsi->dwSkillInfoFlag ^= DSI_TARGETABLE;
			
			::strncpy(lpDsi->szSkillName, "Lower Resist", MAX_SKILLNAME_LEN);
			break;

		case D2S_WEAKEN:
			
			lpDsi->dwSkillInfoFlag ^= DSI_AFFECTGROUP;
			lpDsi->dwSkillInfoFlag ^= DSI_RIGHTONLY;
			lpDsi->dwSkillInfoFlag ^= DSI_TARGETABLE;
			
			::strncpy(lpDsi->szSkillName, "Weaken", MAX_SKILLNAME_LEN);
			break;

		case D2S_TERROR:
			
			lpDsi->dwSkillInfoFlag ^= DSI_AFFECTGROUP;
			lpDsi->dwSkillInfoFlag ^= DSI_RIGHTONLY;
			lpDsi->dwSkillInfoFlag ^= DSI_TARGETABLE;
			
			::strncpy(lpDsi->szSkillName, "Terror", MAX_SKILLNAME_LEN);
			break;

		case D2S_DECREPIFY:
			
			lpDsi->dwSkillInfoFlag ^= DSI_AFFECTGROUP;
			lpDsi->dwSkillInfoFlag ^= DSI_RIGHTONLY;
			lpDsi->dwSkillInfoFlag ^= DSI_TARGETABLE;
			
			::strncpy(lpDsi->szSkillName, "Decrepify", MAX_SKILLNAME_LEN);
			break;



///////////////////////////////////////////////////////////////
//    Paladin Skills
///////////////////////////////////////////////////////////////

// Defensive Auras
		case D2S_PRAYER:
			
			lpDsi->dwSkillInfoFlag ^= DSI_RIGHTONLY;
			lpDsi->dwSkillInfoFlag ^= DSI_ENABLEINTOWN;
			
			::strncpy(lpDsi->szSkillName, "Prayer", MAX_SKILLNAME_LEN);
			break;

		case D2S_CLEANSING:
			
			lpDsi->dwSkillInfoFlag ^= DSI_RIGHTONLY;
			lpDsi->dwSkillInfoFlag ^= DSI_ENABLEINTOWN;
			
			::strncpy(lpDsi->szSkillName, "Cleansing", MAX_SKILLNAME_LEN);
			break;

		case D2S_MEDITATION:
			
			lpDsi->dwSkillInfoFlag ^= DSI_RIGHTONLY;
			lpDsi->dwSkillInfoFlag ^= DSI_ENABLEINTOWN;
			
			::strncpy(lpDsi->szSkillName, "Meditation", MAX_SKILLNAME_LEN);
			break;

		case D2S_DEFIANCE:
			
			lpDsi->dwSkillInfoFlag ^= DSI_RIGHTONLY;
			lpDsi->dwSkillInfoFlag ^= DSI_ENABLEINTOWN;
			
			::strncpy(lpDsi->szSkillName, "Defiance", MAX_SKILLNAME_LEN);
			break;

		case D2S_VIGOR:
			
			lpDsi->dwSkillInfoFlag ^= DSI_RIGHTONLY;
			lpDsi->dwSkillInfoFlag ^= DSI_ENABLEINTOWN;
			
			::strncpy(lpDsi->szSkillName, "Vigor", MAX_SKILLNAME_LEN);
			break;

		case D2S_REDEMPTION:
			
			lpDsi->dwSkillInfoFlag ^= DSI_RIGHTONLY;
			lpDsi->dwSkillInfoFlag ^= DSI_ENABLEINTOWN;
			
			::strncpy(lpDsi->szSkillName, "Redemption", MAX_SKILLNAME_LEN);
			break;

		case D2S_RESISTFIRE:
			
			lpDsi->dwSkillInfoFlag ^= DSI_RIGHTONLY;
			lpDsi->dwSkillInfoFlag ^= DSI_ENABLEINTOWN;
			
			::strncpy(lpDsi->szSkillName, "Resist Fire", MAX_SKILLNAME_LEN);
			break;

		case D2S_RESISTCOLD:
			
			lpDsi->dwSkillInfoFlag ^= DSI_RIGHTONLY;
			lpDsi->dwSkillInfoFlag ^= DSI_ENABLEINTOWN;
			
			::strncpy(lpDsi->szSkillName, "Resist Cold", MAX_SKILLNAME_LEN);
			break;

		case D2S_RESISTLIGHTNING:
			
			lpDsi->dwSkillInfoFlag ^= DSI_RIGHTONLY;
			lpDsi->dwSkillInfoFlag ^= DSI_ENABLEINTOWN;
			
			::strncpy(lpDsi->szSkillName, "Resist Lightning", MAX_SKILLNAME_LEN);
			break;

		case D2S_SALVATION:
			
			lpDsi->dwSkillInfoFlag ^= DSI_RIGHTONLY;
			lpDsi->dwSkillInfoFlag ^= DSI_ENABLEINTOWN;
			
			::strncpy(lpDsi->szSkillName, "Salvation", MAX_SKILLNAME_LEN);
			break;


// Offensive Auras
		case D2S_MIGHT:
			
			lpDsi->dwSkillInfoFlag ^= DSI_RIGHTONLY;
			lpDsi->dwSkillInfoFlag ^= DSI_ENABLEINTOWN;
			
			::strncpy(lpDsi->szSkillName, "Might", MAX_SKILLNAME_LEN);
			break;

		case D2S_BLESSEDAIM:
			
			lpDsi->dwSkillInfoFlag ^= DSI_RIGHTONLY;
			lpDsi->dwSkillInfoFlag ^= DSI_ENABLEINTOWN;
			
			::strncpy(lpDsi->szSkillName, "Blessed Aim", MAX_SKILLNAME_LEN);
			break;

		case D2S_CONCENTRATION:
			
			lpDsi->dwSkillInfoFlag ^= DSI_RIGHTONLY;
			lpDsi->dwSkillInfoFlag ^= DSI_ENABLEINTOWN;
			
			::strncpy(lpDsi->szSkillName, "Concentration", MAX_SKILLNAME_LEN);
			break;

		case D2S_FANATICISM:
			
			lpDsi->dwSkillInfoFlag ^= DSI_RIGHTONLY;
			lpDsi->dwSkillInfoFlag ^= DSI_ENABLEINTOWN;
			
			::strncpy(lpDsi->szSkillName, "Fanaticism", MAX_SKILLNAME_LEN);
			break;

		case D2S_HOLYFIRE:
			
			lpDsi->dwSkillInfoFlag ^= DSI_RIGHTONLY;
			lpDsi->dwSkillInfoFlag ^= DSI_ENABLEINTOWN;
			lpDsi->dwSkillInfoFlag ^= DSI_FIRE;
			
			::strncpy(lpDsi->szSkillName, "Holy Fire", MAX_SKILLNAME_LEN);
			break;

		case D2S_HOLYFREEZE:
			
			lpDsi->dwSkillInfoFlag ^= DSI_RIGHTONLY;
			lpDsi->dwSkillInfoFlag ^= DSI_ENABLEINTOWN;
			
			::strncpy(lpDsi->szSkillName, "Holy Freeze", MAX_SKILLNAME_LEN);
			break;

		case D2S_HOLYSHOCK:
			
			lpDsi->dwSkillInfoFlag ^= DSI_RIGHTONLY;
			lpDsi->dwSkillInfoFlag ^= DSI_ENABLEINTOWN;
			lpDsi->dwSkillInfoFlag ^= DSI_LIGHTNING;
			
			::strncpy(lpDsi->szSkillName, "Holy Shock", MAX_SKILLNAME_LEN);
			break;

		case D2S_THORNS:
			
			lpDsi->dwSkillInfoFlag ^= DSI_RIGHTONLY;
			lpDsi->dwSkillInfoFlag ^= DSI_ENABLEINTOWN;
			
			::strncpy(lpDsi->szSkillName, "Thorns", MAX_SKILLNAME_LEN);
			break;

		case D2S_SANCTUARY:
			
			lpDsi->dwSkillInfoFlag ^= DSI_RIGHTONLY;
			lpDsi->dwSkillInfoFlag ^= DSI_ENABLEINTOWN;
			
			::strncpy(lpDsi->szSkillName, "Sanctuary", MAX_SKILLNAME_LEN);
			break;

		case D2S_CONVICTION:
			
			lpDsi->dwSkillInfoFlag ^= DSI_RIGHTONLY;
			lpDsi->dwSkillInfoFlag ^= DSI_ENABLEINTOWN;
			
			::strncpy(lpDsi->szSkillName, "Conviction", MAX_SKILLNAME_LEN);
			break;


// Combat Skills
		case D2S_SACRIFICE:
			
			lpDsi->dwSkillInfoFlag ^= DSI_MELEE;
			lpDsi->dwSkillInfoFlag ^= DSI_TARGETABLE;
			lpDsi->dwSkillInfoFlag ^= DSI_PHYSICAL;			
			
			::strncpy(lpDsi->szSkillName, "Sacrifice", MAX_SKILLNAME_LEN);
			break;

		case D2S_ZEAL:
			
			lpDsi->dwSkillInfoFlag ^= DSI_MELEE;
			lpDsi->dwSkillInfoFlag ^= DSI_TARGETABLE;
			lpDsi->dwSkillInfoFlag ^= DSI_PHYSICAL;
			lpDsi->dwSkillInfoFlag ^= DSI_AFFECTGROUP;
			
			::strncpy(lpDsi->szSkillName, "Zeal", MAX_SKILLNAME_LEN);
			break;

		case D2S_VENGEANCE:
			
			lpDsi->dwSkillInfoFlag ^= DSI_MELEE;
			lpDsi->dwSkillInfoFlag ^= DSI_TARGETABLE;
			lpDsi->dwSkillInfoFlag ^= DSI_PHYSICAL;
			lpDsi->dwSkillInfoFlag ^= DSI_FIRE;
			lpDsi->dwSkillInfoFlag ^= DSI_COLD;
			lpDsi->dwSkillInfoFlag ^= DSI_LIGHTNING;	
			
			::strncpy(lpDsi->szSkillName, "Vengeance", MAX_SKILLNAME_LEN);
			break;

		case D2S_CONVERSION:
			
			lpDsi->dwSkillInfoFlag ^= DSI_MELEE;
			lpDsi->dwSkillInfoFlag ^= DSI_TARGETABLE;
			lpDsi->dwSkillInfoFlag ^= DSI_PHYSICAL;				
			
			::strncpy(lpDsi->szSkillName, "Conversion", MAX_SKILLNAME_LEN);
			break;

		case D2S_HOLYBOLT:
			
			lpDsi->dwSkillInfoFlag ^= DSI_TARGETABLE;
			
			::strncpy(lpDsi->szSkillName, "Holy Bolt", MAX_SKILLNAME_LEN);
			break;

		case D2S_BLESSEDHAMMER:
			
			lpDsi->dwSkillInfoFlag ^= DSI_MAGICAL;
			lpDsi->dwSkillInfoFlag ^= DSI_TARGETABLE;
			
			::strncpy(lpDsi->szSkillName, "Blessed Hammer", MAX_SKILLNAME_LEN);
			break;

		case D2S_FISTOFTHEHEAVENS:
			
			lpDsi->dwSkillInfoFlag ^= DSI_TARGETABLE;
			lpDsi->dwSkillInfoFlag ^= DSI_LIGHTNING;
			lpDsi->dwCoolDown = 1000;
			
			::strncpy(lpDsi->szSkillName, "Fist of The Heavens", MAX_SKILLNAME_LEN);
			break;

		case D2S_SMITE:
			
			lpDsi->dwSkillInfoFlag ^= DSI_MELEE;
			lpDsi->dwSkillInfoFlag ^= DSI_TARGETABLE;
			lpDsi->dwSkillInfoFlag ^= DSI_PHYSICAL;
			lpDsi->dwSkillInfoFlag ^= DSI_STUN;
			lpDsi->dwSkillInfoFlag ^= DSI_KNOCKBACK;
			
			::strncpy(lpDsi->szSkillName, "Smite", MAX_SKILLNAME_LEN);
			break;

		case D2S_CHARGE:
			
			lpDsi->dwSkillInfoFlag ^= DSI_KNOCKBACK;
			lpDsi->dwSkillInfoFlag ^= DSI_MELEE;
			lpDsi->dwSkillInfoFlag ^= DSI_TARGETABLE;
			lpDsi->dwSkillInfoFlag ^= DSI_PHYSICAL;	
			
			::strncpy(lpDsi->szSkillName, "Charge", MAX_SKILLNAME_LEN);
			break;

		case D2S_HOLYSHIELD:
			
			lpDsi->dwSkillInfoFlag ^= DSI_RIGHTONLY;
			lpDsi->dwSkillInfoFlag ^= DSI_ENABLEINTOWN;
			
			::strncpy(lpDsi->szSkillName, "Holy Shield", MAX_SKILLNAME_LEN);
			break;


///////////////////////////////////////////////////////////////
//    Sorceress Skills
///////////////////////////////////////////////////////////////

// Cold Spells
		case D2S_FROSTNOVA:
			
			lpDsi->dwSkillInfoFlag ^= DSI_RIGHTONLY;
			lpDsi->dwSkillInfoFlag ^= DSI_COLD;
			lpDsi->dwSkillInfoFlag ^= DSI_AFFECTGROUP;
			
			::strncpy(lpDsi->szSkillName, "Frost Nova", MAX_SKILLNAME_LEN);
			break;

		case D2S_BLIZZARD:
			
			lpDsi->dwSkillInfoFlag ^= DSI_RIGHTONLY;
			lpDsi->dwSkillInfoFlag ^= DSI_COLD;
			lpDsi->dwSkillInfoFlag ^= DSI_AFFECTGROUP;
			lpDsi->dwSkillInfoFlag ^= DSI_TARGETABLE;
			lpDsi->dwCoolDown = 1800;
			
			::strncpy(lpDsi->szSkillName, "Blizzard", MAX_SKILLNAME_LEN);
			break;

		case D2S_FROZENORB:
			
			lpDsi->dwSkillInfoFlag ^= DSI_COLD;
			lpDsi->dwSkillInfoFlag ^= DSI_AFFECTGROUP;
			lpDsi->dwSkillInfoFlag ^= DSI_TARGETABLE;
			lpDsi->dwCoolDown = 1000;
			
			::strncpy(lpDsi->szSkillName, "Frozen Orb", MAX_SKILLNAME_LEN);
			break;

		case D2S_ICEBOLT:
			
			lpDsi->dwSkillInfoFlag ^= DSI_COLD;
			lpDsi->dwSkillInfoFlag ^= DSI_TARGETABLE;
			
			::strncpy(lpDsi->szSkillName, "Ice Bolt", MAX_SKILLNAME_LEN);
			break;

		case D2S_ICEBLAST:
			
			lpDsi->dwSkillInfoFlag ^= DSI_COLD;
			lpDsi->dwSkillInfoFlag ^= DSI_TARGETABLE;
			
			::strncpy(lpDsi->szSkillName, "Ice Blast", MAX_SKILLNAME_LEN);
			break;

		case D2S_GLACIALSPIKE:
			
			lpDsi->dwSkillInfoFlag ^= DSI_COLD;
			lpDsi->dwSkillInfoFlag ^= DSI_AFFECTGROUP;
			lpDsi->dwSkillInfoFlag ^= DSI_TARGETABLE;
			
			::strncpy(lpDsi->szSkillName, "Glacial Spike", MAX_SKILLNAME_LEN);
			break;

		case D2S_COLDMASTERY:
			
			lpDsi->dwSkillInfoFlag ^= DSI_PASSIVE;
			
			::strncpy(lpDsi->szSkillName, "Cold Mastery", MAX_SKILLNAME_LEN);
			break;

		case D2S_FROZENARMOR:
			
			lpDsi->dwSkillInfoFlag ^= DSI_RIGHTONLY;
			lpDsi->dwSkillInfoFlag ^= DSI_ENABLEINTOWN;
			
			::strncpy(lpDsi->szSkillName, "Frozen Armor", MAX_SKILLNAME_LEN);
			break;

		case D2S_SHIVERARMOR:
			
			lpDsi->dwSkillInfoFlag ^= DSI_RIGHTONLY;
			lpDsi->dwSkillInfoFlag ^= DSI_ENABLEINTOWN;
			
			::strncpy(lpDsi->szSkillName, "Shiver Armor", MAX_SKILLNAME_LEN);
			break;

		case D2S_CHILLINGARMOR:
			
			lpDsi->dwSkillInfoFlag ^= DSI_RIGHTONLY;
			lpDsi->dwSkillInfoFlag ^= DSI_ENABLEINTOWN;
			
			::strncpy(lpDsi->szSkillName, "Chilling Armor", MAX_SKILLNAME_LEN);
			break;

// Lightning Spells
		case D2S_STATICFIELD:
			
			lpDsi->dwSkillInfoFlag ^= DSI_RIGHTONLY;
			lpDsi->dwSkillInfoFlag ^= DSI_LIGHTNING;
			lpDsi->dwSkillInfoFlag ^= DSI_AFFECTGROUP;
			
			::strncpy(lpDsi->szSkillName, "Static Field", MAX_SKILLNAME_LEN);
			break;

		case D2S_NOVA:
			
			lpDsi->dwSkillInfoFlag ^= DSI_RIGHTONLY;
			lpDsi->dwSkillInfoFlag ^= DSI_LIGHTNING;
			lpDsi->dwSkillInfoFlag ^= DSI_AFFECTGROUP;
			
			::strncpy(lpDsi->szSkillName, "Nova", MAX_SKILLNAME_LEN);
			break;

		case D2S_THUNDERSTORM:
			
			lpDsi->dwSkillInfoFlag ^= DSI_RIGHTONLY;
			lpDsi->dwSkillInfoFlag ^= DSI_LIGHTNING;
			
			::strncpy(lpDsi->szSkillName, "Thunder Storm", MAX_SKILLNAME_LEN);
			break;

		case D2S_CHARGEDBOLT:
			
			lpDsi->dwSkillInfoFlag ^= DSI_LIGHTNING;
			lpDsi->dwSkillInfoFlag ^= DSI_AFFECTGROUP;
			
			::strncpy(lpDsi->szSkillName, "Charged Bolt", MAX_SKILLNAME_LEN);
			break;

		case D2S_LIGHTNING:
			
			lpDsi->dwSkillInfoFlag ^= DSI_LIGHTNING;
			lpDsi->dwSkillInfoFlag ^= DSI_AFFECTGROUP;
			
			::strncpy(lpDsi->szSkillName, "Lightning", MAX_SKILLNAME_LEN);
			break;

		case D2S_CHAINLIGHTNING:
			
			lpDsi->dwSkillInfoFlag ^= DSI_LIGHTNING;
			lpDsi->dwSkillInfoFlag ^= DSI_AFFECTGROUP;
			
			::strncpy(lpDsi->szSkillName, "Chain Lightning", MAX_SKILLNAME_LEN);
			break;

		case D2S_LIGHTNINGMASTERY:
			
			lpDsi->dwSkillInfoFlag ^= DSI_PASSIVE;
			
			::strncpy(lpDsi->szSkillName, "Lightning Mastery", MAX_SKILLNAME_LEN);
			break;

		case D2S_TELEKINESIS:
			
			lpDsi->dwSkillInfoFlag ^= DSI_RIGHTONLY;
			lpDsi->dwSkillInfoFlag ^= DSI_LIGHTNING;
			lpDsi->dwSkillInfoFlag ^= DSI_STUN;
			lpDsi->dwSkillInfoFlag ^= DSI_KNOCKBACK;
			lpDsi->dwSkillInfoFlag ^= DSI_TARGETABLE;			
			
			::strncpy(lpDsi->szSkillName, "Telekinesis", MAX_SKILLNAME_LEN);
			break;

		case D2S_TELEPORT:
			
			lpDsi->dwSkillInfoFlag ^= DSI_RIGHTONLY;
			lpDsi->dwSkillInfoFlag ^= DSI_TARGETABLE;
			
			::strncpy(lpDsi->szSkillName, "Teleport", MAX_SKILLNAME_LEN);
			break;

		case D2S_ENERGYSHIELD:
			
			lpDsi->dwSkillInfoFlag ^= DSI_RIGHTONLY;
			lpDsi->dwSkillInfoFlag ^= DSI_ENABLEINTOWN;
			
			::strncpy(lpDsi->szSkillName, "Energy Shield", MAX_SKILLNAME_LEN);
			break;


// Fire Spells
		case D2S_INFERNO:
			
			lpDsi->dwSkillInfoFlag ^= DSI_RIGHTONLY;
			lpDsi->dwSkillInfoFlag ^= DSI_TARGETABLE;
			lpDsi->dwSkillInfoFlag ^= DSI_FIRE;
			lpDsi->dwSkillInfoFlag ^= DSI_AFFECTGROUP;
			
			::strncpy(lpDsi->szSkillName, "Inferno", MAX_SKILLNAME_LEN);
			break;

		case D2S_BLAZE:
			
			lpDsi->dwSkillInfoFlag ^= DSI_RIGHTONLY;
			lpDsi->dwSkillInfoFlag ^= DSI_FIRE;
			lpDsi->dwSkillInfoFlag ^= DSI_AFFECTGROUP;
			
			::strncpy(lpDsi->szSkillName, "Blaze", MAX_SKILLNAME_LEN);
			break;

		case D2S_FIREWALL:
			
			lpDsi->dwSkillInfoFlag ^= DSI_RIGHTONLY;
			lpDsi->dwSkillInfoFlag ^= DSI_TARGETABLE;
			lpDsi->dwSkillInfoFlag ^= DSI_FIRE;
			lpDsi->dwSkillInfoFlag ^= DSI_AFFECTGROUP;
			lpDsi->dwCoolDown = 1400;
			
			::strncpy(lpDsi->szSkillName, "Fire Wall", MAX_SKILLNAME_LEN);
			break;

		case D2S_FIREBOLT:
			
			lpDsi->dwSkillInfoFlag ^= DSI_TARGETABLE;
			lpDsi->dwSkillInfoFlag ^= DSI_FIRE;
			
			::strncpy(lpDsi->szSkillName, "Fire Bolt", MAX_SKILLNAME_LEN);
			break;

		case D2S_FIREBALL:
			
			lpDsi->dwSkillInfoFlag ^= DSI_TARGETABLE;
			lpDsi->dwSkillInfoFlag ^= DSI_FIRE;
			lpDsi->dwSkillInfoFlag ^= DSI_AFFECTGROUP;
			
			::strncpy(lpDsi->szSkillName, "Fire Ball", MAX_SKILLNAME_LEN);
			break;

		case D2S_METEOR:
			
			lpDsi->dwSkillInfoFlag ^= DSI_RIGHTONLY;
			lpDsi->dwSkillInfoFlag ^= DSI_TARGETABLE;
			lpDsi->dwSkillInfoFlag ^= DSI_FIRE;
			lpDsi->dwSkillInfoFlag ^= DSI_AFFECTGROUP;
			lpDsi->dwCoolDown = 1200;
			
			::strncpy(lpDsi->szSkillName, "Meteor", MAX_SKILLNAME_LEN);
			break;

		case D2S_FIREMASTERY:
			
			lpDsi->dwSkillInfoFlag ^= DSI_PASSIVE;
			
			::strncpy(lpDsi->szSkillName, "Fire Mastery", MAX_SKILLNAME_LEN);
			break;

		case D2S_WARMTH:
			
			lpDsi->dwSkillInfoFlag ^= DSI_PASSIVE;
			
			::strncpy(lpDsi->szSkillName, "Warmth", MAX_SKILLNAME_LEN);
			break;

		case D2S_ENCHANT:
			
			lpDsi->dwSkillInfoFlag ^= DSI_ENABLEINTOWN;
			lpDsi->dwSkillInfoFlag ^= DSI_RIGHTONLY;
			
			::strncpy(lpDsi->szSkillName, "Enchant", MAX_SKILLNAME_LEN);
			break;

		case D2S_HYDRA:
			
			lpDsi->dwSkillInfoFlag ^= DSI_RIGHTONLY;
			lpDsi->dwSkillInfoFlag ^= DSI_TARGETABLE;
			lpDsi->dwSkillInfoFlag ^= DSI_FIRE;
			lpDsi->dwSkillInfoFlag ^= DSI_SUMMON;
			lpDsi->dwCoolDown = 2000;
			
			::strncpy(lpDsi->szSkillName, "Hydra", MAX_SKILLNAME_LEN);
			break;


	default:
		
		lpDsi->szSkillName[0] = 0; // make an empty name
		bSkillExists = FALSE;
		break;
	}

	return bSkillExists;
}

CD2Skill::operator WORD() const
{
	return m_wSkillID;
}

CD2Skill::CD2Skill()
{
	m_wSkillID = D2S_INVALID;
}

CD2Skill::CD2Skill(WORD wSkillID)
{
	m_wSkillID = wSkillID;
}

void CD2Skill::SetSkillID(WORD wSkillID)
{
	m_wSkillID = wSkillID;
}

WORD CD2Skill::GetSkillID() const
{
	return m_wSkillID;
}

BOOL CD2Skill::GetSkillName(LPSTR lpBuffer, DWORD dwMaxLen) const
{
	D2SKILLINFO dsi;
	BOOL bSkillExists = GetSkillInfo(m_wSkillID, &dsi);
	::strncpy(lpBuffer, dsi.szSkillName, dwMaxLen);
	return bSkillExists;
}

BOOL CD2Skill::IsRightOnly() const
{
	D2SKILLINFO dsi;
	GetSkillInfo(m_wSkillID, &dsi);
	return (dsi.dwSkillInfoFlag & DSI_RIGHTONLY) != 0;
}

BOOL CD2Skill::IsTargetable() const
{
	D2SKILLINFO dsi;
	GetSkillInfo(m_wSkillID, &dsi);
	return (dsi.dwSkillInfoFlag & DSI_TARGETABLE) != 0;
}

BOOL CD2Skill::NeedCorpse() const
{
	D2SKILLINFO dsi;
	GetSkillInfo(m_wSkillID, &dsi);
	return (dsi.dwSkillInfoFlag & DSI_NEEDCORPSE) != 0;
}

BOOL CD2Skill::IsPassive() const
{
	D2SKILLINFO dsi;
	GetSkillInfo(m_wSkillID, &dsi);
	return (dsi.dwSkillInfoFlag & DSI_PASSIVE) != 0;
}

BOOL CD2Skill::IsEnableInTown() const
{
	D2SKILLINFO dsi;
	GetSkillInfo(m_wSkillID, &dsi);
	return (dsi.dwSkillInfoFlag & DSI_ENABLEINTOWN) != 0;
}

const CD2Skill& CD2Skill::operator =(const CD2Skill &rhs)
{
	m_wSkillID = rhs.m_wSkillID;
	return *this;
}

BOOL CD2Skill::operator ==(const CD2Skill &rhs) const
{
	return m_wSkillID == rhs.m_wSkillID;
}

BOOL CD2Skill::IsNameSame(LPCSTR lpName) const
{
	D2SKILLINFO dsi;
	GetSkillInfo(m_wSkillID, &dsi);
	return (stricmp(dsi.szSkillName, lpName) == 0);
}

DWORD CD2Skill::GetCoolDown() const
{
	D2SKILLINFO dsi;
	GetSkillInfo(m_wSkillID, &dsi);
	return dsi.dwCoolDown;
}
