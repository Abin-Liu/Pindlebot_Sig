//////////////////////////////////////////////////////////////////////
// D2Abstract.h
//
// CD2Abstract, an abstract class which presents an event-driving interface
// of Diablo II game events, objects and accessors. This class will be the
// topmost base class of any Diablo II game manipulating classes.
// 
// The class encapsulates the following pure virtual methods which must
// be implemented in any derived classes:
//
// OnGameJoin
// OnGameLeave
// OnGamePacketBeforeReceived
// OnGamePacketBeforeSent
// OnGameTimerTick
//
// Written by Abin (abinn32@yahoo.com)
// http://www.wxjindu.com/abin/
//
// July 14th, 2004
//
//////////////////////////////////////////////////////////////////////

#ifndef __D2ABSTRACT_H__
#define __D2ABSTRACT_H__

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <tchar.h>
#include <string.h>
#include "Definition\\D2MessageDef.h"
#include "Definition\\WayPointDef.h"
#include "Template\\ArrayEx.h"
#include "Template\\LinkedList.h"

///////////////////////////////////////////////////////////////////////
// Callback Function Definition
///////////////////////////////////////////////////////////////////////
typedef void (CALLBACK *D2MESSAGEPROC)(LPCVOID lpObject, UINT nMessage, WPARAM wParam, LPARAM lParam, LPVOID lpData); // Notify a D2 message
typedef void (CALLBACK *DEBUGMESSAGEPROC)(LPCVOID lpObject, LPCTSTR lpszMessage, LPVOID lpData); // Send a debug string
typedef BOOL (CALLBACK *SENDPACKETTOSERVERPROC)(LPCVOID lpObject, BYTE* aPacket, DWORD aLen, LPVOID lpData); //Send a packet to server
typedef BOOL (CALLBACK *SENDPACKETTOGAMEPROC)(LPCVOID lpObject, BYTE* aPacket, DWORD aLen, LPVOID lpData); // Send a packet to game

///////////////////////////////////////////////////////////////////////
// CD2Abstract Class Definition
///////////////////////////////////////////////////////////////////////
class CD2Abstract  
{
public:	

	///////////////////////////////////////////////////////////////////////
	// Constructor(s) & destructor
	///////////////////////////////////////////////////////////////////////
	CD2Abstract();
	virtual ~CD2Abstract();

	///////////////////////////////////////////////////////////////////////
	// Client Entries, Must be Implemented in Derived Classes
	///////////////////////////////////////////////////////////////////////
	virtual void OnGameJoin() = 0; // To be called in "OnGameJoin"
	virtual void OnGameLeave() = 0; // To be called in "OnGameLeave"
	virtual void OnGamePacketBeforeReceived(const BYTE* aPacket, DWORD aLen) = 0; // To be called in "OnGamePacketBeforeReceived"
	virtual void OnGamePacketBeforeSent(const BYTE* aPacket, DWORD aLen) = 0; // To be called in "OnGamePacketBeforeSent"
	virtual void OnGameTimerTick() = 0; // To be called in "OnGameTimerTick"

	///////////////////////////////////////////////////////////////////////
	// Public Attributes
	///////////////////////////////////////////////////////////////////////
	BOOL IsDebugging() const; // Whether is in debug mode
	
	///////////////////////////////////////////////////////////////////////
	// Public Operations
	///////////////////////////////////////////////////////////////////////
	void InitAttributes(); // Reset all attributes
	void SetDebugMessageProc(DEBUGMESSAGEPROC lpfn, LPVOID lpData = NULL, LPCTSTR lpszLabel = NULL);
	void SetD2MessageProc(D2MESSAGEPROC lpfn, LPVOID lpData = NULL);
	void SetSendPacketToServerProc(SENDPACKETTOSERVERPROC lpfn, LPVOID lpData = NULL);
	void SetSendPacketToGameProc(SENDPACKETTOGAMEPROC lpfn, LPVOID lpData = NULL);
	static DWORD FormatPacket(BOOL bReceived, const BYTE* aPacket, DWORD aLen, LPTSTR lpszBuffer, DWORD dwMaxChars);

protected:
	
	///////////////////////////////////////////////////////////////////////
	// Protected Operations
	///////////////////////////////////////////////////////////////////////
	BOOL NotifyDebugMessage(LPCTSTR lpszDebug) const; // Send text
	BOOL NotifyD2Message(UINT nMessage, WPARAM wParam, LPARAM lParam) const; // Send a D2 message
	BOOL FormatAndNotifyDebugMessage(LPCTSTR lpszFormat, ...) const; // Format text and send
	BOOL SendPacketToServer(const BYTE* aPacket, DWORD aLen, int nTimes = 1) const; // Send a packet to server
	BOOL SendPacketToGame(const BYTE* aPacket, DWORD aLen, int nTimes = 1) const; // Send a packet to game

private:

	///////////////////////////////////////////////////////////////////////
	// Private Member Data
	///////////////////////////////////////////////////////////////////////
	DEBUGMESSAGEPROC m_lpfnDebugMessageProc; // The callback function for debug
	LPVOID m_lpDebugData; // 32-bit application defined data
	LPTSTR m_lpszDebugLabel; // A text label, will be displayed on the left of the debug messages
	SENDPACKETTOSERVERPROC m_lpfnSendPacketToServerProc; // The callback function for sending packets to server
	LPVOID m_lpSendPacketToServerData; // 32-bit application defined data
	SENDPACKETTOGAMEPROC m_lpfnSendPacketToGameProc; // The callback function for sending packets to game
	LPVOID m_lpSendPacketToGameData; // 32-bit application defined data
	D2MESSAGEPROC m_lpfnMessageProc; // The message handler
	LPVOID m_lpMessageData; // 32-bit application data, sent along with object messages
};

#endif // __D2ABSTRACT_H__
