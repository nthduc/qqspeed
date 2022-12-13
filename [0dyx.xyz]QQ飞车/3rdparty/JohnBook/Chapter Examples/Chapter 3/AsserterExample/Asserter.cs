/*----------------------------------------------------------------------
Debugging Applications for Microsoft .NET and Microsoft Windows
Copyright © 1997-2003 John Robbins -- All rights reserved.
----------------------------------------------------------------------*/

using System ;
using System.Diagnostics ;
using System.Text ;

class HappyAppy
{
    public static void Fum ( )
    {
        Debug.Assert ( false , "Debug.Assert assertion" ) ;
    }
    
    public static void Fo ( StringBuilder sb )
    {
        Fum ( ) ;
    }
    public static void Fi ( IntPtr p )
    {
        Fo ( new StringBuilder ( ) ) ;
    }
    
    public static void Fee ( string Blah )
    {
        Fi ( new System.IntPtr ( )  ) ;
    }
    
    public static void Baz ( double d)
    {
        Fee ( "Hello" ) ;
    }

    public static void Bar ( Object o )
    {
        Baz ( 3.154 ) ;
    }

    public static void Foo ( int i )
    {
        String s ;
        s = "Stuff" ;
        Bar ( s ) ;
    }

    public static void Main ( )
    {
        
        Foo ( 1 ) ;
    }
}
