/*----------------------------------------------------------------------
"Debugging Applications" (Microsoft Press)
Copyright (c) 1997-2000 John Robbins -- All rights reserved.

With excellent updates from Scott Bloom, Ching Ming Kwok, 
Jeff Shanholtz, and Pablo Presedo!
----------------------------------------------------------------------*/
#if !defined ( _CRASHFINDER_H )
#define _CRASHFINDER_H

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#ifndef __AFXWIN_H__
    #error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols

class CCrashFinderApp : public CWinApp
{
public      :

    CCrashFinderApp ( ) ;

public      :
    virtual BOOL InitInstance ( ) ;
    virtual int ExitInstance ( ) ;
    afx_msg void OnAppAbout ( ) ;
    afx_msg void OnViewOptions ( ) ;

    DECLARE_MESSAGE_MAP ( )

public      :
    // Called by documents to get the global application values.
    BOOL ConfirmDeletions ( void ) const ;
    BOOL ShowFullPaths ( void ) const ;

protected   :
    // Saves the global application state.
    void SaveGlobalState ( void ) ;
    // Restored the global application state.
    void RestoreGlobalState ( void ) ;

protected   :
    // The application global options.
    BOOL m_bShowFullPaths ;
    BOOL m_bConfirmDeletions ;

public      :
        BOOL m_bAddress ;
        BOOL m_bModule ;
        BOOL m_bFunction ;
        BOOL m_bFnDisplacement ;
        BOOL m_bSourceFile ;
        BOOL m_bSourceLine ;
        BOOL m_bSrcDisplacement ;
        BOOL m_bOneLine ;

} ;


#endif // _CRASHFINDER_H
