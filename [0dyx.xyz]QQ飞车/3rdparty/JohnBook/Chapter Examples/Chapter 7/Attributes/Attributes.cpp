/*----------------------------------------------------------------------
Debugging Applications for Microsoft .NET and Microsoft Windows
Copyright © 1997-2003 John Robbins -- All rights reserved.
----------------------------------------------------------------------*/
#include "stdafx.h"

// Set up the event source.  This is basically the prototype for 
// an event.
[ event_source ( native ) ]
class CSource 
{
public      :
   __event void MyEvent ( int nValue ) ;
} ;

[ event_receiver ( native) ]
class CReceiver 
{
public      :
   void MyHandler1 ( int nValue )  
   {
      _tprintf ( _T ( "MyHandler1 was called with value %d.\n" ) , nValue ) ;
   }

   void MyHandler2(int nValue) 
   {
      _tprintf ( _T ( "MyHandler2 was called with value %d.\n" ) , nValue ) ;
   }

   void hookEvent(CSource* pSource) 
   {
      __hook ( &CSource::MyEvent , pSource , &CReceiver::MyHandler1 ) ;
      __hook ( &CSource::MyEvent , pSource , &CReceiver::MyHandler2 ) ;
   }

   void unhookEvent(CSource* pSource) 
   {
      __unhook ( &CSource::MyEvent , pSource , &CReceiver::MyHandler1 ) ;
      __unhook ( &CSource::MyEvent , pSource , &CReceiver::MyHandler2 ) ;
   }
};

#ifdef _UNICODE
int wmain ( void ) 
#else
int main ( void ) 
#endif
{
   CSource source ;
   CReceiver receiver ;

   receiver.hookEvent ( &source ) ;
   
   __raise source.MyEvent ( 123 ) ;
   
   receiver.unhookEvent ( &source ) ;
}