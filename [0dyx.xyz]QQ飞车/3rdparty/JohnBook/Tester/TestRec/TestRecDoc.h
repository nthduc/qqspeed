/*----------------------------------------------------------------------
Debugging Applications for Microsoft .NET and Microsoft Windows
Copyright © 1997-2003 John Robbins -- All rights reserved.
----------------------------------------------------------------------*/
#ifndef _TESTRECDOC_H
#define _TESTRECDOC_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "RecordingEngine.h"
#include "HookCode.h"

class CTestRecDoc : public CDocument
{
protected: // create from serialization only
    CTestRecDoc();
    DECLARE_DYNCREATE(CTestRecDoc)

// Attributes
public:

// Operations
public:

// Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CTestRecDoc)
    public:
    virtual BOOL OnNewDocument();
    virtual void Serialize(CArchive& ar);
    virtual BOOL OnOpenDocument(LPCTSTR lpszPathName);
    //}}AFX_VIRTUAL

// Implementation
public:
    virtual ~CTestRecDoc();
#ifdef _DEBUG
    virtual void AssertValid() const;
    virtual void Dump(CDumpContext& dc) const;
#endif

    void StopRecording ( void ) ;

    void AppendText ( LPCTSTR szText ) ;

    BOOL IsRecording ( void )
    {
        return ( m_bIsRecording ) ;
    }

    DWORD GetMinSleepTime ( )
    {
        return ( m_dwMinSleepMilliseconds ) ;
    }

    void LoadUpSettings ( void ) ;

    BOOL IsDefaultLanguageJavaScript ( void ) ;

protected:

    // The flag that indicates the state.
    BOOL m_bIsRecording ;
    // The original window title before recording took place.
    CString m_cWindowTitle ;
    // The title to use when recording.
    CString m_cRecordingTitle ;

    CRecordingUIOutput * m_pOutput ;
    CRecordingEngine * m_pRecEngine ;

    // The minimum amount of time to sleep.
    DWORD m_dwMinSleepMilliseconds ;

// Generated message map functions
protected:
    //{{AFX_MSG(CTestRecDoc)
    afx_msg void OnScriptRecord();
    afx_msg void OnScriptOptions();
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
};

////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}

#endif // !_TESTRECDOC_H
