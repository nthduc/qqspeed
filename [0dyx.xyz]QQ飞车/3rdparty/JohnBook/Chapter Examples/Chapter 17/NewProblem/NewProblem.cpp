/*----------------------------------------------------------------------
Debugging Applications for Microsoft .NET and Microsoft Windows
Copyright © 1997-2003 John Robbins -- All rights reserved.
----------------------------------------------------------------------*/
// This define must occur before any headers are included.
#define _CRTDBG_MAP_ALLOC
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <tchar.h>

// Include CRTDBG.H after all other headers
#include <crtdbg.h>

void main ( void )
{
    // Turn on the full heap checking
    _CrtSetDbgFlag ( _CRTDBG_ALLOC_MEM_DF       |
                     _CRTDBG_CHECK_ALWAYS_DF    |
                     _CRTDBG_DELAY_FREE_MEM_DF  |
                     _CRTDBG_LEAK_CHECK_DF       ) ;

    // Allocate some more memory.
    TCHAR * pNew = new TCHAR[ 200 ] ;
    TCHAR * pNew2 = new TCHAR[ 200 ] ;
    TCHAR * pMemLeak = (TCHAR*)malloc ( 100 ) ;
    
    _tcscpy ( pNew , _T ( "New'd memory..." ) ) ;
    _tcscpy ( pNew2 , _T ( "More new'd memory..." ) ) ;
    _tcscpy ( pMemLeak , _T ( "Malloc'd memory..." ) ) ;
    
 }