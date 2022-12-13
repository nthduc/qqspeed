/*----------------------------------------------------------------------
Debugging Applications for Microsoft .NET and Microsoft Windows
Copyright © 1997-2003 John Robbins -- All rights reserved.
----------------------------------------------------------------------*/
#include "stdafx.h"

#include "stdafx.h"

void Foo ( void ) ;

void Bar ( void )
{
    Foo ( ) ;
}

void Baz ( void )
{
    Bar ( ) ;
}

void Bop ( void )
{
    Baz ( ) ;
}

void Foo ( void )
{
}

extern "C" void YeOlCFunc ( void ) ;
void wmain (void )
{
    YeOlCFunc ( ) ;

    Bop ( ) ;
    Bop ( ) ;
    Foo ( ) ;
    Foo ( ) ;

    YeOlCFunc ( ) ;
}

extern "C" void YeOlCFunc ( void )
{
}

