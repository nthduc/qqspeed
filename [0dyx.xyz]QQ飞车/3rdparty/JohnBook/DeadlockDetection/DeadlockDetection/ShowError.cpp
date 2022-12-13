/*----------------------------------------------------------------------
Debugging Applications for Microsoft .NET and Microsoft Windows
Copyright © 1997-2003 John Robbins -- All rights reserved.
----------------------------------------------------------------------*/

#include "PCH.h"
#include "ShowError.h"


void ShowFatalError ( HINSTANCE hInst , UINT uiRes , ... )
{
    TCHAR szFmt [ MAX_PATH ] ;
    TCHAR szBuff[ MAX_PATH ] ;

    if ( 0 == LoadString ( hInst , uiRes , szFmt , MAX_PATH ) )
    {
        wsprintf ( szFmt                                    ,
                   _T ( "String resource ID %d not found" ) ,
                   uiRes                                     ) ;
    }

    va_list args ;
    va_start ( args , uiRes ) ;
    wvsprintf ( szBuff , szFmt , args ) ;

    MessageBox ( GetForegroundWindow ( )                ,
                 szBuff                                 ,
                 _T ( "Deadlock Detection Core DLL" )   ,
                 MB_OK                                   ) ;
    va_end ( args ) ;
}
