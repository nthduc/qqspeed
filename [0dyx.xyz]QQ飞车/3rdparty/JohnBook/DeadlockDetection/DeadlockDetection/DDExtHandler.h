/*----------------------------------------------------------------------
Debugging Applications for Microsoft .NET and Microsoft Windows
Copyright © 1997-2003 John Robbins -- All rights reserved.
------------------------------------------------------------------------
The class that handles the DeadDetExt DLLs.
----------------------------------------------------------------------*/

#ifndef _DDEXTHANDLER_H
#define _DDEXTHANDLER_H

class CDDExtHandler
{
////////////////////////////////////////////////////////////////////////
//                Public Constructors and Destructors
////////////////////////////////////////////////////////////////////////
public      :

    // The basic initializer constructor.
    CDDExtHandler ( void )
    {
        m_hInst                 = NULL ;
        m_pfnDDExtOpen          = NULL ;
        m_pfnDDExtClose         = NULL ;
        m_pfnDDExtProcessEvent  = NULL ;
        m_pfnDeadDetWriteToLog  = NULL ;
    }

    // The destructor should do no work since the whole system needs to
    // rely on SEH.  The FreeDDExtDll must be called before this class
    // goes away.
    virtual ~CDDExtHandler ( void )
    {
        ASSERT ( NULL == m_hInst ) ;
    }

////////////////////////////////////////////////////////////////////////
//                           Public Methods
////////////////////////////////////////////////////////////////////////
public      :

    // Takes care of loading the extension DLL.
    BOOL LoadDDExtDll ( LPCTSTR szFile ) ;

    // Cleans up the extension DLL stuff.
    void FreeDDExtDll ( void )
    {
        // Make sure this class has been initialized.
        ASSERT ( NULL != m_hInst ) ;
        if ( NULL == m_hInst )
        {
            return ;
        }
        VERIFY ( FreeLibrary ( m_hInst ) ) ;

        m_hInst                 = NULL ;
        m_pfnDDExtOpen          = NULL ;
        m_pfnDDExtClose         = NULL ;
        m_pfnDDExtProcessEvent  = NULL ;
        m_pfnDeadDetWriteToLog  = NULL ;
    }

    // Returns the handle of the extension DLL.
    HINSTANCE GetDDExtHandle ( void )
    {
        return ( m_hInst ) ;
    }

    // Calls the open function.
    BOOL DeadDetExtOpen ( void )
    {
        ASSERT ( FALSE == IsBadCodePtr ( (FARPROC)m_pfnDDExtOpen ) ) ;
        return ( m_pfnDDExtOpen ( ) ) ;
    }

    void DeadDetExtClose ( void )
    {
        ASSERT ( FALSE == IsBadCodePtr ( (FARPROC)m_pfnDDExtClose ) ) ;
        m_pfnDDExtClose ( ) ;
    }

    BOOL DeadDetProcessEvent ( const LPDDEVENTINFO pEvtInfo )
    {
        ASSERT ( FALSE ==IsBadCodePtr((FARPROC)m_pfnDDExtProcessEvent));
        return ( m_pfnDDExtProcessEvent ( pEvtInfo ) ) ;
    }
    
    BOOL DeadDetWriteToLog ( LPCTSTR szMsg )
    {
        ASSERT ( FALSE ==IsBadCodePtr((FARPROC)m_pfnDeadDetWriteToLog));
        return ( m_pfnDeadDetWriteToLog ( szMsg ) ) ;
    }

////////////////////////////////////////////////////////////////////////
//                        Private Data Members
////////////////////////////////////////////////////////////////////////
private     :
    // The HINSTANCE of the loaded DLL.
    HINSTANCE                   m_hInst                 ;
    // The pointers to the specific functions.
    PFNDEADDETEXTOPEN           m_pfnDDExtOpen          ;
    PFNDEADDETEXTCLOSE          m_pfnDDExtClose         ;
    PFNDEADDETEXTPROCESSEVENT   m_pfnDDExtProcessEvent  ;
    PFNDEADDETWRITETOLOG        m_pfnDeadDetWriteToLog  ;
} ;

#endif  // _DDEXTHANDLER_H


