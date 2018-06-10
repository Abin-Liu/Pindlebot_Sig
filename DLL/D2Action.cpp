//#include "stdafx.h"
#include "D2Action.h"
#include <stdio.h>

//////////////////////////////////////////////////////////////////
// Implementation of CD2Action class
//////////////////////////////////////////////////////////////////

CD2Action::CD2Action()
{
	m_wSkillID = D2S_ATTACK;
	m_nSide = SAS_LEFT;
	m_nTarget = CAT_PINDLESKIN;
	m_dwDelay = 0;
	m_dwRepeat = 1;
	m_bEnabled = TRUE;
}

CD2Action::CD2Action(WORD wSkillID, int nSide, int nTarget, DWORD dwDelay, DWORD dwRepeat)
{
	m_wSkillID = wSkillID;
	m_nSide = nSide;
	m_nTarget = nTarget;
	m_dwDelay = dwDelay;
	m_dwRepeat = dwRepeat;
	m_bEnabled = TRUE;
}

void CD2Action::SetAction(WORD wSkillID, int nSide, int nTarget, DWORD dwDelay, DWORD dwRepeat)
{
	m_wSkillID = wSkillID;
	m_nSide = nSide;
	m_nTarget = nTarget;
	m_dwDelay = dwDelay;
	m_dwRepeat = dwRepeat;
	m_bEnabled = TRUE;
}

const CD2Action& CD2Action::operator=(const CD2Action &rhs)
{
	m_wSkillID = rhs.m_wSkillID;
	m_nSide = rhs.m_nSide;
	m_nTarget = rhs.m_nTarget;
	m_dwDelay = rhs.m_dwDelay;
	m_dwRepeat = rhs.m_dwRepeat;
	return *this;
}

WORD CD2Action::GetSkillID() const
{
	return m_wSkillID;
}

int CD2Action::GetSide() const
{
	return m_nSide;
}

int CD2Action::GetTarget() const
{
	return m_nTarget;
}

DWORD CD2Action::GetDelay() const
{
	return m_dwDelay;
}

DWORD CD2Action::GetRepeat() const
{
	return m_dwRepeat;
}

BOOL CD2Action::ParseString(LPCSTR lpString)
{
	// information stored in a string in the following format:
	// skill_ID,side,target,delay,repeat
	LPCSTR pStart = lpString;
	LPCSTR pPos = NULL;
	char sz[32] = "";

	// Get the skill id
	pPos = strchr(pStart, ',');
	if (pPos == NULL || pPos - pStart < 1)
		return FALSE;
	::memset(sz, 0, 32);
	strncpy(sz, pStart, pPos - pStart);
	m_wSkillID = atoi(sz);
	pStart = pPos + 1;

	// Get left/right side
	pPos = strchr(pStart, ',');
	if (pPos == NULL || pPos - pStart < 1)
		return FALSE;
	::memset(sz, 0, 32);
	strncpy(sz, pStart, pPos - pStart);
	m_nSide = atoi(sz);
	pStart = pPos + 1;

	// Get target
	pPos = strchr(pStart, ',');
	if (pPos == NULL || pPos - pStart < 1)
		return FALSE;
	::memset(sz, 0, 32);
	strncpy(sz, pStart, pPos - pStart);
	m_nTarget = atoi(sz);
	pStart = pPos + 1;

	// Get cast delay
	pPos = strchr(pStart, ',');
	if (pPos == NULL || pPos - pStart < 1)
		return FALSE;
	::memset(sz, 0, 32);
	strncpy(sz, pStart, pPos - pStart);
	m_dwDelay = atoi(sz);
	pStart = pPos + 1;

	// The remaining string is repeat times
	m_dwRepeat = atoi(pStart);

	return TRUE; // Indicates valid action
}

void CD2Action::ToString(LPSTR lpString) const
{
	sprintf(lpString, "%d,%d,%d,%u,%u", m_wSkillID, m_nSide, m_nTarget, m_dwDelay, m_dwRepeat);
}

void CD2Action::Enable(BOOL bEnable)
{
	m_bEnabled = bEnable;
}

BOOL CD2Action::IsEnabled() const
{
	return m_bEnabled;
}

void CD2Action::GetSideText(LPSTR lpsz) const
{
	if (m_nSide == SAS_LEFT)
		strcpy(lpsz, "左");
	else
		strcpy(lpsz, "右");
}

void CD2Action::GetTargetText(LPSTR lpsz) const
{
	if (m_nTarget == CAT_PINDLESKIN)
	{
		strcpy(lpsz, "Pindleskin本身");
	}
	else if (m_nTarget == CAT_BEYONDPINDLESKIN)
	{
		strcpy(lpsz, "穿越Pindleskin");
	}
	else if (m_nTarget == CAT_BEFOREPINDLESKIN)
	{
		strcpy(lpsz, "角色与Pindleskin之间");
	}
	else
	{
		strcpy(lpsz, "无需目标");
	}
}

void CD2Action::SetDelay(DWORD dwDelay)
{
	m_dwDelay = dwDelay;
}
