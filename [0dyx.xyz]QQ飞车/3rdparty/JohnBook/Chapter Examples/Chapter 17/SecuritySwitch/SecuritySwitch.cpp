/*----------------------------------------------------------------------
Debugging Applications for Microsoft .NET and Microsoft Windows
Copyright © 1997-2003 John Robbins -- All rights reserved.
----------------------------------------------------------------------*/
#include "stdafx.h"
#include <windows.h>

// _ReturnAddress and _AddressOfReturnAddress should be prototyped before use 
extern "C" void * _AddressOfReturnAddress(void);

extern "C" void * _ReturnAddress(void);
 
#pragma intrinsic(_AddressOfReturnAddress)
#pragma intrinsic(_ReturnAddress)

void WhackTheReturnAddress ( TCHAR * szBuff )
{
    _tprintf ( _T ( "Ret addr         : 0x%08X\n" ) , _ReturnAddress ( ) ) ;
    _tprintf ( _T ( "Addr of Ret addr : 0x%08X\n" ) , _AddressOfReturnAddress ( ) ) ;
    TCHAR szSmallBuff[ 10 ] ;

    _tcscpy ( szSmallBuff , szBuff ) ;

    _tprintf ( _T ( "Ret addr         : 0x%08X\n" ) , _ReturnAddress ( ) ) ;
    _tprintf ( _T ( "Addr of Ret addr : 0x%08X\n" ) , _AddressOfReturnAddress ( ) ) ;
}

#ifdef _UNICODE
void wmain ( void )
#else
void main ( void )
#endif
{
    TCHAR szLargeBuff[] = _T ( "Wait!! This buffer is larger than 10 characters!!" ) ;
    
    WhackTheReturnAddress ( szLargeBuff ) ;
	
}
