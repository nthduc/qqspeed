/*----------------------------------------------------------------------
Debugging Applications for Microsoft .NET and Microsoft Windows
Copyright © 1997-2003 John Robbins -- All rights reserved.
----------------------------------------------------------------------*/
using System ;
using System.Text ;
using System.Windows.Forms ;
using System.Diagnostics ;
using System.Threading ;
using Wintellect ;

class HappyAppy
{
    public static void Fum ( )
    {
        BugslayerStackTrace bst = new BugslayerStackTrace ( ) ;
        
        String str = bst.ToString ( ) ;
        Console.WriteLine ( "Should see full stack" ) ;
        Console.WriteLine ( str ) ;
        
        try
        {
            throw new Exception ( "Whoops!" ) ;
        }
        catch ( Exception e )
        {
            BugslayerStackTrace bst2 = new BugslayerStackTrace ( e , 0 ) ;
            Console.WriteLine ( "\r\nShould only see a single exception frame" ) ;
            Console.WriteLine ( bst2.ToString ( ) ) ;
            
            bst2 = new BugslayerStackTrace ( e ) ;            
            Console.WriteLine ( "\r\nShould only see a single exception frame" ) ;
            Console.WriteLine ( bst2.ToString ( ) ) ;
        }
        
        Console.WriteLine ( "\r\nShould only Main in stack." ) ;
        bst = new BugslayerStackTrace ( 7 ) ;
        Console.WriteLine ( bst.ToString ( ) ) ;
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
    
    [Conditional("RELEASE")]
    public static void ReleaseBuildWarning ( )
    {
        Console.Write ( "If this is a release build, StackTrace " ) ;
        Console.Write ( "does not return all the frames so the " ) ;
        Console.WriteLine ( "output is wrong.\r\n\r\n" ) ;
    }
    
    public static void Main ( )
    {
        ReleaseBuildWarning ( ) ;
        Foo ( 1 ) ;
    }
}
