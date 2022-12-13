/*----------------------------------------------------------------------
Debugging Applications for Microsoft .NET and Microsoft Windows
Copyright © 1997-2003 John Robbins -- All rights reserved.
----------------------------------------------------------------------*/
#ifndef _WDBGPROJDOC_H
#define _WDBGPROJDOC_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// Forward declaration.
class CWDBGProjDoc ;
#include "DocNotifyWnd.h"
#include "WDBGUser.h"
#include "WDBGLogView.h"
#include "Breakpoint.h"

// The states the UI can be in.
enum eDEBUGGEESTATES
{
    // The debuggee is either not started or it has finished.
    eStopped   ,
    // The debuggee is running full bore.
    eRunning   ,
    // The debuggee is halted because of an exception.
    eException
} ;

// A helper class that holds thread information.
class CDebuggeeThread
{
public  :
    CDebuggeeThread ( DWORD   dwTID       ,
                      HANDLE  hThread     ,
                      LPCVOID dwStartAddr  )
    {
        m_dwTID       = dwTID       ;
        m_hThread     = hThread     ;
        m_dwStartAddr = dwStartAddr ;
    }

    DWORD   m_dwTID       ;
    HANDLE  m_hThread     ;
    LPCVOID m_dwStartAddr ;
} ;

// The typedef for holding the thread information.
typedef vector<CDebuggeeThread> DBGTHREADVECT ;

// A helper class for holding the loaded DLL information.
class CLoadedModule
{
public  :
    CLoadedModule ( LPCVOID  dwBaseAddr ,
                    LPCVOID  dwHighAddr ,
                    SYM_TYPE eSymType   ,
                    LPCTSTR  szBaseName ,
                    LPCTSTR  szPath      )
    {
        m_dwBaseAddr = dwBaseAddr ;
        m_dwHighAddr = dwHighAddr ;
        m_eSymType   = eSymType   ;
        _tcscpy ( m_szBaseName , szBaseName ) ;
        _tcscpy ( m_szPath , szPath ) ;
    }

    LPCVOID  m_dwBaseAddr ;
    LPCVOID  m_dwHighAddr ;
    SYM_TYPE m_eSymType ;
    TCHAR    m_szBaseName [ MAX_PATH ] ;
    TCHAR    m_szPath [ MAX_PATH ] ;
} ;

typedef vector<CLoadedModule> LOADEDMODVECT ;

// The helper class to hold the memory display views so they can be
// updated when the debuggee stops at an exception.
class CMemDisplay
{
public  :

    CMemDisplay ( CWDBGLogView * pView        ,
                  LPCVOID        dwStartAddr  ,
                  UINT           iLen         ,
                  int            iWidth        )
    {
        m_pView       = pView       ;
        m_dwStartAddr = dwStartAddr ;
        m_iLen        = iLen        ;
        m_iWidth      = iWidth      ;
    }

    CWDBGLogView * m_pView        ;
    LPCVOID        m_dwStartAddr  ;
    UINT           m_iLen         ;
    int            m_iWidth       ;
} ;

typedef vector<CMemDisplay> MEMDISPLAYVECT ;

// The helper class for the callstack displays so they can be updated
// when the debuggee stops at an exception.
class CStackDisplay
{
public  :
    CStackDisplay ( CWDBGLogView * pView    ,
                    HANDLE         hThread   )
    {
        m_pView   = pView   ;
        m_hThread = hThread ;
    }

    CWDBGLogView * m_pView   ;
    HANDLE         m_hThread ;
} ;

typedef vector<CStackDisplay> STACKDISPLAYVECT ;

// The helper class for register displays so they can be updated when
// the debuggee stops at an exception.
typedef CStackDisplay CRegisterDisplay ;
typedef vector<CRegisterDisplay> REGISTERDISPLAYVECT ;

// The vector of one-shot breakpoints.
typedef CArray<COneShotBP,COneShotBP> ONESHOTBPARRAY ;

typedef CArray<CLocationBP*,CLocationBP*> BPARRAY ;

class CWDBGProjDoc : public CDocument
{
protected: // create from serialization only
    CWDBGProjDoc();
    DECLARE_DYNCREATE(CWDBGProjDoc)

// Attributes
public:

// Operations
public:

// Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CWDBGProjDoc)
    public:
    virtual BOOL OnNewDocument();
    virtual void Serialize(CArchive& ar);
    virtual BOOL OnOpenDocument(LPCTSTR lpszPathName);
    virtual void OnCloseDocument();
    virtual void SetTitle(LPCTSTR lpszTitle);
    virtual void SetPathName(LPCTSTR lpszPathName, BOOL bAddToMRU = TRUE);
    virtual BOOL OnSaveDocument(LPCTSTR lpszPathName);
    //}}AFX_VIRTUAL

    // To work around MFC lameness.
    void ViewDestroyed ( CView * pView ) ;

// Implementation
public:
    virtual ~CWDBGProjDoc();

    // Clears all data out of the various views.
    void ClearAllViews ( void ) ;
#ifdef _DEBUG
    virtual void AssertValid() const;
    virtual void Dump(CDumpContext& dc) const;
#endif

    // The special handlers that the helper window will call when it
    // recieves a WM_DBG_* message.
    DWORD HandleProcessExitEvent ( DWORD               dwPID ,
                                   PSTEXITPROCESSEVENT pEPE   ) ;
    DWORD HandleExceptionEvent ( DWORD             dwPID ,
                                 PSTEXCEPTIONEVENT pEE    ) ;
    DWORD HandleCreateProcessEvent ( DWORD                 dwPID ,
                                     PSTCREATEPROCESSEVENT pCTE   ) ;
    DWORD HandleThreadCreateEvent ( DWORD                dwPID ,
                                    PSTCREATETHREADEVENT pCTE   ) ;
    DWORD HandleThreadExitEvent ( DWORD                  dwPID ,
                                  PSTEXITTHREADEVENT     pETE   ) ;
    DWORD HandleLoadDllEvent ( DWORD            dwPID ,
                               PSTLOADDLLEVENT  pLDE   ) ;
    DWORD HandleUnloadedDllEvent ( DWORD             dwPID ,
                                   PSTUNLOADDLLEVENT pUDE   ) ;
    DWORD HandleOutputDebugStringEvent ( DWORD dwPID ,
                                PSTOUTPUTDEBUGSTRINGEVENT pODSE ) ;

protected   :
    DWORD HandleBreakpoint ( DWORD             dwPID ,
                             PSTEXCEPTIONEVENT pEE    ) ;
    DWORD HandleStep ( DWORD             dwPID ,
                       PSTEXCEPTIONEVENT pEE    ) ;
    DWORD HandleGeneralException ( DWORD             dwPID ,
                                   PSTEXCEPTIONEVENT pEE    ) ;

    // Handler for one-shot BPs.
    BOOL IsOneShotBP ( DWORD             dwPID ,
                       PSTEXCEPTIONEVENT pEE    ) ;
    // Handler for user BPs.
    BOOL IsUserBP ( DWORD             dwPID ,
                    PSTEXCEPTIONEVENT pEE    ) ;


// Other helper functions.
public      :
    // Converts an address to a symbol name, otherwise, converts the
    // address to a string.
    void ConvertAddrToString ( LPCVOID dwAddr , LPTSTR szName ) ;
    // Converts a string into an address by looking it up in the
    // symbol engine.  If the string is a properly formed hexidecimal
    // number, that can be converted as well.
    BOOL ConvertStringToAddr ( LPCTSTR szSymbol , LPCVOID * pdwAddr ) ;
    // Returns TRUE if the debuggee is active under the debug loop.
    BOOL IsDebuggeeRunning ( void ) ;

protected   :
    // Displays the active threads in the threads window.
    void AppendToActiveThreads ( CDebuggeeThread & cDbgThread ) ;
    // Removes the specified index from the threads window.
    void RemoveFromActiveThreads ( int iIndex ) ;
    // Displays active modules in the modules window.
    void AppendToActiveModules ( CLoadedModule & cMod ) ;
    // Removes the specified index from the modules window.
    void RemoveFromActiveModules ( int iIndex ) ;

    // Displays the memory information.
    void DisplayMemoryInfo ( CMemDisplay * pMemDisp ) ;
    // Displays a callstack.
    void DisplayCallStack ( CStackDisplay * pStackDisp ) ;
    // Displays a register set.
    void DisplayRegisterSet ( CRegisterDisplay * pRegDisp ) ;

    // Updates all the dynamic windows.
    void UpdateAllDynamicWindows ( void ) ;

    // Returns the thread handle associated with the thread ID.
    HANDLE FindThreadHandle ( DWORD dwTID ) ;

    // Resets the debuggee data structures and states to a known
    // starting point.
    void ResetDebuggeeInfoStates ( void ) ;

    // A helper function to create a new view.
    CWDBGLogView * CreateNewView ( LPCTSTR szTitle       ,
                                   BOOL    bDisableClose = TRUE ) ;

    // Helper function that tries to resolve all unresolved BPs.  This
    // is called at debuggee start and on each module load.
    void ResolveAllUnresolvedBP ( void ) ;
    // Helper function to clear all enabled BPs.
    void ClearAllEnabledBP ( void ) ;
    // Resets all breakpoints.  Call this function when the debuggee is
    // dead.
    void ResetBP ( void ) ;

protected   :
    static BOOL CALLBACK
            EnumerateLocalsIntoStackWnd ( DWORD64  dwAddr        ,
                                          LPCTSTR  szType        ,
                                          LPCTSTR  szName        ,
                                          LPCTSTR  szValue       ,
                                          int      iIndentLevel  ,
                                          PVOID    pUserCtx       ) ;


    // TRUE if this project was fakes from a binary name and
    // has not been saved.
    BOOL m_bFakedAndNotSaved ;

    // The name of the .EXE to run under the debugger.
    CString m_sEXEName ;
    // Command line options for the program.
    CString m_sCmdLineOpts ;
    // The display name. i.e., just the base filename.
    CString m_sDisplayName ;
    // The full name of the associated binary.
    CString m_sFullBinaryName ;
    // The option to stop at the loader breakpoint.
    BOOL    m_bStopAtLoaderBP ;

    // The symbol engine for the doc..
    CSymbolEngine m_cSymEng ;

    // When stopped at an exception of any kind, these indicate the
    // current thread and stuff.
    DWORD m_dwCurrentPID ;
    DWORD m_dwCurrentTID ;


    // The debuggee state.
    eDEBUGGEESTATES m_eState ;

    // The handle to coordinate between the document and the debugger.
    HANDLE m_hUserInputSyncEvent ;

    // The handle to the debug thread.
    HANDLE m_hDebugThread ;

    // The synchronization events for the debug loop.
    HANDLE m_hSynchEvents[ NUM_DEBUGEVENTS ] ;

    // The allocated CWDBGUser class.
    CWDBGUser m_cDbgUser ;

    // The document notification window.
    CDocNotifyWnd m_cDocWnd ;

    // The breakpoint information.
    BOOL m_bSeenLoaderBP ;

    // The special views that all debugging sessions get.
    CWDBGLogView * m_pLogView ;
    CWDBGLogView * m_pThreadsView ;
    CWDBGLogView * m_pModView ;

    // The map of active threads in the debuggee.
    DBGTHREADVECT  m_vDbgThreads ;
    // The map of loaded modules in the debuggee.
    LOADEDMODVECT  m_vLoadedMods ;
    // The active memory display windows.
    MEMDISPLAYVECT m_vMemDisp ;
    // The active stack display windows.
    STACKDISPLAYVECT m_vStackDisp ;
    // The active register display windows.
    REGISTERDISPLAYVECT m_vRegDisp ;

    // The active one shot breakpoints.
    ONESHOTBPARRAY  m_aOneShots ;
    // The array of oneshots when Debug|Break is selected.
    ONESHOTBPARRAY  m_aDebugBreakBPs ;

    // The array breakpoints.
    BPARRAY m_aBPArray ;
    
// My friend.
    friend CDocNotifyWnd ;

// Generated message map functions
protected:
    //{{AFX_MSG(CWDBGProjDoc)
    afx_msg void OnEditProject();
    afx_msg void OnDebugGo();
    afx_msg void OnUpdateDebugGo(CCmdUI* pCmdUI);
    afx_msg void OnDebugStop();
    afx_msg void OnUpdateDebugStop(CCmdUI* pCmdUI);
    afx_msg void OnViewDisassembly();
    afx_msg void OnUpdateViewDisassembly(CCmdUI* pCmdUI);
    afx_msg void OnViewMemory();
    afx_msg void OnUpdateViewMemory(CCmdUI* pCmdUI);
    afx_msg void OnViewSymbols();
    afx_msg void OnUpdateViewSymbols(CCmdUI* pCmdUI);
    afx_msg void OnViewCallstack();
    afx_msg void OnUpdateViewCallstack(CCmdUI* pCmdUI);
    afx_msg void OnViewRegisters();
    afx_msg void OnUpdateViewRegisters(CCmdUI* pCmdUI);
    afx_msg void OnDebugBreak();
    afx_msg void OnUpdateDebugBreak(CCmdUI* pCmdUI);
    afx_msg void OnDebugBreakpoints();
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()

private     :
    CWDBGProjDoc ( CWDBGProjDoc & ) { }
    CWDBGProjDoc & operator = ( const CWDBGProjDoc & )
        { return ( *this ) ; }

};

//{{AFX_INSERT_LOCATION}}

#endif // _WDBGPROJDOC_H
