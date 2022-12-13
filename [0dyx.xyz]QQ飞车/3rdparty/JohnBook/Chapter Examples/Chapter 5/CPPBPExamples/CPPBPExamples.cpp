/*----------------------------------------------------------------------
Debugging Applications for Microsoft .NET and Microsoft Windows
Copyright © 1997-2003 John Robbins -- All rights reserved.
----------------------------------------------------------------------*/
#include "stdafx.h"

class SomethingToDo 
{
public  :
    
    void Do ( int iParam , int iAnotherParam )
    {
        iAnotherParam = 6 ;
        int jLocal = iParam ;
        Re ( 3.145 * jLocal ) ;
    }
    
    void Re ( double d )
    {
        d = 0.667 ;
        double eLocal = 0.002 ;
        eLocal = eLocal * d ;
        Mi ( _T ( "A me so la ti do" ) ) ;
    }
    
    void Mi ( TCHAR * s ) 
    {
        s = L"Hello from Mi" ;
        Fa ( ) ;
    }
    
    void Fa ( ) 
    {
        So ( ) ;
    }
    
    void So ( )
    {
        La ( ) ;
    }
    
    void La ( )
    {
        Ti ( ) ;
    }
    
    void Ti ( )
    {
        Doh ( ) ;
    }
    
    void Doh ( )
    {
        _tprintf ( _T ( "Hit da Doh!\n" ) ) ;
    }
    
    int Overload ( int i )
    {
        return ( i ) ;
    }
    
    char Overload ( char c )
    {
        return ( c ) ;
    }
    
    static unsigned __stdcall ThreadFunc ( LPVOID )
    {
        for ( int i = 0 ; i < 10 ; i++ )
        {
            Sleep ( 1000 ) ;
        }
        _tprintf ( _T ( "Thread done!\n" ) ) ;
        return ( 0 ) ;
    }
    
} ;      

extern "C" bool CallFromDebugger ( bool r )
{
    if ( true == r )
    {
        _tprintf ( _T ( "CallFromDebugger called\n" ) ) ;
    }
    Sleep ( 20 * 1000 ) ;
    return ( r ) ;
}

class MyThreadClass
{
public  :
    static DWORD WINAPI ThreadFunc ( LPVOID )
    {
        return ( 0 ) ;
    }
} ;


void _tmain ( void ) 
{
	CallFromDebugger ( false ) ;
    SomethingToDo * x = new SomethingToDo ( ) ;
	
    x->Do ( 0x42 , 0x666 ) ;
	
    //x.Mi ( x.RetString ( ) ) ;
	
    x->Overload ( 1 ) ;
	
    //AnotherFile y = new AnotherFile ( ) ;
	
    //y.OverHere ( 2 ) ;
	
	//Console.WriteLine ( "About to start threads!" ) ;
	unsigned dwTID ;
	HANDLE t1 = (HANDLE)
	    _beginthreadex ( NULL                      ,
	                     0                         ,
                         (unsigned int (__stdcall *)(void *))SomethingToDo::ThreadFunc ,
	                     0                         ,
	                     0                         ,
	                     &dwTID                     ) ;
    HANDLE t2 = (HANDLE)
        _beginthreadex ( NULL                      ,
	                     0                         ,
                         (unsigned int (__stdcall *)(void *))SomethingToDo::ThreadFunc ,
	                     0                         ,
	                     0                         ,
	                     &dwTID                     ) ;

    HANDLE t3 = (HANDLE)
        _beginthreadex ( NULL                      ,
	                     0                         ,
                         (unsigned int (__stdcall *)(void *))SomethingToDo::ThreadFunc ,
	                     0                         ,
	                     0                         ,
	                     &dwTID                     ) ;
                
    HANDLE t4 = (HANDLE)
        _beginthreadex ( NULL                      ,
	                     0                         ,
                         (unsigned int (__stdcall *)(void *))SomethingToDo::ThreadFunc ,
	                     0                         ,
	                     0                         ,
	                     &dwTID                     ) ;
	                     
    HANDLE hArray[ 4 ] ;
    hArray[0] = t1 ;
    hArray[1] = t2 ;
    hArray[2] = t3 ;
    hArray[3] = t4 ;
    
    WaitForMultipleObjects ( 4 , (LPHANDLE)&hArray , TRUE , INFINITE ) ;
    
    CloseHandle ( t1 ) ;
    CloseHandle ( t2 ) ;
    CloseHandle ( t3 ) ;
    CloseHandle ( t4 ) ;
}

