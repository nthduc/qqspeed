/*---------------------------------------------------------------------
    Debugging .NET and Windows-based Applications - John Robbins
---------------------------------------------------------------------*/
#include "stdafx.h"
#include "Registry.h"

// The AeDebug registry key.
const LPCTSTR k_AEDEBUGKEY =
    _T ( "Software\\Microsoft\\Windows NT\\CurrentVersion\\AeDebug" );
// The debugger value.
const LPCTSTR k_DEBUGGERVALUE = _T ( "Debugger" ) ;

static BOOL SetAEDebugKey ( LPCTSTR szVal )
{
    HKEY hKey ;

    LONG lRet = RegOpenKeyEx ( HKEY_LOCAL_MACHINE ,
                                k_AEDEBUGKEY       ,
                                0                  ,
                                KEY_ALL_ACCESS     ,
                                &hKey               ) ;
    if ( lRet != ERROR_SUCCESS )
    {
        ASSERT ( !"RegOpenKeyEx failed!" ) ;
        return ( FALSE ) ;
    }

    // Write the Debugger value.
    DWORD dwLen = (DWORD)_tcslen ( szVal ) * sizeof ( TCHAR ) ;
    lRet = RegSetValueEx ( hKey                 ,
                           k_DEBUGGERVALUE      ,
                           0                    ,
                           REG_SZ               ,
                           (const BYTE*)szVal   ,
                           dwLen                 ) ;
                           
    // Flush the key immediately to get the changes written.
    RegFlushKey ( hKey ) ;                           
                           
    // Close the key no matter what happened.
    RegCloseKey ( hKey ) ;
    if ( lRet != ERROR_SUCCESS )
    {
        ASSERT ( !"RegSetValueEx failed" ) ;
        return ( FALSE ) ;
    }
    return ( TRUE ) ;
}

// A helper function that will set DbgChooser as the debugger of choice
// for all crashes.
BOOL SetDbgChooserAsCrashDebugger ( void )
{
    TCHAR szFile [ MAX_PATH ] ;
    CString sFullCommand ;

    if ( 0 == GetModuleFileName ( NULL , szFile , MAX_PATH ) )
    {
        ASSERT ( !"GetModuleFileName failed!" ) ;
        return ( FALSE ) ;
    }
    sFullCommand = szFile ;
    sFullCommand += _T ( " -p %ld -e %ld" ) ;
    
    return ( SetAEDebugKey ( sFullCommand ) ) ;
}


BOOL SetValueAsCrashDebugger ( LPCTSTR szValue ) 
{
    return ( SetAEDebugKey ( szValue ) ) ;
}