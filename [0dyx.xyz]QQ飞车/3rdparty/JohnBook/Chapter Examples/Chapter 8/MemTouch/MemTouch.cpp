/*----------------------------------------------------------------------
Debugging Applications for Microsoft .NET and Microsoft Windows
Copyright © 1997-2003 John Robbins -- All rights reserved.
----------------------------------------------------------------------*/
#include "stdafx.h"

void TouchIt ( char * szBuff )
{
    szBuff[ 0 ] = 'P' ;
    szBuff[ 1 ] = 'a' ;
    szBuff[ 2 ] = 'm' ;
    szBuff[ 3 ] = '\0' ;
}

void ReadIt ( char * szText )
{
    _tprintf ( _T ( "%S\n" ) , szText ) ;
}

void _tmain ( void )
{
    char * szMem = (char*)malloc ( 10 ) ;
    
    TouchIt ( szMem ) ;
    
    ReadIt ( szMem ) ;
}

