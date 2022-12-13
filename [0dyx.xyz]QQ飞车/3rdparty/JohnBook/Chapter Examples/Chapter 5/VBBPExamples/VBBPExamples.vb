'-----------------------------------------------------------------------
' Debugging Applications for Microsoft .NET and Microsoft Windows
' Copyright © 1997-2003 John Robbins -- All rights reserved.
'-----------------------------------------------------------------------
imports System
imports System.Threading 

Module VBBPExamples

    class SomethingToDo 
        
        public Sub Doo ( iParam as Int32 , iAnotherParam as Int32 )
        
            iAnotherParam = 6 
            
            Dim jLocal as Int32 = iParam 
            
            Re ( 3.145 * jLocal ) 
        end sub 
        
        public sub Re ( d as Double )
            d = 0.667 
            dim eLocal as Double = 0.002 
            eLocal = eLocal * d 
            Mi ( "A me so la ti do" ) 
        end sub 
        
        public sub Mi ( s as string ) 
            s = "Hello from Mi" 
            Fa ( ) 
        end sub
        
        public sub Fa ( ) 
            So ( ) 
        end sub 
        
        public sub So ( )
            La ( ) 
        end sub
        
        public sub La ( )
            Ti ( ) 
        end sub
        
        public sub Ti ( )
            Doh ( ) 
        end sub
        
        public sub Doh ( )
            Console.WriteLine ( "Hit da Doh!" ) 
        end sub

        public function Overload ( i as Int32 ) as Int32 
            Overload = i 
        end function 
        
        public function Overload ( c as Char ) as Char 
            Overload = c 
        end function
        
        public sub ThreadFunc ( )
            Dim i as Int32 
            for i = 0 to 10
                Thread.Sleep ( 1000 ) 
            Next  
            Console.WriteLine ( "Thread done!" )
        end sub 
        
        private m_Str as String = "BeepString" 
        
        'public ReadOnly property  BeepString ( ) as String
        '    get
        '        dim i as Integer 
        '        for i = 1 to 3
        '            Beep ( ) 
        '            Thread.Sleep ( 100 ) 
        '        Next                
        '        return m_Str 
        '    End Get            
        'End Property

        Public ReadOnly Property WhereAmICalled() As String
            Get
                Return AppDomain.CurrentDomain.FriendlyName
            End Get
        End Property


        'Public Function ReturnStringParam(ByVal s As String) As String
        '    Dim i As Integer
        '    For i = 1 To 3
        '        'Beep()
        '        Thread.Sleep(20 * 1000)
        '    Next
        '    Return s
        'End Function

        Public Function RetString() As String
            RetString = "The return string..."
        End Function

        Public Sub TestOStr(ByVal s As String)
            Console.WriteLine(s)
        End Sub

    End Class


    Class MyThreadClass
        Public Sub ThreadFunc()

        End Sub
    End Class

    Sub Main()

        Thread.CurrentThread.Name = "Bob"

        Dim x As SomethingToDo = New SomethingToDo()

        x.Doo(42, 666)

        'x.Mi ( x.RetString ( ) ) 

        x.Overload(1)

        x.TestOStr(x.RetString())

        Dim y As AnotherFile = New AnotherFile()

        y.OverHere(2)

        'Console.WriteLine ( "About to start threads!" ) 
        Dim t1 As Thread = New Thread(AddressOf x.ThreadFunc)
        Dim t2 As Thread = New Thread(AddressOf x.ThreadFunc)

        t1.Start()
        t2.Start()

        Thread.Sleep(5000)

    End Sub

End Module
