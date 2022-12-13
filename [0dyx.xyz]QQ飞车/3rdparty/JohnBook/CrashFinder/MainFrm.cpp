/*----------------------------------------------------------------------
"Debugging Applications" (Microsoft Press)
Copyright (c) 1997-2000 John Robbins -- All rights reserved.

With excellent updates from Scott Bloom, Ching Ming Kwok, 
Jeff Shanholtz, and Pablo Presedo!
----------------------------------------------------------------------*/
#include "stdafx.h"
#include "CrashFinder.h"

#include "MainFrm.h"

#include "IgnoreDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE [] = __FILE__ ;
#endif

IMPLEMENT_DYNAMIC ( CMainFrame , CMDIFrameWnd )

BEGIN_MESSAGE_MAP ( CMainFrame , CMDIFrameWnd )
  //{{AFX_MSG_MAP(CMainFrame)
  ON_WM_CREATE ( )
  ON_COMMAND ( ID_EDIT_IGNORED_IMAGE            , OnEditIgnoredImage    )
  //}}AFX_MSG_MAP
END_MESSAGE_MAP ( )

static UINT indicators[ ] =
{
  ID_SEPARATOR      ,
  ID_INDICATOR_CAPS ,
  ID_INDICATOR_NUM  ,
  ID_INDICATOR_SCRL ,
} ;

CMainFrame :: CMainFrame ( )
            : CMDIFrameWnd ( )   ,
              m_wndStatusBar ( ) ,
              m_wndToolBar ( )
{
    ASSERT ( this ) ;
}

CMainFrame :: ~CMainFrame ( )
{
    ASSERT ( this ) ;
}

int CMainFrame :: OnCreate ( LPCREATESTRUCT lpCreateStruct )
{
    ASSERT ( this ) ;

    if ( -1 == CMDIFrameWnd::OnCreate ( lpCreateStruct ) )
    {
        return ( -1 ) ;
    }

    if ( ( !m_wndToolBar.Create ( this )               ) ||
         ( !m_wndToolBar.LoadToolBar ( IDR_MAINFRAME ) )    )
    {
        TRACE0 ( "Failed to create toolbar\n" ) ;
        return ( -1 ) ; // fail to create
    }

    if ( ( !m_wndStatusBar.Create ( this )                          ) ||
         ( !m_wndStatusBar.SetIndicators ( indicators ,
                                           sizeof ( indicators ) /
                                                 sizeof ( UINT ) )  )  )
    {
        TRACE0 ( "Failed to create status bar\n" ) ;
        return ( -1 ) ; // fail to create
    }

    // TODO: Remove this if you don't want tool tips or a resizeable toolbar
    m_wndToolBar.SetBarStyle ( m_wndToolBar.GetBarStyle ( ) |
                               CBRS_TOOLTIPS                |
                               CBRS_FLYBY                   |
                               CBRS_SIZE_DYNAMIC             ) ;

    return ( 0 ) ;
}

BOOL CMainFrame :: PreCreateWindow ( CREATESTRUCT & cs )
{
    ASSERT ( this ) ;

    // TODO: Modify the Window class or styles here by modifying
    //  the CREATESTRUCT cs

    return ( CMDIFrameWnd::PreCreateWindow ( cs ) ) ;
}

#ifdef _DEBUG
void CMainFrame :: AssertValid ( ) const
{
    ASSERT ( this ) ;

    CMDIFrameWnd::AssertValid ( ) ;
}

void CMainFrame :: Dump ( CDumpContext & dc ) const
{
    ASSERT ( this ) ;

    CMDIFrameWnd::Dump ( dc ) ;
}

#endif //_DEBUG

void CMainFrame :: OnEditIgnoredImage()
{
        CIgnoreDlg IgnoreDlg;
        if (IgnoreDlg.DoModal()==IDOK)
        {
        }
}
