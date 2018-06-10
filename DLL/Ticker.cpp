// Ticker.cpp: implementation of the CTicker class.
//
//////////////////////////////////////////////////////////////////////
#ifndef __AFX_WIN__
//#include "stdafx.h"
#endif
#include "Ticker.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CTicker::CTicker(): m_dwCount(0)
{}

CTicker::~CTicker()
{}

void CTicker::ResetTicker(DWORD dwNewCount)
{
	m_dwCount = dwNewCount;
}

void CTicker::IncreaseTicker(DWORD dwIncrease)
{
	m_dwCount += dwIncrease;
}

void CTicker::TickIt(DWORD dwCount)
{
	if (m_dwCount > dwCount)
		m_dwCount -= dwCount;
	else
		m_dwCount = 0;
}

BOOL CTicker::IsTickEnded() const
{
	return m_dwCount == 0;
}

DWORD CTicker::GetRemainer() const
{
	return m_dwCount;
}
