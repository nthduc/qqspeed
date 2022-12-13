/*----------------------------------------------------------------------
Debugging Applications for Microsoft .NET and Microsoft Windows
Copyright © 1997-2003 John Robbins -- All rights reserved.
------------------------------------------------------------------------
The mutex functions.
----------------------------------------------------------------------*/

#include "PCH.h"
#include "DeadlockDetection.h"
#include "Internal.h"

// Turn off all optimizations to avoid registers being changed behind
// my back.
#pragma optimize( "", off )

// Disable the "conditional expression is constant" in the
// HOOKFN_STARTUP macro.
#pragma warning ( disable : 4127 )

HANDLE NAKEDDEF DD_CreateMutexA(LPSECURITY_ATTRIBUTES lpMutexAttributes,
                                BOOL                  bInitialOwner    ,
                                LPCSTR                lpName           )
{
    HOOKFN_STARTUP ( eCreateMutexA , DDOPT_MUTEX , FALSE ) ;

    CreateMutexA ( lpMutexAttributes ,
                   bInitialOwner     ,
                   lpName             ) ;

    HOOKFN_SHUTDOWN ( 3 , DDOPT_MUTEX ) ;
}

HANDLE NAKEDDEF DD_CreateMutexW(LPSECURITY_ATTRIBUTES lpMutexAttributes,
                                BOOL                  bInitialOwner    ,
                                LPCWSTR               lpName           )
{
    HOOKFN_STARTUP ( eCreateMutexW , DDOPT_MUTEX , FALSE ) ;

    CreateMutexW ( lpMutexAttributes ,
                   bInitialOwner     ,
                   lpName             ) ;

    HOOKFN_SHUTDOWN ( 3 , DDOPT_MUTEX ) ;
}

HANDLE NAKEDDEF DD_OpenMutexA ( DWORD  dwDesiredAccess ,
                                BOOL   bInheritHandle  ,
                                LPCSTR lpName           )
{
    HOOKFN_STARTUP ( eOpenMutexA , DDOPT_MUTEX , FALSE ) ;

    OpenMutexA ( dwDesiredAccess ,
                 bInheritHandle  ,
                 lpName           ) ;

    HOOKFN_SHUTDOWN ( 3 , DDOPT_MUTEX ) ;
}

HANDLE NAKEDDEF DD_OpenMutexW ( DWORD   dwDesiredAccess ,
                                BOOL    bInheritHandle  ,
                                LPCWSTR lpName           )
{
    HOOKFN_STARTUP ( eOpenMutexW , DDOPT_MUTEX , FALSE ) ;

    OpenMutexW ( dwDesiredAccess ,
                 bInheritHandle  ,
                 lpName           ) ;

    HOOKFN_SHUTDOWN ( 3 , DDOPT_MUTEX ) ;
}

BOOL NAKEDDEF DD_ReleaseMutex ( HANDLE hMutex )
{

    HOOKFN_STARTUP ( eReleaseMutex , DDOPT_MUTEX , FALSE ) ;
    
    ReleaseMutex ( hMutex ) ;
    
    HOOKFN_SHUTDOWN ( 1 , DDOPT_MUTEX ) ;
}

// Turn optimizations back on.
#pragma optimize( "", on )

