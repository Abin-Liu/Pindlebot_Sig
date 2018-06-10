#include "Algorithm.h"
#include <stdio.h>

int FindBytes(const BYTE* aPacket, int aLen, const BYTE* b, int bLen, int nStartIndex)
{
	bool bFind = false;
	int nLastPos = aLen - bLen - nStartIndex;

	for (int i = nStartIndex; i <= nLastPos; i++)
	{
		for (int j = 0, bFind = true; j < bLen; j++)
		{
			if (b[j] != aPacket[i + j])
			{
				// not match
				bFind = false;
				break;
			}
		}

		if (bFind)
			return i;
	}

	return -1;
}

int FindSingleByte(const BYTE* aPacket, int aLen, BYTE b, int nStartIndex)
{
	for (int i = nStartIndex; i < aLen; i++)
	{
		if (aPacket[i] == b)
		{
			return i;
		}
	}
	return -1;
}

int FindSingleWord(const WORD* aPacket, int aLen, WORD b, int nStartIndex)
{
	for (int i = nStartIndex; i < aLen; i++)
	{
		if (aPacket[i] == b)
		{
			return i;
		}
	}
	return -1;
}

int Randomize(int nNum, WORD wRandomizer, bool bVerifyWORD)
{
	int nRes = nNum;

	if (wRandomizer == 0)
		return nRes;

	WORD wRand = (rand() % wRandomizer) + 1;

	if ((rand() % 2) == 0)
		nRes += wRand;
	else
		nRes -= wRand;


	if (bVerifyWORD)
	{
		nRes = max(0, nRes);
		nRes = min(65535, nRes);
	}

	return nRes;
}

DWORD Swing(DWORD num, int nFlag, int nMax)
{
	int nMod = (nFlag % 2) ? -nFlag : nFlag;
	int ret = num + nMod;
	if (ret < 0)
		ret = 0;
	if (ret > nMax)
		ret = nMax;

	return ret;	
}

void Truncate(WORD& x, WORD low, WORD high)
{
	x = max(x, low);
	x = min(x, high);
}