/*----------------------------------------------------------------------
Debugging Applications for Microsoft .NET and Microsoft Windows
Copyright © 1997-2003 John Robbins -- All rights reserved.
------------------------------------------------------------------------
The critical section functions.
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

VOID NAKEDDEF
     DD_InitializeCriticalSection(LPCRITICAL_SECTION lpCriticalSection)
{
    HOOKFN_STARTUP ( eInitializeCriticalSection ,
                     DDOPT_CRITSEC              ,
                     FALSE                       ) ;
    
    InitializeCriticalSection ( lpCriticalSection ) ;
    
    HOOKFN_SHUTDOWN ( 1 , DDOPT_CRITSEC ) ;
}

BOOL NAKEDDEF DD_InitializeCriticalSectionAndSpinCount (
                                   LPCRITICAL_SECTION lpCriticalSection,
                                   DWORD              dwSpinCount      )
{
    HOOKFN_STARTUP ( eInitializeCriticalSectionAndSpinCount ,
                     DDOPT_CRITSEC                          ,
                     FALSE                                   ) ;
                     
    InitializeCriticalSectionAndSpinCount ( lpCriticalSection ,
                                            dwSpinCount        ) ;
                                            
    HOOKFN_SHUTDOWN ( 2 , DDOPT_CRITSEC ) ;
}

VOID NAKEDDEF
       DD_DeleteCriticalSection ( LPCRITICAL_SECTION lpCriticalSection )
{
    HOOKFN_STARTUP ( eDeleteCriticalSection ,
                     DDOPT_CRITSEC          ,
                     FALSE                   ) ;

    DeleteCriticalSection ( lpCriticalSection ) ;
    
    HOOKFN_SHUTDOWN ( 1 , DDOPT_CRITSEC ) ;
}

VOID NAKEDDEF
          DD_EnterCriticalSection(LPCRITICAL_SECTION lpCriticalSection)
{
    HOOKFN_STARTUP ( eEnterCriticalSection ,
                     DDOPT_CRITSEC         ,
                     TRUE                   ) ;

    EnterCriticalSection ( lpCriticalSection ) ;
    
    HOOKFN_SHUTDOWN ( 1 , DDOPT_CRITSEC ) ;
}

VOID NAKEDDEF
           DD_LeaveCriticalSection(LPCRITICAL_SECTION lpCriticalSection)
{
    HOOKFN_STARTUP ( eLeaveCriticalSection  ,
                     DDOPT_CRITSEC          ,
                     FALSE                   ) ;
    
    LeaveCriticalSection ( lpCriticalSection ) ;
    
    HOOKFN_SHUTDOWN ( 1 , DDOPT_CRITSEC ) ;
}

DWORD NAKEDDEF
    DD_SetCriticalSectionSpinCount(LPCRITICAL_SECTION lpCriticalSection,
                                   DWORD dwSpinCount )
{
    HOOKFN_STARTUP ( eSetCriticalSectionSpinCount ,
                     DDOPT_CRITSEC                ,
                     FALSE                          ) ;

    SetCriticalSectionSpinCount ( lpCriticalSection , dwSpinCount ) ;
    
    HOOKFN_SHUTDOWN ( 2 , DDOPT_CRITSEC ) ;
}

typedef BOOL (WINAPI *PFNTRYENTERCRITSEC)
                              ( LPCRITICAL_SECTION lpCriticalSection ) ;

static PFNTRYENTERCRITSEC g_pfnTECS = NULL ;

BOOL NAKEDDEF
    DD_TryEnterCriticalSection ( LPCRITICAL_SECTION lpCriticalSection )
{
    HOOKFN_STARTUP ( eTryEnterCriticalSection   ,
                     DDOPT_CRITSEC              ,
                     FALSE                       ) ;
    
    TryEnterCriticalSection ( lpCriticalSection ) ;
    
    HOOKFN_SHUTDOWN ( 1 , DDOPT_CRITSEC ) ;
}

// Turn optimizations back on.
#pragma optimize( "", on )
