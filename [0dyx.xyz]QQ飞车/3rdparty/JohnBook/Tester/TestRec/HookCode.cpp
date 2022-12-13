/*----------------------------------------------------------------------
Debugging Applications for Microsoft .NET and Microsoft Windows
Copyright © 1997-2003 John Robbins -- All rights reserved.
----------------------------------------------------------------------*/
#include "stdafx.h"
#include "RecordingEngine.h"
#include "HookCode.h"

/*//////////////////////////////////////////////////////////////////////
// File Scope Globals
//////////////////////////////////////////////////////////////////////*/
// The journal recording hook value.
static HHOOK g_hRecHook = NULL ;
// The get message hook value.
static HHOOK g_hMsgHook = NULL ;
// The pointer to the recording engine the recording hook calls.
static CRecordingEngine * g_pRecEngine = NULL ;

/*//////////////////////////////////////////////////////////////////////
// File Scope Prototypes
//////////////////////////////////////////////////////////////////////*/
// The journal record hook.
LRESULT CALLBACK JournalRecordFunc ( int    nCode  ,
                                     WPARAM wParam ,
                                     LPARAM lParam  ) ;
// The GetMessage hook to monitor for WM_CANCELJOURNAL messages.
LRESULT CALLBACK GetMsgFunc ( int    nCode  ,
                              WPARAM wParam ,
                              LPARAM lParam  ) ;

/*//////////////////////////////////////////////////////////////////////
// Code Starts Here
//////////////////////////////////////////////////////////////////////*/

BOOL StartRecordingHook ( CRecordingEngine * pRecEngine )
{
    // Check the parameter.
    ASSERT ( FALSE == IsBadReadPtr ( pRecEngine ,
                                     sizeof ( CRecordingEngine * ) ) ) ;
    if ( TRUE == IsBadReadPtr ( pRecEngine ,
                                sizeof ( CRecordingEngine * ) ) )
    {
        return ( FALSE ) ;
    }

    TRACE ( "StartRecordingHook called.\n" ) ;

    // Save off the recording engine.
    g_pRecEngine = pRecEngine ;

    // Set the journal record hook.
    g_hRecHook = SetWindowsHookEx ( WH_JOURNALRECORD         ,
                                    JournalRecordFunc        ,
                                    GetModuleHandle ( NULL ) ,
                                    0                         ) ;
    if ( NULL == g_hRecHook )
    {
        ASSERT ( !"Setting record hook failed!" ) ;
        g_pRecEngine = NULL ;
        return ( FALSE ) ;
    }

    // Set the get message hook so I can see the WM_CANCELJOURNAL
    // message for the thread queue that started the recording hook.
    g_hMsgHook = SetWindowsHookEx ( WH_GETMESSAGE            ,
                                    GetMsgFunc               ,
                                    GetModuleHandle ( NULL ) ,
                                    GetCurrentThreadId ( )    ) ;
    if ( NULL == g_hMsgHook )
    {
        ASSERT ( !"Setting GetMessage hook failed!" ) ;
        g_pRecEngine = NULL ;
        VERIFY ( UnhookWindowsHookEx ( g_hRecHook ) ) ;
        g_hRecHook = NULL ;
        return ( FALSE ) ;
    }

    // All OK, Jumpmaster!
    return ( TRUE ) ;
}

BOOL StopRecordingHook ( void )
{
    // Was this function called when no hooks were set?
    if ( NULL == g_pRecEngine )
    {
        return ( FALSE ) ;
    }

    TRACE ( "StopRecordingHook called.\n" ) ;

    // Check before unhooking the journal record hook.  Since
    // CTRL+ALT+DEL will remove the hook automatically, my GetMessage
    // hook will set the record hook value to NULL so I don't call
    // UnhookWindowsHookEx on an invalid hook handle.
    if ( NULL != g_hRecHook )
    {
        VERIFY ( UnhookWindowsHookEx ( g_hRecHook ) ) ;
    }
    // The GetMessage hook is always there so remove it now.
    VERIFY ( UnhookWindowsHookEx ( g_hMsgHook ) ) ;

    // Tell the recording engine.
    g_pRecEngine->AbortRecording ( ) ;


    // Reinitialize all the module globals.
    g_hRecHook = NULL ;
    g_hMsgHook = NULL ;
    g_pRecEngine = NULL ;

    // Done!
    return ( TRUE ) ;
}


LRESULT CALLBACK JournalRecordFunc ( int    nCode  ,
                                     WPARAM wParam ,
                                     LPARAM lParam  )
{
    LRESULT lRet = CallNextHookEx ( g_hRecHook , nCode , wParam , lParam ) ;
    // Only diddle if the code is greater than or equal to zero.
    if ( nCode >= 0 )
    {
        // Just call the recording engine and let it do all the work.
        g_pRecEngine->ProcessJournalEvent ( nCode , (PEVENTMSG)lParam );
    }
    return ( lRet ) ;
}

LRESULT CALLBACK GetMsgFunc ( int    nCode  ,
                              WPARAM wParam ,
                              LPARAM lParam  )
{
    // Only diddle if the code is greater than or equal to zero.
    if ( nCode >= 0 )
    {
        // The only message I care about is WM_CANCELJOURNAL.
        PMSG pMsg = (PMSG)lParam ;
        if ( WM_CANCELJOURNAL == pMsg->message )
        {
            TRACE ( "WM_CANCELJOURNAL message came through!\n" ) ;
            // Zero out the hook value since I just learned it was
            // blown away by the OS.
            g_hRecHook = NULL ;

            // Tell the recording engine to abort processing.
            g_pRecEngine->AbortRecording ( ) ;
        }
    }
    return ( CallNextHookEx ( g_hMsgHook , nCode , wParam , lParam ) ) ;
}

