/*----------------------------------------------------------------------
"Debugging Applications" (Microsoft Press)
Copyright (c) 1997-2000 John Robbins -- All rights reserved.

With excellent updates from Scott Bloom, Ching Ming Kwok, 
Jeff Shanholtz, and Pablo Presedo!
----------------------------------------------------------------------*/
#include "stdafx.h"
#include "CrashFinder.h"

#include "CrashFinderDoc.h"
#include "CrashFinderEditView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE [] = __FILE__ ;
#endif

IMPLEMENT_DYNCREATE ( CCrashFinderEditView , CEditView )

BEGIN_MESSAGE_MAP ( CCrashFinderEditView , CEditView )
    //{{AFX_MSG_MAP(CCrashFinderEditView)
    ON_WM_CREATE ( )
    //}}AFX_MSG_MAP
END_MESSAGE_MAP ( )

CCrashFinderEditView :: CCrashFinderEditView ( )
                      : CEditView ( )
{
    ASSERT ( this ) ;
}

CCrashFinderEditView :: ~CCrashFinderEditView ( )
{
    ASSERT ( this ) ;
}

int CCrashFinderEditView :: OnCreate ( LPCREATESTRUCT lpcs )
{
    ASSERT ( this ) ;
    int iRet = CEditView::OnCreate ( lpcs ) ;
    GetEditCtrl().SetReadOnly ( TRUE ) ;
    return ( iRet ) ;
}


#ifdef _DEBUG
void CCrashFinderEditView :: AssertValid ( ) const
{
    ASSERT ( this ) ;
    CView::AssertValid ( ) ;
}

void CCrashFinderEditView :: Dump ( CDumpContext & dc ) const
{
    ASSERT ( this ) ;
    CView::Dump ( dc ) ;
}

CCrashFinderDoc * CCrashFinderEditView::GetDocument ( )
{
    ASSERT ( m_pDocument->IsKindOf ( RUNTIME_CLASS( CCrashFinderDoc )));
    return ( (CCrashFinderDoc *)m_pDocument ) ;
}
#endif //_DEBUG


