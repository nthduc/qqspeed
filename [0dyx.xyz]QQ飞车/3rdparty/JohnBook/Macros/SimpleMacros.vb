''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''
' Debugging Applications for Microsoft .NET and Microsoft Windows
' Copyright © 1997 - 2003 John Robbins -- All rights reserved.
''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''
' Simple macros to demonstrate how to get things working.
''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''

Option Strict Off
Option Explicit On 

Imports EnvDTE
Imports Microsoft.VisualBasic
Imports System.Diagnostics
Imports System.Array
Imports System.Text
Imports System.Text.RegularExpressions
Imports Microsoft.VisualStudio.VCCodeModel

Public Module SimpleMacros

    Sub HelloWorld()
        MsgBox("Hello world!  This is my first macro!")
    End Sub

    Sub HelloWorld2()
        Dim ow As CommandWindow
        ow = DTE.Windows.Item(EnvDTE.Constants.vsWindowKindCommandWindow).Object
        ow.OutputString("Hello world!  This is output to the Command window." + vbCrLf)
    End Sub


    Sub ParamMacro(Optional ByVal Params As String = "")

        ' The Command window.
        Dim cw As CmdWindow = New CmdWindow()

        cw.WriteLine("Raw parameter string : " + Params)

        ' Get the parameters
        Dim StrSplitArray As String() = SplitParams(Params)

        ' If there are some parameter, show them.
        If (Not (StrSplitArray Is Nothing)) Then
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
        End If

    End Sub

    ' Dumps all the possible commands into the Command window.
    Sub DumpAllCommands()
        Dim Cmd As Command
        Dim ow As OutputPane = New OutputPane("Dumped Commands")

        Dim ArraySize As Integer
        ArraySize = 500
        Dim StrArray(500) As String

        ow.WriteLine("Grinding through all the commands.")
        ' Grind through the commands
        Dim i As Integer = 0
        For Each Cmd In DTE.Commands
            If (Cmd.Name <> "") Then
                StrArray(i) = Cmd.Name + " (id:" + Cmd.ID.ToString() + ")"
                i = i + 1
                If (0 = (i Mod ArraySize)) Then
                    ow.WriteLine("Redimensioning the array.")
                    ArraySize = ArraySize + 500
                    ReDim Preserve StrArray(ArraySize)
                End If
            End If
        Next

        ' Throw out any extra.
        ReDim Preserve StrArray(i - 1)

        ' Sort the output.
        ow.WriteLine("Sorting the array.")
        StrArray.Sort(StrArray)

        ' Clear anything in the OutputPane window.
        ow.Clear()

        ' Dump 'em.
        Dim Str As String
        For Each Str In StrArray
            ow.WriteLine(Str)
        Next

    End Sub

    ' Dumps all the code elements for the open document of a project.
    Public Sub DumpActiveDocCodeElements()
        ' Where the output goes.  Note that the OutputPane class comes
        ' from the Utilities macro project.
        Dim ow As OutputPane = New OutputPane("Open Doc Code Elements")
        ' Clear the output pane.
        ow.Clear()

        ' See if there's a document open.
        Dim Doc As Document = DTE.ActiveDocument
        If (Doc Is Nothing) Then
            ow.WriteLine("No open document")
            Exit Sub
        End If

        ' Get the code model for the doc.  You have to get the project
        ' item to diddle down to the code elements
        Dim FileMod As FileCodeModel = Doc.ProjectItem.FileCodeModel

        If (Not (FileMod Is Nothing)) Then
            DumpElements(ow, FileMod.CodeElements, 0)
        Else
            ow.WriteLine("Unable to get the FileCodeModel!")
        End If
    End Sub

    Private Sub DumpElements(ByVal ow As OutputPane, _
                                ByVal Elems As CodeElements, _
                                ByVal Level As Integer)

        Dim Elem As CodeElement
        For Each Elem In Elems

            Dim i As Integer = 0

            While (i < Level)
                ow.OutPane.OutputString("  ")
                i = i + 1
            End While

            ' If there's an exception accessing the FullName property,
            ' it's probably an unnamed parameter.
            Dim sName As String
            Try
                sName = Elem.FullName
            Catch e As System.Exception
                sName = "'Empty Name'"
            End Try
            ow.WriteLine(sName + "(" + Elem.Kind.ToString() + ")")


            ' This is kinda weird.  Some CodeElements use a Children property
            ' to get sub elements while others use Members.  Then again,
            ' functions use the Parameters property.  
            Dim SubCodeElems As CodeElements = Nothing

            Try
                SubCodeElems = Elem.Children
            Catch
                Try
                    SubCodeElems = Elem.Members
                Catch
                    If (TypeOf Elem Is CodeFunction) Then
                        SubCodeElems = Elem.Parameters
                    Else
                        SubCodeElems = Nothing
                    End If
                End Try
            End Try

            If (Not (SubCodeElems Is Nothing)) Then
                If (SubCodeElems.Count > 0) Then
                    DumpElements(ow, SubCodeElems, Level + 1)
                End If
            End If
        Next
    End Sub

End Module

