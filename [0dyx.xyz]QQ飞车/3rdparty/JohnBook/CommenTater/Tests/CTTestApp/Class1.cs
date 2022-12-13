using System;

namespace CTTestApp
{
    class CTTest
    {
        public CTTest ( int i )
        {
        }
        
        public void Method  ( )
        {
        }
        
        public void MethodB ( int x , int y )
        {
        }
        
        public int PubField ;
        
        private int PrivField ;
        
        public int PubProp
        { get { return 0 ; } }
        
        private int PrivProp
        { get { return 0 ; } }

        public static int StaticPubProp
        { get { return 0 ; } }
        
        public int this[ int i]
        { get { return 0 ; } }
        
        public static int operator +(CTTest o)
        { return 0 ; }
        
        public static int operator <<(CTTest x, int y)
        { return 0; }
    }
    
	class Class1
	{
		static void Main(string[] args)
		{
		}
	}
}
