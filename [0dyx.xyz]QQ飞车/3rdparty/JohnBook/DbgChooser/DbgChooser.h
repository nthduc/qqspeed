/*---------------------------------------------------------------------
    Debugging .NET and Windows-based Applications - John Robbins
---------------------------------------------------------------------*/
#ifndef _DBGCHOOSER_H
#define _DBGCHOOSER_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
    #error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols

class CDbgChooserApp : public CWinApp
{
public:
    CDbgChooserApp();

// Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CDbgChooserApp)
    public:
    virtual BOOL InitInstance();
    //}}AFX_VIRTUAL

// Implementation

    //{{AFX_MSG(CDbgChooserApp)
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
};



//{{AFX_INSERT_LOCATION}}

#endif // _DBGCHOOSER_H
