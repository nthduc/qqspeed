/*----------------------------------------------------------------------
Debugging Applications for Microsoft .NET and Microsoft Windows
Copyright © 1997-2003 John Robbins -- All rights reserved.
----------------------------------------------------------------------*/
#include "stdafx.h"

typedef unsigned (__stdcall *PTHREAD_START) (void *);


extern "C" void __declspec ( dllimport ) DllTwoFunction ( void ) ;
extern "C" void __declspec ( dllimport ) DllOneFunction ( void ) ;
    
ULONG __stdcall TheThread ( LPVOID lVal )
{
    for ( UINT_PTR i = 0 ; i < (UINT_PTR)lVal ; i++ )
    {
        DllOneFunction ( ) ;
        DllTwoFunction ( ) ;
    }
    _endthreadex ( 0 ) ;
    return ( 0 ) ;
}

void main(int argc, char* argv[])
{
    if ( 1 == argc )
    {
        printf ( "Usage : ThreadTest <num>\n" ) ;
        return ;
    }

    UINT_PTR iCount = (UINT_PTR)atoi ( argv[ 1 ] ) ;
    if ( 0 == iCount )
    {
        printf ( "Invalid command line\n" ) ;
        return ;
    }

    HANDLE * paThreads = (HANDLE*)malloc ( sizeof ( HANDLE ) * iCount ) ;

    for ( UINT_PTR i = 0 ; i < iCount ; i++ )
    {
        UINT dwTID ;
        paThreads[ i ] = (HANDLE)_beginthreadex ( NULL      ,
                                                  0         ,
                                   (PTHREAD_START)TheThread ,
                                           (void*)iCount    ,
                                                  0         ,
                                                  &dwTID     ) ;
        if ( 0 == paThreads[ i ] )
        {
            printf ( "Number %d returned false!!!!\n" , i ) ;
        }
    }

    printf ( "Created %d threads!\n" , iCount ) ;
    WaitForMultipleObjects ( (DWORD)iCount , paThreads , TRUE , INFINITE ) ;
    printf ( "All threads signalled!\n" ) ;
    for ( i = 0 ; i < iCount ; i++ )
    {
        CloseHandle ( paThreads[ i ] ) ;
    }
    free ( paThreads ) ;
}

