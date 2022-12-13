/*----------------------------------------------------------------------
Debugging Applications for Microsoft .NET and Windows Applications
Copyright © 2002-2003 John Robbins
----------------------------------------------------------------------*/

#include "stdafx.h"

/*//////////////////////////////////////////////////////////////////////
// The core COM stuff so ProfilerLib.LIB can take care of the COM
// goo for me.
//////////////////////////////////////////////////////////////////////*/

wchar_t * G_szProfilerGUID =
                          L"{D3799FA3-13EE-4bab-A263-843C14397DE4}" ;

GUID G_CLSID_PROFILER =
    { 0xd3799fa3, 0x13ee, 0x4bab,
    { 0xa2, 0x63, 0x84, 0x3c, 0x14, 0x39, 0x7d, 0xe4 } } ;

wchar_t * G_szProgIDPrefix = L"FlowTraceProfiler" ;

wchar_t * G_szProfilerName = L"FlowTrace" ;

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