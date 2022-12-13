/*----------------------------------------------------------------------
"Debugging Applications" (Microsoft Press)
Copyright (c) 1997-2000 John Robbins -- All rights reserved.

With excellent updates from Scott Bloom, Ching Ming Kwok, 
Jeff Shanholtz, and Pablo Presedo!
----------------------------------------------------------------------*/
#ifndef _BINARYIMAGEOPTIONSDLG_H
#define _BINARYIMAGEOPTIONSDLG_H

#include "resource.h"
#include "HexEdit.h"


class CBinaryImageOptionsDlg : public CDialog
{
public      :

    CBinaryImageOptionsDlg ( ) ;

    // Dialog Data
    enum { IDD = IDD_BINARYPROPERTIES } ;

    DWORD m_dwAddr ;

protected   :
    virtual void DoDataExchange ( CDataExchange * pDX ) ;
    virtual BOOL OnInitDialog ( void ) ;
    virtual void OnOK ( void ) ;

// Implementation
protected   :

    CHexEdit    m_cHexCtrl  ;

    DECLARE_MESSAGE_MAP ( )
} ;

#endif  // _BINARYIMAGEOPTIONSDLG_H


