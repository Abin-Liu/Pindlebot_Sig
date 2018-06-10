//////////////////////////////////////////////////////////////////////
// D2Merc.cpp: implementation of the CD2Merc class.
//////////////////////////////////////////////////////////////////////

#include "D2Merc.h"

CD2Merc::CD2Merc()
{
	InitAttributes(); // Initialization
}

CD2Merc::~CD2Merc()
{
}

void CD2Merc::InitAttributes()
{
	CD2Monster::InitAttributes(); // Always call base class's method first!!!

	// TODO: Add extra initialization here.
	CD2Monster::SetFingerprinter(NULL); // clean merc's fingerprinter
	m_dwPlayerID = 0;
	m_pPlayerXAddr = NULL;
	m_pPlayerXAddr = NULL;
}

void CD2Merc::OnGameJoin(DWORD dwPlayerID, const WORD* pPlayerXAddr, const WORD* pPlayerYAddr)
{
	CD2Monster::OnGameJoin(); // Always call base class's method first!!!
	InitAttributes(); // Initialize attributes upon new game.

	// TODO: Add your implementation here.
	m_dwPlayerID = dwPlayerID;	
	m_pPlayerXAddr = pPlayerXAddr;
	m_pPlayerYAddr = pPlayerYAddr;
}

void CD2Merc::OnGameLeave()
{
	CD2Monster::OnGameLeave(); // Always call base class's method first!!!
	InitAttributes(); // Initialize attributes upon leave.

	// TODO: Add your implementation here.	
	
}

void CD2Merc::OnGamePacketBeforeReceived(const BYTE* aPacket, DWORD aLen)
{
	CD2Monster::OnGamePacketBeforeReceived(aPacket, aLen); // Always call base class's method first!!!
	if (aPacket == NULL || aLen == 0)
		return; // Verify packet

	// TODO: Add your implementation here.

	///////////////////////////////////////////////////
	// Merc Created
	///////////////////////////////////////////////////
	if (aLen > 12
		&& aPacket[0] == 0x81
		&& !::memcmp(&m_dwPlayerID, aPacket + 4, 4))
	{
		DWORD dwID;
		::memcpy(&dwID, aPacket + 8, 4);

		// merc's fingerprinter is its own id
		D2OBJFINGERPRINTER fp;
		fp.dwLength = 4;
		fp.dwPosStart = 1;
		fp.dwPosEnd = 4;
		::memcpy(fp.aData, &dwID, 4);
		CD2Monster::SetFingerprinter(&fp);

		CD2Monster::InitAttributes();
		m_dwObjectID = dwID;
		m_wPositionX = m_pPlayerXAddr ? *m_pPlayerXAddr : 0;
		m_wPositionY = m_pPlayerYAddr ? *m_pPlayerYAddr : 0;
		m_iHPPercent = 100; // Merc's HP is always full upon creation
		
		OnMonsterSpawn(m_dwObjectID, m_wPositionX, m_wPositionY);
		FormatAndNotifyDebugMessage(_T("Monster %08X spawned at %04X, %04X"), m_dwObjectID, m_wPositionX, m_wPositionY);
		NotifyD2Message(MM_SPAWN, (WPARAM)m_dwObjectID, MAKELPARAM(m_wPositionX, m_wPositionX));
		return;
	}

	//////////////////////////////////////////////////////////////////////
	// Merc Life Percentage Update
	//////////////////////////////////////////////////////////////////////
	if (aLen == 10
		&& aPacket[0] == 0x7f
		&& aPacket[1] == 0x00
		&& !::memcmp(&m_dwObjectID, aPacket + 4, 4))
	{
		BYTE iOldPercent = m_iHPPercent;
		m_iHPPercent = aPacket[2];
		if (iOldPercent > m_iHPPercent)
		{
			OnHPPercentDown(iOldPercent, m_iHPPercent);
			FormatAndNotifyDebugMessage(_T("Unit HP percent down: %d%% - %d%%"), iOldPercent, m_iHPPercent);
			NotifyD2Message(MM_HPPERCENTDOWN, (WPARAM)iOldPercent, (LPARAM)m_iHPPercent);
		}
		else if (iOldPercent < m_iHPPercent)
		{
			OnHPPercentUp(iOldPercent, m_iHPPercent);
			FormatAndNotifyDebugMessage(_T("Unit HP percent up: %d%% - %d%%"), iOldPercent, m_iHPPercent);
			NotifyD2Message(MM_HPPERCENTUP, (WPARAM)iOldPercent, (LPARAM)m_iHPPercent);
		}

		return;
	}
}

void CD2Merc::OnGamePacketBeforeSent(const BYTE* aPacket, DWORD aLen)
{
	CD2Monster::OnGamePacketBeforeSent(aPacket, aLen); // Always call base class's method first!!!
	if (aPacket == NULL || aLen == 0)
		return; // Verify packet

	// TODO: Add your implementation here.
	
}

void CD2Merc::OnGameTimerTick()
{
	CD2Monster::OnGameTimerTick(); // Always call base class's method first!!!

	// TODO: Add your implementation here.
	
}
