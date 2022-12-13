/*----------------------------------------------------------------------
"Debugging Applications" (Microsoft Press)
Copyright (c) 1997-2000 John Robbins -- All rights reserved.

With excellent updates from Scott Bloom, Ching Ming Kwok, 
Jeff Shanholtz, and Pablo Presedo!
----------------------------------------------------------------------*/
#ifndef _ABOUT_H
#define _ABOUT_H

#include "resource.h"
#include "BigIcon.h"

class CAboutDlg : public CDialog
{
public      :

    CAboutDlg ( void ) ;

    virtual ~CAboutDlg ( void ) ;

    enum { IDD = IDD_ABOUTBOX } ;

protected   :
    virtual BOOL OnInitDialog ( void ) ;
    virtual void DoDataExchange ( CDataExchange * pDX ) ;
    afx_msg LRESULT OnCtrlColorStatic ( WPARAM wParam , LPARAM lParam );

    // The big icon control.
    CBigIcon m_cBigIcon ;
    // The font for the big text.
    CFont * m_pcFont    ;


    DECLARE_MESSAGE_MAP ( )
} ;

#endif  // _ABOUT_H


