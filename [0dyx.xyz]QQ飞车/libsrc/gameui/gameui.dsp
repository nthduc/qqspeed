# Microsoft Developer Studio Project File - Name="gameui" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=gameui - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "gameui.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "gameui.mak" CFG="gameui - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "gameui - Win32 Release" (based on "Win32 (x86) Static Library")
!MESSAGE "gameui - Win32 Debug" (based on "Win32 (x86) Static Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName "gameui"
# PROP Scc_LocalPath "."
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "gameui - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "\___IntermediateFiles___\MMOG\GameUI_Release_WIN32"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /YX /FD /c
# ADD CPP /nologo /MD /W3 /GX /Zi /O1 /I "..\..\Include" /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /YX /FD /c
# ADD BASE RSC /l 0x804 /d "NDEBUG"
# ADD RSC /l 0x804 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:"..\..\lib\gameui.lib"
# Begin Special Build Tool
SOURCE="$(InputPath)"
PostBuild_Cmds=call PostBuild.bat
# End Special Build Tool

!ELSEIF  "$(CFG)" == "gameui - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "\___IntermediateFiles___\MMOG\GameUI_Debug_WIN32"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /YX /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /I "..\..\Include" /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /D "_UIEDITMODE" /FR /YX /FD /GZ /c
# ADD BASE RSC /l 0x804 /d "_DEBUG"
# ADD RSC /l 0x804 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:"..\..\lib\gameui_d.lib"
# Begin Special Build Tool
SOURCE="$(InputPath)"
PostBuild_Cmds=call PostBuild.bat
# End Special Build Tool

!ENDIF 

# Begin Target

# Name "gameui - Win32 Release"
# Name "gameui - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\gameui.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Include\gameui.h
# End Source File
# Begin Source File

SOURCE=.\uicontrol.cpp
# End Source File
# Begin Source File

SOURCE=.\uicontrol.h
# End Source File
# Begin Source File

SOURCE=.\uiimage.cpp
# End Source File
# Begin Source File

SOURCE=.\uiimage.h
# End Source File
# Begin Source File

SOURCE=.\uiscriptfunc.cpp
# End Source File
# Begin Source File

SOURCE=.\widget.cpp
# End Source File
# Begin Source File

SOURCE=.\widget.h
# End Source File
# Begin Source File

SOURCE=.\widget.inl
# End Source File
# End Group
# Begin Source File

SOURCE=.\PostBuild.bat
# End Source File
# End Target
# End Project
