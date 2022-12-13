/*----------------------------------------------------------------------
"Debugging Applications" (Microsoft Press)
Copyright (c) 1997-2000 John Robbins -- All rights reserved.

With excellent updates from Scott Bloom, Ching Ming Kwok, 
Jeff Shanholtz, and Pablo Presedo!
----------------------------------------------------------------------*/
#ifndef _APPOPTIONSDLG_H
#define _APPOPTIONSDLG_H

#include "resource.h"


class CAppOptionsDlg : public CDialog
{
public      :

    CAppOptionsDlg ( ) ;

    // Dialog Data

    enum { IDD = IDD_APPOPTIONS } ;

protected   :
    virtual void DoDataExchange ( CDataExchange * pDX ) ;

// Implementation
protected   :

    DECLARE_MESSAGE_MAP ( )

public      :

    // The boolean options.
    BOOL    m_bConfirmDeletions ;
    BOOL    m_bShowFullPaths    ;

} ;

#endif  // _APPOPTIONSDLG_H


