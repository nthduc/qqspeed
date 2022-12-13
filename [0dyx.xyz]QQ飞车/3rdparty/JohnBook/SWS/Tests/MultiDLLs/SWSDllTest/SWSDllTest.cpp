/*----------------------------------------------------------------------
Debugging Applications for Microsoft .NET and Microsoft Windows
Copyright © 1997-2003 John Robbins -- All rights reserved.
----------------------------------------------------------------------*/
#include "stdafx.h"

extern "C" void __declspec(dllimport) Dll1Function1 ( void ) ;
extern "C" void __declspec(dllimport) Dll1Function2 ( void ) ;
extern "C" void __declspec(dllimport) Dll2Function1 ( void ) ;
extern "C" void __declspec(dllimport) Dll2Function2 ( void ) ;


void main ( void )
{

    Dll1Function1 ( ) ;
    Dll1Function2 ( ) ;
    Dll2Function1 ( ) ;
    Dll2Function2 ( ) ;

}

