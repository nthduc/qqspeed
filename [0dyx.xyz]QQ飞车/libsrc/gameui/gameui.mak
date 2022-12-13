# Microsoft Developer Studio Generated NMAKE File, Based on gameui.dsp
!IF "$(CFG)" == ""
CFG=gameui - Win32 Debug
!MESSAGE No configuration specified. Defaulting to gameui - Win32 Debug.
!ENDIF 

!IF "$(CFG)" != "gameui - Win32 Release" && "$(CFG)" != "gameui - Win32 Debug"
!MESSAGE Invalid configuration "$(CFG)" specified.
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
!ERROR An invalid configuration is specified.
!ENDIF 

!IF "$(OS)" == "Windows_NT"
NULL=
!ELSE 
NULL=nul
!ENDIF 

!IF  "$(CFG)" == "gameui - Win32 Release"

OUTDIR=.\Release
INTDIR=\___IntermediateFiles___\MMOG\GameUI_Release_WIN32

!IF "$(RECURSE)" == "0" 

ALL : "..\..\lib\gameui.lib"

!ELSE 

ALL : "Display - Win32 Release" "ImageLib - Win32 Release" "..\..\lib\gameui.lib"

!ENDIF 

!IF "$(RECURSE)" == "1" 
CLEAN :"ImageLib - Win32 ReleaseCLEAN" "Display - Win32 ReleaseCLEAN" 
!ELSE 
CLEAN :
!ENDIF 
	-@erase "$(INTDIR)\gameui.obj"
	-@erase "$(INTDIR)\uicontrol.obj"
	-@erase "$(INTDIR)\uiimage.obj"
	-@erase "$(INTDIR)\uiscriptfunc.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\vc60.pdb"
	-@erase "$(INTDIR)\widget.obj"
	-@erase "..\..\lib\gameui.lib"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

"$(INTDIR)" :
    if not exist "$(INTDIR)/$(NULL)" mkdir "$(INTDIR)"

CPP=cl.exe
CPP_PROJ=/nologo /MD /W3 /GX /Zi /O1 /I "..\..\Include" /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /Fp"$(INTDIR)\gameui.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

.c{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.c{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

RSC=rc.exe
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\gameui.bsc" 
BSC32_SBRS= \
	
LIB32=link.exe -lib
LIB32_FLAGS=/nologo /out:"..\..\lib\gameui.lib" 
LIB32_OBJS= \
	"$(INTDIR)\gameui.obj" \
	"$(INTDIR)\uicontrol.obj" \
	"$(INTDIR)\uiimage.obj" \
	"$(INTDIR)\uiscriptfunc.obj" \
	"$(INTDIR)\widget.obj" \
	"..\ImageLib\Release\ImageLib.lib" \
	"..\Display\Release\Display.lib"

"..\..\lib\gameui.lib" : "$(OUTDIR)" $(DEF_FILE) $(LIB32_OBJS)
    $(LIB32) @<<
  $(LIB32_FLAGS) $(DEF_FLAGS) $(LIB32_OBJS)
<<

SOURCE="$(InputPath)"
DS_POSTBUILD_DEP=$(INTDIR)\postbld.dep

ALL : $(DS_POSTBUILD_DEP)

$(DS_POSTBUILD_DEP) : "Display - Win32 Release" "ImageLib - Win32 Release" "..\..\lib\gameui.lib"
   call PostBuild.bat
	echo Helper for Post-build step > "$(DS_POSTBUILD_DEP)"

!ELSEIF  "$(CFG)" == "gameui - Win32 Debug"

OUTDIR=.\Debug
INTDIR=\___IntermediateFiles___\MMOG\GameUI_Debug_WIN32
# Begin Custom Macros
OutDir=.\Debug
# End Custom Macros

!IF "$(RECURSE)" == "0" 

ALL : "..\..\lib\gameui_d.lib" "$(OUTDIR)\gameui.bsc"

!ELSE 

ALL : "Display - Win32 Debug" "ImageLib - Win32 Debug" "..\..\lib\gameui_d.lib" "$(OUTDIR)\gameui.bsc"

!ENDIF 

!IF "$(RECURSE)" == "1" 
CLEAN :"ImageLib - Win32 DebugCLEAN" "Display - Win32 DebugCLEAN" 
!ELSE 
CLEAN :
!ENDIF 
	-@erase "$(INTDIR)\gameui.obj"
	-@erase "$(INTDIR)\gameui.sbr"
	-@erase "$(INTDIR)\uicontrol.obj"
	-@erase "$(INTDIR)\uicontrol.sbr"
	-@erase "$(INTDIR)\uiimage.obj"
	-@erase "$(INTDIR)\uiimage.sbr"
	-@erase "$(INTDIR)\uiscriptfunc.obj"
	-@erase "$(INTDIR)\uiscriptfunc.sbr"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\vc60.pdb"
	-@erase "$(INTDIR)\widget.obj"
	-@erase "$(INTDIR)\widget.sbr"
	-@erase "$(OUTDIR)\gameui.bsc"
	-@erase "..\..\lib\gameui_d.lib"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

"$(INTDIR)" :
    if not exist "$(INTDIR)/$(NULL)" mkdir "$(INTDIR)"

CPP=cl.exe
CPP_PROJ=/nologo /MDd /W3 /Gm /GX /ZI /Od /I "..\..\Include" /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /D "_UIEDITMODE" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\gameui.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 

.c{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.c{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

RSC=rc.exe
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\gameui.bsc" 
BSC32_SBRS= \
	"$(INTDIR)\gameui.sbr" \
	"$(INTDIR)\uicontrol.sbr" \
	"$(INTDIR)\uiimage.sbr" \
	"$(INTDIR)\uiscriptfunc.sbr" \
	"$(INTDIR)\widget.sbr"

"$(OUTDIR)\gameui.bsc" : "$(OUTDIR)" $(BSC32_SBRS)
    $(BSC32) @<<
  $(BSC32_FLAGS) $(BSC32_SBRS)
<<

LIB32=link.exe -lib
LIB32_FLAGS=/nologo /out:"..\..\lib\gameui_d.lib" 
LIB32_OBJS= \
	"$(INTDIR)\gameui.obj" \
	"$(INTDIR)\uicontrol.obj" \
	"$(INTDIR)\uiimage.obj" \
	"$(INTDIR)\uiscriptfunc.obj" \
	"$(INTDIR)\widget.obj" \
	"..\ImageLib\Debug\ImageLibD.lib" \
	"..\Display\Debug\DisplayD.lib"

"..\..\lib\gameui_d.lib" : "$(OUTDIR)" $(DEF_FILE) $(LIB32_OBJS)
    $(LIB32) @<<
  $(LIB32_FLAGS) $(DEF_FLAGS) $(LIB32_OBJS)
<<

SOURCE="$(InputPath)"
DS_POSTBUILD_DEP=$(INTDIR)\postbld.dep

ALL : $(DS_POSTBUILD_DEP)

# Begin Custom Macros
OutDir=.\Debug
# End Custom Macros

$(DS_POSTBUILD_DEP) : "Display - Win32 Debug" "ImageLib - Win32 Debug" "..\..\lib\gameui_d.lib" "$(OUTDIR)\gameui.bsc"
   call PostBuild.bat
	echo Helper for Post-build step > "$(DS_POSTBUILD_DEP)"

!ENDIF 


!IF "$(NO_EXTERNAL_DEPS)" != "1"
!IF EXISTS("gameui.dep")
!INCLUDE "gameui.dep"
!ELSE 
!MESSAGE Warning: cannot find "gameui.dep"
!ENDIF 
!ENDIF 


!IF "$(CFG)" == "gameui - Win32 Release" || "$(CFG)" == "gameui - Win32 Debug"
SOURCE=.\gameui.cpp

!IF  "$(CFG)" == "gameui - Win32 Release"


"$(INTDIR)\gameui.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "gameui - Win32 Debug"


"$(INTDIR)\gameui.obj"	"$(INTDIR)\gameui.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\uicontrol.cpp

!IF  "$(CFG)" == "gameui - Win32 Release"


"$(INTDIR)\uicontrol.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "gameui - Win32 Debug"


"$(INTDIR)\uicontrol.obj"	"$(INTDIR)\uicontrol.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\uiimage.cpp

!IF  "$(CFG)" == "gameui - Win32 Release"


"$(INTDIR)\uiimage.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "gameui - Win32 Debug"


"$(INTDIR)\uiimage.obj"	"$(INTDIR)\uiimage.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\uiscriptfunc.cpp

!IF  "$(CFG)" == "gameui - Win32 Release"


"$(INTDIR)\uiscriptfunc.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "gameui - Win32 Debug"


"$(INTDIR)\uiscriptfunc.obj"	"$(INTDIR)\uiscriptfunc.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\widget.cpp

!IF  "$(CFG)" == "gameui - Win32 Release"


"$(INTDIR)\widget.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "gameui - Win32 Debug"


"$(INTDIR)\widget.obj"	"$(INTDIR)\widget.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

!IF  "$(CFG)" == "gameui - Win32 Release"

"ImageLib - Win32 Release" : 
   cd "\lincoln_MMOGProj_int\IED_MMOG_VOB\MMOG\ClientSource\LibSrc\ImageLib"
   $(MAKE) /$(MAKEFLAGS) /F .\ImageLib.mak CFG="ImageLib - Win32 Release" 
   cd "..\gameui"

"ImageLib - Win32 ReleaseCLEAN" : 
   cd "\lincoln_MMOGProj_int\IED_MMOG_VOB\MMOG\ClientSource\LibSrc\ImageLib"
   $(MAKE) /$(MAKEFLAGS) /F .\ImageLib.mak CFG="ImageLib - Win32 Release" RECURSE=1 CLEAN 
   cd "..\gameui"

!ELSEIF  "$(CFG)" == "gameui - Win32 Debug"

"ImageLib - Win32 Debug" : 
   cd "\lincoln_MMOGProj_int\IED_MMOG_VOB\MMOG\ClientSource\LibSrc\ImageLib"
   $(MAKE) /$(MAKEFLAGS) /F .\ImageLib.mak CFG="ImageLib - Win32 Debug" 
   cd "..\gameui"

"ImageLib - Win32 DebugCLEAN" : 
   cd "\lincoln_MMOGProj_int\IED_MMOG_VOB\MMOG\ClientSource\LibSrc\ImageLib"
   $(MAKE) /$(MAKEFLAGS) /F .\ImageLib.mak CFG="ImageLib - Win32 Debug" RECURSE=1 CLEAN 
   cd "..\gameui"

!ENDIF 

!IF  "$(CFG)" == "gameui - Win32 Release"

"Display - Win32 Release" : 
   cd "\lincoln_MMOGProj_int\IED_MMOG_VOB\MMOG\ClientSource\LibSrc\Display"
   $(MAKE) /$(MAKEFLAGS) /F .\Display.mak CFG="Display - Win32 Release" 
   cd "..\gameui"

"Display - Win32 ReleaseCLEAN" : 
   cd "\lincoln_MMOGProj_int\IED_MMOG_VOB\MMOG\ClientSource\LibSrc\Display"
   $(MAKE) /$(MAKEFLAGS) /F .\Display.mak CFG="Display - Win32 Release" RECURSE=1 CLEAN 
   cd "..\gameui"

!ELSEIF  "$(CFG)" == "gameui - Win32 Debug"

"Display - Win32 Debug" : 
   cd "\lincoln_MMOGProj_int\IED_MMOG_VOB\MMOG\ClientSource\LibSrc\Display"
   $(MAKE) /$(MAKEFLAGS) /F .\Display.mak CFG="Display - Win32 Debug" 
   cd "..\gameui"

"Display - Win32 DebugCLEAN" : 
   cd "\lincoln_MMOGProj_int\IED_MMOG_VOB\MMOG\ClientSource\LibSrc\Display"
   $(MAKE) /$(MAKEFLAGS) /F .\Display.mak CFG="Display - Win32 Debug" RECURSE=1 CLEAN 
   cd "..\gameui"

!ENDIF 


!ENDIF 

