/*----------------------------------------------------------------------
Debugging Applications for Microsoft .NET and Microsoft Windows
Copyright © 1997-2003 John Robbins -- All rights reserved.
----------------------------------------------------------------------*/
#include "stdafx.h"
#include "SWSDLL.h"
#include "VerboseMacros.h"

// Helper define to let code compile on pre W2K systems.
#if _WIN32 >= 0x500
#define GET_THREAD_ACP() CP_THREAD_ACP
#else
#define GET_THREAD_ACP() GetACP ( )
#endif

DWORD Wide2Ansi ( const wchar_t * szWide   ,
                  char *          szANSI   ,
                  int             iANSILen  )
{
    ASSERT ( NULL != szWide ) ;
    ASSERT ( NULL != szANSI ) ;
    ASSERT ( FALSE == IsBadStringPtrW ( szWide , MAX_PATH ) ) ;

    int iRet = WideCharToMultiByte ( GET_THREAD_ACP() ,
                                     0                ,
                                     szWide           ,
                                     -1               ,
                                     szANSI           ,
                                     iANSILen         ,
                                     NULL             ,
                                     NULL              ) ;
    return ( iRet ) ;
}


DWORD64 InitSymEngAndLoadModule ( CSymbolEngine  * pSym             ,
                                  TCHAR *          szModule         ,
                                  PFNOUTPUT        pfnVerboseOutput  )
{
    // Add the path to the module search path.
    DWORD dwPathLen = GetEnvironmentVariable ( _T ( "PATH" ) ,
                                               NULL          ,
                                               0              ) ;
    ASSERT ( 0 != dwPathLen ) ;
    if ( 0 == dwPathLen )
    {
        VERBOSE ( _T ( "Unable to get PATH environment var!" ) ) ;
        return ( 0 ) ;
    }
    
    dwPathLen += 1 ;
    
    char * szPath = (char*)malloc ( dwPathLen ) ;
    
    ASSERT ( NULL != szPath ) ;

    GetEnvironmentVariableA ( "PATH"        ,
                              szPath        ,
                              dwPathLen - 1  ) ;
    
    pSym->SymSetSearchPath ( szPath ) ;
    
    free ( szPath ) ;

    VERBOSE ( _T ( "Initializing the symbol engine.\n" ) ) ;
    if ( FALSE == pSym->SymInitialize ( GetCurrentProcess ( ) ,
                                        NULL                  ,
                                        FALSE                  ) )
    {
        ASSERT ( !"Symbol engine initialization failed!" ) ;
        VERBOSE ( _T ( "Symbol engine initialization failed!" ) ) ;
        return ( 0 ) ;
    }
    
    int iLen = (int)_tcslen ( szModule ) + 1 ;
    char * szAnsiMod = (char*)malloc ( iLen ) ;
    Wide2Ansi ( szModule , szAnsiMod , iLen ) ;
    
    VERBOSE ( _T ( "Loading the symbols\n" ) ) ;
    DWORD64 dwBase = pSym->SymLoadModule ( NULL        ,
                                           szAnsiMod   ,
                                           NULL        ,
                                           0           ,
                                           0            ) ;
    ASSERT ( 0 != dwBase ) ;
    if ( 0 == dwBase )
    {
        VERBOSE ( _T ( "Loading symbols failed!!\n" ) ) ;
    }
    
    free ( szAnsiMod ) ;
    
    return ( dwBase ) ;
}