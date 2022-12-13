/*----------------------------------------------------------------------
Debugging Applications for Microsoft .NET and Microsoft Windows
Copyright © 1997-2003 John Robbins -- All rights reserved.
----------------------------------------------------------------------*/

/*//////////////////////////////////////////////////////////////////////
// Necessary Includes
//////////////////////////////////////////////////////////////////////*/
#include "STDAFX.h"
#include "CommandLine.h"

/*//////////////////////////////////////////////////////////////////////
// File Scope Constants & Defines
//////////////////////////////////////////////////////////////////////*/

/*//////////////////////////////////////////////////////////////////////
// File Scope Typedefs
//////////////////////////////////////////////////////////////////////*/

/*//////////////////////////////////////////////////////////////////////
// File Scope Prototypes
//////////////////////////////////////////////////////////////////////*/

/*//////////////////////////////////////////////////////////////////////
// File Scope Globals
//////////////////////////////////////////////////////////////////////*/

/*//////////////////////////////////////////////////////////////////////
// EXTERNALLY VISIBLE CODE STARTS HERE
//////////////////////////////////////////////////////////////////////*/

BOOL ResolveCommandLine ( OPTIONS & stOpts , int argc , TCHAR * argv[] )
{
    // Set the options to the defaults.
    memset ( &stOpts , NULL , sizeof ( OPTIONS ) ) ;

    // Check the easy case.
    if ( 1 == argc )
    {
        stOpts.bShowHelp = TRUE ;
        return ( FALSE ) ;
    }

    for ( int i = 1 ; i < argc ; i++ )
    {
        LPTSTR szString = argv[ i ] ;

        switch ( *szString )
        {
            case _T ( '/' ) :
            case _T ( '-' ) :
            {
                switch (  _totupper ( *(++szString) ) )
                {
                    case _T ( 'V' ) :
                        stOpts.bVerbose = TRUE ;
                    break ;
                    case _T ( 'U' ) :
                        stOpts.bUndecorate = TRUE ;
                    break ;
                    case _T ( 'N' ) :
                    {
                        if ( 0 == _tcsicmp ( szString , _T ( "nologo")))
                        {
                            stOpts.bSkipLogo = TRUE ;
                        }
                        else
                        {
                            stOpts.bShowHelp = TRUE ;
                            return ( FALSE ) ;
                        }
                    }
                    break ;
                    case _T ( '?' ) :
                        stOpts.bShowHelp = TRUE ;
                        return ( TRUE ) ;
                    break ;
                    default         :
                        stOpts.bShowHelp = TRUE ;
                        return ( FALSE ) ;
                    break ;
                }
            }
            break ;

            default :
            {
                // If the filename has already been filled in, then
                // error out.
                if ( _T ( '\0' ) != stOpts.szFile[ 0 ] )
                {
                    stOpts.bShowHelp = TRUE ;
                    return ( FALSE ) ;
                }
                _tcscpy ( stOpts.szFile , szString ) ;
            }
            break ;
        }
    }

    // Make sure the filename exists.
    if ( _T ( '\0' ) == stOpts.szFile[ 0 ] )
    {
        stOpts.bShowHelp = TRUE ;
        return ( FALSE ) ;
    }
    return ( TRUE ) ;
}

/*//////////////////////////////////////////////////////////////////////
// FILE SCOPE CODE STARTS HERE
//////////////////////////////////////////////////////////////////////*/

/*----------------------------------------------------------------------
FUNCTION        :
DISCUSSION      :
PARAMETERS      :
RETURNS         :
----------------------------------------------------------------------*/

