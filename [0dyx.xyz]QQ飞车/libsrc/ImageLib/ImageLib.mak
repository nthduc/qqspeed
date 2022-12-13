# Microsoft Developer Studio Generated NMAKE File, Based on ImageLib.dsp
!IF "$(CFG)" == ""
CFG=ImageLib - Win32 Debug
!MESSAGE No configuration specified. Defaulting to ImageLib - Win32 Debug.
!ENDIF 

!IF "$(CFG)" != "ImageLib - Win32 Release" && "$(CFG)" != "ImageLib - Win32 Debug"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "ImageLib.mak" CFG="ImageLib - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "ImageLib - Win32 Release" (based on "Win32 (x86) Static Library")
!MESSAGE "ImageLib - Win32 Debug" (based on "Win32 (x86) Static Library")
!MESSAGE 
!ERROR An invalid configuration is specified.
!ENDIF 

!IF "$(OS)" == "Windows_NT"
NULL=
!ELSE 
NULL=nul
!ENDIF 

!IF  "$(CFG)" == "ImageLib - Win32 Release"

OUTDIR=.\Release
INTDIR=\___IntermediateFiles___\MMOG\ImageLib_Release_WIN32
# Begin Custom Macros
OutDir=.\Release
# End Custom Macros

!IF "$(RECURSE)" == "0" 

ALL : "$(OUTDIR)\ImageLib.lib" "$(OUTDIR)\ImageLib.bsc"

!ELSE 

ALL : "XImage - Win32 Release" "kernel - Win32 Release" "$(OUTDIR)\ImageLib.lib" "$(OUTDIR)\ImageLib.bsc"

!ENDIF 

!IF "$(RECURSE)" == "1" 
CLEAN :"kernel - Win32 ReleaseCLEAN" "XImage - Win32 ReleaseCLEAN" 
!ELSE 
CLEAN :
!ENDIF 
	-@erase "$(INTDIR)\GameImageLib.obj"
	-@erase "$(INTDIR)\GameImageLib.sbr"
	-@erase "$(INTDIR)\IDImage.obj"
	-@erase "$(INTDIR)\IDImage.sbr"
	-@erase "$(INTDIR)\ImageAni.obj"
	-@erase "$(INTDIR)\ImageAni.sbr"
	-@erase "$(INTDIR)\ImageLib.obj"
	-@erase "$(INTDIR)\ImageLib.sbr"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(OUTDIR)\ImageLib.bsc"
	-@erase "$(OUTDIR)\ImageLib.lib"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

"$(INTDIR)" :
    if not exist "$(INTDIR)/$(NULL)" mkdir "$(INTDIR)"

CPP=cl.exe
CPP_PROJ=/nologo /MD /W3 /GX /O1 /I "..\..\Include" /I ".\\" /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\ImageLib.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

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
BSC32_FLAGS=/nologo /o"$(OUTDIR)\ImageLib.bsc" 
BSC32_SBRS= \
	"$(INTDIR)\GameImageLib.sbr" \
	"$(INTDIR)\IDImage.sbr" \
	"$(INTDIR)\ImageAni.sbr" \
	"$(INTDIR)\ImageLib.sbr"

"$(OUTDIR)\ImageLib.bsc" : "$(OUTDIR)" $(BSC32_SBRS)
    $(BSC32) @<<
  $(BSC32_FLAGS) $(BSC32_SBRS)
<<

LIB32=link.exe -lib
LIB32_FLAGS=/nologo /out:"$(OUTDIR)\ImageLib.lib" 
LIB32_OBJS= \
	"$(INTDIR)\GameImageLib.obj" \
	"$(INTDIR)\IDImage.obj" \
	"$(INTDIR)\ImageAni.obj" \
	"$(INTDIR)\ImageLib.obj" \
	"..\..\lib\kernel.lib" \
	"..\..\Modules\XImage\Release\XImage.lib"

"$(OUTDIR)\ImageLib.lib" : "$(OUTDIR)" $(DEF_FILE) $(LIB32_OBJS)
    $(LIB32) @<<
  $(LIB32_FLAGS) $(DEF_FLAGS) $(LIB32_OBJS)
<<

SOURCE="$(InputPath)"
DS_POSTBUILD_DEP=$(INTDIR)\postbld.dep

ALL : $(DS_POSTBUILD_DEP)

# Begin Custom Macros
OutDir=.\Release
# End Custom Macros

$(DS_POSTBUILD_DEP) : "XImage - Win32 Release" "kernel - Win32 Release" "$(OUTDIR)\ImageLib.lib" "$(OUTDIR)\ImageLib.bsc"
   call PostBuild.bat
	echo Helper for Post-build step > "$(DS_POSTBUILD_DEP)"

!ELSEIF  "$(CFG)" == "ImageLib - Win32 Debug"

OUTDIR=.\Debug
INTDIR=\___IntermediateFiles___\MMOG\ImageLib_Debug_WIN32
# Begin Custom Macros
OutDir=.\Debug
# End Custom Macros

!IF "$(RECURSE)" == "0" 

ALL : "$(OUTDIR)\ImageLibD.lib" "$(OUTDIR)\ImageLib.bsc"

!ELSE 

ALL : "XImage - Win32 Debug" "kernel - Win32 Debug" "$(OUTDIR)\ImageLibD.lib" "$(OUTDIR)\ImageLib.bsc"

!ENDIF 

!IF "$(RECURSE)" == "1" 
CLEAN :"kernel - Win32 DebugCLEAN" "XImage - Win32 DebugCLEAN" 
!ELSE 
CLEAN :
!ENDIF 
	-@erase "$(INTDIR)\GameImageLib.obj"
	-@erase "$(INTDIR)\GameImageLib.sbr"
	-@erase "$(INTDIR)\IDImage.obj"
	-@erase "$(INTDIR)\IDImage.sbr"
	-@erase "$(INTDIR)\ImageAni.obj"
	-@erase "$(INTDIR)\ImageAni.sbr"
	-@erase "$(INTDIR)\ImageLib.obj"
	-@erase "$(INTDIR)\ImageLib.sbr"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\vc60.pdb"
	-@erase "$(OUTDIR)\ImageLib.bsc"
	-@erase "$(OUTDIR)\ImageLibD.lib"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

"$(INTDIR)" :
    if not exist "$(INTDIR)/$(NULL)" mkdir "$(INTDIR)"

CPP=cl.exe
CPP_PROJ=/nologo /MDd /W3 /Gm /GX /ZI /Od /I "..\..\Include" /I ".\\" /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /FR"$(INTDIR)\\" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 

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
BSC32_FLAGS=/nologo /o"$(OUTDIR)\ImageLib.bsc" 
BSC32_SBRS= \
	"$(INTDIR)\GameImageLib.sbr" \
	"$(INTDIR)\IDImage.sbr" \
	"$(INTDIR)\ImageAni.sbr" \
	"$(INTDIR)\ImageLib.sbr"

"$(OUTDIR)\ImageLib.bsc" : "$(OUTDIR)" $(BSC32_SBRS)
    $(BSC32) @<<
  $(BSC32_FLAGS) $(BSC32_SBRS)
<<

LIB32=link.exe -lib
LIB32_FLAGS=/nologo /out:"$(OUTDIR)\ImageLibD.lib" 
LIB32_OBJS= \
	"$(INTDIR)\GameImageLib.obj" \
	"$(INTDIR)\IDImage.obj" \
	"$(INTDIR)\ImageAni.obj" \
	"$(INTDIR)\ImageLib.obj" \
	"..\..\lib\kernel_d.lib" \
	"..\..\Modules\XImage\Debug\XImageD.lib"

"$(OUTDIR)\ImageLibD.lib" : "$(OUTDIR)" $(DEF_FILE) $(LIB32_OBJS)
    $(LIB32) @<<
  $(LIB32_FLAGS) $(DEF_FLAGS) $(LIB32_OBJS)
<<

SOURCE="$(InputPath)"
DS_POSTBUILD_DEP=$(INTDIR)\postbld.dep

ALL : $(DS_POSTBUILD_DEP)

# Begin Custom Macros
OutDir=.\Debug
# End Custom Macros

$(DS_POSTBUILD_DEP) : "XImage - Win32 Debug" "kernel - Win32 Debug" "$(OUTDIR)\ImageLibD.lib" "$(OUTDIR)\ImageLib.bsc"
   call PostBuild.bat
	echo Helper for Post-build step > "$(DS_POSTBUILD_DEP)"

!ENDIF 


!IF "$(NO_EXTERNAL_DEPS)" != "1"
!IF EXISTS("ImageLib.dep")
!INCLUDE "ImageLib.dep"
!ELSE 
!MESSAGE Warning: cannot find "ImageLib.dep"
!ENDIF 
!ENDIF 


!IF "$(CFG)" == "ImageLib - Win32 Release" || "$(CFG)" == "ImageLib - Win32 Debug"
SOURCE=.\GameImageLib.cpp

"$(INTDIR)\GameImageLib.obj"	"$(INTDIR)\GameImageLib.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\IDImage.cpp

"$(INTDIR)\IDImage.obj"	"$(INTDIR)\IDImage.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\ImageAni.cpp

"$(INTDIR)\ImageAni.obj"	"$(INTDIR)\ImageAni.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\ImageLib.cpp

"$(INTDIR)\ImageLib.obj"	"$(INTDIR)\ImageLib.sbr" : $(SOURCE) "$(INTDIR)"


!IF  "$(CFG)" == "ImageLib - Win32 Release"

"kernel - Win32 Release" : 
   cd "\lincoln_MMOGProj_int\IED_MMOG_VOB\MMOG\ClientSource\LibSrc\kernel"
   $(MAKE) /$(MAKEFLAGS) /F .\kernel.mak CFG="kernel - Win32 Release" 
   cd "..\ImageLib"

"kernel - Win32 ReleaseCLEAN" : 
   cd "\lincoln_MMOGProj_int\IED_MMOG_VOB\MMOG\ClientSource\LibSrc\kernel"
   $(MAKE) /$(MAKEFLAGS) /F .\kernel.mak CFG="kernel - Win32 Release" RECURSE=1 CLEAN 
   cd "..\ImageLib"

!ELSEIF  "$(CFG)" == "ImageLib - Win32 Debug"

"kernel - Win32 Debug" : 
   cd "\lincoln_MMOGProj_int\IED_MMOG_VOB\MMOG\ClientSource\LibSrc\kernel"
   $(MAKE) /$(MAKEFLAGS) /F .\kernel.mak CFG="kernel - Win32 Debug" 
   cd "..\ImageLib"

"kernel - Win32 DebugCLEAN" : 
   cd "\lincoln_MMOGProj_int\IED_MMOG_VOB\MMOG\ClientSource\LibSrc\kernel"
   $(MAKE) /$(MAKEFLAGS) /F .\kernel.mak CFG="kernel - Win32 Debug" RECURSE=1 CLEAN 
   cd "..\ImageLib"

!ENDIF 

!IF  "$(CFG)" == "ImageLib - Win32 Release"

"XImage - Win32 Release" : 
   cd "\lincoln_MMOGProj_int\IED_MMOG_VOB\MMOG\ClientSource\Modules\XImage"
   $(MAKE) /$(MAKEFLAGS) /F .\XImage.mak CFG="XImage - Win32 Release" 
   cd "..\..\LibSrc\ImageLib"

"XImage - Win32 ReleaseCLEAN" : 
   cd "\lincoln_MMOGProj_int\IED_MMOG_VOB\MMOG\ClientSource\Modules\XImage"
   $(MAKE) /$(MAKEFLAGS) /F .\XImage.mak CFG="XImage - Win32 Release" RECURSE=1 CLEAN 
   cd "..\..\LibSrc\ImageLib"

!ELSEIF  "$(CFG)" == "ImageLib - Win32 Debug"

"XImage - Win32 Debug" : 
   cd "\lincoln_MMOGProj_int\IED_MMOG_VOB\MMOG\ClientSource\Modules\XImage"
   $(MAKE) /$(MAKEFLAGS) /F .\XImage.mak CFG="XImage - Win32 Debug" 
   cd "..\..\LibSrc\ImageLib"

"XImage - Win32 DebugCLEAN" : 
   cd "\lincoln_MMOGProj_int\IED_MMOG_VOB\MMOG\ClientSource\Modules\XImage"
   $(MAKE) /$(MAKEFLAGS) /F .\XImage.mak CFG="XImage - Win32 Debug" RECURSE=1 CLEAN 
   cd "..\..\LibSrc\ImageLib"

!ENDIF 


!ENDIF 

