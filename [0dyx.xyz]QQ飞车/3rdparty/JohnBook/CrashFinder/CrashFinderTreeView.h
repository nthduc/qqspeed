/*----------------------------------------------------------------------
"Debugging Applications" (Microsoft Press)
Copyright (c) 1997-2000 John Robbins -- All rights reserved.

With excellent updates from Scott Bloom, Ching Ming Kwok, 
Jeff Shanholtz, and Pablo Presedo!
----------------------------------------------------------------------*/
#if !defined ( _CRASHFINDERTREEVIEW_H )
#define _CRASHFINDERTREEVIEW_H

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

class CCrashFinderTreeView : public CTreeView
{
protected   :

    CCrashFinderTreeView ( ) ;

    //lint -e1511
    DECLARE_DYNCREATE ( CCrashFinderTreeView )
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
    //{{AFX_VIRTUAL(CCrashFinderTreeView)
public      :
protected   :
    afx_msg void OnSelChanged ( NMHDR * pnmh , LRESULT * pResult ) ;
    //}}AFX_VIRTUAL

    // Implementation
public      :
    virtual ~CCrashFinderTreeView ( ) ;

#ifdef _DEBUG
    virtual void AssertValid ( ) const ;
    virtual void Dump ( CDumpContext & dc ) const ;
#endif

protected   :

    // Generated message map functions
protected   :

    //{{AFX_MSG(CCrashFinderTreeView)
    //}}AFX_MSG

    DECLARE_MESSAGE_MAP ( )
} ;

#ifndef _DEBUG  // debug version in CrashFinderView.cpp
inline CCrashFinderDoc * CCrashFinderTreeView :: GetDocument ( )
{
    return ( (CCrashFinderDoc *)m_pDocument ) ;
}
#endif

//{{AFX_INSERT_LOCATION}}

#endif // !_CRASHFINDERTREEVIEW_H
