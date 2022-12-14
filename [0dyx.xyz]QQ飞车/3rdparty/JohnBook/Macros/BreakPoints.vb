'''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''
' Debugging Applications for Microsoft .NET and Microsoft Windows
' Copyright © 1997 - 2003 John Robbins -- All rights reserved.
'''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''
' BreakPoints Module
'
' John Robbins - Wintellect - http://www.wintellect.com
'
' A module that will set and clear breakpoints at the entry point of 
' all functions in the current source file.  What's even cooler is 
' that this code will not screw up breakpoints you already have set!
' Additionally, when removing breakpoints, it will only remove the 
' breakpoints put for the current source file.
'
' There are some caveats:
' 0.  The breakpoints set by the SetBreakpointsOnAllCurrentDocFunctions 
'     macro show up as you'd expect in the source windows as a red dot
'     next to the line where they were set.  However, you can click on 
'     that dot all day long as it will not clear it.  Either run
'     RemoveBreakpointsOnAllCurrentDocFunctions or clear them from the
'     Breakpoints window.  This seems to be a bug in the IDE.
' 1.  There's a bug in the CodeModel for C++ header files.  Pretty much
'     anything in one gets called a function.  There's no clean way to
'     double check, short of parsing the file yourself, if the 
'     TextPoint values are real.  If you run this on a header, you'll
'     get breakpoints all over the place.  Fortunately, the debugger 
'     is smart enough to ignore them.
' 2.  The breakpoints are set at what the CodeElement.StartPoint 
'     property says is the first line.  This can be at the start of an 
'     attribute or something.  Don't worry, the debugger does the right 
'     thing and moves the breakpoint to the first executable line 
'     inside the function.  (Go Microsoft!)  If a .NET method is empty,
'     the breakpoint is set on the end of the function.
' 3.  There's an odd bug you might run into when debugging this code.
'     After setting the breakpoint, I access it to set the Tag field so
'     I can identify which breakpoints this macro set.  When debugging,
'     that access seems to cause a Null Reference exception in some 
'     cases.  However, if you don't set breakpoints, it will run fine.
' 4.  In some C++ source files, the CodeModel occasionally does not 
'     have a function or two that's shown in the code window.  Since 
'     you can't get them, you can't set breakpoints on them.
' 5.  The active document returned from DTE.ActiveDocument is odd.  
'     It's the last code document that had focus.  This can mean you're
'     looking at the Start Page, but setting breakpoints on something 
'     hidden.  These macros force you to have the cursor in a real code
'     window before they will run.
' 
' Version 1.0 - August 28, 2002
'''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''

Imports EnvDTE
Imports System.Diagnostics
Imports System.Collections

Public Module BreakPoints

    Const k_ConstantTagVal As String = "Wintellect Rocks "

    Public Sub SetBreakpointsOnAllCurrentDocFunctions()

        ' Get the current source file name doing all the checking.
        Dim CurrDoc As Document = GetCurrentDocument()
        If (CurrDoc Is Nothing) Then
            Exit Sub
        End If

        ' Get the source file name and build up the tag value.
        Dim SrcFile As String = CurrDoc.FullName
        Dim TagValue As String = BuildTagValue(CurrDoc)

        ' While I might have a document, I still need to check this is
        ' one I can get a code model from.
        Dim FileMod As FileCodeModel = _
                                 CurrDoc.ProjectItem.FileCodeModel
        If (FileMod Is Nothing) Then
            MsgBox("Unable to get code model from document.", _
                    MsgBoxStyle.OKOnly, _
                    k_ConstantTagVal)
            Exit Sub
        End If

        ' Everything's lined up to enumerate!
        ProcessCodeElements(FileMod.CodeElements, SrcFile, TagValue)

    End Sub

    Private Sub ProcessCodeElements(ByVal Elems As CodeElements, _
                                    ByVal SrcFile As String, _
                                    ByVal TagValue As String)

        ' Look at each item in this collection.
        Dim CurrElem As CodeElement
        For Each CurrElem In Elems

            ' If I'm looking at a class, struct or namespace, I need 
            ' to recurse.
            If (vsCMElement.vsCMElementNamespace = CurrElem.Kind) Or _
                (vsCMElement.vsCMElementClass = CurrElem.Kind) Or _
                (vsCMElement.vsCMElementStruct = CurrElem.Kind) Then

                ' This is kinda odd.  Some CodeElements use a Children 
                ' property to get sub elements while others use 
                ' Members.
                Dim SubCodeElems As CodeElements = Nothing

                Try
                    SubCodeElems = CurrElem.Children
                Catch
                    Try
                        SubCodeElems = CurrElem.Members
                    Catch
                        SubCodeElems = Nothing
                    End Try
                End Try

                If (Not (SubCodeElems Is Nothing)) Then
                    If (SubCodeElems.Count > 0) Then
                        ProcessCodeElements(SubCodeElems, _
                                            SrcFile, _
                                            TagValue)
                    End If
                End If
            ElseIf (CurrElem.Kind = _
                        vsCMElement.vsCMElementFunction) Or _
                   (CurrElem.Kind = _
                        vsCMElement.vsCMElementProperty) Then
                ' Interestingly, Attributed COM component attributes 
                ' show up broken out into their functions.  The only
                ' thing is that their StartPoint property is invalid
                ' and throws an exception when accessed.
                Dim TxtPt As TextPoint
                Try
                    TxtPt = CurrElem.StartPoint
                Catch
                    TxtPt = Nothing
                End Try

                If (Not (TxtPt Is Nothing)) Then
                    Dim LineNum As Long = TxtPt.Line
                    Dim Bps As EnvDTE.Breakpoints

                    ' Plop in one of my breakpoints.
                    Bps = DTE.Debugger.Breakpoints.Add(File:=SrcFile, _
                                                      Line:=LineNum)
                    ' Get the BP from the collection and set the tag
                    ' property so I can find the ones I set.
                    Try
                        ' There's some sort of bug here.  If you debug
                        ' through this with the VSA debugger, it fails
                        ' (0x8004005's) on accessing the breakpoints 
                        ' collection occasionally.  However, if you 
                        ' run it, life is good. Whateva!
                        Dim Bp As EnvDTE.Breakpoint
                        For Each Bp In Bps
                            Bp.Tag = TagValue
                        Next
                    Catch
                    End Try
                End If
            End If
        Next

    End Sub

    Public Sub RemoveBreakpointsOnAllCurrentDocFunctions()
        ' This is a much simpler function since I set the tag value on
        ' the breakpoints, I can remove them simply by screaming 
        ' through all BPs and removing those.
        Dim CurrDoc As Document = GetCurrentDocument()
        If (CurrDoc Is Nothing) Then
            Exit Sub
        End If

        Dim TagValue As String = BuildTagValue(CurrDoc)

        Dim CurrBP As EnvDTE.Breakpoint
        For Each CurrBP In DTE.Debugger.Breakpoints
            If (CurrBP.Tag = TagValue) Then
                CurrBP.Delete()
            End If
        Next
    End Sub

    Private Function GetCurrentDocument() As Document
        ' Check to see if a project or solution is open.  If not, you
        ' can't get at the code model for the file.
        Dim Projs As System.Array = DTE.ActiveSolutionProjects
        If (Projs.Length = 0) Then
            MsgBox("You must have a project open.", _
                    MsgBoxStyle.OKOnly, _
                    k_ConstantTagVal)
            GetCurrentDocument = Nothing
            Exit Function
        End If

        ' Getting the active document is a little odd.  
        ' DTE.ActiveDocument will return the active code document, but
        ' it might not be the real ACTIVE window.  It's quite 
        ' disconcerting to see macros working on a document when you're
        ' looking at the Start Page.  Anyway, I'll ensure the active 
        ' document is really the active window.
        Dim CurrWin As Window = DTE.ActiveWindow
        Dim CurrWinDoc As Document = CurrWin.Document
        Dim CurrDoc As Document = DTE.ActiveDocument

        ' Gotta play the game to keep from null ref exceptions in the 
        ' real active doc check below.
        Dim WinDocName As String = ""
        If Not (CurrWinDoc Is Nothing) Then
            WinDocName = CurrWinDoc.Name
        End If
        Dim DocName As String = "x"
        If Not (CurrDoc Is Nothing) Then
            DocName = CurrDoc.Name
        End If

        If ((CurrWinDoc Is Nothing) And _
            (WinDocName <> DocName)) Then
            MsgBox("The active cursor is not in a code document.", _
                    MsgBoxStyle.OKOnly, _
                    k_ConstantTagVal)
            GetCurrentDocument = Nothing
            Exit Function
        End If

        ' While I might have a document, I still need to check this is
        ' one I can get a code model from.
        Dim FileMod As FileCodeModel = _
                                   CurrDoc.ProjectItem.FileCodeModel
        If (FileMod Is Nothing) Then
            MsgBox("Unable to get code model from document.", _
                    MsgBoxStyle.OKOnly, _
                    k_ConstantTagVal)
            GetCurrentDocument = Nothing
            Exit Function
        End If

        GetCurrentDocument = CurrDoc
    End Function

    Private Function BuildTagValue(ByVal Doc As Document) As String
        BuildTagValue = k_ConstantTagVal + Doc.FullName
    End Function

End Module
