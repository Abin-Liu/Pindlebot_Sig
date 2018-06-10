// Pindleskin.cpp: implementation of the CPindleskin class.
//
//////////////////////////////////////////////////////////////////////

//#include "stdafx.h"
#include "Pindleskin.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CPindleskin::CPindleskin()
{
	InitAttributes();

	// Fingerprinter for pindleskin
	const D2OBJFINGERPRINTER FP_PINDLESKIN = {{0xA0, 0xA1, 0x01, 0x10}, 4, 15, 18}; // Pindleskin
	CD2Object::SetFingerprinter(&FP_PINDLESKIN);
}

CPindleskin::~CPindleskin()
{

}

void CPindleskin::InitAttributes()
{
	CD2Monster::InitAttributes();
	::memset(m_aBonuses, PINDLE_BONUS_NONE, PINDLE_MAX_BONUS);
}

void CPindleskin::OnGameJoin()
{
	CD2Monster::OnGameJoin();
	InitAttributes();
}

void CPindleskin::OnGameLeave()
{
	CD2Monster::OnGameLeave();
	InitAttributes();
}

void CPindleskin::OnGamePacketBeforeReceived(const BYTE* aPacket, DWORD aLen)
{
	CD2Monster::OnGamePacketBeforeReceived(aPacket, aLen);

	if (!aPacket || !aLen)
		return;

	//////////////////////////////////////////////////////////////////////
	// Pindleskin Spawned
	//////////////////////////////////////////////////////////////////////
	if (aPacket[0] == 0xac
		&& aLen >= 25
		&& !::memcmp(&m_dwObjectID, aPacket + 1, 4))
	{
		::memset(m_aBonuses, PINDLE_BONUS_NONE, PINDLE_MAX_BONUS);
		for (DWORD i = 0; i < PINDLE_MAX_BONUS && i < aLen - 24; i++)
			m_aBonuses[i] = aPacket[i + 20];
		OnBonuses(m_aBonuses);
		
		if (IsDebugging())
		{
			TCHAR szMessage[256] = _T("");
			int nCount = 0;
			
			for (int i = 0; i < PINDLE_MAX_BONUS; i++)
			{
				TCHAR szBonus[64] = _T("");
				if (GetBonusName(m_aBonuses[i], szBonus, 63))
				{
					if (nCount > 0)
						_tcscat(szMessage, _T(" + "));
					_tcscat(szMessage, szBonus);					
					nCount++;
				}				
			}

			NotifyDebugMessage(szMessage);
		}

		NotifyD2Message(PM_BONUSES, (WPARAM)this, (LPARAM)m_aBonuses);
		return;
	}
}

const BYTE* CPindleskin::GetBonuses() const
{
	return m_aBonuses;
}

DWORD CPindleskin::GetBonusName(BYTE iBonus, LPTSTR lpszBuffer, DWORD dwMaxChars)
{
	if (lpszBuffer == NULL)
		return 0;

	lpszBuffer[0] = _T('\0');
	if (dwMaxChars == 0)
		return 0;

	switch (iBonus)
	{
	case PINDLE_BONUS_AURA_ENCHANTED:
		_tcsncpy(lpszBuffer, _T("Aura Enchanted"), dwMaxChars);
		break;

	case PINDLE_BONUS_COLD_ENCHANTED:
		_tcsncpy(lpszBuffer, _T("Cold Enchanted"), dwMaxChars);
		break;

	case PINDLE_BONUS_CURSED:
		_tcsncpy(lpszBuffer, _T("Cursed"), dwMaxChars);
		break;

	case PINDLE_BONUS_EXTRA_FAST:
		_tcsncpy(lpszBuffer, _T("Extra Fast"), dwMaxChars);
		break;

	case PINDLE_BONUS_EXTRA_STRONG:
		_tcsncpy(lpszBuffer, _T("Extra Strong"), dwMaxChars);
		break;

	case PINDLE_BONUS_LIGHTNING_ENCHANTED:
		_tcsncpy(lpszBuffer, _T("Lightning Enchanted"), dwMaxChars);
		break;

	case PINDLE_BONUS_FIRE_ENCHANTED:
		_tcsncpy(lpszBuffer, _T("Fire Enchanted"), dwMaxChars);
		break;

	case PINDLE_BONUS_MAGIC_RESISTANT:
		_tcsncpy(lpszBuffer, _T("Magic Resistant"), dwMaxChars);
		break;

	case PINDLE_BONUS_MANA_BURN:
		_tcsncpy(lpszBuffer, _T("Mana Burn"), dwMaxChars);
		break;

	case PINDLE_BONUS_MULTISHOT:
		_tcsncpy(lpszBuffer, _T("Multishot"), dwMaxChars);
		break;

	case PINDLE_BONUS_SPECTRAL_HIT:
		_tcsncpy(lpszBuffer, _T("Spectral Hit"), dwMaxChars);
		break;

	case PINDLE_BONUS_STONE_SKIN:
		_tcsncpy(lpszBuffer, _T("Stone Skin"), dwMaxChars);
		break;

	case PINDLE_BONUS_TELEPORT:
		_tcsncpy(lpszBuffer, _T("Teleport"), dwMaxChars);
		break;

	default:
		break;
	}

	return _tcslen(lpszBuffer);
}

void CPindleskin::OnBonuses(const BYTE* aBonuses)
{
	aBonuses = aBonuses;
}

void CPindleskin::OnDestroy()
{
	CD2Monster::OnDestroy();

	InitAttributes();
}
