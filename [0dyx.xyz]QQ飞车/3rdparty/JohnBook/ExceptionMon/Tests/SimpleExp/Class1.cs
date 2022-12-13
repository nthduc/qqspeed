using System;

namespace SimpleExp
{
	/// <summary>
	/// Summary description for Class1.
	/// </summary>
	class Class1
	{
	    static void Whew ( )
	    {
            throw new Exception ( "Message" ) ;
        }
	    
	    static void Foo ( )
	    {
	        try
	        {
	            throw new ArgumentException ( "Message" , "ParamName" ) ;
	        }
	        catch ( Exception )
	        {
	        }
	        Whew ( ) ;
        }
        
        static void Bar ( )
        {
            Foo ( ) ;
        }
        
        static void Baz ( )
        {
            Bar ( ) ;
        }
        
        static void Boing ( )
        {
            Baz ( ) ;
        }
        
        static void Bonk ( )
        {
            Boing ( ) ;
        }
        
        static void Bling ( )
        {
            Bonk ( ) ;
        }
        
        static void Bam ( )
        {
            Bling ( ) ;
        }
        
        static void Bloop ( )
        {
            Bam ( ) ;
        }
        
        static void Bap ( )
        {
            Bloop ( ) ;
        }
        
        static void Blork ( )
        {
            Bap ( ) ;
        }
        
        static void Bleech ( )
        {
            Blork ( ) ;
        }
        
        /// <summary>
		/// The main entry point for the application.
		/// </summary>
		[STAThread]
		static void Main(string[] args)
		{
		    try
		    {
		        Bleech ( ) ;
		    }
		    catch ( Exception /*e*/ )
		    {
		        //Console.WriteLine ( e.Message ) ;
		    }
		}
	}
}
