//////////////////////////////////////////////////////////////////////
// D2Unit.cpp: implementation of the CD2Unit class.
//////////////////////////////////////////////////////////////////////

#include "D2Unit.h"

CD2Unit::CD2Unit()
{
	InitAttributes(); // Initialization
}

CD2Unit::~CD2Unit()
{
}

void CD2Unit::InitAttributes()
{
	CD2Object::InitAttributes(); // Always call base class's method first!!!

	// TODO: Add extra initialization here.
	m_bIsMoving = FALSE;	
}

void CD2Unit::OnGameJoin()
{
	CD2Object::OnGameJoin(); // Always call base class's method first!!!
	InitAttributes(); // Initialize attributes upon new game.

	// TODO: Add your implementation here.
	
}

void CD2Unit::OnGameLeave()
{
	CD2Object::OnGameLeave(); // Always call base class's method first!!!
	InitAttributes(); // Initialize attributes upon leave.

	// TODO: Add your implementation here.
	
}

void CD2Unit::OnGamePacketBeforeReceived(const BYTE* aPacket, DWORD aLen)
{
	CD2Object::OnGamePacketBeforeReceived(aPacket, aLen); // Always call base class's method first!!!
	if (aPacket == NULL || aLen == 0)
		return; // Verify packet

	// TODO: Add your implementation here.

	//////////////////////////////////////////////////////////////////////
	// Unit Spawned
	//////////////////////////////////////////////////////////////////////
	if (aPacket[0] == 0xac
		&& aLen >= 13
		&& VerifyFingerprinter(aPacket, aLen))
	{
		// unit spawned
		InitAttributes();
		::memcpy(&m_dwObjectID, aPacket + 1, 4);
		::memcpy(&m_wType, aPacket + 5, 2);
		::memcpy(&m_wPositionX, aPacket + 7, 2);
		::memcpy(&m_wPositionY, aPacket + 9, 2);
		
		OnUnitSpawn(m_dwObjectID, m_wPositionX, m_wPositionY);
		FormatAndNotifyDebugMessage(_T("Unit ID %08X spawned at %04X, %04X"), m_dwObjectID, m_wPositionX, m_wPositionY);
		NotifyD2Message(UM_SPAWN, (WPARAM)m_dwObjectID, MAKELPARAM(m_wPositionX, m_wPositionX));
		return;
	}
	
	//////////////////////////////////////////////////////////////////////
	// Unit Starts Moving
	//////////////////////////////////////////////////////////////////////
	if (aPacket[0] == 0x67 // start moving
		&& aLen == 16
		&& !::memcmp(&m_dwObjectID, aPacket + 1, 4))
	{		
		WORD x = 0;
		WORD y = 0;
		::memcpy(&x, aPacket + 6, 2);
		::memcpy(&y, aPacket + 8, 2);
		m_bIsMoving = TRUE;
		OnStartMove(m_wPositionX, m_wPositionY, x, y);
		FormatAndNotifyDebugMessage(_T("Unit is moving towards %04X, %04X"), m_wPositionX, m_wPositionY);
		NotifyD2Message(UM_STARTMOVE, MAKEWPARAM(m_wPositionX, m_wPositionY), MAKELPARAM(x, y));
		return;
	}

	//////////////////////////////////////////////////////////////////////
	// Unit Ends Moving
	//////////////////////////////////////////////////////////////////////
	if (aPacket[0] == 0x6d
		&& aLen == 10
		&& !::memcmp(&m_dwObjectID, aPacket + 1, 4))
	{
		// object just reached this location
		WORD x = m_wPositionX;
		WORD y = m_wPositionY;		
		::memcpy(&m_wPositionX, aPacket + 5, 2);
		::memcpy(&m_wPositionY, aPacket + 7, 2);
		m_bIsMoving = FALSE;
		OnEndMove(x, y, m_wPositionX, m_wPositionY);
		FormatAndNotifyDebugMessage(_T("Unit has moved to %04X, %04X"), m_wPositionX, m_wPositionY);
		NotifyD2Message(UM_ENDMOVE, MAKEWPARAM(x, y), MAKELPARAM(m_wPositionX, m_wPositionY));
		return;
	}	
}

void CD2Unit::OnGamePacketBeforeSent(const BYTE* aPacket, DWORD aLen)
{
	CD2Object::OnGamePacketBeforeSent(aPacket, aLen); // Always call base class's method first!!!
	if (aPacket == NULL || aLen == 0)
		return; // Verify packet

	// TODO: Add your implementation here.
	
}

void CD2Unit::OnGameTimerTick()
{
	CD2Object::OnGameTimerTick(); // Always call base class's method first!!!

	// TODO: Add your implementation here.
	
}

void CD2Unit::OnUnitSpawn(DWORD dwObjectID, WORD x, WORD y)
{
}

void CD2Unit::OnStartMove(WORD wFromX, WORD wFromY, WORD wToX, WORD wToY)
{

}

void CD2Unit::OnEndMove(WORD wFromX, WORD wFromY, WORD wToX, WORD wToY)
{

}

void CD2Unit::OnDestroy()
{
	CD2Object::OnDestroy(); // Call base class's "OnDestroy" first!!!
	InitAttributes();
}

BOOL CD2Unit::IsMoving() const
{
	return m_bIsMoving;
}