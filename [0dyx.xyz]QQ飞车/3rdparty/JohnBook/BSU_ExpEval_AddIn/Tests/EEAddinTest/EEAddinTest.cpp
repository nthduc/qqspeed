// EEAddinTest.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

void _tmain ( void )
{
    SYSTEMTIME stST ;
    
    GetSystemTime ( &stST ) ;
    
    LPSYSTEMTIME lpST = &stST ;
    
    lpST = lpST ;
    
    FILETIME stFT ;
    
    SystemTimeToFileTime ( &stST , &stFT ) ;
    
    OSVERSIONINFOA stOSA ;
    stOSA.dwOSVersionInfoSize = sizeof ( OSVERSIONINFOA ) ;
    GetVersionExA ( &stOSA ) ; 

    OSVERSIONINFOW stOSW ;
    stOSW.dwOSVersionInfoSize = sizeof ( OSVERSIONINFOW ) ;
    GetVersionExW ( &stOSW ) ;
    
    OSVERSIONINFOEXA stOSExA ; 
    stOSExA.dwOSVersionInfoSize = sizeof ( OSVERSIONINFOEXA ) ;
    GetVersionExA ( (LPOSVERSIONINFOA)&stOSExA ) ;

    OSVERSIONINFOEXW stOSExW ; 
    stOSExW.dwOSVersionInfoSize = sizeof ( OSVERSIONINFOEXW ) ;
    GetVersionExW ( (LPOSVERSIONINFOW)&stOSExW ) ;
    
    stOSExW = stOSExW ;
    
    // Win95.
    stOSA.dwMajorVersion = 4 ;
    stOSA.dwMinorVersion = 0 ;
    stOSA.dwBuildNumber = 100 ;
    stOSA.dwPlatformId = VER_PLATFORM_WIN32_WINDOWS ;
    strcpy ( stOSA.szCSDVersion , "C" ) ;
    
    stOSW.dwMajorVersion = 4 ;
    stOSW.dwMinorVersion = 0 ;
    stOSW.dwBuildNumber = 100 ;
    stOSW.dwPlatformId = VER_PLATFORM_WIN32_WINDOWS ;
    lstrcpyW ( stOSW.szCSDVersion , L"C" ) ;

    // Win98.
    stOSA.dwMajorVersion = 4 ;
    stOSA.dwMinorVersion = 10 ;
    stOSA.dwBuildNumber = 100 ;
    stOSA.dwPlatformId = VER_PLATFORM_WIN32_WINDOWS ;
    strcpy ( stOSA.szCSDVersion , "C" ) ;
    
    stOSW.dwMajorVersion = 4 ;
    stOSW.dwMinorVersion = 10 ;
    stOSW.dwBuildNumber = 100 ;
    stOSW.dwPlatformId = VER_PLATFORM_WIN32_WINDOWS ;
    lstrcpyW ( stOSW.szCSDVersion , L"C" ) ;
    
    // WinMe.
    stOSA.dwMajorVersion = 4 ;
    stOSA.dwMinorVersion = 90 ;
    stOSA.dwBuildNumber = 100 ;
    stOSA.dwPlatformId = VER_PLATFORM_WIN32_WINDOWS ;
    strcpy ( stOSA.szCSDVersion , "C" ) ;
    
    stOSW.dwMajorVersion = 4 ;
    stOSW.dwMinorVersion = 90 ;
    stOSW.dwBuildNumber = 100 ;
    stOSW.dwPlatformId = VER_PLATFORM_WIN32_WINDOWS ;
    lstrcpyW ( stOSW.szCSDVersion , L"C" ) ;
    
    // NT4.0
    stOSA.dwMajorVersion = 4 ;
    stOSA.dwMinorVersion = 0 ;
    stOSA.dwBuildNumber = 100 ;
    stOSA.dwPlatformId = VER_PLATFORM_WIN32_NT ;
    strcpy ( stOSA.szCSDVersion , "Service Pack 2" ) ;
    
    stOSW.dwMajorVersion = 4 ;
    stOSW.dwMinorVersion = 0 ;
    stOSW.dwBuildNumber = 100 ;
    stOSW.dwPlatformId = VER_PLATFORM_WIN32_NT ;
    lstrcpyW ( stOSW.szCSDVersion , L"Service Pack 2" ) ;
    
    // Windows 2000 Workstation.
    stOSExA.dwMajorVersion = 5 ;
    stOSExA.dwMinorVersion = 0 ;
    stOSExA.dwBuildNumber = 100 ;
    stOSExA.dwPlatformId = VER_PLATFORM_WIN32_NT ;
    stOSExA.wProductType = VER_NT_WORKSTATION ;
    strcpy ( stOSExA.szCSDVersion , "Service Pack 1" ) ;
    
    stOSExW.dwMajorVersion = 5 ;
    stOSExW.dwMinorVersion = 0 ;
    stOSExW.dwBuildNumber = 100 ;
    stOSExW.dwPlatformId = VER_PLATFORM_WIN32_NT ;
    stOSExW.wProductType = VER_NT_WORKSTATION ;
    lstrcpyW ( stOSExW.szCSDVersion , L"Service Pack 1" ) ;

    // Windows 2000 Server.
    stOSExA.dwMajorVersion = 5 ;
    stOSExA.dwMinorVersion = 0 ;
    stOSExA.dwBuildNumber = 100 ;
    stOSExA.dwPlatformId = VER_PLATFORM_WIN32_NT ;
    stOSExA.wProductType = VER_NT_SERVER ;
    strcpy ( stOSExA.szCSDVersion , "Service Pack 1" ) ;
    
    stOSExW.dwMajorVersion = 5 ;
    stOSExW.dwMinorVersion = 0 ;
    stOSExW.dwBuildNumber = 100 ;
    stOSExW.dwPlatformId = VER_PLATFORM_WIN32_NT ;
    stOSExW.wProductType = VER_NT_SERVER ;
    lstrcpyW ( stOSExW.szCSDVersion , L"Service Pack 1" ) ;

    // Windows 2000 Advanced Server.
    stOSExA.dwMajorVersion = 5 ;
    stOSExA.dwMinorVersion = 0 ;
    stOSExA.dwBuildNumber = 100 ;
    stOSExA.wSuiteMask = VER_SUITE_ENTERPRISE ;
    stOSExA.dwPlatformId = VER_PLATFORM_WIN32_NT ;
    stOSExA.wProductType = VER_NT_SERVER ;
    strcpy ( stOSExA.szCSDVersion , "Service Pack 1" ) ;
    
    stOSExW.dwMajorVersion = 5 ;
    stOSExW.dwMinorVersion = 0 ;
    stOSExW.dwBuildNumber = 100 ;
    stOSExW.wSuiteMask = VER_SUITE_ENTERPRISE ;
    stOSExW.dwPlatformId = VER_PLATFORM_WIN32_NT ;
    stOSExW.wProductType = VER_NT_SERVER ;
    lstrcpyW ( stOSExW.szCSDVersion , L"Service Pack 1" ) ;

    // Windows 2000 Datacenter Server.
    stOSExA.dwMajorVersion = 5 ;
    stOSExA.dwMinorVersion = 0 ;
    stOSExA.dwBuildNumber = 100 ;
    stOSExA.wSuiteMask = VER_SUITE_DATACENTER ;
    stOSExA.dwPlatformId = VER_PLATFORM_WIN32_NT ;
    stOSExA.wProductType = VER_NT_SERVER ;
    strcpy ( stOSExA.szCSDVersion , "Service Pack 1" ) ;
    
    stOSExW.dwMajorVersion = 5 ;
    stOSExW.dwMinorVersion = 0 ;
    stOSExW.dwBuildNumber = 100 ;
    stOSExW.wSuiteMask = VER_SUITE_DATACENTER ;
    stOSExW.dwPlatformId = VER_PLATFORM_WIN32_NT ;
    stOSExW.wProductType = VER_NT_SERVER ;
    lstrcpyW ( stOSExW.szCSDVersion , L"Service Pack 1" ) ;

    // Windows XP Home.
    stOSExA.dwMajorVersion = 5 ;
    stOSExA.dwMinorVersion = 1 ;
    stOSExA.dwBuildNumber = 100 ;
    stOSExA.wSuiteMask = VER_SUITE_PERSONAL  ;
    stOSExA.dwPlatformId = VER_PLATFORM_WIN32_NT ;
    stOSExA.wProductType = VER_NT_WORKSTATION ;
    strcpy ( stOSExA.szCSDVersion , "Service Pack 2" ) ;
    
    stOSExW.dwMajorVersion = 5 ;
    stOSExW.dwMinorVersion = 1 ;
    stOSExW.dwBuildNumber = 100 ;
    stOSExW.wSuiteMask = VER_SUITE_PERSONAL  ;
    stOSExW.dwPlatformId = VER_PLATFORM_WIN32_NT ;
    stOSExW.wProductType = VER_NT_WORKSTATION ;
    lstrcpyW ( stOSExW.szCSDVersion , L"Service Pack 2" ) ;

    // Windows XP Pro.
    stOSExA.dwMajorVersion = 5 ;
    stOSExA.dwMinorVersion = 1 ;
    stOSExA.dwBuildNumber = 100 ;
    stOSExA.wSuiteMask = 0  ;
    stOSExA.dwPlatformId = VER_PLATFORM_WIN32_NT ;
    stOSExA.wProductType = VER_NT_WORKSTATION ;
    strcpy ( stOSExA.szCSDVersion , "Service Pack 2" ) ;
    
    stOSExW.dwMajorVersion = 5 ;
    stOSExW.dwMinorVersion = 1 ;
    stOSExW.dwBuildNumber = 100 ;
    stOSExW.wSuiteMask = 0  ;
    stOSExW.dwPlatformId = VER_PLATFORM_WIN32_NT ;
    stOSExW.wProductType = VER_NT_WORKSTATION ;
    lstrcpyW ( stOSExW.szCSDVersion , L"Service Pack 2" ) ;

    // Windows Server 2003.
    stOSExA.dwMajorVersion = 5 ;
    stOSExA.dwMinorVersion = 2 ;
    stOSExA.dwBuildNumber = 100 ;
    stOSExA.wSuiteMask = 0  ;
    stOSExA.dwPlatformId = VER_PLATFORM_WIN32_NT ;
    stOSExA.wProductType = VER_NT_SERVER ;
    strcpy ( stOSExA.szCSDVersion , "Service Pack 2" ) ;
    
    stOSExW.dwMajorVersion = 5 ;
    stOSExW.dwMinorVersion = 2 ;
    stOSExW.dwBuildNumber = 100 ;
    stOSExW.wSuiteMask = 0  ;
    stOSExW.dwPlatformId = VER_PLATFORM_WIN32_NT ;
    stOSExW.wProductType = VER_NT_SERVER ;
    lstrcpyW ( stOSExW.szCSDVersion , L"Service Pack 2" ) ;

    // Windows Server 2003, Web Edition.
    stOSExA.dwMajorVersion = 5 ;
    stOSExA.dwMinorVersion = 2 ;
    stOSExA.dwBuildNumber = 100 ;
    stOSExA.wSuiteMask = VER_SUITE_BLADE  ;
    stOSExA.dwPlatformId = VER_PLATFORM_WIN32_NT ;
    stOSExA.wProductType = VER_NT_SERVER ;
    strcpy ( stOSExA.szCSDVersion , "Service Pack 2" ) ;
    
    stOSExW.dwMajorVersion = 5 ;
    stOSExW.dwMinorVersion = 2 ;
    stOSExW.dwBuildNumber = 100 ;
    stOSExW.wSuiteMask = VER_SUITE_BLADE  ;
    stOSExW.dwPlatformId = VER_PLATFORM_WIN32_NT ;
    stOSExW.wProductType = VER_NT_SERVER ;
    lstrcpyW ( stOSExW.szCSDVersion , L"Service Pack 2" ) ;

    // Windows 2003 Server, Enterprise Edition.
    stOSExA.dwMajorVersion = 5 ;
    stOSExA.dwMinorVersion = 2 ;
    stOSExA.dwBuildNumber = 100 ;
    stOSExA.wSuiteMask = VER_SUITE_ENTERPRISE  ;
    stOSExA.dwPlatformId = VER_PLATFORM_WIN32_NT ;
    stOSExA.wProductType = VER_NT_SERVER ;
    strcpy ( stOSExA.szCSDVersion , "Service Pack 2" ) ;
    
    stOSExW.dwMajorVersion = 5 ;
    stOSExW.dwMinorVersion = 2 ;
    stOSExW.dwBuildNumber = 100 ;
    stOSExW.wSuiteMask = VER_SUITE_ENTERPRISE  ;
    stOSExW.dwPlatformId = VER_PLATFORM_WIN32_NT ;
    stOSExW.wProductType = VER_NT_SERVER ;
    lstrcpyW ( stOSExW.szCSDVersion , L"Service Pack 2" ) ;

    // Windows Server 2003, Datacenter Edition.
    stOSExA.dwMajorVersion = 5 ;
    stOSExA.dwMinorVersion = 2 ;
    stOSExA.dwBuildNumber = 100 ;
    stOSExA.wSuiteMask = VER_SUITE_DATACENTER  ;
    stOSExA.dwPlatformId = VER_PLATFORM_WIN32_NT ;
    stOSExA.wProductType = VER_NT_SERVER ;
    strcpy ( stOSExA.szCSDVersion , "Service Pack 2" ) ;
    
    stOSExW.dwMajorVersion = 5 ;
    stOSExW.dwMinorVersion = 2 ;
    stOSExW.dwBuildNumber = 100 ;
    stOSExW.wSuiteMask = VER_SUITE_DATACENTER  ;
    stOSExW.dwPlatformId = VER_PLATFORM_WIN32_NT ;
    stOSExW.wProductType = VER_NT_SERVER ;
    lstrcpyW ( stOSExW.szCSDVersion , L"Service Pack 2" ) ;
}


