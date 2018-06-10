#ifndef __D2MONSTER_H__
#define __D2MONSTER_H__

#include <windows.h>
#include "BonusDef.h"

CString GetMonsterAttribute(WORD wAttr);

class CD2MonsterCombo
{
public:
	CString ToString();
	CD2MonsterCombo();
	CD2MonsterCombo(WORD wAttr1, WORD wAttr2);

private:
	void SwapAttrs();
	WORD m_wAttr1;
	WORD m_wAttr2;
};

#endif