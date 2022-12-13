/*----------------------------------------------------------------------
Debugging Applications for Microsoft .NET and Microsoft Windows
Copyright © 1997-2003 John Robbins -- All rights reserved.
----------------------------------------------------------------------*/
#include "stdafx.h"

// The stack variable overflow is caught because of /RTCs.
// /RTCs also catches _cdecl/__stdcall mismatches.
void DoLocalStackOverflow ( void )
{
    TCHAR szBuff1[ 10 ] ;
    TCHAR szBuff2[ 10 ] ;
    TCHAR szBuff3[ 10 ] ;

    // Here's where the overrun happens.
    _tcscpy ( szBuff2 , _T ( "1234567890" ) ) ;
    
    // Avoid compiler errors...
    _tcscpy ( szBuff1 , _T ( "12345" ) ) ;
    _tcscpy ( szBuff3 , _T ( "12345" ) ) ;
    
    // The overflow check occurs at the end of the function.
}

// Have to shut off this error to compile.
// C4244: conversion from '' to '', possible loss of data
#pragma warning ( disable : 4244 )
// The data trucation is caught because of /RTCc
char DoTruncateValue ( void )
{
    int iVal = 0xFFFFFF ;
    // With /W4, a C4244 error will tell you that you are loosing
    // bits.
    return ( iVal ) ;
}
#pragma warning ( default : 4244 )

// C4701: local variable '' may be used without having been initialized.
#pragma warning ( disable : 4701 )
// The uninitialized usage is caught with the /RTCu
int DoUninitialized ( int k )
{
    int i ;

    if ( k != 0 )
    {
        i = 3 ;
    }
    // With /W4, you will get a C4701 indicating i MAY be used without
    // having been initialized.
    // The /RTCu error will only pop when k != 0.
    return( i ) ;
}
#pragma warning ( default : 4701 )

#define DEBUGHANDLER

#ifdef DEBUGHANDLER
#define TRACE ::printf
#else
#define TRACE (void)0
#endif

/*/////////////////////////////////////////////////////////////////////
FUNCTION        :   HandleRTCFailure
DESCRIPTION     :
    The Run Time Checking (RTC) handler function when NOT running
under the debugger.  When running under a debugger, this function
is ignored.  Consequently, you can't debug this puppy at all!
PARAMETERS      :
    iError   - The type of error as reported through _RTC_SetErrorType.
               Note that I don't use this parameter.
    szFile   - The source filename where the error occurred.
    iLine    - The line where the error occurred.
    szModule - The module where the error occurred.
    szFormat - The printf-style format string for the variable
               parameters.
               Note that I don't use this parameter except to get the
               values out of the variables parameters.
    ...      - The "variable" length parameter list.  There are only
               two values passed here.  The first is the RTC Error
               number.
                    1 = /RTCc
                    2 = /RTCs
                    3 = /RTCu
                The second is the string that appears when the debugger
                shows the message.  This is only important for /RTCs and
                /RTCu as those show the variable where the problem
                happened.
RETURN VALUES   :
    TRUE  - Cause a _DebugBreak to be called after this function
            returns.
    FALSE - Continue execution.
/////////////////////////////////////////////////////////////////////*/
// Turn off run time checks for this function so I don't go recursive
// on myself.
#pragma runtime_checks("", off)

// The critical section to protect against reentrancy in the
// HandleRTCFailure function.
CRITICAL_SECTION g_csRTCLock ;

int HandleRTCFailure ( int          /*iError*/   ,
                       const char * szFile       ,
                       int          iLine        ,
                       const char * szModule     ,
                       const char * szFormat     ,
                       ...                        )
{
    // Just say no to reentrant code!
    EnterCriticalSection ( &g_csRTCLock ) ;

    // Get the two variable length parameters.  I guess they plan on
    // adding a ton of these RTC checks in the future.
    va_list vl ;

    va_start ( vl , szFormat ) ;

    // The first one is the number of the error that occured.
    _RTC_ErrorNumber RTCErrNum = va_arg ( vl , _RTC_ErrorNumber ) ;
    
    // The second is the additional description of the error.
    char * szErrorVariableDesc = va_arg ( vl , char * ) ;

    va_end ( vl ) ;

    TCHAR szBuff [ 512 ] ;

    // Get the error description text based off the error number.
    const char *szErr = _RTC_GetErrDesc ( RTCErrNum ) ;

    // Make sure szFile and szModule have something in them.
    if ( NULL == szFile )
    {
        szFile = "Unknown File" ;
    }
    if ( NULL == szModule )
    {
        szModule = "Unknown Module" ;
    }

    // If it's any error other than /RTCc, I can show some cool info
    // that includes the variable in question!
    if ( 1 != RTCErrNum )
    {
        _stprintf ( szBuff                                        ,
                    _T ( "%S\n%S\nLine #%d\nFile:%S\nModule:%S" ) ,
                    szErr                                         ,
                    szErrorVariableDesc                           ,
                    iLine                                         ,
                    szFile                                        ,
                    szModule                                       ) ;
    }
    else
    {
        // Build the string.
        _stprintf ( szBuff                                    ,
                    _T ( "%S\nLine #%d\nFile:%S\nModule:%S" ) ,
                    szErr                                     ,
                    iLine                                     ,
                    szFile                                    ,
                    szModule                                   ) ;
    }

    int iRes = TRUE ;
    if ( IDYES == MessageBox ( GetForegroundWindow ( )          ,
                               szBuff                           ,
                               _T ( "Run Time Check Failure" )  ,
                               MB_YESNO | MB_ICONQUESTION        ) )
    {
        // Returning 1 means DebugBreak will happen when this
        // function returns.
        iRes = 1 ;
    }
    else
    {
        iRes = 0 ;
    }

    // Pop out of the lock.
    LeaveCriticalSection ( &g_csRTCLock ) ;

    return ( iRes ) ;
}
#pragma runtime_checks("", restore)

#ifdef _UNICODE
void wmain ( void )
#else
void main ( void )
#endif
{
// __MSVC_RUNTIME_CHECKS is defined if /RTC* is used.
#ifdef __MSVC_RUNTIME_CHECKS
    _tprintf ( _T ( "Run time error checking is turned on!\n" ) ) ;
#endif

    // Initialize the critical section.
    InitializeCriticalSection ( &g_csRTCLock ) ;

    _RTC_error_fn oldfunction ;
    
    // Here's where the custom error handler gets set.  Comment out
    // this line and uncomment the lines below to see how to
    // redirect the standard warnings.
    oldfunction = _RTC_SetErrorFunc ( &HandleRTCFailure ) ;
    
    // Uncomment these line to have the errors show to STDERR when
    // running outside the debugger.
    //_CrtSetReportMode ( _CRT_ASSERT , _CRTDBG_MODE_FILE ) ;
    //_CrtSetReportFile ( _CRT_ASSERT , _CRTDBG_FILE_STDERR );

    // Do some errors.
    DoLocalStackOverflow ( ) ;
    DoTruncateValue ( ) ;
    DoUninitialized ( 1 ) ;
    DoUninitialized ( 0 ) ;
    
    DeleteCriticalSection ( &g_csRTCLock ) ;
}
