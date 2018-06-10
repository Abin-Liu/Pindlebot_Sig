# Microsoft Developer Studio Project File - Name="Pindlebot" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

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
!MESSAGE "Pindlebot - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "Pindlebot - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "Pindlebot - Win32 Release"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x409 /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /machine:I386
# ADD LINK32 /nologo /subsystem:windows /machine:I386

!ELSEIF  "$(CFG)" == "Pindlebot - Win32 Debug"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /Yu"stdafx.h" /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x409 /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept

!ENDIF 

# Begin Target

# Name "Pindlebot - Win32 Release"
# Name "Pindlebot - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\ActionDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\ActionListDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\CharClassDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\Clipboard.cpp
# End Source File
# Begin Source File

SOURCE=.\D2Action.cpp
# End Source File
# Begin Source File

SOURCE=.\D2Character.cpp
# End Source File
# Begin Source File

SOURCE=.\D2Monster.cpp
# End Source File
# Begin Source File

SOURCE=.\D2Scheme.cpp
# End Source File
# Begin Source File

SOURCE=.\D2Skill.cpp
# End Source File
# Begin Source File

SOURCE=.\EditEx.cpp
# End Source File
# Begin Source File

SOURCE=.\HSListBox.cpp
# End Source File
# Begin Source File

SOURCE=.\HyperLink.cpp
# End Source File
# Begin Source File

SOURCE=.\IniFile.cpp
# End Source File
# Begin Source File

SOURCE=.\ItemLogDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\LoginPage.cpp
# End Source File
# Begin Source File

SOURCE=.\MonsterAttrDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\MyListCtrl.cpp
# End Source File
# Begin Source File

SOURCE=.\MySheet.cpp
# End Source File
# Begin Source File

SOURCE=.\NewCharDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\PickitPage.cpp
# End Source File
# Begin Source File

SOURCE=.\Pindlebot.cpp
# End Source File
# Begin Source File

SOURCE=.\Pindlebot.rc
# End Source File
# Begin Source File

SOURCE=.\PindlebotDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\ProgDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\ReportCtrl.cpp
# End Source File
# Begin Source File

SOURCE=.\SafeSchemePage.cpp
# End Source File
# Begin Source File

SOURCE=.\SafetyDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\SafetyScheme.cpp
# End Source File
# Begin Source File

SOURCE=.\SafetySchemeNameDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\SelFileDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\StatisticsDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# Begin Source File

SOURCE=.\StrSplit.cpp
# End Source File
# Begin Source File

SOURCE=.\SystemTray.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\ActionDlg.h
# End Source File
# Begin Source File

SOURCE=.\ActionListDlg.h
# End Source File
# Begin Source File

SOURCE=.\ArrayEx.h
# End Source File
# Begin Source File

SOURCE=.\BonusDef.h
# End Source File
# Begin Source File

SOURCE=.\CharClassDlg.h
# End Source File
# Begin Source File

SOURCE=.\Clipboard.h
# End Source File
# Begin Source File

SOURCE=.\D2Action.h
# End Source File
# Begin Source File

SOURCE=.\D2Character.h
# End Source File
# Begin Source File

SOURCE=.\D2Monster.h
# End Source File
# Begin Source File

SOURCE=.\D2Scheme.h
# End Source File
# Begin Source File

SOURCE=.\D2Skill.h
# End Source File
# Begin Source File

SOURCE=.\EditEx.h
# End Source File
# Begin Source File

SOURCE=.\HSListBox.h
# End Source File
# Begin Source File

SOURCE=.\HyperLink.h
# End Source File
# Begin Source File

SOURCE=.\IniFile.h
# End Source File
# Begin Source File

SOURCE=.\ItemLogDlg.h
# End Source File
# Begin Source File

SOURCE=.\LoginPage.h
# End Source File
# Begin Source File

SOURCE=.\MonsterAttrDlg.h
# End Source File
# Begin Source File

SOURCE=.\MyListCtrl.h
# End Source File
# Begin Source File

SOURCE=.\MySheet.h
# End Source File
# Begin Source File

SOURCE=.\NewCharDlg.h
# End Source File
# Begin Source File

SOURCE=.\PickitPage.h
# End Source File
# Begin Source File

SOURCE=.\Pindlebot.h
# End Source File
# Begin Source File

SOURCE=.\PindlebotDlg.h
# End Source File
# Begin Source File

SOURCE=.\ProgDlg.h
# End Source File
# Begin Source File

SOURCE=.\ReportCtrl.h
# End Source File
# Begin Source File

SOURCE=.\Resource.h
# End Source File
# Begin Source File

SOURCE=.\SafeSchemePage.h
# End Source File
# Begin Source File

SOURCE=.\SafetyDlg.h
# End Source File
# Begin Source File

SOURCE=.\SafetyScheme.h
# End Source File
# Begin Source File

SOURCE=.\SafetySchemeNameDlg.h
# End Source File
# Begin Source File

SOURCE=.\SelFileDlg.h
# End Source File
# Begin Source File

SOURCE=.\Singleton.h
# End Source File
# Begin Source File

SOURCE=.\StatisticsDlg.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# Begin Source File

SOURCE=.\StrSplit.h
# End Source File
# Begin Source File

SOURCE=.\SystemTray.h
# End Source File
# Begin Source File

SOURCE=.\Ticker.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\res\BULLDO~1.ICO
# End Source File
# Begin Source File

SOURCE=.\res\dog.ico
# End Source File
# Begin Source File

SOURCE=.\res\ico00001.ico
# End Source File
# Begin Source File

SOURCE=.\res\ico00002.ico
# End Source File
# Begin Source File

SOURCE=.\icon2.ico
# End Source File
# Begin Source File

SOURCE=.\res\icon2.ico
# End Source File
# Begin Source File

SOURCE=.\res\icon3.ico
# End Source File
# Begin Source File

SOURCE=.\res\Login.ico
# End Source File
# Begin Source File

SOURCE=.\res\open.ico
# End Source File
# Begin Source File

SOURCE=.\res\Pindlebot.ico
# End Source File
# Begin Source File

SOURCE=.\res\Pindlebot.rc2
# End Source File
# Begin Source File

SOURCE=.\res\portal.ico
# End Source File
# Begin Source File

SOURCE=.\res\question.ico
# End Source File
# Begin Source File

SOURCE=.\res\Small_active.ico
# End Source File
# Begin Source File

SOURCE=.\res\small_inactive.ico
# End Source File
# Begin Source File

SOURCE=.\res\sorc1.ico
# End Source File
# Begin Source File

SOURCE=.\res\sorc_active.ico
# End Source File
# Begin Source File

SOURCE=.\res\sorc_inactive.ico
# End Source File
# Begin Source File

SOURCE=.\res\Sorceress.ico
# End Source File
# Begin Source File

SOURCE=.\res\TreasureOpen.ico
# End Source File
# Begin Source File

SOURCE=.\res\W95MBX03.ICO
# End Source File
# Begin Source File

SOURCE=.\res\W95MBX04.ICO
# End Source File
# End Group
# Begin Source File

SOURCE=.\ReadMe.txt
# End Source File
# End Target
# End Project
