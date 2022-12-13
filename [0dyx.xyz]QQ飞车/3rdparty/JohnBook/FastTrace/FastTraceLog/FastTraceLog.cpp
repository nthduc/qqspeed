/*----------------------------------------------------------------------
Debugging Applications for Microsoft .NET and Microsoft Windows
Copyright © 1997-2003 John Robbins -- All rights reserved.
----------------------------------------------------------------------*/
#include "stdafx.h"
#include "FastTrace.h"
#include "Resource.h"
#include "MappedLogFile.h"

/*//////////////////////////////////////////////////////////////////////
// File Scope Defines, Structures, and Typedefs
//////////////////////////////////////////////////////////////////////*/
#define LAST_HELP_ID IDS_HELP9
#define IDS_LOGO     IDS_HELP1

// The author of this file.
#ifdef SUPERASSERT_EMAIL
#undef SUPERASSERT_EMAIL
#define SUPERASSERT_EMAIL "john@wintellect.com"
#endif

// The parsed command line options.
typedef struct tag_CMDOPTS
{
    // Don't show the logo.
    BOOL bNoLogo ;
    // If true, szFile specifies an individual file to dump.
    // If false, szFile contains the first part of the string for
    // the filenames to look for.
    BOOL bDump ;
    // The module or partial file to concatenate.
    TCHAR szFile[ MAX_PATH ] ;
} CMDOPTS ;

// The array type I enumerate the filenames into.
typedef JSimpleArray <JString> FILENAMEARRAY ;

// The array of mapped files.
typedef JSimpleArray <CMappedLogFile *> MAPPEDFILEARRAY ;

/*//////////////////////////////////////////////////////////////////////
// File Scope Globals
//////////////////////////////////////////////////////////////////////*/
// The secret "-v" verbose switch.
static BOOL g_bVerbose = FALSE ;

/*//////////////////////////////////////////////////////////////////////
// File Scope Function Prototypes
//////////////////////////////////////////////////////////////////////*/
BOOL ParseCommandLine ( int argc , TCHAR * argv[] , CMDOPTS & stOpts ) ;
void ShowHelp ( void ) ;
BOOL EnumerateFilenames ( LPCTSTR szBaseName , FILENAMEARRAY & aFiles );
BOOL MergeFiles ( LPCTSTR szBaseName , MAPPEDFILEARRAY & aMapped ) ;

int _tmain ( int argc , _TCHAR* argv[] )
{
    // The parsed up command line options.
    CMDOPTS stOpts ;
    // A resource string wrapper.
    CResString cRes ;
    
    // Grind up the command line.
    BOOL bRet = ParseCommandLine ( argc , argv   , stOpts ) ;
    if ( FALSE == bRet )
    {
        ShowHelp ( ) ;
        return ( 1 ) ;
    }
    if ( FALSE == stOpts.bNoLogo )
    {
        cRes.LoadString ( IDS_LOGO ) ;
        _tprintf ( (LPCTSTR)cRes ) ;
    }
    
    if ( TRUE == stOpts.bDump )
    {
        CMappedLogFile cFile ;
        
        // Open the file.
        eOpenRes eRet = cFile.Open ( stOpts.szFile ) ;
        ASSERT ( OPENFAIL_ALL_OK == eRet ) ;
        if ( OPENFAIL_ALL_OK == eRet )
        {
            // Get the first string item.
            LPFTDATA pData = cFile.GetFirstString ( ) ;
            while ( NULL != pData )
            {
                if ( ( 0 == pData->stTime.dwHighDateTime ) &&
                     ( 0 == pData->stTime.dwLowDateTime  )   )
                {
                    _tprintf ( _T ( "[%d]\n" ) , pData->lSeqNum ) ;
                }
                else
                {
                    FILETIME stLocalFileTime ;
                    FileTimeToLocalFileTime ( &pData->stTime   ,
                                              &stLocalFileTime  ) ;

                    // Convert the file time to a readable system time.
                    SYSTEMTIME stSysTime ;
                    FileTimeToSystemTime ( &stLocalFileTime ,
                                           &stSysTime        ) ;
                
                    // Format the date to look appropriate to the user.
                    TCHAR szDate[ 20 ] ;
                    GetDateFormat ( LOCALE_USER_DEFAULT ,
                                    DATE_SHORTDATE      ,
                                    &stSysTime          ,
                                    NULL                ,
                                    szDate              ,
                                    sizeof (szDate) / sizeof (TCHAR) ) ;
                                
                    // Spit out the date and time.
                    _tprintf ( _T ( "[%d %s %02d:%02d:%02d:%03d]\n" ) ,
                               pData->lSeqNum                         ,
                               szDate                                 ,
                               stSysTime.wHour                        ,
                               stSysTime.wMinute                      ,
                               stSysTime.wSecond                      ,
                               stSysTime.wMilliseconds                );
                }
                // Do the data.
                _tprintf ( _T ( "%s\n" ) , pData->szTrace ) ;
                // Get the next string.
                pData = cFile.GetNextString ( ) ;
            }
            cFile.Close ( ) ;
        }
        else
        {
            VERIFY ( cRes.LoadString ( IDS_INVALIDPARAM + eRet ) ) ;
            _tprintf ( (LPCTSTR)cRes ) ;
            return ( 2 ) ;
        }
    }
    else
    {
        // First step is to enumerate the filenames.
        FILENAMEARRAY aFiles ;
        BOOL bRet = EnumerateFilenames ( stOpts.szFile , aFiles ) ;
        if ( TRUE == bRet )
        {
            // Spin through and open the files.
            BOOL bAllGood = TRUE ;
            CMappedLogFile * pFile = NULL ;
            MAPPEDFILEARRAY aMapped ;
            for ( int i = 0 ; i < aFiles.GetSize ( ) ; i++ )
            {
                pFile = new CMappedLogFile ( ) ;
                eOpenRes eRet = pFile->Open ( aFiles [ i ] ) ;
                ASSERT ( OPENFAIL_ALL_OK == eRet ) ;
                if ( OPENFAIL_ALL_OK == eRet )
                {
                    // Check to see that this file actually has data
                    // in it as it's possible a snap occurred and
                    // nothing had been logged on that thread.
                    if ( pFile->StringCount ( ) > 0 )
                    {
                        aMapped.Add ( pFile ) ;
                    }
                    else
                    {
                        pFile->Close ( ) ;
                        delete pFile ;
                    }
                }
                else
                {
                    bAllGood = FALSE ;
                    break ;
                }
            }
            if ( TRUE == bAllGood )
            {
                if ( aMapped.GetSize ( ) > 0 )
                {
                    MergeFiles ( stOpts.szFile , aMapped ) ;
                }
                else
                {
                    VERIFY ( cRes.LoadString ( IDS_NODATATODO ) ) ;
                    _tprintf ( (LPCTSTR)cRes ) ;
                }
            }
            // Clean up the allocated array.
            for ( int j = 0 ; j < aMapped.GetSize ( ) ; j++ )
            {
                aMapped[ j ]->Close ( ) ;
                delete aMapped[ j ] ;
            }
        }
        else
        {
            VERIFY ( cRes.LoadString ( IDS_NOFILESFOUNT ) ) ;
            _tprintf ( (LPCTSTR)cRes , stOpts.szFile ) ;
        }
    }
    
	return ( 0 ) ;
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
                    case _T ( 'C' ) :
                        stOpts.bDump = FALSE ;
                        break ;
                    case _T ( 'D' ) :
                        stOpts.bDump = TRUE ;
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
                // If the filename portion has already been filled in,
                // then error out.
                if ( _T ( '\0' ) != stOpts.szFile[ 0 ] )
                {
                    return ( FALSE ) ;
                }
                _tcscpy ( stOpts.szFile , szString ) ;
            }
            break ;
        }
    }

    // Make sure the filename portion exists.
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

/*----------------------------------------------------------------------
FUNCTION        :   EnumerateFilenames
DISCUSSION      :
    Enumerates the filenames into an array.
PARAMETERS      :
    szBaseName - The base portion of the files to look for.  "_*.FTL" is
                 appended to this in order to find the files.
    aFiles     - The array to store the filenames into.
RETURNS         :
    TRUE  - Life is good.
    FALSE - Either no files were found or there was a serious problem.
----------------------------------------------------------------------*/
BOOL EnumerateFilenames ( LPCTSTR szBaseName , FILENAMEARRAY & aFiles )
{
    BOOL bRet = FALSE ;
    TCHAR szSearch[ MAX_PATH ] ;
    
    wsprintf ( szSearch          ,
               _T ( "%s_*.FTL" ) ,
               szBaseName         ) ;

    WIN32_FIND_DATA stFindData ;
    ZeroMemory ( &stFindData , sizeof ( WIN32_FIND_DATA ) ) ;
    
    HANDLE hFind = FindFirstFile ( szSearch , &stFindData ) ;
    if ( INVALID_HANDLE_VALUE != hFind )
    {
        do
        {
            aFiles.Add ( stFindData.cFileName ) ;
        } while ( FALSE != FindNextFile ( hFind , &stFindData ) ) ;
        VERIFY ( FindClose ( hFind ) ) ;
    }
    if ( aFiles.GetSize ( ) > 0 )
    {
        bRet = TRUE ;
    }
    else
    {
        bRet = FALSE ;
    }
    return ( bRet ) ;
}


/*----------------------------------------------------------------------
FUNCTION        :   MergeFiles
DISCUSSION      :
    Merges all the separate files into a single text file.
PARAMETERS      :
    szBaseName - The base portion of the filename.  ".TXT" will be
                 appended to create the output file.
    aMapped    - The array of mapped files.
RETURNS         :
    TRUE  - Life is good.
    FALSE - There was a serious problem.
----------------------------------------------------------------------*/
BOOL MergeFiles ( LPCTSTR szBaseName , MAPPEDFILEARRAY & aMapped )
{
    BOOL bFinalReturnVal = TRUE ;
    BOOL bDone = FALSE ;
    TCHAR szOutFile[ MAX_PATH ] ;
    
    wsprintf ( szOutFile , _T ( "%s.TXT" ) , szBaseName ) ;
    
    FILE * pOut = _tfopen ( szOutFile , _T ( "wt" ) ) ;
    ASSERT ( NULL != pOut ) ;
    if ( NULL == pOut )
    {
        return ( FALSE ) ;
    }
    
    while ( FALSE == bDone )
    {
        // The index of the file with the earliest time in it.
        int iEarliest = 0 ;
        // The current looping index.
        int iCurrIndex = 1 ;
        
        // The current pointer to the earliest time.
        LPFTDATA pEarly = aMapped[ iEarliest ]->PeekCurrentString ( ) ;
        ASSERT ( NULL != pEarly ) ;
        // Loop through the file looking at active time in each and find
        // the earliest time.
        for ( ; iCurrIndex < aMapped.GetSize ( ) ; iCurrIndex++ )
        {
            LPFTDATA pCurr = aMapped[iCurrIndex]->PeekCurrentString ( );

            // Was this string written early in the sequence?
            if ( pCurr->lSeqNum < pEarly->lSeqNum )
            {
                // Yes it was.  Reset the early pointer.
                pEarly = pCurr ;
                // Reset the index.
                iEarliest = iCurrIndex ;
            }
        }
        // I've found the earliest entry across all the top entries.
        // Write out this entry.
        
        int iRet1 = 0 ;
        LONG lTID = aMapped[iEarliest]->ThreadID ( ) ;

        // If the time is 0, timing was not calculated.
        if ( ( 0 == pEarly->stTime.dwHighDateTime ) &&
             ( 0 == pEarly->stTime.dwLowDateTime  )    )
        {
            iRet1 = _ftprintf ( pOut                   ,
                                _T ( "[0x%04X %d]\n" ) ,
                                lTID                   ,
                                pEarly->lSeqNum         ) ;
        }
        else
        {
            // The times are UTC time so I need to convert it to local
            // time.
            FILETIME stLocalFileTime ;
            FileTimeToLocalFileTime ( &pEarly->stTime ,
                                      &stLocalFileTime ) ;
 
            SYSTEMTIME stSysTime ;
            FileTimeToSystemTime ( &stLocalFileTime , &stSysTime ) ;
        
            // Format the date to look appropriate to the user.
            TCHAR szDate[ 20 ] ;
            GetDateFormat ( LOCALE_USER_DEFAULT ,
                            DATE_SHORTDATE      ,
                            &stSysTime          ,
                            NULL                ,
                            szDate              ,
                            sizeof ( szDate ) / sizeof ( TCHAR ) ) ;

            // Spit out the date and time and TID.
            iRet1 = _ftprintf ( pOut                                  ,
                        _T ( "[0x%04X %d %s %02d:%02d:%02d:%03d]\n" ) ,
                                lTID                                  ,
                                pEarly->lSeqNum                       ,
                                szDate                                ,
                                stSysTime.wHour                       ,
                                stSysTime.wMinute                     ,
                                stSysTime.wSecond                     ,
                                stSysTime.wMilliseconds               );
        }
        int iRet2 = _ftprintf ( pOut            ,
                                _T ( "%s\n" )   ,
                                pEarly->szTrace  ) ;
        ASSERT ( ( iRet1 > 0 ) && ( iRet2 > 0 ) ) ;
        if ( ( iRet1 > 0 ) && ( iRet2 > 0 ) )
        {
            // I've processed that item in this particular file so bump
            // up to the next one.
            LPFTDATA pBump = aMapped[ iEarliest ]->GetNextString ( ) ;
            // If the return value is NULL, I've reached the end of this
            // file so remove it from the array.  This will keep the
            // array contiguous.
            if ( NULL == pBump )
            {
                // Close the file first.
                aMapped[ iEarliest  ]->Close ( ) ;
                // Delete the memory for the file class.
                delete aMapped[ iEarliest ] ;
                // Remove it from the array.
                aMapped.RemoveAt ( iEarliest ) ;
            }
            // If the file array has nothing in it, I'm done.
            if ( 0 == aMapped.GetSize ( ) )
            {
                bDone = TRUE ;
            }
        }
        else
        {
            bDone = FALSE ;
            bFinalReturnVal = FALSE ;
        }
    }
    if ( NULL != pOut )
    {
        fclose ( pOut ) ;
    }
    return ( bFinalReturnVal ) ;
}