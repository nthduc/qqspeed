/*----------------------------------------------------------------------
Debugging Applications for Microsoft .NET and Microsoft Windows
Copyright © 1997-2003 John Robbins -- All rights reserved.
----------------------------------------------------------------------*/
#include "stdafx.h"
#include "MappedLogFile.h"

// The author of this file.
#ifdef SUPERASSERT_EMAIL
#undef SUPERASSERT_EMAIL
#define SUPERASSERT_EMAIL "john@wintellect.com"
#endif

// Opens the file and prepares it for reading.
eOpenRes CMappedLogFile :: Open ( LPCTSTR szFile )
{
    eOpenRes eRet = OPENFAIL_ALL_OK ;

    // Do the two easy checks.
    ASSERT ( NULL != szFile ) ;
    if ( NULL == szFile )
    {
        return ( OPENFAIL_INVALID_PARAM ) ;
    }
    ASSERT ( INVALID_HANDLE_VALUE == m_hFile ) ;
    if ( INVALID_HANDLE_VALUE != m_hFile )
    {
        return ( OPENFAIL_ALREADY_OPEN ) ;
    }
    
    // Open the file.
    m_hFile = CreateFile ( szFile                ,
                           GENERIC_READ          ,
                           FILE_SHARE_READ       ,
                           NULL                  ,
                           OPEN_EXISTING         ,
                           FILE_ATTRIBUTE_NORMAL ,
                           NULL                   ) ;
    ASSERT ( INVALID_HANDLE_VALUE != m_hFile ) ;
    if ( INVALID_HANDLE_VALUE != m_hFile )
    {
        // Map it in.
        m_hMapped = CreateFileMapping ( m_hFile         ,
                                        NULL            ,
                                        PAGE_READONLY   ,
                                        0               ,
                                        0               ,
                                        NULL             ) ;
        ASSERT ( NULL != m_hMapped ) ;
        if ( NULL != m_hMapped )
        {
            // Create the view.
            m_pMappedMem = (LPBYTE)MapViewOfFile ( m_hMapped     ,
                                                   FILE_MAP_READ ,
                                                   0             ,
                                                   0             ,
                                                   0              ) ;
            ASSERT ( NULL != m_pMappedMem ) ;
            if ( NULL != m_pMappedMem )
            {
                // Do a quick check of the header.
                LPFTFILEHEADER pHeader = (LPFTFILEHEADER)m_pMappedMem ;
                ASSERT ( FTFILESIG == pHeader->dwSig ) ;
                if ( FTFILESIG == pHeader->dwSig )
                {
                    m_lCount = pHeader->lItemCount ;
                    m_lCurrIndex = 0 ;
                    eRet = OPENFAIL_ALL_OK ;
                }
                else
                {
                    Close ( ) ;
                    eRet = OPENFAIL_HEADER_MISMATCH ;
                }
            }
            else
            {
                Close ( ) ;
                eRet = OPENFAIL_MAP_VIEW ;
            }
        }
        else
        {
            Close ( ) ;
            eRet = OPENFAIL_CREATE_MAPPING ;
        }
    }
    else
    {
        eRet = OPENFAIL_UNABLE_TO_OPEN ;
    }
    return ( eRet ) ;
}

// Returns the first string in the file.
LPFTDATA CMappedLogFile :: GetFirstString ( void )
{
    if ( 0 == m_lCount )
    {
        return ( NULL ) ;
    }
    m_lCurrIndex = 0 ;
    LPFTDATA pRet = PeekCurrentString ( ) ;
    return ( pRet ) ;
}

// Returns the next trace string in the file.  Returns NULL if at
// the end of the file.
LPFTDATA CMappedLogFile :: GetNextString ( void )
{
    m_lCurrIndex++ ;
    if ( m_lCount == m_lCurrIndex )
    {
        return ( NULL ) ;
    }
    LPFTDATA pRet = PeekCurrentString ( ) ;
    return ( pRet ) ;
}

LPFTDATA CMappedLogFile :: PeekCurrentString ( void )
{
    LPFTDATA pRet = (LPFTDATA)( m_pMappedMem +
                                sizeof ( FTFILEHEADER ) +
                                ( m_lCurrIndex * sizeof ( FTDATA ) ) ) ;
    return ( pRet ) ;
}

// Closes the file.
void CMappedLogFile :: Close ( void )
{
    if ( NULL != m_pMappedMem )
    {
        VERIFY ( UnmapViewOfFile ( m_pMappedMem ) ) ;
        m_pMappedMem = NULL ;
    }
    if ( NULL != m_hMapped )
    {
        VERIFY ( CloseHandle ( m_hMapped ) ) ;
        m_hMapped = INVALID_HANDLE_VALUE ;
    }
    if ( INVALID_HANDLE_VALUE != m_hFile )
    {
        VERIFY ( CloseHandle ( m_hFile ) ) ;
        m_hFile = INVALID_HANDLE_VALUE ;
    }
}
