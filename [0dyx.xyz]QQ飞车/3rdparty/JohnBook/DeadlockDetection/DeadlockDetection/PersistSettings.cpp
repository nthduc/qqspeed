/*----------------------------------------------------------------------
Debugging Applications for Microsoft .NET and Microsoft Windows
Copyright © 1997-2003 John Robbins -- All rights reserved.
------------------------------------------------------------------------
All the persistent storage handling routines
----------------------------------------------------------------------*/

#include "PCH.h"
#include "PersistSettings.h"

/*//////////////////////////////////////////////////////////////////////
                             File Constants
//////////////////////////////////////////////////////////////////////*/
const LPCTSTR k_INIFILENAME         = _T ( "DeadlockDetection.ini" ) ;
const LPCTSTR k_INITSECTION         = _T ( "Initialization"        ) ;
const LPCTSTR k_EXTDLL_KEY          = _T ( "ExtDll"                ) ;
const LPCTSTR k_STARTINDLLMAIN_KEY  = _T ( "StartInDllMain"        ) ;
const LPCTSTR k_INITIALOPTS_KEY     = _T ( "InitialOpts"           ) ;
const LPCTSTR k_IGNORESECTION       = _T ( "IgnoreModules"         ) ;
const LPCTSTR k_IGNOREVALFMT        = _T ( "Ignore%d"              ) ;

/*//////////////////////////////////////////////////////////////////////
                         File Global Variables
//////////////////////////////////////////////////////////////////////*/
static TCHAR g_szINIFile [ MAX_PATH ] = _T ( "\0" ) ;

/*//////////////////////////////////////////////////////////////////////
                        Function Implementation
//////////////////////////////////////////////////////////////////////*/
void SetPersistentFilename ( HINSTANCE hMod  )
{
    // Get the module filename.
    VERIFY ( GetModuleFileName ( hMod           ,
                                 g_szINIFile    ,
                                 MAX_PATH * sizeof ( TCHAR ) ) ) ;

    // Find the last slash in the filename.
    TCHAR * pPos = g_szINIFile + lstrlen ( g_szINIFile ) ;

    while ( _T ( '\\' ) != *pPos )
    {
        pPos-- ;
        if ( pPos == g_szINIFile )
        {
            ASSERT ( FALSE ) ;
            return ;
        }
    }

    pPos++ ;

    lstrcpy ( pPos , k_INIFILENAME ) ;
}

int GetExtDllFilename ( LPTSTR szBuff , UINT uiSize , LPCTSTR szDef )
{
    ASSERT ( _T ( '\0' ) != g_szINIFile[ 0 ] ) ;

    return ( GetPrivateProfileString ( k_INITSECTION    ,
                                       k_EXTDLL_KEY     ,
                                       szDef            ,
                                       szBuff           ,
                                       uiSize           ,
                                       g_szINIFile       ) ) ;
}

BOOL StartInDllMain ( BOOL bDef )
{
    ASSERT ( _T ( '\0' ) != g_szINIFile[ 0 ] ) ;

    return ( GetPrivateProfileInt ( k_INITSECTION           ,
                                    k_STARTINDLLMAIN_KEY    ,
                                    bDef                    ,
                                    g_szINIFile              ) ) ;
}

BOOL GetIgnoreModule ( int iMod , TCHAR * szMod )
{
    ASSERT ( _T ( '\0' ) != g_szINIFile[ 0 ] ) ;

    TCHAR szVal [ 50 ] ;
    wsprintf ( szVal , k_IGNOREVALFMT , iMod ) ;

    int iRet = GetPrivateProfileString ( k_IGNORESECTION    ,
                                         szVal              ,
                                         _T ( "" )          ,
                                         szMod              ,
                                         MAX_PATH           ,
                                         g_szINIFile         ) ;
    return ( iRet != 0 ) ;
}

DWORD DefaultEventOptions ( DWORD dwDef )
{
    ASSERT ( _T ( '\0' ) != g_szINIFile[ 0 ] ) ;

    return ( GetPrivateProfileInt ( k_INITSECTION       ,
                                    k_INITIALOPTS_KEY   ,
                                    dwDef               ,
                                    g_szINIFile          ) ) ;
}

