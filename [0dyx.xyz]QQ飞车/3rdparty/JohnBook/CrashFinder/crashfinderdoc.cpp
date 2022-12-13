/*----------------------------------------------------------------------
"Debugging Applications" (Microsoft Press)
Copyright (c) 1997-2000 John Robbins -- All rights reserved.

With excellent updates from Scott Bloom, Ching Ming Kwok, 
Jeff Shanholtz, and Pablo Presedo!
----------------------------------------------------------------------*/
#include "stdafx.h"
#include "CrashFinder.h"

#include "CrashFinderDoc.h"

#include "FindCrashDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE [] = __FILE__ ;
#endif

// The image list bitmap dimensions.
#define BITMAP_WIDTH    16
#define BITMAP_HEIGHT   15

// The defines for the validated and loaded states that are shown in the
// tree.
#define STATE_NOTVALID      0
#define STATE_VALIDATED     1

IMPLEMENT_DYNCREATE ( CCrashFinderDoc , CDocument )

BEGIN_MESSAGE_MAP ( CCrashFinderDoc , CDocument )
  //{{AFX_MSG_MAP(CCrashFinderDoc)
    ON_COMMAND ( ID_EDIT_FIND_CRASH         , OnEditFindCrash       )
    ON_COMMAND ( ID_EDIT_ADD_IMAGE          , OnEditAddImage        )
    ON_COMMAND ( ID_EDIT_REMOVE_IMAGE       , OnEditRemoveImage     )
    ON_COMMAND ( ID_EDIT_IMAGE_PROPERTIES   , OnEditImageProperties )
        ON_UPDATE_COMMAND_UI ( ID_EDIT_REMOVE_IMAGE     ,
                           OnUpdateEditRemoveImage                  )
    ON_UPDATE_COMMAND_UI ( ID_EDIT_IMAGE_PROPERTIES     ,
                           OnUpdateEditImageProperties              )
  //}}AFX_MSG_MAP
END_MESSAGE_MAP ( )

CCrashFinderDoc :: CCrashFinderDoc ( )
                 : CDocument ( )    ,
                   m_cDataArray ( ) ,
                   m_cSymEng ( )
{
    ASSERT ( this ) ;

    m_pcTreeControl = NULL ;
    m_pcEditControl = NULL ;
    m_pcTreeView    = NULL ;
    m_pcEditView    = NULL ;
    m_pcFnt         = NULL ;
}

CCrashFinderDoc :: ~CCrashFinderDoc ( )
{
    ASSERT ( this ) ;
}

BOOL CCrashFinderDoc :: OnNewDocument ( )
{
    ASSERT ( this ) ;

    if ( ( TRUE == SharedInitialization ( )     ) &&
         ( TRUE == CDocument::OnNewDocument ( ) )    )
    {
        // Let the user start adding binaries immediately.
        OnEditAddImage ( ) ;
        // Force the selection to the first item, if possible.
        HTREEITEM hItem = m_pcTreeControl->GetRootItem ( ) ;
        if ( NULL != hItem )
        {
            m_pcTreeControl->SelectItem ( hItem ) ;
        }
        return ( TRUE ) ;
    }
    return ( FALSE ) ;
}

BOOL CCrashFinderDoc :: OnOpenDocument ( LPCTSTR lpszPathName )
{
    ASSERT ( this ) ;

    if ( ( TRUE == SharedInitialization ( )                   ) &&
         ( TRUE == CDocument::OnOpenDocument ( lpszPathName ) )   )
    {
        return ( TRUE ) ;
    }
    return ( FALSE ) ;
}

void CCrashFinderDoc :: OnCloseDocument ( )
{
    ASSERT ( this ) ;

    int iCount = m_cDataArray.GetSize ( ) ;
    for ( int i = 0 ; i < iCount ; i++ )
    {
        CBinaryImage * pImage = (CBinaryImage *)m_cDataArray[ i ] ;
        ASSERT ( NULL != pImage ) ;
        delete pImage ;
    }

    // Clean out the image list I set earlier, if the tree control is a
    // valid window.
    if ( TRUE == ::IsWindow ( m_pcTreeControl->m_hWnd ) )
    {
        CImageList * pImageList =
                            m_pcTreeControl->GetImageList(TVSIL_NORMAL);
        if ( NULL != pImageList )
        {
            pImageList->DeleteImageList ( ) ;
            delete pImageList ;
        }
    }

    // Get rid of the font I created for the edit control earlier.
    if ( NULL != m_pcFnt )
    {
        delete m_pcFnt ;
    }

    // Clean up the symbol engine.
    BOOL bRet = m_cSymEng.SymCleanup ( ) ;
    if ( FALSE == bRet )
    {
        ASSERT ( TRUE == bRet ) ;
        TRACE ( "m_cSymEng.SymCleanup FAILED!!!\n" ) ;
    }


    // Now that I have done my cleanup, let the main framework do it's
    // cleanup.
    CDocument::OnCloseDocument ( ) ;
}

BOOL CCrashFinderDoc :: SharedInitialization ( void )
{
    // BIG NOTE!
    // This function is called BEFORE the main framework functions for
    // document creation and loading.  Only things that rely on the
    // views can be done here.  Don't try and access any of the
    // document data as it is not valid yet.

    ASSERT ( this ) ;

    // Loop through the views looking for the one with the tree view.
    // if there are no tree views, then initialization must fail.
    POSITION pos = GetFirstViewPosition ( ) ;
    while ( NULL != pos )
    {
        CView * pView = GetNextView ( pos ) ;
        ASSERT ( NULL != pView ) ;

        if ( TRUE == pView->IsKindOf ( RUNTIME_CLASS ( CTreeView ) ) )
        {
            m_pcTreeView = (CTreeView*)pView ;
            m_pcTreeControl = &m_pcTreeView->GetTreeCtrl ( ) ;
            ASSERT ( NULL != m_pcTreeControl ) ;

            if ( NULL == m_pcTreeControl )
            {
                return ( FALSE ) ;
            }
        }
        if ( TRUE ==pView->IsKindOf ( RUNTIME_CLASS ( CEditView ) ) )
        {
            m_pcEditView = (CEditView*)pView ;
            m_pcEditControl = &m_pcEditView->GetEditCtrl ( ) ;
            ASSERT ( NULL != m_pcEditControl ) ;

            if ( NULL == m_pcEditControl )
            {
                return ( FALSE ) ;
            }
        }
    }

    ASSERT ( NULL != m_pcTreeControl ) ;
    ASSERT ( NULL != m_pcEditControl ) ;

    if ( ( NULL == m_pcTreeControl ) || ( NULL == m_pcEditControl ) )
    {
        return ( FALSE ) ;
    }

    // Set the font for the edit control.  This should eventually become
    // a system wide option.
    m_pcFnt = new CFont ;
    BOOL bRet = m_pcFnt->CreatePointFont ( 100 , _T ( "Courier New" ) ) ;
    ASSERT ( bRet ) ;
    m_pcEditControl->SetFont ( m_pcFnt , TRUE ) ;

    // Add the image list to the tree control.
    CImageList * pImageList = new CImageList ( ) ;

    VERIFY ( pImageList->Create ( BITMAP_WIDTH  ,
                                  BITMAP_HEIGHT ,
                                  ILC_MASK      ,
                                  2             ,
                                  4              ) ) ;

    CBitmap cBitmap ;
    int     iRet    ;

    VERIFY ( cBitmap.LoadBitmap ( IDB_BADITEM ) ) ;

    iRet = pImageList->Add ( &cBitmap , (COLORREF)0xFFFFFF ) ;
    ASSERT ( -1 != iRet ) ;

    cBitmap.DeleteObject ( ) ;

    VERIFY ( cBitmap.LoadBitmap ( IDB_GOODITEM ) ) ;

    iRet = pImageList->Add ( &cBitmap , (COLORREF)0xFFFFFF ) ;
    ASSERT ( -1 != iRet ) ;

    cBitmap.DeleteObject ( ) ;

    m_pcTreeControl->SetImageList ( pImageList , TVSIL_NORMAL ) ;

    // Finally, initialize the symbol engine.  Since I am not using the
    // symbol engine with a real process, I will generate a unique
    // value just for this document to use as the hProcess.
    srand ( (unsigned)time ( NULL ) ) ;
    HANDLE hRandHandle = (HANDLE)rand ( ) ;

    bRet = m_cSymEng.SymInitialize ( hRandHandle , NULL , FALSE ) ;
    if ( FALSE == bRet )
    {
        ASSERT ( TRUE == bRet ) ;
        TRACE ( "m_cSymEng.SymInitialize FAILED!!!\n" ) ;
        return ( FALSE ) ;
    }

    // Set the symbol engine to load line information.  This must be
    // because the symbol engine does not load source-lines by default.
    // I also turn on the OMAP lookups so that the super-special OMAP
    // moved blocks will get looked at as well.  Trust me, don't ask.
    DWORD dwOpts = SymGetOptions ( ) ;
    SymSetOptions ( dwOpts                   |
                    SYMOPT_LOAD_LINES        |
                    SYMOPT_OMAP_FIND_NEAREST  ) ;

    return ( TRUE ) ;
}

BOOL CCrashFinderDoc :: LoadAndShowImage ( CBinaryImage * pImage       ,
                                           BOOL           bModifiesDoc )
{
    // Check the assumptions from outside the function.
    ASSERT ( this ) ;
    ASSERT ( NULL != m_pcTreeControl ) ;

    // A string that can be used for any user messages
    CString   sMsg                    ;
    // The state for the tree graphic
    int       iState = STATE_NOTVALID ;
    // A Boolean return value holder
    BOOL      bRet                    ;

    // Make sure the parameter is good.
    ASSERT ( NULL != pImage ) ;
    if ( NULL == pImage )
    {
        // Nothing much can happen with a bad pointer.
        return ( FALSE ) ;
    }

    // Check to see whether this image is valid. If it is, make sure
    // that it isn't already in the list and that it doesn't have
    // a conflicting load address. If it isn't a valid image, I add
    // it anyway because it isn't good form just to throw out user
    // data. If the image is bad, I just show it with the invalid
    // bitmap and don't load it into the symbol engine.
    if ( TRUE == pImage->IsValidImage ( ) )
    {

        // Here I walk through the items in the data array so that I can
        // look for three problem conditions:
        // 1. The binary image is already in the list. If so, I can
        //    only abort.
        // 2. The binary is going to load at an address that's already
        //    in the list. If that's the case, I'll display the
        //    Properties dialog box for the binary image so that its
        //    load address can be changed before adding it to the list.
        // 3. The project already includes an EXE image, and pImage is
        //     also an executable.

        // I always start out assuming that the data in pImage is valid.
        // Call me an optimist!
        BOOL bValid = TRUE ;
        int iCount = m_cDataArray.GetSize ( ) ;
        for ( int i = 0 ; i < iCount ; i++ )
        {
            CBinaryImage * pTemp = (CBinaryImage *)m_cDataArray[ i ] ;

            ASSERT ( NULL != pTemp ) ;
            if ( NULL == pTemp )
            {
                // Not much can happen with a bad pointer!
                return ( FALSE ) ;
            }

            // Do these two CString values match?
            if ( pImage->GetFullName ( ) == pTemp->GetFullName ( ) )
            {
                // Tell the user!!
                sMsg.FormatMessage ( IDS_DUPLICATEFILE      ,
                                     pTemp->GetFullName ( )  ) ;
                AfxMessageBox ( sMsg ) ;

                return ( FALSE ) ;
            }

            // If the current image from the data structure isn't
            // valid, I'm up a creek. Although I can check
            // duplicate names above, it's hard to check load
            // addresses and EXE characteristics. If pTemp isn't valid,
            // I have to skip these checks. Skipping them can lead
            // to problems, but since pTemp is marked in the list as
            // invalid, it's up to the user to reset the properties.
            if ( TRUE == pTemp->IsValidImage ( FALSE ) )
            {

                // Check that I don't add two EXEs to the project.
                if ( 0 == ( IMAGE_FILE_DLL &
                            pTemp->GetCharacteristics ( ) ) )
                {
                    if ( 0 == ( IMAGE_FILE_DLL &
                                pImage->GetCharacteristics ( ) ) )

                    {
                        // Tell the user!!
                        sMsg.FormatMessage ( IDS_EXEALREADYINPROJECT ,
                                             pImage->GetFullName ( ) ,
                                             pTemp->GetFullName ( )   ) ;
                        AfxMessageBox ( sMsg ) ;
                        // Trying to load two images marked as EXEs will
                        // automatically have the data thrown out for
                        // pImage.
                        return ( FALSE ) ;
                    }
                }

                // Check for load address conflicts.
                if ( pImage->GetLoadAddress ( ) ==
                     pTemp->GetLoadAddress( )      )
                {
                    sMsg.FormatMessage ( IDS_DUPLICATELOADADDR      ,
                                         pImage->GetFullName ( )    ,
                                         pTemp->GetFullName ( )      ) ;

                    if ( IDYES == AfxMessageBox ( sMsg , MB_YESNO ) )
                    {
                        // The user wants to change the properties by
                        // hand.
                        pImage->SetProperties ( ) ;

                        // Check that the load address really did
                        // change and that it doesn't now conflict with
                        // another binary.
                        int iIndex ;
                        if ( TRUE ==
                                IsConflictingLoadAddress (
                                               pImage->GetLoadAddress(),
                                               iIndex                 ))
                        {
                            sMsg.FormatMessage
                                          ( IDS_DUPLICATELOADADDRFINAL ,
                                            pImage->GetFullName ( )    ,
                  ((CBinaryImage*)m_cDataArray[iIndex])->GetFullName());
                            AfxMessageBox ( sMsg ) ;

                            // The data in pImage isn't valid, so go
                            //  ahead and exit the loop.
                            bValid = FALSE ;
                            break ;
                        }
                    }
                    else
                    {
                        // The data in pImage isn't valid, so go
                        // ahead and exit the loop.
                        bValid = FALSE ;
                        break ;
                    }
                }
            }
        }
        if ( TRUE == bValid )
        {
            // This image is good (at least up to the symbol load).
            iState = STATE_VALIDATED ;
        }
        else
        {
            iState = STATE_NOTVALID ;
        }
    }
    else
    {
        // This image isn't valid.
        iState = STATE_NOTVALID ;
    }

    if ( STATE_VALIDATED == iState )
    {
        // Try to load this image into the symbol engine.
        bRet =
           m_cSymEng.SymLoadModule(NULL                                ,
                                   (PSTR)(LPCSTR)pImage->GetFullName() ,
                                   NULL                                ,
                                   pImage->GetLoadAddress ( )          ,
                                   0                                  );
        // Watch out.  SymLoadModule returns the load address of the
        //  image, not TRUE.
        ASSERT ( FALSE != bRet ) ;
        if ( FALSE == bRet )
        {
            TRACE ( "m_cSymEng.SymLoadModule failed!!\n" ) ;
            iState = STATE_NOTVALID ;
        }
        else
        {
            iState = STATE_VALIDATED ;
        }
    }

    // Set the extra data value for pImage to the state of the symbol
    // load.
    if ( STATE_VALIDATED == iState )
    {
        pImage->SetExtraData ( TRUE ) ;
    }
    else
    {
        pImage->SetExtraData ( FALSE ) ;
    }

    // Put this item into the array.
    m_cDataArray.Add ( pImage ) ;

    // Does adding the item modify the document?
    if ( TRUE == bModifiesDoc )
    {
        SetModifiedFlag ( ) ;
    }

    CCrashFinderApp * pApp = (CCrashFinderApp*)AfxGetApp ( ) ;
    ASSERT ( NULL != pApp ) ;

    // Put the string into the tree.
    HTREEITEM hItem =
        m_pcTreeControl->InsertItem ( pApp->ShowFullPaths ( )
                                        ? pImage->GetFullName ( )
                                        : pImage->GetName ( )       ,
                                      iState                        ,
                                      iState                         ) ;
    ASSERT ( NULL != hItem ) ;

    // Put a pointer to the image in the item data. The pointer
    // makes it easy to update the module symbol information whenever
    // the view changes.
    bRet = m_pcTreeControl->SetItemData ( hItem , (DWORD)pImage ) ;
    ASSERT ( bRet ) ;

    // Force the item to be selected.
    bRet = m_pcTreeControl->SelectItem ( hItem ) ;

    // All OK, Jumpmaster!
    return ( bRet ) ;
}

// A helper to check if a load address is already in the project.  This
// returns TRUE if the address is a conflicting address and iIndex is
// the one that it conflicts with.
BOOL CCrashFinderDoc :: IsConflictingLoadAddress ( DWORD dwAddr ,
                                                   int & iIndex  )
{
    ASSERT ( this ) ;

    CBinaryImage * pImage ;
    int iCount = m_cDataArray.GetSize ( ) ;

    for ( iIndex = 0 ; iIndex < iCount ; iIndex++ )
    {
        pImage = (CBinaryImage*)m_cDataArray[ iIndex ] ;
        if ( dwAddr == pImage->GetLoadAddress ( ) )
        {
            return ( TRUE ) ;
        }
    }
    return ( FALSE ) ;
}

int CCrashFinderDoc :: LookUpAddress ( DWORD              dwAddr     ,
                                       CString &          sModule    ,
                                       IMAGEHLP_SYMBOL &  stIHSym    ,
                                       DWORD &            dwFnDispl  ,
                                       IMAGEHLP_LINE &    stIHLine   ,
                                       DWORD &            dwSrcDispl  )
{
    ASSERT ( this ) ;

    // Check that there there are things in the symbol engine.
    if ( 0 == m_cDataArray.GetSize ( ) )
    {
                CString sMsg;
        VERIFY ( sMsg.LoadString ( IDS_NOMODULESLOADED ) ) ;
        AfxMessageBox ( sMsg ) ;
        return ( -1 ) ;
    }

    BOOL bSymFound  = TRUE ;
    BOOL bLineFound = TRUE ;
    BOOL bModFound  = TRUE ;

    // Try and find the module.  If this cannot be found, then it is no
    // use looking anything else up.
    CImageHlp_Module cMod ;
    bModFound = m_cSymEng.SymGetModuleInfo ( dwAddr , &cMod ) ;
    if ( FALSE == bModFound )
    {
        bSymFound = FALSE ;
        bLineFound = FALSE ;
    }
    else
    {
        // Pull out the module.
        sModule = cMod.ImageName ;

        // Look up the symbol information.
        BOOL bRet = m_cSymEng.SymGetSymFromAddr ( dwAddr        ,
                                                  &dwFnDispl    ,
                                                  &stIHSym       ) ;
        if ( FALSE == bRet )
        {
            bSymFound = FALSE ;
        }

        // Look up the line stuff.
        bRet = m_cSymEng.SymGetLineFromAddr ( dwAddr        ,
                                              &dwSrcDispl   ,
                                              &stIHLine      ) ;
        if ( FALSE == bRet )
        {
            bLineFound = FALSE ;
        }
    }

    int iRet = 0 ;

    if ( TRUE == bSymFound )
    {
        iRet |= eLUSYM ;
    }
    if ( TRUE == bLineFound )
    {
        iRet |= eLULINE ;
    }
    if ( TRUE == bModFound )
    {
        iRet |= eLUMOD ;
    }

    return ( iRet ) ;
}


// Called when the global application options change.
void CCrashFinderDoc :: ShowFullPaths ( BOOL bShowFull )
{
    ASSERT ( this ) ;
    ASSERT ( NULL != m_pcTreeControl ) ;

    // Here I will just loop through each item in the tree control and
    // using the CBinaryImage pointer stored in each tree item's extra
    // data, just change the node.
    HTREEITEM hItem = m_pcTreeControl->GetRootItem ( ) ;
    while ( NULL != hItem )
    {
        CBinaryImage * pImage =
                (CBinaryImage *)m_pcTreeControl->GetItemData ( hItem ) ;
        ASSERT ( NULL != pImage ) ;

        m_pcTreeControl->SetItemText ( hItem                         ,
                                       bShowFull
                                            ? pImage->GetFullName()
                                            : pImage->GetName()       );
        hItem = m_pcTreeControl->GetNextItem ( hItem , TVGN_NEXT ) ;
    }
}

void CCrashFinderDoc :: Serialize ( CArchive & ar )
{
    ASSERT ( this ) ;

    int            iCount  ;
    CBinaryImage * pImage  ;

    if ( ar.IsStoring ( ) )
    {
        // Write out the count of items.
        iCount = m_cDataArray.GetSize ( ) ;
        ar << iCount ;
        // Now loop through and write out each item in the array.
        for ( int i = 0 ; i < iCount ; i++ )
        {
            pImage = (CBinaryImage*)m_cDataArray[ i ] ;
            ar << pImage ;
        }
    }
    else
    {
        ar >> iCount ;
        for ( int i = 0 ; i < iCount ; i++ )
        {
            ar >> pImage ;
            LoadAndShowImage ( pImage , FALSE ) ;
        }
        // Force the selection to the first item, if possible.
        HTREEITEM hItem = m_pcTreeControl->GetRootItem ( ) ;
        if ( NULL != hItem )
        {
            m_pcTreeControl->SelectItem ( hItem ) ;
        }
    }
}

CView * CCrashFinderDoc :: GetCurrentView ( void )
{
    CMDIFrameWnd * pFrame = (CMDIFrameWnd*)AfxGetApp( )->m_pMainWnd ;
    ASSERT ( NULL != pFrame ) ;

    if ( NULL == pFrame )
    {
        return ( NULL ) ;
    }

    // Get the active MDI child window.
    CMDIChildWnd * pChild = (CMDIChildWnd *)pFrame->GetActiveFrame ( ) ;
    ASSERT ( NULL != pChild ) ;

    if ( NULL == pChild )
    {
        return ( NULL ) ;
    }

    // Get the active view attached to the active MDI child window.
    CView * pView = pChild->GetActiveView ( ) ;

    return ( pView ) ;

}

#ifdef _DEBUG
void CCrashFinderDoc :: AssertValid ( ) const
{
    ASSERT ( this ) ;

    CDocument::AssertValid ( ) ;
}

void CCrashFinderDoc :: Dump ( CDumpContext & dc ) const
{
    ASSERT ( this ) ;

    // Dump the basic stuff.
    CDocument::Dump ( dc ) ;

    // Now dump this document's specific stuff.
    dc << _T ( "m_pcTreeControl " ) << m_pcTreeControl ;
    dc << _T ( "m_pcEditControl " ) << m_pcEditControl ;
    dc << _T ( "m_cDataArray    " ) << m_cDataArray    ;

}
#endif //_DEBUG

void CCrashFinderDoc :: OnEditFindCrash ( )
{
    ASSERT ( this ) ;

    CFindCrashDlg cDlg ( this ) ;

        cDlg.m_strAddresses = m_strAddresses;
        cDlg.m_bAddress = ((CCrashFinderApp *)AfxGetApp( ))->m_bAddress;
        cDlg.m_bModule = ((CCrashFinderApp *)AfxGetApp( ))->m_bModule;
        cDlg.m_bFunction = ((CCrashFinderApp *)AfxGetApp( ))->m_bFunction;
        cDlg.m_bFnDisplacement = ((CCrashFinderApp *)AfxGetApp( ))->m_bFnDisplacement;
        cDlg.m_bSourceFile = ((CCrashFinderApp *)AfxGetApp( ))->m_bSourceFile;
        cDlg.m_bSourceLine = ((CCrashFinderApp *)AfxGetApp( ))->m_bSourceLine;
        cDlg.m_bSrcDisplacement = ((CCrashFinderApp *)AfxGetApp( ))->m_bSrcDisplacement;
        cDlg.m_bOneLine = ((CCrashFinderApp *)AfxGetApp( ))->m_bOneLine;

    cDlg.DoModal ( ) ;

        m_strAddresses = cDlg.m_strAddresses;
        ((CCrashFinderApp *)AfxGetApp( ))->m_bAddress = cDlg.m_bAddress;
        ((CCrashFinderApp *)AfxGetApp( ))->m_bModule = cDlg.m_bModule;
        ((CCrashFinderApp *)AfxGetApp( ))->m_bFunction = cDlg.m_bFunction;
        ((CCrashFinderApp *)AfxGetApp( ))->m_bFnDisplacement = cDlg.m_bFnDisplacement;
        ((CCrashFinderApp *)AfxGetApp( ))->m_bSourceFile = cDlg.m_bSourceFile;
        ((CCrashFinderApp *)AfxGetApp( ))->m_bSourceLine = cDlg.m_bSourceLine;
        ((CCrashFinderApp *)AfxGetApp( ))->m_bSrcDisplacement = cDlg.m_bSrcDisplacement;
        ((CCrashFinderApp *)AfxGetApp( ))->m_bOneLine = cDlg.m_bOneLine;
}

void CCrashFinderDoc :: OnEditAddImage ( )
{
    ASSERT ( this ) ;

    CString szFilter ;
    CString szTitle  ;

    // Get the filter list and the title of the dialog.
    VERIFY ( szFilter.LoadString ( IDS_ADDFILTER ) ) ;
    VERIFY ( szTitle.LoadString ( IDS_ADDTITLE ) ) ;

    // The open file dialog.
    CFileDialog cFD ( TRUE                      ,
                      NULL                      ,
                      NULL                      ,
                      OFN_ALLOWMULTISELECT  |
                        OFN_FILEMUSTEXIST   |
                        OFN_NOCHANGEDIR     |
                        OFN_PATHMUSTEXIST   |
                        OFN_HIDEREADONLY        ,
                      szFilter                  ,
                      AfxGetApp()->m_pMainWnd    ) ;

    cFD.m_ofn.lpstrTitle = (LPCTSTR)szTitle ;

    // Set up the buffers that will be used to return the file
    // information.  Since the dialog allows multple selection, I make
    // sure to allocate a big enough buffer to handle a good number
    // of files.
    #define MAXFILEOPENSIZE 16384
    LPTSTR szFileName = new TCHAR[ MAXFILEOPENSIZE ] ;
    szFileName[ 0 ] = _T ( '\0' ) ;
    cFD.m_ofn.lpstrFile = szFileName ;
    cFD.m_ofn.nMaxFile = MAXFILEOPENSIZE ;

    LPTSTR szFileTitle = new TCHAR[ MAXFILEOPENSIZE ] ;
    szFileTitle[ 0 ] = _T ( '\0' ) ;
    cFD.m_ofn.lpstrFileTitle = szFileTitle ;
    cFD.m_ofn.nMaxFileTitle = MAXFILEOPENSIZE ;

    // Prompt the user.
    if ( IDOK == cFD.DoModal ( ) )
    {
        // Loop through get each file.
        CString sFinalList ;
        CString sCurr      ;

        POSITION pos = cFD.GetStartPosition ( ) ;
        while ( NULL != pos )
        {
            // Get the file name.
            sCurr = cFD.GetNextPathName ( pos ) ;
                        sCurr.MakeUpper();

            // Allocate the individual file information.
            CBinaryImage * pImage = new CBinaryImage ( sCurr ) ;

                        // Load and show the image.
            if ( FALSE == LoadAndShowImage ( pImage ) )
            {
                delete pImage ;
            }

                        // Load this image 's dlls
                        CMapStringToPtr moduleList;
                        CBinaryImage::InitIgnoredImage(moduleList, FALSE);

                        CString DllImageName;
                        LPVOID pPtr;
                        if (CBinaryImage::GetModules(sCurr, moduleList))
            {
                                // GetModules will add sCurr into the moduleList for
                                // ensuring sCurr will not be reload later.
                                // remove it here
                                moduleList.RemoveKey(sCurr);

                                CMapStringToPtr ignoredList;
                                CBinaryImage::InitIgnoredImage(ignoredList, FALSE);
                                POSITION mlPos=ignoredList.GetStartPosition();
                                while (mlPos!=NULL)
                                {
                                        ignoredList.GetNextAssoc(mlPos, DllImageName, pPtr);
                                        moduleList.RemoveKey(DllImageName);
                                }

                                mlPos=moduleList.GetStartPosition();
                                while (mlPos!=NULL)
                                {
                                        moduleList.GetNextAssoc(mlPos, DllImageName, pPtr);
                                        CBinaryImage* pDllImage = new CBinaryImage( DllImageName);
                                        // Load and show the image.
                                        if ( FALSE == LoadAndShowImage ( pDllImage ) )
                                        {
                                                delete pDllImage ;
                                        }

                                }
                        }
        }
    }

    // Get rid of the memory we allocated for the file buffer.
    delete szFileName ;
    delete szFileTitle ;
}

void CCrashFinderDoc :: OnEditRemoveImage ( )
{
    ASSERT ( this ) ;
    ASSERT ( NULL != m_pcTreeControl ) ;

    // A general scratch buffer.
    CString sMsg ;

    // Make sure something is selected.
    HTREEITEM hItem = m_pcTreeControl->GetSelectedItem ( ) ;
    ASSERT ( NULL != hItem ) ;

    // Get the CBinaryImage out of the tree node extra data.
    CBinaryImage * pImage =
                (CBinaryImage *)m_pcTreeControl->GetItemData ( hItem ) ;
    ASSERT ( NULL != pImage ) ;

    if ( NULL == pImage )
    {
        VERIFY ( sMsg.LoadString ( IDS_CATASTROPHICFAILURE ) ) ;
        AfxMessageBox ( sMsg ) ;
        return ;
    }

    // Am I supposed to confirm deletions?
    CCrashFinderApp * pApp = (CCrashFinderApp *)AfxGetApp ( ) ;
    ASSERT ( NULL != pApp ) ;

    if ( TRUE == pApp->ConfirmDeletions ( ) )
    {
        sMsg.FormatMessage ( IDS_CONFIRMREMOVE       ,
                             pImage->GetFullName ( )  ) ;

        if ( IDNO == AfxMessageBox ( sMsg , MB_YESNO ) )
        {
            // Get out now.
            return ;
        }
    }

    // Loop through each item in the data array until I find this item.
    int iCount = m_cDataArray.GetSize ( ) ;
    for ( int iIndex = 0 ; iIndex < iCount ; iIndex++ )
    {
        if ( pImage == m_cDataArray[ iIndex ] )
        {
            break ;
        }
    }
    ASSERT ( iIndex < iCount ) ;

    if ( iIndex == iCount )
    {
        VERIFY ( sMsg.LoadString ( IDS_CATASTROPHICFAILURE ) ) ;
        AfxMessageBox ( sMsg ) ;
        return ;
    }

    // Remove this image from the symbol engine.
        BOOL bRet;
        if (pImage->GetExtraData()==TRUE)
        {
                bRet= m_cSymEng.SymUnloadModule ( pImage->GetLoadAddress ( ));
        }

    // Remove this item from the data structure.
    delete pImage ;
    m_cDataArray.RemoveAt ( iIndex ) ;

    // Now remove this item from the tree.
    bRet = m_pcTreeControl->DeleteItem ( hItem ) ;
    ASSERT ( TRUE == bRet ) ;

    // Set the selected item to the top item in the list.
    hItem = m_pcTreeControl->GetRootItem ( ) ;
    if ( NULL != hItem )
    {
        m_pcTreeControl->SelectItem ( hItem ) ;
    }
    else
    {
        // There are no items left so clear out the edit control.
        m_pcEditControl->SetWindowText ( _T ( "" )  ) ;
    }

    // Mark the document as dirty.
    SetModifiedFlag ( ) ;
}

void CCrashFinderDoc :: OnEditImageProperties ( )
{
    ASSERT ( this ) ;
    // Make sure something is selected.
    HTREEITEM hItem = m_pcTreeControl->GetSelectedItem ( ) ;
    ASSERT ( NULL != hItem ) ;

    // Get the CBinaryImage out of the tree node extra data.
    CBinaryImage * pImage =
                (CBinaryImage *)m_pcTreeControl->GetItemData ( hItem ) ;
    ASSERT ( NULL != pImage ) ;


    // Make a copy of the binary image so I don't screw anything up.
    CBinaryImage cTemp ( *pImage ) ;
    // Now ask the CBinaryImage to take care of it's own properties.
    // However, to make sure that the user does not accidentally cause
    // a load address conflict, I watch the new value and make sure not
    // to let it happen.
    DWORD dwOldAddr = cTemp.GetLoadAddress ( ) ;
    if ( TRUE == cTemp.SetProperties ( ) )
    {
        BOOL bGood = FALSE ;

        do
        {
            int iIndex ;
            if ( FALSE ==
                    IsConflictingLoadAddress(cTemp.GetLoadAddress ( ),
                                             iIndex                   ))
            {
                bGood = TRUE ;
                break ;
            }
            else
            {
                // Force the old address back and see if the user wants
                // to reset the properties.

                CString sMsg ;
                sMsg.FormatMessage
                              ( IDS_DUPLICATELOADADDR      ,
                                cTemp.GetFullName ( )      ,
                  ((CBinaryImage*)m_cDataArray[iIndex])->GetFullName());

                if ( IDYES == AfxMessageBox ( sMsg , MB_YESNO ) )
                {
                    cTemp.SetLoadAddress ( dwOldAddr ) ;

                    if ( FALSE == cTemp.SetProperties ( ) )
                    {
                        break ;
                    }
                }
                else
                {
                    break ;
                }
            }

        } while ( FALSE == bGood ) ;

        // If everything is copacetic, do the actual change.
        if ( TRUE == bGood )
        {
            BOOL bRet ;

            // Remove this image from the symbol engine if they were
            // loaded in the first place.
            if ( TRUE == pImage->GetExtraData ( ) )
            {
                bRet =
                    m_cSymEng.SymUnloadModule(pImage->GetLoadAddress());
                ASSERT ( FALSE != bRet ) ;
            }

            // Overwrite the item with the new data.
            *pImage = cTemp ;

            // Reload the symbol information with the new base address.
            bRet =
            m_cSymEng.SymLoadModule(NULL                               ,
                                    (PSTR)(LPCSTR)pImage->GetFullName(),
                                    NULL                               ,
                                    pImage->GetLoadAddress ( )         ,
                                    0                                 );
            ASSERT ( FALSE != bRet ) ;

            // Set the item image based on how the symbol load went.
            int iState = STATE_NOTVALID ;
            if ( FALSE != bRet )
            {
                // Set the extra data value to show the symbol load.
                pImage->SetExtraData ( TRUE ) ;
                iState = STATE_VALIDATED ;
            }
            else
            {
                // No symbols loaded.
                pImage->SetExtraData ( FALSE ) ;
            }

            m_pcTreeControl->SetItemImage ( hItem , iState , iState ) ;

            // Force the update to the edit control.
            TVITEM stTVI ;

            memset ( &stTVI , NULL , sizeof ( TVITEM ) ) ;
            stTVI.hItem = hItem ;

            bRet = m_pcTreeControl->GetItem ( &stTVI ) ;
            ASSERT ( TRUE == bRet ) ;

            SelectionChanged ( stTVI ) ;
        }
    }
}

void CCrashFinderDoc :: SelectionChanged ( TVITEM & stTVI )
{
    ASSERT ( this ) ;

    // A scratch string buffer.
    CString sMsg ;

    // Get the CBinaryImage information from the LPARM data.
    CBinaryImage * pImage = (CBinaryImage *)stTVI.lParam ;
    ASSERT ( NULL != pImage ) ;

    if ( NULL == pImage )
    {
        VERIFY ( sMsg.LoadString ( IDS_CATASTROPHICFAILURE ) ) ;
        m_pcEditControl->SetWindowText ( sMsg ) ;
        return ;
    }

    // Check to see if the symbols are really loaded for this image.
    if ( FALSE == pImage->GetExtraData ( ) )
    {
        sMsg.FormatMessage ( IDS_SYMMODULENOTLOADED ,
                             pImage->GetName ( )     ) ;
        m_pcEditControl->SetWindowText ( sMsg ) ;
        return ;
    }

    CImageHlp_Module cModInfo ;

    // Try and get the symbol information for this module.
    BOOL bRet = m_cSymEng.SymGetModuleInfo ( pImage->GetLoadAddress() ,
                                             &cModInfo                );
    ASSERT ( FALSE != bRet ) ;
    if ( FALSE == bRet )
    {
        sMsg.FormatMessage ( IDS_SYMMODULENOTLOADED ,
                             pImage->GetName ( )     ) ;
        m_pcEditControl->SetWindowText ( sMsg ) ;
        return ;
    }

    // Do the whole thing.  Since I am just displaying the raw data from
    // an operating system structure, it is not really worth
    // internationalizing it.

    sMsg.Format ( _T ( "Module Symbol Information\r\n"
                       "  Image base    : 0x%08X\r\n"
                       "  Image size    : %d\r\n"
                       "  Time stamp    : 0x%08X\r\n"
                       "  Checksum      : 0x%08X\r\n"
                       "  Symbol count  : %d\r\n"
                       "  Symbol type   : "             ) ,
                  cModInfo.BaseOfImage                    ,
                  cModInfo.ImageSize                      ,
                  cModInfo.TimeDateStamp                  ,
                  cModInfo.CheckSum                       ,
                  cModInfo.NumSyms                         ) ;
    switch ( cModInfo.SymType )
    {
        case SymNone            :
            sMsg += _T ( "SymNone\r\n" ) ;
            break ;
        case SymCoff            :
            sMsg += _T ( "SymCoff\r\n" ) ;
            break ;
        case SymCv              :
            sMsg += _T ( "SymCv\r\n" ) ;
            break ;
        case SymPdb             :
            sMsg += _T ( "SymPdb\r\n" ) ;
            break ;
        case SymExport          :
            sMsg += _T ( "SymExport\r\n" ) ;
            break ;
        case SymDeferred        :
            sMsg += _T ( "SymDeferred\r\n" ) ;
            break ;
        case SymSym             :
            sMsg += _T ( "SymSym\r\n" ) ;
            break ;
        case SymDia             :
            sMsg += _T ( "SymDia\r\n" ) ;
            break ;
        default :
            ASSERT ( FALSE ) ;
            sMsg += _T ( "**UNKNOWN!!!!!\r\n" ) ;
    }

    CString sTemp ;
    sTemp.Format ( _T ( "  Module Name   : %s\r\n"
                        "  Image Name    : %s\r\n"
                        "  Loaded Image  : %s\r\n" ) ,
                   cModInfo.ModuleName               ,
                   cModInfo.ImageName                ,
                   cModInfo.LoadedImageName           ) ;

    sMsg += sTemp ;

    // Add the extra string information, if there is any, from the
    // binary image itself.
    sMsg += pImage->GetAdditionalInfo ( ) ;

    m_pcEditControl->SetWindowText ( sMsg ) ;

}

void CCrashFinderDoc :: CommonItemsUIUpdate ( CCmdUI * pCmdUI )
{
    // For common UI items, the rules are:
    // 1.  The tree view must be active.
    // 2.  There must be an item selected in the tree view.

    ASSERT ( this ) ;

    CTreeView * pView = (CTreeView *)GetCurrentView ( ) ;

    BOOL bEnable = FALSE ;
    if ( pView != m_pcTreeView )
    {
        bEnable = FALSE ;
    }
    else
    {
        HTREEITEM hItem = m_pcTreeControl->GetSelectedItem ( ) ;
        bEnable = ( NULL != hItem ) ;
    }
    pCmdUI->Enable ( bEnable ) ;
}

void CCrashFinderDoc :: OnUpdateEditRemoveImage ( CCmdUI * pCmdUI )
{
    ASSERT ( this ) ;
    CommonItemsUIUpdate ( pCmdUI ) ;
}

void CCrashFinderDoc :: OnUpdateEditImageProperties ( CCmdUI * pCmdUI )
{
    ASSERT ( this ) ;
    CommonItemsUIUpdate ( pCmdUI ) ;
}

