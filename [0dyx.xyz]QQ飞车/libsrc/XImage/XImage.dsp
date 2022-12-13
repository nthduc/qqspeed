# Microsoft Developer Studio Project File - Name="XImage" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=XImage - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "XImage.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "XImage.mak" CFG="XImage - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "XImage - Win32 Release" (based on "Win32 (x86) Static Library")
!MESSAGE "XImage - Win32 Debug" (based on "Win32 (x86) Static Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName "XImage"
# PROP Scc_LocalPath "."
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "XImage - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "\___IntermediateFiles___\MMOG\XImage_Release_Win32"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /YX /FD /c
# ADD CPP /nologo /MD /W3 /GX /O1 /I "..\..\Dx8Include" /I "..\..\include" /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /FR /YX /FD /c
# ADD BASE RSC /l 0x804 /d "NDEBUG"
# ADD RSC /l 0x804 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo
# Begin Special Build Tool
SOURCE="$(InputPath)"
PostBuild_Cmds=call PostBuild.bat
# End Special Build Tool

!ELSEIF  "$(CFG)" == "XImage - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "\___IntermediateFiles___\MMOG\XImage_Debug_Win32"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /YX /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /I "..\..\Dx8Include" /I "..\..\include" /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /FR /YX /FD /GZ /c
# ADD BASE RSC /l 0x804 /d "_DEBUG"
# ADD RSC /l 0x804 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:"..\..\lib\XImageD.lib"
# Begin Special Build Tool
SOURCE="$(InputPath)"
PostBuild_Cmds=call PostBuild.bat
# End Special Build Tool

!ENDIF 

# Begin Target

# Name "XImage - Win32 Release"
# Name "XImage - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\gif89a.cpp
# End Source File
# Begin Source File

SOURCE=.\xBmp.cpp
# End Source File
# Begin Source File

SOURCE=.\xFlic.cpp
# End Source File
# Begin Source File

SOURCE=.\xGif.cpp
# End Source File
# Begin Source File

SOURCE=.\xPcx.cpp
# End Source File
# Begin Source File

SOURCE=.\xPsd.cpp
# End Source File
# Begin Source File

SOURCE=.\xTga.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\Engine.h
# End Source File
# Begin Source File

SOURCE=.\gif89a.h
# End Source File
# Begin Source File

SOURCE=.\xBmp.h
# End Source File
# Begin Source File

SOURCE=.\xFlic.h
# End Source File
# Begin Source File

SOURCE=.\xGif.h
# End Source File
# Begin Source File

SOURCE=.\xImage.h
# End Source File
# Begin Source File

SOURCE=.\xPcx.h
# End Source File
# Begin Source File

SOURCE=.\xPsd.h
# End Source File
# Begin Source File

SOURCE=.\xTga.h
# End Source File
# End Group
# Begin Source File

SOURCE=.\Memo.txt
# End Source File
# Begin Source File

SOURCE=.\PostBuild.bat
# End Source File
# Begin Source File

SOURCE=.\Readme.txt
# End Source File
# End Target
# End Project
