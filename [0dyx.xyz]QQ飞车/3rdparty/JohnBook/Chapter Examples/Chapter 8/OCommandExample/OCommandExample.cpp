/*----------------------------------------------------------------------
Debugging Applications for Microsoft .NET and Microsoft Windows
Copyright © 1997-2003 John Robbins -- All rights reserved.
----------------------------------------------------------------------*/
#include "stdafx.h"

void Baz ( int )
{
    // To see the following convert into WinDBG commands, issue the
    // command ".ocommand WINDBGCMD:" inside WinDBG
    OutputDebugString ( _T ( "WINDBGCMD: .echo \"Hello from WinDBG\";g" ) ) ;
    OutputDebugString ( _T ( "WINDBGCMD: kp;g" ) ) ;
    OutputDebugString ( _T ( "WINDBGCMD: .echo \"The stack walk is done\";g" ) ) ;
}

void Bar ( int j )
{
    Baz ( j++ ) ;
}

void Foo ( int i )
{
    Bar ( i++ ) ;
}

void _tmain ( void )
{
    Foo ( 5 ) ;
}

