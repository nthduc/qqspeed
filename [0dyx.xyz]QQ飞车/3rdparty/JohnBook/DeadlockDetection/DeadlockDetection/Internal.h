/*----------------------------------------------------------------------
Debugging Applications for Microsoft .NET and Microsoft Windows
Copyright © 1997-2003 John Robbins -- All rights reserved.
------------------------------------------------------------------------
The internal functions and declarations used across modules by the
Deadlock Detection system.
----------------------------------------------------------------------*/

#ifndef _INTERNAL_H
#define _INTERNAL_H

// The other internal only headers.
#include "ShowError.h"
#include "DD_Funcs.h"

/*----------------------------------------------------------------------
FUNCTION        : HookAllLoadedModules
DISCUSSION      :
    Hooks all the loaded modules.  This function does the right thing
and only hooks those modules that have not already been hooked.
PARAMETERS      :
    None.
RETURNS         :
    TRUE  - Life is good.
    FALSE - There was a problem.
----------------------------------------------------------------------*/
BOOL HookAllLoadedModules ( void ) ;

/*----------------------------------------------------------------------
FUNCTION        : ResetModuleListAfterModuleUnload
DISCUSSION      :
    Resets the list of loaded modules after a call to FreeLibrary or
    FreeLibraryAndExitThread.
PARAMETERS      :
    None.
RETURNS         :
    None.
----------------------------------------------------------------------*/
void ResetModuleListAfterModuleUnload ( void ) ;

/*----------------------------------------------------------------------
FUNCTION        : DoLogging
DISCUSSION      :
    Returns TRUE if logging for the particular option is enabled.  If
the whole system is suspended, FALSE is returned.
PARAMETERS      :
    dwItem - The DDOPT_* flag to check on.
RETURNS         :
    TRUE  - Log away.
    FALSE - Logging is off for this option.
----------------------------------------------------------------------*/
BOOL DoLogging ( DWORD dwItem ) ;

/*----------------------------------------------------------------------
FUNCTION        : IsSuspended
DISCUSSION      :
    Returns TRUE if DeadlockDetection is suspended.
PARAMETERS      :
    None.
RETURNS         :
    TRUE  - It is suspended.
    FALSE - Nope, detect away.
----------------------------------------------------------------------*/
BOOL IsSuspended ( void ) ;

/*----------------------------------------------------------------------
FUNCTION        : ProcessEvent
DISCUSSION      :
    Posts the event to the extension DLL.  This function is where the
lone critical section is handled.
PARAMETERS      :
    lpvtInfo - The event information.
RETURNS         :
    None.
----------------------------------------------------------------------*/
void ProcessEvent ( LPDDEVENTINFO lpvtInfo ) ;

#endif  // _INTERNAL_H

