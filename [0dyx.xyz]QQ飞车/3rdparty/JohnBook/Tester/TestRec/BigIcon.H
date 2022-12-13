/*----------------------------------------------------------------------
Debugging Applications for Microsoft .NET and Microsoft Windows
Copyright © 1997-2003 John Robbins -- All rights reserved.
----------------------------------------------------------------------*/

#ifndef _BIGICON_H
#define _BIGICON_H

// The BigIcon lifted straight from SuperPad.

class CBigIcon : public CButton
{
    // Attributes
 public :

    // Operations
 public :
    void SizeToContent ( ) ;

    // Implementation
 protected :
    virtual void DrawItem ( LPDRAWITEMSTRUCT lpDrawItemStruct ) ;

    // Message handler.
    afx_msg BOOL OnEraseBkgnd ( CDC * pDC ) ;

    DECLARE_MESSAGE_MAP ( )
} ;

#endif  // _BIGICON_H
