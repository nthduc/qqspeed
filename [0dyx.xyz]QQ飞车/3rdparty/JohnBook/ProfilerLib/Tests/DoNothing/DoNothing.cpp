// DoNothing.cpp : Defines the entry point for the DLL application.
//

#include "stdafx.h"

wchar_t * G_szProfilerGUID = L"{D27965C4-B361-4711-A7BE-465F71A97D87}" ;

GUID G_CLSID_PROFILER =
    { 0xd27965c4, 0xb361, 0x4711,
    { 0xa7, 0xbe, 0x46, 0x5f, 0x71, 0xa9, 0x7d, 0x87 } };

wchar_t * G_szProgIDPrefix = L"DoNothingProfiler" ;

wchar_t * G_szProfilerName = L"DoNothing" ;


class CDoNothingProfilerCallback : public CBaseProfilerCallback
{
public      :

    STDMETHOD ( OnInitialize) ( ) ;
    STDMETHOD ( OnShutdown ) (  ) ;

} ;


HRESULT CDoNothingProfilerCallback :: OnInitialize (  )
{
    ::MessageBeep ( (UINT)-1 ) ;
    ::Sleep ( 100 ) ;
    ::MessageBeep ( (UINT)-1 ) ;
    ::Sleep ( 400 ) ;
    ::MessageBeep ( (UINT)-1 ) ;
    
    // Tell the CLR that I want it all....
    m_pICorProfilerInfo->SetEventMask ( COR_PRF_MONITOR_ALL ) ;
    
    return ( S_OK ) ;
}

HRESULT CDoNothingProfilerCallback :: OnShutdown ( )
{
    ::MessageBeep ( (UINT)-1 ) ;
    ::Sleep ( 100 ) ;
    ::MessageBeep ( (UINT)-1 ) ;
    ::Sleep ( 400 ) ;
    ::MessageBeep ( (UINT)-1 ) ;

    return ( S_OK ) ;
}

ICorProfilerCallback * AllocateProfilerCallback ( )
{
    if ( 0 != GetEnvironmentVariable ( _T ( "DONOTHINGINT3" ) , NULL , 0 ) )
    {
        __asm int 3 ;
    }
    CBaseProfilerCallback * pNew = new CDoNothingProfilerCallback ( ) ;
    return ( pNew ) ;
}

extern "C" BOOL WINAPI DllMain ( HINSTANCE hInstance      ,
                                 DWORD     dwReason       ,
                                 LPVOID    /*lpReserved*/  )
{
    switch ( dwReason )
    {
        case DLL_PROCESS_ATTACH:
            DisableThreadLibraryCalls ( hInstance ) ;
            G_hInst = hInstance ;
            break ;
        default :
            break ;
    }
    return( TRUE ) ;
}
