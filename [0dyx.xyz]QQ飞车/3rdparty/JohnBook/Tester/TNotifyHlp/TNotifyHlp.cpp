/*----------------------------------------------------------------------
Debugging Applications for Microsoft .NET and Microsoft Windows
Copyright © 1997-2003 John Robbins -- All rights reserved.
----------------------------------------------------------------------*/
#include "stdafx.h"

/*//////////////////////////////////////////////////////////////////////
                    File Scope Defines and Constants
//////////////////////////////////////////////////////////////////////*/
// The maximum number of notification slots
static const int TOTAL_NOTIFY_SLOTS = 5 ;
// The mutex name
static const LPCTSTR k_MUTEX_NAME  = _T ( "TNotifyHlp_Mutex" ) ;
// The longest amount of time I'll wait on the mutex
static const int k_WAITLIMIT = 5000 ;

// I have my own trace here because I don't want to drag
// BugslayerUtil.DLL into each address space.
#ifdef _DEBUG
#define TRACE   ::OutputDebugString
#else
#define TRACE   __noop
#endif

/*//////////////////////////////////////////////////////////////////////
// File Scope Typedefs
//////////////////////////////////////////////////////////////////////*/
// The structure for an individual window to look for
typedef struct tag_TNOTIFYITEM
{
    // The PID for the process that created this item
    DWORD   dwOwnerPID  ;
    // The notification type
    int     iNotifyType ;
    // The search parameter
    int     iSearchType ;
    // The handle to the HWND being created
    HWND    hWndCreate  ;
    // The destroy Boolean
    BOOL    bDestroy    ;
    // The title string
    TCHAR   szTitle [ MAX_PATH ] ;
} TNOTIFYITEM , * PTNOTIFYITEM ;

/*//////////////////////////////////////////////////////////////////////
// File Scope Global Variables
//////////////////////////////////////////////////////////////////////*/
// This data is **NOT** shared across processes, so each process gets
// its own copy.

// The HINSTANCE for this module.  Setting global system hooks requires
// a DLL.
static HINSTANCE g_hInst = NULL ;

// The mutex that protects the g_NotifyData table
static HANDLE g_hMutex = NULL ;

// The hook handle. I don't keep this handle in the shared section because
// multiple instances could set the hook when running multiple scripts.
static HHOOK g_hHook = NULL ;

// The number of items added by this process. This number lets me know
// how to handle the hook.
static int  g_iThisProcessItems = 0 ;

/*//////////////////////////////////////////////////////////////////////
// File Scope Function Prototypes
//////////////////////////////////////////////////////////////////////*/
// Our happy hook
LRESULT CALLBACK CallWndRetProcHook ( int    nCode  ,
                                      WPARAM wParam ,
                                      LPARAM lParam  ) ;

// The internal check function
static LONG_PTR __stdcall CheckNotifyItem ( HANDLE hItem , BOOL bCreate ) ;

/*//////////////////////////////////////////////////////////////////////
// Funky Shared Data Across All Hook Instances
//////////////////////////////////////////////////////////////////////*/
#pragma data_seg ( ".HOOKDATA" )
// The notification items table
static TNOTIFYITEM g_shared_NotifyData [ TOTAL_NOTIFY_SLOTS ] =
    {
        { 0 , 0 , 0 , NULL , 0 , '\0' } ,
        { 0 , 0 , 0 , NULL , 0 , '\0' } ,
        { 0 , 0 , 0 , NULL , 0 , '\0' } ,
        { 0 , 0 , 0 , NULL , 0 , '\0' } ,
        { 0 , 0 , 0 , NULL , 0 , '\0' }
    } ;
// The master count
static int g_shared_iUsedSlots = 0 ;
#pragma data_seg ( )

/*//////////////////////////////////////////////////////////////////////
// EXTERNAL IMPLEMENTATION STARTS HERE
//////////////////////////////////////////////////////////////////////*/

extern "C" BOOL WINAPI DllMain ( HINSTANCE hInst       ,
                                 DWORD     dwReason    ,
                                 LPVOID    /*lpReserved*/ )
{
#ifdef _DEBUG
    BOOL bCHRet ;
#endif

    BOOL bRet = TRUE ;
    switch ( dwReason )
    {
        case DLL_PROCESS_ATTACH :
            // Set the global module instance.
            g_hInst = hInst ;
            // I don't need the thread notifications.
            DisableThreadLibraryCalls ( g_hInst ) ;
            // Create the mutex for this process.  The mutex is created
            // here but isn't owned yet.
            g_hMutex = CreateMutex ( NULL , FALSE , k_MUTEX_NAME ) ;
            if ( NULL == g_hMutex )
            {
                TRACE ( _T ( "Unable to create the mutex!\n" ) ) ;
                // If I can't create the mutex, I can't continue, so
                // fail the DLL load.
                bRet = FALSE ;
            }
            break ;
        case DLL_PROCESS_DETACH :

            // Check to see whether this process has any items in the
            // notification array. If it does, remove them to avoid
            // leaving orphaned items.
            if ( 0 != g_iThisProcessItems )
            {
                DWORD dwProcID = GetCurrentProcessId ( ) ;
                // I don't need to grab the mutex here because only a
                // single thread will ever call with the
                // DLL_PROCESS_DETACH reason.

                // Loop through and take a gander.
                for ( INT_PTR i = 0 ; i < TOTAL_NOTIFY_SLOTS ; i++ )
                {
                    if ( g_shared_NotifyData[i].dwOwnerPID == dwProcID )
                    {
#ifdef _DEBUG
                        TCHAR szBuff[ 50 ] ;
                        wsprintf ( szBuff ,
                              _T("DLL_PROCESS_DETACH removing : #%d\n"),
                                   i ) ;
                        TRACE ( szBuff ) ;
#endif
                        // Get rid of it.
                        RemoveNotifyTitle ( (HANDLE)i ) ;
                    }
                }
            }

            // Close the mutex handle.
#ifdef _DEBUG
            bCHRet =
#endif
            CloseHandle ( g_hMutex ) ;
#ifdef _DEBUG
            if ( FALSE == bCHRet )
            {
                TRACE ( _T ( "!!!!!!!!!!!!!!!!!!!!!!!!\n" ) ) ;
                TRACE ( _T ( "CloseHandle(g_hMutex) " ) \
                        _T ( "failed!!!!!!!!!!!!!!!!!!\n" ) ) ;
                TRACE ( _T ( "!!!!!!!!!!!!!!!!!!!!!!!!\n" ) ) ;
            }
#endif
            break ;
        default                 :
            break ;
    }
    return ( bRet ) ;
}


HANDLE TNOTIFYHLP_DLLINTERFACE __stdcall
    AddNotifyTitle ( int     iNotifyType ,
                     int     iSearchType ,
                     LPCTSTR szString     )
{
    // Ensure that the notify type range is correct.
    if ( ( iNotifyType < ANTN_DESTROYWINDOW     ) ||
         ( iNotifyType > ANTN_CREATEANDDESTROY  )   )
    {
        TRACE ( _T( "AddNotify Title : iNotifyType is out of range!\n" ) ) ;
        return ( INVALID_HANDLE_VALUE ) ;
    }
    // Ensure that the search type range is correct.
    if ( ( iSearchType < ANTS_EXACTMATCH  ) ||
         ( iSearchType > ANTS_ANYLOCMATCH )   )
    {
        TRACE ( _T( "AddNotify Title : iSearchType is out of range!\n" ) ) ;
        return ( INVALID_HANDLE_VALUE ) ;
    }
    // Ensure that the string is valid.
    if ( TRUE == IsBadStringPtr ( szString , MAX_PATH ) )
    {
        TRACE ( _T( "AddNotify Title : szString is invalid!\n" ) ) ;
        return ( INVALID_HANDLE_VALUE ) ;
    }

    // Wait to acquire the mutex.
    DWORD dwRet = WaitForSingleObject ( g_hMutex , k_WAITLIMIT ) ;
    if ( WAIT_TIMEOUT == dwRet )
    {
        TRACE (_T( "AddNotifyTitle : Wait on mutex timed out!!\n"));
        return ( INVALID_HANDLE_VALUE ) ;
    }

    // If the slots are used up, abort now.
    if ( TOTAL_NOTIFY_SLOTS == g_shared_iUsedSlots )
    {
        ReleaseMutex ( g_hMutex ) ;
        return ( INVALID_HANDLE_VALUE ) ;
    }

    // Find the first free slot.
    for ( INT_PTR i = 0 ; i < TOTAL_NOTIFY_SLOTS ; i++ )
    {
        if ( _T ( '\0' ) == g_shared_NotifyData[ i ].szTitle[ 0 ] )
        {
            break ;
        }
    }

    // Add this data.
    g_shared_NotifyData[ i ].dwOwnerPID  = GetCurrentProcessId ( ) ;
    g_shared_NotifyData[ i ].iNotifyType = iNotifyType ;
    g_shared_NotifyData[ i ].iSearchType = iSearchType ;
    lstrcpy ( g_shared_NotifyData[ i ].szTitle , szString ) ;

    // Bump up the master count.
    g_shared_iUsedSlots++ ;

    // Bump up the count for this process.
    g_iThisProcessItems++ ;

    TRACE ( _T( "AddNotifyTitle - Added a new item!\n" ) ) ;

    ReleaseMutex ( g_hMutex ) ;

    // If this is the first notification request, enable the hook.
    if ( NULL == g_hHook )
    {
        g_hHook = SetWindowsHookEx ( WH_CALLWNDPROCRET  ,
                                     CallWndRetProcHook ,
                                     g_hInst            ,
                                     0                   ) ;
#ifdef _DEBUG
        if ( NULL == g_hHook )
        {
            TCHAR szBuff[ 50 ] ;
            wsprintf ( szBuff ,
                       _T ( "SetWindowsHookEx failed!!!! (0x%08X)\n" ),
                       GetLastError ( ) ) ;
            TRACE ( szBuff ) ;
        }
#endif
    }

    return ( (HANDLE)i ) ;

}

void TNOTIFYHLP_DLLINTERFACE __stdcall
    RemoveNotifyTitle ( HANDLE hItem )
{
    // Check the value.
    INT_PTR i = (INT_PTR)hItem ;
    if ( ( i < 0 ) || ( i > TOTAL_NOTIFY_SLOTS ) )
    {
        TRACE ( _T ( "RemoveNotifyTitle : Invalid handle!\n" ) ) ;
        return ;
    }

    // Get the mutex.
    DWORD dwRet = WaitForSingleObject ( g_hMutex , k_WAITLIMIT ) ;
    if ( WAIT_TIMEOUT == dwRet )
    {
        TRACE ( _T ( "RemoveNotifyTitle : Wait on mutex timed out!\n"));
        return ;
    }

    if ( 0 == g_shared_iUsedSlots )
    {
        TRACE ( _T ( "RemoveNotifyTitle : Attempting to remove when " )\
                _T ( "no notification handles are set!\n" ) ) ;
        ReleaseMutex ( g_hMutex ) ;
        return ;
    }

    // Before removing anything, make sure this index points to a
    // NotifyData entry that contains a valid value. If I
    // didn't check, you could call this function with the same value
    // over and over, which would mess up the used-slots counts.
    if ( 0 == g_shared_NotifyData[ i ].dwOwnerPID )
    {
        TRACE ( _T ( "RemoveNotifyTitle : ") \
                _T ( "Attempting to double remove!\n" ) ) ;
        ReleaseMutex ( g_hMutex ) ;
        return ;
    }

    // Remove this item from the array.
    g_shared_NotifyData[ i ].dwOwnerPID   = 0 ;
    g_shared_NotifyData[ i ].iNotifyType  = 0 ;
    g_shared_NotifyData[ i ].hWndCreate   = NULL ;
    g_shared_NotifyData[ i ].bDestroy     = FALSE ;
    g_shared_NotifyData[ i ].iSearchType  = 0 ;
    g_shared_NotifyData[ i ].szTitle[ 0 ] = _T ( '\0' ) ;

    // Bump down the master item count.
    g_shared_iUsedSlots-- ;

    // Bump down this process's item count.
    g_iThisProcessItems-- ;

    TRACE ( _T ( "RemoveNotifyTitle - Removed an item!\n" ) ) ;

    ReleaseMutex ( g_hMutex ) ;

    // If this is the last item for this process, unhook this process's
    // hook.
    if ( ( 0 == g_iThisProcessItems ) && ( NULL != g_hHook ) )
    {
        if ( FALSE == UnhookWindowsHookEx ( g_hHook ) )
        {
            TRACE ( _T ( "UnhookWindowsHookEx failed!\n" ) ) ;
        }
        g_hHook = NULL ;
    }

}

HWND TNOTIFYHLP_DLLINTERFACE __stdcall
    CheckNotifyCreateTitle ( HANDLE hItem )
{
    return ( (HWND)CheckNotifyItem ( hItem , TRUE ) ) ;
}

BOOL TNOTIFYHLP_DLLINTERFACE __stdcall
    CheckNotifyDestroyTitle ( HANDLE hItem )
{
    return ( (BOOL)CheckNotifyItem ( hItem , FALSE ) ) ;
}

/*//////////////////////////////////////////////////////////////////////
// INTERNAL IMPLEMENTATION STARTS HERE
//////////////////////////////////////////////////////////////////////*/

static LONG_PTR __stdcall CheckNotifyItem ( HANDLE hItem , BOOL bCreate )
{
    // Check the value.
    INT_PTR i = (INT_PTR)hItem ;
    if ( ( i < 0 ) || ( i > TOTAL_NOTIFY_SLOTS ) )
    {
        TRACE ( _T ( "CheckNotifyItem : Invalid handle!\n" ) ) ;
        return ( NULL ) ;
    }

    LONG_PTR lRet = 0 ;

    // Get the mutex.
    DWORD dwRet = WaitForSingleObject ( g_hMutex , k_WAITLIMIT ) ;
    if ( WAIT_TIMEOUT == dwRet )
    {
        TRACE ( _T ( "CheckNotifyItem : Wait on mutex timed out!\n" ) );
        return ( NULL ) ;
    }

    // If all slots are empty, there's nothing to do.
    if ( 0 == g_shared_iUsedSlots )
    {
        ReleaseMutex ( g_hMutex ) ;
        return ( NULL ) ;
    }

    // Check the item requested.
    if ( TRUE == bCreate )
    {
        // If the HWND value isn't NULL, return that value and NULL it
        // out in the table.
        if ( NULL != g_shared_NotifyData[ i ].hWndCreate )
        {
            lRet = (LONG_PTR)g_shared_NotifyData[ i ].hWndCreate ;
            g_shared_NotifyData[ i ].hWndCreate = NULL ;
        }
    }
    else
    {
        if ( FALSE != g_shared_NotifyData[ i ].bDestroy )
        {
            lRet = TRUE ;
            g_shared_NotifyData[ i ].bDestroy = FALSE ;
        }
    }

    ReleaseMutex ( g_hMutex ) ;

    return ( lRet ) ;
}

static void __stdcall CheckTableMatch ( int     iNotifyType ,
                                        HWND    hWnd        ,
                                        LPCTSTR szTitle      )
{
    // Grab the mutex.
    DWORD dwRet = WaitForSingleObject ( g_hMutex , k_WAITLIMIT ) ;
    if ( WAIT_TIMEOUT == dwRet )
    {
        TRACE ( _T ( "CheckTableMatch : Wait on mutex timed out!\n" ) );
        return ;
    }

    // The table shouldn't be empty, but never assume anything.
    if ( 0 == g_shared_iUsedSlots )
    {
        ReleaseMutex ( g_hMutex ) ;
        TRACE ( _T ( "CheckTableMatch called on an empty table!\n" ) ) ;
        return ;
    }


    // Search through the table.
    for ( int i = 0 ; i < TOTAL_NOTIFY_SLOTS ; i++ )
    {
        // Does this entry have something in it, and does the type of
        // notification match?
        if ( ( _T ( '\0' ) != g_shared_NotifyData[ i ].szTitle[ 0 ] ) &&
             ( g_shared_NotifyData[ i ].iNotifyType & iNotifyType   )  )
        {
            BOOL bMatch = FALSE ;
            // Perform the match.
            switch ( g_shared_NotifyData[ i ].iSearchType )
            {
                case ANTS_EXACTMATCH    :
                    // This is simple.
                    if ( 0 == lstrcmp ( g_shared_NotifyData[i].szTitle ,
                                        szTitle                      ) )
                    {
                        bMatch = TRUE ;
                    }
                    break ;
                case ANTS_BEGINMATCH    :
                    if ( 0 ==
                            _tcsnccmp ( g_shared_NotifyData[i].szTitle ,
                                        szTitle                        ,
                               _tcslen(g_shared_NotifyData[i].szTitle)))
                    {
                        bMatch = TRUE ;
                    }
                    break ;
                case ANTS_ANYLOCMATCH   :
                    if ( NULL != _tcsstr ( szTitle                    ,
                                       g_shared_NotifyData[i].szTitle ))
                    {
                        bMatch = TRUE ;
                    }
                    break ;
                default                 :
                    TRACE ( _T ( "CheckTableMatch invalid " ) \
                            _T ( "search type!!!\n" ) ) ;
                    ReleaseMutex ( g_hMutex ) ;
                    return ;
                    break ;
            }
            // Tell them, Johnny. Do we have a match?
            if ( TRUE == bMatch )
            {
                // If this is a destroy notification, stick "1" in the
                // table.
                if ( ANTN_DESTROYWINDOW == iNotifyType )
                {
                    g_shared_NotifyData[ i ].bDestroy = TRUE ;
                }
                else
                {
                    // Otherwise, stick the HWND in the table.
                    g_shared_NotifyData[ i ].hWndCreate = hWnd ;
                }
            }
        }
    }
    ReleaseMutex ( g_hMutex ) ;
}

LRESULT CALLBACK CallWndRetProcHook ( int    nCode  ,
                                      WPARAM wParam ,
                                      LPARAM lParam  )
{
    // Buffer for storing the window title
    TCHAR szBuff[ MAX_PATH ] ;

    // Always pass the message to the next hook before I do any
    // processing. This way I don't forget and I can do my processing
    // in peace.
    LRESULT lRet = CallNextHookEx ( g_hHook , nCode , wParam , lParam );

    // The docs say never to mess around with a negative code, so I
    // don't.
    if ( nCode < 0 )
    {
        return ( lRet ) ;
    }

    // Get the message structure. Why are there three (or more)
    // different message structures?  What's wrong with consistently
    // using the stock ol' MSG for all message/proc hooks?
    PCWPRETSTRUCT pMsg = (PCWPRETSTRUCT)lParam ;

    // No caption, no work to do
    LONG lStyle = GetWindowLong ( pMsg->hwnd , GWL_STYLE ) ;
    if ( WS_CAPTION != ( lStyle & WS_CAPTION ) )
    {
        return ( lRet ) ;
    }

    // The WM_DESTROY messages are copacetic for both dialog boxes and
    // normal windows. Just get the caption and check for a match.
    if ( WM_DESTROY == pMsg->message )
    {
        if ( 0 != GetWindowText ( pMsg->hwnd , szBuff , MAX_PATH ) )
        {
            CheckTableMatch ( ANTN_DESTROYWINDOW , pMsg->hwnd , szBuff ) ;
        }
        return ( lRet ) ;
    }

    // Window creation isn't as clean as window destruction.

    // Get the window class. If it is a true dialog box, the
    // WM_INITDIALOG is all I need.
    if ( 0 == GetClassName ( pMsg->hwnd , szBuff , MAX_PATH ) )
    {
#ifdef _DEBUG
        TCHAR szBuff[ 50 ] ;
        wsprintf ( szBuff                                           ,
                   _T ( "GetClassName failed for HWND : 0x%08X\n" ) ,
                   pMsg->hwnd                                        ) ;
        TRACE ( szBuff ) ;
#endif
        // There's not much point in going on.
        return ( lRet ) ;
    }
    if ( 0 == lstrcmpi ( szBuff , _T ( "#32770" ) ) )
    {
        // The only message I need to check is WM_INITDIALOG.
        if ( WM_INITDIALOG == pMsg->message )
        {
            // Get the caption of the dialog box.
            if ( 0 != GetWindowText ( pMsg->hwnd , szBuff , MAX_PATH ) )
            {
                CheckTableMatch ( ANTN_CREATEWINDOW ,
                                  pMsg->hwnd        ,
                                  szBuff             ) ;
            }
        }
        return ( lRet ) ;
    }
    // That took care of true dialog boxes. Start figuring out what to do
    // for actual windows.

    if ( WM_CREATE == pMsg->message )
    {
        // Very few windows set the title in WM_CREATE.
        // However, a few do and they don't use WM_SETTEXT, so I have
        // to check.
        if ( 0 != GetWindowText ( pMsg->hwnd , szBuff , MAX_PATH ) )
        {
            CheckTableMatch ( ANTN_CREATEWINDOW ,
                              pMsg->hwnd        ,
                              szBuff             ) ;
        }
    }
    else if ( WM_SETTEXT == pMsg->message )
    {
        // I always default to WM_SETTEXT because that's how captions
        // get set. Unfortunately, some applications, such as Internet
        // Explorer, seem to call WM_SETTEXT a bunch of times with the
        // same title. To keep this hook simple, I just report
        // the WM_SETTEXT instead of maintaining all sorts of weird,
        // hard-to-debug data structures that keep track of the windows
        // that called WM_SETTEXT previously.
        if ( NULL != pMsg->lParam )
        {
            CheckTableMatch ( ANTN_CREATEWINDOW     ,
                              pMsg->hwnd            ,
                              (LPCTSTR)pMsg->lParam  ) ;
        }
    }

    return ( lRet ) ;
}

