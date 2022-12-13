/*----------------------------------------------------------------------
Debugging Applications for Microsoft .NET and Microsoft Windows
Copyright © 1997-2003 John Robbins -- All rights reserved.
------------------------------------------------------------------------
The function tables information.
----------------------------------------------------------------------*/

#ifndef _TABLES_H
#define _TABLES_H

// The structure that holds C run time DLL information.
typedef struct tag_CRTDLLDESC
{
    // The name of this particular DLL.
    LPCTSTR        szDllName          ;
    // The address in memory for this CRT DLL.  If it's zero, the DLL
    // is not present in the address space.
    HMODULE        hMod               ;
    // The list of hook functions.  This is shared across all CRT DLLs.
    LPHOOKFUNCDESC pHookFuncDesc      ;
    // The real functions for this DLL and this DLL only.
    LPHOOKFUNCDESC pRealFuncDesc      ;
} CRTDLLDESC , * LPCRTDLLDESC ;


// The table sizes.
#define NUMBER_KERNEL_FUNCS             43
#define NUMBER_USER_FUNCS               2
#define NUMBER_CRTDLL_FUNCS             4

// The table of DeadlockDetection functions for KERNEL32.DLL.
extern HOOKFUNCDESC g_stDDKernelFuncs[ NUMBER_KERNEL_FUNCS ] ;
// The table of real KERNEL32.DLL functions that are hooked.
extern HOOKFUNCDESC g_stRealKernelFuncs[ NUMBER_KERNEL_FUNCS ] ;
// The table of DeadlockDetection functions for USER32.DLL.
extern HOOKFUNCDESC g_stDDUserFuncs[ NUMBER_USER_FUNCS ] ;
// The table of real USER32.DLL functions that are hooked.
extern HOOKFUNCDESC g_stRealUserFuncs[ NUMBER_USER_FUNCS ] ;

// The one hook function table shared by all C run time DLLs.
extern HOOKFUNCDESC g_stCRTHookFuncs[ NUMBER_CRTDLL_FUNCS ] ;

// The table for MSVCRT.DLL
extern HOOKFUNCDESC g_stMSVCRTRealFuncs[ NUMBER_CRTDLL_FUNCS ] ;
// The table for MSVCRTD.DLL
extern HOOKFUNCDESC g_stMSVCRTDRealFuncs[ NUMBER_CRTDLL_FUNCS ] ;
// The table for MSVCR70.DLL.DLL
extern HOOKFUNCDESC g_stMSVCR70RealFuncs[ NUMBER_CRTDLL_FUNCS ] ;
// The table for MSVCR70D.DLL.DLL
extern HOOKFUNCDESC g_stMSVCR70DRealFuncs[ NUMBER_CRTDLL_FUNCS ] ;
// The table for MSVCR71.DLL.DLL
extern HOOKFUNCDESC g_stMSVCR71RealFuncs[ NUMBER_CRTDLL_FUNCS ] ;
// The table for MSVCR71D.DLL.DLL
extern HOOKFUNCDESC g_stMSVCR71DRealFuncs[ NUMBER_CRTDLL_FUNCS ] ;

// The number of C run time DLLs.
#define NUM_CRT_DLLS 6
extern CRTDLLDESC g_stCRTDLLInfo [ NUM_CRT_DLLS ] ;

// The tables for ExitProcess that needs to be hooked in ALL modules
// except for DeadlockDetection itself.
extern HOOKFUNCDESC g_stSpecialExitProcessHook ;
extern HOOKFUNCDESC g_stSpecialExitProcessReal ;



#endif  // _TABLES_H


