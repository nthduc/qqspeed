/*----------------------------------------------------------------------
Debugging Applications for Microsoft .NET and Microsoft Windows
Copyright © 1997-2003 John Robbins -- All rights reserved.
----------------------------------------------------------------------*/
#include "stdafx.h"
#include "WDBG.h"

#include "WDBGProjDoc.h"
#include "WDBGLogView.h"
#include "MemoryDisplayDlg.h"
#include "DisasmDisplayDlg.h"
#include "SymbolsDisplayDlg.h"
#include "ThreadPickerDlg.h"
#include "BreakpointsDlg.h"

#include "ProjectSettings.h"
#include "WDBGUser.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// The first part of the event synchronization string.
#define SYNC_STRING _T ( "WDBG_Happy_Synch_Event_" )
// The size of the header in the modules and threads window.
#define HEADER_SIZE 2

// String values in case load string fails.
#define ERRORTEXT_THREADNOLONGEREXISTS \
                        _T ( "Thread no longer exists\r\n" )

// Death error messages.
#define GTC_FAILED _T ( "GetThreadContext failed\r\nAborting" )

IMPLEMENT_DYNCREATE(CWDBGProjDoc, CDocument)

BEGIN_MESSAGE_MAP(CWDBGProjDoc, CDocument)
    //{{AFX_MSG_MAP(CWDBGProjDoc)
    ON_COMMAND(ID_EDIT_PROJECT, OnEditProject)
    ON_COMMAND(ID_DEBUG_GO, OnDebugGo)
    ON_UPDATE_COMMAND_UI(ID_DEBUG_GO, OnUpdateDebugGo)
    ON_COMMAND(ID_DEBUG_STOP, OnDebugStop)
    ON_UPDATE_COMMAND_UI(ID_DEBUG_STOP, OnUpdateDebugStop)
    ON_COMMAND(ID_VIEW_DISASSEMBLY, OnViewDisassembly)
    ON_UPDATE_COMMAND_UI(ID_VIEW_DISASSEMBLY, OnUpdateViewDisassembly)
    ON_COMMAND(ID_VIEW_MEMORY, OnViewMemory)
    ON_UPDATE_COMMAND_UI(ID_VIEW_MEMORY, OnUpdateViewMemory)
    ON_COMMAND(ID_VIEW_SYMBOLS, OnViewSymbols)
    ON_UPDATE_COMMAND_UI(ID_VIEW_SYMBOLS, OnUpdateViewSymbols)
    ON_COMMAND(ID_VIEW_CALLSTACK, OnViewCallstack)
    ON_UPDATE_COMMAND_UI(ID_VIEW_CALLSTACK, OnUpdateViewCallstack)
    ON_COMMAND(ID_VIEW_REGISTERS, OnViewRegisters)
    ON_UPDATE_COMMAND_UI(ID_VIEW_REGISTERS, OnUpdateViewRegisters)
    ON_COMMAND(ID_DEBUG_BREAK, OnDebugBreak)
    ON_UPDATE_COMMAND_UI(ID_DEBUG_BREAK, OnUpdateDebugBreak)
    ON_COMMAND(ID_DEBUG_BREAKPOINTS, OnDebugBreakpoints)
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

CWDBGProjDoc::CWDBGProjDoc()
{
    m_bFakedAndNotSaved = FALSE ;
    m_eState = eStopped ;
    m_hUserInputSyncEvent = NULL ;
    m_hDebugThread = INVALID_HANDLE_VALUE ;
    m_bStopAtLoaderBP = FALSE ;

    m_pLogView = NULL ;
    m_pThreadsView = NULL ;
    m_pModView = NULL ;

    m_bSeenLoaderBP = FALSE ;

    m_dwCurrentPID = 0 ;
    m_dwCurrentTID = 0 ;


    memset ( &m_hSynchEvents ,
             NULL            ,
             sizeof ( HANDLE ) * NUM_DEBUGEVENTS ) ;

}

CWDBGProjDoc::~CWDBGProjDoc()
{
    if ( NULL != m_hUserInputSyncEvent )
    {
        VERIFY ( CloseHandle ( m_hUserInputSyncEvent ) ) ;
    }
    // On Win9x, IsWindow returns something other than TRUE if it
    // succeeds.
    ASSERT ( ::IsWindow ( m_cDocWnd.m_hWnd ) ) ;
    if ( ::IsWindow ( ( m_cDocWnd.m_hWnd ) ) )
    {
        // There is nothing much I can do if this fails.
        VERIFY ( m_cDocWnd.DestroyWindow ( ) ) ;
    }

    // Free any memory associated with the BP lists.
    CLocationBP * pBP ;
    for ( int i = 0 ; i < m_aBPArray.GetSize ( ) ; i++ )
    {
        pBP = m_aBPArray[ i ] ;
        delete pBP ;
    }
}

BOOL CWDBGProjDoc::OnNewDocument()
{
    if (!CDocument::OnNewDocument())
        return FALSE;

    // TODO: add reinitialization code here
    // (SDI documents will reuse this document)

    return TRUE;
}

void CWDBGProjDoc::Serialize(CArchive& ar)
{
    if( TRUE == ar.IsStoring() )
    {
        ar << m_sEXEName ;
        ar << m_sCmdLineOpts ;
        ar << m_sDisplayName ;
        ar << m_sFullBinaryName ;
        ar << m_bStopAtLoaderBP ;

        // Write out the number of elements.
        ar << (int)m_aBPArray.GetSize ( ) ;
        // Write out each item.
        CLocationBP cBP ;
        for ( int i = 0 ; i < m_aBPArray.GetSize ( ) ; i++ )
        {
            CLocationBP * pBP = m_aBPArray[ i ] ;
            pBP->Serialize ( ar ) ;
        }
    }
    else
    {
        ar >> m_sEXEName ;
        ar >> m_sCmdLineOpts ;
        ar >> m_sDisplayName ;
        ar >> m_sFullBinaryName ;
        ar >> m_bStopAtLoaderBP ;

        int iCount ;
        ar >> iCount ;

        CLocationBP * pBP ;
        for ( int i = 0 ; i < iCount ; i++ )
        {
            pBP = new CLocationBP ;
            pBP->Serialize ( ar ) ;
            m_aBPArray.Add ( pBP ) ;
        }
    }
}

void CWDBGProjDoc:: ClearAllViews ( void )
{
    POSITION pos = GetFirstViewPosition ( ) ;
    while ( NULL != pos )
    {
        CView * pView = GetNextView ( pos ) ;
        ASSERT ( NULL != pView ) ;

        if ( TRUE ==
                pView->IsKindOf ( RUNTIME_CLASS ( CWDBGLogView ) ) )
        {
            CWDBGLogView * pLog = (CWDBGLogView*)pView ;
            pLog->Clear ( ) ;
        }
    }

    CString sTemp ;
    if ( FALSE == sTemp.LoadString ( IDS_MODVIEWHDR ) )
    {
        ASSERT ( !"sTemp.LoadString ( IDS_MODVIEWHDR ) failed!" ) ;
        sTemp = _T ( "Unable to load IDS_MODVIEWHDR" ) ;
    }

    m_pModView->AppendText ( sTemp ) ;

    if ( FALSE == sTemp.LoadString ( IDS_MODVIEWSEP ) )
    {
        ASSERT ( !"sTemp.LoadString ( IDS_MODVIEWSEP ) failed!" ) ;
        sTemp = _T ( "Unable to load IDS_MODVIEWSEP" ) ;
    }

    m_pModView->AppendText ( sTemp ) ;

    if ( FALSE == sTemp.LoadString ( IDS_THREADVIEWMOD ) )
    {
        ASSERT ( !"sTemp.LoadString ( IDS_THREADVIEWMOD ) failed!" ) ;
        sTemp = _T ( "Unable to load IDS_THREADVIEWMOD" ) ;
    }
    m_pThreadsView->AppendText ( sTemp ) ;

    if ( FALSE == sTemp.LoadString ( IDS_THREADVIEWSEP ) )
    {
        ASSERT ( !"sTemp.LoadString ( IDS_THREADVIEWSEP ) failed!" ) ;
        sTemp = _T ( "Unable to load IDS_THREADVIEWSEP" ) ;
    }
    m_pThreadsView->AppendText ( sTemp ) ;

}

#ifdef _DEBUG
void CWDBGProjDoc::AssertValid() const
{
    CDocument::AssertValid();
}

void CWDBGProjDoc::Dump(CDumpContext& dc) const
{
    CDocument::Dump(dc);
}
#endif //_DEBUG

void CWDBGProjDoc :: ViewDestroyed ( CView * pView )
{
    // Does this view happen to be a memory window view?
    if ( m_vMemDisp.size ( ) > 0 )
    {
        MEMDISPLAYVECT::iterator i ;
        for ( i =  m_vMemDisp.begin ( ) ;
              i != m_vMemDisp.end ( )   ;
              i++                        )
        {
            if ( pView == i->m_pView )
            {
                m_vMemDisp.erase ( i ) ;
                return ;
            }
        }
    }
    // Does this view happen to be a call stack view?
    if ( m_vStackDisp.size ( ) > 0 )
    {
        STACKDISPLAYVECT::iterator j ;
        for ( j =  m_vStackDisp.begin ( ) ;
              j != m_vStackDisp.end ( )   ;
              j++                          )
        {
            if ( pView == j->m_pView )
            {
                m_vStackDisp.erase ( j ) ;
                return ;
            }
        }
    }
    // Does this view happen to be a register view?
    if ( m_vRegDisp.size ( ) > 0 )
    {
        REGISTERDISPLAYVECT::iterator h ;
        for ( h = m_vRegDisp.begin ( ) ;
              h != m_vRegDisp.end ( )  ;
              h++                       )
        {
            if ( pView == h->m_pView )
            {
                m_vRegDisp.erase ( h ) ;
                return ;
            }
        }
    }
}

BOOL CWDBGProjDoc :: IsDebuggeeRunning ( void )
{
    return ( eStopped != m_eState ) ;
}

// A helper function to have the user browse for an exe.
static INT_PTR BrowseForEXE ( LPCTSTR sDLLName , CString & sEXEFile )
{
    CString sMsg ;
    sMsg.Format ( IDS_BROWSEEXEMSG , sDLLName ) ;

    INT_PTR iRet = AfxMessageBox ( sMsg , MB_OKCANCEL ) ;
    if ( IDOK == iRet )
    {
        CFileDialog cDlg ( TRUE ) ;

        cDlg.m_ofn.Flags |= OFN_FILEMUSTEXIST ;

        CString sFilter ;

        sFilter = _T ( "Executable programs (*.exe)" ) ;
        sFilter += _T ( '\0' ) ;
        sFilter += _T ( "*.exe" ) ;
        sFilter += _T ( '\0' ) ;

        // Set the filters.
        cDlg.m_ofn.lpstrFilter = sFilter ;

        CString sFilename ;
        cDlg.m_ofn.lpstrFile = sEXEFile.GetBuffer ( MAX_PATH ) ;

        iRet = cDlg.DoModal ( ) ;
        sEXEFile.ReleaseBuffer ( ) ;
    }
    return ( iRet ) ;
}

BOOL CWDBGProjDoc::OnOpenDocument ( LPCTSTR lpszPathName )
{

    // Create the notification window.  If I cannot create it, nothing
    // else can happen.
    BOOL bRet = m_cDocWnd.Create ( this ) ;
    ASSERT ( FALSE != bRet ) ;
    if ( FALSE == bRet )
    {
        return ( FALSE ) ;
    }

    // I need to check the file extension.  If it ends in .EXE, .DLL. or
    // .OCX, I need to change it to .WPJ.

    TCHAR szDrive[ MAX_PATH ] ;
    TCHAR szDir[ MAX_PATH ] ;
    TCHAR szName [ MAX_PATH ] ;
    TCHAR szExt [ MAX_PATH ] ;

    CString sFinalName ;

    _tsplitpath ( lpszPathName , szDrive , szDir , szName , szExt ) ;

    // Determine the extension type.
    BOOL bIsEXE = FALSE ;
    BOOL bIsDLL = FALSE ;

    bIsDLL = ( 0 == _tcsicmp ( szExt , _T ( ".dll" ) ) ) ||
             ( 0 == _tcsicmp ( szExt , _T ( ".ocx" ) ) )   ;
    bIsEXE = ( 0 == _tcsicmp ( szExt , _T ( ".exe" ) ) ) ;

    BOOL bOpeningWPJ = FALSE ;

    if ( ( TRUE == bIsEXE ) || ( TRUE == bIsDLL ) )
    {
        // Build up the project name.
        sFinalName = szDrive ;
        sFinalName += szDir ;
        sFinalName += szName ;
        sFinalName += _T ( ".wpj" ) ;
    }
    else
    {
        // I will assume that it is a project.
        sFinalName = lpszPathName ;
        bOpeningWPJ = TRUE ;
    }

    // I need to store the full binary name for this project.
    m_sFullBinaryName = lpszPathName ;

    // Now I need to check if the .WPJ exists.  If it does not, I need
    // to create one for the original binary file.
    m_bFakedAndNotSaved = FALSE ;
    HANDLE hFile = CreateFile ( sFinalName              ,
                                GENERIC_READ            ,
                                FILE_SHARE_READ         ,
                                NULL                    ,
                                OPEN_EXISTING           ,
                                FILE_ATTRIBUTE_NORMAL   ,
                                NULL                     ) ;
    if ( INVALID_HANDLE_VALUE == hFile )
    {
        // If the .WPJ does not exist, it is an error.
        if ( TRUE == bOpeningWPJ )
        {
            return ( FALSE ) ;
        }
        // Set up to create the .WPJ file.
        m_sCmdLineOpts.Empty ( ) ;

        m_sDisplayName = szName ;
        m_sDisplayName += szExt ;

        if ( TRUE == bIsDLL )
        {
            CString sEXE ;
            if ( IDOK != BrowseForEXE ( lpszPathName , m_sEXEName ) )
            {
                return ( FALSE ) ;
            }
        }
        else
        {
            m_sEXEName = lpszPathName ;
        }

        // Create a new file.
        try
        {
            CFile tempFile ;
            tempFile.Open ( sFinalName                           ,
                            CFile::modeCreate | CFile::modeWrite  ) ;

            CArchive ar ( &tempFile , CArchive::store ) ;

            Serialize ( ar ) ;

            ar.Flush ( ) ;

            tempFile.Flush ( ) ;

            tempFile.Close ( ) ;

            m_bFakedAndNotSaved = TRUE ;
        }
        catch ( CFileException * e )
        {
            // TODO TODO
            // Fix this so it shows the real error!
            CString sError ;
            sError.Format ( _T ( "Unable to create file : %s" ) ,
                            sFinalName                           ) ;
            AfxMessageBox ( sError ) ;
            e->Delete ( ) ;
            return ( FALSE ) ;
        }

    }
    else
    {
        VERIFY ( CloseHandle ( hFile ) ) ;
    }

    if (!CDocument::OnOpenDocument(sFinalName))
    {
        return ( FALSE ) ;
    }

    // If I had to create a file on the fly to make MFC happy, kill it now.
    if ( TRUE == m_bFakedAndNotSaved )
    {
        VERIFY ( DeleteFile ( sFinalName ) ) ;
        SetModifiedFlag ( ) ;
    }

    // Tell the app not to open any more documents.
    CWDBGApp * pApp = (CWDBGApp*)AfxGetApp ( ) ;
    ASSERT ( NULL != pApp ) ;
    pApp->DocumentWasOpened ( this ) ;

    // The document is opened so now I will deal with getting the
    // special views.

    // The first view is the Trace Output view.

    POSITION pos = GetFirstViewPosition ( ) ;
    ASSERT ( NULL != pos ) ;

    m_pLogView = (CWDBGLogView *)GetNextView ( pos ) ;
    ASSERT ( NULL != m_pLogView ) ;
    if ( NULL == m_pLogView )
    {
        return ( FALSE ) ;
    }

    CString sTitle ;
    if ( FALSE == sTitle.LoadString ( IDS_THREADSTITLE ) )
    {
        ASSERT ( !"sTitle.LoadString ( IDS_THREADSTITLE ) failed!\n" ) ;
        return ( FALSE ) ;
    }

    m_pThreadsView = CreateNewView ( sTitle ) ;
    ASSERT ( NULL != m_pThreadsView ) ;
    if ( NULL == m_pThreadsView )
    {
        return ( FALSE ) ;
    }

    VERIFY ( sTitle.LoadString ( IDS_MODULESTITLE ) ) ;
    m_pModView = CreateNewView ( sTitle ) ;
    ASSERT ( NULL != m_pModView ) ;
        if ( NULL == m_pModView )
    {
        return ( FALSE ) ;
    }
    
    return ( TRUE ) ;
}

void CWDBGProjDoc::OnCloseDocument()
{
    // If the debuggee is currently running, have it stop.
    if ( eStopped != m_eState )
    {
        OnDebugStop ( ) ;

    }
    // Tell the application that it can open another document.
    CWDBGApp * pApp = (CWDBGApp*)AfxGetApp ( ) ;
    ASSERT ( NULL != pApp ) ;

    pApp->DocumentIsClosed ( this ) ;

    CDocument::OnCloseDocument();
}

void CWDBGProjDoc::OnEditProject()
{
    CProjectSettings cDlg ;
    cDlg.m_sCmdLineOpts = m_sCmdLineOpts ;
    cDlg.m_sEXEProgram = m_sEXEName ;
    cDlg.m_sShortName = m_sDisplayName ;
    cDlg.m_bStopOnLdrBP = m_bStopAtLoaderBP ;

    if ( IDOK == cDlg.DoModal ( ) )
    {
        if ( m_sCmdLineOpts != cDlg.m_sCmdLineOpts )
        {
            SetModifiedFlag ( ) ;
            m_sCmdLineOpts = cDlg.m_sCmdLineOpts ;
        }
        if ( m_sEXEName != cDlg.m_sEXEProgram )
        {
            SetModifiedFlag ( ) ;
            m_sEXEName = cDlg.m_sEXEProgram ;
        }
        if ( m_bStopAtLoaderBP != cDlg.m_bStopOnLdrBP )
        {
            SetModifiedFlag ( ) ;
            m_bStopAtLoaderBP = cDlg.m_bStopOnLdrBP ;
        }
    }
}

void CWDBGProjDoc::SetTitle(LPCTSTR /*lpszTitle*/)
{
    // MFC wants to set the title to the name of the file opened by the
    // user, which was the binary file in some cases.  To work around
    // this, I force the title to be the project name.  I also check
    // the state.
    TCHAR szName [ MAX_PATH ] ;

    _tsplitpath ( m_sDisplayName , NULL , NULL , szName , NULL ) ;

    CDocTemplate * pDocTemplate = GetDocTemplate ( ) ;
    ASSERT ( NULL != pDocTemplate ) ;

    CString sExt ;
    VERIFY ( pDocTemplate->GetDocString ( sExt                    ,
                                          CDocTemplate::filterExt  ) ) ;
    CString sState ;
    if ( eException == m_eState )
    {
        if ( FALSE == sState.LoadString ( IDS_EXCEPTIONSTATE ) )
        {
            ASSERT ( !"sState.LoadString ( IDS_EXCEPTIONSTATE ) "
                      "failed\n" ) ;
            sState = _T ( "[Break]" ) ;
        }
    }
    else if ( eRunning == m_eState )
    {
        if ( FALSE == sState.LoadString ( IDS_RUNNINGSTATE ) )
        {
            ASSERT ( !"sState.LoadString ( IDS_RUNNINGSTATE ) "
                      "failed\n" ) ;
            sState = _T ( "[Run]" ) ;
        }
    }

    CString sTitle ;

    if ( FALSE == sState.IsEmpty ( ) )
    {
        sTitle = sState ;
        sTitle += _T ( ' ' ) ;
    }
    sTitle += szName ;
    sTitle += sExt ;

    // Set the title.
    CDocument::SetTitle ( sTitle ) ;
}

void CWDBGProjDoc :: SetPathName ( LPCTSTR lpszPathName ,
                                   BOOL    bAddToMRU     )
{
    if ( TRUE == m_bFakedAndNotSaved )
    {
        // Like the title, I need to set the path name so that the
        // document is the .WPJ file, not the file that the user opened.

        TCHAR szDrive[ MAX_PATH ] ;
        TCHAR szDir[ MAX_PATH ] ;
        TCHAR szName [ MAX_PATH ] ;

        _tsplitpath ( m_sFullBinaryName , szDrive , szDir , szName , NULL ) ;

        CDocTemplate * pDocTemplate = GetDocTemplate ( ) ;
        ASSERT ( NULL != pDocTemplate ) ;

        CString sExt ;
        if ( FALSE == pDocTemplate->GetDocString ( sExt           ,
                                          CDocTemplate::filterExt  ) )
        {
            ASSERT ( !"Failed to get document string!" ) ;
            sExt = _T ( ".wpj" ) ;
        }

        CString sProjName ;
        sProjName = szDrive ;
        sProjName += szDir ;
        sProjName += szName ;
        sProjName += sExt ;


        CDocument::SetPathName ( sProjName, FALSE ) ;
    }
    else
    {
        CDocument::SetPathName ( lpszPathName , bAddToMRU ) ;
    }
}

BOOL CWDBGProjDoc::OnSaveDocument(LPCTSTR lpszPathName)
{
    if ( TRUE == CDocument::OnSaveDocument(lpszPathName) )
    {
        m_bFakedAndNotSaved = FALSE ;
        return ( TRUE ) ;
    }
    else
    {
        return ( FALSE ) ;
    }
}

CWDBGLogView * CWDBGProjDoc :: CreateNewView ( LPCTSTR szTitle       ,
                                               BOOL    bDisableClose  )
{
    // Get the document template.
    CDocTemplate * pTemplate = GetDocTemplate ( ) ;
    ASSERT ( NULL != pTemplate ) ;
    if ( NULL == pTemplate )
    {
        return ( NULL ) ;
    }

    // Create the view itself and get it updated.
    CFrameWnd * pFrame = pTemplate->CreateNewFrame ( this , NULL ) ;
    ASSERT ( NULL != pFrame ) ;
    if ( NULL == pFrame )
    {
        return ( NULL ) ;
    }

    pTemplate->InitialUpdateFrame ( pFrame , this ) ;

    CWDBGLogView * pView =
                        (CWDBGLogView*)pFrame->GetActiveView ( ) ;
    ASSERT ( NULL != pView ) ;
    if ( NULL == pView )
    {
        return ( NULL ) ;
    }

    pView->SetViewText ( szTitle , bDisableClose ) ;
    return ( pView ) ;
}

void CWDBGProjDoc::OnDebugGo()
{

    // Go behaves differently, depending on the state.
    if ( eException == m_eState )
    {
        // Rearm all breakpoints.
        ResolveAllUnresolvedBP ( ) ;
        // Set the state to running.
        m_eState = eRunning ;
        SetTitle ( NULL ) ;
        // Set the synchronization event so that the debug loop cranks
        // back up.
        VERIFY ( ::SetEvent ( m_hUserInputSyncEvent ) ) ;
    }
    else
    {
        if ( FALSE == m_cDbgUser.IsInitialized ( ) )
        {
            // Build the synch string.  In case there are multiple
            // instances of this program running, each instance will
            // have a unique synchronization event because I append the
            // process ID on to the string.
            CString sSyncString ;

            sSyncString = SYNC_STRING ;

            TCHAR szNum[ 33 ] ;
            sSyncString += _ltot ( GetCurrentProcessId ( ) ,
                                   szNum                   ,
                                   16                       ) ;

            // Create the user input synchronization event.
            m_hUserInputSyncEvent = CreateEvent ( NULL          ,
                                                  TRUE          ,
                                                  FALSE         ,
                                                  sSyncString    ) ;
            ASSERT ( NULL != m_hUserInputSyncEvent ) ;

            // Get the logging window view.  That's where output
            // messages will go.
            HWND hWndLogView = m_pLogView->m_hWnd ;

            ASSERT ( ::IsWindow ( hWndLogView ) ) ;
            // Get the window for the document notification special
            // window.
            HWND hDocWnd = m_cDocWnd.m_hWnd ;

            ASSERT ( ::IsWindow ( hDocWnd ) ) ;

            BOOL bRetInit = m_cDbgUser.Initialize ( hDocWnd     ,
                                                    hWndLogView ,
                                                    sSyncString  ) ;
            ASSERT ( FALSE != bRetInit ) ;

            if ( FALSE == bRetInit )
            {
                // TODO TODO
                // I need an explanation for the user here!

                return ;
            }
        }

        // Clear all the views.
        ClearAllViews ( ) ;

        // Reset all BPs.
        ResetBP ( ) ;

        // Clean up the symbol engine.
        m_cSymEng.SymCleanup ( ) ;

        // Get ready to start.
        DWORD dwPID ;

        // Make sure any previous debug thread is done before
        // continuing.
        WaitForSingleObject ( m_hDebugThread , 500 ) ;


        m_hDebugThread = StartDebugging ( m_sEXEName     ,
                                          m_sCmdLineOpts ,
                                          &dwPID         ,
                                          &m_cDbgUser    ,
                                          m_hSynchEvents  ) ;

        ASSERT ( INVALID_HANDLE_VALUE != m_hDebugThread ) ;
        if ( INVALID_HANDLE_VALUE == m_hDebugThread )
        {
            // TODO TODO
            // I need an explanation for the user here!

            return ;
        }

        // If everything worked, set the state.
        m_eState = eRunning ;
        SetTitle ( NULL ) ;
    }
}

void CWDBGProjDoc::OnUpdateDebugGo(CCmdUI* pCmdUI)
{
    pCmdUI->Enable ( ( eStopped == m_eState   ) ||
                     ( eException == m_eState )   ) ;
}

void CWDBGProjDoc::OnDebugStop()
{
    // Before I can stop, I need to check the state.
    if ( eException == m_eState )
    {
        // The debuggee is sitting in an exception state.  What I need
        // to do is to set the suspend event.  Since the suspend event
        // is higher priority that the active debugger event, as soon as
        // I tell the debug thread to continue it will suspend and not
        // call ContinueDebugEvent.  I can then safely stop debugging.
        VERIFY ( SuspendDebugging ( m_hSynchEvents ) ) ;

    }
    // No matter what, set the main synchronization event.  This way,
    // any part of the debug thread that is about to wait on it will be
    // released.
    VERIFY ( ::SetEvent ( m_hUserInputSyncEvent ) ) ;

    VERIFY ( StopDebugging ( m_hSynchEvents ) ) ;

    // Reset states and flags.
    ResetDebuggeeInfoStates ( ) ;
}

void CWDBGProjDoc::OnUpdateDebugStop(CCmdUI* pCmdUI)
{
    // No stopping until the loader breakpoint has been seen.  Trying to
    // stop earlier can lead to synchronization problems, depending on
    // the synchronization methods.
    pCmdUI->Enable ( ( ( eRunning == m_eState   ) ||
                       ( eException == m_eState )   ) &&
                       ( TRUE == m_bSeenLoaderBP    )    ) ;
}

void CWDBGProjDoc :: ResetDebuggeeInfoStates ( void )
{
    m_eState = eStopped ;
    SetTitle ( NULL ) ;
    m_bSeenLoaderBP = FALSE ;

    VERIFY ( ::ResetEvent ( m_hUserInputSyncEvent ) ) ;

    m_vDbgThreads.clear ( ) ;

    m_vLoadedMods.clear ( ) ;

    for ( int i = 0 ; i < NUM_DEBUGEVENTS ; i++ )
    {
        VERIFY ( CloseHandle ( m_hSynchEvents[ i ] ) ) ;
    }

    // Set the enabled breakpoints to unresolved.
    ResetBP ( ) ;

    // Clear the symbol engine.
    m_cSymEng.SymCleanup ( ) ;

}

DWORD CWDBGProjDoc :: HandleProcessExitEvent ( DWORD         /*dwPID*/,
                                         PSTEXITPROCESSEVENT /*pEPE*/ )
{
    // Reset states and flags.
    ResetDebuggeeInfoStates ( ) ;

    return ( DBG_CONTINUE ) ;
}

DWORD CWDBGProjDoc :: HandleCreateProcessEvent ( DWORD       /*dwPID*/ ,
                                            PSTCREATEPROCESSEVENT pCTE )
{
    DWORD dwOpts = m_cSymEng.SymGetOptions ( ) ;

    // Turn on load lines.
    m_cSymEng.SymSetOptions ( dwOpts                |
                              SYMOPT_LOAD_LINES      ) ;

    TCHAR * szSymPath = NULL ;

    DWORD dwLen = GetEnvironmentVariable ( _T ( "_NT_SYMBOL_PATH" ) ,
                                           NULL                     ,
                                           0                         ) ;
    if ( 0 != dwLen )
    {
        szSymPath = new TCHAR [ dwLen ] ;
        dwLen = GetEnvironmentVariable ( _T ( "_NT_SYMBOL_PATH" ) ,
                                         szSymPath                ,
                                         dwLen                     ) ;
    }
    
    // Initialize the symbol engine for this thread.
    m_cSymEng.SymInitialize ( GetDebuggeeProcessHandle ( ) ,
                              (LPSTR)szSymPath             ,
                              FALSE                         ) ;
    if ( NULL != szSymPath )
    {
        delete [] szSymPath ;
    }

    // Add the path to the binary to the symbol search path.
    TCHAR szDrive[ MAX_PATH ] ;
    TCHAR szDir[ MAX_PATH ] ;

    _tsplitpath ( m_sFullBinaryName , szDrive , szDir , NULL , NULL ) ;

    // Remove the trailing slash off the dir.
    szDir [ _tcslen ( szDir ) - 1 ] = _T ( '\0' ) ;
    
    CString sAppendPath ;
    sAppendPath = szDrive ;
    sAppendPath += szDir ;
    
    m_cSymEng.SymAppendSearchPath ( (LPCTSTR)sAppendPath ) ;
                              
    *pCTE->pdwReturnVal = DBG_CONTINUE ;

    // Try to set any breakpoints.
    ResolveAllUnresolvedBP ( ) ;

    VERIFY ( ::SetEvent ( m_hUserInputSyncEvent ) ) ;
    return ( DBG_CONTINUE ) ;
}

DWORD CWDBGProjDoc :: HandleExceptionEvent ( DWORD             dwPID ,
                                             PSTEXCEPTIONEVENT pEE    )
{

    // Set the current info stuff before doing anything else.
    m_dwCurrentPID = dwPID ;
    m_dwCurrentTID = pEE->dwThreadId ;

    // Clear all breakpoints.  You need to do this to keep the
    // disassemblies from having breakpoints in the middle of them.
    ClearAllEnabledBP ( ) ;

    DWORD dwRet ;
    switch ( pEE->pEDI->ExceptionRecord.ExceptionCode )
    {
        case EXCEPTION_BREAKPOINT   :
            dwRet = HandleBreakpoint ( dwPID , pEE ) ;
            break ;
        case EXCEPTION_SINGLE_STEP  :
            dwRet = HandleStep ( dwPID , pEE ) ;
            break ;
        default                     :
            dwRet = HandleGeneralException ( dwPID , pEE ) ;
            break ;
    }
    // Return whatever the handlers returned.
    return ( dwRet ) ;
}

DWORD CWDBGProjDoc :: HandleBreakpoint ( DWORD             dwPID ,
                                         PSTEXCEPTIONEVENT pEE   )
{
    ASSERT ( EXCEPTION_BREAKPOINT ==
                            pEE->pEDI->ExceptionRecord.ExceptionCode ) ;

    dwPID = dwPID ;

    // Let the debug loop fly when it restarts.
    *pEE->pdwReturnVal = DBG_CONTINUE ;

    // Do the easy one.
    if ( FALSE == m_bSeenLoaderBP )
    {
        // I've seen the loader breakpoint.
        m_bSeenLoaderBP = TRUE ;

        // Set the state to exception.
        m_eState = eException ;
        SetTitle ( NULL ) ;

        // Figure out what that user wanted to do at the loader BP.
        if ( FALSE == m_bStopAtLoaderBP )
        {
            // Since this processing is occurring as part of the
            // SendMessage.  I need to fake a "Go" press here so that
            // the SendMessage completes and lets the
            // WaitForSingleObject start right after it.  The posted
            // message then sets the event to clear out the wait.
            m_cDocWnd.PostMessage ( WM_FORCE_A_GO ) ;
            return ( DBG_CONTINUE ) ;
        }
        else
        {
            // Since I am stopping, update the dynamic windows.
            UpdateAllDynamicWindows ( ) ;

            // Report to the user why this is stopping.
            CString sWhy ;
            if ( FALSE == sWhy.LoadString ( IDS_STOPPEDATLDRBP ) )
            {
                ASSERT ( !"Why.LoadString ( IDS_STOPPEDATLDRBP ) "
                          "failed!" ) ;
                sWhy = _T ( ">>Stopped at loader BP\r\n" ) ;
            }
            m_pLogView->AppendLast ( sWhy ) ;
            return ( DBG_CONTINUE ) ;
        }
    }
    else
    {
        // Is this a BP I set?
        if ( TRUE == IsOneShotBP ( dwPID , pEE ) )
        {
            return ( DBG_CONTINUE ) ;
        }
        if ( TRUE == IsUserBP ( dwPID , pEE ) )
        {
            return ( DBG_CONTINUE ) ;
        }
    }
    // If I get to here.  I will just treat it as a hard-coded BP.
    m_eState = eException ;
    SetTitle ( NULL ) ;

    UpdateAllDynamicWindows ( ) ;

    CString sStr  ;
    sStr.Format ( IDS_HARDCODEDBP                             ,
                  pEE->pEDI->ExceptionRecord.ExceptionAddress  ) ;
    m_pLogView->AppendLast ( sStr );

    return ( *pEE->pdwReturnVal ) ;
}

BOOL CWDBGProjDoc :: IsOneShotBP ( DWORD             /*dwPID*/ ,
                                   PSTEXCEPTIONEVENT pEE        )
{
    BOOL bIsOS = FALSE ;

    // Is there anything in the debug break array?
    if ( m_aDebugBreakBPs.GetSize ( ) > 0 )
    {

        // There is!  Loop through and see if I can find the BP.
        int i ;

        for ( i  = 0                      ;
              i < m_aDebugBreakBPs.GetSize ( ) ;
              i++                          )
        {
            if ( m_aDebugBreakBPs[i].GetBreakpointLocation ( ) ==
                     pEE->pEDI->ExceptionRecord.ExceptionAddress )
            {

                // Set the state to exception because I will be
                // stopping.
                m_eState = eException ;
                SetTitle ( NULL ) ;

                // OK, this looks like my one-shot.
                HANDLE hThread = FindThreadHandle( pEE->dwThreadId);
                CONTEXT Ctx ;
                Ctx.ContextFlags = CONTEXT_FULL ;

                if ( FALSE == DBG_GetThreadContext(hThread , &Ctx ))
                {
                    // This is really serious.
                    ASSERT ( !"GetThreadContext failed!" ) ;
                    // Tell the user.
                    m_pLogView->AppendLast ( GTC_FAILED ) ;
                    // Fail it.  Now it will get handled as a hard
                    // coded breakpoint.
                    return ( FALSE ) ;
                }

                m_aDebugBreakBPs[i].HandleBreakpoint ( hThread , Ctx ) ;

                // Since I am stopping, update the dynamic
                // windows.
                UpdateAllDynamicWindows ( ) ;

                CString sString ;
                if ( FALSE == sString.
                              LoadString(IDS_BREAKDUETOMANUALBREAK))
                {
                    ASSERT (
                        !"LoadString(IDS_BREAKDUETOMANUALBREAK"
                         "failed" ) ;
                    sString =_T(">>Break due to manual break\r\n");
                }
                m_pLogView->AppendLast ( sString ) ;

                // Indicate that I found it.
                bIsOS = TRUE ;
                break ;
            }
        }
        // If this was a One Shot BP, now clear all of them.
        if ( TRUE == bIsOS )
        {
            for ( i  = 0                      ;
                  i < m_aDebugBreakBPs.GetSize ( ) ;
                  i++                          )
            {
                // Clear the BP.
                m_aDebugBreakBPs[i].ClearBreakpoint ( ) ;
            }
        }
        // Get rid of these debug break breakpoints.
        m_aDebugBreakBPs.RemoveAll ( ) ;
    }
    return ( bIsOS ) ;
}

BOOL CWDBGProjDoc :: IsUserBP ( DWORD             /*dwPID*/ ,
                                PSTEXCEPTIONEVENT pEE        )
{
    // Are there any user BPs?
    if ( m_aBPArray.GetSize ( ) > 0 )
    {
        for ( int i = 0                  ;
              i < m_aBPArray.GetSize ( ) ;
              i++                         )
        {
            if ( m_aBPArray[i]->GetBreakpointLocation ( ) ==
                    pEE->pEDI->ExceptionRecord.ExceptionAddress )
            {
                // Set the state to exception because I might be
                // stopping.
                m_eState = eException ;
                SetTitle ( NULL ) ;

                HANDLE hThread = FindThreadHandle( pEE->dwThreadId);
                CONTEXT Ctx ;
                Ctx.ContextFlags = CONTEXT_FULL ;

                if ( FALSE == DBG_GetThreadContext( hThread , &Ctx))
                {
                    // This is really serious.
                    ASSERT ( !"GetThreadContext failed!" ) ;
                    // Tell the user.
                    m_pLogView->AppendLast ( GTC_FAILED ) ;
                    // Fail it.  Now it will get handled as a hard
                    // coded BP.
                    return ( FALSE ) ;
                }

                LPCVOID dwRet =
                           m_aBPArray[i]->HandleBreakpoint ( hThread ,
                                                             Ctx      );
                if ( (LPCVOID)1 != dwRet )
                {
                    // This is not stopping.
                    m_cDocWnd.PostMessage ( WM_FORCE_A_GO ) ;
                    return ( TRUE ) ;
                }
                else
                {
                    // Since I am stopping, update the dynamic
                    // windows.
                    UpdateAllDynamicWindows ( ) ;

                    CString sStr  ;
                    sStr.Format(IDS_USERBPFMT ,
                                m_aBPArray[i]->GetBreakpointName ( ) ,
                                m_aBPArray[i]->GetBreakpointLocation());
                    m_pLogView->AppendLast ( sStr );
                    return ( TRUE ) ;
                }
            }
        }
    }
    return ( FALSE ) ;
}


DWORD CWDBGProjDoc :: HandleStep ( DWORD             dwPID ,
                                   PSTEXCEPTIONEVENT pEE   )
{
    ASSERT ( EXCEPTION_SINGLE_STEP ==
                            pEE->pEDI->ExceptionRecord.ExceptionCode ) ;

    // Could this have been set by one of the user breakpoints?
    if ( m_aBPArray.GetSize ( ) > 0 )
    {
        HANDLE hThread = FindThreadHandle( pEE->dwThreadId);
        CONTEXT Ctx ;
        Ctx.ContextFlags = CONTEXT_FULL ;

        if ( FALSE == DBG_GetThreadContext ( hThread , &Ctx ) )
        {
            // This is really serious.
            ASSERT ( !"GetThreadContext failed!" ) ;
            // Tell the user.
            m_pLogView->AppendLast ( GTC_FAILED ) ;
            // Fail it.
            return ( HandleGeneralException ( dwPID , pEE));
        }


        for ( int i = 0                  ;
              i < m_aBPArray.GetSize ( ) ;
              i++                         )
        {
            if ( TRUE == m_aBPArray[i]->HandleSingleStep ( hThread ,
                                                           Ctx     ,
                                                     pEE->pdwReturnVal))
            {
                // Set the state to exception because I might be
                // stopping.
                m_eState = eException ;
                SetTitle ( NULL ) ;
                if ( 1!= *pEE->pdwReturnVal )
                {
                    m_cDocWnd.PostMessage ( WM_FORCE_A_GO ) ;
                    return ( *pEE->pdwReturnVal ) ;
                }
                else
                {
                    UpdateAllDynamicWindows ( ) ;
                    m_pLogView->
                        AppendLast ( _T ( "\r\n>>Single Step!!\r\n" ) );
                    return ( DBG_CONTINUE ) ;
                }
            }
        }
    }
    // The only time I should see the single step is if I set it.
    return ( HandleGeneralException ( dwPID , pEE ) ) ;
}

// A helper to convert an exception code into a string.
static LPCTSTR ExceptToString ( DWORD dwExCode )
{
    LPCTSTR szRet ;
    switch ( dwExCode )
    {
        case EXCEPTION_ACCESS_VIOLATION         :
            szRet = _T ( "Access Violation" ) ;
            break ;
        case EXCEPTION_DATATYPE_MISALIGNMENT    :
            szRet = _T ( "Datatype Misalignment" ) ;
            break ;
        case EXCEPTION_BREAKPOINT               :
            szRet = _T ( "Breakpoint" ) ;
            break ;
        case EXCEPTION_SINGLE_STEP              :
            szRet = _T ( "Single Step" ) ;
            break ;
        case EXCEPTION_ARRAY_BOUNDS_EXCEEDED    :
            szRet = _T ( "Array Bounds Exceeded" ) ;
            break ;
        case EXCEPTION_FLT_DENORMAL_OPERAND     :
            szRet = _T ( "Floating-Point Denormal Operand" ) ;
            break ;
        case EXCEPTION_FLT_DIVIDE_BY_ZERO       :
            szRet = _T ( "Floating-Point Divide By Zero" ) ;
            break ;
        case EXCEPTION_FLT_INEXACT_RESULT       :
            szRet = _T ( "Floating-Point Inexact Result" ) ;
            break ;
        case EXCEPTION_FLT_INVALID_OPERATION    :
            szRet = _T ( "Floating-Point Invalid Operation" ) ;
            break ;
        case EXCEPTION_FLT_OVERFLOW             :
            szRet = _T ( "Floating-Point Overflow" ) ;
            break ;
        case EXCEPTION_FLT_STACK_CHECK          :
            szRet = _T ( "Floating-Point Stack Check" ) ;
            break ;
        case EXCEPTION_FLT_UNDERFLOW            :
            szRet = _T ( "Floating-Point Underflow" ) ;
            break ;
        case EXCEPTION_INT_DIVIDE_BY_ZERO       :
            szRet = _T ( "Integer Divide By Zero" ) ;
            break ;
        case EXCEPTION_INT_OVERFLOW             :
            szRet = _T ( "Integer Overflow" ) ;
            break ;
        case EXCEPTION_PRIV_INSTRUCTION         :
            szRet = _T ( "Privileged Instruction" ) ;
            break ;
        case EXCEPTION_IN_PAGE_ERROR            :
            szRet = _T ( "Page In Error" ) ;
            break ;
        case EXCEPTION_ILLEGAL_INSTRUCTION      :
            szRet = _T ( "Illegal Instruction" ) ;
            break ;
        case EXCEPTION_NONCONTINUABLE_EXCEPTION :
            szRet = _T ( "Noncontinuable exception" ) ;
            break ;
        case EXCEPTION_STACK_OVERFLOW           :
            szRet = _T ( "Stack Overflow" ) ;
            break ;
        case EXCEPTION_INVALID_DISPOSITION      :
            szRet = _T ( "Invalid Disposition" ) ;
            break ;
        case EXCEPTION_GUARD_PAGE               :
            szRet = _T ( "Guard Page" ) ;
            break ;
        case EXCEPTION_INVALID_HANDLE           :
            szRet = _T ( "Invalid Handle" ) ;
            break ;
        case CONTROL_C_EXIT                     :
            szRet = _T ( "Ctrl+C Exit" ) ;
            break ;
        case 0XC0000135                         :
            szRet = _T ( "DLL Not Found" ) ;
            break ;
        case 0XC0000142                         :
            szRet = _T ( "DLL Initialization Failed" ) ;
            break ;
        case 0XC06D007E                         :
            szRet = _T ( "Module Not Found" ) ;
            break ;
        case 0xc06d007f                         :
            szRet = _T ( "Procedure Not Found" ) ;
            break ;
        case 0xe06d7363                         :
            szRet = _T ( "Microsoft C++ Exception" ) ;
            break ;
        case STATUS_NO_MEMORY                   :
            szRet = _T ( "No Memory" ) ;
            break ;
        default                                 :
            szRet = _T ( "!!!!<<UNKNOWN EXCEPTION>>!!!!" ) ;
            break ;
    }
    return ( szRet ) ;
}


DWORD CWDBGProjDoc :: HandleGeneralException ( DWORD          /*dwPID*/,
                                               PSTEXCEPTIONEVENT pEE   )
{
    // Update the dynamic windows to show the latest stuff.
    UpdateAllDynamicWindows ( ) ;
    // Get the base module for the exception address.
    LPCVOID dwBaseAddr = (LPCVOID)
      SymGetModuleBase64 ( GetDebuggeeProcessHandle ( ) ,
                           (DWORD64)pEE->pEDI->
                                      ExceptionRecord.ExceptionAddress);
    ASSERT ( 0 != dwBaseAddr ) ;


    // Look up the name of the module.
    TCHAR szMod [ MAX_PATH ] ;

    DWORD dwRet = GetDebuggeeModuleName ( dwBaseAddr ,
                                          szMod      ,
                                          sizeof(szMod)/sizeof(TCHAR) );
    ASSERT ( 0 != dwRet ) ;
    if ( 0 == dwRet )
    {
        _tcscpy ( szMod , _T ( "!!<<Unknown Module>>!!" ) ) ;
    }

    LPCTSTR pName = _tcsrchr ( szMod , _T ( '\\' ) ) ;
    if ( NULL != pName )
    {
        pName++ ;
    }

    DWORD dwExCode = pEE->pEDI->ExceptionRecord.ExceptionCode ;

    CString szOut ;

    // If it is a first chance exception, show a little info for the
    // user.
    if ( pEE->pEDI->dwFirstChance )
    {

        szOut.Format ( IDS_FIRSTCHANCEFMT                          ,
                       pEE->pEDI->ExceptionRecord.ExceptionAddress ,
                       pName                                       ,
                       dwExCode                                    ,
                       ExceptToString ( dwExCode )                  ) ;

        // Slap the first chance information into the logging window.
        m_pLogView->AppendLast ( szOut ) ;

        // I am not stopping on first chance exceptions.  It would be
        // trivial for you to extend the code to stop if you wanted.

        // Exception was not handled by me!
        *pEE->pdwReturnVal = DBG_EXCEPTION_NOT_HANDLED ;

        // There is nothing else to do so let the debuggee continue.
        VERIFY ( ::SetEvent ( m_hUserInputSyncEvent ) ) ;

    }
    else
    {
        // Report the fatal error to the user.
        szOut.Format ( IDS_FATALEXCEPTION                          ,
                       pEE->pEDI->ExceptionRecord.ExceptionAddress ,
                       pName                                       ,
                       dwExCode                                    ,
                       ExceptToString ( dwExCode )                  ) ;
        m_pLogView->AppendLast ( szOut ) ;

        // The process is dead.....Get rid of it.
        TerminateProcess ( GetDebuggeeProcessHandle ( ) ,
                           (UINT)-1                      ) ;

        // Exception was not handled by me!
        *pEE->pdwReturnVal = DBG_EXCEPTION_NOT_HANDLED ;

        // The debuggee is dead so let the debug loop thread finish.
        VERIFY ( ::SetEvent ( m_hUserInputSyncEvent ) ) ;
    }

    return ( DBG_EXCEPTION_NOT_HANDLED ) ;
}

void CWDBGProjDoc :: ConvertAddrToString ( LPCVOID dwAddr ,
                                           LPTSTR  szName    )
{
    TCHAR szBuff [ 1024 + sizeof ( IMAGEHLP_SYMBOL64 ) ] ;
    PIMAGEHLP_SYMBOL64 pSym = (PIMAGEHLP_SYMBOL64)&szBuff ;

    pSym->SizeOfStruct = sizeof ( IMAGEHLP_SYMBOL64 ) ;
    pSym->MaxNameLength = 1024 ;

    DWORD64 dwDisp ;
    if ( TRUE == m_cSymEng.SymGetSymFromAddr ( (DWORD64)dwAddr  ,
                                               &dwDisp          ,
                                               pSym              ) )
    {
        if ( 0 != dwDisp )
        {
            wsprintf ( szName                  ,
                       _T ( "%S+%d (0x%08X)" ) ,
                       pSym->Name              ,
                       dwDisp                  ,
                       dwAddr                   ) ;
        }
        else
        {
            BSUAnsi2Wide ( pSym->Name , szName , 1024 ) ;
        }
    }
    else
    {
        wsprintf ( szName , _T ( "0x%08X" ) , dwAddr ) ;
    }
}

BOOL CWDBGProjDoc :: ConvertStringToAddr ( LPCTSTR   szSymbol ,
                                           LPCVOID * pdwAddr   )
{
    ASSERT ( NULL != szSymbol ) ;
    ASSERT ( FALSE == IsBadWritePtr ( pdwAddr , sizeof ( LPCVOID ) ) ) ;

    // If the string starts with "0x", treat it as a possible hex
    // number.
    if ( 0 == _tcsncmp ( szSymbol , _T ( "0x" ) , 2 ) )
    {
        TCHAR * pEnd ;
        DWORD64 dwVal = _tcstol ( szSymbol , &pEnd , 16 ) ;
        if ( ( 0 != dwVal ) && ( ERANGE != errno ) )
        {
            *pdwAddr = (LPCVOID)dwVal ;
            return ( TRUE ) ;
        }
    }

    // Try to look the symbol up as a name.
    char szBuff[ sizeof ( IMAGEHLP_SYMBOL64 ) + 1024 ] ;
    PIMAGEHLP_SYMBOL64 pSym = (PIMAGEHLP_SYMBOL64)&szBuff ;

    memset ( pSym                               ,
             NULL                               ,
             sizeof ( IMAGEHLP_SYMBOL64 ) + 1024   ) ;
                
    pSym->SizeOfStruct = sizeof ( IMAGEHLP_SYMBOL64 ) ;
    pSym->MaxNameLength = 1024 ;
    
    char szAnsiStr[ 1024 ] ;
    BSUWide2Ansi ( szSymbol , szAnsiStr , 1024 ) ;
    
    if ( m_cSymEng.SymGetSymFromName ( szAnsiStr , pSym ) )
    {
        *pdwAddr = (LPCVOID)pSym->Address ;
        return ( TRUE ) ;
    }
    return ( FALSE ) ;
}

DWORD CWDBGProjDoc :: HandleThreadCreateEvent ( DWORD        /*dwPID*/ ,
                                             PSTCREATETHREADEVENT pCTE )
{

    // Add this information to the thread map.
    CDebuggeeThread cThread ( pCTE->dwThreadId            ,
                              pCTE->pCTDI->hThread        ,
                              pCTE->pCTDI->lpStartAddress  ) ;

    m_vDbgThreads.push_back ( cThread ) ;

    // Show this thread.
    AppendToActiveThreads ( cThread ) ;

    // Let the debuggee go.
    VERIFY ( ::SetEvent ( m_hUserInputSyncEvent ) ) ;
    return ( TRUE ) ;
}

DWORD CWDBGProjDoc :: HandleThreadExitEvent ( DWORD          /*dwPID*/ ,
                                              PSTEXITTHREADEVENT pETE  )
{
    DBGTHREADVECT::iterator i ;

    int nIndex = 0 ;
    BOOL bFound = FALSE ;
    for ( i =  m_vDbgThreads.begin ( ) ;
          i != m_vDbgThreads.end ( )   ;
          i++ , nIndex++                )
    {
        if ( i->m_dwTID == pETE->dwThreadId )
        {
            m_vDbgThreads.erase ( i ) ;
            bFound = TRUE ;
            break ;
        }
    }
    ASSERT ( TRUE == bFound ) ;

    // Show all the active threads.
    RemoveFromActiveThreads ( nIndex ) ;

    // Let the debuggee go.
    VERIFY ( ::SetEvent ( m_hUserInputSyncEvent ) ) ;
    return ( TRUE ) ;
}

DWORD CWDBGProjDoc :: HandleLoadDllEvent ( DWORD            /*dwPID*/ ,
                                           PSTLOADDLLEVENT  pLDE       )
{
    *pLDE->pdwReturnVal = DBG_CONTINUE ;

    // Try to set any breakpoints.
    ResolveAllUnresolvedBP ( ) ;

    // Break apart the name.
    TCHAR szPath [ MAX_PATH ] ;
    _tcscpy ( szPath , pLDE->szName ) ;

    LPTSTR pName = _tcsrchr ( szPath , _T ( '\\' ) ) ;
    ASSERT ( NULL != pName ) ;
    if ( NULL == pName )
    {
        pName = szPath ;
    }
    else
    {
        *pName = _T ( '\0' ) ;
        pName++ ;
    }
    
    ULONG lAddr = PtrToUlong ( pLDE->pLDDI->lpBaseOfDll ) ;
    LPCVOID lpImageEnd = ULongToPtr ( ( lAddr +
                                        pLDE->pModInfo->ImageSize ) -
                                       1                             ) ;

    CLoadedModule cMod ( pLDE->pLDDI->lpBaseOfDll ,
                         lpImageEnd               ,
                         pLDE->pModInfo->SymType  ,
                         pName                    ,
                         szPath                     ) ;

    m_vLoadedMods.push_back ( cMod ) ;

    // Display the list!
    AppendToActiveModules ( cMod ) ;

    CString sStr ;
    // Do the logging output as well.
    sStr.Format (_T ( "'%s' loaded at 0x%08X with %s symbols\r\n" ) ,
                  pLDE->szName                                      ,
                  pLDE->pLDDI->lpBaseOfDll                          ,
                  SymToString ( pLDE->pModInfo->SymType )            ) ;
    m_pLogView->AppendText ( sStr ) ;
    
    VERIFY ( ::SetEvent ( m_hUserInputSyncEvent ) ) ;
    return ( TRUE ) ;
}

DWORD CWDBGProjDoc :: HandleUnloadedDllEvent ( DWORD         /*dwPID*/ ,
                                               PSTUNLOADDLLEVENT pUDE  )
{
    LOADEDMODVECT::iterator i ;

    int nIndex = 0 ;
    BOOL bFound = FALSE ;
    for ( i =  m_vLoadedMods.begin ( ) ;
          i != m_vLoadedMods.end ( )   ;
          i++ , nIndex++                )
    {
        if ( i->m_dwBaseAddr == pUDE->dwBaseAddr )
        {
            // Do the logging output as well.
            CString sStr ;
            sStr.Format ( _T ( "Unloading '%s\\%s' from 0x%08X\r\n" ) ,
                          i->m_szPath                                 ,
                          i->m_szBaseName                             ,
                          pUDE->dwBaseAddr                            );
            m_pLogView->AppendText ( sStr ) ;

            m_vLoadedMods.erase ( i ) ;

            bFound = TRUE ;

            break ;
        }
    }

    // Delete the corresponding line.
    m_pModView->DeleteLine ( nIndex + 2 ) ;

    // Let the debuggee go.
    VERIFY ( ::SetEvent ( m_hUserInputSyncEvent ) ) ;
    return ( TRUE ) ;
}

DWORD CWDBGProjDoc :: HandleOutputDebugStringEvent ( DWORD /*dwPID*/ ,
                                PSTOUTPUTDEBUGSTRINGEVENT pODSE       )
{
    m_pLogView->AppendText ( pODSE->szString ) ;
    VERIFY ( ::SetEvent ( m_hUserInputSyncEvent ) ) ;
    return ( TRUE ) ;
}


void CWDBGProjDoc :: AppendToActiveThreads ( CDebuggeeThread & cDbgThd )
{
    TCHAR szSym [ MAX_PATH ] ;

    ConvertAddrToString ( cDbgThd.m_dwStartAddr , szSym ) ;

    CString sStr ;
    sStr.Format ( _T ( "0x%08X  0x%08X     %s\r\n" ) ,
                  cDbgThd.m_dwTID                    ,
                  cDbgThd.m_hThread                  ,
                  szSym                               ) ;
    m_pThreadsView->AppendText ( sStr , TRUE ) ;
}

void CWDBGProjDoc :: RemoveFromActiveThreads ( int iIndex )
{
    m_pThreadsView->DeleteLine ( iIndex + HEADER_SIZE ) ;
}

void CWDBGProjDoc :: AppendToActiveModules ( CLoadedModule & cMod )
{
    CString sStr ;
    sStr.Format ( _T ( "%-25s   0x%08X-0x%08X  %-8s  %s\r\n" ) ,
                  cMod.m_szBaseName                             ,
                  cMod.m_dwBaseAddr                             ,
                  cMod.m_dwHighAddr                             ,
                  SymToString ( cMod.m_eSymType )               ,
                  cMod.m_szPath                                   ) ;
    m_pModView->AppendText ( sStr , TRUE ) ;
}

void CWDBGProjDoc :: RemoveFromActiveModules ( int iIndex )
{
    m_pModView->DeleteLine ( iIndex + HEADER_SIZE ) ;
}

HANDLE CWDBGProjDoc :: FindThreadHandle ( DWORD dwTID )
{
    DBGTHREADVECT::iterator i ;

    for ( i  = m_vDbgThreads.begin ( ) ;
          i != m_vDbgThreads.end ( )   ;
          i++                           )
    {
        if ( dwTID == i->m_dwTID )
        {
            return ( i->m_hThread ) ;
        }
    }
    return ( INVALID_HANDLE_VALUE ) ;
}


void CWDBGProjDoc::OnViewDisassembly()
{
    CDisasmDisplayDlg cDlg ;

    cDlg.m_pDoc = this ;

    // I need to validate the starting address
    if ( IDOK == cDlg.DoModal ( ) )
    {
        CString sTitle ;
        sTitle.Format ( IDS_DISTITLEFMT , cDlg.m_sAddress ) ;

        CWDBGLogView * pView = CreateNewView ( sTitle , FALSE ) ;

        // Here's where I do the disassembly!
        pView->Clear ( ) ;

        HANDLE hThread = FindThreadHandle ( m_dwCurrentTID ) ;
        ASSERT ( INVALID_HANDLE_VALUE != hThread ) ;

        DEBUGPACKET dp ;
        dp.context.ContextFlags = CONTEXT_FULL ;

        if ( FALSE == DBG_GetThreadContext ( hThread , &dp.context ) )
        {
            // This is really serious.
            ASSERT ( !"GetThreadContext failed!" ) ;
            // Tell the user.
            m_pLogView->AppendLast (  GTC_FAILED ) ;
            pView->AppendText ( GTC_FAILED ) ;
            return ;
        }

        dp.hProcess = GetDebuggeeProcessHandle ( ) ;

        TCHAR szBuff[ 100 ] ;
        szBuff[ 0 ] = _T ( '\0' ) ;

        CWaitCursor cCur ;

        pView->SetRedraw ( FALSE ) ;

        LPCVOID ulOffset = cDlg.m_dwConvertedAddr ;

        for ( UINT i = 0 ; i < cDlg.m_uiCount ; i++ )
        {
            // Look the address up.
            TCHAR szBuff [ MAX_PATH + sizeof ( IMAGEHLP_SYMBOL64 ) ] ;
            PIMAGEHLP_SYMBOL64 pSym = (PIMAGEHLP_SYMBOL64)&szBuff ;

            pSym->SizeOfStruct = sizeof ( IMAGEHLP_SYMBOL64 ) ;
            pSym->MaxNameLength = MAX_PATH ;

            DWORD64 dwDisp ;
            CString sFmt ;
            if ( TRUE ==
                    m_cSymEng.SymGetSymFromAddr64 ( (DWORD64)ulOffset ,
                                                    &dwDisp           ,
                                                    pSym              ))
            {
                if ( 0 == dwDisp )
                {
                    sFmt.Format ( _T ( "%S:\r\n" ) , pSym->Name ) ;
                    pView->AppendText ( sFmt ) ;
                }
            }

            char szDisBuff[ MAX_PATH ] ;
            if ( disasm ( &dp               ,
                          (PULONG)&ulOffset ,
                          (PUCHAR)szDisBuff ,
                          FALSE              ) )
            {
                sFmt.Format ( _T ( "%S\r\n" ) , szDisBuff ) ;
                pView->AppendText ( sFmt  ) ;

            }
            else
            {
                CString sErr ;
                sErr.Format ( IDS_NODISASMATADDR , ulOffset ) ;
            }
            pView->GetEditCtrl().SetSel ( 0 , 0 ) ;
        }
        
        pView->SetRedraw ( ) ;
        
    }
}

void CWDBGProjDoc::OnUpdateViewDisassembly(CCmdUI* pCmdUI)
{
    pCmdUI->Enable ( eException == m_eState ) ;
}

void CWDBGProjDoc::OnViewMemory()
{
    CMemoryDisplayDlg cDlg ;

    cDlg.m_pDoc = this ;

    // I need to validate the starting address
    if ( IDOK == cDlg.DoModal ( ) )
    {
        CString sTitle ;
        sTitle.Format ( IDS_MEMTITLEFMT , cDlg.m_sAddress ) ;

        CWDBGLogView * pView = CreateNewView ( sTitle , FALSE ) ;

        // Create my holder class.
        CMemDisplay cMD ( pView                  ,
                          cDlg.m_dwConvertedAddr ,
                          cDlg.m_uiLen           ,
                          cDlg.m_iWidth           ) ;

        // Display the data.
        DisplayMemoryInfo ( &cMD ) ;

        // Add it to the list.
        m_vMemDisp.push_back ( cMD ) ;
    }
}

void CWDBGProjDoc::OnUpdateViewMemory(CCmdUI* pCmdUI)
{
    pCmdUI->Enable ( eException == m_eState ) ;
}

// The function necessary to enumerate the symbols for a module.
static BOOL CALLBACK SymCallBack ( LPSTR    SymbolName     ,
                                   DWORD64  SymbolAddress  ,
                                   ULONG    /*SymbolSize*/ ,
                                   PVOID    UserContext     )
{
    CWDBGLogView * pView = (CWDBGLogView *)UserContext ;
    ASSERT ( NULL != pView ) ;

    CString sFmt ;
    sFmt.Format ( _T ( "0x%08X   %S\r\n" ) ,
                  (DWORD)SymbolAddress     ,
                  SymbolName                ) ;

    pView->AppendText ( sFmt ) ;

    return ( TRUE ) ;

}

void CWDBGProjDoc::OnViewSymbols()
{
    CSymbolsDisplayDlg cDlg ( &m_vLoadedMods ) ;

    if ( IDOK == cDlg.DoModal ( ) )
    {
        // Create the view.
        CString sTitle ;
        sTitle.Format ( IDS_SYMTITLEFMT , cDlg.m_sMod ) ;

        CWDBGLogView * pView = CreateNewView ( sTitle , FALSE ) ;
        ASSERT ( NULL != pView ) ;

        CWaitCursor cCur ;
        
        pView->SetRedraw ( FALSE ) ;
        if ( FALSE == m_cSymEng.
                SymEnumerateSymbols ( (DWORD64)cDlg.m_dwSelectedBase ,
                                       SymCallBack                   ,
                                      (PVOID)pView                    ))
        {
            ASSERT ( !"SymEnumerateSymbols failed!" ) ;
        }
        pView->GetEditCtrl().SetSel ( 0 , 0 ) ;
        pView->SetRedraw ( TRUE ) ;
    }
}

void CWDBGProjDoc::OnUpdateViewSymbols(CCmdUI* pCmdUI)
{
    pCmdUI->Enable ( eException == m_eState ) ;
}

void CWDBGProjDoc :: DisplayMemoryInfo ( CMemDisplay * pMemDisp )
{
    // The handle of the process that we are reading memory from.
    HANDLE hReadHandle = GetDebuggeeProcessHandle ( ) ;

    // The buffer that holds the hexadecimal bytes that are output.
    // This is just a single line of output, not the entire thing.
    TCHAR   szBuffer[ 80 ] ;
    // Holds the current position in the buffer that I am
    // using to concatenate.
    LPTSTR  pMarker  ;
    // The number of bytes that are output on the current line so I
    // get the ASCII char side thing to show properly.
    int     iNumUnitsOnLine ;
    // The number of bytes to go.
    DWORD   dwBytesToGo = pMemDisp->m_iLen ;

    // The buffer that I read memory into.  This is the size of the
    // largest item that I will read.
    DWORD dwBuff ;
    // The number of bytes read from the process's memory space.
    DWORD   dwRead ;

    // As I read the memory, I keep the contents in here so that
    // I can output the little ASCII thing on the side.
    WORD szSideBuff[ 20 ] ;

    // The buffers that hold the question marks for bad reads, and
    // the spaces for partial lines.  These are filled based on the
    // number of bytes to show from pMemDisp->m_dwDisplayLen.
    TCHAR szUnkBuff [ 10 ] ;
    TCHAR szSpaceBuff[ 10 ] ;

    // The number of bytes to show on a line.
    int iOutWidth ;
    // The unit format.
    LPCTSTR szUnitFmt ;
    switch ( pMemDisp->m_iWidth )
    {
        case 1  :
            iOutWidth = 16 ;
            szUnitFmt = _T ( "%02X " ) ;
            break ;
        case 2  :
            iOutWidth = 8 ;
            szUnitFmt = _T ( "%04X " ) ;
            break ;
        case 4  :
            iOutWidth = 4 ;
            szUnitFmt = _T ( "%08X " ) ;
            break ;
        default :
            ASSERT ( !"Invalid width setting" ) ;
            iOutWidth = 4 ;
            szUnitFmt = _T ( "%08X " ) ;
            break ;
    }

    // The address currently being used.
    LPCVOID dwAddr = pMemDisp->m_dwStartAddr ;

    // Fill the space and unknown buffers with the proper number of
    // spaces and question marks respectively.  The last character in
    // each buffer is the space used as a separator.
    szUnkBuff [ 0 ] = _T ( '\0' ) ;
    for ( int x = 0 ; x < pMemDisp->m_iWidth ; x++ )
    {
        _tcscat ( szUnkBuff , _T ( "??" ) ) ;
    }
    _tcscat ( szUnkBuff , _T ( " " ) ) ;
    for ( x = 0 ; x < (pMemDisp->m_iWidth * 2 ) + 1 ; x++ )
    {
        szSpaceBuff[ x ] =  _T ( ' ' ) ;
    }
    szSpaceBuff[ x ] = _T ( '\0' ) ;

    // Clear the display.
    pMemDisp->m_pView->Clear ( ) ;

    // While I still have memory to read...
    while ( dwBytesToGo )
    {
        // Calculate how many bytes to display on this line.
        if ( dwBytesToGo >= (DWORD)iOutWidth )
        {
            iNumUnitsOnLine = iOutWidth ;
        }
        else
        {
            iNumUnitsOnLine = dwBytesToGo ;
        }
        // Set the marker to the beginning of the output buffer.
        pMarker = szBuffer ;
        // Output the address of the first character in the line.
        pMarker += wsprintf ( pMarker           ,
                              _T ( "0x%08X  " ) ,
                              dwAddr             ) ;

        // Loop through the number of characters that I want to
        // appear on the line.
        for ( int i = 0 ; i < iOutWidth ; i++ )
        {
            // Read the process memory.  If it fails, then I output a
            // question mark.
            if ( ! DBG_ReadProcessMemory ( hReadHandle             ,
                                           (LPVOID) dwAddr         ,
                                           &dwBuff ,
                                           sizeof ( BYTE ) *
                                                pMemDisp->m_iWidth ,
                                           &dwRead                   ) )
            {
                // Put a '??' in for the hex output and a '?' in the
                // side buffer so that I put it on the ASCII side area
                // as well.
                pMarker += wsprintf ( pMarker      ,
                                     ( ( i < iNumUnitsOnLine )
                                            ? szUnkBuff :
                                              szSpaceBuff )    ) ;
                szSideBuff [ i ] = _T ( '?' ) ;
            }
            else
            {
                // I got a good read.  Append the value onto the line.
                // If I am done showing characters on the screen, I
                // need to put the spaces there so I get the side ASCII
                // characters started in the same place.  We also put
                // the value in the side buffer.
                pMarker += wsprintf ( pMarker      ,
                   ((i < iNumUnitsOnLine) ? szUnitFmt : szSpaceBuff ) ,
                          ( pMemDisp->m_iWidth == 1 ) ? (BYTE)(dwBuff & 0xFF  ) :
                          ( pMemDisp->m_iWidth == 2 ) ? (WORD)(dwBuff & 0xFFFF) :
                          ( pMemDisp->m_iWidth == 4 ) ? (DWORD)dwBuff :
                          (BYTE)dwRead ) ;
                szSideBuff [ i ] =
                          ( pMemDisp->m_iWidth == 1 ) ? (BYTE)(dwBuff & 0xFF   ) :
                          ( pMemDisp->m_iWidth == 2 ) ? (WORD)(dwBuff & 0xFFFF ) :
                          (BYTE)(dwBuff & 0xFF );

            }
            // Increment the address where I need to read from.  Right
            // now this scheme actually reads PAST where I want to
            // stop.  I need to fix this so I STOP at the last item I am
            // supposed to read.
            dwAddr = (LPCVOID)(((DWORD_PTR)dwAddr) +
                                    pMemDisp->m_iWidth ) ;
        }
        // Now that I've done a line I need to show the ASCII side
        // buffer thing only if this is a byte output or a word output.
        // We loop through the number of characters that appeared on the
        // line (iNumUnitsOnLine).  If the character is  ASCII and
        // printable I will show it, otherwise I will just show a dot.
        if ( pMemDisp->m_iWidth != 4 )
        {
            for ( i = 0 ; i < iNumUnitsOnLine ; i++ )
            {
                _TINT c = _T ( '.' ) ;
                if ( _istascii ( szSideBuff[ i ] ) )
                {
                    c = _istprint ( szSideBuff[ i ] )
                                ? szSideBuff[ i ]
                                : _T ( '.' ) ;
                }
                pMarker += _stprintf ( pMarker , _T ( "%c" ) , c ) ;
            }
        }
        // Show the single line I just built.
        pMemDisp->m_pView->AppendText ( szBuffer , TRUE ) ;
        pMemDisp->m_pView->AppendText ( _T ( "\r\n" ) , TRUE ) ;
        // Calculate the number of bytes that we still need to do.
        if ( dwBytesToGo <= (DWORD)iOutWidth )
        {
            dwBytesToGo = 0 ;
        }
        else
        {
            dwBytesToGo -= ( iNumUnitsOnLine * pMemDisp->m_iWidth ) ;
        }
    }
}

void CWDBGProjDoc :: UpdateAllDynamicWindows ( void )
{
    if ( m_vMemDisp.size ( ) > 0 )
    {
        MEMDISPLAYVECT::iterator i ;
        for ( i =  m_vMemDisp.begin ( ) ;
              i != m_vMemDisp.end ( )   ;
              i++                        )
        {
            DisplayMemoryInfo ( &*i ) ;
        }
    }
    if ( m_vStackDisp.size ( ) > 0 )
    {
        STACKDISPLAYVECT::iterator j ;
        for ( j =  m_vStackDisp.begin ( ) ;
              j != m_vStackDisp.end ( )   ;
              j++                          )
        {
            DisplayCallStack ( &*j ) ;
        }
    }
    if ( m_vRegDisp.size ( ) > 0 )
    {
        REGISTERDISPLAYVECT::iterator k ;
        for ( k =  m_vRegDisp.begin ( ) ;
              k != m_vRegDisp.end ( )   ;
              k++                        )
        {
            DisplayRegisterSet ( &*k ) ;
        }
    }
}

void CWDBGProjDoc::OnViewCallstack()
{
    CString sTitle ;

    if ( FALSE == sTitle.LoadString ( IDS_CALLSTACKDLGTITLE ) )
    {
        ASSERT ( !"sTitle.LoadString ( IDS_CALLSTACKDLGTITLE ) failed");
        sTitle = _T ( "Callstack" ) ;
    }

    CThreadPickerDlg cDlg ( &m_vDbgThreads , sTitle ) ;

    if ( IDOK == cDlg.DoModal ( ) )
    {
        // Create the view.
        CString sViewTitle ;
        sViewTitle.Format ( IDS_CALLSTACKTITLEFMT , cDlg.m_sThreadId ) ;

        CWDBGLogView * pView = CreateNewView ( sViewTitle , FALSE ) ;
        ASSERT ( NULL != pView ) ;
        if ( NULL == pView )
        {
            return ;
        }

        CStackDisplay cStack ( pView , cDlg.m_hSelectedThread ) ;

        DisplayCallStack ( &cStack ) ;

        m_vStackDisp.push_back ( cStack ) ;
    }
}

void CWDBGProjDoc::OnUpdateViewCallstack(CCmdUI* pCmdUI)
{
    pCmdUI->Enable ( eException == m_eState ) ;
}

BOOL CALLBACK CWDBGProjDoc ::
            EnumerateLocalsIntoStackWnd ( DWORD64  dwAddr        ,
                                          LPCTSTR  szType        ,
                                          LPCTSTR  szName        ,
                                          LPCTSTR  szValue       ,
                                          int      iIndentLevel  ,
                                          PVOID    pUserCtx       )
{

    CStackDisplay * pStackDisp = (CStackDisplay *)pUserCtx ;
    
    CString cBuff ;
    
    cBuff = _T ( "    " ) ;
    // Plop on the indent.
    for ( int i = 0 ; i < iIndentLevel ; i++ )
    {
        cBuff += _T ( "  " ) ;
    }
    
    // Add the type.
    cBuff += szType ;

    cBuff += _T ( " " ) ;
    
    // Add the variable name.
    cBuff += szName ;

    cBuff += _T ( " " ) ;
    
    if ( ( NULL == szValue ) || ( _T ( '\0' ) == szValue[ 0 ] ) )
    {
        TCHAR szAddrString[ 25 ] ;
        wsprintf ( szAddrString         ,
                   _T ( "0x%08X" )      ,
                   (DWORD)((DWORD)dwAddr ) ) ;
        cBuff += szAddrString ;
    }
    else
    {
        cBuff += _T ( "= " ) ;
        cBuff += szValue ;
    }
    cBuff += _T ( "\r\n" ) ;
    
    pStackDisp->m_pView->AppendText ( cBuff ) ;
    
    return ( TRUE ) ;
}

#define SHARED_MEM_BASE_ADDR            0x000000007FFE0000
#define SHARED_MEM_SYSTEMSTUBCALL       0x000000007FFE0300
#define SHARED_MEM_SYSTEMSTUBCALLRET    0x000000007FFE0304
BOOL IsSharedUserDataSystemStubCallRet ( DWORD64 dwAddr )
{
    BOOL bRet = FALSE ;
    if ( TRUE == IsXPorBetter ( ) )
    {
        // Is the memory in the shared data secton?
        if ( SHARED_MEM_BASE_ADDR == ( dwAddr & SHARED_MEM_BASE_ADDR ) )
        {
            if ( SHARED_MEM_SYSTEMSTUBCALLRET ==
                             ( dwAddr & SHARED_MEM_SYSTEMSTUBCALLRET ) )
            {
                return ( TRUE ) ;
            }
        }
    }
    return ( bRet ) ;
}

void CWDBGProjDoc :: DisplayCallStack ( CStackDisplay * pStackDisp )
{
    // Clear the window first thing.
    pStackDisp->m_pView->Clear ( ) ;

    DEBUGPACKET dp ;
    dp.hProcess = GetDebuggeeProcessHandle ( ) ;
    dp.hThread = pStackDisp->m_hThread ;
    dp.context.ContextFlags = CONTEXT_FULL ;

    BOOL bRet = DBG_GetThreadContext ( dp.hThread , &dp.context ) ;
    // The thread could be gone.
    if ( FALSE == bRet )
    {
        CString cErr ;
        if ( FALSE == cErr.LoadString ( IDS_THREADNOLONGEREXISTS ) )
        {
            ASSERT ( !"cErr.LoadString ( IDS_THREADNOLONGEREXISTS )"
                      "failed!" ) ;
            cErr = ERRORTEXT_THREADNOLONGEREXISTS ;
        }
        pStackDisp->m_pView->AppendText ( cErr ) ;
        return ;
    }

    CWaitCursor cCur ;
    
    pStackDisp->m_pView->SetRedraw ( FALSE ) ;

    STACKFRAME64 stFrame ;

    memset ( &stFrame , NULL , sizeof ( STACKFRAME64 ) ) ;

    InitializeStackFrameWithContext ( &stFrame , &dp.context ) ;

    CString sFinal ;
    CString sTemp ;
    while ( StackWalk64 ( IMAGE_FILE_MACHINE_I386  ,
                          dp.hProcess              ,
                          dp.hThread               ,
                          &stFrame                 ,
                          &dp.context              ,
                          NULL                     ,
                          SymFunctionTableAccess64 ,
                          SymGetModuleBase64       ,
                          NULL                      ) )
    {
        if ( 0 == stFrame.AddrPC.Offset )
        {
            continue ;
        }
        // Get the module name.
        TCHAR szModName [ MAX_PATH ] ;
        szModName[ 0 ] = _T ( '\0' ) ;
        BOOL bIsSharedStubReturn = FALSE ;

        LPCVOID dwBaseAddr = (LPCVOID)
                m_cSymEng.SymGetModuleBase ( stFrame.AddrPC.Offset ) ;
        if ( 0 != dwBaseAddr )
        {
            if ( 0 == GetDebuggeeModuleName ( dwBaseAddr ,
                                              szModName  ,
                                              MAX_PATH    ) )
            {
                ASSERT ( !"GetDebuggeeModuleName failed!" ) ;
            }
        }
        else
        {
            bIsSharedStubReturn =
                    IsSharedUserDataSystemStubCallRet (
                                               stFrame.AddrPC.Offset ) ;
            if ( TRUE == bIsSharedStubReturn )
            {
                _tcscpy ( szModName , _T ( "SharedUserData" ) ) ;
            }
            else
            {
                ASSERT ( !"m_cSymEng.SymGetModuleBase failed!" ) ;
                _tcscpy ( szModName , _T ( "<<Unknown Module>>" ) ) ;
            }
        }

        LPCTSTR pBaseName = _tcsrchr ( szModName , _T ( '\\' ) ) ;
        if ( NULL != pBaseName )
        {
            pBaseName++ ;
        }
        else
        {
            pBaseName = szModName ;
        }

        // The address and module.
        sFinal.Format ( _T ( "%08X %-20s" )          ,
                        (DWORD)stFrame.AddrPC.Offset ,
                        pBaseName                     ) ;



        TCHAR szBuff[ 1024 ] ;
        PIMAGEHLP_SYMBOL64 pSym = (PIMAGEHLP_SYMBOL64)&szBuff ;
        ZeroMemory ( pSym , sizeof ( szBuff )  ) ;
        pSym->SizeOfStruct = sizeof ( IMAGEHLP_SYMBOL64 ) ;
        pSym->MaxNameLength = sizeof ( szBuff )  -
                              sizeof ( IMAGEHLP_SYMBOL64 ) ;
        DWORD64 dwDisp ;

        if ( TRUE ==
              m_cSymEng.SymGetSymFromAddr ( stFrame.AddrPC.Offset ,
                                            &dwDisp               ,
                                            pSym                   ) )
        {
            if ( dwDisp > 0 )
            {
                sTemp.Format ( _T ( "%S()+%d byte(s)\r\n" ) ,
                               pSym->Name                   ,
                               dwDisp                        ) ;
            }
            else
            {
                sTemp.Format ( _T ( "%S\r\n" ) ,
                               pSym->Name       ) ;
            }
            sFinal += sTemp ;
        }
        else if ( TRUE == bIsSharedStubReturn )
        {
            sFinal += _T ( "SystemCallStub+4 bytes\r\n" ) ;
        }
        else
        {
            sFinal += _T ( "<No symbols>\r\n" ) ;
        }
        pStackDisp->m_pView->AppendText ( sFinal ) ;
        
        // Do any locals....
        m_cSymEng.EnumLocalVariables ( EnumerateLocalsIntoStackWnd ,
                                       2                           ,
                                       FALSE                       ,
                                       NULL                        ,
                                       &stFrame                    ,
                                       &dp.context                 ,
                                       pStackDisp                   ) ;
        
    }
    pStackDisp->m_pView->GetEditCtrl().SetSel ( 0 , 0 ) ;

        pStackDisp->m_pView->SetRedraw ( TRUE ) ;
}

void CWDBGProjDoc::OnViewRegisters()
{
    CString sTitle ;

    if ( FALSE == sTitle.LoadString ( IDS_REGISTERDLGTITLE ) )
    {
        ASSERT ( !"sTitle.LoadString ( IDS_REGISTERDLGTITLE ) failed" );
        sTitle = _T ( "Register" ) ;
    }

    CThreadPickerDlg cDlg ( &m_vDbgThreads , sTitle ) ;

    if ( IDOK == cDlg.DoModal ( ) )
    {
        // Create the view.
        CString sViewTitle ;
        sViewTitle.Format ( IDS_REGISTERTITLEFMT , cDlg.m_sThreadId ) ;

        CWDBGLogView * pView = CreateNewView ( sViewTitle , FALSE ) ;
        ASSERT ( NULL != pView ) ;
        if ( NULL == pView )
        {
            return ;
        }

        CRegisterDisplay cRegs ( pView , cDlg.m_hSelectedThread ) ;

        DisplayRegisterSet ( &cRegs ) ;

        m_vRegDisp.push_back ( cRegs ) ;
    }
}

void CWDBGProjDoc::OnUpdateViewRegisters(CCmdUI* pCmdUI)
{
    pCmdUI->Enable ( eException == m_eState ) ;
}

void CWDBGProjDoc::DisplayRegisterSet ( CRegisterDisplay * pRegDisp )
{
    ASSERT ( NULL != pRegDisp ) ;

    pRegDisp->m_pView->Clear ( ) ;

    DEBUGPACKET dp ;
    dp.hThread = pRegDisp->m_hThread ;
    dp.context.ContextFlags = CONTEXT_FULL ;

    BOOL bRet = DBG_GetThreadContext ( dp.hThread , &dp.context ) ;
    // The thread could be gone.
    if ( FALSE == bRet )
    {
        CString cErr ;
        if ( FALSE == cErr.LoadString ( IDS_THREADNOLONGEREXISTS ) )
        {
            ASSERT ( !"cErr.LoadString ( IDS_THREADNOLONGEREXISTS ) "
                      "failed" ) ;
            cErr = ERRORTEXT_THREADNOLONGEREXISTS ;
        }
        pRegDisp->m_pView->AppendText ( cErr ) ;
        return ;
    }

    TCHAR szBuff[ 1024 ] ;

    if ( FALSE == DisplayRegisters ( &dp , szBuff , _T ( "\r\n" ) ) )
    {
        ASSERT ( !"DisplayRegisters failed!" ) ;
        _tcscpy ( szBuff , _T ( "Unable to display registers!" ) ) ;
    }

    pRegDisp->m_pView->AppendText ( szBuff ) ;
}

void CWDBGProjDoc :: OnDebugBreak ( )
{
    // Just for my own peace of mind.
    ASSERT ( m_vDbgThreads.size ( ) > 0 ) ;
    
    // In the first edition of the book, it was much tougher to get
    // the DebugBreak code to work as I was supporting Win9x.  I had to
    // grind through and attempt to force the threads to execute a
    // breakpoint instruction as the next instruction.  That worked
    // fine, except if a process had all threads deadlocked, hence all
    // threads were waiting in kernel mode, there was no documented way
    // to get those threads to drop out of kernel mode.  Additionally,
    // that code did not work in certain situations on Windows XP.
    
    // Now that I dropped Win9x support, I can use the good old
    // CreateRemoteThread to do the deed, which works in all situations.
    // In fact, this is exactly what WinDBG does to affect it's debug
    // break.  VS.NET still uses some special undocumented means to stop
    // a process without creating a thread in that process.  The one
    // drawback to CreateRemoteThread is that all the DllMain's for
    // all loaded DLLs in the debuggee process are called before the
    // remote thread has a chance to call DebugBreak.  That's the
    // contract, and Microsoft is sticking to it and not making any
    // exceptions.  Consquently, using the CreateRemoteThread technique
    // may expose bugs in your DllMain process if you handle the
    // DLL_THREAD_ATTACH and DLL_THREAD_DETACH messages.
    
    HANDLE hProcess = GetDebuggeeProcessHandle ( ) ;
    DWORD  dwTID = 0 ;
    HANDLE hThread = DBG_CreateRemoteBreakpointThread ( hProcess ,
                                                        &dwTID    ) ;
    ASSERT ( NULL != hThread ) ;
    if ( NULL != hThread )
    {
        VERIFY ( CloseHandle ( hThread ) ) ;
        return ;
    }
}

void CWDBGProjDoc::OnUpdateDebugBreak(CCmdUI* pCmdUI)
{
    // Allowing the debug break before the loader BP is not a good idea.
    pCmdUI->Enable ( ( eRunning == m_eState    ) &&
                     ( TRUE == m_bSeenLoaderBP )    ) ;
}

void CWDBGProjDoc::OnDebugBreakpoints()
{
    CBreakpointsDlg cDlg ( &m_aBPArray , this ) ;

    if ( ( IDOK == cDlg.DoModal ( )     ) &&
         ( TRUE == cDlg.m_bModifiedList )    )
    {
        SetModifiedFlag ( ) ;
        // Fixup the breakpoints that the user just added/changed if the
        // debuggee is running.
        if ( TRUE == IsDebuggeeRunning ( ) )
        {
            ResolveAllUnresolvedBP ( ) ;
        }
    }
}

void CWDBGProjDoc :: ResolveAllUnresolvedBP ( void )
{
    INT_PTR iSize = m_aBPArray.GetSize ( ) ;
    if ( iSize > 0 )
    {
        HANDLE hProc = GetDebuggeeProcessHandle ( ) ;
        CLocationBP * pBP ;
        LPCVOID dwAddr ;
        for ( int i = 0 ; i < iSize ; i++ )
        {
            pBP = m_aBPArray[ i ] ;
            if ( eUnresolved == pBP->GetState ( ) )
            {
                // If the BP was on an address, and that address is not
                // mapped in to the debuggee yet, the ArmBreakpoint will
                // fail.  That's fine because it will not set the
                // enabled state.
                if ( TRUE ==
                     ConvertStringToAddr ( pBP->GetBreakpointName ( ) ,
                                           &dwAddr                    ))
                {
                    // This one can be set!
                    pBP->SetBreakpointLocation ( dwAddr ) ;

                    // Do the arming.
                    if ( TRUE != pBP->ArmBreakpoint ( hProc ) )
                    {
                        // If I could not arm, mark as disabled.
                        pBP->SetToDisabled ( ) ;
                    }
                }
            }
        }
    }
}

void CWDBGProjDoc :: ClearAllEnabledBP ( void )
{
    INT_PTR iSize = m_aBPArray.GetSize ( ) ;
    if ( iSize > 0 )
    {
        CLocationBP * pBP ;
        for ( int i = 0 ; i < iSize ; i++ )
        {
            pBP = m_aBPArray[ i ] ;
            if ( ( eEnabled == pBP->GetState ( )         ) ||
                 ( eWaitingForTrace == pBP->GetState ( ) )   )
            {
                pBP->ClearBreakpoint ( ) ;
                if ( eWaitingForTrace != pBP->GetState ( ) )
                {
                    pBP->SetToUnresolved( ) ;
                }
            }
        }
    }
}

void CWDBGProjDoc :: ResetBP ( void )
{
    INT_PTR iSize = m_aBPArray.GetSize ( ) ;
    if ( iSize > 0 )
    {
        CLocationBP * pBP ;
        for ( int i = 0 ; i < iSize ; i++ )
        {
            pBP = m_aBPArray[ i ] ;
            // If the BP is anything but disabled, reset it.
            if ( pBP->GetState ( ) > eDisabled )
            {
                pBP->SetToUnresolved( ) ;
            }
        }
    }
}
