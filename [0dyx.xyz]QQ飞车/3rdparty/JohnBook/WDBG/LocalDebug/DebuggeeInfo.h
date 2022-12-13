/*----------------------------------------------------------------------
Debugging Applications for Microsoft .NET and Microsoft Windows
Copyright © 1997-2003 John Robbins -- All rights reserved.
----------------------------------------------------------------------*/

#ifndef _DEBUGGEEINFO_H
#define _DEBUGGEEINFO_H

// A simple class to wrap DLL information.
class CDllInfo
{
public      :
    CDllInfo ( LPCVOID dwAddr , HANDLE hFile )
    {
        m_dwAddr = dwAddr ;
        m_hFile = hFile ;
    }
    LPCVOID m_dwAddr ;
    HANDLE  m_hFile ;
} ;

// A typedef for holding the DLLs loaded.
typedef vector<CDllInfo> DLLINFOVECT ;

class CDebuggeeInfo
{
public  :
    CDebuggeeInfo ( CDebugBaseUser * pUserClass ,
                    LPCTSTR          szName      ) ;
    virtual ~CDebuggeeInfo ( void ) ;

    // Set and Get's Galore!
    void SetProcessId ( DWORD dwPID ) ;
    DWORD GetProcessId ( void ) const  ;

    void SetProcessHandle ( HANDLE hProc ) ;
    HANDLE GetProcessHandle ( void ) const  ;

    LPCTSTR GetProcessName ( void ) ;

    DWORD GetFullModuleName ( LPCVOID  dwAddr ,
                              LPTSTR   szName ,
                              int      iLen    ) ;
                              
    BOOL GetSeenNTDLLLoad ( void ) ;
    void SetSeenNTDLLLoad ( void ) ;

    // Tells this CDebuggeeInfo class that the loader breakpoint was
    // triggered.
    void SetLoaderBreakpoint ( void ) ;
    // Determines if the loader breakpoint was hit.
    BOOL GetLoaderBreakpoint ( void ) ;


    // Symbol engine stuff.
    BOOL InitializeSymEng ( void ) ;
    void CleanupSymEng ( void ) ;
    LPCVOID SymLoadModule ( HANDLE    hFile      ,
                            LPCVOID   dwBaseAddr  ) ;
    LPCVOID SymLoadModule ( HANDLE    hFile      ,
                            LPCTSTR   szName     ,
                            LPCVOID   dwBaseAddr  ) ;
    LPCVOID SymLoadModule ( LPCTSTR szMod      ,
                            LPCVOID dwBaseAddr  ) ;
    void SymUnloadModule ( LPCVOID dwBaseAddr ) ;
    BOOL SymGetModuleInfo ( LPCVOID            dwAddr          ,
                            PIMAGEHLP_MODULE64 pModInfo        ,
                            LPTSTR             szName   = NULL ,
                            int                iLen     = 0     ) ;
    BOOL SymGetModuleInfo ( PIMAGEHLP_MODULE64 pModInfo ,
                            LPCVOID            dwAddr    ) ;
                            
                            
protected   :
// Helper functions
    void AddModInfo ( LPCVOID dwAddr , HANDLE hFile ) ;
    HANDLE FindModHandle ( LPCVOID dwAddr ) ;

// Data
    // The name of the debuggee.
    TCHAR m_szName [ MAX_PATH ] ;
    // The critical section to protect this entire class.
    CCriticalSection m_CS ;

    // The pointer to the user class.
    CDebugBaseUser * m_pUserClass ;

    // The process ID for this information.
    DWORD m_dwPID ;
    // The process handle for this information.
    HANDLE m_hProc ;
    // The process file handle.
    HANDLE m_hProcFile ;

    // This class holds only those DLLs which load BEFORE the loader
    // breakpoint.  When the loader breakpoint is hit, I will then load
    // all of them into the symbol engine and notify the user.  This is
    // much more convenient than trying to diddle with the DLL loads as
    // they come in.
    DLLINFOVECT m_vDlls ;

    // The symbol engine for this process.
    CSymbolEngine m_cSymEng ;

    // The flag that indicates the symbol engine is initialized.
    BOOL m_bSymEngInit ;

    // The flag that indicates if the loader breakpoint has been seen.
    BOOL m_bSeenLoaderBP ;
    
    // Flag to indicate seen NTDLL.DLL load in the process.
    BOOL m_bSeenNTDLLLoad ;
    
} ;


typedef CDebuggeeInfo * LPDEBUGGEEINFO ;

// Creates a new debugging info dataset.
LPDEBUGGEEINFO CreateDebuggeeInfo ( CDebugBaseUser * const pUserClass ,
                                    LPCTSTR                szDebuggee );
// Destroys the existing debuggee info dataset.
BOOL DestroyDebuggeeInfo ( LPDEBUGGEEINFO pData ) ;
// Gets the debuggee info data for the specified PID.
LPDEBUGGEEINFO GetDebuggeeInfo ( DWORD dwPID ) ;
// Reinitializes the debuggee info.
void ReinitializeDebuggeeInfo ( void ) ;

#endif  // _DEBUGGEEINFO_H


