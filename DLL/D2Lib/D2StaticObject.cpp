//////////////////////////////////////////////////////////////////////
// D2StaticObject.cpp: implementation of the CD2StaticObject class.
//////////////////////////////////////////////////////////////////////

#include "D2StaticObject.h"

CD2StaticObject::CD2StaticObject()
{
	InitAttributes(); // Initialization
}

CD2StaticObject::~CD2StaticObject()
{
}

void CD2StaticObject::InitAttributes()
{
	CD2Object::InitAttributes(); // Always call base class's method first!!!

	// TODO: Add extra initialization here.
	
}

void CD2StaticObject::OnGameJoin()
{
	CD2Object::OnGameJoin(); // Always call base class's method first!!!
	InitAttributes(); // Initialize attributes upon new game.

	// TODO: Add your implementation here.
	
}

void CD2StaticObject::OnGameLeave()
{
	CD2Object::OnGameLeave(); // Always call base class's method first!!!
	InitAttributes(); // Initialize attributes upon leave.

	// TODO: Add your implementation here.
	
}

void CD2StaticObject::OnGamePacketBeforeReceived(const BYTE* aPacket, DWORD aLen)
{
	CD2Object::OnGamePacketBeforeReceived(aPacket, aLen); // Always call base class's method first!!!
	if (aPacket == NULL || aLen == 0)
		return; // Verify packet

	// TODO: Add your implementation here.

	//////////////////////////////////////////////////////////////////////
	// Static Object Spawned
	//////////////////////////////////////////////////////////////////////
	if (aLen > 12
		&&aPacket[0] == 0x51
		&& VerifyFingerprinter(aPacket, aLen))
	{
		InitAttributes();
		::memcpy(&m_dwObjectID, aPacket + 2, 4);
		::memcpy(&m_wType, aPacket + 6, 2);
		::memcpy(&m_wPositionX, aPacket + 8, 2);
		::memcpy(&m_wPositionY, aPacket + 10, 2);
		OnStaticObjectSpawn(m_dwObjectID, m_wPositionX, m_wPositionY);
		FormatAndNotifyDebugMessage(_T("Static Object %08X spawned at %04X, %04X"), m_dwObjectID, m_wPositionX, m_wPositionY);
		NotifyD2Message(SM_SPAWN, (WPARAM)m_dwObjectID, MAKELPARAM(m_wPositionX, m_wPositionX));
		return;
	}	
}

void CD2StaticObject::OnGamePacketBeforeSent(const BYTE* aPacket, DWORD aLen)
{
	CD2Object::OnGamePacketBeforeSent(aPacket, aLen); // Always call base class's method first!!!
	if (aPacket == NULL || aLen == 0)
		return; // Verify packet

	// TODO: Add your implementation here.
	
}

void CD2StaticObject::OnGameTimerTick()
{
	CD2Object::OnGameTimerTick(); // Always call base class's method first!!!

	// TODO: Add your implementation here.
	
}

void CD2StaticObject::OnStaticObjectSpawn(DWORD dwObjectID, WORD x, WORD y)
{

}