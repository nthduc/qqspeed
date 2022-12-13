/*----------------------------------------------------------------------
Debugging Applications for Microsoft .NET and Microsoft Windows
Copyright © 1997-2003 John Robbins -- All rights reserved.
----------------------------------------------------------------------*/
#ifndef _SCRIPTOPTIONS_H
#define _SCRIPTOPTIONS_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ScriptOptions.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CScriptOptions dialog

class CScriptOptions : public CDialog
{
// Construction
public:
    CScriptOptions(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
    //{{AFX_DATA(CScriptOptions)
	enum { IDD = IDD_SCRIPTOPTIONS };
    int     m_iMinPixelDrag;
    BOOL    m_bRecordAllMouseMoves;
    BOOL    m_bRecordFocusChangesWithMouse;
    //BOOL    m_bUseAbsoluteCoordinates;
    DWORD   m_dwSleepTime;
    int     m_iLanguageChoice;
	BOOL	m_bDoChildFocus;
	BOOL	m_bMultiMon;
	//}}AFX_DATA


// Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CScriptOptions)
    protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
    //}}AFX_VIRTUAL

// Implementation
protected:

    // Generated message map functions
    //{{AFX_MSG(CScriptOptions)
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // _SCRIPTOPTIONS_H
