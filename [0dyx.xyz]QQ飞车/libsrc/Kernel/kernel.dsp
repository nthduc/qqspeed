# Microsoft Developer Studio Project File - Name="kernel" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=kernel - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "kernel.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "kernel.mak" CFG="kernel - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "kernel - Win32 Release" (based on "Win32 (x86) Static Library")
!MESSAGE "kernel - Win32 Debug" (based on "Win32 (x86) Static Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName "kernel"
# PROP Scc_LocalPath "."
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "kernel - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "../../__Release/libSrc/kernel"
# PROP Intermediate_Dir "../../__Release/libSrc/kernel"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /YX /FD /c
# ADD CPP /nologo /MD /W3 /GX /Zd /I "../../include" /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /D "GAMEVER_PUBLIC" /YX /FD /MAPINFO:EXPORTS /MAPINFO:LINES /c
# ADD BASE RSC /l 0x804 /d "NDEBUG"
# ADD RSC /l 0x804 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:"..\..\lib\kernel.lib"

!ELSEIF  "$(CFG)" == "kernel - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "../../__Debug/libSrc/kernel"
# PROP Intermediate_Dir "../../__Debug/libSrc/kernel"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /YX /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /I "../../include" /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /D "GAMEVER_PUBLIC" /FD /GZ /c
# SUBTRACT CPP /YX
# ADD BASE RSC /l 0x804 /d "_DEBUG"
# ADD RSC /l 0x804 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:"..\..\lib\kernel_d.lib"

!ENDIF 

# Begin Target

# Name "kernel - Win32 Release"
# Name "kernel - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\charstr.cpp
# End Source File
# Begin Source File

SOURCE=.\k_debug.cpp
# End Source File
# Begin Source File

SOURCE=.\kernel.cpp
# End Source File
# Begin Source File

SOURCE=.\package.cpp
# End Source File
# Begin Source File

SOURCE=.\vfile.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=..\..\Include\charstr.h
# End Source File
# Begin Source File

SOURCE=..\..\Include\k_debug.h
# End Source File
# Begin Source File

SOURCE=..\..\Include\kernel.h
# End Source File
# Begin Source File

SOURCE=.\package.h
# End Source File
# Begin Source File

SOURCE=..\..\Include\script.h
# End Source File
# Begin Source File

SOURCE=..\..\Include\variant.h
# End Source File
# Begin Source File

SOURCE=..\..\include\vfile.h
# End Source File
# End Group
# Begin Source File

SOURCE=.\PostBuild.bat
# End Source File
# End Target
# End Project
