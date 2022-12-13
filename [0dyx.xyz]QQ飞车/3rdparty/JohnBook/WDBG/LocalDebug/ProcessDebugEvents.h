/*----------------------------------------------------------------------
Debugging Applications for Microsoft .NET and Microsoft Windows
Copyright © 1997-2003 John Robbins -- All rights reserved.
----------------------------------------------------------------------*/

#ifndef _PROCESSDEBUGEVENTS_H
#define _PROCESSDEBUGEVENTS_H

/*----------------------------------------------------------------------
FUNCTION    :   ProcessDebugEvents
DISCUSSION  :
    The internal function that takes care of processing the debug
events before releasing them to ContinueDebugEvent.
PARAMETERS  :
    pUserClass - The main thread notification class.
    pData      - The debuggee information for this event.
    pDE        - The debug event to process.
RETURNS     :
    DBG_CONTINUE              - Continue debugging.
    DBG_EXCEPTION_NOT_HANDLED - The exception was not handled.
----------------------------------------------------------------------*/
DWORD ProcessDebugEvents ( CDebugBaseUser * pUserClass ,
                           LPDEBUGGEEINFO   pData      ,
                           LPDEBUG_EVENT    pDE         ) ;



#endif  // _PROCESSDEBUGEVENTS_H

