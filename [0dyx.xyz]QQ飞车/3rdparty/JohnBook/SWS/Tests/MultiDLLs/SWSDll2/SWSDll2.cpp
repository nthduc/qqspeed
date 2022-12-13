/*----------------------------------------------------------------------
Debugging Applications for Microsoft .NET and Microsoft Windows
Copyright © 1997-2003 John Robbins -- All rights reserved.
----------------------------------------------------------------------*/
#include "stdafx.h"

BOOL APIENTRY DllMain( HANDLE /*hModule*/,
                       DWORD  /*ul_reason_for_call*/,
                       LPVOID /*lpReserved*/
                     )
{
    return TRUE;
}

extern "C" void __declspec(dllexport) Dll2Function1 ( void )
{
    _tprintf ( _T ( "Hello from Dll2Function1\n" ) ) ;
}


extern "C" void __declspec(dllexport) Dll2Function2 ( void )
{
    _tprintf ( _T ( "Hello from Dll2Function2\n" ) ) ;
}