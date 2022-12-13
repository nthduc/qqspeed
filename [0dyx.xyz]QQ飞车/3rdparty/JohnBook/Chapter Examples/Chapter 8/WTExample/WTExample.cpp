/*----------------------------------------------------------------------
Debugging Applications for Microsoft .NET and Microsoft Windows
Copyright © 1997-2003 John Robbins -- All rights reserved.
----------------------------------------------------------------------*/
#include "stdafx.h"

void Do2 ( )
{
    Sleep ( 10 ) ;
}

void Ti ( )
{
    Do2 ( ) ;
}

void La ( ) 
{
    Ti ( ) ;
}

void So ( ) 
{
    La ( ) ;
}

void Fa ( ) 
{
    So ( ) ;
}

void Mi ( ) 
{
    Fa ( ) ;
}

void Re ( ) 
{
    Mi ( ) ;
}

void Do ( ) 
{
    Re ( ) ;
}

void Baz ( )
{
    Do ( ) ;
}

void Bar ( )
{
    Baz ( ) ;
}

void Foo ( )
{
    Bar ( ) ;
}

void _tmain ( void )
{
	Foo ( ) ;
}

