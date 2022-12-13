/*----------------------------------------------------------------------
Debugging Applications for Microsoft .NET and Microsoft Windows
Copyright © 1997-2003 John Robbins -- All rights reserved.
----------------------------------------------------------------------*/

#include "stdafx.h"
#include "FastTrace.h"
#include "FastTraceFile.h"

/*//////////////////////////////////////////////////////////////////////
// File Scope Constants and Defines.
//////////////////////////////////////////////////////////////////////*/
// The author of this file.
#ifdef SUPERASSERT_EMAIL
#undef SUPERASSERT_EMAIL
#define SUPERASSERT_EMAIL "john@wintellect.com"
#endif

// The format string for individual thread tracing files.
const LPCTSTR k_THREADFILEFMT = _T ( "%s_%04d_%04d%s" ) ;
// The format string for snap file names.
const LPCTSTR k_SNAPFILEFMT = _T ( "SNAP_%d_%s" ) ;

/*//////////////////////////////////////////////////////////////////////
// Prototypes for Functions in FastTrace.CPP
//////////////////////////////////////////////////////////////////////*/
LONG GetSnapNumber ( void ) ;
HANDLE CreateRawThreadFile ( DWORD dwTID ) ;

/*//////////////////////////////////////////////////////////////////////
// PUBLIC CODE STARTS HERE
//////////////////////////////////////////////////////////////////////*/

/*//////////////////////////////////////////////////////////////////////
// BINARY MODULE ONLY CODE STARTS HERE
//////////////////////////////////////////////////////////////////////*/

BOOL BuildThreadFileName ( LPTSTR szFileNameBuff ,
                           size_t iBuffLen       ,
                           DWORD  dwTID           )
{
    ASSERT ( iBuffLen >= MAX_PATH ) ;
    ASSERT ( NULL != szFileNameBuff ) ;
    if ( ( NULL == szFileNameBuff ) || ( iBuffLen < MAX_PATH ) )
    {
        return ( FALSE ) ;
    }
    
    BOOL bRetVal = TRUE ;
    
    // Get the process directory and name and peel off the .EXE
    // extension.
    TCHAR szExeName[ MAX_PATH + 1 ] ;
    DWORD dwRet = GetModuleFileName ( NULL , szExeName , MAX_PATH ) ;
    ASSERT ( 0 != dwRet ) ;
    if ( 0 != dwRet )
    {
        TCHAR * pPeriod = _tcsrchr ( szExeName , _T ( '.' ) ) ;
        ASSERT ( NULL != pPeriod ) ;
        if ( NULL != pPeriod )
        {
            *pPeriod = _T ( '\0' ) ;
            
            DWORD dwPID = GetCurrentProcessId ( ) ;
            // Build the complete
            HRESULT hr = StringCchPrintf ( szFileNameBuff  ,
                                           iBuffLen        ,
                                           k_THREADFILEFMT ,
                                           szExeName       ,
                                           dwPID           ,
                                           dwTID           ,
                                           k_FTFILEEXT      ) ;
            ASSERT ( SUCCEEDED ( hr ) ) ;
            if ( SUCCEEDED ( hr ) )
            {
                bRetVal = TRUE ;
            }
            else
            {
                bRetVal = FALSE ;
            }
        }
        else
        {
            bRetVal = FALSE ;
        }
    }
    else
    {
        bRetVal = FALSE ;
    }
    return ( bRetVal ) ;
}

BOOL BuildSnapThreadFileName ( LPTSTR szFileNameBuff ,
                               size_t iBuffLen       ,
                               DWORD  dwTID           )
{
    ASSERT ( iBuffLen >= MAX_PATH ) ;
    ASSERT ( NULL != szFileNameBuff ) ;
    if ( ( NULL == szFileNameBuff ) || ( iBuffLen < MAX_PATH ) )
    {
        return ( FALSE ) ;
    }
    
    BOOL bRetVal = TRUE ;
    
    // Get the process directory and name and peel off the .EXE
    // extension.
    TCHAR szExeName[ MAX_PATH + 1 ] ;
    DWORD dwRet = GetModuleFileName ( NULL , szExeName , MAX_PATH ) ;
    ASSERT ( 0 != dwRet ) ;
    if ( 0 != dwRet )
    {
        TCHAR * pPeriod = _tcsrchr ( szExeName , _T ( '.' ) ) ;
        ASSERT ( NULL != pPeriod ) ;
        if ( NULL != pPeriod )
        {
            *pPeriod = _T ( '\0' ) ;
            
            TCHAR * pLastSlash = _tcsrchr ( szExeName , _T ( '\\' ) ) ;
            ASSERT ( NULL != pLastSlash ) ;
            if ( NULL != pLastSlash )
            {
                *pLastSlash = _T ( '\0' ) ;
                pLastSlash++ ;
                
                DWORD dwPID = GetCurrentProcessId ( ) ;
                // Build the complete snap string.
                HRESULT hr = StringCchPrintf ( szFileNameBuff       ,
                                               iBuffLen             ,
                                               _T ( "%s\\SNAP_%d_%s_%04d_%04d.FTL" ) ,
                                               szExeName            ,
                                               GetSnapNumber ( )    ,
                                               pLastSlash           ,
                                               dwPID                ,
                                               dwTID                  ) ;
                ASSERT ( SUCCEEDED ( hr ) ) ;
                if ( SUCCEEDED ( hr ) )
                {
                    bRetVal = TRUE ;
                }
                else
                {
                    bRetVal = FALSE ;
                }
            }
            else
            {
                bRetVal = FALSE ;
            }
        }
        else
        {
            bRetVal = FALSE ;
        }
    }
    else
    {
        bRetVal = FALSE ;
    }
    return ( bRetVal ) ;
}


// Initializes a newly opened logging file.  When you hand over the
// file handle to this class, this class owns it through close.
// Returns TRUE if all is good, FALSE otherwise.
BOOL FTFileWriter :: Initialize ( HANDLE hFile , DWORD dwTID )
{
    ASSERT ( INVALID_HANDLE_VALUE != hFile ) ;
    if ( INVALID_HANDLE_VALUE == hFile )
    {
        return ( FALSE ) ;
    }
    // Save the file handle off.
    m_hFile = hFile ;
    // Save the TID off.
    m_dwTID = dwTID ;
    
    // Ensure the count starts at zero.
    m_lWrites = 0 ;
    BOOL bRet = WriteHeader ( ) ;
    ASSERT ( TRUE == bRet ) ;
    if ( FALSE == bRet )
    {
        // Not much else I can do.
        VERIFY ( CloseHandle ( m_hFile ) ) ;
        m_hFile = INVALID_HANDLE_VALUE ;
    }
    return ( bRet ) ;
}

// Writes a string to the logging file.
// Returns TRUE if all is good, FALSE otherwise.
BOOL FTFileWriter :: WriteString ( LPFTDATA pData )
{
    ASSERT ( INVALID_HANDLE_VALUE != m_hFile ) ;
    if ( INVALID_HANDLE_VALUE == m_hFile )
    {
        return ( FALSE ) ;
    }
    DWORD dwWritten = 0 ;
    BOOL bRet = WriteFile ( m_hFile           ,
                            pData             ,
                            sizeof ( FTDATA ) ,
                            &dwWritten        ,
                            NULL               ) ;
    ASSERT ( TRUE == bRet ) ;
    ASSERT ( sizeof ( FTDATA ) == dwWritten ) ;
    if ( ( FALSE == bRet ) || ( sizeof ( FTDATA ) != dwWritten ) )
    {
        // Let's attempt to write the header so it gets updated.
        WriteHeader ( ) ;
        
        bRet = FALSE ;
        // Not much else I can do.
        VERIFY ( CloseHandle ( m_hFile ) ) ;
        m_hFile = INVALID_HANDLE_VALUE ;
    }
    else
    {
        // It's a good write.
        m_lWrites++ ;
    }
    return ( bRet ) ;
}

// Closes the logging file.
// Returns TRUE if all is good, FALSE otherwise.
BOOL FTFileWriter :: Close ( void )
{
    BOOL bRet = FALSE ;
    ASSERT ( INVALID_HANDLE_VALUE != m_hFile ) ;
    if ( INVALID_HANDLE_VALUE != m_hFile )
    {
        // Scoot back to the beginning of the file and update the
        // header.
        bRet = WriteHeader ( ) ;
        ASSERT ( TRUE == bRet ) ;

        // I'm done with the file.
        VERIFY ( CloseHandle ( m_hFile ) ) ;
        m_hFile = INVALID_HANDLE_VALUE ;
        m_lWrites = 0 ;
    }
    return ( bRet ) ;
}

// Snaps the file to disk.  This closes the current file and renames
// it SNAP#_<PROC>_<PID>_<TID>.FTL.  A new tracing file is opened.
BOOL FTFileWriter :: SnapFile ( void )
{
    // The first step is to close the existing file.
    BOOL bRet = Close ( ) ;
    ASSERT ( TRUE == bRet ) ;
    if ( TRUE == bRet )
    {
        // Build up the filename for this thread.
        TCHAR szFile [ MAX_PATH ] ;
        bRet = BuildThreadFileName ( szFile , MAX_PATH , m_dwTID ) ;
        ASSERT ( TRUE == bRet ) ;
        if ( TRUE == bRet )
        {
            // Create the snap name.
            TCHAR szSnapFile[ MAX_PATH ] ;
            bRet = BuildSnapThreadFileName ( szSnapFile ,
                                             MAX_PATH   ,
                                             m_dwTID     ) ;
            ASSERT ( TRUE == bRet ) ;
            if ( TRUE == bRet )
            {
                // Move the old file over to the snap name.
                bRet = MoveFile ( szFile , szSnapFile ) ;
                ASSERT ( TRUE == bRet ) ;
                if ( TRUE == bRet )
                {
                    // Open the new current tracing file.
                    HANDLE hFile = CreateRawThreadFile ( m_dwTID ) ;
                    ASSERT ( INVALID_HANDLE_VALUE != hFile ) ;
                    if ( INVALID_HANDLE_VALUE != hFile )
                    {
                        // Open up the new file.
                        bRet = Initialize ( hFile , m_dwTID ) ;
                        ASSERT ( TRUE == bRet ) ;
                    }
                    else
                    {
                        bRet = FALSE ;
                    }
                }
            }
        }
    }
    return ( bRet ) ;
}

BOOL FTFileWriter :: WriteHeader ( void )
{
    BOOL bRet = FALSE ;
    ASSERT ( INVALID_HANDLE_VALUE != m_hFile ) ;
    if ( INVALID_HANDLE_VALUE != m_hFile )
    {
        // Scoot back to the beginning of the file and update the
        // header.
        DWORD dwMove = SetFilePointer ( m_hFile     ,
                                        0           ,
                                        NULL        ,
                                        FILE_BEGIN   ) ;
        ASSERT ( INVALID_SET_FILE_POINTER != dwMove ) ;
        if ( INVALID_SET_FILE_POINTER != dwMove )
        {
            FTFILEHEADER stFH ;
            ZeroMemory ( &stFH , sizeof ( FTFILEHEADER ) ) ;
            stFH.dwSig = FTFILESIG ;
            stFH.lItemCount= m_lWrites ;
            stFH.lMaxStringCharCount = MAX_FASTTRACE_LEN ;
            stFH.lPID = (LONG)GetCurrentProcessId ( ) ;
            stFH.lTID = (LONG)m_dwTID ;
            
            DWORD dwWritten = 0 ;
            bRet = WriteFile ( m_hFile                 ,
                               &stFH                   ,
                               sizeof ( FTFILEHEADER ) ,
                               &dwWritten              ,
                               NULL                     ) ;
            ASSERT ( TRUE == bRet ) ;
            ASSERT ( sizeof ( FTFILEHEADER ) == dwWritten ) ;
            if ( ( FALSE == bRet                        ) ||
                 ( sizeof ( FTFILEHEADER ) != dwWritten )   )
            {
                bRet = FALSE ;
            }
        }
        else
        {
            bRet = FALSE ;
        }
    }
    return ( bRet ) ;
}