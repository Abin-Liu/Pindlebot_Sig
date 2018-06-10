#ifndef __PINDLEBOT_H__
#define __PINDLEBOT_H__

#include "External/D2Hackit.h"
#include "External/D2Client.h"
#include <string.h>
#include "GameInfo.h"
#include "PathInfo.h"
#include "Ini.h"
#include "PacketHelper.h"
#include "Ticker.h"

BOOL PRIVATE OnGameCommandPause(char** argv, int argc);
BOOL PRIVATE OnGameCommandStatistics(char** argv, int argc);
BOOL PRIVATE OnGameCommandStart(char** argv, int argc);
BOOL PRIVATE OnGameCommandStop(char** argv, int argc);
BOOL PRIVATE OnGameCommandDebug(char** argv, int argc);

void FinishRun(int nReason, BOOL bRunAway); // finish current run immediately
void GetSkillList(const BYTE* aPacket, DWORD aLen);
BOOL IsSkillValid(WORD wSkillID);
BOOL PerformAction(const CD2Action& action);

// SIG functions
void SIGLeave();
void SIGPause();
void SIGResume();
void SIGStop();
void SIGQuitD2();
void SIGPostString(LPCTSTR lpsz);

#endif