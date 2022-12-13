/*//////////////////////////////////////////////////////////////////////
Debugging Applications for Microsoft .NET and Microsoft Windows
Copyright (c) 1997-2003 John Robbins -- All rights reserved.
//////////////////////////////////////////////////////////////////////*/

#include "stdafx.h"

/*//////////////////////////////////////////////////////////////////////
// The core COM stuff so ProfilerLib.LIB can take care of the COM
// goo for me.
//////////////////////////////////////////////////////////////////////*/

wchar_t * G_szProfilerGUID =
                          L"{F6F3B5B7-4EEC-48f6-82F3-A9CA97311A1D}" ;

GUID G_CLSID_PROFILER =
    { 0xf6f3b5b7 , 0x4eec , 0x48f6 ,
    { 0x82 , 0xf3 , 0xa9 , 0xca , 0x97 , 0x31 , 0x1a , 0x1d } } ;

wchar_t * G_szProgIDPrefix = L"ExceptionMonProfiler" ;

wchar_t * G_szProfilerName = L"ExceptionMon" ;

/*//////////////////////////////////////////////////////////////////////
// The function DllMain will call to ensure cleanup of the COM object
// in case ICorProfilerCallback::ShutDown does not get called.
//////////////////////////////////////////////////////////////////////*/
void DllUnloadingCheck ( void ) ;

extern "C" BOOL WINAPI DllMain ( HINSTANCE hInstance      ,
                                 DWORD     dwReason       ,
                                 LPVOID    /*lpReserved*/  )
{
    switch ( dwReason )
    {
        case DLL_PROCESS_ATTACH :
            DisableThreadLibraryCalls ( hInstance ) ;
            G_hInst = hInstance ;
            break ;
        case DLL_PROCESS_DETACH :
            DllUnloadingCheck ( ) ;
        default :
            break ;
    }
    return( TRUE ) ;
}