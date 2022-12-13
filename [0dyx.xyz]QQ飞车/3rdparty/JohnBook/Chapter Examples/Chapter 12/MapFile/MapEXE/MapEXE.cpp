/*----------------------------------------------------------------------
Debugging Applications for Microsoft .NET and Microsoft Windows
Copyright © 1997-2003 John Robbins -- All rights reserved.
----------------------------------------------------------------------*/
#include "stdafx.h"

#include "MapDLL.h"

void main ( void )
{
    MapDLLFunction ( ) ;
	printf("Hello from MapEXE!\n");
    char * p = NULL ;
    MapDLLHappyFunc ( p ) ;
}

