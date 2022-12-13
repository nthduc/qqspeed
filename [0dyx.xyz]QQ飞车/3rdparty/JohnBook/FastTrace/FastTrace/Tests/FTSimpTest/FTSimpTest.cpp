/*----------------------------------------------------------------------
Debugging Applications for Microsoft .NET and Microsoft Windows
Copyright © 1997-2003 John Robbins -- All rights reserved.
----------------------------------------------------------------------*/
#include "stdafx.h"
#include "FastTrace.h"

#pragma warning ( disable : 4312 )

unsigned __stdcall AThread ( void * )
{
    Sleep ( 103 ) ;
    FastTrace ( _T ( "Hello from AThread!" ) ) ;
    SnapFastTraceFiles ( ) ;
    return ( 0 ) ;
}

unsigned __stdcall BThread ( void * pVal )
{
    size_t iLoop = (size_t)pVal ;
    for ( size_t i = 0 ; i < iLoop ; i++ )
    {
        Sleep ( 92 ) ;
        FastTrace ( _T ( "Hello from BThread -> %d!" ) , i ) ;
    }
    return ( 0 ) ;
}

unsigned __stdcall CThread ( void * pVal )
{
    size_t iLoop = (size_t)pVal ;
    for ( size_t i = 0 ; i < iLoop ; i++ )
    {
        Sleep ( 271 ) ;
        FastTrace ( _T ( "Hello from CThread -> %d!" ) , i ) ;
    }
    return ( 0 ) ;
}

void DoSomeWork ( int iParam )
{
    unsigned uTID = 0 ;
    HANDLE hThread = (HANDLE)_beginthreadex ( NULL      ,
                                              0         ,
                                              &AThread  ,
                                              NULL      ,
                                              0         ,
                                              &uTID      ) ;
    ::WaitForSingleObject ( hThread , INFINITE ) ;
    CloseHandle ( hThread ) ;
    
    #define BTHREADCOUNT 3
    HANDLE hBThread[BTHREADCOUNT] ;
    for ( int iB = 0 ; iB < BTHREADCOUNT ; iB++ )
    {
        hBThread[ iB ] = (HANDLE)_beginthreadex ( NULL            ,
                                                  0               ,
                                                  &BThread        ,
                                                  (void*)(iB + 2) ,
                                                  0               ,
                                                  &uTID            ) ;
    }
    
    #define CTHREADCOUNT 5
    HANDLE hCThread[CTHREADCOUNT] ;
    for ( int iC = 0 ; iC < CTHREADCOUNT ; iC++ )
    {
        hCThread[ iC ] = (HANDLE)_beginthreadex ( NULL            ,
                                                  0               ,
                                                  &CThread        ,
                                                  (void*)(6 - iC) ,
                                                  0               ,
                                                  &uTID            ) ;
    }

    if ( 1 == iParam )
    {
        SnapFastTraceFiles ( ) ;
    }

    ::WaitForMultipleObjects ( BTHREADCOUNT ,
                               hBThread     ,
                               TRUE         ,
                               INFINITE      ) ;
    ::WaitForMultipleObjects ( CTHREADCOUNT ,
                               hCThread     ,
                               TRUE         ,
                               INFINITE      ) ;
                               
    for ( int iB = 0 ; iB < BTHREADCOUNT ; iB++ )
    {
        CloseHandle ( hBThread[ iB ] ) ;
    }

    for ( int iC = 0 ; iC < CTHREADCOUNT ; iC++ )
    {
        CloseHandle ( hCThread[ iC ] ) ;
    }
    
    Sleep ( 182 ) ;
                                                
}

void _tmain ( void )
{
    FASTTRACEOPTIONS stOpts ;
    
    GetFastTraceOptions ( &stOpts ) ;
    stOpts.pDebugOut = OutputDebugString ;
    SetFastTraceOptions ( &stOpts ) ;
    
    FastTrace ( _T ( "Hello from FTSimpTest\n!" ) ) ;
    
    DoSomeWork ( 0 ) ;
    
    FlushFastTraceFiles ( ) ;
    
    stOpts.bDoTimings = TRUE ;
    SetFastTraceOptions ( &stOpts ) ;
    DoSomeWork ( 1 ) ;
    stOpts.bDoTimings = FALSE ;
    SetFastTraceOptions ( &stOpts ) ;
    
    FastTrace ( _T ( "THIS SHOULD BE THE LAST LINE!" ) ) ;
}

