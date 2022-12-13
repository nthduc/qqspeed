/*----------------------------------------------------------------------
Debugging Applications for Microsoft .NET and Microsoft Windows
Copyright © 1997-2003 John Robbins -- All rights reserved.
----------------------------------------------------------------------*/
#include "stdafx.h"

BOOL APIENTRY DllMain ( HMODULE hModule         ,
                        DWORD   dwReason        ,
                        LPVOID /*lpReserved*/    )
{
    switch ( dwReason )
    {
        case DLL_PROCESS_ATTACH :
            DisableThreadLibraryCalls ( hModule ) ;
            break ;
    }
    return ( TRUE ) ;
}

