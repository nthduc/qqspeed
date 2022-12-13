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
typedef struct tag_ENUMSYMCTX
{
   CSWSFile * pSWSFile ;
   PFNOUTPUT  pfnVerboseOutput ;
   int        iAddedCount ;
}  ENUMSYMCTX , * LPENUMSYMCTX ;

// The value I throw in RealGenerateInitialFiles with C++ Exception
// Handling in order to simplify the error handling.
#define RGIF_ERRORCODE 13

// The NT/W2K version of NULL
const LPCTSTR k_NTNULL = _T ( "nul" ) ;

/*//////////////////////////////////////////////////////////////////////
                           File Scope Globals
//////////////////////////////////////////////////////////////////////*/
// TRUE if the symbol engine has already been initialized.
static BOOL g_bSymEngInitialized = FALSE ;
// The symbol engine itself.
static CSymbolEngine g_cSym ;

// The list of partial symbol names to ignore.  Keep these ANSI as the
// strings returned by the symbol engine are ANSI.
#define IGNORE_CONTAINING_COUNT 6
char * g_szIgnoreContaining[ IGNORE_CONTAINING_COUNT ] =
{
    "_IMPORT_DESCRIPTOR" ,
    "_imp__"             ,
    "NULL_THUNK_DATA"    ,
    "_chkstk"            ,
    "__alloca_probe"     ,
    "penter"
} ;

/*//////////////////////////////////////////////////////////////////////
                          Function Prototypes
//////////////////////////////////////////////////////////////////////*/
BOOL __stdcall RealGenerateInitialFiles ( LPCTSTR   szModule        ,
                                          PFNOUTPUT pfnVerboseOutput ) ;

// The symbol enumeration function.
BOOL CALLBACK SymEnumSyms ( PSTR    szSymbolName    ,
                            DWORD64 ulSymbolAddress ,
                            ULONG   ulSymbolSize    ,
                            PVOID   pUserContext     ) ;

/*//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////*/
BOOL SWSDLL_DLLINTERFACE __stdcall
                   GenerateInitialFiles ( LPCTSTR   szModule  ,
                                          PFNOUTPUT pfnVerboseOutput )
{
    VERBOSE ( "GenerateInitialFiles called\n" ) ;

    // You might be wondering why I am using SEH here and making this
    // function simply a wrapper.
    // 1.  Since this function will be called inside the user's process
    //     I wanted to make sure that there was no way an exception
    //     in my stuff will percolate up.
    // 2.  Since SEH and C++ objects don't mix, I can get the benefit
    //     of SEH and still use objects in the other function.

    BOOL bRet = FALSE ;
    __try
    {
        bRet = RealGenerateInitialFiles ( szModule , pfnVerboseOutput );
    }
    __except ( EXCEPTION_EXECUTE_HANDLER )
    {
        SUPERASSERT ( !"Yikes! Handling an exception in "
                       "GenerateInitialFiles!"            ) ;
        VERBOSE ( _T ( "Exception in RealGenerateInitialFiles!\n" ) ) ;
        bRet = FALSE ;
    }
    VERBOSE1 ( _T ( "GenerateInitialFiles returning : %d\n" ) ,
               bRet                                            ) ;
    return ( bRet ) ;
}

BOOL __stdcall RealGenerateInitialFiles ( LPCTSTR   szModule        ,
                                          PFNOUTPUT pfnVerboseOutput )
{
    if ( FALSE == g_bSymEngInitialized )
    {
        VERBOSE ( _T ( "Initializing the symbol engine.\n" ) ) ;
        if ( FALSE == g_cSym.SymInitialize ( GetCurrentProcess ( ) ,
                                            NULL                  ,
                                            FALSE                  ) )
        {
            ASSERT ( !"Symbol engine initialization failed!" ) ;
            return ( FALSE ) ;
        }
        g_bSymEngInitialized = TRUE ;
    }

    // I want all symbols to be decorated and I want line info.
    g_cSym.SymSetOptions ( SYMOPT_PUBLICS_ONLY |
                           SYMOPT_LOAD_LINES   &
                           ~SYMOPT_UNDNAME      ) ;

    // Various resources I use.
    
    // The return value.
    BOOL bRet = FALSE ;
    // The file handle to the module.
    HANDLE hFile = INVALID_HANDLE_VALUE ;
    // The base address of the module.
    DWORD64 dwBase = 0 ;

    try
    {
        // The symbol engine is initialized so open the module and load
        // it into the symbol engine.
        VERBOSE ( _T ( "Opening the file\n" ) ) ;
        hFile = CreateFile ( szModule               ,
                             GENERIC_READ           ,
                             FILE_SHARE_READ        ,
                             NULL                   ,
                             OPEN_EXISTING          ,
                             FILE_ATTRIBUTE_NORMAL  ,
                             NULL                    ) ;
        ASSERT ( INVALID_HANDLE_VALUE != hFile ) ;
        if ( INVALID_HANDLE_VALUE == hFile )
        {
            VERBOSE1 ( _T ( "Unable to open : %s\n" ) , szModule ) ;
            throw  RGIF_ERRORCODE ;
        }
    
        // Convert the wide string to ANSI so I can call the
        // symbol engine function.
        size_t iCharLen = _tcslen ( szModule ) + 1 ;
        char * szAnsiMod = (char*)_alloca (  iCharLen ) ;
        Wide2Ansi ( szModule , szAnsiMod , (int)iCharLen ) ;

        VERBOSE ( _T ( "Loading the symbols" ) ) ;
        dwBase = g_cSym.SymLoadModule ( hFile       ,
                                        szAnsiMod   ,
                                        NULL        ,
                                        0           ,
                                        0            ) ;
        ASSERT ( 0 != dwBase ) ;
        if ( 0 == dwBase )
        {
            VERBOSE ( _T ( "Loading symbols failed!!\n" ) ) ;
            throw RGIF_ERRORCODE ;
        }

        // The symbols were loaded.  Now check that the symbols are
        // PDB or DIA.
        CImageHlp_Module cIM ;

        VERBOSE ( _T ( "Getting module info\n" ) ) ;
        if ( FALSE == g_cSym.SymGetModuleInfo ( dwBase , &cIM ) )
        {
            VERBOSE ( _T ( "Unable to get module info\n" ) ) ;
            throw RGIF_ERRORCODE ;
        }
        if ( ( SymPdb != cIM.SymType  ) &&
             ( SymDia != cIM.SymType  )   )
        {
            VERBOSE ( _T ( "Symbols not PDB or DIA\n" ) ) ;
            throw RGIF_ERRORCODE ;
        }

        // Figure out the output directory which will be the
        // current location for the module or the current
        // directory.
        
        TCHAR szOutDir [ MAX_PATH ] ;
        
        _tcscpy ( szOutDir , szModule ) ;
        TCHAR * pSlash = _tcsrchr ( szOutDir , _T ( '\\' ) ) ;
        if ( NULL != pSlash )
        {
            pSlash++ ;
            *pSlash = _T ( '\0' ) ;
        }
        else
        {
            _tgetcwd ( szOutDir , MAX_PATH ) ;
            _tcscat ( szOutDir , _T ( "\\" ) ) ;
        }
        
        // Get the module base name.
        TCHAR szBaseName [ MAX_PATH ] ;
        TCHAR * pChar = _tcsrchr ( szModule , _T ( '\\' ) ) ;
        if ( NULL == pChar )
        {
            _tcscpy ( szBaseName , szModule ) ;
        }
        else
        {
            _tcscpy ( szBaseName , pChar ) ;
        }
        pChar = _tcsrchr ( szBaseName , _T ( '.' ) ) ;
        if ( NULL != pChar )
        {
            *pChar = _T ( '\0' ) ;
        }
        
        // The file classes append the appropriate extensions so I don't
        // need to do it here.
        TCHAR szFinalName [ MAX_PATH ] ;
        _tcscpy ( szFinalName , szOutDir ) ;
        _tcscat ( szFinalName , szBaseName ) ;

        // Open up the SWS file.
        CSWSFile cSWS ;

        if ( FALSE == cSWS.Create ( szFinalName ) )
        {
            VERBOSE1 ( _T ( "Unable to open the SWS file : %s\n" ) ,
                       szFinalName                                  ) ;
            throw RGIF_ERRORCODE ;
        }

        // Get the link time information.
        IMAGEHLP_MODULE64 stIM ;
        ZeroMemory ( &stIM , sizeof ( IMAGEHLP_MODULE64 ) ) ;
        stIM.SizeOfStruct = sizeof ( IMAGEHLP_MODULE64 ) ;
        if ( FALSE == g_cSym.SymGetModuleInfo ( dwBase , &stIM ) )
        {
            VERBOSE ( _T ( "Unable to get the module info!\n" ) ) ;
            cSWS.Delete ( ) ;
            throw RGIF_ERRORCODE ;
        }
        
        // Set the initial file header data.
        TCHAR szName[ MAX_PATH ] ;
        TCHAR szExt [ MAX_PATH ] ;
        
        _tsplitpath ( szModule , NULL , NULL , szName , szExt ) ;
        _tcscat ( szName , szExt ) ;
        
        cSWS.SetHeaderData ( stIM.TimeDateStamp ,
                             0                  ,
                             stIM.BaseOfImage   ,
                             stIM.ImageSize     ,
                             0                  ,
                             szName              ) ;

        // Prep the stuff passed to the symbol enumerator.
        ENUMSYMCTX stCTX ;
        stCTX.pSWSFile = &cSWS ;
        stCTX.pfnVerboseOutput = pfnVerboseOutput ;
        stCTX.iAddedCount = 0 ;

        // Enumerate to my little heart's content.
        bRet = g_cSym.SymEnumerateSymbols ( dwBase      ,
                                            SymEnumSyms ,
                                            &stCTX       ) ;
        ASSERT ( TRUE == bRet ) ;
        
        VERBOSE1 ( _T ( "Symbol enumeration returned : %d\n" ) ,
                   bRet                                         ) ;

        if ( FALSE == bRet )
        {
            cSWS.Delete ( ) ;
            throw RGIF_ERRORCODE ;
        }
        
        // Turn off the SYMOPT_PUBLICS_ONLY option.  While that
        // flag works great for getting decorated names, it doesn't
        // return the symbol sizes.  Here I'll reset the symbol
        // options and update the symbol sizes for those found on
        // the enumeration.
        g_cSym.SymSetOptions ( SYMOPT_LOAD_LINES ) ;

        // The symbol structure.
        BYTE aByte[ 1024 ] ;
        PIMAGEHLP_SYMBOL64 pSym = (PIMAGEHLP_SYMBOL64)&aByte ;
        pSym->SizeOfStruct = sizeof ( IMAGEHLP_SYMBOL64 ) ;
        pSym->MaxNameLength = 1024 - sizeof ( IMAGEHLP_SYMBOL64 ) ;
        
        DWORD64 dwSymDisp ;
        
        // Loop through and get'em.
        for ( int i = 0 ; i < stCTX.iAddedCount ; i++ )
        {
            LPSWSENTRY pEntry = cSWS.GetIndex ( i ) ;
                
            bRet = g_cSym.SymGetSymFromAddr ( pEntry->dwFnAddr  ,
                                              &dwSymDisp        ,
                                              pSym               ) ;
            if ( FALSE == bRet )
            {
#ifdef _WIN64
                VERBOSE1( _T( "Failed getting size for 0x%016I64X\n" ) ,
                         (DWORD_PTR)pEntry->dwFnAddr                 );
#else
                VERBOSE1( _T( "Failed getting size for 0x%08X\n" ) ,
                         (DWORD_PTR)pEntry->dwFnAddr                 );
#endif
                throw RGIF_ERRORCODE ;
            }
            pEntry->dwSize = pSym->Size ;
#ifdef _WIN64
            VERBOSE2 ( _T ( "  Size for 0x%016I64X is %d\n" ) ,
                       (DWORD_PTR)pEntry->dwFnAddr        ,
                       pEntry->dwSize                          ) ;
#else
            VERBOSE2 ( _T ( "  Size for 0x%08X is %d\n" ) ,
                       (DWORD_PTR)pEntry->dwFnAddr        ,
                       pEntry->dwSize                          ) ;
#endif
        }
                   
        if ( TRUE == bRet )
        {
            // Close it.
            cSWS.Close ( ) ;
        }
        else
        {
            // Delete it.
            cSWS.Delete ( ) ;
        }

    }
    catch ( int e )
    {
        ASSERT ( !"Error in RealGenerateInitialFiles\n" ) ;
        if ( e == RGIF_ERRORCODE )
        {
            bRet = FALSE ;
        }
        else
        {
            throw e ;
        }
    }

    // Unload the module from the symbol engine to keep the memory
    // requirements down.
    if ( 0 != dwBase )
    {
        VERIFY ( g_cSym.SymUnloadModule ( dwBase ) ) ;
    }

    if ( INVALID_HANDLE_VALUE != hFile )
    {
        VERIFY ( CloseHandle ( hFile ) ) ;
    }
    return ( bRet ) ;
}

/*----------------------------------------------------------------------
FUNCTION        :   SymEnumSyms
DISCUSSION      :
    The symbol enumeration callback.  The only thing done here is that
the data is added to the SWS files.
PARAMETERS      :
    szSymbolName    - The symbol name.
    ulSymbolAddress - The symbol address.
    ulSymbolSize    - The size, in bytes, of the symbol.
    pUserContext    - The SWS file.
    
RETURNS         :
    TRUE  - Everything is cool.
    FALSE - There was a problem adding data to the file.
----------------------------------------------------------------------*/
BOOL CALLBACK SymEnumSyms ( PSTR    szSymbolName    ,
                            DWORD64 ulSymbolAddress ,
                            ULONG   ulSymbolSize    ,
                            PVOID   pUserContext     )
{
    LPENUMSYMCTX pCTX = (LPENUMSYMCTX)pUserContext ;

    CImageHlp_Line cLine ;
    DWORD dwDisp ;
    if ( FALSE == g_cSym.SymGetLineFromAddr ( ulSymbolAddress ,
                                              &dwDisp         ,
                                              &cLine            ) )
    {
        // If no source and line was found for this symbol, ignore
        // it.
        return ( TRUE ) ;
    }

    // Future Enhancements to ignoring particular symbols:
    // 1.  Add an ignore file list to search here.
    // 2.  Check if this address is in the modules code section.  This
    //     will avoid putting IAT symbols into the output files.
    
    // Is this symbol in the ignore containing list?
    for ( int i = 0 ; i < IGNORE_CONTAINING_COUNT ; i++ )
    {
        if ( NULL != strstr ( szSymbolName              ,
                              g_szIgnoreContaining[ i ]  ) )
        {
            // Drop out now.
            return ( TRUE ) ;
        }
    }
    
    if ( NULL != pCTX->pfnVerboseOutput )
    {
#ifdef _WIN64
        pCTX->pfnVerboseOutput(_T("  Adding Symbol : 0x%016I64X %S\n"),
#else
        pCTX->pfnVerboseOutput(_T("  Adding Symbol : 0x%08X %S\n" ) ,
#endif
                                (DWORD_PTR)ulSymbolAddress          ,
                                szSymbolName                          );
    }
    if ( FALSE == pCTX->pSWSFile->AddData ( ulSymbolAddress ,
                                            ulSymbolSize    ,
                                            0                ) )
    {
        ASSERT ( !"Adding to SWS file failed!" ) ;
        return ( FALSE ) ;
    }
    pCTX->iAddedCount++ ;
    return ( TRUE ) ;
}

