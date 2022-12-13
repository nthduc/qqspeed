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

extern "C" void __declspec ( dllexport ) DllTwoFunction ( void )
{
    _tprintf ( _T ( "DllTwoFunction called from thread 0x%08X\n" ) , GetCurrentThreadId ( ) ) ;
}