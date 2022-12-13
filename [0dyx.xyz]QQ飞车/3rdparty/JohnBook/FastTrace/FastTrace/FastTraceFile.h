/*----------------------------------------------------------------------
Debugging Applications for Microsoft .NET and Microsoft Windows
Copyright © 1997-2003 John Robbins -- All rights reserved.
----------------------------------------------------------------------*/
#ifndef _FASTTRACEFILE_H
#define _FASTTRACEFILE_H

#pragma once

class FTFileWriter
{
/*//////////////////////////////////////////////////////////////////////
// Public construction and destruction.
//////////////////////////////////////////////////////////////////////*/
public      :
    FTFileWriter ( void )
    {
        m_hFile = INVALID_HANDLE_VALUE ;
        m_lWrites = 0 ;
        m_dwTID = 0 ;
    }
    
    virtual ~FTFileWriter ( void )
    {
        if ( INVALID_HANDLE_VALUE != m_hFile )
        {
            Close ( ) ;
        }
        m_lWrites = 0 ;
    }

/*//////////////////////////////////////////////////////////////////////
// Public worker functions.
//////////////////////////////////////////////////////////////////////*/
public      :

    // Initializes a newly opened logging file.  When you hand over the
    // file handle to this class, this class owns it through close.
    // Returns TRUE if all is good, FALSE otherwise.
    BOOL Initialize ( HANDLE hFile , DWORD dwTID ) ;
    
    // Writes a string to the logging file.
    // Returns TRUE if all is good, FALSE otherwise.
    BOOL WriteString ( LPFTDATA pData ) ;
    
    // Closes the logging file.
    // Returns TRUE if all is good, FALSE otherwise.
    BOOL Close ( void ) ;
    
    // Flushes this file to disk.
    BOOL Flush ( void )
    {
        BOOL bRet = FALSE ;
        ASSERT ( INVALID_HANDLE_VALUE != m_hFile ) ;
        if ( INVALID_HANDLE_VALUE != m_hFile )
        {
            bRet = FlushFileBuffers ( m_hFile ) ;
            ASSERT ( TRUE == bRet ) ;
        }
        return ( bRet ) ;
    }
    
    // Snaps the file to disk.  This closes the current file and renames
    // it SNAP#_<PROC>_<PID>_<TID>.FTL.  A new tracing file is opened.
    BOOL SnapFile ( void ) ;

/*//////////////////////////////////////////////////////////////////////
// Protected methods
//////////////////////////////////////////////////////////////////////*/
protected   :
    BOOL WriteHeader ( void ) ;
    
/*//////////////////////////////////////////////////////////////////////
// Protected data members
//////////////////////////////////////////////////////////////////////*/
protected   :
    // The actual file handle.
    HANDLE m_hFile ;
    // The total number of writes to the file.
    LONG   m_lWrites ;
    // The thread this file wraps.
    DWORD  m_dwTID ;
} ;

/*----------------------------------------------------------------------
FUNCTION    :   BuildThreadFileName
DESCRIPTION :
    Builds a filename for the specified thread trace file.
PARAMETERS  :
    szFileNameBuff - Contains the buffer used to build the filename.
                     This buffer must be at least MAX_PATH long.
                     On failure, the value in the buffer is undefined.
    iBuffLen       - The buffer length.  If this is less than MAX_PATH,
                     the function will fail.
    dwTID          - The thread ID.
RETURNS     :
    TRUE  - Filename is complete.
    FALSE - There was a problem and filename is invalid.
----------------------------------------------------------------------*/
BOOL BuildThreadFileName ( LPTSTR szFileNameBuff ,
                           size_t iBuffLen       ,
                           DWORD  dwTID           ) ;


#endif  // _FASTTRACEFILE_H