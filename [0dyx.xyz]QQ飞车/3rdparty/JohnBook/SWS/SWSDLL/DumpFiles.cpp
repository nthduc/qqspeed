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
//////////////////////////////////////////////////////////////////////*/
BOOL SWSDLL_DLLINTERFACE __stdcall
                                DumpFiles ( LPCTSTR   szModule        ,
                                            PFNOUTPUT pfnOutput       ,
                                            PFNOUTPUT pfnVerboseOutput )
{
    VERBOSE ( "DumpFiles called\n" ) ;
    ASSERT ( FALSE == IsBadCodePtr ( (FARPROC)pfnOutput ) ) ;
    if ( TRUE == IsBadCodePtr ( (FARPROC)pfnOutput ) )
    {
        VERBOSE ( _T ( "Invalid output parameter\n" ) ) ;
        return ( FALSE ) ;
    }
    // The first thing to do is to figure out if the file extension is
    // XXX.#.SWS.  In that case, I will use that particular SWS file
    // to report the function call counts.

    TCHAR szSWSFile[ MAX_PATH ] ;
    TCHAR szModuleName[ MAX_PATH ] ;
    TCHAR szTemp[ MAX_PATH ] ;
    

    _tcscpy ( szTemp , szModule ) ;

    TCHAR * pExt = _tcsrchr ( szTemp , _T ( '.' ) ) ;
    if ( NULL == pExt )
    {
        // This is just a module name.  Slap on the .SWS extension.
        _tcscpy ( szSWSFile , szModule ) ;
        _tcscat ( szSWSFile , EXT_SWSFILE ) ;
        
        _tcscpy ( szModuleName , szModule ) ;
    }
    else
    {
        // There's an extension.  If it's not .SWS strip it off and
        // slap on the .SWS extension.  This is the case where someone
        // wants to dump FOO.EXE.
        if ( 0 != _tcsicmp ( EXT_SWSFILE , pExt ) )
        {
            *pExt = _T ( '\0' ) ;
            _tcscpy ( szSWSFile , szTemp ) ;
            _tcscat ( szSWSFile , EXT_SWSFILE ) ;
            
            _tcscpy ( szModuleName , szTemp ) ;
        }
        else
        {
            _tcscpy ( szSWSFile , szTemp ) ;
        }
    }

    VERBOSE1 ( _T ( "Processing file -> %s\n" ) , szSWSFile ) ;

    // Try and open the two file.
    CSWSFile cSWS ;

    if ( FALSE == cSWS.Open ( szSWSFile ) )
    {
        ASSERT ( !"Unable to open SWS file" ) ;
        VERBOSE1 ( _T ( "Unable to open %s\n" ) , szSWSFile ) ;
        return ( FALSE ) ;
    }

    // Verify these file pairs match.
    DWORD   dwSWSLinkTime ;
    DWORD   dwSWSEntryCount ;
    DWORD64 dwSWSLoadAddr ;
    DWORD   dwSWSImageSize ;
    DWORD   dwFlags ;
    TCHAR   szCoreMod[ MAX_PATH ] ;

    if ( FALSE == cSWS.GetHeaderData ( dwSWSLinkTime   ,
                                       dwSWSEntryCount ,
                                       dwSWSLoadAddr   ,
                                       dwSWSImageSize  ,
                                       dwFlags         ,
                                       szCoreMod         ) )
    {
        ASSERT ( !"Getting SWS header data failed!" ) ;
        VERBOSE ( _T ( "Unable to get SWS header data!\n" ) ) ;
        return ( FALSE ) ;
    }

    pfnOutput ( _T ( "Link time   : 0x%08X\n" ) , dwSWSLinkTime ) ;
    pfnOutput ( _T ( "Entry count : %d\n" ) , dwSWSEntryCount ) ;
#ifdef _WIN64
    pfnOutput ( _T ( "Image base  : 0x%016I64X\n" ) , dwSWSLoadAddr ) ;
#else
    pfnOutput ( _T ( "Image base  : 0x%08X\n" ) , dwSWSLoadAddr ) ;
#endif
    pfnOutput ( _T ( "Image size  : 0x%08X\n" ) , dwSWSImageSize ) ;
    pfnOutput ( _T ( "Module name : %s\n\n" ) , szCoreMod ) ;
    
    // Try and load up the module into the symbol engine.
    CSymbolEngine cSym ;
    
    DWORD64 dwBase = InitSymEngAndLoadModule ( &cSym            ,
                                               szCoreMod        ,
                                               pfnVerboseOutput  ) ;
    ASSERT ( 0 != dwBase ) ;
    if ( 0 == dwBase )
    {
        VERBOSE ( _T ( "Loading symbols failed!!\n" ) ) ;
        return ( FALSE ) ;
    }
    
    // I want all symbols to be decorated.
    cSym.SymSetOptions ( SYMOPT_PUBLICS_ONLY & ~SYMOPT_UNDNAME ) ;
    
    
    IMAGEHLP_MODULE64 stIM ;
    ZeroMemory ( &stIM , sizeof ( IMAGEHLP_MODULE64 ) ) ;
    stIM.SizeOfStruct = sizeof ( IMAGEHLP_MODULE64 ) ;
    
    if ( FALSE == cSym.SymGetModuleInfo64 ( dwBase , &stIM ) )
    {
        ASSERT ( !"Unable to get mod info!" ) ;
        VERBOSE ( _T ( "Unable to get mod info!" ) ) ;
        return ( FALSE ) ;
    }
    
    if ( dwSWSLinkTime != stIM.TimeDateStamp )
    {
        ASSERT ( !"symbols and SWS do not match!" ) ;
        VERBOSE(_T("The symbols and SWS link times do not match\n"));
        return ( FALSE ) ;
    }

    // Everything is kosher so rip through it.
    LPSWSENTRY pSWSEntry ;

    BYTE * pByte = (BYTE*)malloc ( 4096 ) ;
    ASSERT ( NULL != pByte ) ;
    
    SYMBOL_INFO * pSym = (PSYMBOL_INFO)pByte ;
    pSym->SizeOfStruct = sizeof ( SYMBOL_INFO ) ;
    pSym->MaxNameLen = 4096 - sizeof ( SYMBOL_INFO ) ;
    
    DWORD64 dwDisp ;

#ifdef _WIN64
    pfnOutput ( _T ( "Address            Count    Size  Name\n" ) ) ;
    pfnOutput ( _T ( "----------         -------- ----- ----\n" ) ) ;
#else
    pfnOutput ( _T ( "Address    Count    Size  Name\n" ) ) ;
    pfnOutput ( _T ( "---------- -------- ----- ----\n" ) ) ;
#endif

    for ( DWORD i = 0 ; i < dwSWSEntryCount ; i++ )
    {
        pSWSEntry = cSWS.GetIndex ( i ) ;
        

        BOOL bRet = cSym.SymFromAddr ( pSWSEntry->dwFnAddr ,
                                       &dwDisp             ,
                                       pSym                 ) ;
        ASSERT ( bRet ) ;
        ASSERT ( 0 == dwDisp ) ;
        if ( ( FALSE == bRet ) || ( 0 != dwDisp ) )
        {
#ifdef _WIN64
            VERBOSE1(_T("Failed to lookup symbol at : 0x%016I64X\n"),
                     pSWSEntry->dwFnAddr                            ) ;
#else
            VERBOSE1 ( _T("Failed to lookup symbol at : 0x%08X\n") ,
                       pSWSEntry->dwFnAddr                          ) ;
#endif
            free ( pByte ) ;
            return ( FALSE ) ;
        }

#ifdef _WIN64
        pfnOutput ( _T ( "0x%016I64X %8d %5d %S\n" ) ,
#else
        pfnOutput ( _T ( "0x%08X %8d %5d %S\n" )     ,
#endif
                    (DWORD_PTR)pSWSEntry->dwFnAddr   ,
                    pSWSEntry->dwExecCount           ,
                    pSWSEntry->dwSize                ,
                    pSym->Name                        ) ;
    }
    
    free ( pByte ) ;

    return ( TRUE ) ;
}
