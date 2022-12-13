/*----------------------------------------------------------------------
Debugging Applications for Microsoft .NET and Microsoft Windows
Copyright © 1997-2003 John Robbins -- All rights reserved.
----------------------------------------------------------------------*/
#include "stdafx.h"

void Funcarrific ( void )
{
    throw L"This is a test throw!" ;
}

void Funcadelic ( void )
{
    Funcarrific ( ) ;
}

void Funcarama ( void )
{
    Funcadelic ( ) ;
}

void main ( void )
{
    try
    {
        Funcarama ( ) ;
    }
    catch ( TCHAR * e )
    {
        printf ( "Error is: %s\n" , e ) ;
    }
}
