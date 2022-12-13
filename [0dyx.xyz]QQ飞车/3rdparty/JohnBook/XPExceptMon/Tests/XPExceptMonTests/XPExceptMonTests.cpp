/*----------------------------------------------------------------------
Debugging Applications for Microsoft .NET and Microsoft Windows
Copyright © 1997-2003 John Robbins -- All rights reserved.
----------------------------------------------------------------------*/
#include "stdafx.h"

void CPPException ( void )
{
    try
    {
        throw _T ( "This is a string..." ) ;
    }
    catch ( TCHAR * )
    {
        _tprintf ( _T ( "Did a C++ catch.\n" ) ) ;
    }
}

void Raised ( void )
{
    __try
    {
        RaiseException ( 0x123 , 0 , 0 , NULL ) ;
    }
    __except ( EXCEPTION_EXECUTE_HANDLER )
    {
        _tprintf ( _T ( "Caught a RaiseException error\n" ) ) ;
    }
}

void Access ( void )
{
    __try
    {
        char * p = NULL ;
        *p = 'p' ;
    }
    __except ( EXCEPTION_EXECUTE_HANDLER )
    {
        _tprintf ( _T ( "Ate an access violation!\n" ) ) ;
    }
}

void DoEmAll ( void )
{
    CPPException ( ) ;
    Raised ( ) ;
    Access ( ) ;
}

UINT WINAPI DaThread ( LPVOID )
{
    Access ( ) ;
    Raised ( ) ;
    CPPException ( ) ;
    return ( 0 ) ;
}

void _tmain ( void )
{
    HINSTANCE hInst = LoadLibrary ( _T ( "XPExceptMon.dll" ) ) ;
    if ( NULL != hInst )
    {
        UINT dwTID ;
        HANDLE hThread = (HANDLE)_beginthreadex ( NULL      , 
                                                  0         , 
                                                  &DaThread ,
                                                  NULL      , 
                                                  0         , 
                                                  &dwTID     ) ;
        DoEmAll ( ) ;
        WaitForSingleObject ( hThread , INFINITE ) ;
        CloseHandle ( hThread ) ;
    }
}

