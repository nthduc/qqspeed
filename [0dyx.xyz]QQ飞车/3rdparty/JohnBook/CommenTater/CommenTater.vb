''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''
' Debugging Applications for Microsoft .NET and Microsoft Windows
' Copyright © 1997 - 2003 John Robbins -- All rights reserved.
''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''
' CommenTater (The Cure for the Common Potato?)
'
' This module implements a set of routines to run through an active 
' C# file or a solution containing C# files and add the  CSC /doc 
' comments to each appropriate item.  Each item is added with a 
' "TODO" comment so it shows up in the Task List.  That helps the 
' developer to remember to go through and comment their code!
'
' May 31, 2001      - Version 1.0
' August 6, 2001    - Version 2.0 
'       - Added AddNoCommentTasksForSolution.
'       - Updated for Beta 2.
' March 8, 2002     - Version 3.0
'       - Updated for VS.NET V1 RTM.
'       - Added type name as part of comment.
' June 18, 2002     - Version 4.0
'       - Rewrote the portions dealing with the actual doc comments 
'         completely as I finally figured out how doc comments really
'         work.
'       - Comments are kept up to date and CommonTater will properly
'         check method parameters for existing doc comments to ensure
'         they are kept up to date.  If a parameter name has changed
'         or been removed, CommonTater still leaves the comment in, 
'         but marks it as needing to be removed.
' December 23, 2002 - Version 5.0
'       - Fixed problem handling "operator &", "operator >>", 
'         "operator <<", "operator >", "operator <", "operator <=", 
'         and "operator >=" since &, <, and > are special 
'         characters in XML.
'       - The UndoContext object is still broken if you attempt to use
'         it as a global undo.  However, if it can be used on each 
'         indovidual change.  While not perfect, it's worth something.
'       - The namespace DocComment property is still broken.
' 
' NOTE: These macros update the files, but do not save them.  If you
'       would like the macro to save the files, it's trivial to add.
'
' Caveats:
' This set of macros was originally written for build 7.0.9129 and 
' updated for VS.NET 2002 Beta 2, VS.NET 2002 V1 RTM, VS.NET 2003 
' Final Beta 7.1.2292  When you run through the file, you'll notice 
' several areas where I point out bugs in the extensability model.
' 1.  There's a big bug when using undo contexts.  In both public
'     methods, I tried setting up an undo context globally and found 
'     that when I used one, the code elements are not properly updated.  
'     Since I am pumping in text, the fact the code element line and 
'     column location isn't updated means all the comments are off!  
'     I can use the undo context on each change starting with 
'     VS.NET 2003.  While not perfect, at lease you can finally have 
'     some form of undo.
' 2.  The CodeNameSpace object has a bug where it's DocComment property
'     always returns an empty string.  Instead of messing around with
'     attempting to find it myself, I simply don't process namespaces.
'
' Notes:
' - One thing you may want to consider adding is an option to run 
'   through a function or property and look for throw statements so 
'   you could automatically add <exception> statements.  However, the
'   only way I can see to do get it to work was open all files
'   because only open files can be searched.  If you were really up
'   for a challenge, you'd look in the generated IL for a function for
'   throw instructions as well!!
'
''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''
Option Explicit On 
Imports EnvDTE
Imports System
Imports System.Diagnostics
Imports System.Text
Imports System.Xml
Imports System.Text.RegularExpressions
Imports System.Collections


Public Module CommenTater

    ' If set to true, ensures all function comments have a <remarks> section.
    ' If false, <remarks> sections in functions will be kept.
    Private m_FuncShowsRemarks As Boolean = True

    ' Enumerates an entire solution and adds the doc comments items 
    ' to all C# files.
    Public Sub AddNoCommentTasksForSolution()

        ' This is a bug.  Simply setting the undo context means that 
        ' any doc comment inserts will not properly update the 
        ' documents so everything shifts off.
        ' Note: this bug only occurs when you set it globally like 
        ' I am here. 
        'Dim UndoWasOpen As Boolean

        'If (True = DTE.UndoContext.IsOpen) Then
        '    UndoWasOpen = True
        'Else
        '    DTE.UndoContext.Open("Comment Undo", False)
        '    UndoWasOpen = False
        'End If

        Try
            ' Now I can enumerate the projects.  
            Dim Proj As Project
            For Each Proj In DTE.Solution.Projects
                ' Get the project's language.  If it's not a C# project, 
                ' I don't touch it.
                If (CodeModelLanguageConstants.vsCMLanguageCSharp = _
                                    Proj.CodeModel.Language) Then
                    ' It's a C# project so do each code file.
                    Dim ProjIt As ProjectItem
                    For Each ProjIt In Proj.ProjectItems
                        ' If the file is in the project, but missing on 
                        ' disk, accessing it's properties will cause an 
                        ' exception.  I'll handle it but still report it 
                        ' to the user.
                        Try
                            If (CodeModelLanguageConstants. _
                                            vsCMLanguageCSharp = _
                                            ProjIt.FileCodeModel.Language) Then

                                ' Stick this item in the status bar.
                                Dim OrigSBText As String
                                OrigSBText = ShowDocNameInStatusBar(ProjIt.Name)

                                Try

                                    Dim Elem As CodeElement
                                    For Each Elem In _
                                               ProjIt.FileCodeModel.CodeElements
                                        ' Do the work.
                                        RecurseCodeElements(Elem)
                                    Next
                                Catch ex As Exception
                                    MsgBox("EXCEPTION: " & ProjIt.Name & _
                                           " from " & Proj.Name & _
                                           " has an error : " & ex.Message, , _
                                           "CommenTater")
                                End Try

                                ' Reset the statusbar text.
                                RestoreStatusBar(OrigSBText)
                            End If
                        Catch ex As Exception
                            ' This exception meant there was a problem accessing 
                            ' the language model.  It happens all the time.
                        End Try
                    Next
                End If
            Next
        Finally
            'If (False = UndoWasOpen) Then
            '    DTE.UndoContext.Close()
            'End If
        End Try

        ShowAllItemsInTaskList()

    End Sub

    ' Takes care of adding tasks to do the comments for the currently 
    ' open file.
    Public Sub CurrentSourceFileAddNoCommentTasks()

        Dim CmdWin As CmdWindow = New CmdWindow

        ' Get the current document.
        Dim Doc As Document = DTE.ActiveDocument

        ' If no document is open, there's nothing to do.
        If (Doc Is Nothing) Then
            CmdWin.WriteLine("No open document")
            Exit Sub
        End If

        ' To get the document type, I'll get the docs project item.
        Dim FileCodeMod As FileCodeModel = Doc.ProjectItem.FileCodeModel

        ' With no code model, I can't go on.
        If (FileCodeMod Is Nothing) Then
            CmdWin.WriteLine("Active document is not a code document.")
            Exit Sub
        End If

        ' Check to see if the document is a C# file.  Unfortunately,
        ' Microsoft didn't see fit to offer the same functionality
        ' in VB.NET.  Bummer!

        If (CodeModelLanguageConstants.vsCMLanguageCSharp <> _
                                                FileCodeMod.Language) Then
            CmdWin.WriteLine("Unfortunately, " & Doc.Name & _
                              " is not a C# file.  Unable to continue.")
            Exit Sub
        End If

        ' I'm ready to rip.  To show that the editor is doing 
        ' something, I'll plop some text into the status bar to 
        ' indicate what's happening.
        Dim OrigSBText As String = ShowDocNameInStatusBar(Doc.Name)

        ' This is an bug.  Uncomment the undo context lines in this
        ' function and you'll see that by setting an undo context,
        ' the code system does not update the individual code start
        ' points!!  Therefore, all insertions are way off since
        ' inserting a single item will shift everything.
        ' This is only a bug when set globally like I am doing here.
        ' TODO - Is the undo context bug in fixed in a future release?

        'Dim UndoWasOpen As Boolean

        'If (True = DTE.UndoContext.IsOpen) Then
        '    UndoWasOpen = True
        'Else
        '    DTE.UndoContext.Open("Comment Undo", False)
        '    UndoWasOpen = False
        'End If

        ' Always protect with exception handling so I can close the
        ' undo element.
        Try
            ' It's a C# file so I can work with it.  
            ' Start enumerating the code elements from the top one.
            Dim Elem As CodeElement
            For Each Elem In FileCodeMod.CodeElements
                RecurseCodeElements(Elem)
            Next
        Catch ex As Exception
            MsgBox("EXCEPTION: " & Doc.Name & _
                   " from " & Doc.Name & _
                   " has an error : " & ex.Message & _
                   vbCrLf & ex.StackTrace, _
                   MsgBoxStyle.OKOnly, _
                   "CommenTater")

        Finally
            ' See the comment about the undo context above.
            ' TODO - Is the undo context bug fixed?
            ' Close my undo context.
            'If (False = UndoWasOpen) Then
            '    DTE.UndoContext.Close()
            'End If
        End Try

        ' Reset the status bar text.
        RestoreStatusBar(OrigSBText)

        ShowAllItemsInTaskList()

    End Sub

    ' Takes care of recursing all the code elements starting at Elem.
    Private Sub RecurseCodeElements(ByRef Elem As CodeElement)

        ' I found that the UndoContext works on each update individually, but
        ' won't work if you try to make it global so you can undo all changes 
        ' at once.
        Dim UndoWasOpen As Boolean
        If (True = DTE.UndoContext.IsOpen) Then
            UndoWasOpen = True
        Else
            DTE.UndoContext.Open("Comment Undo", False)
            UndoWasOpen = False
        End If

        Try
            ' I really wish all the various code elements that supported the
            ' DocComment property were derived off the same root so I didn't
            ' have to special case each one.  Bummer.
            ' Note that the NameSpace class has a bug where the doc comment
            ' is ALWAYS blank.
            Select Case Elem.Kind
                Case vsCMElement.vsCMElementFunction
                    Dim Func As CodeFunction = CType(Elem, CodeFunction)
                    If ("" = Func.DocComment) Then
                        Func.DocComment = BuildFunctionComment(Func)
                    Else
                        ProcessFunctionComment(Func)
                    End If
                Case vsCMElement.vsCMElementClass
                    Dim Cls As CodeClass = CType(Elem, CodeClass)
                    If ("" = Cls.DocComment) Then
                        Cls.DocComment = BuildClassComment(Cls.Name)
                    End If
                Case vsCMElement.vsCMElementProperty
                    Dim Prop As CodeProperty = CType(Elem, CodeElement)
                    If ("" = Prop.DocComment) Then
                        Prop.DocComment = SimpleSummaryComment(Prop.Name, _
                                                               "property")
                    End If
                Case vsCMElement.vsCMElementVariable
                    Dim Var As CodeVariable = CType(Elem, CodeVariable)
                    If ("" = Var.DocComment) Then
                        Var.DocComment = SimpleSummaryComment(Var.Name, _
                                                              "variable")
                    End If
                    'Case vsCMElement.vsCMElementNamespace
                    ' No use touching it as the DocComment will always be
                    ' empty.
                    'Dim NameSpc As CodeNamespace = CType(Elem, CodeNamespace)
                    'If ("" = NameSpc.DocComment) Then
                    '    NameSpc.DocComment = SimpleSummaryComment(NameSpc.Name, _
                    '                                              "namespace")
                    'End If
                Case vsCMElement.vsCMElementInterface
                    Dim Intf As CodeInterface = CType(Elem, CodeInterface)
                    If ("" = Intf.DocComment) Then
                        Intf.DocComment = SimpleSummaryComment(Intf.Name, _
                                                               "interface")
                    End If
                Case vsCMElement.vsCMElementDelegate
                    Dim Delg As CodeDelegate = CType(Elem, CodeDelegate)
                    If ("" = Delg.DocComment) Then
                        Delg.DocComment = SimpleSummaryComment(Delg.Name, _
                                                               "delegate")
                    End If
                Case vsCMElement.vsCMElementEnum
                    Dim Enm As CodeEnum = CType(Elem, CodeEnum)
                    If ("" = Enm.DocComment) Then
                        Enm.DocComment = SimpleSummaryComment(Enm.Name, _
                                                              "enum")
                    End If
                Case vsCMElement.vsCMElementStruct
                    Dim Struct As CodeStruct = CType(Elem, CodeStruct)
                    If ("" = Struct.DocComment) Then
                        Struct.DocComment = SimpleSummaryComment(Struct.Name, _
                                                                 "struct")
                    End If
                Case vsCMElement.vsCMElementTypeDef
                    Dim Type As CodeType = CType(Elem, CodeType)
                    If ("" = Type.DocComment) Then
                        Type.DocComment = SimpleSummaryComment(Type.Name, _
                                                               "typedef")
                    End If
            End Select
        Finally
            ' Close my undo context if necessary.
            If (False = UndoWasOpen) Then
                DTE.UndoContext.Close()
            End If
        End Try

        ' Prior to VS.NET 2002 V1, all CodeElement types supported the 
        ' Children property to get sub elements.  However, with the 
        ' VS.NET 2002 RTM build some use Children and some use Members.  
        ' What a mess.  Consequently, I need to do the ugly stuff below 
        ' to get the sub elements.  We don't need no stinkin' consistency!
        Dim SubCodeElems As CodeElements

        Try
            SubCodeElems = Elem.Children
        Catch
            Try
                SubCodeElems = Elem.Members
            Catch
                SubCodeElems = Nothing
            End Try
        End Try

        If (Not (SubCodeElems Is Nothing)) Then
            Dim ChildElem As CodeElement
            For Each ChildElem In SubCodeElems
                RecurseCodeElements(ChildElem)
            Next
        End If

    End Sub

    ' Simply adds <doc></doc> to finish off a doc comment.  I wish I had
    ' realized this was the magic of doc comments earlier.
    Private Function FinishOffDocComment(ByVal Str As String) As String
        Dim RawXML As New StringBuilder

        RawXML.AppendFormat("<doc>{0}{1}</doc>", vbCrLf, Str)

        FinishOffDocComment = RawXML.ToString()
    End Function

    ' Creates a simple summary node with the item name and type in it.
    Private Function BuildSummaryComment(ByVal VarItemName As String, _
                                         ByVal TypeName As String) As String
        Dim RawXML As New StringBuilder

        RawXML.AppendFormat("<summary>{0}TODO - Add {1} {2} " + _
                                            "summary comment{0}</summary>", _
                            vbCrLf, VarItemName, TypeName)

        BuildSummaryComment = RawXML.ToString()

    End Function

    ' Creates a complete doc comment for a simple summary statement.
    Private Function SimpleSummaryComment(ByVal VarItemName As String, _
                                          ByVal TypeName As String) As String
        SimpleSummaryComment = _
            FinishOffDocComment(BuildSummaryComment(VarItemName, TypeName))
    End Function

    ' Creates a class summary comment and also adds a remarks section 
    ' as you should do a longer section of text on the hows and whys of
    ' the class.
    Private Function BuildClassComment(ByVal ClsName As String) As String

        Dim RawXML As New StringBuilder

        RawXML.Append(BuildSummaryComment(ClsName, "class"))

        RawXML.AppendFormat("<remarks>{0}TODO - Add {1} class remarks " + _
                                                    "comment{0}</remarks>", _
                            vbCrLf, ClsName)

        BuildClassComment = FinishOffDocComment(RawXML.ToString())
    End Function

    ' Creates a function summary comment with any parameters and a 
    ' return type if appropriate.
    Private Function BuildFunctionComment(ByVal Func As CodeFunction) As String

        Dim RawXML As New StringBuilder

        ' Handle the special character processing.
        Dim FName As String = Func.Name
        Dim OperatorPos = InStr(Func.Name, "operator ")
        If (OperatorPos > 0) Then
            ' This function is an operator.  If that's the case, I need to 
            ' ensure none of the special characters end up in the mix.
            Dim BadCharPos = InStr(FName, "&")
            If (BadCharPos > 0) Then
                FName = Mid(FName, 1, BadCharPos - 1) + "&amp;" + _
                                Mid(FName, BadCharPos + 1)
            Else
                BadCharPos = InStr(FName, ">>")
                If (BadCharPos > 0) Then
                    FName = Mid(FName, 1, BadCharPos - 1) + "&gt;&gt;" + _
                                    Mid(FName, BadCharPos + 2)
                Else
                    BadCharPos = InStr(FName, "<<")
                    If (BadCharPos > 0) Then
                        FName = Mid(FName, 1, BadCharPos - 1) + "&lt;&lt;" + _
                                        Mid(FName, BadCharPos + 2)
                    Else
                        BadCharPos = InStr(FName, ">")
                        If (BadCharPos > 0) Then
                            FName = Mid(FName, 1, BadCharPos - 1) + "&gt;" + _
                                            Mid(FName, BadCharPos + 1)
                        Else
                            BadCharPos = InStr(FName, "<")
                            If (BadCharPos > 0) Then
                                FName = Mid(FName, 1, BadCharPos - 1) + "&lt;" + _
                                                Mid(FName, BadCharPos + 1)
                            End If
                        End If
                    End If
                End If
            End If
        End If

        ' Append the summary stuff.
        RawXML.Append(BuildSummaryComment(FName, "function"))
        RawXML.Append(vbCrLf)

        If (True = m_FuncShowsRemarks) Then

            RawXML.AppendFormat("<remarks>{0}TODO - Add {1} function remarks " + _
                                                        "comment{0}</remarks>", _
                                    vbCrLf, FName)
        End If

        If (Func.Parameters.Count > 0) Then
            Dim Elem As CodeElement
            For Each Elem In Func.Parameters
                RawXML.AppendFormat("<param name=""{0}"">{1}TODO - Add {0} " + _
                                            "parameter comment{1}</param>{1}", _
                                    Elem.Name, vbCrLf)
            Next
        End If

        If ("" <> Func.Type.AsFullName) Then
            RawXML.AppendFormat("<returns>{0}TODO - Add return comment{0}" + _
                                                                "</returns>{0}", _
                                vbCrLf)
        End If

        BuildFunctionComment = FinishOffDocComment(RawXML.ToString())

    End Function


    ' Does all the work to take an existing function comment and ensure
    ' that everything in it is kosher.  This might reorder your 
    ' comments, so you might want to change it.
    Private Sub ProcessFunctionComment(ByVal Func As CodeFunction)

        Debug.Assert("" <> Func.DocComment, """"" <> Func.DocComment")

        ' Holds the original doc comment.
        Dim XmlDocOrig As New XmlDocument
        ' I LOVE THIS!  By setting PreserveWhitespace to true, the 
        ' XmlDocument class will keep most of the formatting...
        XmlDocOrig.PreserveWhitespace = True
        XmlDocOrig.LoadXml(Func.DocComment)

        Dim RawXML As New StringBuilder

        ' Get the summary node.
        Dim Node As XmlNode
        Dim Nodes As XmlNodeList = XmlDocOrig.GetElementsByTagName("summary")

        If (0 = Nodes.Count) Then
            RawXML.Append(SimpleSummaryComment(Func.Name, "function"))
        Else
            RawXML.AppendFormat("<summary>{0}", vbCrLf)
            For Each Node In Nodes
                RawXML.AppendFormat("{0}{1}", Node.InnerXml, vbCrLf)
            Next
            RawXML.AppendFormat("</summary>{0}", vbCrLf)
        End If

        ' Get the remarks node.
        Nodes = XmlDocOrig.GetElementsByTagName("remarks")
        If (Nodes.Count > 0) Then
            RawXML.AppendFormat("<remarks>{0}", vbCrLf)
            For Each Node In Nodes
                RawXML.AppendFormat("{0}{1}", Node.InnerXml, vbCrLf)
            Next
            RawXML.AppendFormat("</remarks>{0}", vbCrLf)
        ElseIf (True = m_FuncShowsRemarks) Then
            RawXML.AppendFormat("<remarks>{0}TODO - Add {1} function " + _
                                    "remarks comment{0}</remarks>", _
                                vbCrLf, Func.Name)
        End If

        ' Get any parameters described in the doc comments.
        Nodes = XmlDocOrig.GetElementsByTagName("param")

        ' Does the function have parameters?
        If (0 <> Func.Parameters.Count) Then

            ' Slap any existing doc comment params into a hash table with
            ' the parameter name as the key.
            Dim ExistHash As New Hashtable
            For Each Node In Nodes
                Dim ParamName As String
                Dim ParamText As String
                ParamName = Node.Attributes("name").InnerXml
                ParamText = Node.InnerText
                ExistHash.Add(ParamName, ParamText)
            Next

            ' Loop through the parameters.
            Dim Elem As CodeElement
            For Each Elem In Func.Parameters
                ' Is this one in the hash of previous filled in params?
                If (True = ExistHash.ContainsKey(Elem.Name)) Then
                    RawXML.AppendFormat("<param name=""{0}"">{1}{2}{1}" + _
                                                                "</param>{1}", _
                                          Elem.Name, _
                                          vbCrLf, _
                                          ExistHash(Elem.Name))
                    ' Get rid of this key.
                    ExistHash.Remove(Elem.Name)
                Else
                    ' A new parameter was added.
                    RawXML.AppendFormat("<param name=""{0}"">{1}TODO - Add " + _
                                        "{0} parameter comment{1}</param>{1}", _
                                         Elem.Name, vbCrLf)
                End If
            Next

            ' If there is anything left in the hash table, a param 
            ' was eitherremoved or renamed.  I'll add the remaining 
            ' with TODOs so the user can do the manual deletion.
            If (ExistHash.Count > 0) Then
                Dim KeyStr As String
                For Each KeyStr In ExistHash.Keys
                    Dim Desc = ExistHash(KeyStr)
                    RawXML.AppendFormat("<param name=""{0}"">{1}{2}{1}{3}" + _
                                                            "{1}</param>{1}", _
                                          KeyStr, _
                                          vbCrLf, _
                                          Desc, _
                                          "TODO - Remove param tag")
                Next
            End If
        End If

        ' Take care of returns if necessary.
        If ("" <> Func.Type.AsFullName) Then
            Nodes = XmlDocOrig.GetElementsByTagName("returns")
            ' Do any returns nodes.
            If (0 = Nodes.Count) Then
                RawXML.AppendFormat("<returns>{0}TODO - Add return comment" + _
                                                          "{0}</returns>{0}", _
                                    vbCrLf)
            Else
                RawXML.AppendFormat("<returns>{0}", vbCrLf)

                For Each Node In Nodes
                    RawXML.AppendFormat("{0}{1}", Node.InnerXml, vbCrLf)
                Next

                RawXML.AppendFormat("</returns>{0}", vbCrLf)
            End If
        End If

        ' Do any example nodes.
        Nodes = XmlDocOrig.GetElementsByTagName("example")
        If (Nodes.Count > 0) Then
            RawXML.AppendFormat("<example>{0}", vbCrLf)
            For Each Node In Nodes
                RawXML.AppendFormat("{0}{1}", Node.InnerXml, vbCrLf)
            Next
            RawXML.AppendFormat("</example>{0}", vbCrLf)
        End If

        ' Do any permission nodes.
        Nodes = XmlDocOrig.GetElementsByTagName("permission")
        If (Nodes.Count > 0) Then
            For Each Node In Nodes
                RawXML.AppendFormat("<permission cref=""{0}"">{1}", _
                                    Node.Attributes("cref").InnerText, _
                                    vbCrLf)
                RawXML.AppendFormat("{0}{1}", Node.InnerXml, vbCrLf)
                RawXML.AppendFormat("</permission>{0}", vbCrLf)
            Next
        End If

        ' Finally exceptions.
        Nodes = XmlDocOrig.GetElementsByTagName("exception")

        If (Nodes.Count > 0) Then
            For Each Node In Nodes
                RawXML.AppendFormat("<exception cref=""{0}"">{1}", _
                                    Node.Attributes("cref").InnerText, _
                                    vbCrLf)
                RawXML.AppendFormat("{0}{1}", Node.InnerXml, vbCrLf)
                RawXML.AppendFormat("</exception>{0}", vbCrLf)
            Next
        End If
        Func.DocComment = FinishOffDocComment(RawXML.ToString())
    End Sub

    Private Function ShowDocNameInStatusBar(ByVal NewText As String)
        Dim StatBar As StatusBar = DTE.StatusBar
        Dim OrigSBText As String = StatBar.Text

        StatBar.Text = "Adding comments for " & NewText & "..."

        ShowDocNameInStatusBar = OrigSBText

    End Function

    Private Sub RestoreStatusBar(ByVal OrigText As String)
        Dim StatBar As StatusBar = DTE.StatusBar
        StatBar.Text = OrigText
    End Sub

    Private Sub ShowAllItemsInTaskList()
        ' Save the active window.
        Dim CurrAct As Window = DTE.ActiveWindow()
        ' Get the task list.
        Dim win As Window = DTE.Windows.Item(Constants.vsWindowKindTaskList)
        ' Make the task list active.
        win.Activate()
        ' Force the view to show all items so the TODO's show up.
        DTE.ExecuteCommand("View.All")
        ' Set the window back.
        CurrAct.Activate()
    End Sub

    ' Originally, I actually jammed the text in using EditPoints.  Once
    ' I figured out that you could write to the DocComment property
    ' by surrounding the comment with <doc></doc>, I shifted to that.
    ' I've left these here in case you wanted to see how to do manual
    ' insertion.
    '' Does the work to handle a function comment.
    'Private Sub InsertFunctionComment(ByRef Func As CodeFunction)

    '    ' Stick in the summary text
    '    Dim ep As EditPoint = Func.StartPoint.CreateEditPoint()
    '    InsertSummaryText(ep, "function")

    '    ' Look through the function information for parameters.
    '    If (Func.Parameters.Count > 0) Then
    '        Dim Elem As CodeElement
    '        For Each Elem In Func.Parameters
    '            InsertParamText(ep, Elem.FullName)
    '        Next
    '    End If

    '    ' If there's a return type, put in the string for it.
    '    If ("" <> Func.Type.AsFullName) Then
    '        InsertReturnText(ep)
    '    End If

    'End Sub

    '' Inserts the summary text making sure to keep everything lined up.
    'Private Sub InsertSummaryText(ByRef ep As EditPoint, ByRef TypeString As String)

    '    Const SummaryString1 As String = "/// <summary>" + vbCrLf
    '    Const SummaryString2 As String = "/// TODO - Add "
    '    Const SummaryString3 As String = " summary comment" + vbCrLf
    '    Const SummaryString4 As String = "/// </summary>" + vbCrLf

    '    ' Get the offset where the insertion point starts.  If I just 
    '    ' plunk the string in, the editor forgets all about any sort 
    '    ' of normal indenting and just puts everything in the first
    '    ' column.
    '    Dim ColLoc As Integer = ep.DisplayColumn
    '    ' Do the first string.
    '    ep.Insert(SummaryString1)
    '    ' Move the insertion point over.
    '    ep.PadToColumn(ColLoc)
    '    ' Put in the second part.
    '    ep.Insert(SummaryString2)
    '    ' Put in the type part.
    '    ep.Insert(TypeString)
    '    ' Put in the rest of the line.
    '    ep.Insert(SummaryString3)
    '    ' Move the insertion point over.
    '    ep.PadToColumn(ColLoc)
    '    ' Plop in the last part.
    '    ep.Insert(SummaryString4)
    '    ' Now the item is moved over to the first column,
    '    ' so make sure to put it back.
    '    ep.StartOfLine()
    '    ' Move the text over.
    '    ep.PadToColumn(ColLoc)
    'End Sub

    '' Inserts the parameter text, keeping everything lined up.
    'Private Sub InsertParamText(ByRef ep As EditPoint, ByRef Str As String)
    '    Const ParamString1 As String = "/// <param name=" + Chr(34)
    '    Const ParamString2 As String = Chr(34) + ">" + vbCrLf
    '    Const ParamString3 As String = "/// TODO - Add parameter comment" + vbCrLf
    '    Const ParamString4 As String = "/// </param>" + vbCrLf
    '    Dim ColLoc As Integer = ep.DisplayColumn
    '    ' Do the first string.
    '    ep.Insert(ParamString1)
    '    ' Insert the parameter name.
    '    ep.Insert(Str)
    '    ' Do the second string.
    '    ep.Insert(ParamString2)
    '    ' Move the insertion point over.
    '    ep.PadToColumn(ColLoc)
    '    ' Put in the third part.
    '    ep.Insert(ParamString3)
    '    ' Move the insertion point over.
    '    ep.PadToColumn(ColLoc)
    '    ' Plop in the last part.
    '    ep.Insert(ParamString4)
    '    ' Now the item is moved over to the first column,
    '    ' so make sure to put it back.
    '    ep.StartOfLine()
    '    ' Move the text over.
    '    ep.PadToColumn(ColLoc)
    'End Sub

    '' Does the return text.
    'Private Sub InsertReturnText(ByRef ep As EditPoint)

    '    Const ReturnString1 As String = "/// <returns>" + vbCrLf
    '    Const ReturnString2 As String = "/// TODO - Add return comment" + vbCrLf
    '    Const ReturnString3 As String = "/// </returns>" + vbCrLf

    '    Dim ColLoc As Integer = ep.DisplayColumn
    '    ' Do the first string.
    '    ep.Insert(ReturnString1)
    '    ' Move the insertion point over.
    '    ep.PadToColumn(ColLoc)
    '    ' Put in the second part.
    '    ep.Insert(ReturnString2)
    '    ' Move the insertion point over.
    '    ep.PadToColumn(ColLoc)
    '    ' Plop in the last part.
    '    ep.Insert(ReturnString3)
    '    ' Now the item is moved over to the first column,
    '    ' so make sure to put it back.
    '    ep.StartOfLine()
    '    ' Move the text over.
    '    ep.PadToColumn(ColLoc)
    'End Sub


End Module
