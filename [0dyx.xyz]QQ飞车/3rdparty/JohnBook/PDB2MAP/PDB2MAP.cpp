/*----------------------------------------------------------------------
Debugging Applications for Microsoft .NET and Microsoft Windows
Copyright © 1997-2003 John Robbins -- All rights reserved.
----------------------------------------------------------------------*/

/*//////////////////////////////////////////////////////////////////////
// Necessary Includes
//////////////////////////////////////////////////////////////////////*/
#include "stdafx.h"
#include "CommandLine.h"
#include "Resource.h"
#include "Verbose.h"
#include "SymbolEngine.h"
#include "Array.h"

/*//////////////////////////////////////////////////////////////////////
// File Scope Constants & Defines
//////////////////////////////////////////////////////////////////////*/
// The major version number.
const UINT k_MAJORVERSION = 2 ;
// The minor version number.
const UINT k_MINORVERSION = 0 ;

#ifndef SYMOPT_PUBLICS_ONLY
#define SYMOPT_PUBLICS_ONLY             0x00004000
#endif

// The type of build determines the resources to use.
#ifdef _WIN64
#define IDS_HEADERINFO     IDS_HEADERINFO64
#define IDS_FUNCTIONHEADER IDS_FUNCTIONHEADER64
#define IDS_FUNCTFORMAT    IDS_FUNCTFORMAT64
#define IDS_SPACERS        IDS_SPACERS64
#define IDS_SRCHEADER      IDS_SRCHEADER64
#define IDS_LINEFORMAT     IDS_LINEFORMAT64
#define k_SRCLINESPERLINE  2
#else
#define IDS_HEADERINFO     IDS_HEADERINFO32
#define IDS_FUNCTIONHEADER IDS_FUNCTIONHEADER32
#define IDS_FUNCTFORMAT    IDS_FUNCTFORMAT32
#define IDS_SPACERS        IDS_SPACERS32
#define IDS_SRCHEADER      IDS_SRCHEADER32
#define IDS_LINEFORMAT     IDS_LINEFORMAT32
#define k_SRCLINESPERLINE  4
#endif

/*//////////////////////////////////////////////////////////////////////
// File Scope Typedefs and Classes
//////////////////////////////////////////////////////////////////////*/

// The typedef to ensure proper casting when dealing with addresses.
#ifdef _WIN64
typedef DWORD64 ADDRCAST ;
#else
typedef DWORD ADDRCAST ;
#endif

// A function information structure filled in when enumerating the
// public functions.
typedef struct tag_FUNCINFO
{
    // The function address.
    DWORD64 ulAddr ;
    // The size of the function.
    ULONG   ulSymSize ;
    // The name of the function.
    TCHAR   szName [ MAX_PATH ] ;
} FUNCINFO , * LPFUNCINFO ;

// An address/line pair for a source file.
typedef struct tag_ADDRLINE
{
    // The address for the line.
    DWORD64 ulAddr ;
    // The line number.
    ULONG   ulLine ;
} ADDRLINE , * LPADDRLINE ;


// The information about a single source file.
typedef struct tag_SRCINFO
{
    // The name of the source file.
    TCHAR szName[ MAX_PATH ] ;
    // The array of address line pairs.
    FASTARRAYINFO stAddrLineArray ;
} SRCINFO , * LPSRCINFO ;

/*//////////////////////////////////////////////////////////////////////
// File Scope Prototypes and Inlines
//////////////////////////////////////////////////////////////////////*/
// Displays the logo information.
void ShowLogo ( void ) ;
// Shows the help information.
void ShowHelp ( void ) ;
// Enumerates the functions in the symbol engine.
BOOL CALLBACK FunctionEnumeration ( PSTR    szName   ,
                                    DWORD64 ulAddr   ,
                                    ULONG   ulSize   ,
                                    PVOID   pContext  ) ;
// Compares function information structures.
int CompareFuncInfos ( LPFUNCINFO p1 , LPFUNCINFO p2 ) ;
// Compares source information structures.
int CompareSrcInfos ( LPSRCINFO p1 , LPSRCINFO p2 ) ;
// Compares address and line information.
int CompareAddrLine ( LPADDRLINE p1 , LPADDRLINE p2 ) ;
// Initializes a SRCINFO structure.
void InitializeSrcInfo ( LPSRCINFO pSrcInfo , TCHAR * szSrcName ) ;
// Adds a source range to the specific source info structure.
void AddSourceRange ( PIMAGEHLP_LINE64 pIL , LPSRCINFO pSrc ) ;

/*//////////////////////////////////////////////////////////////////////
// File Scope Globals
//////////////////////////////////////////////////////////////////////*/
// Holds the command line option settings.
static OPTIONS g_stOpts ;

// The symbol engine instance.
CSymbolEngine g_cSym ;

// The function information structure.
static FASTARRAYINFO g_aFuncArray =
{
    NULL                         ,  // pArray
    0                            ,  // ulTotalSlots
    0                            ,  // ulActiveSlots
    100                          ,  // ulReallocUnits
    sizeof ( FUNCINFO )          ,  // ulStructSize
    (PFNCOMPARE)CompareFuncInfos ,  // pfnCompare
    0                               // ulCurrIterator
} ;

// The source information structure.
static FASTARRAYINFO g_aSrcArray =
{
    NULL                         , // pArray
    0                            , // ulTotalSlots
    0                            , // ulActiveSlots
    100                          , // ulReallocUnits
    sizeof ( SRCINFO )           , // ulStructSize
    (PFNCOMPARE)CompareSrcInfos  , // pfnCompare
    0                              // ulCurrIterator
} ;

/*//////////////////////////////////////////////////////////////////////
// EXTERNALLY VISIBLE CODE STARTS HERE
//////////////////////////////////////////////////////////////////////*/

/*----------------------------------------------------------------------
FUNCTION        :   main
DISCUSSION      :
    The entry point for this code.
PARAMETERS      :
    argc - The number of commmand line arguments
    argv - The array of commmand line arguments
RETURNS         :
    0 - Life is good and everything processed fine.
    1 - There was a general problem.
    2 - The module either did not have symbols or did not have symbols
        that supported line information.
----------------------------------------------------------------------*/
int _tmain ( int argc , TCHAR * argv[] )
{
    // Turn on the memory tracking stuff.
#ifdef _DEBUG
    _CrtSetDbgFlag ( _CRTDBG_CHECK_ALWAYS_DF |
                        _CRTDBG_DELAY_FREE_MEM_DF |
                         _CRTDBG_LEAK_CHECK_DF      ) ;
#endif

    // A scratch string resource.
    CResString cRS ( GetModuleHandle ( NULL ) ) ;

    // The return value.  Positive thinking says the return will be
    // good.
    int iRetValue = 0 ;

    // Get the command line options.
    BOOL bRet = ResolveCommandLine ( g_stOpts , argc , argv ) ;

    if ( FALSE == g_stOpts.bSkipLogo )
    {
        ShowLogo ( ) ;
    }

    // Was there an invalid command line?
    if ( FALSE == bRet )
    {
        _tprintf ( cRS.LoadString ( IDS_INVALIDCMDLINE ) ) ;
    }

    if ( TRUE == g_stOpts.bShowHelp )
    {
        ShowHelp ( ) ;
        return ( 1 ) ;
    }

    if ( TRUE == g_stOpts.bVerbose )
    {
        SetVerbose ( TRUE ) ;
    }

    // Now that all the command line stuff is done, it's time to try
    // and load the module into the symbol engine.

    // The handle to the module use to load the symbols.
    HANDLE hFile = INVALID_HANDLE_VALUE ;
    // The output filename.
    TCHAR szOutputName [ MAX_PATH ] ;
    // The file output pointer.
    FILE * fileOutput = NULL ;
    try
    {
        // Tell the symbol engine to load the line information and
        // only public symbols
        SymSetOptions ( SYMOPT_LOAD_LINES | SYMOPT_PUBLICS_ONLY ) ;

        // Initialize the symbol engine class.
        VerboseOut ( cRS.LoadString ( IDS_VB_INITSYMENG ) ) ;
        if ( FALSE ==
                  g_cSym.SymInitialize ( (HANDLE)((INT_PTR)0xFEEDBEEF),
                                          NULL                        ,
                                          FALSE                      ))
        {
            iRetValue = 1 ;
            throw IDS_FAILEDSYMINIT ;
        }

        // Open the file so the symbol engine can load it.
        VerboseOut ( cRS.LoadString ( IDS_VB_OPENINGFILE ) ) ;
        hFile = CreateFile ( g_stOpts.szFile        ,
                             GENERIC_READ           ,
                             FILE_SHARE_READ        ,
                             NULL                   ,
                             OPEN_EXISTING          ,
                             FILE_ATTRIBUTE_NORMAL  ,
                             NULL                    ) ;
        if ( INVALID_HANDLE_VALUE == hFile )
        {
            WarningOut ( cRS.LoadString ( IDS_NOOPENFILE ) ,
                         g_stOpts.szFile                    ) ;
            iRetValue = 1 ;
            throw 0 ;
        }

        // Try and load the symbols
        VerboseOut ( cRS.LoadString ( IDS_VB_LOADINGSYMS ) ) ;
        DWORD64 dwBase = g_cSym.SymLoadModule64 ( hFile           ,
                                                  g_stOpts.szFile ,
                                                  NULL            ,
                                                  0               ,
                                                  0                ) ;
        if ( 0 == dwBase )
        {
            WarningOut ( cRS.LoadString ( IDS_NOLOADSYMBOLS ) ,
                         g_stOpts.szFile                       ) ;
            iRetValue = 1 ;
            throw 0 ;
        }

        // Double check that the symbols loaded are either C7 (CodeView)
        // or PDB.
        CImageHlp_Module cIM ;

        VerboseOut ( cRS.LoadString ( IDS_VB_GETMODULEINFO ) ) ;
        if ( FALSE == g_cSym.SymGetModuleInfo ( dwBase , &cIM ) )
        {
            WarningOut ( cRS.LoadString ( IDS_NOMODINFO ) ,
                         g_stOpts.szFile                   ) ;
            iRetValue = 2 ;
            throw 0 ;
        }
        if ( ( SymDia != cIM.SymType  ) &&
             ( SymCv != cIM.SymType   ) &&
             ( SymPdb != cIM.SymType  )    )
        {
            WarningOut ( cRS.LoadString ( IDS_WRONGSYMTYPE ) ,
                         g_stOpts.szFile                      ) ;
            iRetValue = 2 ;
            throw 0 ;
        }

        // The symbols are loaded so I can start the fun.

        // The first thing to do is to calculate the output filename.
        // Notice that I always write the P2M file to the current
        // directory.
        TCHAR * pMark = _tcsrchr ( g_stOpts.szFile , _T ( '\\' ) ) ;
        if ( NULL != pMark )
        {
            pMark++ ;
            _tcscpy ( szOutputName , pMark ) ;
        }
        else
        {
            _tcscpy ( szOutputName , g_stOpts.szFile ) ;
        }
        pMark = _tcsrchr ( szOutputName , _T ( '.' ) ) ;
        if ( NULL != pMark )
        {
            *pMark = _T ( '\0' ) ;
        }

        TCHAR szBaseName [ MAX_PATH ] ;
        _tcscpy ( szBaseName , szOutputName ) ;

        _tcscpy ( pMark , cRS.LoadString ( IDS_P2MEXT ) ) ;


        // Open the text file for writing.
        fileOutput = _tfopen ( szOutputName , _T ( "wt" ) ) ;
        if ( NULL == fileOutput )
        {
            WarningOut ( cRS.LoadString ( IDS_NOOPENOUTPUT ) ,
                         szOutputName                         ) ;
            iRetValue = 2 ;
            throw 0 ;
        }

        VerboseOut ( cRS.LoadString ( IDS_VB_WRITINGHEADER ) ) ;

        // Write out the header information.
        if ( 0 > _ftprintf ( fileOutput                              ,
                             cRS.LoadString ( IDS_HEADERINFO )       ,
                             szBaseName                              ,
                             cIM.TimeDateStamp                       ,
                             _tctime ( (time_t*)&cIM.TimeDateStamp ) ,
                             dwBase                                   ))
        {
            WarningOut ( cRS.LoadString ( IDS_WRITEFAILED ) ,
                         szOutputName                        ) ;
            iRetValue = 2 ;
            throw 0 ;
        }

        // Now it's time to do all the functions.
        VerboseOut ( cRS.LoadString ( IDS_VB_STARTENUM ) ) ;
        BOOL bEnumRet =
            g_cSym.SymEnumerateSymbols ( dwBase              ,
                                         FunctionEnumeration ,
                                         NULL                 ) ;
        if ( FALSE == bEnumRet )
        {
            iRetValue = 2 ;
            throw IDS_FUNCENUMFAILED ;
        }

        // Before getting too carried away, check that there were some
        // functions even there.
        if ( 0 == g_aFuncArray.ulActiveSlots )
        {
            iRetValue = 2 ;
            throw IDS_NOFUNCTIONINFO ;
        }

        // Sort the functions into address order.
        SortArrayInfo ( &g_aFuncArray ) ;
        
        // Now that I have all the publics, reset the symbol engine so
        // I can look up the symbol sizes.  When you set publics only,
        // none of the sizes are enumerated.
        SymSetOptions ( SYMOPT_LOAD_LINES ) ;

        // Write out the functions.
        VerboseOut ( cRS.LoadString ( IDS_FUNCTIONHEADER ) ) ;
        if ( 0 > _ftprintf ( fileOutput    ,
                             (LPCTSTR)cRS  ) )
        {
            WarningOut ( cRS.LoadString ( IDS_WRITEFAILED ) ,
                         szOutputName                        ) ;
            iRetValue = 2 ;
            throw 0 ;
        }

        CResString cFuncFMT ( GetModuleHandle ( NULL ) ) ;
        cFuncFMT.LoadString ( IDS_FUNCTFORMAT ) ;

        for ( LPFUNCINFO pFInfo =
                    (LPFUNCINFO)GetFirstArrayItem ( &g_aFuncArray )   ;
              NULL != pFInfo                                          ;
              pFInfo = (LPFUNCINFO)GetNextArrayItem ( &g_aFuncArray )  )
        {
            // The buffer where the undecorated name goes.
            TCHAR szUndecName[ MAX_PATH * 2 ] ;
            TCHAR * pSymName = pFInfo->szName ;

            if ( TRUE == g_stOpts.bUndecorate )
            {
                if ( 0 != UnDecorateSymbolNameW ( pFInfo->szName     ,
                                                  szUndecName        ,
                                                  MAX_PATH * 2       ,
                                                  UNDNAME_NAME_ONLY   ) )
                {
                    pSymName = szUndecName ;
                }
            }

            // Look up the symbol so I can get it's real size.  Keep in
            // mind that the size returned can be zero.
            SYMBOL_INFO stSymInfo ;
            ZeroMemory ( &stSymInfo , sizeof ( SYMBOL_INFO ) ) ;
            stSymInfo.SizeOfStruct = sizeof ( SYMBOL_INFO ) ;
            DWORD64 dwDispTemp ;
            
            if ( FALSE == g_cSym.SymFromAddr ( pFInfo->ulAddr ,
                                               &dwDispTemp    ,
                                               &stSymInfo      ) )
            {
                throw IDS_GETSIZEFAILED ;
            }
            pFInfo->ulSymSize = stSymInfo.Size;

            if ( TRUE == g_stOpts.bVerbose )
            {
                VerboseOut ( (LPCTSTR)cFuncFMT          ,
                             (ADDRCAST)pFInfo->ulAddr   ,
                             pFInfo->ulSymSize          ,
                             pSymName                    ) ;
            }
            if ( 0 > _ftprintf ( fileOutput                 ,
                                 (LPCTSTR)cFuncFMT          ,
                                 (ADDRCAST)pFInfo->ulAddr   ,
                                 pFInfo->ulSymSize          ,
                                 pSymName                    ) )
            {
                WarningOut ( cRS.LoadString ( IDS_WRITEFAILED ) ,
                             szOutputName                        ) ;
                iRetValue = 1 ;
                throw 0 ;
            }

            // Check to see if this function has a source file
            // associated with it.  Since there is no way to cleanly
            // enumerate the lines in a source file, I have to do it
            // the brute force way.
            IMAGEHLP_LINE64  stIL ;
            DWORD           dwDisp ;

            // The first step is to check if this address has source
            // with it.  If it does not have any, there is nothing to do
            // for that address.
            memset ( &stIL , NULL , sizeof ( IMAGEHLP_LINE64 ) ) ;
            stIL.SizeOfStruct = sizeof ( IMAGEHLP_LINE64 ) ;
            if ( TRUE ==
                 g_cSym.SymGetLineFromAddr ( pFInfo->ulAddr ,
                                             &dwDisp        ,
                                             &stIL           ) )
            {
                VerboseOut ( cRS.LoadString ( IDS_VBS_HASSRCLINE ) ,
                             stIL.LineNumber                       ,
                             stIL.FileName                          ) ;

                SRCINFO stSrc ;
                BSUAnsi2Wide ( stIL.FileName ,
                               stSrc.szName  ,
                               MAX_PATH       ) ;
                // Have I already seen this source file.
                LPSRCINFO pSrcInfo =
                            (LPSRCINFO)FindArrayItem ( &g_aSrcArray  ,
                                                      (BYTE*)&stSrc  ) ;
                if ( NULL == pSrcInfo )
                {
                    // This is a new file so initialize the full SRCINFO
                    // structure accordingly.
                    InitializeSrcInfo ( &stSrc , stSrc.szName ) ;

                    // Add this source range.
                    AddSourceRange ( &stIL , &stSrc ) ;

                    // Finally, add it to the array.
                    AddArrayItem ( &g_aSrcArray , (BYTE*)&stSrc ) ;

                    // Resort the source files.
                    SortArrayInfo ( &g_aSrcArray ) ;
                }
                else
                {
                    // I've already got this file in the array so check
                    // if the line number ranges overlap.  If they don't
                    // I will add this set of ranges to the source
                    // information structure.
                    ADDRLINE stAL ;
                    stAL.ulAddr = stIL.Address ;
                    stAL.ulLine = stIL.LineNumber ;

                    LPADDRLINE pAL = (LPADDRLINE)
                           FindArrayItem ( &pSrcInfo->stAddrLineArray ,
                                           (BYTE*)&stAL               );
                    if ( NULL == pAL )
                    {
                        VerboseOut ( cRS.LoadString (IDS_VBS_NEWRANGE));
                        // This is new range so add away!
                        AddSourceRange ( &stIL , pSrcInfo ) ;
                    }
                }
            }
        }

        // That's it for the function information.  I will go ahead and
        // free up the function memory as I no longer need it and
        // there's no sense wasting the working set.
        ClearAllArrayInfo ( &g_aFuncArray ) ;

        // Put a little space between the functions and the line number
        // information.
        if ( 0 > _ftprintf ( fileOutput                     ,
                             cRS.LoadString ( IDS_SPACERS )  ) )
        {
            WarningOut ( cRS.LoadString ( IDS_WRITEFAILED ) ,
                         szOutputName                        ) ;
            iRetValue = 1 ;
            throw 0 ;
        }


        // Loop through the source infos and write out the line
        // information.
        if ( NULL != g_aSrcArray.pArray )
        {
            for ( LPSRCINFO pSrcInfo = (LPSRCINFO)
                                GetFirstArrayItem ( &g_aSrcArray ) ;
                  NULL != pSrcInfo                                 ;
                  pSrcInfo = (LPSRCINFO)
                                GetNextArrayItem ( &g_aSrcArray )   )
            {

                if ( 0 > _ftprintf ( fileOutput                       ,
                                     cRS.LoadString ( IDS_SRCHEADER ) ,
                                     pSrcInfo->szName                 ))
                {
                    WarningOut ( cRS.LoadString ( IDS_WRITEFAILED ) ,
                                 szOutputName                        ) ;
                    iRetValue = 1 ;
                    throw 0 ;
                }
                // Now loop through the line information and plop them
                // out.
                cRS.LoadString ( IDS_LINEFORMAT ) ;
                int iCurr = 0 ;
                for ( LPADDRLINE pAddrLine = (LPADDRLINE)
                          GetFirstArrayItem(&pSrcInfo->stAddrLineArray);
                      NULL != pAddrLine                                ;
                      pAddrLine = (LPADDRLINE)
                          GetNextArrayItem(&pSrcInfo->stAddrLineArray ))
                {
                    if ( 0 == ( iCurr % k_SRCLINESPERLINE ) )
                    {
                        iCurr = 0 ;
                        if ( 0 >_ftprintf ( fileOutput , _T ( "\n" ) ) )
                        {
                            WarningOut (
                                    cRS.LoadString ( IDS_WRITEFAILED ) ,
                                         szOutputName                 );
                            iRetValue = 1 ;
                            throw 0 ;
                        }
                    }

                    iCurr++ ;

                    if ( 0 > _ftprintf ( fileOutput        ,
                                         (LPCTSTR)cRS      ,
                                         pAddrLine->ulLine ,
                                         pAddrLine->ulAddr  ) )
                    {
                            WarningOut (
                                    cRS.LoadString ( IDS_WRITEFAILED ) ,
                                         szOutputName                 );
                            iRetValue = 1 ;
                            throw 0 ;
                    }
                }

                // Get rid of the line array for this source file.
                ClearAllArrayInfo ( &pSrcInfo->stAddrLineArray ) ;
            }
        }
        else
        {
            WarningOut ( cRS.LoadString ( IDS_NOLINEINFO ) ) ;
        }

        // Get rid of the source file array.
        ClearAllArrayInfo ( &g_aSrcArray ) ;
    }
    catch ( int uiErr )
    {
        // Yikes!  Some sort of problem cropped up along the way.
        if ( 0 != uiErr )
        {
            // Tell the user.
            WarningOut ( cRS.LoadString ( uiErr ) ) ;
        }

    }

    // Cleanup any open and allocated stuff.
    if ( INVALID_HANDLE_VALUE != hFile )
    {
#ifdef _DEBUG
        BOOL bRet =
#endif
        CloseHandle ( hFile ) ;
#ifdef _DEBUG
        if ( FALSE == bRet )
        {
            MessageBox ( GetForegroundWindow ( )      ,
                         _T ( "PDB2MAP" )             ,
                         _T ( "CloseHandle failed!" ) ,
                         MB_OK                         ) ;
        }
#endif
    }
    if ( NULL != fileOutput )
    {
        fclose ( fileOutput ) ;
    }

    // Get rid of the func array if need be.
    if ( NULL != g_aFuncArray.pArray )
    {
        ClearAllArrayInfo ( &g_aFuncArray ) ;
    }

    // If there a any stray source information, get rid of them.
    if ( NULL != g_aSrcArray.pArray )
    {
        // Loop through all active source info structures and delete the
        // address/line pairs each has.
        for ( LPSRCINFO pSrcInfo =
                    (LPSRCINFO)GetFirstArrayItem ( &g_aSrcArray )     ;
              NULL != pSrcInfo                                        ;
              pSrcInfo = (LPSRCINFO)GetNextArrayItem ( &g_aSrcArray )  )
        {
            ClearAllArrayInfo ( &pSrcInfo->stAddrLineArray ) ;
        }

        ClearAllArrayInfo ( &g_aSrcArray ) ;
    }

    // All OK, Jumpmaster!
    return ( iRetValue ) ;
}

/*//////////////////////////////////////////////////////////////////////
                      FILE SCOPE CODE STARTS HERE
//////////////////////////////////////////////////////////////////////*/

/*----------------------------------------------------------------------
FUNCTION        :   ShowLogo
DISCUSSION      :
    Show's the program's logo
PARAMETERS      :
    None.
RETURNS         :
    None.
----------------------------------------------------------------------*/
void ShowLogo ( void )
{
    CResString cRS ( GetModuleHandle ( NULL ) ) ;
    _tprintf ( cRS.LoadString ( IDS_LOGO ) ,
               k_MAJORVERSION              ,
               k_MINORVERSION               ) ;
}

/*----------------------------------------------------------------------
FUNCTION        :
DISCUSSION      :
PARAMETERS      :
RETURNS         :
----------------------------------------------------------------------*/
void ShowHelp ( void )
{
    CResString cRS ( GetModuleHandle ( NULL ) ) ;
    for ( UINT i = IDS_USAGESTART ; i < IDS_USAGELAST ; i++ )
    {
        _tprintf ( cRS.LoadString ( i ) ) ;
    }
}

/*----------------------------------------------------------------------
FUNCTION        :   FunctionEnumeration
DISCUSSION      :
    The function enumeration callback used to enumerate all the
    functions in the loaded module and plop them into the global
    g_cFInfo array.
PARAMETERS      :
    szName   - The function name.
    ulAddr   - The function address.
    ulSize   - The size of the function.
    pContext - The user supplies context.
RETURNS         :
    Always TRUE to continue the enumeration.
----------------------------------------------------------------------*/
BOOL CALLBACK FunctionEnumeration ( PSTR    szName       ,
                                    DWORD64 ulAddr       ,
                                    ULONG   /*ulSize*/   ,
                                    PVOID   /*pContext*/  )
{
    // All I need to do here is to plop the individual functions into
    // the main vector.
    FUNCINFO stFunc ;
    stFunc.ulAddr = ulAddr ;
    // The symbol size is always zero on enumeration.
    stFunc.ulSymSize = 0 ;
    BSUAnsi2Wide ( szName , stFunc.szName , MAX_PATH ) ;
    AddArrayItem ( &g_aFuncArray , (BYTE*)&stFunc ) ;
    return ( TRUE ) ;
}


/*----------------------------------------------------------------------
FUNCTION        :   CompareFuncInfos
DISCUSSION      :
    Takes care of comparing function info structure.
PARAMETERS      :
    The two func infos to compare.
RETURNS         :
    Same as strcmp.
----------------------------------------------------------------------*/
int CompareFuncInfos ( LPFUNCINFO p1 , LPFUNCINFO p2 )
{
    if ( p1->ulAddr < p2->ulAddr )
    {
        return ( -1 ) ;
    }
    else if ( p1->ulAddr == p2->ulAddr )
    {
        return ( 0 ) ;
    }
    return ( 1 ) ;
}

/*----------------------------------------------------------------------
FUNCTION        :   CompareSrcInfos
DISCUSSION      :
    Takes care of comparing source info structure.
PARAMETERS      :
    The two source infos to compare.
RETURNS         :
    Same as strcmp.
----------------------------------------------------------------------*/
int CompareSrcInfos ( LPSRCINFO p1 , LPSRCINFO p2 )
{
    return ( _tcscmp ( p1->szName , p2->szName ) ) ;
}

/*----------------------------------------------------------------------
FUNCTION        :   CompareAddrLine
DISCUSSION      :
    Takes care of comparing address and line structures.
PARAMETERS      :
    The two addr lines to compare.
RETURNS         :
    Same as strcmp.
----------------------------------------------------------------------*/
int CompareAddrLine ( LPADDRLINE p1 , LPADDRLINE p2 )
{
    if ( p1->ulAddr < p2->ulAddr )
    {
        return ( -1 ) ;
    }
    else if ( p1->ulAddr == p2->ulAddr )
    {
        return ( 0 ) ;
    }
    return ( 1 ) ;
}

/*----------------------------------------------------------------------
FUNCTION        :   InitializeSrcInfo
DISCUSSION      :
    Initializes a SRCINFO structure.
PARAMETERS      :
    pSrcInfo  - The structure to initialize.
    szSrcName - The source file name.
RETURNS         :
    None.
----------------------------------------------------------------------*/
void InitializeSrcInfo ( LPSRCINFO pSrcInfo , TCHAR * szSrcName )
{
    _tcscpy ( pSrcInfo->szName , szSrcName ) ;

    pSrcInfo->stAddrLineArray.pArray = NULL ;
    pSrcInfo->stAddrLineArray.ulTotalSlots = 0 ;
    pSrcInfo->stAddrLineArray.ulActiveSlots = 0 ;
    pSrcInfo->stAddrLineArray.ulReallocUnits = 100 ;
    pSrcInfo->stAddrLineArray.ulStructSize = sizeof ( ADDRLINE ) ;
    pSrcInfo->stAddrLineArray.pfnCompare = (PFNCOMPARE)CompareAddrLine ;
    pSrcInfo->stAddrLineArray.ulCurrIterator = 0 ;
}

// Add's a source files line range to the source info structure.
void AddSourceRange ( PIMAGEHLP_LINE64 pIL , LPSRCINFO pSrc )
{
    CResString cRS ( GetModuleHandle ( NULL ) ) ;

    // The temporary line information.
    IMAGEHLP_LINE64 stIL = *pIL ;

    // My line information structure.
    ADDRLINE stAL ;

    // Move back to the beginning of this range adding all the ranges
    // before the one in pIL.
    while ( TRUE == g_cSym.SymGetLinePrev( &stIL ) )
    {
        // Add this one.
        stAL.ulAddr = stIL.Address ;
        stAL.ulLine = stIL.LineNumber ;
        AddArrayItem ( &pSrc->stAddrLineArray ,
                       (BYTE*)&stAL            ) ;
        VerboseOut(cRS.LoadString(IDS_VBS_ADDINGLINE) ,
                   stAL.ulLine                        );
    }

    // Add the source lines from pIL forward (inclusive).
    do
    {
        stAL.ulAddr = pIL->Address ;
        stAL.ulLine = pIL->LineNumber ;

        AddArrayItem ( &pSrc->stAddrLineArray ,
                       (BYTE*)&stAL            ) ;
        VerboseOut(cRS.LoadString(IDS_VBS_ADDINGLINE) ,
                   stAL.ulLine                        );
    } while ( TRUE == g_cSym.SymGetLineNext ( pIL ) );

    // Sort all the lines by address.
    SortArrayInfo ( &pSrc->stAddrLineArray ) ;
}
