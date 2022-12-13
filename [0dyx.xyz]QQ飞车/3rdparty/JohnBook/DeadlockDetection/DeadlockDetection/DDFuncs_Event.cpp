/*----------------------------------------------------------------------
Debugging Applications for Microsoft .NET and Microsoft Windows
Copyright © 1997-2003 John Robbins -- All rights reserved.
------------------------------------------------------------------------
The event functions.
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

HANDLE NAKEDDEF DD_CreateEventA(LPSECURITY_ATTRIBUTES lpEventAttributes,
                                BOOL                  bManualReset     ,
                                BOOL                  bInitialState    ,
                                LPCSTR                lpName           )
{
    HOOKFN_STARTUP ( eCreateEventA , DDOPT_EVENT , FALSE ) ;
    
    CreateEventA ( lpEventAttributes ,
                    bManualReset      ,
                    bInitialState     ,
                    lpName             ) ;
                    
    HOOKFN_SHUTDOWN ( 4 , DDOPT_EVENT ) ;
}

HANDLE NAKEDDEF DD_CreateEventW(LPSECURITY_ATTRIBUTES lpEventAttributes,
                                  BOOL                bManualReset     ,
                                  BOOL                bInitialState    ,
                                  LPCWSTR             lpName           )
{
    HOOKFN_STARTUP ( eCreateEventW , DDOPT_EVENT , FALSE ) ;
    
    CreateEventW ( lpEventAttributes ,
                    bManualReset      ,
                    bInitialState     ,
                    lpName             ) ;
                    
    HOOKFN_SHUTDOWN ( 4 , DDOPT_EVENT ) ;
}

HANDLE NAKEDDEF DD_OpenEventA ( DWORD  dwDesiredAccess ,
                                BOOL   bInheritHandle  ,
                                LPCSTR lpName           )
{
    HOOKFN_STARTUP ( eOpenEventA , DDOPT_EVENT , FALSE ) ;
    
    OpenEventA ( dwDesiredAccess ,
                 bInheritHandle  ,
                 lpName           ) ;
                    
    HOOKFN_SHUTDOWN ( 3 , DDOPT_EVENT ) ;
}

HANDLE NAKEDDEF DD_OpenEventW ( DWORD   dwDesiredAccess ,
                                BOOL    bInheritHandle  ,
                                LPCWSTR lpName           )
{
    HOOKFN_STARTUP ( eOpenEventW , DDOPT_EVENT , FALSE ) ;
    
    OpenEventW ( dwDesiredAccess ,
                 bInheritHandle  ,
                 lpName           ) ;
                    
    HOOKFN_SHUTDOWN ( 3 , DDOPT_EVENT ) ;
}

BOOL NAKEDDEF DD_PulseEvent ( HANDLE hEvent )
{
    HOOKFN_STARTUP ( ePulseEvent , DDOPT_EVENT , FALSE ) ;
    
    PulseEvent ( hEvent ) ;
    
    HOOKFN_SHUTDOWN ( 1 , DDOPT_EVENT ) ;
}

BOOL NAKEDDEF DD_ResetEvent ( HANDLE hEvent )
{
    HOOKFN_STARTUP ( eResetEvent , DDOPT_EVENT , FALSE ) ;
    
    ResetEvent ( hEvent ) ;
    
    HOOKFN_SHUTDOWN ( 1 , DDOPT_EVENT ) ;
}

BOOL NAKEDDEF DD_SetEvent ( HANDLE hEvent )
{
    HOOKFN_STARTUP ( eSetEvent , DDOPT_EVENT , FALSE ) ;
    
    SetEvent ( hEvent ) ;
    
    HOOKFN_SHUTDOWN ( 1 , DDOPT_EVENT ) ;
}

// Turn optimizations back on.
#pragma optimize( "", on )

