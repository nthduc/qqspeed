/*----------------------------------------------------------------------
Debugging Applications for Microsoft .NET and Microsoft Windows
Copyright © 1997-2003 John Robbins -- All rights reserved.
----------------------------------------------------------------------*/
#include "stdafx.h"
#include "FastTrace.h"
#include "FastTraceFile.h"

/*//////////////////////////////////////////////////////////////////////
// File Scope Constants, Defines and Types.
//////////////////////////////////////////////////////////////////////*/
// The author of this file.
#ifdef SUPERASSERT_EMAIL
#undef SUPERASSERT_EMAIL
#define SUPERASSERT_EMAIL "john@wintellect.com"
#endif

// The typedef for the array of file handles.
typedef JSimpleArray < FTFileWriter * > FILEARRAY ;

/*//////////////////////////////////////////////////////////////////////
// File Scope Globals.
//////////////////////////////////////////////////////////////////////*/
// The TLS value used to hold the file handle.
static DWORD g_dwTLSSlot = TLS_OUT_OF_INDEXES ;

// The current user settable settings.
static FASTTRACEOPTIONS g_stTraceOpts =
{
    // Write through.
    FALSE ,
    // Do timings.
    FALSE ,
    // Debugging output function.
    NULL
} ;

// The critical section that protects the file handle vector.
static CRITICAL_SECTION g_ArrayCS ;

// The array of file handles so I can flush them on a crash.
static FILEARRAY g_FileArray ;

// The current snap number for this process.
static LONG g_lSnapNumber = 1 ;

// The sequence number for this process.
static ULONG g_lSequenceNumber = 0 ;
        
/*//////////////////////////////////////////////////////////////////////
// File Scope Prototypes.
//////////////////////////////////////////////////////////////////////*/
// Returns the file for the thread.  If this returns
// INVALID_HANDLE_VALUE, there was a problem with the file so you should
// ignore writing to it.
FTFileWriter * GetThreadFile ( void ) ;

// Does the raw file creating.
HANDLE CreateRawThreadFile ( DWORD dwTID ) ;

// Helper functions to isolate the global array access so it's thread
// safe.
BOOL AddFileToArray ( FTFileWriter * pFile ) ;
BOOL RemoveFileFromArray ( FTFileWriter * pFile ) ;
void CloseAllFilesInArray ( void ) ;

/*//////////////////////////////////////////////////////////////////////
// PUBLIC CODE STARTS HERE
//////////////////////////////////////////////////////////////////////*/

// Your average DllMain.  Simply handles the work to create and destroy
// the TLS slots.
BOOL APIENTRY DllMain ( HMODULE /*hModule          */ ,
                        DWORD   ul_reason_for_call    ,
                        LPVOID  /*lpReserved       */   )
{
    BOOL bRet = TRUE ;
    
    if ( DLL_PROCESS_ATTACH == ul_reason_for_call )
    {
        // Gotta do the TLS slot allocation.
        g_dwTLSSlot = TlsAlloc ( ) ;
        ASSERT ( TLS_OUT_OF_INDEXES != g_dwTLSSlot ) ;
        if ( TLS_OUT_OF_INDEXES == g_dwTLSSlot )
        {
            // Got a serious problem.
            bRet = FALSE ;
        }
        else
        {
            // Initialize the array class critical section.
            bRet = InitializeCriticalSectionAndSpinCount ( &g_ArrayCS ,
                                                           4000       );
        }
        ASSERT ( TRUE == bRet ) ;
    }
    else if ( DLL_PROCESS_DETACH == ul_reason_for_call )
    {
        ASSERT ( TLS_OUT_OF_INDEXES != g_dwTLSSlot ) ;
        if ( TLS_OUT_OF_INDEXES != g_dwTLSSlot )
        {
            VERIFY ( TlsFree ( g_dwTLSSlot ) ) ;
            g_dwTLSSlot = TLS_OUT_OF_INDEXES ;

            // Clear out any remaining file handles.
            CloseAllFilesInArray ( ) ;

            // I'm done with the critical section.
            DeleteCriticalSection ( &g_ArrayCS ) ;
        }
    }
    else if ( DLL_THREAD_DETACH == ul_reason_for_call )
    {
        // A thread is shutting down so get the TLS value.  If it exists
        // I can close the file handle.
        ASSERT ( TLS_OUT_OF_INDEXES != g_dwTLSSlot ) ;
        if ( TLS_OUT_OF_INDEXES != g_dwTLSSlot )
        {
            FTFileWriter * pFile = (FTFileWriter *)
                                           TlsGetValue ( g_dwTLSSlot ) ;
            if ( INVALID_HANDLE_VALUE != pFile )
            {
                // Close this file.
                bRet = pFile->Close ( ) ;
                ASSERT ( TRUE == bRet ) ;
                // Remove this handle from the array of handles.
                bRet = RemoveFileFromArray ( pFile ) ;
                ASSERT ( bRet ) ;
            }
        }
    }
    return ( bRet ) ;
}

FASTTRACE_DLLINTERFACE void __stdcall
                SetFastTraceOptions ( LPFASTTRACEOPTIONS pOpts )
{
    InterlockedExchange ( (LPLONG)&(g_stTraceOpts.bWriteThrough)  ,
                          pOpts->bWriteThrough                     ) ;
    InterlockedExchange ( (LPLONG)&(g_stTraceOpts.bDoTimings) ,
                          pOpts->bDoTimings                    ) ;
                          
// InterlockedExchangePointer is not properly defined for Win32....
#pragma warning ( disable : 4311 4312 )
    InterlockedExchangePointer ( (PVOID)&(g_stTraceOpts.pDebugOut) ,
                                 (PVOID)pOpts->pDebugOut          ) ;
#pragma warning ( default : 4311 4312 )
}

FASTTRACE_DLLINTERFACE void __stdcall
                GetFastTraceOptions ( LPFASTTRACEOPTIONS pOpts )
{
    pOpts->bWriteThrough = g_stTraceOpts.bWriteThrough ;
    pOpts->bDoTimings = g_stTraceOpts.bDoTimings ;
    pOpts->pDebugOut = g_stTraceOpts.pDebugOut ;
}

FASTTRACE_DLLINTERFACE void FastTrace ( LPCTSTR szFmt , ... )
{
    FTDATA stData ;

    if ( TRUE == g_stTraceOpts.bDoTimings )
    {
        // Get the current time stamp.
        GetSystemTimeAsFileTime ( &stData.stTime ) ;
    }
    else
    {
        stData.stTime.dwHighDateTime = 0 ;
        stData.stTime.dwLowDateTime = 0 ;
    }
        
    FTFileWriter * pFile = GetThreadFile ( ) ;
    if ( (FTFileWriter*)INVALID_HANDLE_VALUE != pFile )
    {
        // Take care of the sequence number as this one'll be written.
        stData.lSeqNum =
                   InterlockedIncrement ( (LPLONG)&g_lSequenceNumber ) ;

        va_list args ;
        va_start ( args , szFmt ) ;
        
#ifdef _DEBUG
        HRESULT hr =
#endif
                     StringCchVPrintf ( stData.szTrace    ,
                                        MAX_FASTTRACE_LEN ,
                                        szFmt             ,
                                        args               ) ;
        // This will trigger if STRSAFE_E_INSUFFICIENT_BUFFER is
        // returned, which is good and user's need to know when they
        // are pumping too much data in debug builds.
        ASSERT ( SUCCEEDED ( hr ) ) ;
        
        va_end ( args ) ;
        
        // Write it to the file.
        BOOL bRet = pFile->WriteString ( &stData ) ;
        ASSERT ( TRUE == bRet ) ;
        
        // Was there any kind of problem writing?
        if ( FALSE == bRet )
        {
            // Yep, start by clearing this slot in TLS.
            TlsSetValue ( g_dwTLSSlot                   ,
                          (LPVOID)INVALID_HANDLE_VALUE   ) ;
            // Close down the file.
            pFile->Close ( ) ;
            // Take this file out of my file array.
            RemoveFileFromArray ( pFile ) ;
            // Free the memory.
            delete pFile ;
        }
    }
}

FASTTRACE_DLLINTERFACE void FlushFastTraceFiles ( void )
{
    EnterCriticalSection ( &g_ArrayCS ) ;
    for ( int i = 0 ; i < g_FileArray.GetSize ( ) ; i++ )
    {
        g_FileArray[i]->Flush ( ) ;
    }
    LeaveCriticalSection ( &g_ArrayCS ) ;
}

FASTTRACE_DLLINTERFACE void SnapFastTraceFiles ( void )
{
    EnterCriticalSection ( &g_ArrayCS ) ;
    for ( int i = 0 ; i < g_FileArray.GetSize ( ) ; i++ )
    {
        g_FileArray[i]->SnapFile ( ) ;
    }
    InterlockedIncrement ( &g_lSnapNumber ) ;
    LeaveCriticalSection ( &g_ArrayCS ) ;
}

/*//////////////////////////////////////////////////////////////////////
// INTERNAL CODE STARTS HERE
//////////////////////////////////////////////////////////////////////*/

FTFileWriter * GetThreadFile ( void )
{
    ASSERT ( TLS_OUT_OF_INDEXES != g_dwTLSSlot ) ;
    if ( TLS_OUT_OF_INDEXES == g_dwTLSSlot )
    {
        return ( (FTFileWriter*)(LPVOID)INVALID_HANDLE_VALUE ) ;
    }
    
    FTFileWriter * pRetValue = (FTFileWriter *)
                                           TlsGetValue ( g_dwTLSSlot ) ;
    // The first time TlsGetValue is called, it returns a zero.  That's
    // my sign that I need to create a new FTFileWriter for this thread.
    // (A return of INVALID_HANDLE_VALUE indicates a thread data has
    // been closed).
    if ( 0 == pRetValue )
    {
        BOOL bRet = FALSE ;
        DWORD dwTID = GetCurrentThreadId ( ) ;
        HANDLE hFile = CreateRawThreadFile ( dwTID ) ;
        if ( INVALID_HANDLE_VALUE != hFile )
        {
            // Allocate the file writer class.
            pRetValue = new FTFileWriter ( ) ;
            // Initialize the file.
            bRet = pRetValue->Initialize ( hFile , dwTID ) ;
            ASSERT ( TRUE == bRet ) ;
            if ( FALSE == bRet )
            {
                // With a problem writing the header, don't mess with
                // the file any more.
                pRetValue->Close ( ) ;
                delete pRetValue ;
                hFile = INVALID_HANDLE_VALUE ;
                pRetValue = (FTFileWriter *)INVALID_HANDLE_VALUE ;
            }
        }
        
        // At lease initialize this thread's slot to
        // INVALID_HANDLE_VALUE so if AddFileToArray fails, I've got
        // the TLS value already initialized.
        bRet = TlsSetValue ( g_dwTLSSlot                 ,
                            (LPVOID)INVALID_HANDLE_VALUE  ) ;
        
        ASSERT ( bRet ) ;
        // If it's not an invalid handle value, drop it into the array.
        if ( INVALID_HANDLE_VALUE != pRetValue )
        {
            bRet = AddFileToArray ( pRetValue ) ;
            ASSERT ( TRUE == bRet ) ;
            if ( FALSE == bRet )
            {
                // Reset the TLS value.
                TlsSetValue ( g_dwTLSSlot                   ,
                              (LPVOID)INVALID_HANDLE_VALUE   ) ;
                // Close the writer.
                pRetValue->Close ( ) ;
                // Free the writer class memory.
                delete pRetValue ;
                // Indicate an error condition.
                pRetValue = (FTFileWriter*)(LPVOID)INVALID_HANDLE_VALUE;
            }
            else
            {
                // Set the value into this thread's slot.
                bRet = TlsSetValue ( g_dwTLSSlot , (LPVOID)pRetValue ) ;
                ASSERT ( TRUE == bRet ) ;
                if ( FALSE == bRet )
                {
                    // Gak!  What can I do?
                    pRetValue->Close ( ) ;
                    delete pRetValue ;
                    pRetValue = (FTFileWriter*)
                                          (LPVOID)INVALID_HANDLE_VALUE ;
                }
            }
        }
    }
    return ( pRetValue ) ;
}

HANDLE CreateRawThreadFile ( DWORD dwTID )
{
    HANDLE hRet = INVALID_HANDLE_VALUE ;
    TCHAR szFileName [ MAX_PATH + 1 ] ;
    BOOL bRet = BuildThreadFileName ( szFileName ,
                                      MAX_PATH   ,
                                      dwTID       ) ;
    ASSERT ( TRUE == bRet ) ;
    if ( TRUE == bRet )
    {
        DWORD dwFlags = FILE_ATTRIBUTE_ARCHIVE |
                            FILE_FLAG_SEQUENTIAL_SCAN ;
        if ( TRUE == g_stTraceOpts.bWriteThrough )
        {
            dwFlags |= FILE_FLAG_WRITE_THROUGH ;
        }
            
        // I've got the filename so I can now create the file.
        hRet = CreateFile ( szFileName         ,
                            GENERIC_WRITE      ,
                            FILE_SHARE_READ    ,
                            NULL               ,
                            CREATE_ALWAYS      ,
                            dwFlags            ,
                            NULL                ) ;
       ASSERT ( INVALID_HANDLE_VALUE != hRet ) ;
    }
    return ( hRet ) ;
}

LONG GetSnapNumber ( void )
{
    return ( g_lSnapNumber ) ;
}

BOOL AddFileToArray ( FTFileWriter * pFile )
{
    EnterCriticalSection ( &g_ArrayCS ) ;
    BOOL bRet = g_FileArray.Add ( pFile ) ;
    LeaveCriticalSection ( &g_ArrayCS ) ;
    ASSERT ( TRUE == bRet ) ;
    return ( bRet ) ;
}

BOOL RemoveFileFromArray ( FTFileWriter * pFile )
{
    EnterCriticalSection ( &g_ArrayCS ) ;
    BOOL bRet = g_FileArray.Remove ( pFile ) ;
    LeaveCriticalSection ( &g_ArrayCS ) ;
    ASSERT ( TRUE == bRet ) ;
    if ( TRUE == bRet )
    {
        delete pFile ;
    }
    return ( bRet ) ;
}

void CloseAllFilesInArray ( void )
{
    EnterCriticalSection ( &g_ArrayCS ) ;
    BOOL bRet = FALSE ;
    for ( int i = 0 ; i < g_FileArray.GetSize ( ) ; i++ )
    {
        bRet = g_FileArray[ i ]->Close ( ) ;
        ASSERT ( TRUE == bRet ) ;
        delete g_FileArray[ i ] ;
    }
    g_FileArray.RemoveAll ( ) ;
    LeaveCriticalSection ( &g_ArrayCS ) ;
}
