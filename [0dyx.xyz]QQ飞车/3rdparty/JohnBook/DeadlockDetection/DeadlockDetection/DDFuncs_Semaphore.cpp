/*----------------------------------------------------------------------
Debugging Applications for Microsoft .NET and Microsoft Windows
Copyright © 1997-2003 John Robbins -- All rights reserved.
------------------------------------------------------------------------
The semaphore functions.
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

HANDLE NAKEDDEF
    DD_CreateSemaphoreA ( LPSECURITY_ATTRIBUTES lpSemaphoreAttributes,
                          LONG lInitialCount,
                          LONG lMaximumCount,
                          LPCSTR lpName )
{
    HOOKFN_STARTUP ( eCreateSemaphoreA , DDOPT_SEMAPHORE , FALSE ) ;

    CreateSemaphoreA ( lpSemaphoreAttributes ,
                       lInitialCount         ,
                       lMaximumCount         ,
                       lpName                 ) ;

    HOOKFN_SHUTDOWN ( 4 , DDOPT_SEMAPHORE ) ;
}

HANDLE NAKEDDEF
    DD_CreateSemaphoreW ( LPSECURITY_ATTRIBUTES lpSemaphoreAttributes,
                          LONG lInitialCount,
                          LONG lMaximumCount,
                          LPCWSTR lpName )
{
    HOOKFN_STARTUP ( eCreateSemaphoreW , DDOPT_SEMAPHORE , FALSE ) ;

    CreateSemaphoreW ( lpSemaphoreAttributes ,
                       lInitialCount         ,
                       lMaximumCount         ,
                       lpName                 ) ;

    HOOKFN_SHUTDOWN ( 4 , DDOPT_SEMAPHORE ) ;
}

HANDLE NAKEDDEF DD_OpenSemaphoreA ( DWORD  dwDesiredAccess ,
                                    BOOL   bInheritHandle  ,
                                    LPCSTR lpName           )
{
    HOOKFN_STARTUP ( eOpenSemaphoreA , DDOPT_SEMAPHORE , FALSE ) ;

    OpenSemaphoreA ( dwDesiredAccess ,
                     bInheritHandle  ,
                     lpName           ) ;
                     
    HOOKFN_SHUTDOWN ( 3 , DDOPT_SEMAPHORE ) ;
}

HANDLE NAKEDDEF DD_OpenSemaphoreW ( DWORD dwDesiredAccess,
                                    BOOL bInheritHandle,
                                    LPCWSTR lpName )
{
    HOOKFN_STARTUP ( eOpenSemaphoreW , DDOPT_SEMAPHORE , FALSE ) ;

    OpenSemaphoreW ( dwDesiredAccess ,
                     bInheritHandle  ,
                     lpName           ) ;
                     
    HOOKFN_SHUTDOWN ( 3 , DDOPT_SEMAPHORE ) ;
}

BOOL NAKEDDEF DD_ReleaseSemaphore ( HANDLE hSemaphore,
                                    LONG lReleaseCount,
                                    LPLONG lpPreviousCount)
{
    HOOKFN_STARTUP ( eReleaseSemaphore , DDOPT_SEMAPHORE , FALSE ) ;

    ReleaseSemaphore ( hSemaphore       ,
                       lReleaseCount    ,
                       lpPreviousCount   )  ;
                     
    HOOKFN_SHUTDOWN ( 3 , DDOPT_SEMAPHORE ) ;

}

// Turn optimizations back on.
#pragma optimize( "", on )

