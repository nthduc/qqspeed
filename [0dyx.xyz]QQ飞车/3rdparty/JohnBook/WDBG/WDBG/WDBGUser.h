/*----------------------------------------------------------------------
Debugging Applications for Microsoft .NET and Microsoft Windows
Copyright © 1997-2003 John Robbins -- All rights reserved.
----------------------------------------------------------------------*/
/*//////////////////////////////////////////////////////////////////////

This file contains the class that is passed on to the debug loop through
the interface set out in DebugInterface.h.  The important thing to
remember here is that everything in this class is called in the DEBUG
LOOP thread context.  Therefore, it cannot access anything in the main
CWDBGProjDoc class.

To control the synchronization, I do two things.  The first is that all
the information in this class is sent through Windows messages to the
main app's thread.  The document creates a hidden window in order to
process these messages.  If the UI needs something from the user, the
second level of synchronization is done through a standard Windows
event.  The debug thread will block on this event until the event is
signaled in the UI thread.

When sending the messages, I use SendMessage.  However, I think that I
could change this to a SendMessageTimeout.  That way, you get the
benefit of the SendMessage, but you do not block permanently.  I have
played around with it both ways.  The only problem with
SendMessageTimeout is that it gets very hard to debug WDBG because you
can easily timeout and information will not get through.  This is a case
where the debugger gets in your way a little.

The Output messages are handled slightly differently.  Since those will
be going straight to the logging window, I use the HWND of the view.
Additionally, since I do not want to block on each output event, I use
PostMessage to get the information over.  This means that I must keep a
circular array of buffers that hold the output so that the information
is still valid when the UI thread gets around to processing it.  I also
handle the LoadedDllInfo function the same way.

Just to reiterate: the CWDBGUser class does not do anything with the
debuggee.  It just acts as a synchronization and communications channel
between the UI and the debug loop thread.  Additionally, the debug
thread does nothing to do the fancy MFC multithreaded link up stuff so
you cannot call any MFC functions in the CWDBGUser class.

//////////////////////////////////////////////////////////////////////*/

#ifndef _WDBGUSER_H
#define _WDBGUSER_H

/*//////////////////////////////////////////////////////////////////////
                      MESSAGES AND PARAMETER TYPES
//////////////////////////////////////////////////////////////////////*/
// Please note that not all of these are used.
// They are here if you need them.

// For the CDebugBaseUser::Output function.
// WPARAM = nothing
// LPARAM = The data string to output.
#define WM_DBG_OUTPUT (WM_USER + 0x100)

// For the CDebugBaseUser::ExitProcessEvent
typedef struct tag_STEXITPROCESSEVENT
{
    DWORD   dwThreadId   ;
    DWORD   dwExitCode   ;
    // What is returned to the debug loop.
    LPDWORD pdwReturnVal ;
} STEXITPROCESSEVENT , * PSTEXITPROCESSEVENT ;
// WPARAM = The process ID.
// LPARAM = The PSTEXITPROCESSEVENT structure.
#define WM_DBG_EXITPROCESSEVENT (WM_USER + 0x101)

// For the CDebugBaseUser::ExceptionEvent
typedef struct tag_STEXCEPTIONEVENT
{
    DWORD                  dwThreadId   ;
    EXCEPTION_DEBUG_INFO * pEDI         ;
    // What is returned to the debug loop.
    LPDWORD                pdwReturnVal ;
} STEXCEPTIONEVENT , * PSTEXCEPTIONEVENT ;
// WPARAM = The process ID.
// LPARAM = The PSTEXCEPTIONEVENT structure.
#define WM_DBG_EXCEPTIONEVENT (WM_USER + 0x102)

// For the CDebugBaseUser::CreateThreadEvent
typedef struct tag_STCREATETHREADEVENT
{
    DWORD                      dwThreadId ;
    CREATE_THREAD_DEBUG_INFO * pCTDI      ;
    // What is returned to the debug loop.
    LPDWORD                pdwReturnVal ;
} STCREATETHREADEVENT , * PSTCREATETHREADEVENT ;
// WPARAM = The process ID.
// LPARAM = The PSTCREATETHREADEVENT structure.
#define WM_DBG_CREATETHREADEVENT (WM_USER + 0x103)

// For the CDebugBaseUser::CreateProcessEvent
typedef struct tag_STCREATEPROCESSEVENT
{
    DWORD                       dwThreadId ;
    CREATE_PROCESS_DEBUG_INFO * pCPDI      ;
    // What is returned to the debug loop.
    LPDWORD                pdwReturnVal ;
} STCREATEPROCESSEVENT , * PSTCREATEPROCESSEVENT ;
// WPARAM = The process ID.
// LPARAM = The PSTCREATETHREADEVENT structure.
#define WM_DBG_CREATEPROCESSEVENT (WM_USER + 0x104)

// For the CDebugBaseUser::ExitThreadEvent
typedef STEXITPROCESSEVENT STEXITTHREADEVENT ;
typedef PSTEXITPROCESSEVENT PSTEXITTHREADEVENT ;
// WPARAM = The process ID.
// LPARAM = The PSTEXITTHREADEVENT structure.
#define WM_DBG_EXITTHREADEVENT (WM_USER + 0x105)

// For the CDebugBaseUser::LoadDllEvent
typedef struct tag_STLOADDLLEVENT
{
    DWORD                 dwThreadId          ;
    LOAD_DLL_DEBUG_INFO * pLDDI               ;
    TCHAR                 szName [ MAX_PATH ] ;
    PIMAGEHLP_MODULE64    pModInfo            ;
    // What is returned to the debug loop.
    LPDWORD               pdwReturnVal ;
} STLOADDLLEVENT , * PSTLOADDLLEVENT ;
// WPARAM = The process ID.
// LPARAM = The PSTLOADDLLEVENT structure.
#define WM_DBG_LOADDLLEVENT (WM_USER + 0x106)

// For the CDebugBaseUser::UnloadDllEvent
typedef struct tag_STUNLOADDLLEVENT
{
    DWORD   dwThreadId   ;
    LPCVOID dwBaseAddr   ;
    // What is returned to the debug loop.
    LPDWORD pdwReturnVal ;
} STUNLOADDLLEVENT , * PSTUNLOADDLLEVENT ;
// WPARAM = The process ID.
// LPARAM = The PSTUNLOADDLLEVENT structure.
#define WM_DBG_UNLOADDLLEVENT (WM_USER + 0x108)

// For the CDebugBaseUser::OutputDebugStringEvent
typedef struct tag_STOUTPUTDEBUGSTRINGEVENT
{
    DWORD   dwThreadId   ;
    LPCTSTR szString     ;
    // What is returned to the debug loop.
    LPDWORD pdwReturnVal ;
} STOUTPUTDEBUGSTRINGEVENT , * PSTOUTPUTDEBUGSTRINGEVENT ;
// WPARAM = The process ID.
// LPARAM = The PSTOUTPUTDEBUGSTRINGEVENT structure.
#define WM_DBG_OUTPUTDEBUGSTRINGEVENT (WM_USER + 0x109)

/*//////////////////////////////////////////////////////////////////////
                          The CWDBGUser class
//////////////////////////////////////////////////////////////////////*/
// The number of Output buffers and DllLoadedInfo structures to
// allocate.
const int k_NUMPOSTITEMS = 8 ;
// The size of each output buffer.
const int k_BUFFSIZE = 512 ;

class CWDBGUser : public CDebugBaseUser
{
public  :
// Constructors and destructors.
    CWDBGUser ( void ) ;

    virtual ~CWDBGUser ( void ) ;

    // Initializes this class.
    BOOL Initialize ( HWND    hMainWnd    ,
                      HWND    hOutputWnd  ,
                      LPCTSTR szEventName  ) ;
    BOOL IsInitialized ( void ) ;

    // A general textual output function.
    virtual void Output ( LPCTSTR szFmt , ... ) ;

    virtual LPCTSTR ConvertCRLF ( LPTSTR szBuff , int iBuffLenp ) ;

    virtual void OutputCRLF ( void )
    {
        Output ( _T ( "\r\n" ) ) ;
    }


    // This function is called when the debuggee ends of its
    // own accord.  In other words, the debuggee exited.
    virtual void ExitProcessEvent ( DWORD dwProcessId ,
                                    DWORD dwThreadId  ,
                                    DWORD dwExitCode   ) ;

    // The exception handler.
    virtual
    DWORD ExceptionEvent ( DWORD                  dwProcessId ,
                           DWORD                  dwThreadId  ,
                           EXCEPTION_DEBUG_INFO & stEDI        ) ;

    // Process creation handler.
    virtual
    DWORD CreateProcessEvent ( DWORD                       dwProcessId,
                               DWORD                       dwThreadId ,
                               CREATE_PROCESS_DEBUG_INFO & stCPDI     );

    // Thread notifications.
    virtual
    DWORD CreateThreadEvent ( DWORD                      dwProcessId ,
                              DWORD                      dwThreadId  ,
                              CREATE_THREAD_DEBUG_INFO & stCTDI       );
    virtual
    DWORD ExitThreadEvent ( DWORD            dwProcessId ,
                            DWORD            dwThreadId  ,
                            DWORD            dwExitCode   ) ;

    virtual
    DWORD LoadDllEvent ( DWORD                 dwProcessId ,
                         DWORD                 dwThreadId  ,
                         LOAD_DLL_DEBUG_INFO & stLDDI      ,
                         LPCTSTR               szName      ,
                         IMAGEHLP_MODULE64 &   ModInfo      ) ;

    virtual
    DWORD UnloadDllEvent ( DWORD   dwProcessId ,
                           DWORD   dwThreadId  ,
                           LPCVOID lpBaseOfDll  ) ;
                           
    virtual
    DWORD OutputDebugStringEvent ( DWORD   dwProcessId ,
                                   DWORD   dwThreadId  ,
                                   LPCTSTR szString     ) ;
                           

protected       :

    // A helper function to wrap sending the message to the UI thread.
    // If this returns FALSE, the message was not processed and should
    // be considered a fatal error.
    BOOL SendMsgToUI ( UINT    uiMsg    ,
                       WPARAM  wParam   ,
                       LPARAM  lParam    ) ;

    // The main HWND these messages are sent to.
    HWND    m_hMainWnd   ;
    // The HWND for handling only Output message calls.
    HWND    m_hOutputWnd ;
    // The event the debug thread will stall on.
    HANDLE  m_hWaitEvent ;

    // The array of output buffers.
    LPTSTR m_aszOutBuffs[ k_NUMPOSTITEMS ] ;
    // The current output buffer.
    int m_iCurrBuff ;

} ;


#endif  // _WDBGUSER_H
