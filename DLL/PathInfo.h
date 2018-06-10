// path info character move

#ifndef __PATHINFO__
#define __PATHINFO__

#include <windows.h>

// states definition
struct MOTIONSTATE
{
	//int nState; // current motion state
	int nStepOnPortal; // current step towards red portal
	int nStepOnQual; // current step to Qual
	int nStepOnlarzuk; // current step towards larzuk
	int nStepOnStash; // current step to stash
	int nStepOnApproachPindle; // current step on approaching pindleskin
	int nStepOnAction; 
	int nStepOnSkill;
	int nStepOnTeleport;
};

struct LOCATION
{
	WORD x;
	WORD y;
};

const LOCATION INITLOCATION = { 0x13ea, 0x139f }; //{ 0x13ea, 0x139f }; // Upon beginning of game
const LOCATION APPROACH_MALAH = { 0x13d7, 0x13aa }; // approach Malah
const LOCATION LARZUK_STORE = { 0x1419, 0x13b5 }; // Larzuk's shop
const LOCATION AVOID_MALAH = { 0x13d2, 0x13a2 }; // Avoid Malah

const LOCATION TO_PORTAL[] =
				 {					
					{ 0x13ce, 0x13aa }, // after Malah
					{ 0x13d9, 0x13c5 }, // downstairs
					{ 0x13da, 0x13db }, // junction 
					// visit Qual here, then rollback to junction
					{ 0x13dd, 0x13f6 }, // after junction
					{ 0x13e3, 0x1402 }, // ready tomake turn
					{ 0x13fe, 0x1400 }, // approach red portal
					{ 0x0000, 0x0000 }};

const LOCATION APPROACH_QUAL = { 0x13c8, 0x13d3 }; // approach Qual-Kehk
const LOCATION TO_LARZUK[] = {
					{ 0x13f5, 0x13b4 }, // middle platform
					{ 0x1407, 0x13b1 }, // down stairs
					{ 0x1413, 0x13b5 }, // Larzuk's shop
					{ 0x0000, 0x0000 }};

const LOCATION TO_STASH[] = {
					{ 0x13f1, 0x13b5 }, // middle platform
					{ 0x13f7, 0x13c6 }, // down stairs
					{ 0x1404, 0x13c3 }, // stash
					{ 0x0000, 0x0000 }};

const LOCATION LARZUK_TO_PORTAL[] = {
				{ 0x140d, 0x13b4 },
				{ 0x140c, 0x13c9 },
				{ 0x140d, 0x13df },
				{ 0x140e, 0x13f3 },
				{ 0x13fe, 0x1400 }, // protal location
				{ 0x0000, 0x0000 }}; 

const LOCATION INRED_LOC = { 0x2754, 0x33fc };
			
// approaching pindleskin points

// not using teleport, 4 steps required
const LOCATION WALK_TO_PINDLE[] = {
						{ 0x274c, 0x33eb },
						{ 0x274e, 0x33d3 }, 
						{ 0x274b, 0x33c5 }, // the door
						{ 0x274b, 0x33ac },
						{ 0x0000, 0x0000 }};

// using teleport
const LOCATION TELE_TO_PINDLE[] = {
						{ 0x2754, 0x33e1 },
						{ 0x274f, 0x33c3 }, 
						{ 0x274b, 0x33ac },
						{ 0x0000, 0x0000 }};
// position adjustment
#define LA_NORMAL	0
#define LA_CLOSER	1
#define LA_FARTHER	2

const LOCATION ADJ_NORMAL  = { 0x274b, 0x33ac }; // normal
const LOCATION ADJ_CLOSER  = { 0x274b, 0x33a9 }; // closer than normal
const LOCATION ADJ_FARTHER = { 0x274b, 0x33b0 }; // farther than normal
const LOCATION ADJ_RETREAT = { 0x274b, 0x33bb }; // retreat

#define INIT_OFFSET 50 // maximum position offset allowed

#endif