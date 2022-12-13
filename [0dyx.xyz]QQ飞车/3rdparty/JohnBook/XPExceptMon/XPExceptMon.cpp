/*----------------------------------------------------------------------
Debugging Applications for Microsoft .NET and Microsoft Windows
Copyright © 1997-2003 John Robbins -- All rights reserved.
----------------------------------------------------------------------*/
#include "stdafx.h"

/*//////////////////////////////////////////////////////////////////////
// Typedefs and Constants
//////////////////////////////////////////////////////////////////////*/
typedef PVOID (WINAPI *ADDVECTOREDEXCEPTIONHANDLER)
                                ( ULONG                       ,
                                  PVECTORED_EXCEPTION_HANDLER  );
const char * k_ADDVECTOREDEXCEPTIONHANDLER =
                                         "AddVectoredExceptionHandler" ;

typedef ULONG (WINAPI *REMOVEVECTOREDEXCEPTIONHANDLER) ( PVOID ) ;
const char * k_REMOVEVECTOREDEXCEPTIONHANDLER =
                                      "RemoveVectoredExceptionHandler" ;
                                      
const TCHAR * k_LOGFILEEXT = _T ( ".XPEXCEPTLog" ) ;

#ifdef _WIN64
const TCHAR * k_ADDRFMT = _T ( "0x%016I64X" ) ;
const TCHAR * k_REASONANDADDRFMT = _T ( "%s at address 0x%016I64X\n" ) ;
#else
const TCHAR * k_ADDRFMT = _T ( "0x%08X" ) ;
const TCHAR * k_REASONANDADDRFMT = _T ( "%s at address 0x%08X\n" ) ;
#endif

/*//////////////////////////////////////////////////////////////////////
// Function Prototypes
//////////////////////////////////////////////////////////////////////*/
// Sets up everything.
void SetupExceptionVectorStuff ( void ) ;
// Tears down everything.
void RemoveExceptionVectorStuff ( void ) ;

// Gets the vector EH function pointers.
BOOL GetFunctions ( void ) ;
// Opens the file.
BOOL OpenFile ( void ) ;

// Returns the string for an exception.
LPCTSTR ConvertSimpleException ( DWORD dwExcept ) ;

// The vectored exception itself.
LONG WINAPI VectoredExceptionLogger ( PEXCEPTION_POINTERS pExpt ) ;


/*//////////////////////////////////////////////////////////////////////
// File Global Variables
//////////////////////////////////////////////////////////////////////*/
// The add function.
static ADDVECTOREDEXCEPTIONHANDLER g_AddVectorEH = NULL ;
// The remove function.
static REMOVEVECTOREDEXCEPTIONHANDLER g_RemoveVectorEH = NULL ;
// The file to write to.
static FILE * g_pFile = NULL ;
// The critical section to protect everything.
static CRITICAL_SECTION g_CS ;
// Indicates the critical section is live.
static BOOL g_bCritSecLive = FALSE ;
// Indicates the exception hook was set.
static BOOL g_bHookSet = FALSE ;


/*//////////////////////////////////////////////////////////////////////
// The one entry point to the code!
//////////////////////////////////////////////////////////////////////*/
BOOL APIENTRY DllMain ( HMODULE hModule        ,
                        DWORD   dwReason       ,
                        LPVOID  /*lpReserved*/  )
{
    if ( DLL_PROCESS_ATTACH == dwReason )
    {
        DisableThreadLibraryCalls ( hModule ) ;
        SetupExceptionVectorStuff ( ) ;
    }
    else if ( DLL_PROCESS_DETACH == dwReason )
    {
        RemoveExceptionVectorStuff ( ) ;
    }
    return ( TRUE ) ;
}

void SetupExceptionVectorStuff ( void )
{
    BOOL bRet = GetFunctions ( ) ;
    ASSERT ( TRUE == bRet ) ;
    if ( TRUE == bRet )
    {
        bRet = InitializeCriticalSectionAndSpinCount ( &g_CS , 4000 ) ;
        ASSERT ( TRUE == bRet ) ;
        if ( TRUE == bRet )
        {
            g_bCritSecLive = TRUE ;
            
            bRet = OpenFile ( ) ;
            ASSERT ( TRUE == bRet ) ;
            if ( TRUE == bRet )
            {
                PVOID pTemp = g_AddVectorEH ( TRUE                    ,
                                             &VectoredExceptionLogger );
                ASSERT ( NULL != pTemp ) ;
                if ( NULL != pTemp )
                {
                    g_bHookSet = TRUE ;
                }
            }
        }
    }
}

void RemoveExceptionVectorStuff ( void )
{
    if ( ( TRUE == g_bHookSet ) && ( NULL != g_RemoveVectorEH ) )
    {
        g_RemoveVectorEH ( &VectoredExceptionLogger ) ;
        g_bHookSet = FALSE ;
        g_RemoveVectorEH = NULL ;
        g_AddVectorEH = NULL ;
    }
    if ( NULL != g_pFile )
    {
        fclose ( g_pFile ) ;
        g_pFile = NULL ;
    }
    if ( TRUE == g_bCritSecLive )
    {
        DeleteCriticalSection ( &g_CS ) ;
        g_bCritSecLive = FALSE ;
    }
}

BOOL GetFunctions ( void )
{
    BOOL bRet = FALSE ;
    HMODULE hKern = GetModuleHandle ( _T ( "KERNEL32.DLL" ) ) ;
    ASSERT ( NULL != hKern ) ;
    if ( NULL != hKern )
    {
        g_AddVectorEH = (ADDVECTOREDEXCEPTIONHANDLER)
                        GetProcAddress ( hKern ,
                                         k_ADDVECTOREDEXCEPTIONHANDLER);
        ASSERT ( NULL != g_AddVectorEH ) ;
        if ( NULL != g_AddVectorEH )
        {
            g_RemoveVectorEH = (REMOVEVECTOREDEXCEPTIONHANDLER)
                                GetProcAddress ( hKern ,
                                     k_REMOVEVECTOREDEXCEPTIONHANDLER );
            ASSERT ( NULL != g_RemoveVectorEH ) ;
            if ( NULL != g_RemoveVectorEH )
            {
                bRet = TRUE ;
            }
        }
    }
    return ( bRet ) ;
}

BOOL OpenFile ( void )
{
    BOOL bRet = FALSE ;
    TCHAR szName[ MAX_PATH ] ;
    
    DWORD dwLen = GetModuleFileName ( NULL , szName , MAX_PATH ) ;
    ASSERT ( dwLen > 0 ) ;
    if ( dwLen > 0 )
    {
        TCHAR * pPeriod = _tcsrchr ( szName , _T ( '.' ) ) ;
        if ( NULL != pPeriod )
        {
            _tcscpy ( pPeriod , k_LOGFILEEXT ) ;
        }
        else
        {
            _tcscat ( szName , k_LOGFILEEXT ) ;
        }
        
        g_pFile = _tfopen ( szName , _T ( "wt" ) ) ;
        ASSERT ( NULL != g_pFile ) ;
        
        bRet = ( g_pFile != NULL ) ;
    }
    else
    {
        bRet = FALSE ;
    }
    return ( bRet ) ;
}

LONG WINAPI VectoredExceptionLogger ( PEXCEPTION_POINTERS pExpt )
{
    EnterCriticalSection ( &g_CS ) ;
    
    ASSERT ( NULL != g_pFile ) ;
    
    TCHAR szBuff[ 100 ] ;
    LPCTSTR pExceptString = NULL ;
    
    pExceptString = ConvertSimpleException ( pExpt->
                                                ExceptionRecord->
                                                       ExceptionCode ) ;
    if ( NULL == pExceptString )
    {
        wsprintf ( szBuff                                ,
                   k_ADDRFMT                             ,
                   pExpt->ExceptionRecord->ExceptionCode  ) ;
        pExceptString = szBuff ;
    }
    
    _ftprintf ( g_pFile                                  ,
                k_REASONANDADDRFMT                       ,
                pExceptString                            ,
                pExpt->ExceptionRecord->ExceptionAddress  ) ;
    
    LeaveCriticalSection ( &g_CS ) ;
    
    return ( EXCEPTION_CONTINUE_SEARCH ) ;
}

LPCTSTR ConvertSimpleException ( DWORD dwExcept )
{
    switch ( dwExcept )
    {
        case EXCEPTION_ACCESS_VIOLATION         :
            return ( _T ( "EXCEPTION_ACCESS_VIOLATION" ) ) ;
        break ;

        case EXCEPTION_DATATYPE_MISALIGNMENT    :
            return ( _T ( "EXCEPTION_DATATYPE_MISALIGNMENT" ) ) ;
        break ;

        case EXCEPTION_BREAKPOINT               :
            return ( _T ( "EXCEPTION_BREAKPOINT" ) ) ;
        break ;

        case EXCEPTION_SINGLE_STEP              :
            return ( _T ( "EXCEPTION_SINGLE_STEP" ) ) ;
        break ;

        case EXCEPTION_ARRAY_BOUNDS_EXCEEDED    :
            return ( _T ( "EXCEPTION_ARRAY_BOUNDS_EXCEEDED" ) ) ;
        break ;

        case EXCEPTION_FLT_DENORMAL_OPERAND     :
            return ( _T ( "EXCEPTION_FLT_DENORMAL_OPERAND" ) ) ;
        break ;

        case EXCEPTION_FLT_DIVIDE_BY_ZERO       :
            return ( _T ( "EXCEPTION_FLT_DIVIDE_BY_ZERO" ) ) ;
        break ;

        case EXCEPTION_FLT_INEXACT_RESULT       :
            return ( _T ( "EXCEPTION_FLT_INEXACT_RESULT" ) ) ;
        break ;

        case EXCEPTION_FLT_INVALID_OPERATION    :
            return ( _T ( "EXCEPTION_FLT_INVALID_OPERATION" ) ) ;
        break ;

        case EXCEPTION_FLT_OVERFLOW             :
            return ( _T ( "EXCEPTION_FLT_OVERFLOW" ) ) ;
        break ;

        case EXCEPTION_FLT_STACK_CHECK          :
            return ( _T ( "EXCEPTION_FLT_STACK_CHECK" ) ) ;
        break ;

        case EXCEPTION_FLT_UNDERFLOW            :
            return ( _T ( "EXCEPTION_FLT_UNDERFLOW" ) ) ;
        break ;

        case EXCEPTION_INT_DIVIDE_BY_ZERO       :
            return ( _T ( "EXCEPTION_INT_DIVIDE_BY_ZERO" ) ) ;
        break ;

        case EXCEPTION_INT_OVERFLOW             :
            return ( _T ( "EXCEPTION_INT_OVERFLOW" ) ) ;
        break ;

        case EXCEPTION_PRIV_INSTRUCTION         :
            return ( _T ( "EXCEPTION_PRIV_INSTRUCTION" ) ) ;
        break ;

        case EXCEPTION_IN_PAGE_ERROR            :
            return ( _T ( "EXCEPTION_IN_PAGE_ERROR" ) ) ;
        break ;

        case EXCEPTION_ILLEGAL_INSTRUCTION      :
            return ( _T ( "EXCEPTION_ILLEGAL_INSTRUCTION" ) ) ;
        break ;

        case EXCEPTION_NONCONTINUABLE_EXCEPTION :
            return ( _T ( "EXCEPTION_NONCONTINUABLE_EXCEPTION" ) ) ;
        break ;

        case EXCEPTION_STACK_OVERFLOW           :
            return ( _T ( "EXCEPTION_STACK_OVERFLOW" ) ) ;
        break ;

        case EXCEPTION_INVALID_DISPOSITION      :
            return ( _T ( "EXCEPTION_INVALID_DISPOSITION" ) ) ;
        break ;

        case EXCEPTION_GUARD_PAGE               :
            return ( _T ( "EXCEPTION_GUARD_PAGE" ) ) ;
        break ;

        case EXCEPTION_INVALID_HANDLE           :
            return ( _T ( "EXCEPTION_INVALID_HANDLE" ) ) ;
        break ;

        case 0xE06D7363                         :
            return ( _T ( "Microsoft C++ Exception" ) ) ;
        break ;

        default :
            return ( NULL ) ;
        break ;
    }
}
