#ifndef __D2CHARACTER_H__
#define __D2CHARACTER_H__

#include <windows.h>
#include "D2Skill.h"

//////////////////////////////////////////////////////////////
// Character Class Definition
//////////////////////////////////////////////////////////////

// Character class ID values
#define CCD_AMAZON				0
#define CCD_ASSASSIN			1
#define CCD_BARBARIAN			2
#define CCD_DRUID				3
#define CCD_NECROMANCER			4
#define CCD_PALADIN				5
#define CCD_SORCERESS			6
#define CCD_UNKNOWN				7

// Character class string representation
const char* const CHARCLASSNAME[] =
 { "Amazon", "Assassin", "Barbarian", "Druid", "Necromancer", "Paladin", "Sorceress" }; 

//////////////////////////////////////////////////////////////
// CD2Character Class Definition
//////////////////////////////////////////////////////////////

#define _SKILLNUMBER	37 // number of available skills for each character class, unsommon excluded.
#define _CHARNAME		15 // Max length of character name
class CD2Character
{
public:
	LPCSTR GetClassName() const;
	CD2Character();
	CD2Character(LPCSTR lpName, BYTE byCharClass);
	~CD2Character();

	void SetCharClass(LPCSTR lpName, BYTE byCharClass);
	const CD2Skill* GetSkillArray() const;
	const CD2Skill* FindSkill(LPCSTR lpSkillName) const;
	const CD2Skill* FindSkill(WORD wSkillID) const;
	BYTE GetCharClass() const;
	LPCSTR GetCharName() const;

private:
	char m_szCharName[_CHARNAME + 1]; // Character Name
	char m_szClassName[12]; // Class name, Druid, Amazon, Paladin, etc
	BYTE m_byCharClass; // Character class, Amazon, Assassin, Barbarian, etc
	CD2Skill m_aSkills[_SKILLNUMBER]; // Array of skills for current character class
	char m_szSafetyScheme[65]; // safety scheme name
	void InitialSkills(); // Initialize skill array, huge work...
};

#endif