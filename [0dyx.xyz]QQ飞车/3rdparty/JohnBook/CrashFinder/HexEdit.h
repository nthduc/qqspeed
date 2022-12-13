/*----------------------------------------------------------------------
"Debugging Applications" (Microsoft Press)
Copyright (c) 1997-2000 John Robbins -- All rights reserved.

With excellent updates from Scott Bloom, Ching Ming Kwok, 
Jeff Shanholtz, and Pablo Presedo!
----------------------------------------------------------------------*/
#ifndef _HEXEDIT_H
#define _HEXEDIT_H

class CHexEdit : public CEdit
{

public      :
    CHexEdit ( void ) ;

protected   :
    afx_msg void OnChar ( UINT nChar , UINT nRepCnt , UINT nFlags ) ;

    DECLARE_MESSAGE_MAP ( )

} ;

#endif      // _HEXEDIT_H


