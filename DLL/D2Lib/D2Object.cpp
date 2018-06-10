//////////////////////////////////////////////////////////////////////
// D2Object.cpp: implementation of the CD2Object class.
//////////////////////////////////////////////////////////////////////

#include "D2Object.h"

CD2Object::CD2Object()
{
	::memset(&m_fingerprinter, 0, sizeof(D2OBJFINGERPRINTER));
	InitAttributes(); // Initialization
}

CD2Object::~CD2Object()
{
}

void CD2Object::InitAttributes()
{
	CD2Abstract::InitAttributes(); // Always call base class's method first!!!

	// TODO: Add extra initialization here.
	m_dwObjectID = 0;
	m_wPositionX = 0;
	m_wPositionY = 0;
	m_wType = 0;	
}

void CD2Object::OnGameJoin()
{
	CD2Abstract::OnGameJoin(); // Always call base class's method first!!!
	InitAttributes(); // Initialize attributes upon new game.

	// TODO: Add your implementation here.
	
}

void CD2Object::OnGameLeave()
{
	CD2Abstract::OnGameLeave(); // Always call base class's method first!!!
	InitAttributes(); // Initialize attributes upon leave.

	// TODO: Add your implementation here.
	if (m_dwObjectID != 0)
		NotifyD2Message(OM_DESTROY, (WPARAM)this, MAKELPARAM(m_wPositionX, m_wPositionX));	
}

void CD2Object::OnGamePacketBeforeReceived(const BYTE* aPacket, DWORD aLen)
{
	CD2Abstract::OnGamePacketBeforeReceived(aPacket, aLen); // Always call base class's method first!!!
	if (aPacket == NULL || aLen == 0)
		return; // Verify packet

	// TODO: Add your implementation here.

	//////////////////////////////////////////////////////////////////////
	// Object is Destroyed by the Game (be Gone from Player's Vision) 
	//////////////////////////////////////////////////////////////////////
	if (aLen == 6
		&& aPacket[0] == 0x0a
		//&& aPacket[1] == 0x01
		&& !::memcmp(&m_dwObjectID, aPacket + 2, 4))
	{
		// unit is gone from player's vision
		OnDestroy();
		NotifyDebugMessage(_T("Object destroyed"));
		NotifyD2Message(OM_DESTROY, (WPARAM)m_dwObjectID, MAKELPARAM(m_wPositionX, m_wPositionX));
		InitAttributes();
		return;
	}	
}

void CD2Object::OnGamePacketBeforeSent(const BYTE* aPacket, DWORD aLen)
{
	CD2Abstract::OnGamePacketBeforeSent(aPacket, aLen); // Always call base class's method first!!!
	if (aPacket == NULL || aLen == 0)
		return; // Verify packet

	// TODO: Add your implementation here.
	
}

void CD2Object::OnGameTimerTick()
{
	CD2Abstract::OnGameTimerTick(); // Always call base class's method first!!!

	// TODO: Add your implementation here.
	
}

BOOL CD2Object::VerifyFingerprinter(const BYTE *aPacket, DWORD aLen) const
{
	if (!m_fingerprinter.dwLength || !aPacket)
		return FALSE;

	for (DWORD i = m_fingerprinter.dwPosStart; i + m_fingerprinter.dwLength < aLen && i + m_fingerprinter.dwLength <= m_fingerprinter.dwPosEnd + 1; i++)
	{
		if (!::memcmp(aPacket + i, m_fingerprinter.aData, m_fingerprinter.dwLength))
			return TRUE;
	}

	return FALSE;
}

DWORD CD2Object::GetObjectID() const
{
	return m_dwObjectID;
}

WORD CD2Object::GetPositionX() const
{
	return m_wPositionX;
}

WORD CD2Object::GetPositionY() const
{
	return m_wPositionY;
}

BOOL CD2Object::IsValidObject() const
{
	return m_dwObjectID != 0;
}

BOOL CD2Object::SetFingerprinter(LPCD2OBJFINGERPRINTER lpFingerprinter)
{
	if (lpFingerprinter == NULL)
	{
		::memset(&m_fingerprinter, 0, sizeof(D2OBJFINGERPRINTER));
		return FALSE;
	}

	::memcpy(&m_fingerprinter, lpFingerprinter, sizeof(D2OBJFINGERPRINTER));
	return TRUE;
}

void CD2Object::GetFingerprinter(LPD2OBJFINGERPRINTER lpBuffer) const
{
	if (lpBuffer)
		::memcpy(lpBuffer, &m_fingerprinter, sizeof(D2OBJFINGERPRINTER));
}

WORD CD2Object::GetType() const
{
	return m_wType;
}

void CD2Object::OnDestroy()
{

}