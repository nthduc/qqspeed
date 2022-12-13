/*----------------------------------------------------------------------
"Debugging Applications" (Microsoft Press)
Copyright (c) 1997-2000 John Robbins -- All rights reserved.

With excellent updates from Scott Bloom, Ching Ming Kwok, 
Jeff Shanholtz, and Pablo Presedo!
----------------------------------------------------------------------*/
#if !defined ( _CRASHFINDERDOC_H )
#define _CRASHFINDERDOC_H

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include "BinaryImage.h"

#include "SymbolEngine.h"


// CCrashFinderDoc::LookUpAddress return values.
typedef enum tagELURES
{
    eLUFAILED   = 0x0 ,
    eLUMOD      = 0x1 ,
    eLUSYM      = 0x2 ,
    eLULINE     = 0x4
} ELURES ;


class CCrashFinderDoc : public CDocument
{
protected   :

    CCrashFinderDoc ( ) ;

    DECLARE_DYNCREATE ( CCrashFinderDoc )

public      :

public      :
    virtual BOOL OnNewDocument ( ) ;
    virtual BOOL OnOpenDocument ( LPCTSTR lpszPathName ) ;
    virtual void OnCloseDocument ( ) ;
    virtual void Serialize ( CArchive & ar ) ;

public      :
    virtual ~CCrashFinderDoc ( ) ;

#ifdef _DEBUG
    virtual void AssertValid ( ) const ;
    virtual void Dump ( CDumpContext & dc ) const ;
#endif

    // Called by the view to tell the document that something new is
    // selected.
    void SelectionChanged ( TVITEM & stTVI ) ;

    // Called when the global application options change.
    void ShowFullPaths ( BOOL bShowFull ) ;

    // A helper function to look up the specified address and fill in
    // the symbol data structures.
    int LookUpAddress ( DWORD              dwAddr     ,
                        CString &          sModule    ,
                        IMAGEHLP_SYMBOL &  stIHSym    ,
                        DWORD &            dwFnDispl  ,
                        IMAGEHLP_LINE &    stIHLine   ,
                        DWORD &            dwSrcDispl  ) ;



protected   :
    // The function that does the shared initialization for new and
    // opened documents.
    virtual BOOL SharedInitialization ( void ) ;

    // The function that does the actual work to load a binary image
    // into the symbol engine and show it in the tree.
    BOOL LoadAndShowImage ( CBinaryImage * pImage               ,
                            BOOL           bModifiesDoc = TRUE   ) ;

    // A helper to get the current view for the document.
    CView * GetCurrentView ( void ) ;

    // A helper for those UI updates that are the same such as the
    // remove binary image and set binary image properties.
    void CommonItemsUIUpdate ( CCmdUI * pCmdUI ) ;

    // A helper to check if a load address is already in the project.
    // This returns TRUE if the address is a conflicting address.
    BOOL IsConflictingLoadAddress ( DWORD dwAddr , int & iIndex ) ;

    // Generated message map functions
protected   :
    afx_msg void OnEditFindCrash ( ) ;
    afx_msg void OnEditAddImage ( ) ;
    afx_msg void OnEditRemoveImage ( ) ;
    afx_msg void OnEditImageProperties ( ) ;
    afx_msg void OnUpdateEditRemoveImage ( CCmdUI * pCmdUI ) ;
    afx_msg void OnUpdateEditImageProperties ( CCmdUI * pCmdUI ) ;

    DECLARE_MESSAGE_MAP ( )

////////////////////////////////////////////////////////////////////////
//                       Protected Data Members
////////////////////////////////////////////////////////////////////////
protected   :
    // The tree control from the tree view.
    CTreeCtrl *     m_pcTreeControl ;
    // The tree view itself.
    CTreeView *     m_pcTreeView    ;

    // The edit control from the edit view.
    CEdit *         m_pcEditControl ;
    // The edit view itself.
    CEditView *     m_pcEditView    ;
    // The font for the edit control.
    CFont *         m_pcFnt         ;

    // The data structure that holds all the items.
    CObArray        m_cDataArray    ;

    // The symbol engine itself for this instance of the document.
    CSymbolEngine   m_cSymEng       ;

        CString                 m_strAddresses;
} ;

//{{AFX_INSERT_LOCATION}}

#endif // !_CRASHFINDERDOC_H
