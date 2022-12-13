/*----------------------------------------------------------------------
Debugging Applications for Microsoft .NET and Microsoft Windows
Copyright © 1997-2003 John Robbins -- All rights reserved.
------------------------------------------------------------------------
The class that handles the DeadDetExt DLLs.
----------------------------------------------------------------------*/

#include "PCH.h"
#include "DeadlockDetection.h"
#include "DDExtHandler.h"

BOOL CDDExtHandler :: LoadDDExtDll ( LPCTSTR szFile )
{
    // Make sure this is not called multiple times.
    ASSERT ( NULL == m_hInst ) ;
    if ( NULL != m_hInst )
    {
        return ( FALSE ) ;
    }

    // Get the library loaded.
    m_hInst = ::LoadLibrary ( szFile ) ;

    ASSERT ( NULL != m_hInst ) ;
    if ( NULL == m_hInst )
    {
        return ( FALSE ) ;
    }

    // Grab all of the exported functions.
    m_pfnDDExtOpen         = (PFNDEADDETEXTOPEN)
                              GetProcAddress ( m_hInst ,
                                               FN_DEADDETEXTOPEN ) ;
    ASSERT ( NULL != m_pfnDDExtOpen ) ;

    m_pfnDDExtClose        = (PFNDEADDETEXTCLOSE)
                              GetProcAddress ( m_hInst ,
                                               FN_DEADDETEXTCLOSE );
    ASSERT ( NULL != m_pfnDDExtClose ) ;

    m_pfnDDExtProcessEvent = (PFNDEADDETEXTPROCESSEVENT)
                              GetProcAddress ( m_hInst ,
                                             FN_DEADDETEXTPROCESSEVENT);
    ASSERT ( NULL != m_pfnDDExtProcessEvent ) ;
    
    m_pfnDeadDetWriteToLog = (PFNDEADDETWRITETOLOG)
                             GetProcAddress ( m_hInst ,
                                              FN_DEADDETWRITETOLOG ) ;
    ASSERT ( NULL != m_pfnDeadDetWriteToLog ) ;

    if ( ( NULL == m_pfnDDExtOpen         ) ||
         ( NULL == m_pfnDDExtClose        ) ||
         ( NULL == m_pfnDDExtProcessEvent ) ||
         ( NULL == m_pfnDeadDetWriteToLog )   )
    {
        return ( FALSE ) ;
    }

    // All OK, Jumpmaster!
    return ( TRUE ) ;
}

