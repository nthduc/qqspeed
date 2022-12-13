/*----------------------------------------------------------------------
Debugging Applications for Microsoft .NET and Microsoft Windows
Copyright © 1997-2003 John Robbins -- All rights reserved.
----------------------------------------------------------------------*/
#include "PCH.h"
#include "Dll.h"

static HINSTANCE g_hInst = NULL ;

BOOL APIENTRY DllMain ( HINSTANCE hModule  ,
                        DWORD     dwReason ,
                        LPVOID              )
{
    switch ( dwReason )
    {
        case DLL_PROCESS_ATTACH :
            g_hInst = hModule ;
            VERIFY ( DisableThreadLibraryCalls ( hModule ) ) ;
            break ;
        default :
            break ;
    }
    return ( TRUE ) ;
}

HINSTANCE GetDllHandle ( void )
{
    ASSERT ( NULL != g_hInst ) ;
    return ( g_hInst ) ;
}
