/*----------------------------------------------------------------------
Debugging Applications for Microsoft .NET and Microsoft Windows
Copyright © 1997-2003 John Robbins -- All rights reserved.
----------------------------------------------------------------------*/
#ifndef _MAPPEDLOGFILE_H
#define _MAPPEDLOGFILE_H

// The file data stuff.
#include "FastTrace.h"

#pragma once

// The error values returned by CMappedLogFile::Open.
typedef enum
{
    OPENFAIL_ALL_OK             = 0 ,
    OPENFAIL_INVALID_PARAM      = 1 ,
    OPENFAIL_ALREADY_OPEN       = 2 ,
    OPENFAIL_UNABLE_TO_OPEN     = 3 ,
    OPENFAIL_CREATE_MAPPING     = 4 ,
    OPENFAIL_MAP_VIEW           = 5 ,
    OPENFAIL_HEADER_MISMATCH    = 6
} eOpenRes ;

class CMappedLogFile
{
/*//////////////////////////////////////////////////////////////////////
// Public ctor and dtor stuff.
//////////////////////////////////////////////////////////////////////*/
public      :
    CMappedLogFile ( void )
    {
        m_hFile = INVALID_HANDLE_VALUE ;
        m_hMapped = INVALID_HANDLE_VALUE ;
        m_pMappedMem = NULL ;
        m_lCurrIndex = (LONG)-1 ;
        m_lCount = (LONG)-1 ;
        
        m_lTID = 0 ;
        m_lPID = 0 ;
    }
    
    virtual ~CMappedLogFile ( void )
    {
        if ( INVALID_HANDLE_VALUE != m_hFile )
        {
            Close ( ) ;
        }
    }
    
/*//////////////////////////////////////////////////////////////////////
// Opening, closing and reading methods.
//////////////////////////////////////////////////////////////////////*/
public      :
    // Opens the file and prepares it for reading.
    eOpenRes Open ( LPCTSTR szFile ) ;
    
    // Returns the first string in the file.
    LPFTDATA GetFirstString ( void ) ;
    
    // Returns the next trace string in the file.  Returns NULL if at
    // the end of the file.  This increments the current location in the
    // file.
    LPFTDATA GetNextString ( void ) ;
    
    // Returns the current item in the file but does NOT increment and
    // prepare to read the next item.
    LPFTDATA PeekCurrentString ( void ) ;
    
    // Returns the thread ID for this file.
    LONG ThreadID ( void )
    {
        if ( 0 == m_lTID )
        {
            LPFTFILEHEADER pHeader = (LPFTFILEHEADER)m_pMappedMem ;
            m_lTID = pHeader->lTID ;
        }
        return ( m_lTID ) ;
    }
    
    // Returns the process ID for this file.
    LONG ProcessID ( void )
    {
        if ( 0 == m_lPID )
        {
            LPFTFILEHEADER pHeader = (LPFTFILEHEADER)m_pMappedMem ;
            m_lPID = pHeader->lPID ;
            
        }
        return ( m_lPID ) ;
    }
    
    // Returns the count for this file.
    LONG StringCount ( void )
    {
        if ( 0 == m_lCount )
        {
            LPFTFILEHEADER pHeader = (LPFTFILEHEADER)m_pMappedMem ;
            m_lCount = pHeader->lItemCount ;
        }
        return ( m_lCount ) ;
    }
    
    // Closes the file.
    void Close ( void ) ;

/*//////////////////////////////////////////////////////////////////////
// Protected data members.
//////////////////////////////////////////////////////////////////////*/
protected   :
    // The file handle.
    HANDLE m_hFile ;
    // The mapping handle.
    HANDLE m_hMapped ;
    // The mapped memory.
    LPBYTE m_pMappedMem ;
    // The current position in the array of data.
    LONG   m_lCurrIndex ;
    // The total number of strings in this file.
    LONG   m_lCount ;
    // The thread ID for the file.
    LONG   m_lTID ;
    // The thread ID for this file.
    LONG   m_lPID ;
} ;

#endif      // _MAPPEDLOGFILE_H