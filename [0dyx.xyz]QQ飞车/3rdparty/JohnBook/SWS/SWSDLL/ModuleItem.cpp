/*----------------------------------------------------------------------
Debugging Applications for Microsoft .NET and Microsoft Windows
Copyright © 1997-2003 John Robbins -- All rights reserved.
----------------------------------------------------------------------*/

/*//////////////////////////////////////////////////////////////////////
                              The Includes
//////////////////////////////////////////////////////////////////////*/
#include "stdafx.h"
#include "SWSDLL.h"
#include "ModuleItem.h"

/*//////////////////////////////////////////////////////////////////////
                     Defines, Constants, & Typedefs
//////////////////////////////////////////////////////////////////////*/
static const LPCTSTR k_SWSEXT = _T ( ".SWS" ) ;
static const int k_MAX_SWS_FILES = 100000 ;

/*//////////////////////////////////////////////////////////////////////
                           File Scope Globals
//////////////////////////////////////////////////////////////////////*/

/*//////////////////////////////////////////////////////////////////////
                          Function Prototypes
//////////////////////////////////////////////////////////////////////*/
// Takes care of the grunt work to get the appropriate SWS file for this
// run.
BOOL InitializeDiskFiles ( LPCTSTR   szModuleName ,
                           HINSTANCE hInstance    ,
                           LPTSTR    szSWSName     ) ;
// Returns TRUE if the specified file exists.
BOOL FileExists ( LPCTSTR szFile ) ;
// Returns the date time stamp of a module loaded into memory.
DWORD GetDateTimeStamp ( HINSTANCE hInst ) ;


/*//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////*/
// Initialize this class based on the particular address.
BOOL CModuleItem :: Initialize ( DWORD64 dwAddr )
{
    // There's lots of work to do here.

    // Get the module handle this address belongs to.
     MEMORY_BASIC_INFORMATION mbi ;
     VirtualQuery ( (LPCVOID)dwAddr ,
                    &mbi            ,
                    sizeof ( MEMORY_BASIC_INFORMATION ) ) ;

    HINSTANCE hInstModule = (HINSTANCE)mbi.AllocationBase ;

    // Armed with the base address, find the module name.
    TCHAR szModuleName[ MAX_PATH ] ;
    if ( 0 == GetModuleFileName ( hInstModule  ,
                                  szModuleName ,
                                  MAX_PATH      ) )
    {
        ASSERT ( !"GetModuleFileName failed!!" ) ;
        return ( FALSE ) ;
    }

    TCHAR szSWSName[ MAX_PATH ] ;
    if ( FALSE == InitializeDiskFiles ( szModuleName ,
                                        hInstModule  ,
                                        szSWSName     ) )
    {
        ASSERT ( !"Unable to initialize disk files!" ) ;
        return ( FALSE ) ;
    }

    // Allocate the SWS file class to handle the file stuff!
    m_pFile = new CSWSFile ( ) ;
    if ( FALSE == m_pFile->Open ( szSWSName ) )
    {
        ASSERT ( !"Unable to open SWS file!!" ) ;
        return ( FALSE ) ;
    }

    // Get the header information from the file.
    m_pcHeader = m_pFile->GetReadOnlyHeaderData ( ) ;

    // Check to see if this module has been relocated in memory.
    if ( (DWORD64)hInstModule != (DWORD64)m_pcHeader->dwLoadAddr )
    {
        OutputDebugString(_T("The following module needs rebasing: " ));
        OutputDebugString ( szModuleName ) ;
        OutputDebugString ( _T ( "\n" ) ) ;
        m_dwLoadAddr = (DWORD64)hInstModule ;
    }

    return ( TRUE ) ;
}

/*//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////*/
BOOL CModuleItem :: AddressIsInThisModule (DWORD64 dwAddr )
{
    ASSERT ( NULL != m_pFile ) ;
    if ( NULL == m_pFile )
    {
        return ( FALSE ) ;
    }

    // Fiddle the address so that it matches what the SWS file
    // has in it.
    DWORD64 dwConverted = ConvertRelocatedAddress ( dwAddr ) ;
    if ( ( dwConverted > m_pcHeader->dwLoadAddr     )   &&
         ( dwConverted < ( m_pcHeader->dwLoadAddr +
                           m_pcHeader->dwImageSize  ) )    )
    {
        return ( TRUE ) ;
    }
    return ( FALSE ) ;
}

/*//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////*/
BOOL CModuleItem :: IncrementAddressCount ( DWORD64 dwAddr )
{
    ASSERT ( TRUE == AddressIsInThisModule ( dwAddr ) ) ;
    ASSERT ( NULL != m_pFile ) ;
    if ( NULL == m_pFile )
    {
        return ( FALSE ) ;
    }

    // Make sure I convert the address to what is stored in the SWS
    // file.
    DWORD64 dwConverted = ConvertRelocatedAddress ( dwAddr ) ;

    // Hunt that address down.
    LPSWSENTRY pEntry = m_pFile->FindAddress ( dwConverted ) ;

    // If the hunt failed, it's probably one of those $E1-type symbols.
    if ( NULL == pEntry )
    {
        return ( FALSE ) ;
    }
    InterlockedIncrement ( (LPLONG)&pEntry->dwExecCount ) ;
    return ( TRUE ) ;
}


/*//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////*/
BOOL InitializeDiskFiles ( LPCTSTR   szModuleName ,
                           HINSTANCE hInstance    ,
                           LPTSTR    szSWSName     )
{
    // Check to see if I am supposed to plop all SWS files into the
    // same directory.
    TCHAR szOutDir [ MAX_PATH ] ;

    // The different pieces of the module filename.
    TCHAR szModDrive[ MAX_PATH ] ;
    TCHAR szModPath[ MAX_PATH ] ;
    TCHAR szModName[ MAX_PATH ] ;
    TCHAR szModExt[ MAX_PATH ] ;

    _tsplitpath ( szModuleName ,
                  szModDrive   ,
                  szModPath    ,
                  szModName    ,
                  szModExt      ) ;

    // Put the .SWS files in the same directory as the module.
    _tcscpy ( szOutDir , szModDrive ) ;
    _tcscat ( szOutDir , szModPath ) ;

    // Build up the initial SWS filename.
    TCHAR szBaseSWSFile[ MAX_PATH ] ;
    _tcscpy ( szBaseSWSFile , szOutDir ) ;
    _tcscat ( szBaseSWSFile , szModName ) ;
    _tcscat ( szBaseSWSFile , k_SWSEXT ) ;

    // Check to see if the .SWS exists for this module.
    if ( FALSE == FileExists ( szBaseSWSFile ) )
    {
        // The base file does not exist so I need to generate them
        // here.
        if ( FALSE == GenerateInitialFiles ( szModuleName , NULL ) )
        {
            ASSERT ( !"Unable to generate SWS and SDW files!" ) ;
            return ( FALSE ) ;
        }
    }
    else
    {
        // The SWS file exists so I need to quickly peek into it so I
        // can check that the data time stamp in the file match that
        // in the loaded module.  If they don't match, I regenerate
        // a new .SWS.  Yes, that means any data runs currently sitting
        // on the disk are now invalid.
        CSWSFile cTemp ;
        if ( FALSE == cTemp.Open ( szBaseSWSFile ) )
        {
            ASSERT ( !"Unable to peek at SWS file!!" ) ;
            return ( FALSE ) ;
        }
        // Get the DTS in memory.
        DWORD dwMemDTS = GetDateTimeStamp ( hInstance ) ;
        ASSERT ( 0 != dwMemDTS ) ;
        if ( 0 == dwMemDTS )
        {
            return ( FALSE ) ;
        }
        // Compare against the file.
        LPSWSFILEHEADER pHeader = cTemp.GetReadOnlyHeaderData ();
        if ( dwMemDTS != pHeader->dwLinkTime )
        {
            OutputDebugString ( _T ( "SWS file on on disk does not ")
                                _T ( "match that in memory for "   ) );
            OutputDebugString ( szModuleName ) ;
            OutputDebugString ( _T ( ". Regenerating....\n" ) ) ;
            // Close the file as I need to access it to write it!
            cTemp.Close ( ) ;
            if ( FALSE == GenerateInitialFiles ( szModuleName , NULL ) )
            {
                ASSERT ( !"Unable to generate SWS and SDW files!" ) ;
                return ( FALSE ) ;
            }
        }
    }

    // At this point, I know the base .SWS file exists.  Now I need to
    // figure out what name to give the data file (.#.SWS).
    _tcscpy ( szSWSName , szOutDir ) ;
    _tcscat ( szSWSName , szModName ) ;
    TCHAR * pExtSpot = szSWSName + _tcslen ( szSWSName ) ;
    for ( int iCurr = 1 ; iCurr < k_MAX_SWS_FILES ; iCurr++ )
    {
        wsprintf ( pExtSpot , _T ( ".%d.SWS" ) , iCurr ) ;
        if ( FALSE == FileExists ( szSWSName ) )
        {
            // Found a filename I can copy the base file to for this
            // run.
            if ( FALSE == CopyFile ( szBaseSWSFile ,
                                     szSWSName     ,
                                     TRUE           ) )
            {
                ASSERT ( !"Unable to copy file!!" ) ;
                return ( FALSE ) ;
            }
            else
            {
                // Cool!
                return ( TRUE ) ;
            }
        }
    }
    return ( FALSE ) ;
}

/*//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////*/
BOOL FileExists ( LPCTSTR szFile )
{
    if ( (DWORD)-1 == GetFileAttributes (  szFile ) )
    {
        return ( FALSE ) ;
    }
    return ( TRUE ) ;
}

/*//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////*/
// A useful macro.
#define MakePtr( cast , ptr , AddValue ) \
                            (cast)( (DWORD64)(ptr)+(DWORD)(AddValue))

DWORD GetDateTimeStamp ( HINSTANCE hModule )
{
    PIMAGE_DOS_HEADER pDOSHeader = (PIMAGE_DOS_HEADER)hModule ;

    // Is this the MZ header?
    if ( ( TRUE == IsBadReadPtr ( pDOSHeader                  ,
                                 sizeof ( IMAGE_DOS_HEADER )  ) ) ||
         ( IMAGE_DOS_SIGNATURE != pDOSHeader->e_magic           )   )
    {
        ASSERT ( !"Unable to read MZ header!" ) ;
        return ( 0 ) ;
    }

    // Get the PE header.
    PIMAGE_NT_HEADERS pNTHeader = MakePtr ( PIMAGE_NT_HEADERS       ,
                                            pDOSHeader              ,
                                            pDOSHeader->e_lfanew     ) ;

    // Is this a real PE image?
    if ( ( TRUE == IsBadReadPtr ( pNTHeader ,
                                  sizeof ( IMAGE_NT_HEADERS ) ) ) ||
         ( IMAGE_NT_SIGNATURE != pNTHeader->Signature           )   )
    {
        ASSERT ( !"Not able to read PE header" ) ;
        return ( 0 ) ;
    }

    return ( pNTHeader->FileHeader.TimeDateStamp ) ;

}
