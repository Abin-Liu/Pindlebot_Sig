#ifndef __MEMDEF_H__
#define __MEMDEF_H__

#define D2_CLASS_NAME "Diablo II" // diablo 2 class name

#define AMMO_OTHER	0
#define AMMO_ARROW	1
#define AMMO_BOLT	2

// reason for finishing a game
enum {
	FR_UNKNOWN = 0,
	FR_SUCCEEDED,
	FR_INDANGER,
	FR_DEADLYCOMBO,
	FR_CHARDEAD,
	FR_MERCDEAD,
	FR_FAILED,
	FR_REPAIR,
	FR_OUTSKILL,
	FR_WARPWP
};

// Motion States
enum {  MS_NONE = 0, // initial state

		MS_INIT_START, // initial state OK
		MS_ADJUST_LOCATION, // first move
		MS_VERYFY_INIT_LOCATION, // verify first move

		MS_APPROACH_WP, // walking to WP, if it's at location #2
		MS_WALK_WP, // walk to WP
		MS_WARP_ACT5, // goto act5 through WP
		MS_AFTER_WP, // after using WP

		MS_WALK_PORTAL, // travel to portal
		MS_VERIFY_STEP, // verify character position
		MS_VERIFY_LOCATION, // check if is by portal
		MS_TALK_PORTAL, // take portal
		MS_CHECK_PORTAL, // check if entered portal successfully


		MS_WALK_MALAH, // step to entity - Malah
		MS_TALK_MALAH, // interact with entity - Malah
		MS_LEAVE_MALAH, // calculate/recheck HP/MP
		MS_AFTER_MALAH, // back to portal track[0]


		MS_WALK_QUAL, // step to entity - Qual
		MS_TALK_QUAL, // interact with entity - Qual
		MS_RES_MERC, // resurrect merc
		MS_LEAVE_QUAL, // back to junction

		MS_TRAVEL_LARZUK, // travel to Larzuk
		MS_WALK_LARZUK, // step to entity - Larzuk
		MS_TALK_LARZUK, // open store - Larzuk
		MS_ASK_REPAIR, // repair all
		MS_LEAVE_LARZUK, // ...
		MS_AFTER_LARZUK,

		MS_FINISHED,

		MS_CAST_FINDITEM, // cast fint item skill(Barbarians)
		MS_PICK_ITEMS, // enable pickit and radius
		MS_PICKITEMS_OK, // ready to exit

		MS_APPROACH_PINDLESKIN, // approcah pindleskin
		MS_CHECK_TELEPORT, // check steps to pindleskin

		MS_PREPARETOQUIT,

		MS_CAST_SPELLS, // casting spells
		MS_CAST_BEFORERED,
		MS_CAST_AFTERRED,
		MS_CAST_ONPINDLESKIN,

		MS_PINDLE_DEAD,

		MS_TRAVEL_STASH, // walk to stash
		MS_DEPOSIT, // deposit items
		MS_AFTER_DEPOSIT, 
		
		MS_DEBUG
	 };

#endif