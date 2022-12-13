/*----------------------------------------------------------------------
Debugging Applications for Microsoft .NET and Microsoft Windows
Copyright © 1997-2003 John Robbins -- All rights reserved.
----------------------------------------------------------------------*/
using System ;
using System.Threading ;

namespace ConditionalBP
{

class TestO
{
    public TestO ( )
    {
        m_CallCount = 0 ;
        m_bToggle = false ;
    }
    
    private Int32 m_CallCount ;
    
    public Int32 CondTest ( )
    {
        m_CallCount++ ;
        return ( m_CallCount ) ;
    }
    
    private Boolean m_bToggle ;
    
    public Boolean Toggle ( )
    {
        m_bToggle = !m_bToggle ;
        return ( m_bToggle ) ;
    }
}

class App
{
	static void Main(string[] args)
	{
		TestO x = new TestO ( ) ;
		
		for ( Int32 i = 0 ; i < 10 ; i++ )
		{
		    // Set BP: (x.Toggle() == true) || (x.CondTest() == 0 )
		    Console.WriteLine ( "{0}" , i ) ;
		}
		
		x = null ;
	}
}
}
