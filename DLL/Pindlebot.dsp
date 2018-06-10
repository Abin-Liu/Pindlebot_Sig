# Microsoft Developer Studio Project File - Name="Pindlebot" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=Pindlebot - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "Pindlebot.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "Pindlebot.mak" CFG="Pindlebot - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "Pindlebot - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "Pindlebot - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "Pindlebot - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "AUTO_EXPORTS" /YX /FD /c
# ADD CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "AUTO_EXPORTS" /YX /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /machine:I386 /out:"Release\Pindlebot.d2h"
# SUBTRACT LINK32 /incremental:yes

!ELSEIF  "$(CFG)" == "Pindlebot - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "AUTO_EXPORTS" /YX /FD /GZ /c
# ADD CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "AUTO_EXPORTS" /YX /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /debug /machine:I386 /pdbtype:sept
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /incremental:no /machine:I386 /out:"..\Release\Pindlebot.d2h" /pdbtype:sept
# SUBTRACT LINK32 /debug

!ENDIF 

# Begin Target

# Name "Pindlebot - Win32 Release"
# Name "Pindlebot - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\Algorithm.cpp
# End Source File
# Begin Source File

SOURCE=.\Belt.cpp
# End Source File
# Begin Source File

SOURCE=.\D2Lib\D2Abstract.cpp
# End Source File
# Begin Source File

SOURCE=.\D2Action.cpp
# End Source File
# Begin Source File

SOURCE=.\D2Character.cpp
# End Source File
# Begin Source File

SOURCE=.\D2Lib\D2Merc.cpp
# End Source File
# Begin Source File

SOURCE=.\D2Lib\D2Monster.cpp
# End Source File
# Begin Source File

SOURCE=.\D2Lib\D2Object.cpp
# End Source File
# Begin Source File

SOURCE=.\D2Lib\D2Player.cpp
# End Source File
# Begin Source File

SOURCE=.\D2Scheme.cpp
# End Source File
# Begin Source File

SOURCE=.\D2Skill.cpp
# End Source File
# Begin Source File

SOURCE=.\D2Lib\D2StaticObject.cpp
# End Source File
# Begin Source File

SOURCE=.\D2Lib\D2Unit.cpp
# End Source File
# Begin Source File

SOURCE=.\GameInfo.cpp
# End Source File
# Begin Source File

SOURCE=.\GoldManager.cpp
# End Source File
# Begin Source File

SOURCE=.\Ini.cpp
# End Source File
# Begin Source File

SOURCE=.\item.cpp
# End Source File
# Begin Source File

SOURCE=.\KeyBind.cpp
# End Source File
# Begin Source File

SOURCE=.\packethelper.cpp
# End Source File
# Begin Source File

SOURCE=.\PickitHelper.cpp
# End Source File
# Begin Source File

SOURCE=.\Pindlebot.cpp
# End Source File
# Begin Source File

SOURCE=.\Pindleskin.cpp
# End Source File
# Begin Source File

SOURCE=.\Ticker.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\Algorithm.h
# End Source File
# Begin Source File

SOURCE=.\Belt.h
# End Source File
# Begin Source File

SOURCE=.\D2Lib\D2Abstract.h
# End Source File
# Begin Source File

SOURCE=.\D2Action.h
# End Source File
# Begin Source File

SOURCE=.\D2Character.h
# End Source File
# Begin Source File

SOURCE=.\D2Lib\D2Merc.h
# End Source File
# Begin Source File

SOURCE=.\D2Lib\D2MessageDef.h
# End Source File
# Begin Source File

SOURCE=.\D2Lib\D2Monster.h
# End Source File
# Begin Source File

SOURCE=.\D2Lib\D2Object.h
# End Source File
# Begin Source File

SOURCE=.\D2Lib\D2ObjFingerPrinter.h
# End Source File
# Begin Source File

SOURCE=.\D2Lib\D2Player.h
# End Source File
# Begin Source File

SOURCE=.\D2Scheme.h
# End Source File
# Begin Source File

SOURCE=.\D2Skill.h
# End Source File
# Begin Source File

SOURCE=.\D2Lib\D2StaticObject.h
# End Source File
# Begin Source File

SOURCE=.\D2Lib\D2Unit.h
# End Source File
# Begin Source File

SOURCE=.\GameInfo.h
# End Source File
# Begin Source File

SOURCE=.\Globals.h
# End Source File
# Begin Source File

SOURCE=.\GoldManager.h
# End Source File
# Begin Source File

SOURCE=.\Ini.h
# End Source File
# Begin Source File

SOURCE=.\item.h
# End Source File
# Begin Source File

SOURCE=.\KeyBind.h
# End Source File
# Begin Source File

SOURCE=.\MemDef.h
# End Source File
# Begin Source File

SOURCE=.\packethelper.h
# End Source File
# Begin Source File

SOURCE=.\PathInfo.h
# End Source File
# Begin Source File

SOURCE=.\PickitHelper.h
# End Source File
# Begin Source File

SOURCE=.\Pindlebot.h
# End Source File
# Begin Source File

SOURCE=.\Pindleskin.h
# End Source File
# Begin Source File

SOURCE=.\SkillDef.h
# End Source File
# Begin Source File

SOURCE=.\Ticker.h
# End Source File
# Begin Source File

SOURCE=.\waypoint.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# End Group
# End Target
# End Project
