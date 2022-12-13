/*----------------------------------------------------------------------
Debugging Applications for Microsoft .NET and Microsoft Windows
Copyright © 1997-2003 John Robbins -- All rights reserved.
----------------------------------------------------------------------*/
#include "stdafx.h"

// The stack variable overflow is caught because of /RTCs.
// /RTCs also catches _cdecl/__stdcall mismatches.
void DoLocalStackOverflow ( void )
{
    TCHAR szBuff1[ 10 ] ;
    TCHAR szBuff2[ 10 ] ;
    TCHAR szBuff3[ 10 ] ;

    // Here's where the overrun happens.
    _tcscpy ( szBuff2 , _T ( "1234567890" ) ) ;
    
    // Avoid compiler errors...
    _tcscpy ( szBuff1 , _T ( "12345" ) ) ;    
    _tcscpy ( szBuff3 , _T ( "12345" ) ) ;    
    
    // The overflow check occurs at the end of the function.
}

// The data trucation is caught because of /RTCc
char DoTruncateValue ( void )
{
    int iVal = 0xFFFFFF ;
    // With /W4, a C4244 error will tell you that you are loosing 
    // bits.
    return ( iVal ) ;
}

// The uninitialized usage is caught with the /RTCu
int DoUninitialized ( int k )
{
    int i ;

    if ( k != 0 )
    {
        i = 3 ;
    }
    // With /W4, you will get a C4701 indicating i MAY be used without
    // having been initialized.
    // The /RTCu error will only pop when k != 0.  
    return( i ) ;
}

#ifdef _UNICODE
void wmain ( void )
#else
void main ( void ) 
#endif
{
// __MSVC_RUNTIME_CHECKS is defined if /RTC* is used.
#ifdef __MSVC_RUNTIME_CHECKS
    _tprintf ( _T ( "Run time error checking is turned on!\n" ) ) ;
#endif
    // Do some errors.
    DoLocalStackOverflow ( ) ;
    DoTruncateValue ( ) ;
    DoUninitialized ( 1 ) ;
    // The call below will make the /RTCu pop happen.
    DoUninitialized ( 0 ) ;
}
