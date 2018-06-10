// Ticker.h: interface for the CTicker class.
//
//////////////////////////////////////////////////////////////////////

#ifndef __TICKER_H__
#define __TICKER_H__

#include <windows.h>

class CTicker  
{
public:
	DWORD GetRemainer() const;
	BOOL IsTickEnded() const;
	void TickIt(DWORD dwCount);
	void IncreaseTicker(DWORD dwIncrease);
	void ResetTicker(DWORD dwNewCount);
	CTicker();
	virtual ~CTicker();

private:
	DWORD m_dwCount;
};

#endif
