/*----------------------------------------------------------------------
Debugging Applications for Microsoft .NET and Microsoft Windows
Copyright © 1997-2003 John Robbins -- All rights reserved.
----------------------------------------------------------------------*/
#include "stdafx.h"
#include "WDBG.h"
#include "DocNotifyWnd.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

BEGIN_MESSAGE_MAP(CDocNotifyWnd, CWnd)
    //{{AFX_MSG_MAP(CDocNotifyWnd)
    ON_MESSAGE ( WM_DBG_EXITPROCESSEVENT   , HandleExitProcessEvent )
    ON_MESSAGE ( WM_DBG_EXCEPTIONEVENT     , HandleExceptionEvent )
    ON_MESSAGE ( WM_DBG_CREATEPROCESSEVENT , HandleCreateProcessEvent )
    ON_MESSAGE ( WM_FORCE_A_GO             , ForceAGo )
    ON_MESSAGE ( WM_DBG_CREATETHREADEVENT  , HandleThreadCreateEvent )
    ON_MESSAGE ( WM_DBG_EXITTHREADEVENT    , HandleThreadExitEvent )
    ON_MESSAGE ( WM_DBG_LOADDLLEVENT       , HandleLoadDllEvent )
    ON_MESSAGE ( WM_DBG_UNLOADDLLEVENT     , HandleUnloadDllEvent )
    ON_MESSAGE ( WM_DBG_OUTPUTDEBUGSTRINGEVENT , HandleOutputDebugStringEvent )
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()


CDocNotifyWnd::CDocNotifyWnd ( )
{
    m_sClassName = AfxRegisterWndClass ( CS_NOCLOSE ) ;
}

CDocNotifyWnd::~CDocNotifyWnd()
{
}

BOOL CDocNotifyWnd :: Create ( CWDBGProjDoc * pDoc )
{
    m_pDoc = pDoc ;
    ASSERT_VALID ( m_pDoc ) ;
    ASSERT_KINDOF ( CWDBGProjDoc , m_pDoc ) ;

    BOOL bRetCreateEx ;
    bRetCreateEx = CreateEx ( 0                                  ,
                              m_sClassName                       ,
                              _T ( "DocNotifyWnd" )              ,
                              WS_POPUP                           ,
                              0 , 0 , 0 , 0                      ,
                              AfxGetApp()->GetMainWnd( )->m_hWnd ,
                              NULL                                ) ;
    ASSERT ( TRUE == bRetCreateEx ) ;
    return ( bRetCreateEx ) ;
}

LRESULT CDocNotifyWnd :: HandleExitProcessEvent ( WPARAM wParam ,
                                                  LPARAM lParam  )
{
    return ( m_pDoc->HandleProcessExitEvent ( (DWORD)wParam  ,
                                         (PSTEXITPROCESSEVENT)lParam ));
}

LRESULT CDocNotifyWnd :: HandleExceptionEvent ( WPARAM wParam ,
                                                LPARAM lParam  )
{
    return ( m_pDoc->HandleExceptionEvent ( (DWORD)wParam ,
                                         (PSTEXCEPTIONEVENT)lParam ) ) ;
}

LRESULT CDocNotifyWnd :: HandleCreateProcessEvent ( WPARAM wParam ,
                                                    LPARAM lParam  )
{
    return ( m_pDoc->HandleCreateProcessEvent ( (DWORD)wParam ,
                                     (PSTCREATEPROCESSEVENT)lParam ) ) ;
}

LRESULT CDocNotifyWnd :: ForceAGo ( WPARAM , LPARAM )
{
    // Give up the timeslice.
    Sleep ( 0 ) ;
    m_pDoc->OnDebugGo ( ) ;
    return ( 0 ) ;
}

LRESULT CDocNotifyWnd :: HandleThreadCreateEvent ( WPARAM wParam ,
                                                   LPARAM lParam )
{
    return ( m_pDoc->HandleThreadCreateEvent ( (DWORD)wParam ,
                                         (PSTCREATETHREADEVENT)lParam));
}

LRESULT CDocNotifyWnd :: HandleThreadExitEvent ( WPARAM wParam ,
                                                 LPARAM lParam )
{
    return ( m_pDoc->HandleThreadExitEvent ( (DWORD)wParam ,
                                          (PSTEXITTHREADEVENT)lParam));
}

LRESULT CDocNotifyWnd :: HandleLoadDllEvent ( WPARAM wParam ,
                                              LPARAM lParam  )
{
    return ( m_pDoc->HandleLoadDllEvent ( (DWORD)wParam                  ,
                                          (PSTLOADDLLEVENT)lParam  ) ) ;
}

LRESULT CDocNotifyWnd :: HandleUnloadDllEvent ( WPARAM wParam ,
                                                LPARAM lParam  )
{
    return ( m_pDoc->HandleUnloadedDllEvent ( (DWORD)wParam ,
                                            (PSTUNLOADDLLEVENT)lParam));
}

LRESULT CDocNotifyWnd ::
               HandleOutputDebugStringEvent ( WPARAM wParam ,
                                              LPARAM lParam  )
{
    return ( m_pDoc->HandleOutputDebugStringEvent ( (DWORD)wParam ,
                                (PSTOUTPUTDEBUGSTRINGEVENT)lParam  ) );
}
