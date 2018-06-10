#ifndef __ALGORITHM_H__
#define __ALGORITHM_H__

#include <windows.h>
#include <string.h>
#include <math.h>
#include <stdlib.h>

// find single byte
int FindSingleByte(const BYTE* aPacket, int aLen, BYTE b, int nStartIndex = 0);

// find specified bytes
int FindBytes(const BYTE* aPacket, int aLen, const BYTE* b, int bLen, int nStartIndex);

int FindSingleWord(const WORD* aPacket, int aLen, WORD b, int nStartIndex = 0);

// randomize a number by range nRange
int Randomize(int nNum, WORD wRandomizer, bool bVerifyWORD = false);

DWORD Swing(DWORD num, int nFlag, int nMax = 65535);

void Truncate(WORD& x, WORD low, WORD high);

#endif
