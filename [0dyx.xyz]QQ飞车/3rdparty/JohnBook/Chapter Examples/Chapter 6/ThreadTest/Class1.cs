/*----------------------------------------------------------------------
Debugging Applications for Microsoft .NET and Microsoft Windows
Copyright © 1997-2003 John Robbins -- All rights reserved.
----------------------------------------------------------------------*/
using System ;
using System.Threading ;
using System.Diagnostics ;

namespace ThreadTest
{

class ThreadO
{
    public void ThreadMethod ( )
    {
        StepThrough ( ) ;
        Hidden ( ) ;
        for ( Int32 i = 0 ; i < 10 ; i++ )
        {
            Thread.Sleep ( 100 ) ;
            Console.WriteLine ( "ThreadMethod func : {0}" , i ) ;
        }
    }
    
    [DebuggerStepThrough]
    public void StepThrough ( )
    {
        for ( Int32 i = 0 ; i < 10 ; i++ )
        {
            Console.WriteLine ( "StepThrough func : {0}" , i ) ;
        }
    }
    
    [DebuggerHidden]
    public void Hidden ( )
    {
        for ( Int32 i = 0 ; i < 10 ; i++ )
        {
            Console.WriteLine ( "Hidden func : {0}" , i ) ;
        }
    }
    
}


class App
{
	static void Main(string[] args)
	{
	    ThreadO x = new ThreadO ( ) ;
	    
	    Thread t1 = null ;
	    Thread t2 = new Thread ( new ThreadStart ( x.ThreadMethod ) ) ;
        t1 = new Thread ( new ThreadStart ( x.ThreadMethod ) ) ;

        t1.Name = "Bob" ;
        t2.Name = "Joe" ;
	    
	    t1.Start ( ) ;
	    t2.Start ( ) ;
	    
        t1.Join ( ) ;
        t2.Join ( ) ;	
        
        t1 = null ;
        t2 = null ;    
        
        Main2 ( Thread.CurrentThread ) ;
	}
	
    static int count = 0;

    public static void Main2 ( Thread t )
    {
        WaitCallback callback = new WaitCallback (ProcessRequest);

        ThreadPool.QueueUserWorkItem (callback);
        ThreadPool.QueueUserWorkItem (callback);
        ThreadPool.QueueUserWorkItem (callback);
        ThreadPool.QueueUserWorkItem (callback);
        ThreadPool.QueueUserWorkItem (callback);
        ThreadPool.QueueUserWorkItem (callback);
        ThreadPool.QueueUserWorkItem (callback);
        ThreadPool.QueueUserWorkItem (callback);
        ThreadPool.QueueUserWorkItem (callback);
        ThreadPool.QueueUserWorkItem (callback);

        Thread.Sleep (5000); // Give the requests a chance to execute
    }

    static void ProcessRequest (object state)
    {
        int n = Interlocked.Increment (ref count);        
        Console.WriteLine (n);
    }
	
}
}
