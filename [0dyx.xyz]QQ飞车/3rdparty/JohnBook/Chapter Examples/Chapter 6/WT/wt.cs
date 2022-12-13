/*----------------------------------------------------------------------
Debugging Applications for Microsoft .NET and Microsoft Windows
Copyright © 1997-2003 John Robbins -- All rights reserved.
----------------------------------------------------------------------*/
using System ;

class App
{
    static void Main ( )
    {
        Foo ( ) ;
    }

    public static void Foo ( )
    {
        Bar ( ) ;
    }
   
    public static void Bar ( )
    {
        Baz ( ) ;
    }
    public static void Baz ( )
    {
        
    }

}