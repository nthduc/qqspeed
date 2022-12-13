/*----------------------------------------------------------------------
Debugging Applications for Microsoft .NET and Microsoft Windows
Copyright © 1997-2003 John Robbins -- All rights reserved.
----------------------------------------------------------------------*/
#include "PCH.h"
#include "DebugInterface.h"
#include "DebuggeeInfo.h"

/*//////////////////////////////////////////////////////////////////////
                     File Scope Function Prototypes
//////////////////////////////////////////////////////////////////////*/

/*//////////////////////////////////////////////////////////////////////
                      File Scope Global Variables
//////////////////////////////////////////////////////////////////////*/
// The one global instance.
static CDebuggeeInfo * g_cDbgInfo = NULL ;

/*//////////////////////////////////////////////////////////////////////
        !!EXTERNAL TO DLL FUNCTION IMPLEMENTATION STARTS HERE!!
//////////////////////////////////////////////////////////////////////*/

HANDLE DEBUGINTERFACE_DLLINTERFACE __stdcall
    GetDebuggeeProcessHandle ( void )
{
    ASSERT ( NULL != g_cDbgInfo ) ;
    return ( g_cDbgInfo->GetProcessHandle ( ) ) ;
}

DWORD DEBUGINTERFACE_DLLINTERFACE __stdcall
    GetDebuggeeProcessId ( void )
{
    ASSERT ( NULL != g_cDbgInfo ) ;
    return ( g_cDbgInfo->GetProcessId ( ) ) ;
}

DWORD DEBUGINTERFACE_DLLINTERFACE __stdcall
    GetDebuggeeModuleName ( LPCVOID dwAddr , LPTSTR szName , int iLen )
{
    ASSERT ( NULL != g_cDbgInfo ) ;
    return ( g_cDbgInfo->GetFullModuleName ( dwAddr , szName , iLen ) );
}

LPCTSTR DEBUGINTERFACE_DLLINTERFACE __stdcall
    SymToString ( SYM_TYPE eType )
{
    LPCTSTR szRet = NULL ;
    switch ( eType )
    {
        case SymNone    :
            szRet = _T ( "NO" ) ;
            break ;
        case SymCoff    :
            szRet = _T ( "COFF" ) ;
            break ;
        case SymCv      :
            szRet = _T ( "CodeView" ) ;
            break ;
        case SymPdb     :
            szRet = _T ( "PDB" ) ;
            break ;
        case SymExport  :
            szRet = _T ( "Export" ) ;
            break ;
        case SymDeferred:
            szRet = _T ( "Deferred" ) ;
            break ;
        case SymSym     :
            szRet = _T ( "SYM" ) ;
            break ;
        case SymDia     :
            szRet = _T ( "DIA" ) ;
            break ;
        default :
            ASSERT ( !"Unknown symbol type!" ) ;
            szRet = _T ( "**Unknown Symbols**" ) ;
            break ;
    }
    return ( szRet ) ;
}

/*//////////////////////////////////////////////////////////////////////
        !!EXTERNAL TO FILE FUNCTION IMPLEMENTATION STARTS HERE!!
//////////////////////////////////////////////////////////////////////*/
LPDEBUGGEEINFO CreateDebuggeeInfo ( CDebugBaseUser * pUserClass ,
                                    LPCTSTR          szName      )
{
    ASSERT ( NULL == g_cDbgInfo ) ;
    if ( NULL != g_cDbgInfo )
    {
        return ( NULL ) ;
    }
    ASSERT ( NULL != pUserClass ) ;
    if ( NULL == pUserClass )
    {
        return ( NULL ) ;
    }
    g_cDbgInfo = new CDebuggeeInfo ( pUserClass , szName ) ;
    return ( g_cDbgInfo ) ;
}

BOOL DestroyDebuggeeInfo ( LPDEBUGGEEINFO pData )
{
    ASSERT ( NULL != pData ) ;
    ASSERT ( g_cDbgInfo == pData ) ;

    pData = pData ;

    delete g_cDbgInfo ;
    g_cDbgInfo = NULL ;
    return ( TRUE ) ;

}

LPDEBUGGEEINFO GetDebuggeeInfo ( DWORD dwPID )
{
    ASSERT ( NULL != g_cDbgInfo ) ;
    if ( g_cDbgInfo->GetProcessId ( ) == dwPID )
    {
        return ( g_cDbgInfo ) ;
    }
    return ( NULL ) ;
}

void ReinitializeDebuggeeInfo ( void )
{
    if ( NULL != g_cDbgInfo )
    {
        delete g_cDbgInfo ;
    }
}

/*//////////////////////////////////////////////////////////////////////
                        The CDebuggeeInfo Class
//////////////////////////////////////////////////////////////////////*/

CDebuggeeInfo :: CDebuggeeInfo ( CDebugBaseUser * pUserClass ,
                                 LPCTSTR          szName      )
{
    m_dwPID = 0 ;
    m_hProc = 0 ;
    m_bSymEngInit = FALSE ;
    m_bSeenLoaderBP = FALSE ;
    m_bSeenNTDLLLoad = FALSE ;
    m_pUserClass = pUserClass ;
    _tcscpy ( m_szName , szName ) ;
}

CDebuggeeInfo :: ~CDebuggeeInfo ( void )
{
    if ( TRUE == m_bSymEngInit )
    {
        m_cSymEng.SymCleanup ( ) ;
    }
}

// Set and Get's Galore!
void CDebuggeeInfo :: SetProcessId ( DWORD dwPID )
{
    CUseCriticalSection cs ( m_CS ) ;
    m_dwPID = dwPID ;
}
DWORD CDebuggeeInfo :: GetProcessId ( void ) const
{
    return ( m_dwPID ) ;
}

void CDebuggeeInfo :: SetProcessHandle ( HANDLE hProc )
{
    CUseCriticalSection cs ( m_CS ) ;
    m_hProc = hProc ;
}
HANDLE CDebuggeeInfo :: GetProcessHandle ( void ) const
{
    return ( m_hProc ) ;
}

LPCTSTR CDebuggeeInfo :: GetProcessName ( void )
{
    return ( m_szName ) ;
}

BOOL CDebuggeeInfo :: GetSeenNTDLLLoad ( void )
{
    return ( m_bSeenNTDLLLoad ) ;
}

void CDebuggeeInfo :: SetSeenNTDLLLoad ( void )
{
    m_bSeenNTDLLLoad = TRUE ;
}

DWORD CDebuggeeInfo :: GetFullModuleName ( LPCVOID dwAddr ,
                                           LPTSTR  szName ,
                                           int     iLen    )
{
    DWORD dwRetGMFEX = GetModuleFileNameEx ( m_hProc         ,
                                            (HMODULE)dwAddr  ,
                                             szName          ,
                                             iLen             ) ;
    ASSERT ( 0 != dwRetGMFEX ) ;
    return ( dwRetGMFEX ) ;
}


void CDebuggeeInfo :: AddModInfo ( LPCVOID dwAddr , HANDLE hFile )
{
    CUseCriticalSection cs ( m_CS ) ;

    CDllInfo cData ( dwAddr , hFile ) ;
    m_vDlls.push_back ( cData  ) ;
}

HANDLE CDebuggeeInfo :: FindModHandle ( LPCVOID dwAddr )
{
    CUseCriticalSection cs ( m_CS ) ;

    DLLINFOVECT::iterator i ;
    for ( i = m_vDlls.begin ( ) ;
          i != m_vDlls.end ( )  ;
          i++                    )
    {
        if ( i->m_dwAddr == dwAddr )
        {
            return ( i->m_hFile ) ;
        }
    }
    return ( INVALID_HANDLE_VALUE ) ;
}

void CDebuggeeInfo :: SetLoaderBreakpoint ( void )
{
    // Set the flag so that SymLoadModule will do the real load.
    m_bSeenLoaderBP = TRUE ;
}

BOOL CDebuggeeInfo :: GetLoaderBreakpoint ( void )
{
    return ( m_bSeenLoaderBP ) ;
}

BOOL CDebuggeeInfo :: InitializeSymEng ( void )
{
    ASSERT ( 0 != m_hProc ) ;

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
    m_bSymEngInit = m_cSymEng.SymInitialize ( m_hProc           ,
                                              (LPSTR)szSymPath  ,
                                              FALSE              ) ;
    if ( NULL != szSymPath )
    {
        delete [] szSymPath ;
    }
    
    ASSERT ( TRUE == m_bSymEngInit ) ;
    if ( TRUE == m_bSymEngInit )
    {
        TCHAR szDrive[ MAX_PATH ] ;
        TCHAR szDir[ MAX_PATH ] ;
        
        _tsplitpath ( m_szName , szDrive , szDir , NULL , NULL ) ;

        // Remove the trailing slash off the dir.
        szDir [ _tcslen ( szDir ) - 1 ] = _T ( '\0' ) ;

        _tcscat ( szDrive , szDir ) ;
        
        m_cSymEng.SymAppendSearchPath ( szDrive ) ;
     
    }
    return ( m_bSymEngInit ) ;
}

void CDebuggeeInfo :: CleanupSymEng ( void )
{
    VERIFY ( m_cSymEng.SymCleanup ( ) ) ;
    m_bSymEngInit = FALSE ;
}

LPCVOID CDebuggeeInfo :: SymLoadModule ( HANDLE  hFile      ,
                                         LPCVOID dwBaseAddr  )
{
    return ( SymLoadModule ( hFile , NULL , dwBaseAddr ) ) ;
}

LPCVOID CDebuggeeInfo :: SymLoadModule ( HANDLE    hFile      ,
                                         LPCTSTR   szName     ,
                                         LPCVOID   dwBaseAddr  )
{
    ASSERT ( 0 != m_hProc ) ;
    ASSERT ( TRUE == m_bSymEngInit ) ;

    DWORD64 dwSymLoad = 0 ;
    
    // Load the DLL into the symbol engine.
    dwSymLoad = m_cSymEng.SymLoadModule64 ( hFile               ,
                                            (PWSTR)szName       ,
                                            (PWSTR)NULL         ,
                                            (DWORD64)dwBaseAddr ,
                                            0                    ) ;
    // Add the module info to my list.
    AddModInfo ( dwBaseAddr , hFile ) ;

    return ( (LPCVOID)dwSymLoad ) ;
}

LPCVOID CDebuggeeInfo :: SymLoadModule ( LPCTSTR szMod      ,
                                         LPCVOID dwBaseAddr  )
{
    ASSERT ( 0 != m_hProc ) ;
    ASSERT ( TRUE == m_bSymEngInit ) ;
    
    DWORD64 dwSymLoad = 0 ;
    
    // Load the DLL into the symbol engine.
    dwSymLoad = m_cSymEng.SymLoadModule64 ( NULL                ,
                                            (PWSTR)szMod        ,
                                            (PWSTR)NULL         ,
                                            (DWORD64)dwBaseAddr ,
                                            0                    ) ;
    ASSERT ( 0 != dwSymLoad ) ;

    return ( (LPCVOID)dwSymLoad ) ;
}

void CDebuggeeInfo :: SymUnloadModule ( LPCVOID dwBaseAddr )
{
    ASSERT ( 0 != m_hProc ) ;
    ASSERT ( TRUE == m_bSymEngInit ) ;

    m_cSymEng.SymUnloadModule ( (DWORD64)dwBaseAddr ) ;
}

BOOL CDebuggeeInfo :: SymGetModuleInfo ( LPCVOID            dwAddr   ,
                                         PIMAGEHLP_MODULE64 pModInfo ,
                                         LPTSTR             szName   ,
                                         int                iLen      )
{
    ASSERT ( 0 != m_hProc ) ;
    ASSERT ( TRUE == m_bSymEngInit ) ;
    ASSERT ( FALSE == IsBadWritePtr ( pModInfo ,
                                      sizeof ( IMAGEHLP_MODULE64 ) ) ) ;

    BOOL bSymModInfo = m_cSymEng.SymGetModuleInfo ( (DWORD64)dwAddr ,
                                                    pModInfo         ) ;

    if ( ( TRUE == bSymModInfo ) && ( NULL != szName ) )
    {
        // I can't use TOOLHELP here as they can hang because they
        // create a thread in the debuggee.
        DWORD dwRetGMFEX = GetModuleFileNameEx ( m_hProc         ,
                                                 (HMODULE)dwAddr ,
                                                 szName          ,
                                                 iLen             ) ;
        if ( 0 == dwRetGMFEX )
        {
            return ( FALSE ) ;
        }
    }

    return ( bSymModInfo ) ;
}

BOOL CDebuggeeInfo :: SymGetModuleInfo ( PIMAGEHLP_MODULE64 pModInfo  ,
                                         LPCVOID            dwAddr     )
                                         
{
    ASSERT ( 0 != m_hProc ) ;
    ASSERT ( TRUE == m_bSymEngInit ) ;
    ASSERT ( FALSE == IsBadWritePtr ( pModInfo ,
                                      sizeof ( IMAGEHLP_MODULE64 ) ) ) ;

    BOOL bSymModInfo = m_cSymEng.SymGetModuleInfo ( (DWORD64)dwAddr ,
                                                    pModInfo         ) ;

    return ( bSymModInfo ) ;
}


/*//////////////////////////////////////////////////////////////////////
        !!INTERNAL TO FILE FUNCTION IMPLEMENTATION STARTS HERE!!
//////////////////////////////////////////////////////////////////////*/

