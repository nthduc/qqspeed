/*----------------------------------------------------------------------
Debugging Applications for Microsoft .NET and Microsoft Windows
Copyright © 1997-2003 John Robbins -- All rights reserved.
----------------------------------------------------------------------*/
#include "stdafx.h"
#include "MapDLL.h"

BOOL APIENTRY DllMain( HANDLE /*hModule*/,
                       DWORD  ul_reason_for_call,
                       LPVOID /*lpReserved*/ )
{
    switch (ul_reason_for_call)
	{
		case DLL_PROCESS_ATTACH:
		case DLL_THREAD_ATTACH:
		case DLL_THREAD_DETACH:
		case DLL_PROCESS_DETACH:
			break;
    }
    return TRUE;
}


MAPDLL_API int MapDLLFunction ( void )
{
    printf ( "Hello from MapDLLFunction!\n" ) ;
    return ( 0x42 ) ;
}

static void InternalStaticFunction ( void )
{
    printf ( "Hello from InternalStaticFunction\n" ) ;
}

MAPDLL_API char * MapDLLHappyFunc ( char * p )
{
    InternalStaticFunction ( ) ;
    printf ( "Whoops, a crash is about to occur!\n" ) ;
    *p = 'c' ;
    return ( p ) ;
}