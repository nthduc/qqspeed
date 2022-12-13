/*----------------------------------------------------------------------
Debugging Microsoft .NET and Windows Applications
By John Robbins

Examples of Expression Evaluator Add Ins to display advanced type
expansion in the native debugging Watch window.  See Chapter 7 for the
whole truth and nothing but the truth about EEAddIns!

----------------------------------------------------------------------*/
#include "stdafx.h"
#include "CustView.h"

/*----------------------------------------------------------------------
Add the following to you AUTOEXP.DAT.
_SYSTEMTIME=$ADDIN(<PATH>\BSU_ExpEval_AddIn.dll,AddIn_SYSTEMTIME)
_FILETIME=$ADDIN(<PATH>\BSU_ExpEval_AddIn.dll,AddIn_FILETIME)
_OSVERSIONINFOA=$ADDIN(<PATH>\BSU_ExpEval_AddIn.dll,AddIn_OSVERSIONINFOA)
_OSVERSIONINFOW=$ADDIN(<PATH>\BSU_ExpEval_AddIn.dll,AddIn_OSVERSIONINFOW)
_OSVERSIONINFOEXA=$ADDIN(<PATH>\BSU_ExpEval_AddIn.dll,AddIn_OSVERSIONINFOEXA)
_OSVERSIONINFOEXW=$ADDIN(<PATH>\BSU_ExpEval_AddIn.dll,AddIn_OSVERSIONINFOEXW)
----------------------------------------------------------------------*/

// Proper prototypes for all exported functions.
#define ADDIN_API __declspec(dllexport)

extern "C" {
ADDIN_API HRESULT WINAPI AddIn_SYSTEMTIME ( DWORD         dwAddress   ,
                                            DEBUGHELPER * pHelper     ,
                                            int           nBase       ,
                                            BOOL          bUniStrings ,
                                            char *        pResult     ,
                                            size_t        max         ,
                                            DWORD         reserved    );
ADDIN_API HRESULT WINAPI AddIn_FILETIME ( DWORD         dwAddress   ,
                                          DEBUGHELPER * pHelper     ,
                                          int           nBase       ,
                                          BOOL          bUniStrings ,
                                          char *        pResult     ,
                                          size_t        max         ,
                                          DWORD         reserved     ) ;

ADDIN_API HRESULT WINAPI
        AddIn_OSVERSIONINFOA ( DWORD         dwAddress   ,
                               DEBUGHELPER * pHelper     ,
                               int           nBase       ,
                               BOOL          bUniStrings ,
                               char *        pResult     ,
                               size_t        max         ,
                               DWORD         reserved     ) ;

ADDIN_API HRESULT WINAPI
        AddIn_OSVERSIONINFOW ( DWORD         dwAddress   ,
                               DEBUGHELPER * pHelper     ,
                               int           nBase       ,
                               BOOL          bUniStrings ,
                               char *        pResult     ,
                               size_t        max         ,
                               DWORD         reserved     ) ;

ADDIN_API HRESULT WINAPI
        AddIn_OSVERSIONINFOEXA ( DWORD         dwAddress   ,
                                 DEBUGHELPER * pHelper     ,
                                 int           nBase       ,
                                 BOOL          bUniStrings ,
                                 char *        pResult     ,
                                 size_t        max         ,
                                 DWORD         reserved     ) ;

ADDIN_API HRESULT WINAPI
        AddIn_OSVERSIONINFOEXW ( DWORD         dwAddress   ,
                                 DEBUGHELPER * pHelper     ,
                                 int           nBase       ,
                                 BOOL          bUniStrings ,
                                 char *        pResult     ,
                                 size_t        max         ,
                                 DWORD         reserved     ) ;
}


BOOL WINAPI DllMain ( HINSTANCE hInst       ,
                      DWORD     dwReason    ,
                      LPVOID    /*lpReserved*/ )
{
    BOOL bRet = TRUE ;
    switch ( dwReason )
    {
    case DLL_PROCESS_ATTACH :
        // I don't need the thread notifications.
        DisableThreadLibraryCalls ( hInst ) ;
        break ;
    case DLL_PROCESS_DETACH :
        break ;
    default                 :
        break ;
    }
    return( bRet ) ;
}

// The following three functions I lifted from the EEAddIn sample simply
// because I wanted to keep everything together.  Additionally, I wanted
// to provide error handling!


// Helper routine that converts a system time into ASCII
static HRESULT FormatDateTime ( SYSTEMTIME * pSystemTime ,
                                char *       pResult     ,
                                size_t       max          )
{
    GetDateFormat( GetThreadLocale ( ) ,
                   DATE_SHORTDATE      ,
                   pSystemTime         ,
                   NULL                ,
                   pResult             ,
                   (int)max             ) ;

    size_t len = _tcslen ( pResult ) ;

    if ( (max-len) < 2 )
    {
        // if not enough room in buffer
        return ( E_FAIL ) ;
    }
    pResult[ len ] = ' ' ;
    len++ ;
    GetTimeFormat( GetThreadLocale ( ) ,
                   TIME_NOSECONDS      ,
                   pSystemTime         ,
                   NULL                ,
                   pResult + len       ,
                   (int)(max - len)     ) ;
    return ( S_OK ) ;
}

ADDIN_API HRESULT WINAPI AddIn_SYSTEMTIME ( DWORD         /*dwAddress*/,
                                            DEBUGHELPER*  pHelper      ,
                                            int           /*nBase*/    ,
                                            BOOL        /*bUniStrings*/,
                                            char *        pResult      ,
                                            size_t        max          ,
                                            DWORD         /*reserved*/ )
{
    if ( pHelper->dwVersion < 0x20000 )
    {
        // I'm not touching less than VS.NET.
        return ( E_FAIL ) ;
    }

    HRESULT hr = S_OK ;

    __try
    {
        SYSTEMTIME SysTime ;
        DWORD      nGot = 0 ;
        DWORDLONG  dwRealAddr = pHelper->GetRealAddress ( pHelper ) ;

        // Always put the "..." in.
        _tcscpy ( pResult , _T ( "..." ) ) ;

        // read system time from debuggee memory space
        if ( S_OK ==
                pHelper->ReadDebuggeeMemoryEx ( pHelper               ,
                                                dwRealAddr            ,
                                                sizeof ( SYSTEMTIME ) ,
                                                &SysTime              ,
                                                &nGot                 ))
        {
            if ( nGot == sizeof ( SYSTEMTIME ) )
            {
                hr = FormatDateTime ( &SysTime, pResult, max ) ;
            }
        }
    }
    __except ( EXCEPTION_EXECUTE_HANDLER )
    {
        hr = E_FAIL ;
    }
    return ( hr ) ;
}

ADDIN_API HRESULT WINAPI AddIn_FILETIME ( DWORD        /*dwAddress*/   ,
                                          DEBUGHELPER* pHelper         ,
                                          int          /*nBase*/       ,
                                          BOOL         /*bUniStrings*/ ,
                                          char *       pResult         ,
                                          size_t       max             ,
                                          DWORD        /*reserved*/    )
{
    if ( pHelper->dwVersion < 0x20000 )
    {
        // I'm not touching less than VS.NET.
        return ( E_FAIL ) ;
    }

    HRESULT hr = S_OK ;

    __try
    {
        FILETIME   FileTime ;
        SYSTEMTIME SysTime ;
        DWORD      nGot = 0 ;
        DWORDLONG  dwRealAddr = pHelper->GetRealAddress ( pHelper ) ;

        // Always put the "..." in.
        _tcscpy ( pResult , _T ( "..." ) ) ;

        // Read file time from debuggee memory space.
        if ( S_OK ==
                pHelper->ReadDebuggeeMemoryEx ( pHelper             ,
                                                dwRealAddr          ,
                                                sizeof ( FILETIME ) ,
                                                &FileTime           ,
                                                &nGot                ) )
        {
            if ( nGot == sizeof ( FILETIME ) )
            {
                // convert to SystemTime
                if ( TRUE == FileTimeToSystemTime ( &FileTime ,
                                                    &SysTime   ) )
                {
                    hr = FormatDateTime ( &SysTime, pResult, max ) ;
                }
            }
        }
    }
    __except ( EXCEPTION_EXECUTE_HANDLER )
    {
        hr = E_FAIL ;
    }
    return ( hr ) ;
}

// This only touched the first 5 DWORDS in the structs so you can pass
// both the ANSI and UNICODE versions in.
static int ConvertBaseOSV ( LPOSVERSIONINFOA pOSVA , char * szStr )
{
    int iCurrPos = 0 ;

    if ( ( pOSVA->dwMajorVersion == 4 ) && ( pOSVA->dwMinorVersion ==0))
    {
        if ( pOSVA->dwPlatformId == VER_PLATFORM_WIN32_NT )
        {
            iCurrPos = wsprintf ( szStr , _T ( "Windows NT 4.0 " ) ) ;
        }
        else
        {
            iCurrPos = wsprintf ( szStr , _T ( "Windows 95 " ) ) ;
        }
    }
    else if ( ( pOSVA->dwMajorVersion == 4  ) &&
              ( pOSVA->dwMinorVersion == 10 )   )
    {
        iCurrPos = wsprintf ( szStr , _T ( "Windows 98 " ) ) ;
    }
    else if ( ( pOSVA->dwMajorVersion == 4  ) &&
              ( pOSVA->dwMinorVersion == 90 )   )
    {
        iCurrPos = wsprintf ( szStr , _T ( "Windows Me " ) ) ;
    }
    else if ( ( pOSVA->dwMajorVersion == 5  ) &&
              ( pOSVA->dwMinorVersion == 0  )   )
    {
        iCurrPos = wsprintf ( szStr , _T ( "Windows 2000 " ) ) ;
    }
    else if ( ( pOSVA->dwMajorVersion == 5  ) &&
              ( pOSVA->dwMinorVersion == 1  )   )
    {
        iCurrPos = wsprintf ( szStr , _T ( "Windows XP " ) ) ;
    }
    else if ( ( pOSVA->dwMajorVersion == 5  ) &&
              ( pOSVA->dwMinorVersion == 2  )   )
    {
        iCurrPos = wsprintf ( szStr , _T ( "Windows Server 2003 " ) ) ;
    }
    else
    {
        // Beats me!
        iCurrPos = 0 ;
    }
    return ( iCurrPos ) ;
}

// Again, this function uses the shared field between the A and W
// versions so you can use it for both.
static int ConvertBuildNumber ( LPOSVERSIONINFOA pOSVA , char * szStr )
{
    int iCurrPos = 0 ;
    if ( VER_PLATFORM_WIN32_NT == pOSVA->dwPlatformId )
    {
        iCurrPos = wsprintf ( szStr                ,
                              _T ( "(%d) " )       ,
                              pOSVA->dwBuildNumber  ) ;
    }
    else if ( VER_PLATFORM_WIN32_WINDOWS == pOSVA->dwPlatformId )
    {
        WORD wBuild = LOWORD ( pOSVA->dwBuildNumber ) ;
        iCurrPos = wsprintf ( szStr , _T ( "(%d) " ) , wBuild ) ;
    }
    return ( iCurrPos ) ;
}

ADDIN_API HRESULT WINAPI
        AddIn_OSVERSIONINFOA ( DWORD        /*dwAddress*/   ,
                               DEBUGHELPER* pHelper         ,
                               int          /*nBase*/       ,
                               BOOL         /*bUniStrings*/ ,
                               char *       pResult         ,
                               size_t       /*max*/         ,
                               DWORD        /*reserved*/     )
{
    if ( pHelper->dwVersion < 0x20000 )
    {
        // I'm not touching less than VS.NET.
        return ( E_FAIL ) ;
    }

    HRESULT hRet = E_FAIL ;

    __try
    {

        DWORDLONG      dwRealAddr = pHelper->GetRealAddress ( pHelper );
        DWORD          nGot       = 0 ;
        OSVERSIONINFOA stOSA ;


        // Try and read in the structure.
        if ( S_OK ==
                pHelper->
                    ReadDebuggeeMemoryEx ( pHelper                   ,
                                           dwRealAddr                ,
                                           sizeof ( OSVERSIONINFOA ) ,
                                           &stOSA                    ,
                                           &nGot                      ))
        {

            // Make sure I got all of it.
            if ( nGot == sizeof ( OSVERSIONINFOA ) )
            {

                // Do the dance...
                char * pCurr = pResult ;
                int iCurr = ConvertBaseOSV ( &stOSA , pCurr ) ;
                if ( 0 != iCurr )
                {
                    pCurr += iCurr ;

                    iCurr = ConvertBuildNumber ( &stOSA , pCurr ) ;

                    pCurr += iCurr ;
                    if ( '\0' != stOSA.szCSDVersion[0] )
                    {
                        wsprintf ( pCurr                ,
                                   _T ( "%s" )          ,
                                   stOSA.szCSDVersion    ) ;
                    }
                }
                else
                {
                    _tcscpy ( pResult , _T ( "..." ) ) ;
                }
            }
            hRet = S_OK ;
        }
    }
    __except ( EXCEPTION_EXECUTE_HANDLER )
    {
        hRet = E_FAIL ;
    }
    return ( hRet ) ;
}

ADDIN_API HRESULT WINAPI
        AddIn_OSVERSIONINFOW ( DWORD        /*dwAddress*/   ,
                               DEBUGHELPER* pHelper         ,
                               int          /*nBase*/       ,
                               BOOL         /*bUniStrings*/ ,
                               char *       pResult         ,
                               size_t       /*max*/         ,
                               DWORD        /*reserved*/     )
{
    if ( pHelper->dwVersion < 0x20000 )
    {
        // I'm not touching less than VS.NET.
        return ( E_FAIL ) ;
    }

    HRESULT hRet = E_FAIL ;

    __try
    {

        DWORDLONG      dwRealAddr = pHelper->GetRealAddress ( pHelper );
        DWORD          nGot       = 0 ;
        OSVERSIONINFOW stOSW ;


        // Try and read in the structure.
        if ( S_OK ==
                pHelper->
                    ReadDebuggeeMemoryEx ( pHelper                   ,
                                           dwRealAddr                ,
                                           sizeof ( OSVERSIONINFOW ) ,
                                           &stOSW                    ,
                                           &nGot                      ))
        {

            // Make sure I got all of it.
            if ( nGot == sizeof ( OSVERSIONINFOW ) )
            {

                // Do the dance...
                char * pCurr = pResult ;
                int iCurr = ConvertBaseOSV ( (LPOSVERSIONINFOA)&stOSW ,
                                             pCurr                    );
                if ( 0 != iCurr )
                {
                    pCurr += iCurr ;

                    iCurr =
                        ConvertBuildNumber ( (LPOSVERSIONINFOA)&stOSW ,
                                             pCurr                    );

                    pCurr += iCurr ;
                    if ( L'\0' != stOSW.szCSDVersion[0] )
                    {
                        wsprintf ( pCurr                ,
                                   _T ( "%S" )          ,
                                   stOSW.szCSDVersion    ) ;
                    }
                }
                else
                {
                    _tcscpy ( pResult , _T ( "..." ) ) ;
                }
            }
            hRet = S_OK ;
        }
    }
    __except ( EXCEPTION_EXECUTE_HANDLER )
    {
        hRet = E_FAIL ;
    }
    return ( hRet ) ;
}

static int ConvertOSVEx ( DWORD  dwMinorVersion ,
                          WORD   wProductType   ,
                          WORD   wSuiteMask     ,
                          char * szStr           )
{
    int iCurrPos = 0 ;
    char * pCurr = szStr ;

    // Is it W2K
    if ( 0 == dwMinorVersion )
    {
        iCurrPos += wsprintf ( pCurr , _T ( "Windows 2000 " ) ) ;

        pCurr = szStr + iCurrPos ;

        if ( VER_NT_WORKSTATION == wProductType )
        {
            iCurrPos += wsprintf ( pCurr , _T ( "Professional " ) ) ;

            pCurr = szStr + iCurrPos ;
        }
        else if ( VER_NT_SERVER == wProductType )
        {
            if ( VER_SUITE_DATACENTER ==
                        (  VER_SUITE_DATACENTER & wSuiteMask ) )
            {
                iCurrPos += wsprintf ( pCurr ,_T("Datacenter Server "));
                pCurr = szStr + iCurrPos ;
            }
            else if ( VER_SUITE_ENTERPRISE ==
                        ( VER_SUITE_ENTERPRISE & wSuiteMask ) )
            {
                iCurrPos += wsprintf ( pCurr , _T ("Advanced Server "));
                pCurr = szStr + iCurrPos ;
            }
            else
            {
                iCurrPos += wsprintf ( pCurr , _T ( "Server " ) ) ;
                pCurr = szStr + iCurrPos ;
            }
        }
        else
        {
            iCurrPos = 0 ;
        }
    }
    // Is it WinXP? or .NET Server?
    else if ( 1 == dwMinorVersion )
    {
        if ( VER_NT_WORKSTATION == wProductType )
        {
            iCurrPos += wsprintf ( pCurr , _T ( "Windows XP " ) ) ;
            pCurr = szStr + iCurrPos ;

            if ( VER_SUITE_PERSONAL ==
                        ( wSuiteMask & VER_SUITE_PERSONAL ) )
            {
                iCurrPos += wsprintf ( pCurr , _T ( "Home Edition " ) );
                pCurr = szStr + iCurrPos ;
            }
            else 
            {
                iCurrPos += wsprintf ( pCurr , _T ( "Professional " ) );
                pCurr = szStr + iCurrPos ;
            }
        }
	}
	else if ( 2 == dwMinorVersion )
	{
        iCurrPos += wsprintf ( pCurr , _T ( "Windows Server 2003 " ) );
        pCurr = szStr + iCurrPos ;

        if ( VER_SUITE_DATACENTER ==
                    (  VER_SUITE_DATACENTER & wSuiteMask ) )
        {
            iCurrPos += wsprintf ( pCurr ,_T("Datacenter Edition "));
            pCurr = szStr + iCurrPos ;
        }
        else if ( VER_SUITE_ENTERPRISE ==
                    ( VER_SUITE_ENTERPRISE & wSuiteMask ) )
        {
            iCurrPos += wsprintf ( pCurr ,_T("Enterprise Edition "));
            pCurr = szStr + iCurrPos ;
        }
        else if ( VER_SUITE_BLADE ==
                    ( VER_SUITE_BLADE & wSuiteMask ) )
        {
            iCurrPos += wsprintf ( pCurr , _T ( "Web Edition " ) ) ;
            pCurr = szStr + iCurrPos ;
        }
        else
        {
            iCurrPos += wsprintf ( pCurr , _T ( "Standard Edition " ) );
            pCurr = szStr + iCurrPos ;
        }
    }
    else
    {
        // Beats me!
        iCurrPos = 0 ;
    }
    return ( iCurrPos ) ;
}


ADDIN_API HRESULT WINAPI
        AddIn_OSVERSIONINFOEXA ( DWORD        /*dwAddress*/   ,
                                 DEBUGHELPER* pHelper         ,
                                 int          /*nBase*/       ,
                                 BOOL         /*bUniStrings*/ ,
                                 char *       pResult         ,
                                 size_t       /*max*/         ,
                                 DWORD        /*reserved*/     )
{
    if ( pHelper->dwVersion < 0x20000 )
    {
        // I'm not touching less than VS.NET.
        return ( E_FAIL ) ;
    }

    HRESULT hRet = E_FAIL ;

    __try
    {

        DWORDLONG      dwRealAddr = pHelper->GetRealAddress ( pHelper );
        DWORD          nGot       = 0 ;
        OSVERSIONINFOEXA stOSA ;


        // Try and read in the structure.
        if ( S_OK ==
                pHelper->
                    ReadDebuggeeMemoryEx ( pHelper                     ,
                                           dwRealAddr                  ,
                                           sizeof ( OSVERSIONINFOEXA ) ,
                                           &stOSA                      ,
                                           &nGot                      ))
        {

            // Make sure I got all of it.
            if ( nGot == sizeof ( OSVERSIONINFOEXA ) )
            {

                // Do the dance...
                char * pCurr = pResult ;
                int iCurr = 0 ;
                // Check if it's XP or W2K based.
                if ( 5 == stOSA.dwMajorVersion )
                {
                    iCurr = ConvertOSVEx ( stOSA.dwMinorVersion ,
                                           stOSA.wProductType   ,
                                           stOSA.wSuiteMask     ,
                                           pResult               ) ;
                }
                else
                {
                    iCurr = ConvertBaseOSV ( (LPOSVERSIONINFOA)&stOSA ,
                                             pCurr                   );
                }
                if ( 0 != iCurr )
                {
                    pCurr += iCurr ;

                    iCurr = ConvertBuildNumber((LPOSVERSIONINFOA)&stOSA,
                                                pCurr                 );

                    pCurr += iCurr ;
                    if ( '\0' != stOSA.szCSDVersion[0] )
                    {
                        wsprintf ( pCurr                ,
                                   _T ( "%s" )          ,
                                   stOSA.szCSDVersion    ) ;
                    }
                }
                else
                {
                    _tcscpy ( pResult , _T ( "..." ) ) ;
                }
            }
            hRet = S_OK ;
        }
    }
    __except ( EXCEPTION_EXECUTE_HANDLER )
    {
        hRet = E_FAIL ;
    }
    return ( hRet ) ;
}

ADDIN_API HRESULT WINAPI
        AddIn_OSVERSIONINFOEXW ( DWORD        /*dwAddress*/   ,
                                 DEBUGHELPER* pHelper         ,
                                 int          /*nBase*/       ,
                                 BOOL         /*bUniStrings*/ ,
                                 char *       pResult         ,
                                 size_t       /*max*/         ,
                                 DWORD        /*reserved*/     )
{
    if ( pHelper->dwVersion < 0x20000 )
    {
        // I'm not touching less than VS.NET.
        return ( E_FAIL ) ;
    }

    HRESULT hRet = E_FAIL ;

    __try
    {

        DWORDLONG      dwRealAddr = pHelper->GetRealAddress ( pHelper );
        DWORD          nGot       = 0 ;
        OSVERSIONINFOEXW stOSW ;


        // Try and read in the structure.
        if ( S_OK ==
                pHelper->
                    ReadDebuggeeMemoryEx ( pHelper                     ,
                                           dwRealAddr                  ,
                                           sizeof ( OSVERSIONINFOEXW ) ,
                                           &stOSW                      ,
                                           &nGot                      ))
        {

            // Make sure I got all of it.
            if ( nGot == sizeof ( OSVERSIONINFOEXW ) )
            {

                // Do the dance...
                char * pCurr = pResult ;
                int iCurr = 0 ;
                // Check if it's XP or W2K based.
                if ( 5 == stOSW.dwMajorVersion )
                {
                    iCurr = ConvertOSVEx ( stOSW.dwMinorVersion ,
                                           stOSW.wProductType   ,
                                           stOSW.wSuiteMask     ,
                                           pResult               ) ;
                }
                else
                {
                    iCurr = ConvertBaseOSV ( (LPOSVERSIONINFOA)&stOSW ,
                                             pCurr                    );
                }
                if ( 0 != iCurr )
                {
                    pCurr += iCurr ;

                    iCurr = ConvertBuildNumber((LPOSVERSIONINFOA)&stOSW,
                                                pCurr                 );

                    pCurr += iCurr ;
                    if ( '\0' != stOSW.szCSDVersion[0] )
                    {
                        wsprintf ( pCurr                ,
                                   _T ( "%S" )          ,
                                   stOSW.szCSDVersion    ) ;
                    }
                }
                else
                {
                    _tcscpy ( pResult , _T ( "..." ) ) ;
                }
            }
            hRet = S_OK ;
        }
    }
    __except ( EXCEPTION_EXECUTE_HANDLER )
    {
        hRet = E_FAIL ;
    }
    return ( hRet ) ;
}

