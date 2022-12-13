/*----------------------------------------------------------------------
Debugging Applications for Microsoft .NET and Microsoft Windows
Copyright © 1997-2003 John Robbins -- All rights reserved.
----------------------------------------------------------------------*/
#include "stdafx.h"
#include "WDBG.h"
#include "WDBGChildFrame.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

IMPLEMENT_DYNCREATE(CWDBGChildFrame, CMDIChildWnd)

CWDBGChildFrame::CWDBGChildFrame()
{
}

CWDBGChildFrame::~CWDBGChildFrame()
{
}

void CWDBGChildFrame :: DisableCloseButton ( void )
{
    // This disable the system menu option.
    CMenu * pMenu = GetSystemMenu ( FALSE ) ;
    ASSERT ( NULL != pMenu ) ;
    if ( NULL != pMenu )
    {
        pMenu->EnableMenuItem ( SC_CLOSE , MF_BYCOMMAND | MF_GRAYED ) ;
    }
}

void CWDBGChildFrame :: EnableCloseButton ( void )
{
    CMenu * pMenu = GetSystemMenu ( FALSE ) ;
    ASSERT ( NULL != pMenu ) ;
    if ( NULL != pMenu )
    {
        pMenu->EnableMenuItem ( SC_CLOSE , MF_BYCOMMAND ) ;
    }
}

BEGIN_MESSAGE_MAP(CWDBGChildFrame, CMDIChildWnd)
    //{{AFX_MSG_MAP(CWDBGChildFrame)
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

BOOL CWDBGChildFrame::PreCreateWindow(CREATESTRUCT& cs)
{
    if ( FALSE == CMDIChildWnd::PreCreateWindow(cs) )
    {
        return ( FALSE ) ;
    }
    // Turn off the title updating.
    cs.style &= ~(LONG)FWS_ADDTOTITLE ;
    return ( TRUE ) ;
}
