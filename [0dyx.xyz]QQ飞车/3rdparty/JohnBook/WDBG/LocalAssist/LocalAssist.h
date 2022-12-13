/*----------------------------------------------------------------------
Debugging Applications for Microsoft .NET and Microsoft Windows
Copyright © 1997-2003 John Robbins -- All rights reserved.
----------------------------------------------------------------------*/
#ifndef _LOCALASSIST_H
#define _LOCALASSIST_H

#ifdef __cplusplus
extern "C" {
#endif

// The defines that set up how the functions or classes are exported or
// imported.
#ifndef LOCALASSIST_DLLINTERFACE
#ifdef BUILDING_LOCALASSIST_DLL
#define LOCALASSIST_DLLINTERFACE __declspec ( dllexport )
#else
#define LOCALASSIST_DLLINTERFACE __declspec ( dllimport )
#endif  // BUILDING_LOCALASSIST_DLL
#endif  // LOCALASSIST_DLLINTERFACE


// The following functions are stand-in functions for the real
// function names after the "DBG."  My idea with this Debug Interface is
// that you could implement remote debugging and the main UI would not
// have to change.  By having all these functions in this module, the UI
// portions would never care if the debuggee was local or remote.

BOOL LOCALASSIST_DLLINTERFACE __stdcall
    DBG_ReadProcessMemory ( HANDLE  hProcess             ,
                            LPCVOID lpBaseAddress        ,
                            LPVOID  lpBuffer             ,
                            DWORD   nSize                ,
                            LPDWORD lpNumberOfBytesRead   ) ;

BOOL LOCALASSIST_DLLINTERFACE __stdcall
    DBG_WriteProcessMemory ( HANDLE  hProcess                ,
                             LPVOID  lpBaseAddress           ,
                             LPVOID  lpBuffer                ,
                             DWORD   nSize                   ,
                             LPDWORD lpNumberOfBytesWritten   ) ;


BOOL LOCALASSIST_DLLINTERFACE __stdcall
    DBG_GetThreadContext ( HANDLE    hThread   ,
                           LPCONTEXT lpContext  ) ;

BOOL LOCALASSIST_DLLINTERFACE __stdcall
    DBG_SetThreadContext( HANDLE          hThread   ,
                          CONST CONTEXT * lpContext  ) ;

DWORD LOCALASSIST_DLLINTERFACE __stdcall
    DBG_SuspendThread ( HANDLE hThread ) ;

DWORD LOCALASSIST_DLLINTERFACE __stdcall
    DBG_ResumeThread ( HANDLE hThread ) ;

BOOL LOCALASSIST_DLLINTERFACE __stdcall
    DBG_FlushInstructionCache ( HANDLE  hProcess      ,
                                LPCVOID lpBaseAddress ,
                                DWORD   dwSize         ) ;

DWORD LOCALASSIST_DLLINTERFACE __stdcall
    DBG_VirtualQueryEx ( HANDLE  hProcess                   ,
                         LPCVOID lpAddress                  ,
                         PMEMORY_BASIC_INFORMATION lpBuffer ,
                         DWORD   dwLength                    ) ;

BOOL LOCALASSIST_DLLINTERFACE __stdcall
    DBG_VirtualProtectEx ( HANDLE hProcess          ,
                           LPVOID lpAddress         ,
                           DWORD  dwSize            ,
                           DWORD  flNewProtect      ,
                           PDWORD lpflOldProtect     ) ;
                           
HANDLE LOCALASSIST_DLLINTERFACE __stdcall
    DBG_CreateRemoteBreakpointThread ( HANDLE   hProcess   ,
                                       LPDWORD  lpThreadId  ) ;

#ifdef __cplusplus
}
#endif

#endif  // _LOCALASSIST_H

