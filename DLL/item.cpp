#include "item.h"
#include <string.h>

class CBitFields
{
public:
	const BYTE *data;
	DWORD pos;
	DWORD max;
	CBitFields(const BYTE *d, DWORD mx)
	{
		data = d; 
		pos = 0; 
		max = (mx * 8);  // convert to max bits
	}

	static DWORD GetBitField(const BYTE* data, DWORD pos, DWORD len, DWORD max)
	{
		if(pos + len > max)
		{
			// if we are already at or beyond the end of packet, return 0;
			if(pos >= max)
			{
				return 0;
			}
			// otherwise adjust len to all remaining bits

			len = max - pos;
		}

		return (DWORD)(*(unsigned __int64 *)(data+pos/8)<<(64-len-(pos&7))>>(64-len));
	}

	DWORD GetField(DWORD len) {	return GetBitField(data, (pos+=len)-len, len, max);	}
};

BOOL ParseItemDrop(const BYTE *aPacket, DWORD aLen, ITEM& item)
{
	if (aPacket == NULL || aLen < 3 || aPacket[0] != 0x9c)
		return FALSE;
	
	if (aPacket[1] != ITEM_ACTION_NEW_GROUND
		&& aPacket[1] != ITEM_ACTION_DROP
		&& aPacket[1] != ITEM_ACTION_OLD_GROUND)
		return FALSE;
	
	return ParseItemInfo(aPacket, aLen, item);
}

BOOL GetItemCode(const BYTE *aPacket, DWORD aLen, LPTSTR szBuffer)
{
	if (szBuffer == NULL)
		return FALSE;
	
	::memset(szBuffer, 0, (ITEM_CODE_LEN + 1) * sizeof(TCHAR));
	if(aPacket[0] != 0x9c && aPacket[0] != 0x9d)
		return FALSE;

	CBitFields iPacket(aPacket,aLen);	
	iPacket.GetField((aPacket[0] == 0x9d) ? 164 : 124);

	szBuffer[0] = (TCHAR)iPacket.GetField(8);
	szBuffer[1] = (TCHAR)iPacket.GetField(8);
	szBuffer[2] = (TCHAR)iPacket.GetField(8);
	return _tcslen(szBuffer) == ITEM_CODE_LEN;
}

BOOL IsItemMatchConfig(const ITEMCONFIG& config, const ITEM& item)
{
	if (_tcsicmp(config.szItemCode, item.szItemCode) != 0)
		return FALSE;

	if (config.iLevel != ITEM_UNSET && config.iLevel != item.iLevel)
		return FALSE;

	if (config.iSocketed != ITEM_UNSET && config.iSocketed != item.iSocketed)
		return FALSE;

	if (config.iIdentified != ITEM_UNSET && config.iIdentified != item.iIdentified)
		return FALSE;

	if (config.iEthereal != ITEM_UNSET && config.iEthereal != item.iEthereal)
		return FALSE;

	if (config.iType != ITEM_UNSET && config.iType != item.iType) 
		return FALSE;	

	return TRUE;
}

BOOL ParseItemInfo(const BYTE *aPacket, DWORD aLen, ITEM& item)
{
	if (aPacket == NULL || aLen < 3)
		return FALSE;

	CBitFields iPacket(aPacket, aLen);

	iPacket.GetField(24);
	
	item.iType = (BYTE)iPacket.GetField(8);

	item.dwItemID = iPacket.GetField(32);

	// flags
	iPacket.GetField(4);
	item.iIdentified = (BYTE)iPacket.GetField(1);
	iPacket.GetField(6);
	item.iSocketed = (BYTE)iPacket.GetField(1);
	iPacket.GetField(4);
	BYTE iEar = (BYTE)iPacket.GetField(1);
	if (iEar)
		return FALSE; // who want to pickup a fucking ear???

	iPacket.GetField(5);
	item.iEthereal = (BYTE)iPacket.GetField(1);
	iPacket.GetField(19);	
	item.iLocation = (BYTE)iPacket.GetField(3);
	item.wPositionX = (WORD)iPacket.GetField(16);
	item.wPositionY = (WORD)iPacket.GetField(16);

	// itemcode string;
	item.szItemCode[0] = (TCHAR)iPacket.GetField(8);
	item.szItemCode[1] = (TCHAR)iPacket.GetField(8);
	item.szItemCode[2] = (TCHAR)iPacket.GetField(8);
	iPacket.GetField(8);	

	if (_tcslen(item.szItemCode) != ITEM_CODE_LEN)
		return FALSE; // invalid code

	// gold is a special case
	if(_tcsicmp(item.szItemCode, _T("gld")) == 0)
	{
		iPacket.GetField(1);
		item.iLevel = ITEM_UNSET;
		item.dwGoldAmount = iPacket.GetField(32);
		return TRUE;
	}

	// more special cases need to look into properly decoding them
	if(_tcsicmp(item.szItemCode, _T("ibk")) == 0 ||
		_tcsicmp(item.szItemCode, _T("tbk")) == 0 ||
		_tcsicmp(item.szItemCode, _T("key")) == 0)
	{
		item.iLevel = ITEM_UNSET;
		return TRUE;
	}	

	// packet not long enough to read ItemLevel
	if(iPacket.pos + 14 >= iPacket.max)
	{
		item.iLevel = ITEM_UNSET;
		return TRUE;
	}

	iPacket.GetField(10);
	item.iLevel = (BYTE)iPacket.GetField(4);

	return TRUE;
}

BOOL IsGold(LPCTSTR lpszItemCode)
{
	return lpszItemCode != NULL && _tcsicmp(lpszItemCode, ITEM_GOLD) == 0;
}

BOOL IsRejuv(LPCTSTR lpszItemCode)
{
	return lpszItemCode != NULL && _tcsicmp(lpszItemCode, ITEM_REJUV) == 0;
}

BOOL IsFullRejuv(LPCTSTR lpszItemCode)
{
	return lpszItemCode != NULL && _tcsicmp(lpszItemCode, ITEM_FULL_REJUV) == 0;
}