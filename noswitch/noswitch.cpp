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

char ConfigPath[_MAX_PATH] = ""; // Dll path
FUNCTIONENTRYPOINTS	*server = NULL;
THISGAMESTRUCT *thisgame = NULL;

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
		"debug",
		OnGameCommandDebug,
		"debug ÿc0for debug purpose only."
	},

	// Add your own commands here
	{NULL}	// No more commands
};

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
	if (aPacket[0] == 0x60 && aLen == 1)
	{
		server->GamePrintInfo("No God damn switch!");
		return 0;
	}
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

