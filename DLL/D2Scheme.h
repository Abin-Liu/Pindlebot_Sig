
//////////////////////////////////////////////////////////////
// CD2Scheme Class Definition
//////////////////////////////////////////////////////////////

#ifndef __D2SCHEME_H__
#define __D2SCHEME_H__

#include "D2lib\\template\\ArrayEx.h"
#include "D2Skill.h"
#include "D2Character.h"
#include "D2Action.h"

// D2 action situation
#define DAS_BEFORERED		0
#define DAS_AFTERRED		1
#define DAS_ONPINDLESKIN	2
#define DAS_ONCORPOSE		3

class CD2Scheme
{
public:
	void LimitMinDelays(DWORD dwMinDelay);
	void SetSafety(LPCTSTR lpSafety);
	LPCSTR GetSafetyScheme() const;
	void SetScheme(LPCSTR lpCharName, BYTE byCharClass, LPCSTR lpSafetyScheme);
	CD2Scheme();
	CD2Scheme(LPCSTR lpCharName, BYTE byCharClass, LPCSTR lpSafetyScheme);

	char m_szCharName[_CHARNAME + 1]; // Character Name
	BYTE m_byCharClass; // Character class, Amazon, Assassin, Barbarian, etc
	char m_szSafetyScheme[65];
	CArrayEx<CD2Action, const CD2Action&> m_aBeforeRed;
	CArrayEx<CD2Action, const CD2Action&> m_aAfterRed;
	CArrayEx<CD2Action, const CD2Action&> m_aOnPindleskin;
	CArrayEx<CD2Action, const CD2Action&> m_aOnCorpose;
};

#endif