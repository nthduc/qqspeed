/*----------------------------------------------------------------------
Debugging Applications for Microsoft .NET and Microsoft Windows
Copyright © 1997-2003 John Robbins -- All rights reserved.
----------------------------------------------------------------------*/

#ifndef _CREATEDEBUGSYNCHEVENTS_H
#define _CREATEDEBUGSYNCHEVENTS_H

/*----------------------------------------------------------------------
FUNCTION    :   CreateDebugSyncEvents
DISCUSSION  :
    Creates the synchronization events needed to coordinate the UI and
the debug loop.
PARAMETERS  :
    lpDebugEvents - The array of handles.  This must be NUM_DEBUGEVENTS
                    items long.
    dwProcessID   - The unique PID to account for this debug thread
                    instance.  This must be the same value returned by
                    the StartDebugging function.
RETURNS     :
    TRUE  - The events were created.
    FALSE - There was a problem.
----------------------------------------------------------------------*/
BOOL CreateDebugSyncEvents ( LPHANDLE lpDebugSyncEvents ,
                             DWORD    dwProcessID        ) ;

#endif  //_CREATEDEBUGSYNCHEVENTS_H

