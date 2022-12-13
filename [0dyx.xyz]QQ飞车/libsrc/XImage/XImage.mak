# Microsoft Developer Studio Generated NMAKE File, Based on XImage.dsp
!IF "$(CFG)" == ""
CFG=XImage - Win32 Debug
!MESSAGE No configuration specified. Defaulting to XImage - Win32 Debug.
!ENDIF 

!IF "$(CFG)" != "XImage - Win32 Release" && "$(CFG)" != "XImage - Win32 Debug"
!MESSAGE Invalid configuration "$(CFG)" specified.
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
!ERROR An invalid configuration is specified.
!ENDIF 

!IF "$(OS)" == "Windows_NT"
NULL=
!ELSE 
NULL=nul
!ENDIF 

!IF  "$(CFG)" == "XImage - Win32 Release"

OUTDIR=.\Release
INTDIR=\___IntermediateFiles___\MMOG\XImage_Release_Win32
# Begin Custom Macros
OutDir=.\Release
# End Custom Macros

ALL : "$(OUTDIR)\XImage.lib" "$(OUTDIR)\XImage.bsc"


CLEAN :
	-@erase "$(INTDIR)\gif89a.obj"
	-@erase "$(INTDIR)\gif89a.sbr"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\xBmp.obj"
	-@erase "$(INTDIR)\xBmp.sbr"
	-@erase "$(INTDIR)\xFlic.obj"
	-@erase "$(INTDIR)\xFlic.sbr"
	-@erase "$(INTDIR)\xGif.obj"
	-@erase "$(INTDIR)\xGif.sbr"
	-@erase "$(INTDIR)\xPcx.obj"
	-@erase "$(INTDIR)\xPcx.sbr"
	-@erase "$(INTDIR)\xPsd.obj"
	-@erase "$(INTDIR)\xPsd.sbr"
	-@erase "$(INTDIR)\xTga.obj"
	-@erase "$(INTDIR)\xTga.sbr"
	-@erase "$(OUTDIR)\XImage.bsc"
	-@erase "$(OUTDIR)\XImage.lib"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

"$(INTDIR)" :
    if not exist "$(INTDIR)/$(NULL)" mkdir "$(INTDIR)"

CPP=cl.exe
CPP_PROJ=/nologo /MD /W3 /GX /O1 /I "..\..\Dx8Include" /I "..\..\include" /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\XImage.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

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
BSC32_FLAGS=/nologo /o"$(OUTDIR)\XImage.bsc" 
BSC32_SBRS= \
	"$(INTDIR)\gif89a.sbr" \
	"$(INTDIR)\xBmp.sbr" \
	"$(INTDIR)\xFlic.sbr" \
	"$(INTDIR)\xGif.sbr" \
	"$(INTDIR)\xPcx.sbr" \
	"$(INTDIR)\xPsd.sbr" \
	"$(INTDIR)\xTga.sbr"

"$(OUTDIR)\XImage.bsc" : "$(OUTDIR)" $(BSC32_SBRS)
    $(BSC32) @<<
  $(BSC32_FLAGS) $(BSC32_SBRS)
<<

LIB32=link.exe -lib
LIB32_FLAGS=/nologo /out:"$(OUTDIR)\XImage.lib" 
LIB32_OBJS= \
	"$(INTDIR)\gif89a.obj" \
	"$(INTDIR)\xBmp.obj" \
	"$(INTDIR)\xFlic.obj" \
	"$(INTDIR)\xGif.obj" \
	"$(INTDIR)\xPcx.obj" \
	"$(INTDIR)\xPsd.obj" \
	"$(INTDIR)\xTga.obj"

"$(OUTDIR)\XImage.lib" : "$(OUTDIR)" $(DEF_FILE) $(LIB32_OBJS)
    $(LIB32) @<<
  $(LIB32_FLAGS) $(DEF_FLAGS) $(LIB32_OBJS)
<<

SOURCE="$(InputPath)"
DS_POSTBUILD_DEP=$(INTDIR)\postbld.dep

ALL : $(DS_POSTBUILD_DEP)

# Begin Custom Macros
OutDir=.\Release
# End Custom Macros

$(DS_POSTBUILD_DEP) : "$(OUTDIR)\XImage.lib" "$(OUTDIR)\XImage.bsc"
   call PostBuild.bat
	echo Helper for Post-build step > "$(DS_POSTBUILD_DEP)"

!ELSEIF  "$(CFG)" == "XImage - Win32 Debug"

OUTDIR=.\Debug
INTDIR=\___IntermediateFiles___\MMOG\XImage_Debug_Win32
# Begin Custom Macros
OutDir=.\Debug
# End Custom Macros

ALL : "$(OUTDIR)\XImageD.lib" "$(OUTDIR)\XImage.bsc"


CLEAN :
	-@erase "$(INTDIR)\gif89a.obj"
	-@erase "$(INTDIR)\gif89a.sbr"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\vc60.pdb"
	-@erase "$(INTDIR)\xBmp.obj"
	-@erase "$(INTDIR)\xBmp.sbr"
	-@erase "$(INTDIR)\xFlic.obj"
	-@erase "$(INTDIR)\xFlic.sbr"
	-@erase "$(INTDIR)\xGif.obj"
	-@erase "$(INTDIR)\xGif.sbr"
	-@erase "$(INTDIR)\xPcx.obj"
	-@erase "$(INTDIR)\xPcx.sbr"
	-@erase "$(INTDIR)\xPsd.obj"
	-@erase "$(INTDIR)\xPsd.sbr"
	-@erase "$(INTDIR)\xTga.obj"
	-@erase "$(INTDIR)\xTga.sbr"
	-@erase "$(OUTDIR)\XImage.bsc"
	-@erase "$(OUTDIR)\XImageD.lib"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

"$(INTDIR)" :
    if not exist "$(INTDIR)/$(NULL)" mkdir "$(INTDIR)"

CPP=cl.exe
CPP_PROJ=/nologo /MDd /W3 /Gm /GX /ZI /Od /I "..\..\Dx8Include" /I "..\..\include" /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\XImage.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 

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
BSC32_FLAGS=/nologo /o"$(OUTDIR)\XImage.bsc" 
BSC32_SBRS= \
	"$(INTDIR)\gif89a.sbr" \
	"$(INTDIR)\xBmp.sbr" \
	"$(INTDIR)\xFlic.sbr" \
	"$(INTDIR)\xGif.sbr" \
	"$(INTDIR)\xPcx.sbr" \
	"$(INTDIR)\xPsd.sbr" \
	"$(INTDIR)\xTga.sbr"

"$(OUTDIR)\XImage.bsc" : "$(OUTDIR)" $(BSC32_SBRS)
    $(BSC32) @<<
  $(BSC32_FLAGS) $(BSC32_SBRS)
<<

LIB32=link.exe -lib
LIB32_FLAGS=/nologo /out:"$(OUTDIR)\XImageD.lib" 
LIB32_OBJS= \
	"$(INTDIR)\gif89a.obj" \
	"$(INTDIR)\xBmp.obj" \
	"$(INTDIR)\xFlic.obj" \
	"$(INTDIR)\xGif.obj" \
	"$(INTDIR)\xPcx.obj" \
	"$(INTDIR)\xPsd.obj" \
	"$(INTDIR)\xTga.obj"

"$(OUTDIR)\XImageD.lib" : "$(OUTDIR)" $(DEF_FILE) $(LIB32_OBJS)
    $(LIB32) @<<
  $(LIB32_FLAGS) $(DEF_FLAGS) $(LIB32_OBJS)
<<

SOURCE="$(InputPath)"
DS_POSTBUILD_DEP=$(INTDIR)\postbld.dep

ALL : $(DS_POSTBUILD_DEP)

# Begin Custom Macros
OutDir=.\Debug
# End Custom Macros

$(DS_POSTBUILD_DEP) : "$(OUTDIR)\XImageD.lib" "$(OUTDIR)\XImage.bsc"
   call PostBuild.bat
	echo Helper for Post-build step > "$(DS_POSTBUILD_DEP)"

!ENDIF 


!IF "$(NO_EXTERNAL_DEPS)" != "1"
!IF EXISTS("XImage.dep")
!INCLUDE "XImage.dep"
!ELSE 
!MESSAGE Warning: cannot find "XImage.dep"
!ENDIF 
!ENDIF 


!IF "$(CFG)" == "XImage - Win32 Release" || "$(CFG)" == "XImage - Win32 Debug"
SOURCE=.\gif89a.cpp

"$(INTDIR)\gif89a.obj"	"$(INTDIR)\gif89a.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\xBmp.cpp

"$(INTDIR)\xBmp.obj"	"$(INTDIR)\xBmp.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\xFlic.cpp

"$(INTDIR)\xFlic.obj"	"$(INTDIR)\xFlic.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\xGif.cpp

"$(INTDIR)\xGif.obj"	"$(INTDIR)\xGif.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\xPcx.cpp

"$(INTDIR)\xPcx.obj"	"$(INTDIR)\xPcx.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\xPsd.cpp

"$(INTDIR)\xPsd.obj"	"$(INTDIR)\xPsd.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\xTga.cpp

"$(INTDIR)\xTga.obj"	"$(INTDIR)\xTga.sbr" : $(SOURCE) "$(INTDIR)"



!ENDIF 

