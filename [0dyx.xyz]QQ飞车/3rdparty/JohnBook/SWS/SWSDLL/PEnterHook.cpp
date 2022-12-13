/*----------------------------------------------------------------------
Debugging Applications for Microsoft .NET and Microsoft Windows
Copyright © 1997-2003 John Robbins -- All rights reserved.
----------------------------------------------------------------------*/
/*//////////////////////////////////////////////////////////////////////
                              The Includes
//////////////////////////////////////////////////////////////////////*/
#include "stdafx.h"
#include "SWSDLL.h"
#include "SymbolEngine.h"
#include "VerboseMacros.h"
#include "ModuleItemArray.h"

/*//////////////////////////////////////////////////////////////////////
                     Defines, Constants, & Typedefs
//////////////////////////////////////////////////////////////////////*/
// The event handle the _penter function checks to see if the processing
// is disabled.
static HANDLE g_hStartStopEvent = NULL ;
// The simple automatic class I used to ensure various things are
// initialized.
class CAutoMatic
{
public  :
    CAutoMatic ( void )
    {
        g_hStartStopEvent = ::CreateEvent ( NULL                ,
                                            TRUE                ,
                                            FALSE               ,
                                            SWS_STOPSTART_EVENT  ) ;
        ASSERT ( NULL != g_hStartStopEvent ) ;
    }
    ~CAutoMatic ( void )
    {
        VERIFY ( ::CloseHandle ( g_hStartStopEvent ) ) ;
    }
} ;

/*//////////////////////////////////////////////////////////////////////
                           File Scope Globals
//////////////////////////////////////////////////////////////////////*/
// The automatic class.
static CAutoMatic g_cAuto ;

// The module array.
static CModuleItemArray g_cModArray ;


/*//////////////////////////////////////////////////////////////////////
                          Function Prototypes
//////////////////////////////////////////////////////////////////////*/

/*//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////*/

extern "C" void SWSDLL_DLLINTERFACE __declspec(naked) _penter ( void )
{
    DWORD_PTR dwCallerFunc ;

    // The function prolog.
    __asm
    {
        PUSH EAX                    // Save off EAX as I need to use it
                                    // before saving all registers.

        PUSH EBP                    // Set up the standard stack frame.
        MOV  EBP , ESP

        MOV  EAX , ESP              // Get the current stack value into
                                    //  EAX.

        SUB  ESP , __LOCAL_SIZE     // Save off the space needed by the
                                    // local variables.

        PUSHAD                      // Save off all general register
                                    // values.

        // Now I can calculate the return address.
        ADD  EAX , 04h + 04h        // Account for the PUSH EBP and the
                                    // PUSH EAX.
        MOV  EAX , [EAX]            // Grab the return address.
        SUB  EAX , 5                // Take off the 5 byte jump used to
                                    // call _penter to get to the start
                                    // of the function.
        MOV  [dwCallerFunc] , EAX   // Save the caller function.

    }

    // If the start/stop event is signaled, don't do any processing.
    if ( WAIT_TIMEOUT == WaitForSingleObject ( g_hStartStopEvent , 0 ))
    {
        // Do the work.
        g_cModArray.IncrementFunctionEntry ( dwCallerFunc ) ;
    }
    // The function epilog.
    __asm
    {
        POPAD                       // Restore all general purpose
                                    // values.

        ADD ESP , __LOCAL_SIZE      // Remove space needed for locals.

        MOV ESP , EBP               // Restore the standard stack frame.
        POP EBP

        POP EAX                     // Restore EAX

        RET                         // Return to caller.
    }
}
