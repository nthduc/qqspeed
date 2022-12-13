/*----------------------------------------------------------------------
Debugging Applications for Microsoft .NET and Microsoft Windows
Copyright © 1997-2003 John Robbins -- All rights reserved.
----------------------------------------------------------------------*/
#include "stdafx.h"
#include "SWSFile.h"

// Why is this not an API function?
#define GetFilePointer(hFile) \
                    SetFilePointer ( hFile , 0 , NULL , FILE_CURRENT )
                    
                    
// The comparison function for SWSENTRY elements.
static int __cdecl CompareAddr ( const void * pIn1 ,
                                 const void * pIn2  )
{
    LPSWSENTRY p1 = (LPSWSENTRY)pIn1 ;
    LPSWSENTRY p2 = (LPSWSENTRY)pIn2 ;

    if ( p1->dwFnAddr < p2->dwFnAddr )
    {
        return ( -1 ) ;
    }
    else if ( p1->dwFnAddr == p2->dwFnAddr )
    {
        return ( 0 ) ;
    }
    return ( 1 ) ;
}


// Do the TWS file compare.  Note that since I want the file sorted in
// reverse order, this function returns "backwards!"
static int __cdecl CompareExecCount ( const void * pIn1 ,
                                      const void * pIn2  )
{
    LPSWSENTRY p1 = (LPSWSENTRY)pIn1 ;
    LPSWSENTRY p2 = (LPSWSENTRY)pIn2 ;

    if ( p1->dwExecCount < p2->dwExecCount )
    {
        return ( 1 ) ;
    }
    else if ( p1->dwExecCount == p2->dwExecCount )
    {
        return ( 0 ) ;
    }
    return ( -1 ) ;
}

////////////////////////////////////////////////////////////////////////
/*//////////////////////////////////////////////////////////////////////
                             CSWSFile Class
//////////////////////////////////////////////////////////////////////*/
////////////////////////////////////////////////////////////////////////

/*//////////////////////////////////////////////////////////////////////
                  Public Construction and Destruction
//////////////////////////////////////////////////////////////////////*/
CSWSFile :: CSWSFile ( void )
{
    m_bCreated = FALSE ;
    m_hFile = INVALID_HANDLE_VALUE ;
    m_hMapped = INVALID_HANDLE_VALUE ;
    m_pMappedMem = NULL ;

    m_pBuffer = NULL ;
    m_uiTotalElems = 0 ;
    m_uiNumElems = 0 ;

    m_szFileName[ 0 ] = _T ( '\0' ) ;
}

CSWSFile :: ~CSWSFile ( void )
{
    Close ( ) ;
}

/*//////////////////////////////////////////////////////////////////////
                        Public Open Methods
                   (These are only for reading data!)
//////////////////////////////////////////////////////////////////////*/
// Opens the file for reading and updating.
BOOL CSWSFile :: Open ( LPCTSTR szFileName )
{
    // Go through and do all the junk necessary to map the file into
    // memory.
    _tcscpy ( m_szFileName , szFileName ) ;

    // Get the file opened up.
    m_hFile = CreateFile ( szFileName                   ,
                           GENERIC_READ | GENERIC_WRITE ,
                           FILE_SHARE_READ              ,
                           NULL                         ,
                           OPEN_EXISTING                ,
                           FILE_ATTRIBUTE_NORMAL        ,
                           NULL                          ) ;
    if ( INVALID_HANDLE_VALUE == m_hFile )
    {
        return ( FALSE ) ;
    }

    // Map it in.
    m_hMapped = CreateFileMapping ( m_hFile         ,
                                    NULL            ,
                                    PAGE_READWRITE  ,
                                    0               ,
                                    0               ,
                                    NULL             ) ;
    ASSERT ( NULL != m_hMapped ) ;
    if ( NULL == m_hMapped )
    {
        VERIFY ( CloseHandle ( m_hFile ) ) ;
        return ( FALSE ) ;
    }

    // Create the view.
    m_pMappedMem = (LPBYTE)MapViewOfFile ( m_hMapped        ,
                                           FILE_MAP_WRITE   ,
                                           0                ,
                                           0                ,
                                           0                 ) ;

    ASSERT ( NULL != m_pMappedMem ) ;
    if ( NULL == m_pMappedMem )
    {
        VERIFY ( CloseHandle ( m_hFile ) ) ;
        VERIFY ( CloseHandle ( m_hMapped ) ) ;
        return ( FALSE ) ;
    }

    m_bCreated = FALSE ;
    
    // Look at the header to determine the file type.
    LPSWSFILEHEADER pHdr = (LPSWSFILEHEADER)m_pMappedMem ;
    ASSERT ( SIG_SWSFILE == pHdr->dwSignature ) ;

    if ( SIG_SWSFILE != pHdr->dwSignature )
    {
        FreeResources ( ) ;
        return ( FALSE ) ;
    }
    
    m_uiNumElems = pHdr->dwEntryCount ;
    
    return ( TRUE ) ;
}

// Returns the header data.
BOOL CSWSFile :: GetHeaderData ( DWORD &   dwLinkTime   ,
                                 DWORD &   dwEntryCount ,
                                 DWORD64 & dwLoadAddr   ,
                                 DWORD &   dwImageSize  ,
                                 DWORD &   dwFlags      ,
                                 TCHAR *   szModuleName  )
{
    ASSERT ( INVALID_HANDLE_VALUE != m_hFile ) ;

    if ( TRUE == m_bCreated )
    {
        if ( INVALID_SET_FILE_POINTER == SetFilePointer ( m_hFile    ,
                                                          0          ,
                                                          0          ,
                                                          FILE_BEGIN  ) )
        {
            ASSERT ( !"Unable to set the file pointer!!" ) ;
            return ( FALSE ) ;
        }

        SWSFILEHEADER stFH ;
        DWORD dwRead ;

        if ( FALSE == ReadFile ( m_hFile                        ,
                                 &stFH                          ,
                                 sizeof ( SWSFILEHEADER ) ,
                                 &dwRead                        ,
                                 NULL                            ) )
        {
            ASSERT ( !"Unable to read header!!" ) ;
            return ( FALSE ) ;
        }

        // Check the sig.
        if ( SIG_SWSFILE != stFH.dwSignature )
        {
            ASSERT ( !"File header does not match!!" ) ;
            return ( FALSE ) ;
        }

        dwLinkTime = stFH.dwLinkTime ;
        dwEntryCount = stFH.dwEntryCount ;
        dwLoadAddr = stFH.dwLoadAddr ;
        dwImageSize = stFH.dwImageSize ;
        dwFlags = stFH.dwFlags ;
        _tcscpy ( szModuleName , stFH.szModuleName ) ;
    }
    else
    {
        ASSERT ( NULL != m_pMappedMem ) ;
        if ( NULL == m_pMappedMem )
        {
            ASSERT ( !"File not properly mapped!!" ) ;
            return ( FALSE ) ;
        }

        // Read the mapped memory for the answers....
        LPSWSFILEHEADER pstFH = (LPSWSFILEHEADER)m_pMappedMem ;

        if ( SIG_SWSFILE != pstFH->dwSignature )
        {
            ASSERT ( !"File header does not match!!" ) ;
            return ( FALSE ) ;
        }
        dwLinkTime = pstFH->dwLinkTime ;
        dwEntryCount = pstFH->dwEntryCount ;
        dwLoadAddr = pstFH->dwLoadAddr ;
        dwImageSize = pstFH->dwImageSize ;
        dwFlags = pstFH->dwFlags ;
        _tcscpy ( szModuleName , pstFH->szModuleName ) ;
    }

    return ( TRUE ) ;
}

// Returns the a pointer to the header data for READING only!
// This function is implemented in this class.
const LPSWSFILEHEADER CSWSFile :: GetReadOnlyHeaderData ( void )
{
    ASSERT ( NULL != m_pMappedMem ) ;
    ASSERT ( FALSE == m_bCreated ) ;

    if ( NULL == m_pMappedMem )
    {
        ASSERT ( !"File not properly mapped!!" ) ;
        return ( NULL ) ;
    }
    if ( TRUE == m_bCreated )
    {
        ASSERT ( !"GetReadOnlyHeaderData called while creating!" ) ;
        return ( NULL ) ;
    }

    // Read the mapped memory for the answers....
    LPSWSFILEHEADER pstFH = (LPSWSFILEHEADER)m_pMappedMem ;

    if ( SIG_SWSFILE != pstFH->dwSignature )
    {
        ASSERT ( !"File header does not match!!" ) ;
        return ( NULL ) ;
    }

    return ( (const LPSWSFILEHEADER)pstFH ) ;
}

/*//////////////////////////////////////////////////////////////////////
                        Public Creation Methods
                   (These are only for writing data!)
//////////////////////////////////////////////////////////////////////*/

// Creates a new file.  If the file already exists, it will be
// overwritten.
BOOL CSWSFile :: Create ( LPCTSTR szFileName )
{
    // Do the obvious checks.
    ASSERT ( NULL != szFileName ) ;
    if ( NULL == szFileName )
    {
        return ( FALSE ) ;
    }
    
    ASSERT ( INVALID_HANDLE_VALUE == m_hFile ) ;
    if ( INVALID_HANDLE_VALUE != m_hFile )
    {
        return ( FALSE ) ;
    }

    // First step is to ensure the .SWS is on the end.
    _tcscpy ( m_szFileName , szFileName ) ;
    TCHAR * pDot = _tcsrchr ( m_szFileName , _T ( '.' ) ) ;
    if ( ( NULL == pDot ) ||
         ( 0 != _tcsicmp ( pDot ,EXT_SWSFILE ) ) )
    {
        _tcscat ( m_szFileName , EXT_SWSFILE ) ;
    }

    // Try and create the file.
    m_hFile = CreateFile ( m_szFileName                 ,
                           GENERIC_READ | GENERIC_WRITE ,
                           FILE_SHARE_READ              ,
                           NULL                         ,
                           CREATE_ALWAYS                ,
                           FILE_ATTRIBUTE_ARCHIVE |
                              FILE_FLAG_SEQUENTIAL_SCAN ,
                           NULL                          ) ;
    ASSERT ( INVALID_HANDLE_VALUE != m_hFile ) ;
    if ( INVALID_HANDLE_VALUE == m_hFile )
    {
        return ( FALSE ) ;
    }

    // Mark that this is a created instance of this class.
    m_bCreated = TRUE ;

    // Allocate the data buffers.
    if ( FALSE == AllocateDataBuffer ( 1000 ) )
    {
        VERIFY ( CloseHandle ( m_hFile ) ) ;
        ASSERT ( !"Buffer allocation failed!!" ) ;
        return ( FALSE ) ;
    }

    // Force an empty header to be written.
    if ( FALSE == SetHeaderData ( 0               ,
                                  0               ,
                                  0               ,
                                  0               ,
                                  0               ,
                                  _T ( "Empty!" )  ) )
    {
        ASSERT ( !"SetHeaderData failed!" ) ;
        VERIFY ( CloseHandle ( m_hFile ) ) ;
        return ( FALSE ) ;
    }

    return ( TRUE ) ;
}

// Sets the header data.  This can be called at any time and will
// ensure that the file pointer is not disturbed.
BOOL CSWSFile :: SetHeaderData ( DWORD   dwLinkTime   ,
                                 DWORD   dwEntryCount ,
                                 DWORD64 dwLoadAddr   ,
                                 DWORD   dwImageSize  ,
                                 DWORD   dwFlags      ,
                                 LPCTSTR szModuleName  )
{
    ASSERT ( INVALID_HANDLE_VALUE != m_hFile ) ;
    ASSERT ( m_bCreated == TRUE ) ;
    if ( ( INVALID_HANDLE_VALUE == m_hFile ) ||
         ( m_bCreated == FALSE )                )
    {
        return ( FALSE ) ;
    }
    
    DWORD dwCurrPointer = GetFilePointer ( m_hFile ) ;

    if ( INVALID_SET_FILE_POINTER == SetFilePointer ( m_hFile    ,
                                                      0          ,
                                                      0          ,
                                                      FILE_BEGIN  ) )
    {
        ASSERT ( !"Unable to set the file pointer!!" ) ;
        return ( FALSE ) ;
    }

    SWSFILEHEADER stFH ;
    ZeroMemory ( &stFH , sizeof ( SWSFILEHEADER ) ) ;

    stFH.dwSignature = SIG_SWSFILE ;
    stFH.dwLinkTime = dwLinkTime ;
    stFH.dwEntryCount = dwEntryCount ;
    stFH.dwLoadAddr = dwLoadAddr ;
    stFH.dwImageSize = dwImageSize ;
    stFH.dwFlags = dwFlags ;
    _tcscpy ( stFH.szModuleName , szModuleName ) ;

    DWORD dwWritten ;
    if ( FALSE == WriteFile ( m_hFile                        ,
                              &stFH                          ,
                              sizeof ( SWSFILEHEADER ) ,
                              &dwWritten                     ,
                              NULL                            ) )
    {
        ASSERT ( !"Unable to write the header!!" ) ;
        return ( FALSE ) ;
    }
    
    SetFilePointer ( m_hFile , dwCurrPointer , 0 , FILE_BEGIN ) ;

    return ( TRUE ) ;

}

// Adds data to the file.
BOOL CSWSFile :: AddData ( DWORD64 dwAddr       ,
                           DWORD   dwSize       ,
                           DWORD   dwExecCount   )
{
    SWSENTRY stSE ;
    stSE.dwExecCount = dwExecCount ;
    stSE.dwFnAddr = dwAddr ;
    stSE.dwSize = dwSize ;

    return ( AppendToDataBuffer ( &stSE ) ) ;
}

/*//////////////////////////////////////////////////////////////////////
                             Public Methods
//////////////////////////////////////////////////////////////////////*/
// Both opening and creating can call this functions.
void CSWSFile :: Close ( void )
{
    // If the file handle is invalid, there is nothing to close.
    if ( INVALID_HANDLE_VALUE == m_hFile )
    {
        return ;
    }

    // If this is a created file, sort the memory buffer and write it
    // out.
    if ( TRUE == m_bCreated )
    {
        // Do the sort.
        SortByAddress ( ) ;

        // Update the header information.
        DWORD   dwLinkTime   ;
        DWORD   dwEntryCount ;
        DWORD64 dwLoadAddr   ;
        DWORD   dwImageSize  ;
        DWORD   dwFlags      ;
        TCHAR   szModuleName[ MAX_PATH ] ;
        if ( TRUE == GetHeaderData ( dwLinkTime   ,
                                     dwEntryCount ,
                                     dwLoadAddr   ,
                                     dwImageSize  ,
                                     dwFlags      ,
                                     szModuleName  ) )
        {
            if ( TRUE == SetHeaderData ( dwLinkTime   ,
                                         m_uiNumElems ,
                                         dwLoadAddr   ,
                                         dwImageSize  ,
                                         dwFlags      ,
                                         szModuleName  ) )
            {
                // Move to right after the header.
                if ( INVALID_SET_FILE_POINTER !=
                         SetFilePointer ( m_hFile                    ,
                                          sizeof(SWSFILEHEADER),
                                          0                          ,
                                          FILE_BEGIN                  ))
                {
                    DWORD dwWritten ;
                    if ( FALSE == WriteFile ( m_hFile                ,
                                              m_pBuffer              ,
                                              sizeof(SWSENTRY)*
                                                    m_uiNumElems     ,
                                              &dwWritten             ,
                                              NULL                   ))
                    {
                        ASSERT ( !"Writing data failed!" ) ;
                    }
                }
                else
                {
                    ASSERT ( !"Unable to set file pointer!" ) ;
                }
            }
            else
            {
                ASSERT ( !"Unable to write header data!" ) ;
            }
        }
        else
        {
            ASSERT ( !"Unable to read header data!!!" ) ;
        }
    }
    // Free up the resources.
    FreeResources ( ) ;
}

// Sorts by address.
BOOL CSWSFile :: SortByAddress ( void )
{
    void * pMem = NULL ;
    if ( NULL != m_pMappedMem )
    {
        pMem = m_pMappedMem + sizeof ( SWSFILEHEADER ) ;
    }
    else
    {
        pMem = m_pBuffer ;
    }
    qsort ( pMem                ,
            m_uiNumElems        ,
            sizeof ( SWSENTRY ) ,
            CompareAddr          ) ;
    return ( TRUE ) ;
}

// Sorts by execution count.
BOOL CSWSFile :: SortByExecutionCount ( void )
{
    void * pMem = NULL ;
    if ( NULL != m_pMappedMem )
    {
        pMem = m_pMappedMem + sizeof ( SWSFILEHEADER ) ;
    }
    else
    {
        pMem = m_pBuffer ;
    }
    qsort ( pMem                ,
            m_uiNumElems        ,
            sizeof ( SWSENTRY ) ,
            CompareExecCount     ) ;
    return ( TRUE ) ;
}

// Deletes an opened file.
void CSWSFile :: Delete ( void )
{
    ASSERT ( _T ( '\0' ) != m_szFileName[ 0 ] ) ;
    if ( _T ( '\0' ) == m_szFileName[ 0 ] )
    {
        return ;
    }
    // Save the filename.
    TCHAR szFile[ MAX_PATH ] ;
    _tcscpy ( szFile , m_szFileName ) ;

    FreeResources ( ) ;

    // Nuke it!
    VERIFY ( DeleteFile ( szFile ) ) ;
}

LPSWSENTRY CSWSFile :: GetIndex ( int iIndex )
{
    LPSWSENTRY pRet = NULL ;
    if ( NULL != m_pMappedMem )
    {
        pRet = (LPSWSENTRY)
               ( m_pMappedMem                    +
                 sizeof ( SWSFILEHEADER )  +
                 ( iIndex * sizeof ( SWSENTRY ) ) ) ;
    }
    else
    {
        pRet = (LPSWSENTRY)
               ( m_pBuffer           +
                 ( iIndex * sizeof ( SWSENTRY ) ) ) ;
    }
    return ( pRet ) ;
}

LPSWSENTRY CSWSFile :: FindAddress ( DWORD64 dwAddr )
{
    ASSERT ( NULL != m_pMappedMem ) ;

    LPSWSENTRY pRet ;
    SWSENTRY stKey ;

    stKey.dwFnAddr = dwAddr ;
    
    
    LPSWSFILEHEADER pHeader = (LPSWSFILEHEADER)m_pMappedMem ;

    pRet = (LPSWSENTRY)bsearch ( &stKey                         ,
                                 m_pMappedMem +
                                       sizeof ( SWSFILEHEADER ) ,
                                 pHeader->dwEntryCount          ,
                                 sizeof ( SWSENTRY )            ,
                                 CompareAddr                     ) ;
    return ( pRet ) ;
}

/*//////////////////////////////////////////////////////////////////////
                        Protected Helper Methods
//////////////////////////////////////////////////////////////////////*/
// Frees all associated resources.
void CSWSFile :: FreeResources ( void )
{
    if ( NULL != m_pMappedMem )
    {
        VERIFY ( UnmapViewOfFile ( m_pMappedMem ) ) ;
        m_pMappedMem = NULL ;
    }
    if ( INVALID_HANDLE_VALUE != m_hMapped )
    {
        VERIFY ( CloseHandle ( m_hMapped ) ) ;
        m_hMapped = INVALID_HANDLE_VALUE ;
    }
    if ( INVALID_HANDLE_VALUE != m_hFile )
    {
        VERIFY ( CloseHandle ( m_hFile ) ) ;
        m_hFile = INVALID_HANDLE_VALUE ;
    }
    m_bCreated = FALSE ;

    if ( NULL != m_pBuffer )
    {
        free ( m_pBuffer ) ;
        m_pBuffer = NULL ;
        m_uiTotalElems = 0 ;
        m_uiNumElems = 0 ;
    }
    m_szFileName[ 0 ] = _T ( '\0' ) ;
}

// Allocates the buffer used to store data points when creating a
// file.
BOOL CSWSFile :: AllocateDataBuffer ( UINT uiCount )
{
    // Check the easy stuff.
    ASSERT ( TRUE == m_bCreated ) ;
    ASSERT ( NULL == m_pBuffer ) ;
    
    if ( ( FALSE == m_bCreated  ) || ( NULL != m_pBuffer ) )
    {
        return ( FALSE ) ;
    }

    m_uiTotalElems = uiCount ;

    m_pBuffer = (LPBYTE)malloc ( sizeof ( SWSENTRY ) *
                                 m_uiTotalElems              ) ;
    ASSERT ( NULL != m_pBuffer ) ;

    // Always start on the first element!
    m_uiNumElems = 0 ;
    
    return ( NULL != m_pBuffer ) ;
}

// Appends an element to the data buffer.
BOOL CSWSFile :: AppendToDataBuffer ( PVOID pElement )
{
    ASSERT ( NULL != m_pBuffer ) ;
    if ( NULL == m_pBuffer )
    {
        return ( FALSE ) ;
    }

    if ( m_uiNumElems == m_uiTotalElems )
    {
        // Gotta reallocate!
        UINT uiNewSize = ( sizeof ( SWSENTRY ) *
                           m_uiTotalElems             ) * 2 ;
        m_uiTotalElems *= 2 ;
        m_pBuffer = (LPBYTE)realloc ( m_pBuffer , uiNewSize ) ;
    }
    // Plop the memory in.
    memcpy ( m_pBuffer + ( m_uiNumElems * sizeof ( SWSENTRY ) ),
             pElement                                                ,
             sizeof ( SWSENTRY )                               );
    m_uiNumElems++ ;

    return ( TRUE ) ;
}




