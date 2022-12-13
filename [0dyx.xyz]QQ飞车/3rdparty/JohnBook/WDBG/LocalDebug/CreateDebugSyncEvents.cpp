/*----------------------------------------------------------------------
Debugging Applications for Microsoft .NET and Microsoft Windows
Copyright © 1997-2003 John Robbins -- All rights reserved.
----------------------------------------------------------------------*/

#include "PCH.h"
#include "DebugInterface.h"
#include "Dll.h"
#include "Resource.h"


// This is the exception that we throw if we need to except out of a
// local function.
#define LOCAL_EXCEPTION    13


/*----------------------------------------------------------------------
FUNCTION    :   CreateManualResetEvent
DISCUSSION  :
   A helper function to create a manual reset event based on string
resource ID and the a string to append to that string.  The event
created can either be signaled or not signaled.
PARAMETERS  :
    uiRes      - The resource string to load to name the event.
    szID       - The ID string to append to the end of the loaded
                 resource string to make the event unique.
    bInitState - The initial state of the event.
RETURNS     :
    NULL  - There was a problem creating the event.
    !NULL - The event was created.
----------------------------------------------------------------------*/
static HANDLE CreateManualResetEvent ( UINT     uiRes      ,
                                       LPCTSTR  szID       ,
                                       BOOL     bInitState  )
{
    // The buffer used to load the resource strings into.
    TCHAR szBuff[ MAX_PATH ] ;
    // The handle we create.
    HANDLE hEvent ;

    if ( FALSE == LoadString ( GetDllHandle (  )  ,
                               uiRes              ,
                               szBuff             ,
                               sizeof ( szBuff )   ) )
    {
        return ( FALSE ) ;
    }
    _tcscat ( szBuff , szID ) ;
    if ( FALSE ==
            ( hEvent = CreateEvent ( NULL      , // Default security.
                                     TRUE      , // TRUE ==MANUAL RESET
                                     bInitState, // TRUE == SIGNALED
                                     szBuff    )))// Name.
    {
        return ( FALSE ) ;
    }
    return ( hEvent ) ;
}

BOOL CreateDebugSyncEvents ( LPHANDLE lpDebugSyncEvents ,
                             DWORD    dwProcessID        )
{
    ASSERT ( FALSE ==
                IsBadWritePtr ( lpDebugSyncEvents ,
                                sizeof ( HANDLE ) * NUM_DEBUGEVENTS ) );
    if ( TRUE == IsBadWritePtr ( lpDebugSyncEvents ,
                                sizeof ( HANDLE ) * NUM_DEBUGEVENTS ) )
    {
        SetLastError ( ERROR_INVALID_PARAMETER ) ;
        return ( FALSE ) ;
    }
    // Holds the string of the process ID.
    TCHAR szID[ 15 ] ;

    // Convert the ID into a string that we can append to all event
    // names that we need to create.
    _ltot ( dwProcessID , szID , 10 ) ;

    // Always NULL out the event handle memory.
    memset ( lpDebugSyncEvents ,
             NULL              ,
             sizeof ( HANDLE ) * NUM_DEBUGEVENTS ) ;

    try
    {
        // Create the ACTIVEDEBUGGER event with an initial state as
        // SIGNALED.
        if ( FALSE == ( lpDebugSyncEvents[ ACTIVEDEBUGGER ] =
                              CreateManualResetEvent ( IDS_DBGACTIVE ,
                                                       szID          ,
                                                       TRUE          )))
        {
            throw LOCAL_EXCEPTION ;
        }
        // Create the CLOSEDEBUGGER event with an initial state as
        // NOT SIGNALED.
        if ( FALSE == ( lpDebugSyncEvents[ CLOSEDEBUGGER ] =
                              CreateManualResetEvent ( IDS_DBGCLOSING ,
                                                       szID           ,
                                                       FALSE         )))
        {
            throw LOCAL_EXCEPTION ;
        }
        // Create the SUSPENDDEBUGGER event with an initial state as
        // NOT SIGNALED.
        if ( FALSE == ( lpDebugSyncEvents[ SUSPENDDEBUGGER ] =
                              CreateManualResetEvent ( IDS_DBGSUSPEND ,
                                                       szID           ,
                                                       FALSE         )))
        {
            throw LOCAL_EXCEPTION ;
        }
        // Create the RESUMEDEBUGGER event with an initial state as
        // NOT SIGNALED.
        if ( FALSE == ( lpDebugSyncEvents[ RESUMEDEBUGGER ] =
                              CreateManualResetEvent ( IDS_DBGRESUME ,
                                                       szID          ,
                                                       FALSE         )))
        {
            throw LOCAL_EXCEPTION ;
        }
   }
   catch ( int iReason )
   {
        TRACE ( "EXCEPTION in CreateDebugEvents!\n" , iReason ) ;
        // Only catch LOCAL_EXCEPTIONS...
        if ( LOCAL_EXCEPTION == iReason )
        {
            TRACE ( "Cleaning up from a local exception\n" ) ;
            if ( lpDebugSyncEvents[ ACTIVEDEBUGGER ] )
            {
                CloseHandle ( lpDebugSyncEvents[ ACTIVEDEBUGGER ] ) ;
            }
            if ( lpDebugSyncEvents[ CLOSEDEBUGGER ] )
            {
                CloseHandle ( lpDebugSyncEvents[ CLOSEDEBUGGER ] ) ;
            }
            if ( lpDebugSyncEvents[ SUSPENDDEBUGGER ] )
            {
                CloseHandle ( lpDebugSyncEvents[ SUSPENDDEBUGGER ] ) ;
            }
            if ( lpDebugSyncEvents[ RESUMEDEBUGGER ] )
            {
                CloseHandle ( lpDebugSyncEvents[ RESUMEDEBUGGER ] ) ;
            }
        }
        else
        {
            TRACE ( "Unknown exception\n" ) ;
            throw ;
        }
        return ( FALSE ) ;
    }
    return ( TRUE ) ;

}
