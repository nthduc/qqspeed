/*----------------------------------------------------------------------
Debugging Applications for Microsoft .NET and Microsoft Windows
Copyright © 1997-2003 John Robbins -- All rights reserved.
----------------------------------------------------------------------*/
using System ;
using System.Diagnostics ;

namespace ExceptApp
{
    enum Days { Sat = 1 , Sun , Mon , Tue , Wed , Thu , Fri } ;
    
    class DoSomething
    {
        public void Doh ( String StrParam , Days ValueParam )
        {
            Console.WriteLine ( StrParam ) ;
            Console.WriteLine ( ValueParam.ToString ( ) ) ;
            throw new ArgumentException ( "Thowing an exception" , "x" ) ;
        }
        
        public void Reh ( int i , String StrParam )
        {
            Console.WriteLine ( "Reh = " + i ) ;
            String s = "Tommy can you see me? " + StrParam ;
            Days e = Days.Fri ;
            Doh ( s , e ) ;
        }
        
        public void Mi ( Object o )
        {
            Console.WriteLine ( "Mi = " + o.ToString ( ) ) ;
            String LocalStr = "Can you see me?" ;
            Reh ( 0x42 , LocalStr ) ;
        }
        
        public void Fa ( )
        {
            Mi ( this ) ;
        }
    }
	class Class1
	{
		static void Main(string[] args)
		{
		    try
		    {
		        DoSomething x = new DoSomething ( ) ;
		        x.Fa ( ) ;
		    }
		    catch 
		    {
		    }
		}
	}
}
