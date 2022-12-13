/*----------------------------------------------------------------------
Debugging Applications for Microsoft .NET and Microsoft Windows
Copyright © 1997-2003 John Robbins -- All rights reserved.
----------------------------------------------------------------------*/
#include "stdafx.h"

void main(void)
{
    // Create an operating system heap.
    HANDLE hHeap = HeapCreate ( 0 , 128 , 0 ) ;

    // Allocate a 10-byte block.
    LPVOID pMem = HeapAlloc ( hHeap , 0 , 10 ) ;

    // Write 12 bytes to a 10-byte block (an overwrite).
    memset ( pMem , 0xAC , 12 ) ;

    // Allocate a new 20-byte block.
    LPVOID pMem2 = HeapAlloc ( hHeap , 0 , 20 ) ;

    // Underwrite 1 byte on the second block.
    char * pUnder = (char *)((DWORD_PTR)pMem2 - 1 );
    *pUnder = 'P' ;

    // Free the first block. This call to HeapFree will trigger a
    // breakpoint from the operating system debug heap code.
    HeapFree ( hHeap , 0 , pMem ) ;

    // Free the second block. Notice that this call won't report
    // a problem.
    HeapFree ( hHeap , 0 , pMem2 ) ;

    // Free a bogus block. Notice that this call won’t report a problem.
    HeapFree ( hHeap , 0 , (LPVOID)0x1 ) ;

    HeapDestroy ( hHeap ) ;

}

