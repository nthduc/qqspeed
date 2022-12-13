/*----------------------------------------------------------------------
Debugging Applications for Microsoft .NET and Microsoft Windows
Copyright © 1997-2003 John Robbins -- All rights reserved.
----------------------------------------------------------------------*/
/*//////////////////////////////////////////////////////////////////////
                              The Includes
//////////////////////////////////////////////////////////////////////*/
#include "stdafx.h"
//#include "SWSFile.h"
#include "ResString.h"
#include "Resource.h"
#include "SWSDLL.h"

/*//////////////////////////////////////////////////////////////////////
                          Defines and Typedefs
//////////////////////////////////////////////////////////////////////*/
// The macros that take care of the verbose output.
#define VERBOSE(sz)                     \
    if ( TRUE == g_bVerbose )           \
    {                                   \
        _tprintf ( _T ( "%s" ) , sz ) ; \
    }
#define VERBOSE1(sz, p1)                \
    if ( TRUE == g_bVerbose )           \
    {                                   \
        _tprintf ( sz , p1) ;           \
    }
#define VERBOSE2(sz, p1, p2)            \
    if ( TRUE == g_bVerbose )           \
    {                                   \
        _tprintf ( sz , p1 , p2 ) ;     \
    }
#define VERBOSE3(sz, p1, p2, p3)        \
    if ( TRUE == g_bVerbose )           \
    {                                   \
        _tprintf ( sz , p1 , p2 , p3 ) ;\
    }


// The last help string resource value.
#define LAST_HELP_ID IDS_HELP7

// The parsed command line options.
typedef struct tag_CMDOPTS
{
    // Don't show the logo.
    BOOL bNoLogo ;
    // Tune the module specified.
    BOOL bTune ;
    // Dump the specified data file.
    BOOL bDump ;
    // Generate the SWS and SDW for the module specified.
    BOOL bGenerate ;
    // The module or file to work on.
    TCHAR szFile[ MAX_PATH ] ;
} CMDOPTS ;

/*//////////////////////////////////////////////////////////////////////
                           File Scope Globals
//////////////////////////////////////////////////////////////////////*/
// The secret "-v" verbose switch.
static BOOL g_bVerbose = FALSE ;

/*//////////////////////////////////////////////////////////////////////
                          Function Prototypes
//////////////////////////////////////////////////////////////////////*/
BOOL ParseCommandLine ( int argc , TCHAR * argv[] , CMDOPTS & stOpts ) ;
void ShowHelp ( void ) ;

////////////////////////////////////////////////////////////////////////
/*//////////////////////////////////////////////////////////////////////
                           !!!ENTRY POINT!!!
//////////////////////////////////////////////////////////////////////*/
////////////////////////////////////////////////////////////////////////
#ifdef UNICODE
int wmain ( int argc , wchar_t * argv[] )
#else
int _tmain ( int argc , TCHAR* argv[] )
#endif
{
    // Parsed command line options.
    CMDOPTS stOpts ;
    // A resource string wrapper.
    CResString cRes ;

    // Load the logo just in case.
    VERIFY ( cRes.LoadString ( IDS_LOGO ) ) ;

    if ( FALSE == ParseCommandLine ( argc , argv , stOpts ) )
    {
        _tprintf ( (LPCTSTR)cRes ) ;
        ShowHelp ( ) ;
        return ( FALSE ) ;
    }

    if ( FALSE == stOpts.bNoLogo )
    {
        _tprintf ( (LPCTSTR)cRes ) ;
    }

    VERBOSE ( _T ( "Verbose output turned on\n" ) ) ;

    BOOL bRet = FALSE ;
    PFNOUTPUT pfnVerboseOut = NULL ;
    if ( TRUE == g_bVerbose )
    {
        pfnVerboseOut = _tprintf ;
    }
    // The command line parameters are good so do it.
    if ( TRUE == stOpts.bDump )
    {
        VERBOSE1 ( _T ( "Action -> Dumping data for : %s\n" ) ,
                   stOpts.szFile                               ) ;
        bRet = DumpFiles ( stOpts.szFile ,
                           _tprintf      ,
                           pfnVerboseOut  ) ;
        if ( FALSE == bRet )
        {
            cRes.LoadString ( IDS_DUMPFAILED ) ;
            _tprintf ( (LPCTSTR)cRes ) ;
        }
    }
    else if ( TRUE == stOpts.bGenerate )
    {
        VERBOSE1 ( _T ("Action -> Generating initial files for : %s\n"),
                   stOpts.szFile                                      );
        bRet = GenerateInitialFiles ( stOpts.szFile ,
                                      pfnVerboseOut  ) ;
    }
    else if ( TRUE == stOpts.bTune )
    {
        VERBOSE1 ( _T ( "Action -> Tuning for : %s\n" ) ,stOpts.szFile);
        bRet = TuneModule ( stOpts.szFile , pfnVerboseOut ) ;
    }
    else
    {
        // I should never get here...
        ASSERT ( !"Invalid action!!!" ) ;
        bRet = FALSE ;
    }

    return ( bRet ) ;
}

/*----------------------------------------------------------------------
FUNCTION        :   ParseCommandLine
DISCUSSION      :
    Parses the command line up.
PARAMETERS      :
    argc   - Param count.
    argv   - The array of parameters.
    stOpts - The parsed up data.
RETURNS         :
    TRUE  - The command line was properly parsed.
    FALSE - There was an error or help was requested.
----------------------------------------------------------------------*/
BOOL ParseCommandLine ( int argc , TCHAR * argv[] , CMDOPTS & stOpts )
{
    // Do the easy check.
    if ( argc < 3 )
    {
        return ( FALSE ) ;
    }

    ZeroMemory ( &stOpts , sizeof ( CMDOPTS ) ) ;

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
                    case _T ( '?' ) :
                        return ( FALSE ) ;
                        break ;

                    case _T ( 'T' ) :
                        if ( ( TRUE == stOpts.bDump     ) ||
                             ( TRUE == stOpts.bGenerate )   )
                        {
                            return ( FALSE ) ;
                        }
                        stOpts.bTune = TRUE ;
                        break ;
                    case _T ( 'D' ) :
                        if ( ( TRUE == stOpts.bTune     ) ||
                             ( TRUE == stOpts.bGenerate )   )
                        {
                            return ( FALSE ) ;
                        }
                        stOpts.bDump = TRUE ;
                        break ;
                    case _T ( 'G' ) :
                        if ( ( TRUE == stOpts.bTune ) ||
                             ( TRUE == stOpts.bDump )   )
                        {
                            return ( FALSE ) ;
                        }
                        stOpts.bGenerate = TRUE ;
                        break ;
                    case _T ( 'N' ) :
                        {
                            if ( 0 ==
                                  _tcsicmp ( szString , _T ( "nologo")))
                            {
                                stOpts.bNoLogo = TRUE ;
                            }
                            else
                            {
                                return ( FALSE ) ;
                            }
                        }
                        break ;
                    // The secret verbose switch.
                    case _T ( 'V' ) :
                        g_bVerbose = TRUE ;
                        break ;
                    default :
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
        return ( FALSE ) ;
    }
    return ( TRUE ) ;
}

/*----------------------------------------------------------------------
FUNCTION        :   ShowHelp
DISCUSSION      :
    Shows the help information.
PARAMETERS      :
    None.
RETURNS         :
    None.
----------------------------------------------------------------------*/
void ShowHelp ( void )
{
    CResString cRes ;
    for ( int i = IDS_HELP1 ; i <= LAST_HELP_ID ; i++ )
    {
        VERIFY ( cRes.LoadString ( i ) ) ;
        _tprintf ( (LPCTSTR)cRes ) ;
    }
}
