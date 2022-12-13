/*----------------------------------------------------------------------
Debugging Applications for Microsoft .NET and Microsoft Windows
Copyright © 1997-2003 John Robbins -- All rights reserved.
----------------------------------------------------------------------*/
#include "stdafx.h"
#include "ParsePlayInputString.h"
#include "PlayInput.h"

BOOL ParseCommandLine ( int      argc    ,
                        _TCHAR * argv[]  ,
                        BOOL &   bDoKeys ,
                        TCHAR *  szFile   ) ;

void ShowUsage ( void ) ;
BOOL TestIt ( LPCTSTR szString , BOOL bDoKeys ) ;

int _tmain ( int argc , _TCHAR * argv[] )
{
    int iRet = 1 ;
    
    TCHAR szFile[ MAX_PATH ] ;
    BOOL  bDoKeys = TRUE ;
    if ( TRUE == ParseCommandLine ( argc ,argv , bDoKeys , szFile ) )
    {
        FILE * pFile = _tfopen ( szFile , _T ( "rt" ) ) ;
        if ( NULL != pFile )
        {
            if ( TRUE == bDoKeys )
            {
                _tprintf ( _T ( "Watch Notepad!!!! The output " ) \
                           _T ( "goes there!\n" ) ) ;

                WinExec ( "Notepad" , SW_SHOWNORMAL ) ;

                Sleep ( 1000 ) ;
                
                TCHAR  szReadBuff[ MAX_PATH ] ;
                LPTSTR pReadStart = szReadBuff ;
                while ( NULL != _fgetts ( szReadBuff ,
                                          MAX_PATH   ,
                                          pFile       ) )
                {
                    // Does the line start with a semicolon?
                    if ( _T ( ';' ) == szReadBuff[ 0 ] )
                    {
                        // Print the comment to stdout.
                        _tprintf ( szReadBuff ) ;
                        continue ;
                    }

                    // Strip off the \n on the end.
                    LPTSTR pCRLF = pReadStart +
                                      _tcslen ( szReadBuff ) - 1 ;
                    *pCRLF = _T ( '\0' ) ;

                    // Don't let empty lines through.
                    if ( _T ( '\0' ) == szReadBuff[ 0 ] )
                    {
                        continue ;
                    }

                    // Check for special codes.
                    if ( 0 == _tcscmp ( _T ( ":Sleep" ) , szReadBuff ) )
                    {
                        Sleep ( 1000 ) ;
                        continue ;
                    }

                    // DO IT!!
                    TestIt ( szReadBuff , bDoKeys ) ;

                }
            }
        }
        else
        {
            _tprintf ( _T ( "Unable to open the file : %s\n" ) ,
                       szFile                                   ) ;
            iRet = 0 ;
        }
    }
    else
    {
        iRet = 0 ;
        ShowUsage ( ) ;
    }
    return ( iRet ) ;
}

BOOL TestIt ( LPCTSTR szString , BOOL bDoKeys )
{
    PINPUT pInput    = NULL ;
    UINT   uiErrChar = 0 ;
    UINT   uiCount   = 0 ;

    _tprintf ( _T ( "The test string : %s\n" ) , szString ) ;

    BOOL bRet = ParsePlayInputString ( szString  ,
                                       uiErrChar ,
                                       pInput    ,
                                       uiCount    ) ;

    if ( 0 != uiCount )
    {
        UINT i = 0 ;
        if ( TRUE == bDoKeys )
        {
            for ( i = 0 ; i < uiCount ; i++ )
            {
                TCHAR szBuff[ 100 ] ;
                UINT uiTrans =
                          MapVirtualKey ( pInput[i].ki.wVk , 0 ) << 16 ;
                if ( 0 != GetKeyNameText ( uiTrans  ,
                                           szBuff   ,
                                           100       ) )
                {
                    TCHAR * szDir = _T ( "(d)" ) ;
                    if ( pInput[i].ki.dwFlags == KEYEVENTF_KEYUP )
                    {
                        szDir = _T ( "(u)" ) ;
                    }
                    _tprintf ( _T ( "%s%s " ) , szBuff , szDir ) ;
                }
            }
        }
        _tprintf ( _T ( "\n" ) ) ;



        UINT iRet = SendInput ( uiCount , pInput , sizeof ( INPUT ) ) ;
        DWORD le = GetLastError ( ) ;
        _tprintf ( _T ( "SendInput returned : %d, LE = (0x%08X)\n" ) ,
                   iRet ,
                   le    ) ;
    }
    else
    {
        _tprintf ( _T ( "Failure at position : %d\n" ) , uiErrChar ) ;
    }

    if ( NULL != pInput )
    {
        delete [] pInput ;
    }

    _tprintf ( _T ( "\n" ) ) ;

    return ( bRet ) ;
}

void ShowUsage ( void )
{
_tprintf (
_T ( "ParsePlayKeysTest [-m|-k] file\n" ) \
_T ( "  -m   - Input file is mouse commands, just move the mouse\n" ) \
_T ( "  -k   - Input file is keystroke commands, show the keys in\n" )\
_T ( "         the console, start NOTEPAD and send the output there\n")\
_T ( "  file - The input file contains the strings to test, one per\n")\
_T ( "         line.  Comment lines start with a semicolon in the\n")\
_T ( "         first position. A line with ':Sleep' on it will\n" ) \
_T ( "         do a a 1 sec sleep.\n" ) );
}

BOOL ParseCommandLine ( int      argc    ,
                        _TCHAR * argv[]  ,
                        BOOL &   bDoKeys ,
                        TCHAR *  szFile   )
{
    BOOL bRet = TRUE ;
    if ( argc > 2 )
    {
        if ( _tcsicmp ( argv[1] , _T ( "-m" ) ) == 0 )
        {
            bDoKeys = FALSE ;
        }
        else if ( _tcsicmp ( argv[1] , _T ( "-k" ) ) == 0 )
        {
            bDoKeys = TRUE ;
        }
        else
        {
            bRet = FALSE ;
        }
        
        if ( FALSE != bRet )
        {
            _tcscpy ( szFile , argv[2] ) ;
        }
    }
    else
    {
        bRet = FALSE ;
    }
    return ( bRet ) ;
}