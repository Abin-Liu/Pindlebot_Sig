//#include "stdafx.h"
#include "D2Character.h"
#include <assert.h>

//////////////////////////////////////////////////////////////////
// ImplementationCD2Character class
//////////////////////////////////////////////////////////////////

CD2Character::CD2Character()
{
	::memset(m_szCharName, 0, _CHARNAME + 1);
	::memset(m_szClassName, 0, 12);
	m_byCharClass = CCD_UNKNOWN;
}

CD2Character::CD2Character(LPCSTR lpName, BYTE byCharClass)
{
	::memset(m_szCharName, 0, _CHARNAME + 1);
	strncpy(m_szCharName, lpName, _CHARNAME);
	::memset(m_szClassName, 0, 12);
	m_byCharClass = byCharClass;
	if (m_byCharClass < 7)
		strcpy(m_szClassName, CHARCLASSNAME[m_byCharClass]);
	InitialSkills();
}

CD2Character::~CD2Character()
{
}

void CD2Character::SetCharClass(LPCSTR lpName, BYTE byCharClass)
{
	::memset(m_szCharName, 0, _CHARNAME + 1);
	strncpy(m_szCharName, lpName, _CHARNAME);
	::memset(m_szClassName, 0, 12);
	m_byCharClass = byCharClass;
	if (m_byCharClass < 7)
		strcpy(m_szClassName, CHARCLASSNAME[m_byCharClass]);
	InitialSkills();
}

const CD2Skill* CD2Character::GetSkillArray() const
{
	return m_aSkills;
}

// Initialize skill list(32 skills in total) for current character class.
// Each class have 32 skills(2 common skills and 30 class specified skills).
void CD2Character::InitialSkills()
{
	::memset(m_aSkills, D2S_INVALID, _SKILLNUMBER * sizeof(CD2Skill));
	int nIndex = 0;

	// Common spells (melee and throwing)
	m_aSkills[nIndex++].SetSkillID(D2S_ATTACK);
	m_aSkills[nIndex++].SetSkillID(D2S_THROW);

	switch (m_byCharClass)
	{
	case CCD_AMAZON:

		// Javelin and Spear Skills
		m_aSkills[nIndex++].SetSkillID(D2S_JAB);
		m_aSkills[nIndex++].SetSkillID(D2S_IMPALE);
		m_aSkills[nIndex++].SetSkillID(D2S_FEND);
		m_aSkills[nIndex++].SetSkillID(D2S_POWERSTRIKE);
		m_aSkills[nIndex++].SetSkillID(D2S_CHARGEDSTRIKE);
		m_aSkills[nIndex++].SetSkillID(D2S_LIGHTNINGSTRIKE);
		m_aSkills[nIndex++].SetSkillID(D2S_POISONJAVELIN);
		m_aSkills[nIndex++].SetSkillID(D2S_LIGHTNINGBOLT);
		m_aSkills[nIndex++].SetSkillID(D2S_PLAGUEJAVELIN);
		m_aSkills[nIndex++].SetSkillID(D2S_LIGHTNINGFURY);

		// Passive and Magic Skills
		m_aSkills[nIndex++].SetSkillID(D2S_INNERSIGHT);
		m_aSkills[nIndex++].SetSkillID(D2S_SLOWMISSILES);
		m_aSkills[nIndex++].SetSkillID(D2S_DECOY);
		m_aSkills[nIndex++].SetSkillID(D2S_VALKYRIE);
		m_aSkills[nIndex++].SetSkillID(D2S_DODGE);
		m_aSkills[nIndex++].SetSkillID(D2S_AVOID);
		m_aSkills[nIndex++].SetSkillID(D2S_EVADE);
		m_aSkills[nIndex++].SetSkillID(D2S_CRITICALSTRIKE);
		m_aSkills[nIndex++].SetSkillID(D2S_PENETRATE);
		m_aSkills[nIndex++].SetSkillID(D2S_PIERCE);

		// Bow and Crossbow Skills
		m_aSkills[nIndex++].SetSkillID(D2S_COLDARROW);
		m_aSkills[nIndex++].SetSkillID(D2S_ICEARROW);
		m_aSkills[nIndex++].SetSkillID(D2S_FREEZINGARROW);
		m_aSkills[nIndex++].SetSkillID(D2S_MAGICARROW);
		m_aSkills[nIndex++].SetSkillID(D2S_MULTIPLESHOT);
		m_aSkills[nIndex++].SetSkillID(D2S_GUIDEDARROW);
		m_aSkills[nIndex++].SetSkillID(D2S_STRAFE);
		m_aSkills[nIndex++].SetSkillID(D2S_FIREARROW);
		m_aSkills[nIndex++].SetSkillID(D2S_EXPLODINGARROW);
		m_aSkills[nIndex++].SetSkillID(D2S_IMMOLATIONARROW);
		break;

	case CCD_ASSASSIN:

		// Martial Arts
		m_aSkills[nIndex++].SetSkillID(D2S_FISTSOFFIRE);
		m_aSkills[nIndex++].SetSkillID(D2S_CLAWSOFTHUNDER);
		m_aSkills[nIndex++].SetSkillID(D2S_BLADESOFICE);
		m_aSkills[nIndex++].SetSkillID(D2S_TIGERSTRIKE);
		m_aSkills[nIndex++].SetSkillID(D2S_COBRASTRIKE);
		m_aSkills[nIndex++].SetSkillID(D2S_PHOENIXSTRIKE);
		m_aSkills[nIndex++].SetSkillID(D2S_DRAGONTALON);
		m_aSkills[nIndex++].SetSkillID(D2S_DRAGONCLAW);
		m_aSkills[nIndex++].SetSkillID(D2S_DRAGONTAIL);
		m_aSkills[nIndex++].SetSkillID(D2S_DRAGONFLIGHT);

		// Shadow Disciplines
		m_aSkills[nIndex++].SetSkillID(D2S_BURSTOFSPEED);
		m_aSkills[nIndex++].SetSkillID(D2S_FADE);
		m_aSkills[nIndex++].SetSkillID(D2S_VENOM);
		m_aSkills[nIndex++].SetSkillID(D2S_CLAWMASTERY);
		m_aSkills[nIndex++].SetSkillID(D2S_WEAPONBLOCK);
		m_aSkills[nIndex++].SetSkillID(D2S_SHADOWWARRIOR);
		m_aSkills[nIndex++].SetSkillID(D2S_SHADOWMASTER);
		m_aSkills[nIndex++].SetSkillID(D2S_PSYCHICHAMMER);
		m_aSkills[nIndex++].SetSkillID(D2S_CLOAKOFSHADOWS);
		m_aSkills[nIndex++].SetSkillID(D2S_MINDBLAST);

		// Traps
		m_aSkills[nIndex++].SetSkillID(D2S_SHOCKWEB);
		m_aSkills[nIndex++].SetSkillID(D2S_CHARGEDBOLTSENTRY);
		m_aSkills[nIndex++].SetSkillID(D2S_LIGHTNINGSENTRY);
		m_aSkills[nIndex++].SetSkillID(D2S_DEATHSENTRY);
		m_aSkills[nIndex++].SetSkillID(D2S_FIREBLAST);
		m_aSkills[nIndex++].SetSkillID(D2S_WAKEOFFIRE);
		m_aSkills[nIndex++].SetSkillID(D2S_WAKEOFINFERNO);
		m_aSkills[nIndex++].SetSkillID(D2S_BLADESENTINEL);
		m_aSkills[nIndex++].SetSkillID(D2S_BLADEFURY);
		m_aSkills[nIndex++].SetSkillID(D2S_BLADESHIELD);
		break;

	case CCD_BARBARIAN:

		// Warcries
		m_aSkills[nIndex++].SetSkillID(D2S_HOWL);
		m_aSkills[nIndex++].SetSkillID(D2S_TAUNT);
		m_aSkills[nIndex++].SetSkillID(D2S_BATTLECRY);
		m_aSkills[nIndex++].SetSkillID(D2S_WARCRY);
		m_aSkills[nIndex++].SetSkillID(D2S_SHOUT);
		m_aSkills[nIndex++].SetSkillID(D2S_BATTLEORDERS);
		m_aSkills[nIndex++].SetSkillID(D2S_BATTLECOMMAND);
		m_aSkills[nIndex++].SetSkillID(D2S_FINDPOTION);
		m_aSkills[nIndex++].SetSkillID(D2S_FINDITEM);
		m_aSkills[nIndex++].SetSkillID(D2S_GRIMWARD);

		// Combat Masteries
		m_aSkills[nIndex++].SetSkillID(D2S_SWORDMASTERY);
		m_aSkills[nIndex++].SetSkillID(D2S_POLEARMMASTERY);
		m_aSkills[nIndex++].SetSkillID(D2S_INCREASEDSTAMINA);
		m_aSkills[nIndex++].SetSkillID(D2S_INCREASEDSPEED);
		m_aSkills[nIndex++].SetSkillID(D2S_AXEMASTERY);
		m_aSkills[nIndex++].SetSkillID(D2S_THROWINGMASTERY);
		m_aSkills[nIndex++].SetSkillID(D2S_MACEMASTERY);
		m_aSkills[nIndex++].SetSkillID(D2S_SPEARMASTERY);
		m_aSkills[nIndex++].SetSkillID(D2S_IRONSKIN);
		m_aSkills[nIndex++].SetSkillID(D2S_NATURALRESISTANCE);

		// Combat Skills
		m_aSkills[nIndex++].SetSkillID(D2S_LEAP);
		m_aSkills[nIndex++].SetSkillID(D2S_LEAPATTACK);
		m_aSkills[nIndex++].SetSkillID(D2S_WHIRLWIND);
		m_aSkills[nIndex++].SetSkillID(D2S_BASH);
		m_aSkills[nIndex++].SetSkillID(D2S_STUN);
		m_aSkills[nIndex++].SetSkillID(D2S_CONCENTRATE);
		m_aSkills[nIndex++].SetSkillID(D2S_BERSERK);
		m_aSkills[nIndex++].SetSkillID(D2S_DOUBLESWING);
		m_aSkills[nIndex++].SetSkillID(D2S_DOUBLETHROW);
		m_aSkills[nIndex++].SetSkillID(D2S_FRENZY);
		break;

	case CCD_DRUID:

		// Elemental
		m_aSkills[nIndex++].SetSkillID(D2S_FIRESTORM);
		m_aSkills[nIndex++].SetSkillID(D2S_MOLTENBOULDER);
		m_aSkills[nIndex++].SetSkillID(D2S_FISSURE);
		m_aSkills[nIndex++].SetSkillID(D2S_VOLCANO);
		m_aSkills[nIndex++].SetSkillID(D2S_ARMAGEDDON);
		m_aSkills[nIndex++].SetSkillID(D2S_TWISTER);
		m_aSkills[nIndex++].SetSkillID(D2S_TORNADO);
		m_aSkills[nIndex++].SetSkillID(D2S_HURRICANE);
		m_aSkills[nIndex++].SetSkillID(D2S_ARCTICBLAST);
		m_aSkills[nIndex++].SetSkillID(D2S_CYCLONEARMOR);

		// Shape Shifting
		m_aSkills[nIndex++].SetSkillID(D2S_WEREWOLF);
		m_aSkills[nIndex++].SetSkillID(D2S_FERALRAGE);
		m_aSkills[nIndex++].SetSkillID(D2S_RABIES);
		m_aSkills[nIndex++].SetSkillID(D2S_FURY);
		m_aSkills[nIndex++].SetSkillID(D2S_LYCANTHROPY);
		m_aSkills[nIndex++].SetSkillID(D2S_FIRECLAWS);
		m_aSkills[nIndex++].SetSkillID(D2S_HUNGER);
		m_aSkills[nIndex++].SetSkillID(D2S_WEREBEAR);
		m_aSkills[nIndex++].SetSkillID(D2S_MAUL);
		m_aSkills[nIndex++].SetSkillID(D2S_SHOCKWAVE);

		// Summoning
		m_aSkills[nIndex++].SetSkillID(D2S_OAKSAGE);
		m_aSkills[nIndex++].SetSkillID(D2S_HEARTOFWOLVERINE);
		m_aSkills[nIndex++].SetSkillID(D2S_SPIRITOFBARBS);
		m_aSkills[nIndex++].SetSkillID(D2S_RAVEN);
		m_aSkills[nIndex++].SetSkillID(D2S_SUMMONSPIRITWOLF);
		m_aSkills[nIndex++].SetSkillID(D2S_SUMMONDIREWOLF);
		m_aSkills[nIndex++].SetSkillID(D2S_SUMMONGRIZZLY);
		m_aSkills[nIndex++].SetSkillID(D2S_POISONCREEPER);
		m_aSkills[nIndex++].SetSkillID(D2S_CARRIONVINE);
		m_aSkills[nIndex++].SetSkillID(D2S_SOLARCREEPER);

		break;

	case CCD_NECROMANCER:

		// Summoning Spells
		m_aSkills[nIndex++].SetSkillID(D2S_SKELETONMASTERY);
		m_aSkills[nIndex++].SetSkillID(D2S_GOLEMMASTERY);
		m_aSkills[nIndex++].SetSkillID(D2S_SUMMONRESIST);
		m_aSkills[nIndex++].SetSkillID(D2S_CLAYGOLEM);
		m_aSkills[nIndex++].SetSkillID(D2S_BLOODGOLEM);
		m_aSkills[nIndex++].SetSkillID(D2S_IRONGOLEM);
		m_aSkills[nIndex++].SetSkillID(D2S_FIREGOLEM);
		m_aSkills[nIndex++].SetSkillID(D2S_RAISESKELETON);
		m_aSkills[nIndex++].SetSkillID(D2S_RAISESKELETALMAGE);
		m_aSkills[nIndex++].SetSkillID(D2S_REVIVE);

		// and Bone Spells
		m_aSkills[nIndex++].SetSkillID(D2S_POISONDAGGER);
		m_aSkills[nIndex++].SetSkillID(D2S_POISONEXPLOSION);
		m_aSkills[nIndex++].SetSkillID(D2S_POISONNOVA);
		m_aSkills[nIndex++].SetSkillID(D2S_TEETH);
		m_aSkills[nIndex++].SetSkillID(D2S_CORPOSEEXPLOSION);
		m_aSkills[nIndex++].SetSkillID(D2S_BONESPEAR);
		m_aSkills[nIndex++].SetSkillID(D2S_BONESPIRIT);
		m_aSkills[nIndex++].SetSkillID(D2S_BONEARMOR);
		m_aSkills[nIndex++].SetSkillID(D2S_BONEWALL);
		m_aSkills[nIndex++].SetSkillID(D2S_BONEPRISON);

		// Curses
		m_aSkills[nIndex++].SetSkillID(D2S_DIMVISION);
		m_aSkills[nIndex++].SetSkillID(D2S_CONFUSE);
		m_aSkills[nIndex++].SetSkillID(D2S_ATTRACT);
		m_aSkills[nIndex++].SetSkillID(D2S_AMPLIFYDAMAGE);
		m_aSkills[nIndex++].SetSkillID(D2S_IRONMAIDEN);
		m_aSkills[nIndex++].SetSkillID(D2S_LIFETAP);
		m_aSkills[nIndex++].SetSkillID(D2S_LOWERRESIST);
		m_aSkills[nIndex++].SetSkillID(D2S_WEAKEN);
		m_aSkills[nIndex++].SetSkillID(D2S_TERROR);
		m_aSkills[nIndex++].SetSkillID(D2S_DECREPIFY);

		break;

	case CCD_PALADIN:

		// Defensive Auras
		m_aSkills[nIndex++].SetSkillID(D2S_PRAYER);
		m_aSkills[nIndex++].SetSkillID(D2S_CLEANSING);
		m_aSkills[nIndex++].SetSkillID(D2S_MEDITATION);
		m_aSkills[nIndex++].SetSkillID(D2S_DEFIANCE);
		m_aSkills[nIndex++].SetSkillID(D2S_VIGOR);
		m_aSkills[nIndex++].SetSkillID(D2S_REDEMPTION);
		m_aSkills[nIndex++].SetSkillID(D2S_RESISTFIRE);
		m_aSkills[nIndex++].SetSkillID(D2S_RESISTCOLD);
		m_aSkills[nIndex++].SetSkillID(D2S_RESISTLIGHTNING);
		m_aSkills[nIndex++].SetSkillID(D2S_SALVATION);

		// Offensive Auras
		m_aSkills[nIndex++].SetSkillID(D2S_MIGHT);
		m_aSkills[nIndex++].SetSkillID(D2S_BLESSEDAIM);
		m_aSkills[nIndex++].SetSkillID(D2S_CONCENTRATION);
		m_aSkills[nIndex++].SetSkillID(D2S_FANATICISM);
		m_aSkills[nIndex++].SetSkillID(D2S_HOLYFIRE);
		m_aSkills[nIndex++].SetSkillID(D2S_HOLYFREEZE);
		m_aSkills[nIndex++].SetSkillID(D2S_HOLYSHOCK);
		m_aSkills[nIndex++].SetSkillID(D2S_THORNS);
		m_aSkills[nIndex++].SetSkillID(D2S_SANCTUARY);
		m_aSkills[nIndex++].SetSkillID(D2S_CONVICTION);

		// Combat Skills
		m_aSkills[nIndex++].SetSkillID(D2S_SACRIFICE);
		m_aSkills[nIndex++].SetSkillID(D2S_ZEAL);
		m_aSkills[nIndex++].SetSkillID(D2S_VENGEANCE);
		m_aSkills[nIndex++].SetSkillID(D2S_CONVERSION);
		m_aSkills[nIndex++].SetSkillID(D2S_HOLYBOLT);
		m_aSkills[nIndex++].SetSkillID(D2S_BLESSEDHAMMER);
		m_aSkills[nIndex++].SetSkillID(D2S_FISTOFTHEHEAVENS);
		m_aSkills[nIndex++].SetSkillID(D2S_SMITE);
		m_aSkills[nIndex++].SetSkillID(D2S_CHARGE);
		m_aSkills[nIndex++].SetSkillID(D2S_HOLYSHIELD);
		break;

	case CCD_SORCERESS:

		// Cold Spells
		m_aSkills[nIndex++].SetSkillID(D2S_FROSTNOVA);
		m_aSkills[nIndex++].SetSkillID(D2S_BLIZZARD);
		m_aSkills[nIndex++].SetSkillID(D2S_FROZENORB);
		m_aSkills[nIndex++].SetSkillID(D2S_ICEBOLT);
		m_aSkills[nIndex++].SetSkillID(D2S_ICEBLAST);
		m_aSkills[nIndex++].SetSkillID(D2S_GLACIALSPIKE);
		m_aSkills[nIndex++].SetSkillID(D2S_COLDMASTERY);
		m_aSkills[nIndex++].SetSkillID(D2S_FROZENARMOR);
		m_aSkills[nIndex++].SetSkillID(D2S_SHIVERARMOR);
		m_aSkills[nIndex++].SetSkillID(D2S_CHILLINGARMOR);

		// Spells
		m_aSkills[nIndex++].SetSkillID(D2S_STATICFIELD);
		m_aSkills[nIndex++].SetSkillID(D2S_NOVA);
		m_aSkills[nIndex++].SetSkillID(D2S_THUNDERSTORM);
		m_aSkills[nIndex++].SetSkillID(D2S_CHARGEDBOLT);
		m_aSkills[nIndex++].SetSkillID(D2S_LIGHTNING);
		m_aSkills[nIndex++].SetSkillID(D2S_CHAINLIGHTNING);
		m_aSkills[nIndex++].SetSkillID(D2S_LIGHTNINGMASTERY);
		m_aSkills[nIndex++].SetSkillID(D2S_TELEKINESIS);
		m_aSkills[nIndex++].SetSkillID(D2S_TELEPORT);
		m_aSkills[nIndex++].SetSkillID(D2S_ENERGYSHIELD);

		// Spells
		m_aSkills[nIndex++].SetSkillID(D2S_INFERNO);
		m_aSkills[nIndex++].SetSkillID(D2S_BLAZE);
		m_aSkills[nIndex++].SetSkillID(D2S_FIREWALL);
		m_aSkills[nIndex++].SetSkillID(D2S_FIREBOLT);
		m_aSkills[nIndex++].SetSkillID(D2S_FIREBALL);
		m_aSkills[nIndex++].SetSkillID(D2S_METEOR);
		m_aSkills[nIndex++].SetSkillID(D2S_FIREMASTERY);
		m_aSkills[nIndex++].SetSkillID(D2S_WARMTH);
		m_aSkills[nIndex++].SetSkillID(D2S_ENCHANT);
		m_aSkills[nIndex++].SetSkillID(D2S_HYDRA);

		break;

	default: // error handling
		//assert(FALSE); // unexpected character class
		break;
	}

	// Total skill number must be _SKILLNUMBER, otherwise something
	// went wrong.
	if (m_byCharClass < 7)
		assert(nIndex <= _SKILLNUMBER);
}

const CD2Skill* CD2Character::FindSkill(WORD wSkillID) const
{
	for (int i = 0; i < _SKILLNUMBER; i++)
	{
		if (m_aSkills[i].GetSkillID() == wSkillID)
			return &(m_aSkills[i]);
	}

	return NULL;
}

const CD2Skill* CD2Character::FindSkill(LPCSTR lpSkillName) const
{
	for (int i = 0; i < _SKILLNUMBER; i++)
	{
		if (m_aSkills[i].IsNameSame(lpSkillName))
			return &(m_aSkills[i]);
	}

	return NULL;
}

LPCSTR CD2Character::GetCharName() const
{
	return m_szCharName;
}

BYTE CD2Character::GetCharClass() const
{
	return m_byCharClass;
}

LPCSTR CD2Character::GetClassName() const
{
	return m_szClassName;
}

