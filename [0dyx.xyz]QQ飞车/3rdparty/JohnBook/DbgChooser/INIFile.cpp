/*---------------------------------------------------------------------
    Debugging .NET and Windows-based Applications - John Robbins
---------------------------------------------------------------------*/

#include "STDAFX.h"
#include "INIFile.h"

BOOL GetVCDebugger ( LPTSTR szBuff , int iLen )
{
    GetPrivateProfileString ( INI_SECNAME ,
                              INI_KEY_VC  ,
                              VC_DEFAULT  ,
                              szBuff      ,
                              iLen        ,
                              INI_FILE     ) ;
    if ( _T ( '\0' ) != szBuff[ 0 ] )
    {
        return ( TRUE ) ;
    }
    return ( FALSE ) ;
}

BOOL GetWinDBGDebugger ( LPTSTR szBuff , int iLen )
{
    GetPrivateProfileString ( INI_SECNAME     ,
                              INI_KEY_WINDBG  ,
                              WINDBG_DEFAULT  ,
                              szBuff          ,
                              iLen            ,
                              INI_FILE         ) ;
    if ( _T ( '\0' ) != szBuff[ 0 ] )
    {
        return ( TRUE ) ;
    }
    return ( FALSE ) ;
}

BOOL GetDrWatsonDebugger ( LPTSTR szBuff , int iLen )
{
    GetPrivateProfileString ( INI_SECNAME      ,
                              INI_KEY_DRWATSON ,
                              DRWATSON_DEFAULT ,
                              szBuff           ,
                              iLen             ,
                              INI_FILE          ) ;
    if ( _T ( '\0' ) != szBuff[ 0 ] )
    {
        return ( TRUE ) ;
    }
    return ( FALSE ) ;
}

BOOL GetVCSevenDebugger ( LPTSTR szBuff , int iLen )
{
    GetPrivateProfileString ( INI_SECNAME       ,
                              INI_KEY_VCSEVEN   ,
                              VCSEVEN_DEFAULT   ,
                              szBuff            ,
                              iLen              ,
                              INI_FILE           ) ;
    if ( _T ( '\0' ) != szBuff[ 0 ] )
    {
        return ( TRUE ) ;
    }
    return ( FALSE ) ;
}


BOOL SetVCDebugger ( LPCTSTR szBuff )
{
    return ( WritePrivateProfileString ( INI_SECNAME ,
                                         INI_KEY_VC  ,
                                         szBuff      ,
                                         INI_FILE     ) ) ;
}

BOOL SetWinDBGDebugger ( LPCTSTR szBuff )
{
    return ( WritePrivateProfileString ( INI_SECNAME     ,
                                         INI_KEY_WINDBG  ,
                                         szBuff          ,
                                         INI_FILE         ) ) ;
}

BOOL SetDrWatsonDebugger ( LPCTSTR szBuff )
{
    return ( WritePrivateProfileString ( INI_SECNAME       ,
                                         INI_KEY_DRWATSON  ,
                                         szBuff            ,
                                         INI_FILE           ) ) ;
}

BOOL SetVCSevenDebugger ( LPCTSTR szBuff )
{
    return ( WritePrivateProfileString ( INI_SECNAME        ,
                                         INI_KEY_VCSEVEN    ,
                                         szBuff             ,
                                         INI_FILE            ) ) ;
}

int SpawnPauseWait ( void ) 
{
    return ( GetPrivateProfileInt ( INI_SECNAME         , 
                                    INI_SPAWN_WAIT_KEY  , 
                                    MIN_SPAWN_WAIT_TIME , 
                                    INI_FILE             ) ) ;
}

