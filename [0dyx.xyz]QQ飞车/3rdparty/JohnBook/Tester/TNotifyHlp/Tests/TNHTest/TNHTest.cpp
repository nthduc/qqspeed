#include "stdafx.h"
#include "TNotifyHlp.h"

void main ( void )
{
    // Do some quicky tests.
    HANDLE hRet1 = AddNotifyTitle ( ANTN_CREATEWINDOW , ANTS_EXACTMATCH , _T ( "1This is a test!" ) ) ;
    HANDLE hRet2 = AddNotifyTitle ( ANTN_CREATEWINDOW , ANTS_EXACTMATCH , _T ( "2This is a test!" ) ) ;
    HANDLE hRet3 = AddNotifyTitle ( ANTN_CREATEWINDOW , ANTS_EXACTMATCH , _T ( "3This is a test!" ) ) ;
    HANDLE hRet4 = AddNotifyTitle ( ANTN_CREATEWINDOW , ANTS_EXACTMATCH , _T ( "4This is a test!" ) ) ;
    HANDLE hRet5 = AddNotifyTitle ( ANTN_CREATEWINDOW , ANTS_EXACTMATCH , _T ( "5This is a test!" ) ) ;
    HANDLE hRet6 = AddNotifyTitle ( ANTN_CREATEWINDOW , ANTS_EXACTMATCH , _T ( "6This is a test!" ) ) ;
    RemoveNotifyTitle ( hRet3 ) ;
    hRet6 = AddNotifyTitle ( ANTN_CREATEWINDOW , ANTS_EXACTMATCH , _T ( "6This is a test!" ) ) ;
    RemoveNotifyTitle ( hRet3 ) ;
    RemoveNotifyTitle ( hRet2 ) ;
    RemoveNotifyTitle ( hRet5 ) ;
    RemoveNotifyTitle ( hRet1 ) ;
    RemoveNotifyTitle ( hRet4 ) ;

    // To test this, start up TNHTest in a different window and start
    // and stop Notepad multiple times.

    hRet1 = AddNotifyTitle ( ANTN_CREATEWINDOW     , ANTS_ANYLOCMATCH , _T ( "Notepad" ) ) ;
    hRet2 = AddNotifyTitle ( ANTN_DESTROYWINDOW    , ANTS_ANYLOCMATCH , _T ( "Notepad" ) ) ;
    hRet3 = AddNotifyTitle ( ANTN_CREATEANDDESTROY , ANTS_ANYLOCMATCH , _T ( "Notepad" ) ) ;
    for ( int i = 0 ; i < 10 ; i++ )
    {
        Sleep ( 1000 ) ;
        HWND hWnd ;
        BOOL bDest ;
        hWnd = CheckNotifyCreateTitle ( hRet1 ) ;
        if ( NULL != hWnd )
        {
            _tprintf ( _T ( "%d hRet1 (Create, Anymatch)  = 0x%08X\n" ) , i , hWnd ) ;
        }
        hWnd = CheckNotifyCreateTitle ( hRet2 ) ;
        if ( NULL != hWnd )
        {
            _tprintf ( _T ( "%d hRet2 (Destroy, Anymatch) = 0x%08X\n" ) , i , hWnd ) ;
        }
        hWnd = CheckNotifyCreateTitle ( hRet3 ) ;
        bDest = CheckNotifyDestroyTitle ( hRet3 ) ;
        if ( ( NULL != hWnd ) || ( TRUE == bDest ) )
        {
            _tprintf ( _T ( "%d hRet3 (Both, Anymatch)    = 0x%08X / %d\n" ) , i , hWnd , bDest ) ;
        }
    }
    RemoveNotifyTitle ( hRet1 ) ;
    RemoveNotifyTitle ( hRet2 ) ;
    RemoveNotifyTitle ( hRet3 ) ;


    hRet3 = AddNotifyTitle ( ANTN_CREATEWINDOW     , ANTS_BEGINMATCH , _T ( "Untitled - " ) ) ;
    hRet4 = AddNotifyTitle ( ANTN_DESTROYWINDOW    , ANTS_BEGINMATCH , _T ( "Untitled - " ) ) ;
    hRet5 = AddNotifyTitle ( ANTN_CREATEANDDESTROY , ANTS_BEGINMATCH , _T ( "Untitled - " ) ) ;
    for ( i = 0 ; i < 10 ; i++ )
    {
        Sleep ( 1000 ) ;
        HWND hWnd ;
        BOOL bDest ;
        hWnd = CheckNotifyCreateTitle ( hRet3 ) ;
        if ( NULL != hWnd )
        {
            _tprintf ( _T ( "%d hRet3 (Create, Beginmatch)  = 0x%08X\n" ) , i , hWnd ) ;
        }
        hWnd = CheckNotifyCreateTitle ( hRet4 ) ;
        if ( NULL != hWnd )
        {
            _tprintf ( _T ( "%d hRet4 (Destroy, Beginmatch) = 0x%08X\n" ) , i , hWnd ) ;
        }
        hWnd = CheckNotifyCreateTitle ( hRet5 ) ;
        bDest = CheckNotifyDestroyTitle ( hRet5 ) ;
        if ( ( NULL != hWnd ) || ( TRUE == bDest ) )
        {
            _tprintf ( _T ( "%d hRet5 (Both, Anymatch)    = 0x%08X / %d\n" ) , i , hWnd , bDest ) ;
        }
    }
    RemoveNotifyTitle ( hRet3 ) ;
    RemoveNotifyTitle ( hRet4 ) ;
    RemoveNotifyTitle ( hRet5 ) ;

    hRet3 = AddNotifyTitle ( ANTN_CREATEWINDOW     , ANTS_EXACTMATCH , _T ( "Untitled - Notepad" ) ) ;
    hRet4 = AddNotifyTitle ( ANTN_DESTROYWINDOW    , ANTS_EXACTMATCH , _T ( "Untitled - Notepad" ) ) ;
    hRet5 = AddNotifyTitle ( ANTN_CREATEANDDESTROY , ANTS_EXACTMATCH , _T ( "Untitled - Notepad" ) ) ;
    for ( i = 0 ; i < 10 ; i++ )
    {
        Sleep ( 1000 ) ;
        HWND hWnd ;
        BOOL bDest ;
        hWnd = CheckNotifyCreateTitle ( hRet3 ) ;
        if ( NULL != hWnd )
        {
            _tprintf ( _T ( "%d hRet3 (Create, Exactmatch)  = 0x%08X\n" ) , i , hWnd ) ;
        }
        hWnd = CheckNotifyCreateTitle ( hRet4 ) ;
        if ( NULL != hWnd )
        {
            _tprintf ( _T ( "%d hRet4 (Destroy, Exactmatch) = 0x%08X\n" ) , i , hWnd ) ;
        }
        hWnd = CheckNotifyCreateTitle ( hRet5 ) ;
        bDest = CheckNotifyDestroyTitle ( hRet5 ) ;
        if ( ( NULL != hWnd ) || ( TRUE == bDest ) )
        {
            _tprintf ( _T ( "%d hRet5 (Both, Anymatch)    = 0x%08X / %d\n" ) , i , hWnd , bDest ) ;
        }
    }
    RemoveNotifyTitle ( hRet3 ) ;
    RemoveNotifyTitle ( hRet4 ) ;
    RemoveNotifyTitle ( hRet5 ) ;

    // Check the DllMain removal.
    hRet3 = AddNotifyTitle ( ANTN_CREATEWINDOW     , ANTS_EXACTMATCH , _T ( "Untitled - Notepad" ) ) ;
    hRet4 = AddNotifyTitle ( ANTN_DESTROYWINDOW    , ANTS_EXACTMATCH , _T ( "Untitled - Notepad" ) ) ;

}
