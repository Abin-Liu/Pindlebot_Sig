#include "stdafx.h"
#include "D2Monster.h"

CString GetMonsterAttribute(WORD wAttr)
{
	CString str;
	switch (wAttr)
	{
	case PINDLE_BONUS_NONE:
		str = _T("�κ�-Any");
		break;

	case PINDLE_BONUS_AURA_ENCHANTED:
		str = _T("�⻷��ǿ-Aura Enchanted");
		break;

	case PINDLE_BONUS_COLD_ENCHANTED:
		str = _T("������ǿ-Cold Enchanted");
		break;

	case PINDLE_BONUS_CURSED:
		str = _T("����-Cursed");
		break;

	case PINDLE_BONUS_EXTRA_FAST:
		str = _T("��������-Extra Fast");
		break;

	case PINDLE_BONUS_EXTRA_STRONG:
		str = _T("����ǿ׳-Extra Strong");
		break;

	case PINDLE_BONUS_LIGHTNING_ENCHANTED:
		str = _T("������ǿ-Lightning Enchanted");
		break;

	case PINDLE_BONUS_MAGIC_RESISTANT:
		str = _T("ħ���ֿ�-Magic Resistant");
		break;

	case PINDLE_BONUS_MANA_BURN:
		str = _T("����ȼ��-Mana Burn");
		break;

	case PINDLE_BONUS_FIRE_ENCHANTED:
		str = _T("��ϵ��ǿ-Fire Enchanted");
		break;		

	case PINDLE_BONUS_MULTISHOT:
		str = _T("�������-Multishot");
		break;

	case PINDLE_BONUS_SPECTRAL_HIT:
		str = _T("������-Spectral Hit");
		break;

	case PINDLE_BONUS_STONE_SKIN:
		str = _T("ʯͷƤ��-Stone Skin");
		break;

	case PINDLE_BONUS_TELEPORT:
		str = _T("˲��ת��-Teleport");
		break;

	case UNIT_AURA_HOLYFREEZE:
		str = _T("ʥ���⻷-Holy Freeze");
		break;

	case UNIT_AURA_BLESSEDAIM:
		str = _T("��׼�⻷-Blessed Aim");
		break;

	case UNIT_AURA_MIGHT:
		str = _T("�����⻷-Might");
		break;

	case UNIT_AURA_HOLYFIRE:
		str = _T("ʥ��⻷-Holy Fire");
		break;

	case UNIT_AURA_HOLYSHOCK:
		str = _T("ʥ��⻷-Holy Shock");
		break;

	case UNIT_AURA_CONVICTION:
		str = _T("���й⻷-Conviction");
		break;

	case UNIT_AURA_FANATICISM:
		str = _T("���ȹ⻷-Fanaticism");
		break;

	default:
		break;
	}

	return str;
}

CD2MonsterCombo::CD2MonsterCombo()
{
	m_wAttr1 = PINDLE_BONUS_NONE;
	m_wAttr2 = PINDLE_BONUS_NONE;
}

CD2MonsterCombo::CD2MonsterCombo(WORD wAttr1, WORD wAttr2)
{
	m_wAttr1 = wAttr1;
	m_wAttr2 = wAttr2;
}

CString CD2MonsterCombo::ToString()
{
	SwapAttrs();
	CString s1 = GetMonsterAttribute(m_wAttr1);
	CString s2 = GetMonsterAttribute(m_wAttr2);
	if (s1.IsEmpty() || s2.IsEmpty())
		return _T("");
	else
		return s1 + _T(" + ") + s2;
}

void CD2MonsterCombo::SwapAttrs()
{
	if (m_wAttr1 < m_wAttr2)
	{
		WORD w = m_wAttr1;
		m_wAttr1 = m_wAttr2;
		m_wAttr2 = w;
	}
}
