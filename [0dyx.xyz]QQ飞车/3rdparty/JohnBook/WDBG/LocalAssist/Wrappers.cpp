/*----------------------------------------------------------------------
Debugging Applications for Microsoft .NET and Microsoft Windows
Copyright © 1997-2003 John Robbins -- All rights reserved.
----------------------------------------------------------------------*/
#include "PCH.h"
#include "LocalAssist.h"


BOOL LOCALASSIST_DLLINTERFACE __stdcall
    DBG_ReadProcessMemory ( HANDLE  hProcess             ,
                            LPCVOID lpBaseAddress        ,
                            LPVOID  lpBuffer             ,
                            DWORD   nSize                ,
                            LPDWORD lpNumberOfBytesRead   )
{
    return ( ReadProcessMemory ( hProcess             ,
                                 lpBaseAddress        ,
                                 lpBuffer             ,
                                 nSize                ,
                                 lpNumberOfBytesRead   ) ) ;
}


BOOL LOCALASSIST_DLLINTERFACE __stdcall
    DBG_WriteProcessMemory ( HANDLE  hProcess                ,
                             LPVOID  lpBaseAddress           ,
                             LPVOID  lpBuffer                ,
                             DWORD   nSize                   ,
                             LPDWORD lpNumberOfBytesWritten   )
{
    return ( WriteProcessMemory ( hProcess                ,
                                  lpBaseAddress           ,
                                  lpBuffer                ,
                                  nSize                   ,
                                  lpNumberOfBytesWritten   ) ) ;
}

BOOL LOCALASSIST_DLLINTERFACE __stdcall
    DBG_GetThreadContext ( HANDLE    hThread   ,
                           LPCONTEXT lpContext  )
{
    return ( GetThreadContext ( hThread , lpContext ) ) ;
}

BOOL LOCALASSIST_DLLINTERFACE __stdcall
    DBG_SetThreadContext( HANDLE          hThread   ,
                          CONST CONTEXT * lpContext  )
{
    return ( SetThreadContext ( hThread , lpContext ) ) ;
}

DWORD LOCALASSIST_DLLINTERFACE __stdcall
    DBG_SuspendThread ( HANDLE hThread )
{
    return ( SuspendThread ( hThread ) ) ;
}

DWORD LOCALASSIST_DLLINTERFACE __stdcall
    DBG_ResumeThread ( HANDLE hThread )
{
    return ( ResumeThread ( hThread ) ) ;
}

BOOL LOCALASSIST_DLLINTERFACE __stdcall
    DBG_FlushInstructionCache ( HANDLE  hProcess      ,
                                LPCVOID lpBaseAddress ,
                                DWORD   dwSize         )
{
    return ( FlushInstructionCache ( hProcess      ,
                                     lpBaseAddress ,
                                     dwSize         ) ) ;
}

DWORD LOCALASSIST_DLLINTERFACE __stdcall
    DBG_VirtualQueryEx ( HANDLE  hProcess                   ,
                         LPCVOID lpAddress                  ,
                         PMEMORY_BASIC_INFORMATION lpBuffer ,
                         DWORD   dwLength                    )
{
    return ( (DWORD)VirtualQueryEx ( hProcess  ,
                                     lpAddress ,
                                     lpBuffer  ,
                                     dwLength   ) ) ;
}

BOOL LOCALASSIST_DLLINTERFACE __stdcall
    DBG_VirtualProtectEx ( HANDLE hProcess       ,
                           LPVOID lpAddress      ,
                           DWORD  dwSize         ,
                           DWORD  flNewProtect   ,
                           PDWORD lpflOldProtect  )
{
    return ( VirtualProtectEx ( hProcess       ,
                                lpAddress      ,
                                dwSize         ,
                                flNewProtect   ,
                                lpflOldProtect  ) ) ;
}

HANDLE LOCALASSIST_DLLINTERFACE __stdcall
    DBG_CreateRemoteBreakpointThread ( HANDLE   hProcess   ,
                                       LPDWORD  lpThreadId  )
{
    HANDLE hRet = CreateRemoteThread ( hProcess             ,
                                       NULL                 ,
                                       0                    ,
                        (LPTHREAD_START_ROUTINE)DebugBreak  ,
                                       0                    ,
                                       0                    ,
                                       lpThreadId            ) ;
    return ( hRet ) ;
}

