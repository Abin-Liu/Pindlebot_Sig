//////////////////////////////////////////////////////////////////////
// ClientImplementation.cpp
// -------------------------------------------------------------------
// This is the file to change when writing new modules. No other code
// should be needed!
//
// The modules are event based, and the functions they export will be
// called by the D2HackIt! dll at appropriate times. Any events you do
// not need can safely be deleted from this file, as D2HackIt! only
// calls the events it can bind.
//
// 
//
// <thohell@home.se>
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// Global stuff.
// -------------------------------------------------------------------
// This is a very convenient place to your function declarations and
// global variables.
//////////////////////////////////////////////////////////////////////
#include <windows.h>
#include "External/D2Hackit.h"
#include "External/D2Client.h"
#include "d2skill.h"
#include "ticker.h"
#include "packethelper.h"
#include "ArrayEx.h"

char ConfigPath[_MAX_PATH] = ""; // Dll path
FUNCTIONENTRYPOINTS	*server = NULL;
THISGAMESTRUCT *thisgame = NULL;

enum { STATE_NONE = 0, // Not available
	   STATE_SWAPIN, // swap weapons
	   STATE_WAITSWAPIN, // wait for weapon swapping packet
	   STATE_BC, // cast Battle command
	   STATE_BO, // cast Battle orders
	   STATE_SHOUT, // cast Shout
	   STATE_SWAPOUT, // swap weapons back
	   STATE_WAITSWAPOUT, // wait for weapon swapping packet
	   STATE_FINISH }; // restore original skill and finish

int g_nCurState = STATE_NONE;
int g_bSwap = FALSE;
CTicker g_ticker;
DWORD DELAY = 500;
WORD g_wOrigSkill = D2S_ATTACK;
CArrayEx<WORD, WORD> g_aSkills;

//////////////////////////////////////////////////////////////////////
// CLIENTINFO
//////////////////////////////////////////////////////////////////////
DWORD   ModuleVersion=MAKELONG(0, 1);
char    ModuleAuthor[]="Abin";
char    ModuleWebsite[]="http://www.wxjindu.com/abin/";
char    ModuleDescription[]="BO: Automatically swaps weapons and casts Battle command, Battle orders and Shout";
char    ModuleEmail[]="abinn32@yahoo.com";

//////////////////////////////////////////////////////////////////////
// MODULECOMMANDSTRUCT ModuleCommands[]
// -------------------------------------------------------------------
// To add your own commands, just add them to this list like:
//
//	{
//		"name"			// Name of the command
//		pFunction		// The function that handles command
//		"usage"			// Help text for the command
//	}
// 
// ...where pFunction is the name of the function that handles the
// command. The function must be declared as...
//
// BOOL PRIVATE OnGameCommandName(char** argv, int argc)
//
// ...and return TRUE on success or FALSE to show usage of the command.
//
// The "usage" line can contain "\n" as linebreaks and should be in 
// the following format for consistency:
//
// "commandname <required parameter> [optional parameter]ÿc0 Description"
//
//
// To use your commands in the game, just type 
// .modulename <command> [arguments]
//////////////////////////////////////////////////////////////////////

BOOL PRIVATE OnGameCommandBo(char** argv, int argc);
BOOL PRIVATE OnGameCommandSwapBo(char** argv, int argc);
BOOL PRIVATE OnGameCommandDelay(char** argv, int argc);
BOOL PRIVATE OnGameCommandDebug(char** argv, int argc);

MODULECOMMANDSTRUCT ModuleCommands[]=
{
	{	// The 'help' command is implemented in ClientCore.cpp and 
		// should not need to be changed.
		"help",
		OnGameCommandHelp,
		"helpÿc0 List commands available in this module.\n"
		"<command> helpÿc0 Shows detailed help for <command> in this module."
	},

	{
		"bo",
		OnGameCommandBo,
		"bo ÿc0cast bc, bo and shout without swaping weapons."
	},

	{
		"swapbo",
		OnGameCommandSwapBo,
		"bo ÿc0swap weapons and cast bc, bo and shout."
	},

	{
		"delay",
		OnGameCommandDelay,
		"bo ÿc0set delay for casting bc, bo and shout, in milliseconds."
	},

	{
		"debug",
		OnGameCommandDebug,
		"debug ÿc0for debug purpose only."
	},

	// Add your own commands here
	{NULL}	// No more commands
};

BOOL SwapWeapon()
{
	BYTE aPacket[1] = { 0x60 };
	return server->GameSendPacketToServer(aPacket, 1);
}

void GetSkillList(const BYTE* aPacket, DWORD aLen)
{
	g_aSkills.RemoveAll();

	const BYTE COUNT = aPacket[1];// number of skills
	WORD wSkillID = 0x0000;

	for (int i = 0, j = 6; i < COUNT; i++, j += 3)
	{
		::memcpy(&wSkillID, aPacket + j, 2);
		g_aSkills.Add(wSkillID);
	}
}

BOOL IsSkillValid(WORD wSkillID)
{
	return (g_aSkills.Find(wSkillID) != -1);
}

BOOL PRIVATE OnGameCommandBo(char** argv, int argc)
{
	if (thisgame == NULL || thisgame->player == NULL)
	{
		server->GamePrintError("\"thisgame\" srtucture not ready.");
		return TRUE;
	}

	if (g_aSkills.IsEmpty())
	{
		server->GamePrintError("Module \"ÿc7BO\" ÿc0must be loaded before joining game. Please rejoin a game.");
		return TRUE;
	}

	if (thisgame->player->CharacterClass != 4)
	{
		server->GamePrintError("You are not a ÿc2Barbarianÿc0.");
		return TRUE;
	}

	if (g_nCurState != STATE_NONE)
	{
		server->GamePrintError("Previous functions not ended, please wait...");
		return TRUE;
	}

	g_bSwap = FALSE;
	g_nCurState = STATE_SWAPIN;
	return TRUE;
}

BOOL PRIVATE OnGameCommandSwapBo(char** argv, int argc)
{
	if (thisgame == NULL || thisgame->player == NULL)
	{
		server->GamePrintError("\"thisgame\" srtucture not ready.");
		return TRUE;
	}

	if (g_aSkills.IsEmpty())
	{
		server->GamePrintError("Module \"ÿc7BO\" ÿc0must be loaded before joining game. Please rejoin a game.");
		return TRUE;
	}

	if (thisgame->player->CharacterClass != 4)
	{
		server->GamePrintError("You are not a ÿc2Barbarianÿc0.");
		return TRUE;
	}

	if (g_nCurState != STATE_NONE)
	{
		server->GamePrintError("Previous functions not ended, please wait...");
		return TRUE;
	}

	g_bSwap = TRUE;
	g_nCurState = STATE_SWAPIN;
	return TRUE;
}

BOOL PRIVATE OnGameCommandDelay(char** argv, int argc)
{
	if (argc < 3)
	{
		server->GamePrintError("Syntax: .bo delay <milliseconds>");
		return TRUE;
	}

	int ndelay = abs(atoi(argv[2]));
	DELAY = min(ndelay, 5000);

	char sz[_MAX_PATH] = "";
	char szDelay[64] = "";
	sprintf(szDelay, "%d", DELAY);
	sprintf(sz, "%s\\bo.ini", ConfigPath);
	::WritePrivateProfileString("Settings", "Delay", szDelay, sz);

	sprintf(szDelay, "ÿc7Module \"BO\"ÿc0: Casting delay set to ÿc2%dÿc0.", DELAY);
	server->GamePrintInfo(szDelay);
	return TRUE;
}

BOOL PRIVATE OnGameCommandDebug(char** argv, int argc)
{
	return TRUE;
}

//////////////////////////////////////////////////////////////////////
// OnClientStart
// -------------------------------------------------------------------
// Runs *once* when the module loads. 
// This function can be removed.
//
// Put any initialization code here.
//
// Return FALSE to prevent module from loading.
//////////////////////////////////////////////////////////////////////
BOOL EXPORT OnClientStart()
{
	char sz[_MAX_PATH] = "";
	sprintf(sz, "%s\\bo.ini", ConfigPath);
	DELAY = ::GetPrivateProfileInt("Settings", "Delay", 500, sz);
	DELAY = min(DELAY, 2000);
	return TRUE;
}

//////////////////////////////////////////////////////////////////////
// OnClientStop
// -------------------------------------------------------------------
// Runs *once* before client is unloaded. 
// This function can be removed.
// 
// Put any cleanup code here.
//
// Return value should be TRUE.
//////////////////////////////////////////////////////////////////////
BOOL EXPORT OnClientStop()
{	
	return TRUE;
}

//////////////////////////////////////////////////////////////////////
// OnGameJoin
// -------------------------------------------------------------------
// Executes when joining a game, and if loading the module while 
// inside a game.
//
// Refer to the documentation for the format of THISGAMESTRUCT.
//////////////////////////////////////////////////////////////////////
VOID EXPORT OnGameJoin(THISGAMESTRUCT* game)
{	
	g_bSwap = FALSE;
	g_wOrigSkill = D2S_ATTACK;
	g_nCurState = STATE_NONE;
	g_ticker.ResetTicker(0);
	thisgame = game;
	return;
}

//////////////////////////////////////////////////////////////////////
// OnGameLeave
// -------------------------------------------------------------------
// Executes when leaving a game.
//
// Refer to the documentation for the format of THISGAMESTRUCT.
//////////////////////////////////////////////////////////////////////
VOID EXPORT OnGameLeave(THISGAMESTRUCT* thisgame)
{
	g_aSkills.RemoveAll();
	return;
}

//////////////////////////////////////////////////////////////////////
// OnGamePacketBeforeSent
// -------------------------------------------------------------------
// Executes before packets are sent from the game to the server.
// 
// If you rewrite the packet, you must make sure to return the length
// of the new packet!
//
// NOTE: This event also works in single player games.
//////////////////////////////////////////////////////////////////////
DWORD EXPORT OnGamePacketBeforeSent(BYTE* aPacket, DWORD aLen)
{
	
	return aLen;
}

//////////////////////////////////////////////////////////////////////
// OnGamePacketBeforeReceived
// -------------------------------------------------------------------
// Executes before packets are received to the game from the server.
// 
// If you rewrite the packet, you must make sure to return the length
// of the new packet!
//
// NOTE: This event also works in single player games.
//////////////////////////////////////////////////////////////////////
DWORD EXPORT OnGamePacketBeforeReceived(BYTE* aPacket, DWORD aLen)
{
	// Skill list
	if (aPacket[0] == 0x94)
	{
		GetSkillList(aPacket, aLen);
		return aLen;
	}

	if (aPacket[0] == 0x23 && aLen == 13 && aPacket[6] == 0x00 && g_nCurState == STATE_NONE)
	{
		// Skill in use on right side
		::memcpy(&g_wOrigSkill, aPacket + 7, 2); // record it
		return aLen;
	}

	if (aPacket[0] == 0x97 && aLen == 1)
	{
		// weapon switched
		if (g_nCurState == STATE_WAITSWAPIN)
		{
			g_ticker.ResetTicker(0);
			g_nCurState = STATE_BC;
		}
		else if (g_nCurState == STATE_WAITSWAPOUT)
		{
			g_ticker.ResetTicker(0);
			g_nCurState = STATE_FINISH;
		}
		else
		{
		}

		return aLen;
	}
	
	return aLen;
}


//////////////////////////////////////////////////////////////////////
// OnGameTimerTick
// -------------------------------------------------------------------
// This gets executed apx. every 1/10th of a second when in a game.
//
// You can use this to create custom timers.
//////////////////////////////////////////////////////////////////////
DWORD EXPORT OnGameTimerTick(void)
{
	g_ticker.TickIt(100);

	if (!g_ticker.IsTickEnded())
		return 0;	

	switch (g_nCurState)
	{
	case STATE_SWAPIN:
		
		if (g_bSwap)
		{
			//server->GamePrintInfo("swap weapons");
			SwapWeapon();
			g_ticker.ResetTicker(3000);
			g_nCurState = STATE_WAITSWAPIN;
		}
		else
		{
			g_nCurState = STATE_BC;
		}
		
		break;

	case STATE_WAITSWAPIN:
		// something is wrong if we reach here
		server->GamePrintError("Failed to swap weapons, lag bad?");
		g_nCurState = STATE_NONE;
		break;

	case STATE_BC:
		//server->GamePrintInfo("cast bc");
		if (IsSkillValid(D2S_BATTLECOMMAND))
		{
			g_ticker.ResetTicker(DELAY);
			CastOnMap(thisgame->player->PlayerID, thisgame->player->PlayerPositionX, thisgame->player->PlayerPositionY, D2S_BATTLECOMMAND, TRUE);
		}

		g_nCurState = STATE_BO;
		break;

	case STATE_BO:
		//server->GamePrintInfo("cast bo");
		if (IsSkillValid(D2S_BATTLEORDERS))
		{
			g_ticker.ResetTicker(DELAY);
			CastOnMap(thisgame->player->PlayerID, thisgame->player->PlayerPositionX, thisgame->player->PlayerPositionY, D2S_BATTLEORDERS, TRUE);
		}
		g_nCurState = STATE_SHOUT;
		break;

	case STATE_SHOUT:
		//server->GamePrintInfo("cast shout");
		if (IsSkillValid(D2S_SHOUT))
		{
			g_ticker.ResetTicker(max(DELAY * 2, 1000));
			CastOnMap(thisgame->player->PlayerID, thisgame->player->PlayerPositionX, thisgame->player->PlayerPositionY, D2S_SHOUT, TRUE);
		}
		g_nCurState = STATE_SWAPOUT;
		break;

	case STATE_SWAPOUT:

		if (g_bSwap)
		{
			//server->GamePrintInfo("swap weapons back");
			SwapWeapon();
			g_ticker.ResetTicker(3000);
			g_nCurState = STATE_WAITSWAPOUT;
		}
		else
		{
			g_nCurState = STATE_NONE;
		}
		
		break;

	case STATE_WAITSWAPOUT:
		// something is wrong if we reach here
		server->GamePrintError("Failed to swap weapons, lag bad?");
		g_nCurState = STATE_FINISH;
		break;

	case STATE_FINISH:
		if (!g_bSwap)
			SelectSkill(g_wOrigSkill, TRUE); // restore original skill
		g_nCurState = STATE_NONE;
		break;

	default:
		break;
	}
	return 0;
}

//////////////////////////////////////////////////////////////////////
// stuff below here is from ClientCore.cpp
//////////////////////////////////////////////////////////////////////
// Dll entry/exit
//////////////////////////////////////////////////////////////////////
BOOL APIENTRY DllMain(HANDLE hModule, DWORD  ul_reason_for_call, LPVOID lpReserved)
{
	BOOL hResult = TRUE;
	char *t;
    switch (ul_reason_for_call)
	{
		case DLL_PROCESS_ATTACH:

			// Create server struct
			server = new FUNCTIONENTRYPOINTS;

			// Bind exported functions from server
			HMODULE hModule;
			hModule = (HMODULE)GetModuleHandle("D2HackIt");

			// Macros make this look a lot nicer :)
			BIND_TO_SERVER(GamePrintInfo);
			BIND_TO_SERVER(GamePrintError);
			BIND_TO_SERVER(GamePrintVerbose);
			BIND_TO_SERVER(GamePrintString);
			BIND_TO_SERVER(GameCommandLine);
			BIND_TO_SERVER(GameSendPacketToServer);
			BIND_TO_SERVER(GameSendPacketToGame);
			BIND_TO_SERVER(GetFingerprint);
			BIND_TO_SERVER(Intercept);
			BIND_TO_SERVER(GetHackProfileString);
			BIND_TO_SERVER(SetHackProfileString);
			BIND_TO_SERVER(GetThisgameStruct);

			// Get plugin path
			t = ConfigPath;
			if (GetModuleFileName((HINSTANCE)hModule, t, _MAX_PATH)) {
				int p=strlen(ConfigPath);
				while (p) {
						if (ConfigPath[p] == '\\')
							{ ConfigPath[p] = 0; p=0;}
						else
					p--;
				}
			}
			// initiate client
			// hResult = OnClientStart();
			break;

		case DLL_PROCESS_DETACH:
			// kill client

			// hResult = OnClientStop();

			delete server;
			break;
    }
    return hResult;
}

//////////////////////////////////////////////////////////////////////
// Stubfunctions for 'property get' functions.
//////////////////////////////////////////////////////////////////////
LPCSTR	EXPORT GetModuleAuthor()		{return ModuleAuthor;}
LPCSTR	EXPORT GetModuleWebsite()		{return ModuleWebsite;}
DWORD	EXPORT GetModuleVersion()		{return ModuleVersion;}
LPCSTR	EXPORT GetModuleEmail()			{return ModuleEmail;}
LPCSTR	EXPORT GetModuleDescription()	{return ModuleDescription;}

//////////////////////////////////////////////////////////////////////
// OnClientCommandLine
// -------------------------------------------------------------------
// The modules own extension of the command line interface. Any custom
// commands you add are parsed here.
//
// Return value should be TRUE, but it is not used at this
// time.
//
// Arguments when we get here:
// argv[0]			Name of module
// argv[1]			Name of command (If supplied)
// argv[2 ... n]	The rest
//
// Syntax in the game: .<module> <arguments>
//////////////////////////////////////////////////////////////////////
BOOL EXPORT OnGameCommandLine(char* argv[], int argc)
{
	// Check if user supplied anything at all, if not assume help...
	if (argc==1)
		argv[argc++]="help";


	MODULECOMMANDSTRUCT* mcs=ModuleCommands;

	while (mcs->szName) {
		if (!stricmp(mcs->szName, argv[1]))
			break;
		mcs++;
	}

	char *p,*t,*msg,*fMsg;
	fMsg=new char[256];
	//
	// Is this a built-in function ?
	if (mcs->szName) {
		//
		// If functions returns false, show usage help
		if (!mcs->pFunc(argv, argc)) {
			t=new char[strlen(mcs->szUsage)+1];
			server->GamePrintInfo("Usage:");
			sprintf((char*)t, "%s", mcs->szUsage);
			if (strlen((char*)t))
			{
				msg=p=t;
				while (*p != 0) {
					if (*p == '\n')
					{
						*(p++) = 0;
						sprintf(fMsg, "?c4.%s %s", argv[0], msg);
						server->GamePrintInfo((char*)fMsg);
					if (*p != 0)
						msg = p;
					} else
						p++;
					}
				sprintf(fMsg, "?c4.%s %s", argv[0], msg);
				server->GamePrintInfo((char*)fMsg);
			}
			delete t;
		}
	}
	else {
	// Unknown command, show catch-all help phraze.
	t=new char[128];
	sprintf(t, "Unknown command ?c4'%s %s'?c0 - try ?c4'.%s help'?c0 to get help.",
		argv[0], argv[1], argv[0]);
	server->GamePrintError(t);
	delete t;
	}
	delete fMsg;
	return TRUE;
}

//////////////////////////////////////////////////////////////////////
// OnGameCommandHelp
// -------------------------------------------------------------------
// Our default help function.
//
// Syntax in the game: .<module> <arguments>
//////////////////////////////////////////////////////////////////////
BOOL PRIVATE OnGameCommandHelp(char** argv, int argc)
{
	// If command line is longer than 2, show usage for 'help'
	if (argc>2) return FALSE;

	char t[1024];
	sprintf(t, "Available commands for %s:", argv[0]);
	server->GamePrintInfo(t);

	// Loop through ModuleCommands[] and print their names
	for (int i=0; ModuleCommands[i].szName != NULL; i++)
	{
		sprintf(t, "?c4.%s %s", argv[0], ModuleCommands[i].szName);
		server->GamePrintInfo(t);
	}

	sprintf(t, "For help on a specific command, type ?c4.%s <command> help", argv[0]);
	server->GamePrintInfo(t);
	return TRUE;
}

