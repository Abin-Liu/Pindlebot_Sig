#ifndef __D2ACTION_H__
#define __D2ACTION_H__

#include <windows.h>
#include "D2Skill.h"

//////////////////////////////////////////////////////////////
// CD2Action Class Definition
//////////////////////////////////////////////////////////////

// Skill assign side
#define SAS_LEFT			0
#define SAS_RIGHT			1

// Casting aim target
#define CAT_NONE				0
#define CAT_PINDLESKIN			1
#define CAT_BEYONDPINDLESKIN	2
#define CAT_BEFOREPINDLESKIN	3

class CD2Action
{
public:
	void SetDelay(DWORD dwDelay);
	void GetTargetText(LPSTR lpsz) const;
	void GetSideText(LPSTR lpsz) const;
	BOOL IsEnabled() const;
	void Enable(BOOL bEnable);
	void ToString(LPSTR lpString) const;
	BOOL ParseString(LPCSTR lpString);
	DWORD GetRepeat() const;
	DWORD GetDelay() const;
	int GetTarget() const;
	int GetSide() const;
	WORD GetSkillID() const;
	const CD2Action& operator=(const CD2Action& rhs);
	void SetAction(WORD wSkillID, int nSide, int nTarget, DWORD dwDelay, DWORD dwRepeat);
	CD2Action(WORD wSkillID, int nSide, int nTarget, DWORD dwDelay, DWORD dwRepeat);
	CD2Action();

private:
	WORD m_wSkillID; // The skill ID
	int m_nSide; // Set skill on left/right side
	int m_nTarget; // Target, 0-none, 1-pindleskin, 2-behind pindleskin, 3-before pindleskin
	DWORD m_dwDelay; // Delay between 2 casting
	DWORD m_dwRepeat; // Repeat this action x times
	BOOL m_bEnabled; // Action enabled/disabled
};


#endif