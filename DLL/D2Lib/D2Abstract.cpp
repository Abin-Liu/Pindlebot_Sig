// D2Abstract.cpp: implementation of the CD2Abstract class.
//
//////////////////////////////////////////////////////////////////////

#include "D2Abstract.h"
#include <stdio.h>

#define MAX_DEBUG_BUF	1024

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CD2Abstract::CD2Abstract()
{
	m_lpfnDebugMessageProc = NULL;
	m_lpDebugData = NULL;
	m_lpszDebugLabel = NULL;
	m_lpfnSendPacketToServerProc = NULL;
	m_lpSendPacketToServerData = NULL;
	m_lpfnSendPacketToGameProc = NULL;
	m_lpSendPacketToGameData = NULL;
	m_lpfnMessageProc = NULL;
	m_lpMessageData = NULL;
}

CD2Abstract::~CD2Abstract()
{
	if (m_lpszDebugLabel != NULL)
		delete [] m_lpszDebugLabel;
}

void CD2Abstract::SetDebugMessageProc(DEBUGMESSAGEPROC lpfn, LPVOID lpData, LPCTSTR lpszLabel)
{
	m_lpfnDebugMessageProc = lpfn;
	m_lpDebugData = lpData;
	if (m_lpszDebugLabel != NULL)
	{
		delete [] m_lpszDebugLabel;
		m_lpszDebugLabel = NULL;
	}

	if (lpszLabel && lpszLabel[0] != _T('\0'))
		m_lpszDebugLabel = _tcsdup(lpszLabel);
}

BOOL CD2Abstract::IsDebugging() const
{
	return m_lpfnDebugMessageProc != NULL;
}

void CD2Abstract::SetD2MessageProc(D2MESSAGEPROC lpfn, LPVOID lpData)
{
	m_lpfnMessageProc = lpfn;
	m_lpMessageData = lpData;
}

BOOL CD2Abstract::NotifyDebugMessage(LPCTSTR lpszDebug) const
{
	if (m_lpfnDebugMessageProc == NULL || lpszDebug == NULL || lpszDebug[0] == _T('\0'))
		return FALSE;

	if (m_lpszDebugLabel)
	{
		TCHAR szMessage[MAX_DEBUG_BUF + 1] = _T("");
		_sntprintf(szMessage, MAX_DEBUG_BUF, _T("%s - %s"), m_lpszDebugLabel, lpszDebug);
		(*m_lpfnDebugMessageProc)((LPCVOID)this, szMessage, m_lpDebugData);
	}
	else
	{
		(*m_lpfnDebugMessageProc)((LPCVOID)this, lpszDebug, m_lpDebugData);
	}
	
	return TRUE;
}

BOOL CD2Abstract::FormatAndNotifyDebugMessage(LPCTSTR lpszFormat, ...) const
{
	if (m_lpfnDebugMessageProc == NULL || lpszFormat == NULL || lpszFormat[0] == _T('\0'))
		return FALSE;

	TCHAR szMessage[MAX_DEBUG_BUF + 1] = _T("");
	va_list args;
	va_start(args, lpszFormat);
	_vsntprintf(szMessage, MAX_DEBUG_BUF, lpszFormat, args);
	va_end(args);
	return NotifyDebugMessage(szMessage);	
}

void CD2Abstract::OnGameJoin()
{
}

void CD2Abstract::OnGameLeave()
{
}

void CD2Abstract::OnGamePacketBeforeReceived(const BYTE* aPacket, DWORD aLen)
{
	// Appeases C++ warning level 4
	aPacket = aPacket;
	aLen = aLen;
}

void CD2Abstract::OnGamePacketBeforeSent(const BYTE *aPacket, DWORD aLen)
{
	// Appeases C++ warning level 4
	aPacket = aPacket;
	aLen = aLen;
}

void CD2Abstract::OnGameTimerTick()
{
}

void CD2Abstract::InitAttributes()
{

}

BOOL CD2Abstract::SendPacketToServer(const BYTE *aPacket, DWORD aLen, int nTimes) const
{
	if (aPacket == NULL || aLen == 0)
		return FALSE;

	if (m_lpfnSendPacketToServerProc == NULL)
	{
		NotifyDebugMessage(_T("D2Lib Error: m_lpfnSendPacketToServerProc is NULL."));
		return FALSE;
	}

	for (int i = 0; i < nTimes; i++)
	{
		if (!(*m_lpfnSendPacketToServerProc)((LPCVOID)this, (BYTE*)aPacket, aLen, m_lpSendPacketToServerData))
			return FALSE;
	}

	return TRUE;
}

BOOL CD2Abstract::SendPacketToGame(const BYTE *aPacket, DWORD aLen, int nTimes) const
{
	if (aPacket == NULL || aLen == 0)
		return FALSE;

	if (m_lpfnSendPacketToGameProc == NULL)
	{
		NotifyDebugMessage(_T("D2Lib Error: m_lpfnSendPacketToGameProc is NULL."));
		return FALSE;
	}
	
	for (int i = 0; i < nTimes; i++)
	{
		if (!(*m_lpfnSendPacketToGameProc)((LPCVOID)this, (BYTE*)aPacket, aLen, m_lpSendPacketToGameData))
			return FALSE;
	}
	return TRUE;
}

void CD2Abstract::SetSendPacketToServerProc(SENDPACKETTOSERVERPROC lpfn, LPVOID lpData)
{
	m_lpfnSendPacketToServerProc = lpfn;
	m_lpSendPacketToServerData = lpData;
}

void CD2Abstract::SetSendPacketToGameProc(SENDPACKETTOGAMEPROC lpfn, LPVOID lpData)
{
	m_lpfnSendPacketToGameProc = lpfn;
	m_lpSendPacketToGameData = lpData;
}

BOOL CD2Abstract::NotifyD2Message(UINT nMessage, WPARAM wParam, LPARAM lParam) const
{
	if (m_lpfnMessageProc == NULL)
		return FALSE;
	
	(*m_lpfnMessageProc)((LPCVOID)this, nMessage, wParam, lParam, m_lpMessageData);
	return TRUE;
}

DWORD CD2Abstract::FormatPacket(BOOL bReceived, const BYTE *aPacket, DWORD aLen, LPTSTR lpszBuffer, DWORD dwMaxChars)
{
	if (lpszBuffer == NULL)
		return 0;

	lpszBuffer[0] = _T('\0');
	if (aPacket == NULL || aLen == 0)
		return 0;

	TCHAR sz[32] = _T("");
	if (bReceived)
		_stprintf(sz, _T("R %d:"));
	else
		_stprintf(sz, _T("R %d:"));

	DWORD dwCopied = _tcslen(sz);
	if (dwCopied > dwMaxChars)
		return 0;

	_tcscat(lpszBuffer, sz);

	for (DWORD i = 0; i < aLen && dwCopied + 3 <= dwMaxChars; i++)
	{
		_stprintf(sz, " %02X", aPacket[i]);
		_tcscat(lpszBuffer, sz);
		dwCopied += 3;
	}

	return dwCopied;
}
