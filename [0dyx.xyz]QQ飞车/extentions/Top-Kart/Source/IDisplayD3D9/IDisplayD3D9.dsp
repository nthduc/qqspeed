# Microsoft Developer Studio Project File - Name="IDisplayD3D9" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=IDisplayD3D9 - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "IDisplayD3D9.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "IDisplayD3D9.mak" CFG="IDisplayD3D9 - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "IDisplayD3D9 - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "IDisplayD3D9 - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName "IDisplayD3D9"
# PROP Scc_LocalPath "."
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "IDisplayD3D9 - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "..\\..\\..\\IntermediateFile\\Intermediate_Release_DisplayD3D9"
# PROP Intermediate_Dir "..\\..\\..\\IntermediateFile\\Intermediate_Release_DisplayD3D9"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "IDISPLAYD3D9_EXPORTS" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /W3 /GX /Zd /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "IDISPLAYD3D9_EXPORTS" /D "_USEDLL_" /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x804 /d "NDEBUG"
# ADD RSC /l 0x804 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /map /machine:I386 /out:"..\..\..\ShareMoudle\output\Dragon\IDisplayD3D9.dll" /MAPINFO:EXPORTS /MAPINFO:LINES
# SUBTRACT LINK32 /pdb:none

!ELSEIF  "$(CFG)" == "IDisplayD3D9 - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "..\\..\\..\\IntermediateFile\\Intermediate_Debug_DisplayD3D9"
# PROP Intermediate_Dir "..\\..\\..\\IntermediateFile\\Intermediate_Debug_DisplayD3D9"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "IDISPLAYD3D9_EXPORTS" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /ML /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "IDISPLAYD3D9_EXPORTS" /D "_USEDLL_" /Yu"stdafx.h" /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x804 /d "_DEBUG"
# ADD RSC /l 0x804 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /debug /machine:I386 /pdbtype:sept
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /debug /machine:I386 /out:"..\..\..\ShareMoudle\output\Dragon\IDisplayD3D9_D.dll" /pdbtype:sept

!ENDIF 

# Begin Target

# Name "IDisplayD3D9 - Win32 Release"
# Name "IDisplayD3D9 - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\CoreRender.cpp
# End Source File
# Begin Source File

SOURCE=.\d3d9warpper.cpp

!IF  "$(CFG)" == "IDisplayD3D9 - Win32 Release"

!ELSEIF  "$(CFG)" == "IDisplayD3D9 - Win32 Debug"

# ADD CPP /Yu

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Display.cpp
# SUBTRACT CPP /YX /Yc /Yu
# End Source File
# Begin Source File

SOURCE=.\IDisplayD3D9.cpp
# End Source File
# Begin Source File

SOURCE=.\IDisplayD3D91.cpp
# End Source File
# Begin Source File

SOURCE=.\RenderD3D9.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# Begin Source File

SOURCE=.\Texture9.cpp
# End Source File
# Begin Source File

SOURCE=.\XFont\XBmp.cpp
# SUBTRACT CPP /YX /Yc /Yu
# End Source File
# Begin Source File

SOURCE=.\XFont\XFont.cpp
# SUBTRACT CPP /YX /Yc /Yu
# End Source File
# Begin Source File

SOURCE=.\XFont\XFontGl.cpp
# SUBTRACT CPP /YX /Yc /Yu
# End Source File
# Begin Source File

SOURCE=.\XFont\XFontInf.cpp
# SUBTRACT CPP /YX /Yc /Yu
# End Source File
# Begin Source File

SOURCE=.\XFont\XFontLib.cpp
# SUBTRACT CPP /YX /Yc /Yu
# End Source File
# Begin Source File

SOURCE=.\XFont\XFontTex.cpp
# SUBTRACT CPP /YX /Yc /Yu
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\CoreRender.h
# End Source File
# Begin Source File

SOURCE=.\CoreStruct.h
# End Source File
# Begin Source File

SOURCE=.\d3d9wrapper.h
# End Source File
# Begin Source File

SOURCE=.\d3dmath.h
# End Source File
# Begin Source File

SOURCE=.\d3dutil.h
# End Source File
# Begin Source File

SOURCE=.\Engine.h
# End Source File
# Begin Source File

SOURCE=.\HandleMgr.h
# End Source File
# Begin Source File

SOURCE=.\IDisplayD3D9.h
# End Source File
# Begin Source File

SOURCE=.\IDisplayD3D91.h
# End Source File
# Begin Source File

SOURCE=.\RenderD3D9.h
# End Source File
# Begin Source File

SOURCE=.\resource.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# Begin Source File

SOURCE=.\Texture9.h
# End Source File
# Begin Source File

SOURCE=.\XFont\XBmp.h
# End Source File
# Begin Source File

SOURCE=.\XFont\XFont.h
# End Source File
# Begin Source File

SOURCE=.\XFont\XFontGl.h
# End Source File
# Begin Source File

SOURCE=.\XFont\XFontInf.h
# End Source File
# Begin Source File

SOURCE=.\XFont\XFontLib.h
# End Source File
# Begin Source File

SOURCE=.\XFont\XFontTex.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# End Group
# Begin Source File

SOURCE=.\ReadMe.txt
# End Source File
# End Target
# End Project
