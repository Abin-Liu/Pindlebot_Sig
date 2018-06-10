//////////////////////////////////////////////////////////////////////
// D2Player.cpp: implementation of the CD2Player class.
//////////////////////////////////////////////////////////////////////

#include "D2Player.h"
#include <math.h>
#include <stdlib.h>

BOOL operator==(const PLAYERCORPSE& lhs, const PLAYERCORPSE& rhs)
{
	return lhs.dwCorpseID == rhs.dwCorpseID;
}

BOOL operator>(const PLAYERCORPSE& lhs, const PLAYERCORPSE& rhs)
{
	return lhs.dwCorpseID > rhs.dwCorpseID;
}

#define MAXGOLD(char_level)((char_level) * (DWORD)10000)
#define GOLDSPACE(char_level, inventory_gold)(MAXGOLD(char_level) > (inventory_gold) ? MAXGOLD(char_level) - (inventory_gold) : 0)
#define GUESSLEVEL(inventory_gold)(((inventory_gold) % 10000) ? (inventory_gold) / 10000 + 1 : (inventory_gold) / 10000)


CD2Player::CD2Player()
{
	InitAttributes(); // Initialization
}

CD2Player::~CD2Player()
{
}

void CD2Player::InitAttributes()
{
	CD2Abstract::InitAttributes(); // Always call base class's method first!!!

	// TODO: Add extra initialization here.
	m_dwPlayerID = 0;
	m_pPositionXAddr = NULL;
	m_pPositionYAddr = NULL;
	m_pActAddr = NULL;
	m_pLocationAddr = NULL;
	m_iCharClass = 0;
	m_iCharLevel = 1;
	::memset(m_szCharName, 0, sizeof(m_szCharName));
	m_aSkillList.RemoveAll();
	m_aHostilerList.RemoveAll();
	m_aCorpseList.RemoveAll();	
	m_wHP = 0;
	m_wMana = 0;
	m_dwInventoryGold = 0;
	m_dwStashGold = 0;
	m_wLeftSpell = 0;
	m_wRightSpell = 0;
}

void CD2Player::OnGameJoin(DWORD dwPlayerID, const WORD* pPositionXAddr, const WORD* pPositionYAddr, const BYTE* pActAddr, const DWORD* pLocationAddr, BYTE iCharClass, LPCTSTR lpszCharName)
{
	CD2Abstract::OnGameJoin(); // Always call base class's method first!!!
	InitAttributes(); // Initialize attributes upon new game.

	// TODO: Add your implementation here.
	m_dwPlayerID = dwPlayerID;
	m_pPositionXAddr = pPositionXAddr;
	m_pPositionYAddr = pPositionYAddr;
	m_pActAddr = pActAddr;
	m_pLocationAddr = pLocationAddr;
	m_iCharClass = iCharClass;
	_tcsncpy(m_szCharName, lpszCharName, PLAYER_MAX_NAME);
}

void CD2Player::OnGameLeave()
{
	CD2Abstract::OnGameLeave(); // Always call base class's method first!!!
	InitAttributes(); // Initialize attributes upon leave.

	// TODO: Add your implementation here.
	
}

void CD2Player::OnGamePacketBeforeReceived(const BYTE* aPacket, DWORD aLen)
{
	CD2Abstract::OnGamePacketBeforeReceived(aPacket, aLen); // Always call base class's method first!!!
	if (aPacket == NULL || aLen == 0)
		return; // Verify packet

	// TODO: Add your implementation here.

	//////////////////////////////////////////////////////////
	// Player/Corpse spawned
	//////////////////////////////////////////////////////////
	if (aLen == 26
		&& aPacket[0] == 0x59)
	{
		DWORD dwID;
		::memcpy(&dwID, aPacket + 1, 4);
		if (m_dwPlayerID != dwID)
		{
			// player's corpse appear? check the name
			TCHAR szName[PLAYER_MAX_NAME + 1] = _T("");
			::memcpy(szName, aPacket + 6, PLAYER_MAX_NAME + 1);
			if (!_tcsicmp(m_szCharName, szName))
			{
				// it's my corpse
				PLAYERCORPSE corpse;
				corpse.dwCorpseID = dwID;
				::memcpy(&corpse.x, aPacket + 22, 2);
				::memcpy(&corpse.y, aPacket + 24, 2);
				if (corpse.x > 0 && corpse.y > 0)
				{
					m_aCorpseList.Add(corpse);
					OnCorpse(corpse.dwCorpseID, corpse.x, corpse.y);
					FormatAndNotifyDebugMessage(_T("Player Corpse %08X, %04X, %04X"), corpse.dwCorpseID, corpse.x, corpse.y);
					NotifyD2Message(PM_CORPSE, (WPARAM)corpse.dwCorpseID, MAKELPARAM(corpse.x, corpse.y));
				}
			}
		}

		return;
	}

	//////////////////////////////////////////////////////////////////////
	// Player's Corpse is Destroyed by the Game (be Gone from Player's Vision) 
	//////////////////////////////////////////////////////////////////////
	if (aLen == 6
		&& aPacket[0] == 0x0a
		&& aPacket[1] == 0x00)
	{
		PLAYERCORPSE corpse;
		::memcpy(&corpse.dwCorpseID, aPacket + 2, 4);
		int n = m_aCorpseList.Find(corpse);
		if (n >= 0)
		{
			m_aCorpseList.RemoveAt(n);
		}

		return;
	}
	
	//////////////////////////////////////////////////////////////////////
	// Player's Corpse is Picked Up 
	//////////////////////////////////////////////////////////////////////
	if ( aLen >= 6
		&& aPacket[0] == 0x8e
		&& aPacket[1] == 0x00
		&& !::memcmp(&m_dwPlayerID, aPacket + 2, 4))
	{
		OnCorpsePicked();
		NotifyDebugMessage(_T("Corpse Picked Up"));
		NotifyD2Message(PM_CORPSEPICKEDUP, 0, 0);
		return;
	}	

	//////////////////////////////////////////////////////////
	// Player Character Level
	//////////////////////////////////////////////////////////
	if (aLen > 24
		&& aPacket[0] == 0x5b 
		&& !::memcmp(aPacket + 3, &m_dwPlayerID, 4))
	{
		m_iCharLevel = aPacket[24];
		OnCharLevel(m_iCharLevel);
		FormatAndNotifyDebugMessage(_T("Player level %d"), m_iCharLevel);
		NotifyD2Message(PM_LEVEL, (WPARAM)0, (LPARAM)m_iCharLevel);
		return;
	}

	//////////////////////////////////////////////////////////
	// Player Gold Updates
	//////////////////////////////////////////////////////////
	// inventory gold updates
	if (aPacket[0] == 0x19 && aLen == 2)
	{
		// less than 255 gold update
		m_dwInventoryGold += aPacket[1];
		OnGoldUpdate(m_dwInventoryGold, m_dwStashGold);
		FormatAndNotifyDebugMessage(_T("Inventory Gold %u, Stash Gole %u"), m_dwInventoryGold, m_dwStashGold);
		NotifyD2Message(PM_GOLD, (WPARAM)m_dwInventoryGold, (LPARAM)m_dwStashGold);
		return;
	}

	if (aPacket[0] == 0x1d && aPacket[1] == 0x0e && aLen >= 3)
	{
		// inventory gold BYTE update
		BYTE gTmp = 0;
		::memcpy(&gTmp, &aPacket[2], sizeof(BYTE));
		m_dwInventoryGold = (DWORD)gTmp;
		m_iCharLevel = max(m_iCharLevel, (BYTE)GUESSLEVEL(m_dwInventoryGold));
		OnGoldUpdate(m_dwInventoryGold, m_dwStashGold);
		FormatAndNotifyDebugMessage(_T("Inventory Gold %u, Stash Gole %u"), m_dwInventoryGold, m_dwStashGold);
		NotifyD2Message(PM_GOLD, (WPARAM)m_dwInventoryGold, (LPARAM)m_dwStashGold);
		return;
	}

	if (aPacket[0] == 0x1e && aPacket[1] == 0x0e && aLen >= 4)
	{
		// inventory gold WORD update
		WORD gTmp = 0;
		memcpy(&gTmp, &aPacket[2], sizeof(WORD));
		m_dwInventoryGold = (DWORD)gTmp;
		m_iCharLevel = max(m_iCharLevel, (BYTE)GUESSLEVEL(m_dwInventoryGold));
		OnGoldUpdate(m_dwInventoryGold, m_dwStashGold);
		FormatAndNotifyDebugMessage(_T("Inventory Gold %u, Stash Gole %u"), m_dwInventoryGold, m_dwStashGold);
		NotifyD2Message(PM_GOLD, (WPARAM)m_dwInventoryGold, (LPARAM)m_dwStashGold);
		return;
	}

	if (aPacket[0] == 0x1f && aPacket[1] == 0x0e && aLen >= 6)
	{
		// inventory gold DWORD update
		DWORD gTmp = 0;
		memcpy(&gTmp, &aPacket[2], sizeof(DWORD));
		m_dwInventoryGold = (DWORD)gTmp;
		m_iCharLevel = max(m_iCharLevel, (BYTE)GUESSLEVEL(m_dwInventoryGold));
		OnGoldUpdate(m_dwInventoryGold, m_dwStashGold);
		FormatAndNotifyDebugMessage(_T("Inventory Gold %u, Stash Gole %u"), m_dwInventoryGold, m_dwStashGold);
		NotifyD2Message(PM_GOLD, (WPARAM)m_dwInventoryGold, (LPARAM)m_dwStashGold);
		return;
	}

	// stash gold update
	if (aPacket[0] == 0x1d && aPacket[1] == 0x0f)
	{	
		// stash gold BYTE update
		BYTE gTmp;
		memcpy(&gTmp, &aPacket[2], sizeof(BYTE));
		m_dwStashGold = (DWORD)gTmp;
		OnGoldUpdate(m_dwInventoryGold, m_dwStashGold);
		FormatAndNotifyDebugMessage(_T("Inventory Gold %u, Stash Gole %u"), m_dwInventoryGold, m_dwStashGold);
		NotifyD2Message(PM_GOLD, (WPARAM)m_dwInventoryGold, (LPARAM)m_dwStashGold);
		return;
	}
	
	if (aPacket[0] == 0x1e && aPacket[1] == 0x0f)
	{
		// stash gold WORD update
		WORD gTmp;
		memcpy(&gTmp, &aPacket[2], sizeof(WORD));
		m_dwStashGold = (DWORD)gTmp;
		OnGoldUpdate(m_dwInventoryGold, m_dwStashGold);
		FormatAndNotifyDebugMessage(_T("Inventory Gold %u, Stash Gole %u"), m_dwInventoryGold, m_dwStashGold);
		NotifyD2Message(PM_GOLD, (WPARAM)m_dwInventoryGold, (LPARAM)m_dwStashGold);
		return;
	}
	
	if (aPacket[0] == 0x1f && aPacket[1] == 0x0f)
	{	
		// stash gold DWORD update
		DWORD gTmp;
		memcpy(&gTmp, &aPacket[2], sizeof(DWORD));
		m_dwStashGold = (DWORD)gTmp;
		OnGoldUpdate(m_dwInventoryGold, m_dwStashGold);
		FormatAndNotifyDebugMessage(_T("Inventory Gold %u, Stash Gole %u"), m_dwInventoryGold, m_dwStashGold);
		NotifyD2Message(PM_GOLD, (WPARAM)m_dwInventoryGold, (LPARAM)m_dwStashGold);
		return;
	}	

	//////////////////////////////////////////////////////////
	// Player HP/Mana Updates
	//////////////////////////////////////////////////////////
	if (aPacket[0] == 0x95)
	{
		WORD wOldHP = m_wHP;
		WORD wOldMana = m_wMana;
		m_wHP = *((WORD *)(aPacket + 1)) & 0x0FFF;
		m_wMana = (*((WORD *)(aPacket + 3)) & 0x0FFF) << 1;

		if (wOldHP < m_wHP)
		{
			// HP up
			OnHPUp(wOldHP, m_wHP);
			FormatAndNotifyDebugMessage(_T("HP up: %u - %u"), wOldHP, m_wHP);
			NotifyD2Message(PM_HPUP, (WPARAM)wOldHP, (LPARAM)m_wHP);
		}
		else if (wOldHP > m_wHP)
		{
			// HP down
			OnHPDown(wOldHP, m_wHP);
			FormatAndNotifyDebugMessage(_T("HP down: %u - %u"), wOldHP, m_wHP);
			NotifyD2Message(PM_HPDOWN, (WPARAM)wOldHP, (LPARAM)m_wHP);
		}

		if (wOldMana < m_wMana)
		{
			// mana up
			OnManaUp(wOldMana, m_wMana);
			FormatAndNotifyDebugMessage(_T("Mana up: %u - %u"), wOldMana, m_wMana);
			NotifyD2Message(PM_MANAUP, (WPARAM)wOldMana, (LPARAM)m_wMana);
		}
		else if (wOldMana > m_wMana)
		{
			// mana down
			OnManaDown(wOldMana, m_wMana);
			FormatAndNotifyDebugMessage(_T("Mana down: %u - %u"), wOldMana, m_wMana);
			NotifyD2Message(PM_MANADOWN, (WPARAM)wOldMana, (LPARAM)m_wMana);
		}

		return;
	}

	//////////////////////////////////////////////////////////
	// Player Death
	//////////////////////////////////////////////////////////
	if ( aLen >= 6
		&& aPacket[0] == 0x8e
		&& aPacket[1] == 0x01
		&& !::memcmp(&m_dwPlayerID, aPacket + 2, 4)
		&& GetLocation() != 0x05)
	{
		OnPlayerDeath(GetAct(), GetPositionX(), GetPositionY());
		FormatAndNotifyDebugMessage(_T("Player Died: %d, %04X, %04X"), GetAct(), GetPositionX(), GetPositionY());
		NotifyD2Message(PM_DEATH, (WPARAM)GetAct(), MAKELPARAM(GetPositionX(), GetPositionY()));
		return;
	}

	//////////////////////////////////////////////////////////
	// Hostility
	//////////////////////////////////////////////////////////
	if (   aPacket[0] == 0x8c		
		&& aPacket[9] == 0x08
		&& !::memcmp(&m_dwPlayerID, aPacket + 5, 4))
	{
		DWORD dwHostilerID;
		::memcpy(&dwHostilerID, aPacket + 1, 4);

		m_aHostilerList.Add(dwHostilerID);
		m_aHostilerList.Sort();

		DWORD dwCount = GetHostilerCount();
		OnHostile(dwHostilerID, dwCount);
		FormatAndNotifyDebugMessage(_T("Hostiled by %08X, Hostilers %u"), dwHostilerID, dwCount);
		NotifyD2Message(PM_HOSTILE, (WPARAM)dwHostilerID, (LPARAM)dwCount);
		return;
	}


	//////////////////////////////////////////////////////////
	// Unhostility
	//////////////////////////////////////////////////////////
	if (aPacket[0] == 0x8c
		&& aPacket[9] == 0x00
		&& !::memcmp(&m_dwPlayerID, aPacket + 5, 4))
	{
		DWORD dwUnhostilerID;
		::memcpy(&dwUnhostilerID, aPacket + 1, 4);

		int n = m_aHostilerList.Find(dwUnhostilerID);
		if (n >= 0)
			m_aHostilerList.RemoveAt(n);

		DWORD dwCount = GetHostilerCount();
		OnUnhostile(dwUnhostilerID, dwCount);
		FormatAndNotifyDebugMessage(_T("Unhostiled by %08X, Hostilers %u"), dwUnhostilerID, dwCount);
		NotifyD2Message(PM_UNHOSTILE, (WPARAM)dwUnhostilerID, (LPARAM)dwCount);
		return;
	}

	//////////////////////////////////////////////////////////
	// Other Player Quit
	//////////////////////////////////////////////////////////
	if (aPacket[0] == 0x5c
		&& aLen == 5)
	{
		DWORD dwQuitterID;
		::memcpy(&dwQuitterID, aPacket + 1, 4);
		
		int n = m_aHostilerList.Find(dwQuitterID);
		if (n >= 0)
		{
			m_aHostilerList.RemoveAt(n);		
			DWORD dwCount = GetHostilerCount();
			OnUnhostile(dwQuitterID, dwCount);
			FormatAndNotifyDebugMessage(_T("Unhostiled by %08X, Hostilers %u"), dwQuitterID, dwCount);
			NotifyD2Message(PM_UNHOSTILE, (WPARAM)dwQuitterID, (LPARAM)dwCount);
		}
		
		return;
	}

	//////////////////////////////////////////////////////////
	// Skill List
	//////////////////////////////////////////////////////////
	if (aPacket[0] == 0x94)
	{
		WORD wSkill;
		for (BYTE i = 0, j = 6; i < aPacket[1] && j <= aLen - 3; i++, j += 3)
		{
			::memcpy(&wSkill, aPacket + j, 2);
			m_aSkillList.Add(wSkill);
		}

		m_aSkillList.Sort();
		OnSkillList(aPacket[1]);
		FormatAndNotifyDebugMessage(_T("%d skills retrieved."), aPacket[1]);
		NotifyD2Message(PM_SKILLLIST, (WPARAM)0, (LPARAM)aPacket[1]);
		return;
	}

	//////////////////////////////////////////////////////////
	// Player Left/Right Spell Selected
	//////////////////////////////////////////////////////////
	if (aLen == 13
		&& aPacket[0] == 0x23
		&& aPacket[1] == 0x00
		&& !::memcmp(&m_dwPlayerID, aPacket + 2, 4))
	{
		WORD wSpell;
		::memcpy(&wSpell, aPacket + 7, 2);
		WORD wOldSpell = 0;

		if (aPacket[6] == 0x01)
		{
			// Left spell
			if (m_wLeftSpell != wSpell)
			{
				wOldSpell = m_wLeftSpell;
				m_wLeftSpell = wSpell;
				OnLSpell(wOldSpell, wSpell);
				FormatAndNotifyDebugMessage(_T("Left Spell: %04X - %04X"), wOldSpell, wSpell);
				NotifyD2Message(PM_LSPELL, (WPARAM)wOldSpell, (LPARAM)wSpell);
			}
		}
		else if (aPacket[6] == 0x00)
		{
			// Right spell
			if (m_wRightSpell != wSpell)
			{
				wOldSpell = m_wRightSpell;
				m_wRightSpell = wSpell;
				OnRSpell(wOldSpell, wSpell);
				FormatAndNotifyDebugMessage(_T("Right Spell: %04X - %04X"), wOldSpell, wSpell);
				NotifyD2Message(PM_RSPELL, (WPARAM)wOldSpell, (LPARAM)wSpell);
			}
		}
	}
}

void CD2Player::OnGamePacketBeforeSent(const BYTE* aPacket, DWORD aLen)
{
	CD2Abstract::OnGamePacketBeforeSent(aPacket, aLen); // Always call base class's method first!!!
	if (aPacket == NULL || aLen == 0)
		return; // Verify packet

	// TODO: Add your implementation here.
	
}

void CD2Player::OnGameTimerTick()
{
	CD2Abstract::OnGameTimerTick(); // Always call base class's method first!!!

	// TODO: Add your implementation here.
	
}

DWORD CD2Player::GetHostilerCount() const
{
	return (DWORD)m_aHostilerList.GetSize();
}

void CD2Player::OnSkillList(BYTE iSkillCount)
{

}

void CD2Player::OnCharLevel(BYTE iLevel)
{

}

void CD2Player::OnHPUp(WORD wOldHP, WORD wNewHP)
{

}

void CD2Player::OnHPDown(WORD wOldHP, WORD wNewHP)
{

}

void CD2Player::OnManaUp(WORD wOldMana, WORD wNewMana)
{

}

void CD2Player::OnManaDown(WORD wOldMana, WORD wNewMana)
{

}

void CD2Player::OnPlayerDeath(BYTE iAct, WORD x, WORD y)
{

}

void CD2Player::OnCorpsePicked()
{

}

void CD2Player::OnHostile(DWORD dwHostilerID, DWORD dwHostilerCount)
{

}

void CD2Player::OnUnhostile(DWORD dwUnhostilerID, DWORD dwHostilerCount)
{

}

void CD2Player::OnGoldUpdate(DWORD dwInventoryGold, DWORD dwStashGold)
{

}

BYTE CD2Player::GetAct() const
{
	return m_pActAddr ? *m_pActAddr : 0;
}

BOOL CD2Player::IsValidPlayer() const
{
	return m_dwPlayerID
		&& GetPositionX()
		&& GetPositionY()
		&& m_pLocationAddr
		&& m_pActAddr;
}

WORD CD2Player::GetPositionX() const
{
	return m_pPositionXAddr ? *m_pPositionXAddr : 0;
}

WORD CD2Player::GetPositionY() const
{
	return m_pPositionYAddr ? *m_pPositionYAddr : 0;
}

BYTE CD2Player::GetLevel() const
{
	return m_iCharLevel;
}

DWORD CD2Player::GetLocation() const
{
	return m_pLocationAddr ? *m_pLocationAddr : 0;
}

LPCTSTR CD2Player::GetCharName() const
{
	return m_szCharName;
}

BOOL CD2Player::HasSpell(WORD wSpellID) const
{
	return m_aSkillList.Find(wSpellID) >= 0;
}

WORD CD2Player::GetHP() const
{
	return m_wHP;
}

WORD CD2Player::GetMana() const
{
	return m_wMana;
}

DWORD CD2Player::GetInventoryGold() const
{
	return m_dwInventoryGold;
}

DWORD CD2Player::GetStashGold() const
{
	return m_dwStashGold;
}

DWORD CD2Player::GetCorpseCount() const
{
	return (DWORD)m_aCorpseList.GetSize();
}

BOOL CD2Player::PickupCorpse() const
{
	if (m_aCorpseList.IsEmpty())
	{
		NotifyDebugMessage(_T("Player does not have a nearby corpse."));
		return FALSE;
	}

	PLAYERCORPSE corpse = m_aCorpseList.GetLast();
	return RunToEntity(0, corpse.dwCorpseID, corpse.x, corpse.y) && InteractWithEntity(0, corpse.dwCorpseID);
}

BOOL CD2Player::RunToEntity(DWORD dwType, DWORD dwEntityID, WORD x, WORD y) const
{
	if (!IsValidPlayer())
	{
		NotifyDebugMessage(_T("Player data is not valid."));
		return FALSE;
	}

	BYTE aPacket[16] = { 0 };
	aPacket[0] = 0x04;
	::memcpy(aPacket + 1, &dwType, 4);
	::memcpy(aPacket + 5, &dwEntityID, 4);

	// send 3 times
	if (!SendPacketToServer(aPacket, 9, 3))
	{
		NotifyDebugMessage(_T("Unable to send RunToEntity packet"));
		return FALSE;
	}

	aPacket[0] = 0x10;
	aPacket[1] = 0x00;
	::memcpy(aPacket + 2, &m_dwPlayerID, 4);
	aPacket[6] = 0x18;
	aPacket[7] = (BYTE)dwType;
	::memcpy(aPacket + 8, &dwEntityID, 4);
	::memcpy(aPacket + 12, &x, 2);
	::memcpy(aPacket + 14, &y, 2);

	return SendPacketToGame(aPacket, 16, 3);	
}

BOOL CD2Player::InteractWithEntity(DWORD dwType, DWORD dwEntityID) const
{
	BYTE aPacket[9] = { 0 };
	aPacket[0] = 0x13;
	::memcpy(aPacket + 1, &dwType, 4);
	::memcpy(aPacket + 5, &dwEntityID, 4);
	if (!SendPacketToServer(aPacket, 9, 2))
	{
		NotifyDebugMessage(_T("Unable to send InteractWithEntity packet"));
		return FALSE;
	}

	if (dwType == 0x01)
	{
		aPacket[0] = 0x2f;
		if (!SendPacketToServer(aPacket, 9, 1))
		{
			NotifyDebugMessage(_T("Unable to send InteractWithEntity2 packet"));
			return FALSE;
		}
	}

	return TRUE;
}

BOOL CD2Player::PrepareInteractWithNPC(DWORD dwNpcID, WORD x, WORD y) const
{
	BYTE aPacket[17] = { 0 };
	aPacket[0] = 0x59;
	aPacket[1] = 0x01;
	::memcpy(aPacket + 5, &dwNpcID, 4);
	::memcpy(aPacket + 9, &x, 2);
	::memcpy(aPacket + 13, &y, 2);
	if (!SendPacketToServer(aPacket, 17))
	{
		NotifyDebugMessage(_T("Unable to send PrepareInteractWithNPC packet"));
		return FALSE;
	}

	return TRUE;
}

BOOL CD2Player::TalkToNpc(const CD2Unit *pNpc) const
{
	if (pNpc == NULL || !pNpc->IsValidObject())
	{
		NotifyDebugMessage(_T("NPC data is not valid."));
		return FALSE;
	}

	return PrepareInteractWithNPC(pNpc->GetObjectID(), pNpc->GetPositionX(), pNpc->GetPositionY())
		&& RunToEntity(0x01, pNpc->GetObjectID(), pNpc->GetPositionX(), pNpc->GetPositionY())
		&& InteractWithEntity(0x01, pNpc->GetObjectID());
}

BOOL CD2Player::RunTo(WORD x, WORD y)
{
	if (!IsValidPlayer())
	{
		NotifyDebugMessage(_T("Player data is not valid."));
		return FALSE;
	}

	BYTE aPacket[16] = { 0 };
	aPacket[0] = 0x03;
	::memcpy(&aPacket[1], &x, 2);
	::memcpy(&aPacket[3], &y, 2);

	// send twice
	if (!SendPacketToServer(aPacket, 5, 2))
		return FALSE;

	aPacket[0] = 0x0f;
	aPacket[1] = 0x00;
	::memcpy(aPacket + 2, &m_dwPlayerID, 2);
	aPacket[6] = 0x17;
	::memcpy(aPacket + 7, &x, 2); // New position x
	::memcpy(aPacket + 9, &y, 2); // New position y
	::memcpy(aPacket + 12, &m_pPositionXAddr, 2); // Old position X
	::memcpy(aPacket + 14, &m_pPositionYAddr, 2); // Old position Y
	
	if (!SendPacketToGame(aPacket, 16, 2))
	{
		NotifyDebugMessage(_T("Unable to send RunTo packet"));
		return FALSE;
	}

	return TRUE;
}

BOOL CD2Player::LeaveNpc(const CD2Unit *pNpc) const
{
	if (pNpc == NULL || !pNpc->IsValidObject())
	{
		NotifyDebugMessage(_T("NPC data is not valid."));
		return FALSE;
	}

	return StopInteractWithEntity(0x01, pNpc->GetObjectID());	
}

BOOL CD2Player::StopInteractWithEntity(DWORD dwType, DWORD dwEntityID) const
{
	BYTE aPacket[9] = { 0 };
	aPacket[0] = 0x30;
	::memcpy(&aPacket[1], &dwType, 4);
	::memcpy(&aPacket[5], &dwEntityID, 4);
	
	if (!SendPacketToServer(aPacket, 9, 2))
	{
		NotifyDebugMessage(_T("Unable to send StopInteractWithEntity packet"));
		return FALSE;
	}

	return TRUE;
}

WORD CD2Player::GetDistance(WORD x, WORD y) const
{
	WORD wPlayerX = GetPositionX();
	WORD wPlayerY = GetPositionY();

	if (wPlayerX == 0 || wPlayerY == 0)
		return 0xffff;

	return (WORD)::sqrt((double)(((int)wPlayerX - (int)x) * ((int)wPlayerX - (int)x) + ((int)wPlayerY - (int)y) * ((int)wPlayerY - (int)y)));
}

void CD2Player::OnCorpse(DWORD dwCorpseID, WORD x, WORD y)
{

}

BOOL CD2Player::OpenStaticObject(const CD2StaticObject *pStaticObject) const
{
	if (pStaticObject == NULL || !pStaticObject->IsValidObject())
	{
		NotifyDebugMessage(_T("Object data is not valid."));
		return FALSE;
	}

	return RunToEntity(0x02, pStaticObject->GetObjectID(), pStaticObject->GetPositionX(), pStaticObject->GetPositionY())
		&& InteractWithEntity(0x02, pStaticObject->GetObjectID());
}

BOOL CD2Player::LeaveStaticObject(const CD2StaticObject *pStaticObject) const
{
	if (pStaticObject == NULL || !pStaticObject->IsValidObject())
	{
		NotifyDebugMessage(_T("Object data is not valid."));
		return FALSE;
	}

	return StopInteractWithEntity(0x02, pStaticObject->GetObjectID());
}

BOOL CD2Player::RunTo(const CD2Unit *pNpc) const
{
	if (pNpc == NULL || !pNpc->IsValidObject())
	{
		NotifyDebugMessage(_T("NPC data is not valid."));
		return FALSE;
	}

	return RunToEntity(0x01, pNpc->GetObjectID(), pNpc->GetPositionX(), pNpc->GetPositionY());
}

BOOL CD2Player::RunTo(const CD2StaticObject *pStaticObject) const
{
	if (pStaticObject == NULL || !pStaticObject->IsValidObject())
	{
		NotifyDebugMessage(_T("Object data is not valid."));
		return FALSE;
	}

	return RunToEntity(0x02, pStaticObject->GetObjectID(), pStaticObject->GetPositionX(), pStaticObject->GetPositionY());
}

BOOL CD2Player::EnterWP(const CD2StaticObject *pWP, DWORD dwDestination) const
{
	if (pWP == NULL || !pWP->IsValidObject())
	{
		NotifyDebugMessage(_T("WP data is not valid."));
		return FALSE;
	}

	DWORD dwID = pWP->GetObjectID();
	InteractWithEntity(0x02, dwID);
	BYTE aPacket[9] = { 0 };
	aPacket[0] = 0x49;
	::memcpy(&aPacket[1], &dwID, 4);
	::memcpy(&aPacket[5], &dwDestination, 4);
	return SendPacketToServer(aPacket, 9);
}

BOOL CD2Player::OpenNpcStore(const CD2Unit *pNpc) const
{
	if (pNpc == NULL || !pNpc->IsValidObject())
	{
		NotifyDebugMessage(_T("NPC data is not valid."));
		return FALSE;
	}
	
	DWORD dwNpcID = pNpc->GetObjectID();	
	InteractWithEntity(0x01, dwNpcID);
	
	BYTE aPacket[13] = { 0 };
	aPacket[0] = 0x38;
	aPacket[1] = 0x01;
	::memcpy(&aPacket[5], &dwNpcID, 4);
	if (!SendPacketToServer(aPacket, 13))
	{
		NotifyDebugMessage(_T("Unable to send OpenNpcStore packet"));
		return FALSE;
	}

	return TRUE;
}

BOOL CD2Player::BuyItem(DWORD dwItemID, const CD2Unit *pNpc) const
{
	if (dwItemID == 0 || pNpc == NULL || !pNpc->IsValidObject())
	{
		NotifyDebugMessage(_T("Item/NPC data are not valid."));
		return FALSE;
	}

	DWORD dwNpcID = pNpc->GetObjectID();
	BYTE aPacket[17] = { 0 };
	aPacket[0] = 0x32;
	::memcpy(&aPacket[1], &dwNpcID, 4);
	::memcpy(&aPacket[5], &dwItemID, 4);

	if (!SendPacketToServer(aPacket, 17))
	{
		NotifyDebugMessage(_T("Unable to send BuyItem packet"));
		return FALSE;
	}

	return TRUE;
}

BOOL CD2Player::RedrawClient(BOOL bFade) const
{
	if (!IsValidPlayer())
	{
		NotifyDebugMessage(_T("Player data is not valid."));
		return FALSE;
	}

	BYTE aPacket[11];

	aPacket[0] = 0x15;
	aPacket[1] = 0x00;
	::memcpy(aPacket + 2, &m_dwPlayerID, 4);
	::memcpy(aPacket + 6, m_pPositionXAddr, 2);
	::memcpy(aPacket + 8, m_pPositionYAddr, 2);
	aPacket[10] = bFade ? 0x01 : 0x00;

	if (!SendPacketToServer(aPacket, 11))
	{
		NotifyDebugMessage(_T("Unable to send RedrawClient packet"));
		return FALSE;
	}

	return TRUE;
}

void CD2Player::OnLSpell(WORD wOldSpell, WORD wNewSpell)
{

}

void CD2Player::OnRSpell(WORD wOldSpell, WORD wNewSpell)
{

}

BOOL CD2Player::RepairItem(const CD2Unit *pNpc, DWORD dwItemID) const
{
	if (pNpc == NULL || !pNpc->IsValidObject())
	{
		NotifyDebugMessage(_T("NPC data is not valid."));
		return FALSE;
	}

	DWORD dwNpcID = pNpc->GetObjectID();
	BYTE aPacket[17] = { 0 };
	aPacket[0] = 0x35;
	::memcpy(aPacket + 1, &dwNpcID, 4);

	if (dwItemID)
	{
		// repair a particular item
		::memcpy(aPacket + 5, &dwItemID, 4);
	}
	else
	{
		// repair all
		aPacket[16] = 0x80;
	}

	if (!SendPacketToServer(aPacket, 17, 2))
	{
		NotifyDebugMessage(_T("Unable to send RepairItem packet"));
		return FALSE;
	}

	return TRUE;
}

BOOL CD2Player::Say(LPCTSTR lpszMessage) const
{
	if (lpszMessage == NULL || lpszMessage[0] == _T('\0'))
		return FALSE;

	const int LEN = _tcslen(lpszMessage);
	BYTE* aPacket = new BYTE[LEN + 6];
	memset(aPacket, 0, LEN + 6);

	aPacket[0] = 0x15;
	aPacket[1] = 0x01;
	::memcpy(aPacket + 3, lpszMessage, LEN);
	BOOL bOK = SendPacketToServer(aPacket, LEN + 6);
	if (!bOK)
		NotifyDebugMessage(_T("Unable to send Say packet"));

	delete [] aPacket;
	return bOK;
}

BOOL CD2Player::DepositGoldToStash(DWORD dwAmount) const
{
	if (dwAmount == 0)
		dwAmount = m_dwInventoryGold;

	BYTE aPacket[7] = { 0 };
	WORD g1 = 0, g2 = 0;

	// build deposit packet
	aPacket[0] = 0x4f;
	aPacket[1] = 0x14;

	g2 = WORD(dwAmount / 65536);
	g1 = WORD(dwAmount % 65536);
	
	::memcpy(&aPacket[3], &g2, 2);
	::memcpy(&aPacket[5], &g1, 2);

	if (!SendPacketToServer(aPacket, 7))
	{
		NotifyDebugMessage(_T("Unable to send DepositGoldToStash packet"));
		return FALSE;
	}

	return TRUE;
}

BOOL CD2Player::ResurrectMerc(const CD2Unit *pNpc) const
{
	if (pNpc == NULL || !pNpc->IsValidObject())
	{
		NotifyDebugMessage(_T("NPC data is not valid."));
		return FALSE;
	}

	DWORD dwNpcID = pNpc->GetObjectID();
	BYTE aPacket[5] = { 0 };
	aPacket[0] = 0x62;
	::memcpy(aPacket + 1, &dwNpcID, 4);

	if (!SendPacketToServer(aPacket, 5))
	{
		NotifyDebugMessage(_T("Unable to send ResurrectMerc packet"));
		return FALSE;
	}

	return TRUE;
}

BOOL CD2Player::AntiIdle() const
{
	DWORD dwTick = ::GetTickCount();
	if (dwTick % 2)
	{
		return OverHead(_T("Anti-idle"), FALSE);
	}
	else
	{
		// send a random sound
		BYTE i = BYTE(dwTick % 0x08) + 0x19;
		return SendSound(i);
	}
}

BOOL CD2Player::OverHead(LPCTSTR lpszMessage, BOOL bClientOnly) const
{
	if (lpszMessage == NULL || lpszMessage[0] == _T('\0'))
		return FALSE;

	const int MSGLEN = _tcslen(lpszMessage);
	BYTE* aPacket = new BYTE[MSGLEN + 12];
	::memset(aPacket, 0, MSGLEN + 12);

	BOOL bOK = FALSE;
	if (bClientOnly)
	{
		aPacket[0] = 0x26;
		aPacket[1] = 0x05;
		::memcpy(aPacket + 4, &m_dwPlayerID, 4);
		WORD wRand = (rand() % 65535) + 1;
		::memcpy(aPacket + 8, &wRand, 2);
		::memcpy(aPacket + 11, lpszMessage, MSGLEN); // copy whole string including the tail-null
		bOK = SendPacketToGame(aPacket, MSGLEN + 12);
	}
	else
	{
		aPacket[0] = 0x14;
		::memcpy(aPacket + 3, lpszMessage, MSGLEN);
		bOK = SendPacketToServer(aPacket, MSGLEN + 6);
	}

	if (!bOK)
		NotifyDebugMessage(_T("Unable to send OverHead packet"));

	delete [] aPacket;
	return bOK;
}

BOOL CD2Player::SendSound(BYTE iSound) const
{
	// 0x19 - 0x20
	iSound = max(0x19, iSound);
	iSound = min(0x20, iSound);

	BYTE aPacket[3] = { 0 };
	aPacket[0] = 0x3f;
	aPacket[1] = iSound;

	if (!SendPacketToServer(aPacket, 3))
	{
		NotifyDebugMessage(_T("Unable to send SendSound packet"));
		return FALSE;
	}

	return TRUE;

}

BOOL CD2Player::SelectSpell(WORD wSpell, BOOL bLeft) const
{
	if ((bLeft && wSpell == m_wLeftSpell) || (!bLeft && wSpell == m_wRightSpell))
		return TRUE;

	if (!HasSpell(wSpell))
	{
		FormatAndNotifyDebugMessage(_T("Player does not have spell %04X"), wSpell);
		return FALSE;
	}

	BYTE aPacket[9] = { 0 };
	aPacket[0] = 0x3c;
	memcpy(aPacket + 1, &wSpell, 2);
	aPacket[4] = bLeft ? 0x80 : 0x00;
	aPacket[5] = aPacket[6] =aPacket[7] = aPacket[8] = 0xff;

	if (!SendPacketToServer(aPacket, 9, 2))
	{
		NotifyDebugMessage(_T("Unable to send SelectSpell packet"));
		return FALSE;
	}

	return TRUE;
}

BOOL CD2Player::CastSpell(WORD wSpell, BOOL bLeft, WORD x, WORD y) const
{
	if (!IsValidPlayer())
	{
		NotifyDebugMessage(_T("Player is not valid."));
		return FALSE;
	}

	if (!SelectSpell(wSpell, bLeft))
		return FALSE;

	if (x == 0 || y == 0)
	{
		x = GetPositionX();
		y = GetPositionY();
	}

	BYTE aPacket[15];
	aPacket[0] = bLeft ? 0x05 : 0x0c;
	memcpy(aPacket + 1, &x, 2);
	memcpy(aPacket + 3, &y, 2);	

	if (!SendPacketToServer(aPacket, 5))
	{
		NotifyDebugMessage(_T("Unable to send CastSpell packet"));
		return FALSE;
	}

	aPacket[0] = 0x4d;
	aPacket[1] = 0x00;
	::memcpy(aPacket + 2, &m_dwPlayerID, 4);
	::memcpy(aPacket + 6, &wSpell, 2);
	aPacket[8] = 0x01;
	::memcpy(aPacket + 9, &x, 2);
	::memcpy(aPacket + 11, &y, 2);
	return SendPacketToGame(aPacket, 15);
}

BOOL CD2Player::CastSpell(WORD wSpell, BOOL bLeft, CD2StaticObject *pTarget) const
{
	if (pTarget == NULL || !pTarget->IsValidObject())
	{
		NotifyDebugMessage(_T("Taget is not valid."));
		return FALSE;
	}

	return CastSpellOnEntity(wSpell, bLeft, 0x02, pTarget->GetObjectID());
}

BOOL CD2Player::CastSpell(WORD wSpell, BOOL bLeft, const CD2Unit *pTarget) const
{
	if (pTarget == NULL || !pTarget->IsValidObject())
	{
		NotifyDebugMessage(_T("Taget is not valid."));
		return FALSE;
	}

	return CastSpellOnEntity(wSpell, bLeft, 0x01, pTarget->GetObjectID());
}

BOOL CD2Player::CastSpellOnEntity(WORD wSpell, BOOL bLeft, DWORD dwType, DWORD dwEntityID) const
{
	if (!IsValidPlayer())
	{
		NotifyDebugMessage(_T("Player is not valid."));
		return FALSE;
	}

	if (!SelectSpell(wSpell, bLeft))
		return FALSE;

	BYTE aPacket[16] = { 0 };
	aPacket[0] = bLeft ? 0x06 : 0x0d;
	::memcpy(aPacket + 1, &dwType, 4);
	::memcpy(aPacket + 5, &dwEntityID, 4);

	if (!SendPacketToServer(aPacket, 9, 2))
	{
		NotifyDebugMessage(_T("Unable to send CastSpellOnEntity packet"));
		return FALSE;
	}

	aPacket[0] = 0x4c;
	aPacket[1] = 0x00;
	::memcpy(aPacket + 2, &m_dwPlayerID, 4);
	::memcpy(aPacket + 6, &wSpell, 2);
	aPacket[8] = aPacket[9] = 0x01;
	::memcpy(aPacket + 10, &dwEntityID, 4);	
	return SendPacketToGame(aPacket, 16);
}

WORD CD2Player::GetLeftSpell() const
{
	return m_wLeftSpell;
}

WORD CD2Player::GetRightSpell() const
{	
	return m_wRightSpell;
}

DWORD CD2Player::GetPlayerID() const
{
	return m_dwPlayerID;
}

DWORD CD2Player::GetInventoryGoldLimit() const
{
	return MAXGOLD(m_iCharLevel);
}
