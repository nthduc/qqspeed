/*----------------------------------------------------------------------
"Debugging Applications" (Microsoft Press)
Copyright (c) 1997-2000 John Robbins -- All rights reserved.

With excellent updates from Scott Bloom, Ching Ming Kwok, 
Jeff Shanholtz, and Pablo Presedo!
----------------------------------------------------------------------*/
#if !defined ( _CRASHFINDEREDITVIEW_H )
#define _CRASHFINDEREDITVIEW_H

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

class CCrashFinderEditView : public CEditView
{
protected   :

    CCrashFinderEditView ( ) ;

    //lint -e1511
    DECLARE_DYNCREATE ( CCrashFinderEditView )
    //lint +e1511

// Attributes
public      :
    //lint -e1511
    CCrashFinderDoc * GetDocument ( ) ;
    //lint +e1511

// Operations
public      :

    // Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CCrashFinderEditView)
public      :

protected   :
    //}}AFX_VIRTUAL

    // Implementation
public      :
    virtual ~CCrashFinderEditView ( ) ;

#ifdef _DEBUG
    virtual void AssertValid ( ) const ;
    virtual void Dump ( CDumpContext & dc ) const ;
#endif

protected   :
    int OnCreate ( LPCREATESTRUCT lpcs ) ;

    // Generated message map functions
protected   :

    //{{AFX_MSG(CCrashFinderEditView)
    //}}AFX_MSG

    DECLARE_MESSAGE_MAP ( )
} ;

#ifndef _DEBUG  // debug version in CrashFinderView.cpp
inline CCrashFinderDoc * CCrashFinderEditView :: GetDocument ( )
{
    return ( (CCrashFinderDoc *)m_pDocument ) ;
}
#endif

//{{AFX_INSERT_LOCATION}}

#endif // !_CRASHFINDEREDITVIEW_H
