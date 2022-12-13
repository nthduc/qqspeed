/*----------------------------------------------------------------------
"Debugging Applications" (Microsoft Press)
Copyright (c) 1997-2000 John Robbins -- All rights reserved.

With excellent updates from Scott Bloom, Ching Ming Kwok, 
Jeff Shanholtz, and Pablo Presedo!
----------------------------------------------------------------------*/
#include "stdafx.h"
#include "CrashFinder.h"
#include "CrashFinderDoc.h"
#include "CrashFinderTreeView.h"
#include "CrashFinderEditView.h"
#include "ChildFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE [] = __FILE__ ;
#endif

IMPLEMENT_DYNCREATE ( CChildFrame , CMDIChildWnd )

BEGIN_MESSAGE_MAP ( CChildFrame , CMDIChildWnd )
//{{AFX_MSG_MAP(CChildFrame)
//}}AFX_MSG_MAP
END_MESSAGE_MAP ( )

CChildFrame :: CChildFrame ( )
             : CMDIChildWnd ( ) , m_wndSplitter ( )
{
    ASSERT ( this ) ;
}

CChildFrame :: ~CChildFrame ( )
{
    ASSERT ( this ) ;
}

BOOL CChildFrame :: PreCreateWindow ( CREATESTRUCT & cs )
{
    ASSERT ( this ) ;

    return ( CMDIChildWnd::PreCreateWindow ( cs ) ) ;
}

#ifdef _DEBUG
void CChildFrame :: AssertValid ( ) const
{
    ASSERT ( this ) ;

    CMDIChildWnd::AssertValid ( ) ;
}

void CChildFrame :: Dump ( CDumpContext & dc ) const
{
    ASSERT ( this ) ;

    CMDIChildWnd::Dump ( dc ) ;
}

#endif //_DEBUG

BOOL CChildFrame :: OnCreateClient ( LPCREATESTRUCT   /*lpcs*/ ,
                                     CCreateContext * pContext  )
{
    ASSERT ( this ) ;

    // Create the static splitter.
    if ( FALSE == m_wndSplitter.CreateStatic ( this , 1 , 2 ) )
    {
        return ( FALSE ) ;
    }
    // Make the left hand view the tree control.
    if ( FALSE ==
          m_wndSplitter.CreateView ( 0                  ,
                                     0                  ,
                                    RUNTIME_CLASS(CCrashFinderTreeView),
                                     CSize ( 128 , 0 )  ,
                                     pContext            ) )
    {
        return ( FALSE ) ;
    }
    // Make the right hand view the edit control.
    if ( FALSE ==
            m_wndSplitter.CreateView ( 0                ,
                                       1                ,
                                    RUNTIME_CLASS(CCrashFinderEditView),
                                       CSize ( 0 , 0 )  ,
                                       pContext          ) )
    {
        return ( FALSE ) ;
    }
    return ( TRUE ) ;
}

