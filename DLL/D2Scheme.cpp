//#include "stdafx.h"
#include "D2Scheme.h"

//////////////////////////////////////////////////////////////////
// Implementation of CD2Scheme class
//////////////////////////////////////////////////////////////////

CD2Scheme::CD2Scheme()
{
	::memset(m_szCharName, 0, _CHARNAME + 1);
	::memset(m_szSafetyScheme, 0, 64);
	m_byCharClass = CCD_UNKNOWN;
}

CD2Scheme::CD2Scheme(LPCSTR lpCharName, BYTE byCharClass, LPCSTR lpSafetyScheme)
{
	::memset(m_szCharName, 0, _CHARNAME + 1);
	::strncpy(m_szCharName, lpCharName, _CHARNAME);
	::memset(m_szSafetyScheme, 0, 64);
	::strncpy(m_szSafetyScheme, lpSafetyScheme, 64);
	m_byCharClass = byCharClass;
}

void CD2Scheme::SetScheme(LPCSTR lpCharName, BYTE byCharClass, LPCSTR lpSafetyScheme)
{
	::memset(m_szCharName, 0, _CHARNAME + 1);
	::strncpy(m_szCharName, lpCharName, _CHARNAME);
	::memset(m_szSafetyScheme, 0, 64);
	::strncpy(m_szSafetyScheme, lpSafetyScheme, 64);
	m_byCharClass = byCharClass;
}

LPCSTR CD2Scheme::GetSafetyScheme() const
{
	return m_szSafetyScheme;
}

void CD2Scheme::SetSafety(LPCTSTR lpSafety)
{
	::memset(m_szSafetyScheme, 0, 64);
	::strncpy(m_szSafetyScheme, lpSafety, 64);
}

void CD2Scheme::LimitMinDelays(DWORD dwMinDelay)
{
	int i = 0;

	for (i = 0; i < m_aBeforeRed.GetSize(); i++)
	{
		if (m_aBeforeRed[i].GetDelay() < dwMinDelay)
			m_aBeforeRed[i].SetDelay(dwMinDelay);
	}

	for (i = 0; i < m_aAfterRed.GetSize(); i++)
	{
		if (m_aAfterRed[i].GetDelay() < dwMinDelay)
			m_aAfterRed[i].SetDelay(dwMinDelay);
	}

	for (i = 0; i < m_aOnPindleskin.GetSize(); i++)
	{
		if (m_aOnPindleskin[i].GetDelay() < dwMinDelay)
			m_aOnPindleskin[i].SetDelay(dwMinDelay);
	}

	for (i = 0; i < m_aOnCorpose.GetSize(); i++)
	{
		if (m_aOnCorpose[i].GetDelay() < dwMinDelay)
			m_aOnCorpose[i].SetDelay(dwMinDelay);
	}
}
