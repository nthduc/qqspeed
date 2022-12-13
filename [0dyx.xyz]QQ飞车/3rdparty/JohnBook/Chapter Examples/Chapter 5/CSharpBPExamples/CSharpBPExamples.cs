/*----------------------------------------------------------------------
Debugging Applications for Microsoft .NET and Microsoft Windows
Copyright © 1997-2003 John Robbins -- All rights reserved.
----------------------------------------------------------------------*/
using System;
using System.Threading ;

namespace CSharpBPExamples
{

    class SomethingToDo 
    {
        public void Do ( Int32 iParam , Int32 iAnotherParam )
        {
            iAnotherParam = 6 ;
            int jLocal = iParam ;
            Re ( 3.145 * jLocal ) ;
        }
        
        public void Re ( double d )
        {
            d = 0.667 ;
            double eLocal = 0.002 ;
            eLocal = eLocal * d ;
            Mi ( "A me so la ti do" ) ;
        }
        
        public void Mi ( String s ) 
        {
            s = "Hello from Mi" ;
            Fa ( ) ;
        }
        
        public void Fa ( ) 
        {
            So ( ) ;
        }
        
        public void So ( )
        {
            La ( ) ;
        }
        
        public void La ( )
        {
            Ti ( ) ;
        }
        
        public void Ti ( )
        {
            Doh ( ) ;
        }
        
        public void Doh ( )
        {
            Console.WriteLine ( "Hit da Doh!" ) ;
        }
        
        public int Overload ( int i )
        {
            return ( i ) ;
        }
        
        public char Overload ( char c )
        {
            return ( c ) ;
        }
        
        public void ThreadFunc ( )
        {
            for ( int i = 0 ; i < 20 ; i++ )
            {
                Thread.Sleep ( 500 ) ;
            }
            //Console.WriteLine ( "Thread done!" ) ;
        }
        
        public bool CallFromDebugger ( bool r )
        {
            Console.WriteLine ( "CallFromDebugger called" ) ;
            return ( r ) ;
        }
        
        public void BigOlForLoop ( )
        {
        /*82*/    int i ;
        /*83*/    int m ;
        /*84*/    for ( i = 0 , m = 0 ; i < 10 ; i++ , m-- )
        /*85*/    {
        /*86*/        Console.WriteLine ( "i = {0} m = {0}" , i , m ) ;
        /*87*/    }
        }
        
        
        private String m_Str = "SleepString" ;
        
        public String SleepString
        {
            get
            {
                Thread.Sleep ( 10000000 ) ;
                return ( m_Str ) ;
            }
        }
    }       
    
    class MyThreadClass
    {
        public void ThreadFunc ( )
        {
        }
    } 

	/// <summary>
	/// Summary description for Class1.
	/// </summary>
	class DaMain
	{
		/// <summary>
		/// The main entry point for the application.
		/// </summary>
		[STAThread]
		static void Main(string[] args)
		{
            //Console.WriteLine ( "Starting the program!!" ) ;
		    
            Thread.CurrentThread.Name = "Bob" ;
            
            SomethingToDo x = new SomethingToDo ( ) ;
		    
            x.Do ( 0x42 , 0x666 ) ;
		    
            //x.Mi ( x.RetString ( ) ) ;
		    
            x.Overload ( 1 ) ;
            
            x.BigOlForLoop ( ) ;
            
            AnotherFile y = new AnotherFile ( ) ;
		    
            y.OverHere ( 2 ) ;
		    
		    //Console.WriteLine ( "About to start threads!" ) ;
            Thread t1 = new Thread ( new ThreadStart ( x.ThreadFunc ) ) ;
            Thread t2 = new Thread ( new ThreadStart ( x.ThreadFunc ) ) ;
                        
            t1.Start ( ) ;
            t2.Start ( ) ;
			
            Thread.Sleep ( 5000 ) ;
            
            t1.Abort ( ) ;
        }
	}
}
