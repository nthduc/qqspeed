/*----------------------------------------------------------------------
"Debugging Applications" (Microsoft Press)
Copyright (c) 1997-2000 John Robbins -- All rights reserved.

With excellent updates from Scott Bloom, Ching Ming Kwok, 
Jeff Shanholtz, and Pablo Presedo!
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
