''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''
' Debugging Applications for Microsoft .NET and Microsoft Windows
' Copyright © 1997 - 2003 John Robbins -- All rights reserved.
''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''
Imports EnvDTE
Imports System.Diagnostics

Public Module Utilities

    ' My wrapper around the CommandWindow to make it easier to use.
    Public Class CmdWindow
        ' The internal Command window.
        Private m_CmdWnd As CommandWindow

        ' The constructor that simply gets the command window.
        Public Sub New()
            m_CmdWnd = DTE.Windows.Item(EnvDTE.Constants.vsWindowKindCommandWindow).Object
        End Sub

        ' Public method to write a line with a CRLF appended.
        Public Sub WriteLine(Optional ByVal Str As String = "")
            m_CmdWnd.OutputString(Str + vbCrLf)
        End Sub

        ' Public method to write a line.
        Public Sub Write(ByVal Str As String)
            m_CmdWnd.OutputString(Str)
        End Sub

        ' Clears the command window.
        Public Sub Clear()
            m_CmdWnd.Clear()
        End Sub

        ' Sends the input to the command window like you typed it.
        Public Sub SendInput(ByVal Command As String, Optional ByVal Commit As Boolean = True)
            m_CmdWnd.SendInput(Command, Commit)
        End Sub

    End Class

    Public Class OutputPane

        ' The output pane this class wraps.
        Private m_OutPane As OutputWindowPane

        ' The class constructor.
        Public Sub New(ByVal Name As String, _
                       Optional ByVal ShowIt As Boolean = True)

            ' First, get the main output window itself.
            Dim Win As Window = _
                          DTE.Windows.Item(EnvDTE.Constants.vsWindowKindOutput)
            ' Show the window if I'm supposed to.
            If (True = ShowIt) Then
                Win.Visible = True
            End If

            ' Convert the window to a real output window.  
            ' The VB way of casting!
            Dim OutWin As OutputWindow = Win.Object

            ' Use exception handling to access this pane if it already exists.
            Try
                m_OutPane = OutWin.OutputWindowPanes.Item(Name)
            Catch e As System.Exception
                ' This output tab doesn't exist, so create it.
                m_OutPane = OutWin.OutputWindowPanes.Add(Name)
            End Try

            ' If it's supposed to be visible, make it so.
            If (True = ShowIt) Then
                m_OutPane.Activate()
            End If
        End Sub

        ' Allows access to the value itself.
        Public ReadOnly Property OutPane() As OutputWindowPane
            Get
                Return m_OutPane
            End Get
        End Property

        ' Wrapper.  Get the underlying object for all the rest 
        ' of the OutputWindowPane methods.
        Public Sub Clear()
            m_OutPane.Clear()
        End Sub

        ' The functions I wanted to add.
        Public Sub WriteLine(ByVal Text As String)
            m_OutPane.OutputString(Text + vbCrLf)
        End Sub

    End Class



    ' SplitParams - Splits up a parameter string passed to a macro 
    ' into an array of parameters.
    ' 
    ' For whatever reason, you can't have any spaces in parameter 
    ' lists.  Consequently, you can only pass parameters like the
    ' following in the Command window:
    '   SplitParams x,y,x
    '       Raw parameter string : x,y,z
    '       Parameter length     : 5
    '   SplitParams x,"a string",y
    '       Raw parameter string : x,a string,y
    '       Parameter length     : 12
    ' Notice you can't have any spaces unless the string is 
    ' surrounded by quotes.  Also notice that quotes don't 
    ' get passed into the macro as well.  The upshot is that
    ' you can't pass in strings with quotes!
    ' Also, notice you can't use any spaces between parameters
    ' either.
    Function SplitParams(ByVal ParamStr As String) As String()
#If SPLITPARAMSDEBUG Then
        ' Gotta start with the Command window.
        Dim cw As CmdWindow = New CmdWindow()

        ' Show the raw input.
        cw.WriteLine("Raw parameter string : " & ParamStr)
        cw.WriteLine("Parameter length     : " & ParamStr.Length)
#End If

        ' No sense doing anything if no parameters.
        If (0 = ParamStr.Length) Then
            Exit Function
        End If

        ' The array that'll hold the individual parameters.
        Dim StrSplitArray As String()

        ' Do the splitter!
        StrSplitArray = ParamStr.Split(","c)

#If SPLITPARAMSDEBUG Then
        ' Verify each string is a string.
        Dim i As Integer
        For i = 0 To StrSplitArray.Length - 1
            If (0 = StrSplitArray(i).Length) Then
                cw.WriteLine("PARAM " & i & " is empty!!")
            End If
        Next

        For i = 0 To StrSplitArray.Length - 1
            cw.WriteLine("     Param " & i & " = " & StrSplitArray(i))
        Next
#End If

        SplitParams = StrSplitArray

    End Function


End Module
