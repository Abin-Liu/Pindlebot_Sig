//////////////////////////////////////////////////////////////////////
// D2Monster.cpp: implementation of the CD2Monster class.
//////////////////////////////////////////////////////////////////////

#include "D2Monster.h"

// Calculate 0x80 based HP percentage
#define CALC80BASEDHPPERCENT(x) ((BYTE)((double)x * 100.0 / 128.0))

CD2Monster::CD2Monster()
{
	InitAttributes(); // Initialization
}

CD2Monster::~CD2Monster()
{
}

void CD2Monster::InitAttributes()
{
	CD2Unit::InitAttributes(); // Always call base class's method first!!!

	// TODO: Add extra initialization here.
	m_bIsAlive = FALSE;
	m_bIsCorpse = FALSE;
	m_iHPPercent = 0;
	m_iAuraActivated = MONSTER_AURA_NONE;
	m_iAuraAffected = MONSTER_AURA_NONE;	
}

void CD2Monster::OnGameJoin()
{
	CD2Unit::OnGameJoin(); // Always call base class's method first!!!
	InitAttributes(); // Initialize attributes upon new game.

	// TODO: Add your implementation here.
	
}

void CD2Monster::OnGameLeave()
{
	CD2Unit::OnGameLeave(); // Always call base class's method first!!!
	InitAttributes(); // Initialize attributes upon leave.

	// TODO: Add your implementation here.
	
}

void CD2Monster::OnGamePacketBeforeReceived(const BYTE* aPacket, DWORD aLen)
{
	CD2Unit::OnGamePacketBeforeReceived(aPacket, aLen); // Always call base class's method first!!!
	if (aPacket == NULL || aLen == 0)
		return; // Verify packet

	// TODO: Add your implementation here.

	//////////////////////////////////////////////////////////////////////
	// Monster Spawned
	//////////////////////////////////////////////////////////////////////
	if (aPacket[0] == 0xac
		&& aLen >= 13
		&& VerifyFingerprinter(aPacket, aLen))
	{
		InitAttributes();

		::memcpy(&m_dwObjectID, aPacket + 1, 4);
		::memcpy(&m_wType, aPacket + 5, 2);
		::memcpy(&m_wPositionX, aPacket + 7, 2);
		::memcpy(&m_wPositionY, aPacket + 9, 2);
		
		m_bIsAlive = TRUE;
		m_bIsCorpse = FALSE;
		m_iHPPercent = CALC80BASEDHPPERCENT(aPacket[11]);		
		
		OnMonsterSpawn(m_dwObjectID, m_wPositionX, m_wPositionY);
		FormatAndNotifyDebugMessage(_T("Monster %08X spawned at %04X, %04X"), m_dwObjectID, m_wPositionX, m_wPositionY);
		NotifyD2Message(MM_SPAWN, (WPARAM)m_dwObjectID, MAKELPARAM(m_wPositionX, m_wPositionX));
		return;
	}	

	//////////////////////////////////////////////////////////////////////
	// Monster Being Hurt
	//////////////////////////////////////////////////////////////////////
	if (aLen == 12
		&& aPacket[0] == 0x69
		&& aPacket[5] == 0x06
		&& !::memcmp(&m_dwObjectID, aPacket + 1, 4))
	{
		BYTE iOldPercent = m_iHPPercent;
		m_iHPPercent = CALC80BASEDHPPERCENT(aPacket[10]);

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

	//////////////////////////////////////////////////////////////////////
	// Monster Aura
	//////////////////////////////////////////////////////////////////////
	if (aPacket[0] == 0xa8
		&& aLen >= 8
		&& !::memcmp(&m_dwObjectID, aPacket + 2, 4))
	{
		if (aPacket[1] == 0x00) // 0x00-initiative, 0x01-passitive)
		{
			m_iAuraActivated = aPacket[7];
			OnAuraActivated(m_iAuraActivated);

			if (IsDebugging())
			{
				TCHAR szAura[65] = _T("");
				GetAuraName(m_iAuraActivated, szAura, 64);
				FormatAndNotifyDebugMessage(_T("Unit activated aura: %s"), szAura);
			}
			
			NotifyD2Message(MM_AURAACTIVATE, (WPARAM)0, (LPARAM)m_iAuraActivated);
		}
		else
		{
			m_iAuraAffected = aPacket[7];
			OnAuraAffected(m_iAuraAffected);

			if (IsDebugging())
			{
				TCHAR szAura[65] = _T("");
				GetAuraName(m_iAuraAffected, szAura, 64);
				FormatAndNotifyDebugMessage(_T("Unit affected by aura: %s"), szAura);
			}
			
			NotifyD2Message(MM_AURAAFFECTED, (WPARAM)0, (LPARAM)m_iAuraAffected);
		}
		
		return;
	}	

	//////////////////////////////////////////////////////////////////////
	// Monster Death
	//////////////////////////////////////////////////////////////////////
	if ( aLen == 12
		&& aPacket[0] == 0x69
		&& aPacket[5] == 0x08
		&& !::memcmp(&m_dwObjectID, aPacket + 1, 4))
	{
		m_bIsAlive = FALSE;
		m_bIsCorpse = FALSE;
		m_iHPPercent = 0;
		m_bIsMoving = FALSE;
		OnDeath();
		NotifyDebugMessage(_T("Unit died"));
		NotifyD2Message(MM_DEATH, (WPARAM)m_dwObjectID, 0);
		return;
	}

	//////////////////////////////////////////////////////////////////////
	// Monster Corpse Appeared
	//////////////////////////////////////////////////////////////////////
	if ( aLen == 12
		&& aPacket[0] == 0x69
		&& aPacket[5] == 0x09
		&& !::memcmp(&m_dwObjectID, aPacket + 1, 4))
	{
		m_bIsAlive = FALSE;
		m_bIsCorpse = TRUE;
		m_iHPPercent = 0;
		m_bIsMoving = FALSE;
		::memcpy(&m_wPositionX, aPacket + 6, 2);
		::memcpy(&m_wPositionY, aPacket + 8, 2);
		OnCorpseAppeared(m_wPositionX, m_wPositionY);
		FormatAndNotifyDebugMessage(_T("Unit corpse appeared at %04X, %04X"), m_wPositionX, m_wPositionY);
		NotifyD2Message(MM_CORPSE, (WPARAM)m_dwObjectID, MAKELPARAM(m_wPositionX, m_wPositionY));
		return;
	}

	//////////////////////////////////////////////////////////////////////
	// Monster Attacking
	//////////////////////////////////////////////////////////////////////
	if (aLen == 21
		&& aPacket[0] == 0x68
		&& !::memcmp(&m_dwObjectID, aPacket + 1, 4))
	{
		::memcpy(&m_wPositionX, aPacket + 6, 2);
		::memcpy(&m_wPositionY, aPacket + 8, 2);

		// attacking whom?
		DWORD dwVictimID = 0;
		::memcpy(&dwVictimID, aPacket + 11, 4);
		OnAttack(m_wPositionX, m_wPositionY, dwVictimID);
		FormatAndNotifyDebugMessage(_T("Unit is attacking %08X at %04X, %04X"), dwVictimID, m_wPositionX, m_wPositionY);
		NotifyD2Message(MM_ATTACK, (WPARAM)dwVictimID, MAKELPARAM(m_wPositionX, m_wPositionY));
		return;
	}

	//////////////////////////////////////////////////////////////////////
	// Monster Under Attack
	//////////////////////////////////////////////////////////////////////
	if (aLen == 21
		&& aPacket[0] == 0x68
		&& !::memcmp(&m_dwObjectID, aPacket + 11, 4))
	{
		// attacked by whom?
		DWORD dwAttackerID = 0;
		::memcpy(&dwAttackerID, aPacket + 1, 4);
		OnUnderAttack(m_wPositionX, m_wPositionY, dwAttackerID);
		FormatAndNotifyDebugMessage(_T("Unit is attacked by %08X at %04X, %04X"), dwAttackerID, m_wPositionX, m_wPositionY);
		NotifyD2Message(MM_UNDERATTACK, (WPARAM)dwAttackerID, MAKELPARAM(m_wPositionX, m_wPositionY));
		return;
	}	

	//////////////////////////////////////////////////////////////////////
	// Monster Revived by It's Resurrecter (Shaman, Greater Mummy, etc) 
	//////////////////////////////////////////////////////////////////////
	if (aLen == 16
		&& aPacket[0] == 0x6b
		&& !::memcmp(&m_dwObjectID, aPacket + 1, 4))
	{
		::memcpy(&m_wPositionX, aPacket + 12, 2);
		::memcpy(&m_wPositionY, aPacket + 14, 2);
		m_bIsAlive = TRUE;
		m_bIsCorpse = FALSE;
		m_bIsMoving = FALSE;
		m_iHPPercent = 100;
		m_iAuraActivated = MONSTER_AURA_NONE;
		m_iAuraAffected = MONSTER_AURA_NONE;		
		OnRevived(m_wPositionX, m_wPositionY);
		FormatAndNotifyDebugMessage(_T("Unit is revived at %04X, %04X"), m_wPositionX, m_wPositionY);
		NotifyD2Message(MM_REVIVED, (WPARAM)1, MAKELPARAM(m_wPositionX, m_wPositionY));
		return;
	}

	//////////////////////////////////////////////////////////////////////
	// Monster Revived by Itself (Act5 Zombies) 
	//////////////////////////////////////////////////////////////////////
	if (aLen == 17
		&& aPacket[0] == 0x4d
		&& !::memcmp(&m_dwObjectID, aPacket + 2, 4))
	{
		m_bIsAlive = TRUE;
		m_bIsCorpse = FALSE;
		m_iHPPercent = 100;
		m_bIsMoving = FALSE;
		m_iAuraActivated = MONSTER_AURA_NONE;
		m_iAuraAffected = MONSTER_AURA_NONE;		
		OnRevived(m_wPositionX, m_wPositionY);
		FormatAndNotifyDebugMessage(_T("Unit is revived at %04X, %04X"), m_wPositionX, m_wPositionY);
		NotifyD2Message(MM_REVIVED, (WPARAM)0, MAKELPARAM(m_wPositionX, m_wPositionY));
		return;
	}	
}

void CD2Monster::OnGamePacketBeforeSent(const BYTE* aPacket, DWORD aLen)
{
	CD2Unit::OnGamePacketBeforeSent(aPacket, aLen); // Always call base class's method first!!!
	if (aPacket == NULL || aLen == 0)
		return; // Verify packet

	// TODO: Add your implementation here.
	
}

void CD2Monster::OnGameTimerTick()
{
	CD2Unit::OnGameTimerTick(); // Always call base class's method first!!!

	// TODO: Add your implementation here.
	
}

BOOL CD2Monster::IsAlive() const
{
	return m_bIsAlive;
}

BOOL CD2Monster::IsCorpse() const
{
	return m_bIsCorpse;
}

BYTE CD2Monster::GetHPPercent() const
{
	return m_iHPPercent;
}

DWORD CD2Monster::GetAuraName(BYTE iAura, LPTSTR lpszBuffer, DWORD dwMaxChars)
{
	if (lpszBuffer == NULL)
		return 0;

	lpszBuffer[0] = _T('\0');
	if (dwMaxChars == 0)
		return 0;

	switch (iAura)
	{
	case MONSTER_AURA_HOLYFREEZE:
		_tcsncpy(lpszBuffer, _T("Holy Freeze"), dwMaxChars);
		break;

	case MONSTER_AURA_BLESSEDAIM:
		_tcsncpy(lpszBuffer, _T("Blessed Aim"), dwMaxChars);
		break;

	case MONSTER_AURA_MIGHT:
		_tcsncpy(lpszBuffer, _T("Might"), dwMaxChars);
		break;

	case MONSTER_AURA_HOLYFIRE:
		_tcsncpy(lpszBuffer, _T("Holy Fire"), dwMaxChars);
		break;

	case MONSTER_AURA_HOLYSHOCK:
		_tcsncpy(lpszBuffer, _T("Holy Shock"), dwMaxChars);
		break;

	case MONSTER_AURA_CONVICTION:
		_tcsncpy(lpszBuffer, _T("Conviction"), dwMaxChars);
		break;

	case MONSTER_AURA_FANATICISM:
		_tcsncpy(lpszBuffer, _T("Fanaticism"), dwMaxChars);
		break;

	case MONSTER_AURA_SALVATION:
		_tcsncpy(lpszBuffer, _T("Salvation"), dwMaxChars);
		break;

	case MONSTER_AURA_REDEMPTION:
		_tcsncpy(lpszBuffer, _T("Redemption"), dwMaxChars);
		break;

	case MONSTER_AURA_MEDITATION:
		_tcsncpy(lpszBuffer, _T("Meditation"), dwMaxChars);
		break;

	case MONSTER_AURA_VIGOR:
		_tcsncpy(lpszBuffer, _T("Vigor"), dwMaxChars);
		break;

	case MONSTER_AURA_RESISTLIGHTNING:
		_tcsncpy(lpszBuffer, _T("Resist Lightning"), dwMaxChars);
		break;

	case MONSTER_AURA_CLEANSING:
		_tcsncpy(lpszBuffer, _T("Cleansing"), dwMaxChars);
		break;

	case MONSTER_AURA_RESISTCOLD:
		_tcsncpy(lpszBuffer, _T("Resist Cold"), dwMaxChars);
		break;

	case MONSTER_AURA_DEFIANCE:
		_tcsncpy(lpszBuffer, _T("Defiance"), dwMaxChars);
		break;

	case MONSTER_AURA_RESISTFIRE:
		_tcsncpy(lpszBuffer, _T("Resist Fire"), dwMaxChars);
		break;

	case MONSTER_AURA_PRAYER:
		_tcsncpy(lpszBuffer, _T("Prayer"), dwMaxChars);
		break;

	case MONSTER_AURA_SANCTUARY:
		_tcsncpy(lpszBuffer, _T("Sanctuary"), dwMaxChars);
		break;

	case MONSTER_AURA_CONCENTRATION:
		_tcsncpy(lpszBuffer, _T("Concentration"), dwMaxChars);
		break;

	case MONSTER_AURA_THORNS:
		_tcsncpy(lpszBuffer, _T("Thorns"), dwMaxChars);
		break;

	default:
		break;
	}
	
	return _tcslen(lpszBuffer);
}

void CD2Monster::OnAuraActivated(BYTE iAura)
{

}

void CD2Monster::OnAuraAffected(BYTE iAura)
{

}

void CD2Monster::OnHPPercentUp(BYTE iOldPercent, BYTE iNewPercent)
{

}

void CD2Monster::OnHPPercentDown(BYTE iOldPercent, BYTE iNewPercent)
{

}

void CD2Monster::OnStartMove(WORD wFromX, WORD wFromY, WORD wToX, WORD wToY)
{
	CD2Unit::OnStartMove(wFromX, wFromY, wToX, wToY);
	
	// if an object is moving, it is not dead
	m_bIsAlive = TRUE;
	m_bIsCorpse = FALSE;
}

void CD2Monster::OnEndMove(WORD wFromX, WORD wFromY, WORD wToX, WORD wToY)
{
	CD2Unit::OnEndMove(wFromX, wFromY, wToX, wToY);
	
	// if an object is moving, it is not dead
	m_bIsAlive = TRUE;
	m_bIsCorpse = FALSE;
}

void CD2Monster::OnAttack(WORD x, WORD y, DWORD dwVictimID)
{

}

void CD2Monster::OnUnderAttack(WORD x, WORD y, DWORD dwAttackerID)
{

}

void CD2Monster::OnCorpseAppeared(WORD x, WORD y)
{

}

void CD2Monster::OnDestroy()
{
	CD2Unit::OnDestroy(); // Call base class's "OnDestroy" first!!!
	InitAttributes();
}

BYTE CD2Monster::GetActivatedAura() const
{
	return m_iAuraActivated;
}

BYTE CD2Monster::GetAffectedAura() const
{
	return m_iAuraAffected;
}

void CD2Monster::OnRevived(WORD x, WORD y)
{

}

void CD2Monster::OnDeath()
{

}

void CD2Monster::OnMonsterSpawn(DWORD dwObjectID, WORD x, WORD y)
{

}
