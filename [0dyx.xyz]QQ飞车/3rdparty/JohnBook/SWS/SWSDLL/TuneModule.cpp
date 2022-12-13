/*----------------------------------------------------------------------
Debugging Applications for Microsoft .NET and Microsoft Windows
Copyright © 1997-2003 John Robbins -- All rights reserved.
----------------------------------------------------------------------*/

#include "stdafx.h"
#include "SWSDLL.h"
#include "SWSFile.h"
#include "Helper.h"
#include "VerboseMacros.h"

/*//////////////////////////////////////////////////////////////////////
                     Defines, Constants, & Typedefs
//////////////////////////////////////////////////////////////////////*/
const LPCTSTR k_TEMPSWSEXT = _T ( ".SWS.TEMP" ) ;
const LPCTSTR k_PRFEXT     = _T ( ".PRF" ) ;

/*//////////////////////////////////////////////////////////////////////
                           File Scope Globals
//////////////////////////////////////////////////////////////////////*/

/*//////////////////////////////////////////////////////////////////////
                          Function Prototypes
//////////////////////////////////////////////////////////////////////*/

/*//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////*/
BOOL SWSDLL_DLLINTERFACE __stdcall
                     TuneModule ( LPCTSTR   szModule         ,
                                  PFNOUTPUT pfnVerboseOutput  )
{
    ASSERT ( FALSE == IsBadStringPtr ( szModule , MAX_PATH ) ) ;
    if ( TRUE == IsBadStringPtr ( szModule , MAX_PATH ) )
    {
        ASSERT ( !"Invalid first param to TuneModule!" ) ;
        return ( FALSE ) ;
    }
    if ( NULL != pfnVerboseOutput )
    {
        ASSERT ( FALSE == IsBadCodePtr ( (FARPROC)pfnVerboseOutput ) ) ;
        if ( TRUE == IsBadCodePtr ( (FARPROC)pfnVerboseOutput ) )
        {
            return ( FALSE ) ;
        }
    }

    // Strip off any extension and build the base name.  The base name
    // is used several places in this function.
    TCHAR szBaseName [ MAX_PATH ] ;

    _tcscpy ( szBaseName , szModule ) ;
    TCHAR * pExt = _tcsrchr (szBaseName , _T ( '.' ) ) ;
    if ( NULL != pExt )
    {
        *pExt = _T ( '\0' ) ;
    }

    // Try and copy the initial .SWS for this module to .SWSTEMP.
    TCHAR szTempFileName[ MAX_PATH ] ;
    _tcscpy ( szTempFileName , szBaseName ) ;
    _tcscat ( szTempFileName , k_TEMPSWSEXT ) ;

    TCHAR szSWSFileName[ MAX_PATH ] ;
    _tcscpy ( szSWSFileName , szBaseName ) ;
    _tcscat ( szSWSFileName , EXT_SWSFILE ) ;

    if ( FALSE == CopyFile ( szSWSFileName , szTempFileName , FALSE ) )
    {
        VERBOSE1 ( _T ( "Unable to copy .SWS to a temp name for %s\n" ),
                   szModule                                           );
        return ( FALSE ) ;
    }

    // Open the temporary file so I can accumulate all the counts.
    CSWSFile cTempSWSFile ;
    if ( FALSE == cTempSWSFile.Open ( szTempFileName ) )
    {
        VERBOSE1 ( _T ( "Unable to open temporary file: %s\n" ) ,
                   szTempFileName                                ) ;
        return ( FALSE ) ;
    }

    // Make sure I can get the header info.
    LPSWSFILEHEADER pTempHeader =
                    cTempSWSFile.GetReadOnlyHeaderData ( ) ;
    if ( NULL == pTempHeader )
    {
        VERBOSE1 ( _T ( "Unable to read header for %s\n" ) ,
                   szTempFileName                           ) ;
        cTempSWSFile.Delete ( ) ;
        return ( FALSE ) ;
    }

    // Load up the module in the symbol engine.
    CSymbolEngine cSym ;
    
    DWORD64 dwBase =
            InitSymEngAndLoadModule ( &cSym                     ,
                                      pTempHeader->szModuleName ,
                                      pfnVerboseOutput           ) ;
    if ( 0 == dwBase )
    {
        VERBOSE ( _T ( "Loading symbols failed!!\n" ) ) ;
        cTempSWSFile.Delete ( ) ;
        return ( FALSE ) ;
    }
    
    // I want all symbols to be decorated as that's how the linker
    // wants them in the .PRF file.
    SymSetOptions ( SYMOPT_PUBLICS_ONLY & ~SYMOPT_UNDNAME ) ;

    IMAGEHLP_MODULE64 stIM ;
    ZeroMemory ( &stIM , sizeof ( IMAGEHLP_MODULE64 ) ) ;
    stIM.SizeOfStruct = sizeof ( IMAGEHLP_MODULE64 ) ;
    
    if ( FALSE == cSym.SymGetModuleInfo64 ( dwBase , &stIM ) )
    {
        ASSERT ( !"Unable to get mod info!" ) ;
        VERBOSE ( _T ( "Unable to get mod info!" ) ) ;
        cTempSWSFile.Delete ( ) ;
        return ( FALSE ) ;
    }
    
    // Make sure the symbols and the .SWS filestamp match.
    if ( pTempHeader->dwLinkTime != stIM.TimeDateStamp )
    {
        ASSERT ( !"symbols and SWS do not match!" ) ;
        VERBOSE(_T("The symbols and SWS link times do not match\n"));
        cTempSWSFile.Delete ( ) ;
        return ( FALSE ) ;
    }

    // Loop through all .SWS files and add up the counts.
    BOOL bKeepLooping = TRUE ;
    int iCurrNum = 1 ;
    do
    {
        wsprintf ( szSWSFileName        ,
                   _T ( "%s.%d%s" )    ,
                   szBaseName           ,
                   iCurrNum             ,
                   EXT_SWSFILE           ) ;
        iCurrNum++ ;

        CSWSFile cCurrSWSFile ;
        if ( TRUE == cCurrSWSFile.Open ( szSWSFileName ) )
        {
            LPSWSFILEHEADER pCurrHeader =
                                cCurrSWSFile.GetReadOnlyHeaderData ( ) ;

            // Do the time stamps match?
            if ( pTempHeader->dwLinkTime == pCurrHeader->dwLinkTime )
            {
                VERBOSE1 ( _T ( "Processing : %s\n" ) ,
                           szSWSFileName                ) ;
                // Loop away.
                for ( DWORD i = 0                   ;
                      i < pTempHeader->dwEntryCount ;
                      i++                            )
                {
                    LPSWSENTRY pTempEntry =
                                          cTempSWSFile.GetIndex ( i ) ;
                    LPSWSENTRY pCurrEntry =
                                          cCurrSWSFile.GetIndex ( i ) ;
                    pTempEntry->dwExecCount += pCurrEntry->dwExecCount;
                }
            }
            else
            {
                VERBOSE1 ( _T ( "Header on %s does not match .SWS!\n" ),
                          szSWSFileName                               );
            }
            cCurrSWSFile.Close ( ) ;
        }
        else
        {
            bKeepLooping = FALSE ;
        }
    } while ( TRUE == bKeepLooping ) ;

    // All the counts have been updated.  Sort the file by execution
    // count so I can do the .PRF file work.
    cTempSWSFile.SortByExecutionCount ( ) ;

    // Open up the text output filename.
    TCHAR szPRFFile[ MAX_PATH ] ;
    _tcscpy ( szPRFFile , szBaseName ) ;
    _tcscat ( szPRFFile , k_PRFEXT ) ;
    FILE * pFile = _tfopen ( szPRFFile , _T ( "wt" ) ) ;
    if ( NULL == pFile )
    {
        VERBOSE1 ( _T ( "Unable to create the output .PRF file %s\n" ) ,
                   szPRFFile                                          );
        cTempSWSFile.Delete ( ) ;
        return ( FALSE ) ;
    }
    VERBOSE1 ( _T ( "Order file output: %s\n" ) , szPRFFile ) ;

    // Get the page size for the computer.  Yes, if you bring the .SWS
    // files from an Itanium to a Pentium (or vice-versa), things will
    // be screwed up.  The quick fix:  Just don't do that!
    SYSTEM_INFO stSI ;
    GetSystemInfo ( &stSI ) ;

    DWORD dwPageRemaining = stSI.dwPageSize ;
    
    BYTE * pByte = (BYTE*)malloc ( 4096 ) ;
    ASSERT ( NULL != pByte ) ;
    
    SYMBOL_INFO * pSym = (PSYMBOL_INFO)pByte ;
    pSym->SizeOfStruct = sizeof ( SYMBOL_INFO ) ;
    pSym->MaxNameLen = 4096 - sizeof ( SYMBOL_INFO ) ;
    
    DWORD64 dwDisp ;
    
    BOOL bContinue = TRUE ;
    do
    {
        LPSWSENTRY pEntry = NULL ;
        // Look for the first entry that's has an address
        for ( DWORD dwCurrItem = 0                   ;
              dwCurrItem < pTempHeader->dwEntryCount ;
              dwCurrItem++                            )
        {
            pEntry = cTempSWSFile.GetIndex ( dwCurrItem ) ;
            if ( 0 != pEntry->dwFnAddr )
            {
                break ;
            }
        }

        // If I looped through all of the entries, I'm done!
        if ( dwCurrItem == pTempHeader->dwEntryCount )
        {
            bContinue = FALSE ;
            break ;
        }

        // The first index I hit with an execution count of zero will
        // stop all the processing.
        if ( 0 == pEntry->dwExecCount )
        {
            bContinue = FALSE ;
            break ;
        }

        // I got a live one!

        // If the remaining page size can hold this one, output it,
        // decrement the remaining page size, and zero out this entry.
        if ( pEntry->dwSize < dwPageRemaining )
        {
            if ( FALSE == cSym.SymFromAddr ( pEntry->dwFnAddr ,
                                             &dwDisp          ,
                                             pSym              ) )
            {
                ASSERT ( !"Symbol lookup failed!!" ) ;
#ifdef _WIN64
                VERBOSE1 ( _T ( "Sym lookup failed : 0x016I64X\n" ) ,
                           pEntry->dwFnAddr                          ) ;
#else
                VERBOSE1 ( _T ( "Sym lookup failed : 0x08X\n" ) ,
                           (DWORD_PTR)pEntry->dwFnAddr           ) ;
#endif
                free ( pByte ) ;
                cTempSWSFile.Delete ( ) ;
                fclose ( pFile ) ;
                return ( FALSE ) ;
            }
            
            // Even though the linker expects C++ methods to be fully
            // decorated coming into the .PRF file.  However, for C
            // names, they are supposed to be undecorated (i.e., with
            // the underscore prefix pulled off.)  Why the difference?
            // Beats me!
            char * pSymbol = pSym->Name ;
            if ( '_' == *pSymbol )
            {
                pSymbol++ ;
            }
                               
            _ftprintf ( pFile , _T ( "%S\n" ) , pSymbol ) ;
            dwPageRemaining -= pEntry->dwSize ;
            pEntry->dwFnAddr = 0 ;

            VERBOSE4 ( _T ("Page Remaining (%04d) (%04d) : (%5d) %S\n"),
                       dwPageRemaining                             ,
                       pEntry->dwSize                              ,
                       pEntry->dwExecCount                         ,
                       pSymbol                                      ) ;
        }
        // Because the symbol engine doesn't return proper sizes for
        // some items (especially those that end the symbol lists), I
        // need to check if the symbol size is larger than the page
        // size.  (The WordPad test program demonstrates this!)  If the
        // symbol size is bigger, so just plop this one down and move
        // on.
        else if ( pEntry->dwSize > stSI.dwPageSize )
        {
            if ( FALSE == cSym.SymFromAddr ( pEntry->dwFnAddr ,
                                             &dwDisp          ,
                                             pSym              ) )
            {
                ASSERT ( !"Symbol lookup failed!!" ) ;
#ifdef _WIN64
                VERBOSE1 ( _T ( "Sym lookup failed : 0x016I64X\n" ) ,
                           pEntry->dwFnAddr                          ) ;
#else
                VERBOSE1 ( _T ( "Sym lookup failed : 0x08X\n" ) ,
                           (DWORD_PTR)pEntry->dwFnAddr            ) ;
#endif
                           
                free ( pByte ) ;
                cTempSWSFile.Delete ( ) ;
                fclose ( pFile ) ;
                return ( FALSE ) ;
            }
            
            char * pSymbol = pSym->Name ;
            if ( '_' == *pSymbol )
            {
                pSymbol++ ;
            }

        
            _ftprintf ( pFile , _T ( "%S\n" ) , pSymbol ) ;
            pEntry->dwFnAddr = 0 ;

            VERBOSE3 ( _T ( "Symbol size larger than page " )
                       _T ( "(punting): [%d] (%5d) %S\n"    ) ,
                       pEntry->dwSize                         ,
                       pEntry->dwExecCount                    ,
                       pSymbol                                 ) ;

            // Reset for the next page.
            dwPageRemaining = stSI.dwPageSize ;
        }
        else
        {
            // The entry is too big for this page so look to see if I
            // can poink a function with an execution count into this
            // page.
            LPSWSENTRY pBestFitOverall = NULL ;
            LPSWSENTRY pBestExecFit = NULL ;
            for ( DWORD dwFillerItem = 0                   ;
                  dwFillerItem < pTempHeader->dwEntryCount ;
                  dwFillerItem++                            )
            {
                LPSWSENTRY pFiller =
                                cTempSWSFile.GetIndex ( dwFillerItem ) ;
                if ( 0 != pFiller->dwFnAddr )
                {
                    if ( pFiller->dwSize < dwPageRemaining )
                    {
                        // The easy case.
                        if ( NULL == pBestFitOverall )
                        {
                            pBestFitOverall = pFiller ;
                            pBestExecFit = pFiller ;
                        }
                        else
                        {
                            if ( pFiller->dwSize >
                                   pBestFitOverall->dwSize  )
                            {
                                pBestFitOverall = pFiller ;
                            }
                            if ( 0 != pFiller->dwExecCount )
                            {
                                if ( pFiller->dwSize >
                                        pBestExecFit->dwSize )
                                {
                                    pBestExecFit = pFiller ;
                                }
                            }
                        }
                    }
                }
            }
            if ( NULL == pBestExecFit )
            {
                VERBOSE ( _T ( "Unable to pack page with " )
                          _T ( "executed function...\n"    ) ) ;
                if ( NULL != pBestFitOverall )
                {
                    pBestExecFit = pBestFitOverall ;
                }
                else
                {
                    // Just use the entry...
                    pBestExecFit = pEntry ;
                }
            }

            ASSERT ( NULL != pBestExecFit ) ;
            
            if ( FALSE == cSym.SymFromAddr ( pBestExecFit->dwFnAddr ,
                                             &dwDisp                ,
                                             pSym                    ))
            {
                ASSERT ( !"Symbol lookup failed!!" ) ;
#ifdef _WIN64
                VERBOSE1 ( _T ( "Sym lookup failed : 0x016I64X\n" ) ,
                           pBestExecFit->dwFnAddr                    ) ;
#else
                VERBOSE1 ( _T ( "Sym lookup failed : 0x08X\n" ) ,
                           (DWORD_PTR)pBestExecFit->dwFnAddr         ) ;
#endif
                           
                free ( pByte ) ;
                cTempSWSFile.Delete ( ) ;
                fclose ( pFile ) ;
                return ( FALSE ) ;
            }
            
            char * pSymbol = pSym->Name ;
            if ( '_' == *pSymbol )
            {
                pSymbol++ ;
            }
            

            _ftprintf ( pFile , _T ( "%S\n" ) , pSymbol ) ;
            dwPageRemaining -= pBestExecFit->dwSize ;
            pBestExecFit->dwFnAddr = 0 ;

            VERBOSE4 ( _T ("Page Remaining (%04d) (%04d) : (%5d) %S\n"),
                       dwPageRemaining                             ,
                       pEntry->dwSize                              ,
                       pEntry->dwExecCount                         ,
                       pSymbol                                      ) ;
                       
                       
            // Reset for the next page.
            dwPageRemaining = stSI.dwPageSize ;
            VERBOSE ( _T ( "Page packed...\n" ) ) ;
        }
    }
    while ( TRUE == bContinue ) ;

    free ( pByte ) ;
    
    // Done!
    cTempSWSFile.Delete ( ) ;
    fclose ( pFile ) ;

    return ( TRUE ) ;
}

