#include "stdafx.h"
#include "D2Monster.h"

CString GetMonsterAttribute(WORD wAttr)
{
	CString str;
	switch (wAttr)
	{
	case PINDLE_BONUS_NONE:
		str = _T("任何-Any");
		break;

	case PINDLE_BONUS_AURA_ENCHANTED:
		str = _T("光环增强-Aura Enchanted");
		break;

	case PINDLE_BONUS_COLD_ENCHANTED:
		str = _T("冰冻增强-Cold Enchanted");
		break;

	case PINDLE_BONUS_CURSED:
		str = _T("诅咒-Cursed");
		break;

	case PINDLE_BONUS_EXTRA_FAST:
		str = _T("超级快速-Extra Fast");
		break;

	case PINDLE_BONUS_EXTRA_STRONG:
		str = _T("超级强壮-Extra Strong");
		break;

	case PINDLE_BONUS_LIGHTNING_ENCHANTED:
		str = _T("闪电增强-Lightning Enchanted");
		break;

	case PINDLE_BONUS_MAGIC_RESISTANT:
		str = _T("魔法抵抗-Magic Resistant");
		break;

	case PINDLE_BONUS_MANA_BURN:
		str = _T("法力燃烧-Mana Burn");
		break;

	case PINDLE_BONUS_FIRE_ENCHANTED:
		str = _T("火系增强-Fire Enchanted");
		break;		

	case PINDLE_BONUS_MULTISHOT:
		str = _T("多重射击-Multishot");
		break;

	case PINDLE_BONUS_SPECTRAL_HIT:
		str = _T("怪异打击-Spectral Hit");
		break;

	case PINDLE_BONUS_STONE_SKIN:
		str = _T("石头皮肤-Stone Skin");
		break;

	case PINDLE_BONUS_TELEPORT:
		str = _T("瞬间转移-Teleport");
		break;

	case UNIT_AURA_HOLYFREEZE:
		str = _T("圣冰光环-Holy Freeze");
		break;

	case UNIT_AURA_BLESSEDAIM:
		str = _T("瞄准光环-Blessed Aim");
		break;

	case UNIT_AURA_MIGHT:
		str = _T("力量光环-Might");
		break;

	case UNIT_AURA_HOLYFIRE:
		str = _T("圣火光环-Holy Fire");
		break;

	case UNIT_AURA_HOLYSHOCK:
		str = _T("圣电光环-Holy Shock");
		break;

	case UNIT_AURA_CONVICTION:
		str = _T("审判光环-Conviction");
		break;

	case UNIT_AURA_FANATICISM:
		str = _T("狂热光环-Fanaticism");
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
