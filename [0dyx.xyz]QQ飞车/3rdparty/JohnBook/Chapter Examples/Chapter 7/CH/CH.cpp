/*----------------------------------------------------------------------
Debugging Applications for Microsoft .NET and Microsoft Windows
Copyright © 1997-2003 John Robbins -- All rights reserved.
----------------------------------------------------------------------*/
#include "stdafx.h"

void _tmain ( void )
{

    __try
    {
        __asm out 3 , al
    }
    __except ( EXCEPTION_EXECUTE_HANDLER )
    {
        OutputDebugString ( L"Hello from the OUT exception handler\n" ) ;
    }
    
    __try
    {
        char * p = NULL ;
        *p = 'P' ;
    }
    __except ( EXCEPTION_EXECUTE_HANDLER )
    {
        OutputDebugString ( L"Hello from the AV exception handler\n" ) ;
    }
    
    RaiseException ( 0xCCCCCCCC , 0 , 0 , NULL ) ;

    BOOL bRet = CloseHandle ( (HANDLE)0x01 ) ;
    bRet = bRet ;
    
    OutputDebugString ( _T ( "Now is the time\n" ) ) ;
}

