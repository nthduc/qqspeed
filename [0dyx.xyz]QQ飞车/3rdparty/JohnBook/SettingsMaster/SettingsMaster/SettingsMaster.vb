'-----------------------------------------------------------------------
' Debugging Applications for Microsoft .NET and Microsoft Windows
' Copyright © 1997 - 2003 John Robbins
'------------------------------------------------------------------------
Imports Microsoft.Office.Core
Imports Extensibility
Imports System.Runtime.InteropServices
Imports EnvDTE
Imports System.Diagnostics
Imports System.Text
Imports System.Xml
Imports System.IO
Imports Microsoft.VisualStudio.VCProjectEngine
Imports System.Reflection
Imports System.Windows.Forms


Public Class SettingsMaster

#Region "Class Fields"

    Private RegSettings As PersistSettings = Nothing

    Private m_ApplicationObject As EnvDTE.DTE = Nothing
    Private m_AddInInstance As EnvDTE.AddIn = Nothing


    Private WithEvents DevTooleEvents As EnvDTE.DTEEvents = Nothing
    Public WithEvents SolEvents As EnvDTE.SolutionEvents = Nothing

    'Toolbar handling.
    Private m_eIDEMode As vsIDEMode = vsIDEMode.vsIDEModeDesign
    Private m_SolutionOpen As Boolean = False

    ' The current SettingsMaster XML property file I'm processing.  This way I
    ' don't have to sling a parameter through all the processing.  I only use
    ' this for error handling.
    Private m_CurrentSMPropFile As String = Nothing

#End Region

#Region "Startup, Initialization, and Shutdown"
    ' Ye ol' ctor.
    Public Sub New(ByVal ApplicationObject As EnvDTE.DTE, _
                   ByVal AddInInstance As EnvDTE.AddIn)

        m_ApplicationObject = ApplicationObject
        m_AddInInstance = AddInInstance

        RegSettings = New PersistSettings(m_ApplicationObject)

        DevTooleEvents = m_ApplicationObject.Events.DTEEvents
        SolEvents = m_ApplicationObject.Events.SolutionEvents

    End Sub

    Public Sub Shutdown()
        m_ApplicationObject = Nothing
        m_AddInInstance = Nothing
        DevTooleEvents = Nothing
        SolEvents = Nothing
    End Sub

#Region "Command Initialization"
    Private Sub RemoveCommandsAndCommandBars()

        Dim Cmds As Commands = m_ApplicationObject.Commands
        Dim Cmd As Command

        ' Delete the commands.
        Try
            Cmd = Cmds.Item(ResConstants.CorrectCurrentSolutionFullCommand)
            Cmd.Delete()
        Catch
        End Try

        Try
            Cmd = Cmds.Item(ResConstants.CustomProjectUpdateFullCommand)
            Cmd.Delete()
        Catch
        End Try

        ' Delete the toolbar.
        Try
            Dim CmdBars As _CommandBars = m_ApplicationObject.CommandBars
            Dim CmdBar As CommandBar = _
                    CType(CmdBars(ResConstants.CommandBarName), CommandBar)

            Cmds.RemoveCommandBar(CmdBar)
        Catch
        End Try

    End Sub

    Public Sub SetupCommandsAndToolbars()

        Try
            ' Get the commands and commandbars
            Dim Cmds As Commands = m_ApplicationObject.Commands
            Dim CmdBars As CommandBars = m_ApplicationObject.CommandBars

            ' Remove any commands that might already be there so I can ensure
            ' these add correctly.
            RemoveCommandsAndCommandBars()

            ' Create the command bar.
            Dim SetMstrCmdBar As CommandBar

            Dim ObjTemp As Object
            ObjTemp = Cmds.AddCommandBar(ResConstants.CommandBarName, _
                               vsCommandBarType.vsCommandBarTypeToolbar)

            SetMstrCmdBar = CType(ObjTemp, CommandBar)

            ' Fix the command bar to the correct position.
            SetMstrCmdBar.Position = MsoBarPosition.msoBarTop

            ' Add the CorrectCurrentSolution command and button.
            Dim Cmd As Command

            Cmd = Cmds.AddNamedCommand(m_AddInInstance, _
                            ResConstants.CorrectCurrentSolution, _
                            ResConstants.CorrectCurrentSolutionButtonText, _
                            ResConstants.CorrectCurrentSolutionToolTip, _
                            False, _
                            ResConstants.CorrectCurrentSolutionBitmapID, _
                            Nothing, _
                            3)

            Cmd.AddControl(SetMstrCmdBar, 1)

            Cmd = Cmds.AddNamedCommand(m_AddInInstance, _
                            ResConstants.CustomProjectUpdate, _
                            ResConstants.CustomProjectUpdateButtonText, _
                            ResConstants.CustomProjectUpdateToolTip, _
                            False, _
                            ResConstants.CustomProjectUpdateBitmapID, _
                            Nothing, _
                            3)

            Cmd.AddControl(SetMstrCmdBar, 2)

            ' Now I can loop through and fix all the controls to display as
            ' icons and not buttons.
            Dim Ctl As CommandBarControl
            For Each Ctl In SetMstrCmdBar.Controls
                If TypeOf Ctl Is CommandBarButton Then
                    Dim Btn As CommandBarButton = _
                            CType(Ctl, CommandBarButton)
                    Btn.Style = MsoButtonStyle.msoButtonIcon
                End If
            Next

            ' Finally, try and put the toolbar someplace other than the
            ' first item.
            SetMstrCmdBar.RowIndex = 4

        Catch eXe As System.Exception
            MsgBox(eXe.Message + vbCrLf + eXe.StackTrace, , _
                   "Exception in SetupCommandsAndToolbars")
        End Try

    End Sub
#End Region

#End Region

#Region "Query Status"
    Public Function QueryStatus(ByVal CommandName As String, _
                                ByVal NeededText As _
                                            EnvDTE.vsCommandStatusTextWanted) _
                    As EnvDTE.vsCommandStatus


        Dim Status As EnvDTE.vsCommandStatus = _
                vsCommandStatus.vsCommandStatusUnsupported

        ' If debugging or no solution open, there's nothing availible.
        If ((m_eIDEMode = vsIDEMode.vsIDEModeDebug) Or _
            (False = m_SolutionOpen)) Then
            QueryStatus = Status
            Exit Function
        End If


        If (vsCommandStatusTextWanted.vsCommandStatusTextWantedNone = _
                                                               NeededText) Then

            ' Which command are they asking for?
            If (ResConstants.CorrectCurrentSolutionFullCommand = _
                                                               CommandName) Then
                ' Here I need to look to see if a solution is open and it \1
                ' projects.
                If (("" <> m_ApplicationObject.Solution.FullName) And _
                    (m_ApplicationObject.Solution.Projects.Count > 0)) Then

                    Status = CType(vsCommandStatus.vsCommandStatusEnabled + _
                                    vsCommandStatus.vsCommandStatusSupported, _
                                   vsCommandStatus)
                End If

            ElseIf (ResConstants.CustomProjectUpdateFullCommand = _
                                                               CommandName) Then

                ' Are there any active projects?
                If (m_ApplicationObject.ActiveSolutionProjects.Length > 0) Then
                    ' Check to see if any are read only.  I can't touch those.
                    Dim HasReadOnly As Boolean = False
                    Dim Curr As Int32 = 0
                    Dim Proj As Project
                    For Curr = 0 To _
                          m_ApplicationObject.ActiveSolutionProjects.Length - 1

                        Proj = m_ApplicationObject.ActiveSolutionProjects(Curr)
                        If (IsProjectReadOnly(Proj) = True) Then
                            HasReadOnly = True
                            Exit For
                        End If
                    Next

                    If (HasReadOnly = False) Then

                        Status = CType(vsCommandStatus.vsCommandStatusEnabled + _
                                        vsCommandStatus.vsCommandStatusSupported, _
                                       vsCommandStatus)
                    End If

                End If

            End If

        End If

        QueryStatus = Status

    End Function
#End Region

#Region "Exec Function"
    Public Sub Exec(ByVal CommandName As String, _
                    ByVal ExecuteOption As EnvDTE.vsCommandExecOption, _
                    ByRef VarIn As Object, _
                    ByRef VarOut As Object, _
                    ByRef Handled As Boolean)

        Handled = False

        If (ExecuteOption = _
                vsCommandExecOption.vsCommandExecOptionDoDefault) Then

            If (ResConstants.CorrectCurrentSolutionFullCommand = _
                                                        CommandName) Then
                Try

                    CorrectCurrentSolution()
                Catch eX As System.Exception
#If Debug Then
                    MsgBox(eX.Message + vbCrLf + eX.StackTrace, , "SettingsMaster")
#Else
                    MsgBox(eX.Message ,, "SettingsMaster")
#End If
                End Try
                Handled = True

            ElseIf (ResConstants.CustomProjectUpdateFullCommand = _
                                                        CommandName) Then

                Try
                    CustomProjectUpdate()
                Catch eX As System.Exception
#If Debug Then
                    MsgBox(eX.Message + vbCrLf + eX.StackTrace, , "SettingsMaster")
#Else
                    MsgBox(eX.Message ,, "SettingsMaster")
#End If
                End Try
                Handled = True

            End If

        End If

    End Sub

#End Region

#Region "Event Handling"
    Private Sub DevTooleEvents_ModeChanged(ByVal LastMode As vsIDEMode) _
                Handles DevTooleEvents.ModeChanged

        If (LastMode = vsIDEMode.vsIDEModeDesign) Then
            m_eIDEMode = vsIDEMode.vsIDEModeDebug
        Else
            m_eIDEMode = vsIDEMode.vsIDEModeDesign
        End If
    End Sub

    Private Sub SolEvents_Opened() Handles SolEvents.Opened
        m_SolutionOpen = True
    End Sub

    Private Sub SolEvents_AfterClosing() Handles SolEvents.AfterClosing
        m_SolutionOpen = False
    End Sub
#End Region

    Sub CorrectCurrentSolution()
        ' At this point, I know there's at least a solution open with at \1
        ' one project in it.  (See QueryStatus).
        Debug.Assert("" <> m_ApplicationObject.Solution.FullName, _
                     "'' <> m_ApplicationObject.Solution.FullName")

        Debug.Assert(m_ApplicationObject.Solution.Projects.Count > 0, _
                     "m_ApplicationObject.Solution.Projects.Count > 0")

        Dim Settings As PersistSettings = _
                            New PersistSettings(m_ApplicationObject)
        Dim FileNameHash As Hashtable = Settings.LanguageArray

        Dim ProjsColl As New Collection()

        Dim Proj As Project
        For Each Proj In m_ApplicationObject.Solution.Projects
            If (IsProjectReadOnly(Proj) = False) Then
                ProjsColl.Add(Proj)
            End If
        Next

        CoreWorkFunction(ProjsColl, _
                         m_ApplicationObject.Solution.Projects.Count, _
                         FileNameHash, _
                         False)

    End Sub

    Sub CustomProjectUpdate()
        Debug.Assert(m_ApplicationObject.ActiveSolutionProjects.Length > 0, _
                    "m_ApplicationObject.ActiveSolutionProjects.Length > 0")

        ' Notice this is EMPTY!  It will be filled as the user is prompted for
        ' files.
        Dim FileNameHash As New Hashtable()

        ' Nothing like a little consistency.  The solution projects are a
        ' Projects collection, while the ActiveSolutionProjects are a
        ' System.Array.
        Dim Projs As New Collection()

        Dim Curr As Int32 = 0
        For Curr = 0 To m_ApplicationObject.ActiveSolutionProjects.Length - 1
            ' The Query stuff above should protect against this command
            ' being called if there are any read only projects.  However,
            ' I'll be a little paranoid and check again.
            Dim Proj As Project = _
                        m_ApplicationObject.ActiveSolutionProjects(Curr)
            If (IsProjectReadOnly(Proj) = False) Then
                Projs.Add(Proj)
            End If
        Next

        CoreWorkFunction(Projs, _
                         m_ApplicationObject.ActiveSolutionProjects.Length, _
                         FileNameHash, _
                         True)



    End Sub

    Private Sub CoreWorkFunction(ByVal Projs As Collection, _
                                 ByVal TotalSteps As Int32, _
                                 ByVal FileNameHash As Hashtable, _
                                 ByVal PromptForFilenames As Boolean)

        ' Grab the status bar so I can let users know what's happening.
        Dim StatBar As EnvDTE.StatusBar = m_ApplicationObject.StatusBar
        StatBar.Clear()

        ' The progress count through the projects.
        Dim CurrStep As Int32 = 0
        StatBar.Progress(True, "SettingsMaster", CurrStep, TotalSteps)

        ' The hash tables to hold the files to open and the open XML documents
        ' themselves.
        Dim Settings As PersistSettings = _
                            New PersistSettings(m_ApplicationObject)
        Dim OpenFileHash As Hashtable = New Hashtable()

        ' Iterate through the solution.

        ' This is the common project object.
        Dim Proj As Project = Nothing
        For Each Proj In Projs

            ' Check to see if this project has a language associated with
            ' its CodeModel.  If it doesn't, I can't touch it.
            Dim LangStr As String
            Try
                LangStr = Proj.CodeModel.Language
            Catch
                LangStr = Nothing
            End Try

            If (LangStr <> Nothing) Then

                ' Indicate which project is having the work done.
                StatBar.Progress(True, _
                                 Proj.UniqueName, _
                                 CurrStep, _
                                 TotalSteps)

                ' I need the configuration XML file name multiple places so
                ' I'll get it once per loop.
                If (False = FileNameHash.ContainsKey(LangStr)) Then
                    If (True = PromptForFilenames) Then
                        Dim StrTemp = PromptForFileName(LangStr)
                        If (Nothing Is StrTemp) Then
                            Exit Sub
                        End If
                        FileNameHash(LangStr) = StrTemp
                    Else
                        ' We got us an unknown language!
                        Throw New _
                        ApplicationException _
                          ("Language not properly registered : " + LangStr)
                    End If

                End If

                m_CurrentSMPropFile = FileNameHash(LangStr)

                ' Do I have the XML settings file already loaded?
                If (False = OpenFileHash.ContainsKey(LangStr)) Then

                    Dim XmlDoc As XmlDocument = _
                                    OpenXMLConfigFile(m_CurrentSMPropFile, _
                                                      LangStr)
                    OpenFileHash(LangStr) = XmlDoc

                    ' Just double check the language matches.
                    If (LangStr <> GetConfigLanguageName(XmlDoc)) Then
                        Throw New _
                        ApplicationException("ProgLanguage element in " + _
                                             m_CurrentSMPropFile + _
                                             " does not exist or is not correct.")
                    End If
                End If


                ' Get the configuration manager and configurations for this
                ' project.
                Dim CfgMgr As ConfigurationManager = Proj.ConfigurationManager
                Dim CfgNames As System.Array = CfgMgr.ConfigurationRowNames()

                ' A general StringBuilder class.
                Dim StrBld As New StringBuilder()

                ' Loop through all the configuration names.
                Dim CurrNameIdx As Int32
                For CurrNameIdx = 0 To (CfgNames.Length - 1)

                    ' Get the configurations for this name.
                    Dim Cfgs As Configurations
                    Cfgs = CfgMgr.ConfigurationRow(CfgNames(CurrNameIdx))

                    ' Iterate the configurations (shouldn't this be
                    ' simpler?)
                    Dim Cfg As Configuration
                    For Each Cfg In Cfgs

                        ' Add the configuration name to the status bar
                        ' display.
                        StrBld.Remove(0, StrBld.Length)
                        StrBld.Append(Proj.UniqueName)
                        StrBld.Append(" ")
                        StrBld.Append(Cfg.ConfigurationName)

                        StatBar.Progress(True, _
                                        StrBld.ToString(), _
                                        CurrStep, _
                                        TotalSteps)

                        ' After all of that, I can now process the actual
                        ' configuration.
                        If (LangStr = _
                                CodeModelLanguageConstants.vsCMLanguageVB) Then
                            ProcessDotNetConfiguration(Cfg, _
                                                       OpenFileHash(LangStr))
                            ' Am I'm supposed to save any updated projects?
                            ' Yes, this is stupid to do this here.  However,
                            ' the screwing, weird project system does no have
                            ' a global Project.Save that works in all cases.
                            ' Consistency, smenistency.
                            If (True = Settings.SaveProjAfterUpdating) Then
                                Proj.Save()
                            End If
                        ElseIf (LangStr = _
                                CodeModelLanguageConstants. _
                                                       vsCMLanguageCSharp) Then
                            ProcessDotNetConfiguration(Cfg, _
                                                       OpenFileHash(LangStr))
                            ' Am I'm supposed to save any updated projects?
                            ' Yes, this is stupid to do this here.  However,
                            ' the screwing, weird project system does no have
                            ' a global Project.Save that works in all cases.
                            ' Consistency, smenistency.
                            If (True = Settings.SaveProjAfterUpdating) Then
                                Proj.Save()
                            End If
                        ElseIf (LangStr = _
                                CodeModelLanguageConstants.vsCMLanguageVC) Then
                            Dim VCPrj As VCProject
                            Dim VCCfg As VCConfiguration

                            ' Now that I've determined the project is a
                            ' VCProject, I can convert the generic project into
                            ' a specific VCProject using the Object property.
                            VCPrj = Proj.Object

                            ' Unfortunately, the same technique does not work on
                            ' configurations so I have to grind through the
                            ' configurations and find the right one manually.
                            For Each VCCfg In VCPrj.Configurations
                                If (Cfg.ConfigurationName = _
                                                VCCfg.ConfigurationName) Then
                                    ' Cool.  Got it.  I can leave the routine.
                                    Exit For
                                End If
                            Next

                            Debug.Assert(Not (VCCfg Is Nothing), _
                                         "Not ( VCCfg is Nothing )")
                            If (VCCfg Is Nothing) Then
                                Throw New ApplicationException _
                                ("Active project is not VC project when it should be!")
                            End If

                            ' Grind out the settings for this project
                            ProcessVCConfiguration(VCCfg, _
                                                   OpenFileHash(LangStr))

                            ' Am I'm supposed to save any updated projects?
                            ' Yes, this is stupid to do this here.  However,
                            ' the screwing, weird project system does no have
                            ' a global Project.Save that works in all cases.
                            ' Consistency, smenistency.
                            If (True = Settings.SaveProjAfterUpdating) Then
                                VCPrj.Save()
                            End If

                        End If
                    Next
                Next
            End If
            ' Bump to the next count for the progress bar.
            CurrStep += 1

        Next

        ' Clear the status bar so I don't leave crud on the screen.
        StatBar.Clear()

    End Sub

    ' The Project object does not have a property on it that indicates
    ' read only status (grumble....)
    Private Function IsProjectReadOnly(ByVal Proj As Project) As Boolean

        Try
            Dim f As FileInfo = New FileInfo(Proj.FullName)
            Dim fa As FileAttributes = f.Attributes
            If ((f.Attributes And FileAttributes.ReadOnly) = _
                                    FileAttributes.ReadOnly) Then
                IsProjectReadOnly = True
            Else
                IsProjectReadOnly = False
            End If
        Catch
            ' If there's any exception, I will return true so my code
            ' won't diddle with the file.  I don't want this exception
            ' to propigate.
            IsProjectReadOnly = True
        End Try
    End Function

    Private Function PromptForFileName(ByVal LangStr As String)

        Dim CurrAsm As System.Reflection.Assembly
        CurrAsm = System.Reflection.Assembly.GetExecutingAssembly()

        Dim StrLoc As StringBuilder = New StringBuilder()

        Dim StrTemp As String = CurrAsm.Location.ToLower()
        Dim iPos As Int32 = StrTemp.IndexOf("settingsmaster.dll")

        StrLoc.Append(CurrAsm.Location.Substring(0, iPos))

        Dim OpenDialog As New OpenFileDialog()

        OpenDialog.InitialDirectory = StrLoc.ToString()

        OpenDialog.FileName = ""
        OpenDialog.DefaultExt = "XML"
        OpenDialog.Filter = "Config Files (*.XML)|*.XML|All files (*.*)|*.*"
        OpenDialog.FilterIndex = 1
        OpenDialog.CheckFileExists = True
        OpenDialog.CheckPathExists = True
        OpenDialog.ValidateNames = True

        If (True = LanguageGUID.IsKnownGUID(LangStr)) Then
            OpenDialog.Title = "Browse for " + _
                                LanguageGUID.GUIDToName(LangStr) + _
                                " Configuration"
        Else
            OpenDialog.Title = "Browse for Unknown Language Configuration"
        End If


        If (OpenDialog.ShowDialog() = DialogResult.OK) Then
            PromptForFileName = OpenDialog.FileName
        Else
            PromptForFileName = Nothing
        End If

    End Function

    Private Function OpenXMLConfigFile(ByVal FileName As String, _
                                       ByVal LangStr As String) As XmlDocument

        ' Get the XML document loaded.
        Dim XmlDoc As XmlDocument = New XmlDocument()

        ' Check to see if there's anything resembling path characters in the
        ' string.  If so, I'll just try the file directly.
        If ((-1 <> FileName.LastIndexOf("\")) Or _
                    (-1 <> FileName.LastIndexOf(":"))) Then

            Try
                XmlDoc.Load(FileName)
            Catch eX As FileNotFoundException
                XmlDoc = Nothing
            End Try
        Else

            ' I'll first try the same directory as the Add In by appending the name
            ' that that directory.
            Dim CurrAsm As System.Reflection.Assembly
            CurrAsm = System.Reflection.Assembly.GetExecutingAssembly()

            Dim StrLoc As StringBuilder = New StringBuilder()

            Dim StrTemp As String = CurrAsm.Location.ToLower()
            Dim iPos As Int32 = StrTemp.IndexOf("settingsmaster.dll")

            StrLoc.Append(CurrAsm.Location.Substring(0, iPos))
            StrLoc.Append(FileName)

            Try
                XmlDoc.Load(StrLoc.ToString())
            Catch eX As FileNotFoundException
                ' It's no where to be found!
                XmlDoc = Nothing
            End Try
        End If

        If Nothing Is XmlDoc Then
            ' Let the user know.
            Dim StrErr As New StringBuilder()
            StrErr.Append("The SettingsMaster config file ")
            StrErr.Append(FileName)
            StrErr.Append(" for language ")
            If (True = LanguageGUID.IsKnownGUID(LangStr)) Then
                StrErr.Append(LanguageGUID.GUIDToName(LangStr))
            Else
                StrErr.Append(LangStr)
            End If
            StrErr.Append(" does not exist.  Check your installation.")
            Throw New ApplicationException(StrErr.ToString)
        End If


        OpenXMLConfigFile = XmlDoc

    End Function

    Private Function GetConfigLanguageName(ByVal XmlDoc As XmlDocument)
        ' Get the configurations nodes list
        Dim Nodes As XmlNodeList = _
                            XmlDoc.GetElementsByTagName("ProgLanguage")
        GetConfigLanguageName = Nodes(0).InnerText
    End Function

    Private Sub ProcessDotNetConfiguration(ByVal Cfg As Configuration, _
                                           ByVal XmlDoc As XmlDocument)
        ' Get the configuration nodes list
        Dim Nodes As XmlNodeList = _
                            XmlDoc.GetElementsByTagName("Configuration")

        ' Loop through the nodes looking for the matching configuration.
        Dim Node As XmlNode
        For Each Node In Nodes
            If (Cfg.ConfigurationName = Node("ConfigName").InnerText) Then
                ' Get the properties node.
                Node = Node("Properties")

                Dim CurrProp As XmlNode
                For Each CurrProp In Node.ChildNodes
                    ProcessDotNetProperty(CurrProp, Cfg)
                Next

                ' We're cooked so I can pop out of the loop.
                Exit For

            End If
        Next
    End Sub

    Private Sub ProcessDotNetProperty(ByVal CurrProp As XmlNode, _
                                      ByVal Cfg As Configuration)
        Dim XmlPropName As String
        Dim XmlPropType As String
        Dim XmlPropAttr As String
        Dim XmlPropVal As String

        ' Snarf up the values so I don't have to keep
        ' hitting them.  One function call is always
        ' better than a bunch...
        XmlPropName = CurrProp("PropertyName").InnerText
        XmlPropType = CurrProp("PropertyType").InnerText
        XmlPropVal = CurrProp("PropertyValue").InnerText

        ' The property type drives everything.
        If ("String" = XmlPropType) Then

            Dim FinalSettingStr As String
            Dim XmlStringOpAttr As String

            ' Get the attribute, which will determine if this is
            ' an overwrite or append operation.
            XmlStringOpAttr = CurrProp("PropertyType"). _
                                                Attributes("OpType").InnerText
            If ("Overwrite" = XmlStringOpAttr) Then
                ' The easy case.
                FinalSettingStr = XmlPropVal
            ElseIf ("Append" = XmlStringOpAttr) Then
                ' I've got to get the value so I can append to it.
                FinalSettingStr = Cfg.Properties.Item(XmlPropName).Value
                FinalSettingStr = FinalSettingStr + XmlPropVal
            Else
                Throw New System.Exception("Invalid string operation " + _
                                           XmlStringOpAttr + " in " + _
                                           XmlPropName + " in " + _
                                           m_CurrentSMPropFile + _
                                           "unable to process string")
            End If

            '' Set it!
            Cfg.Properties.Item(XmlPropName).Value = FinalSettingStr
        ElseIf ("Boolean" = XmlPropType) Then
            ' Determine the value.
            Dim BoolVal As Boolean = False
            If (XmlPropVal = "1") Then
                BoolVal = True
            End If
            ' Set the value.
            Cfg.Properties.Item(XmlPropName).Value = BoolVal
        ElseIf ("Enum" = XmlPropType) Then
            ' I need the the VSLangProj assembly for the reflection.
            Dim VSLangProjAssem As System.Reflection.Assembly
            VSLangProjAssem = System.Reflection.Assembly.Load("VSLangProj")

            ' Get the name attribute out of the XML node as that's the
            ' name of the enum.
            Dim XmlAttrType As String
            XmlAttrType = CurrProp("PropertyType").Attributes("Name").InnerText

            ' First try creating it with the dotted name first
            Dim EnumType As System.Type

            EnumType = VSLangProjAssem.GetType("VSLangProj." + XmlAttrType)
            If (Nothing Is EnumType) Then
                EnumType = VSLangProjAssem.GetType(XmlAttrType)
                If (Nothing Is EnumType) Then
                    Throw New System.Exception("invalid enum type " + _
                                               XmlAttrType + " for " + _
                                               XmlPropName + " in " + _
                                               m_CurrentSMPropFile + _
                                               "unable to create enum")
                End If
            End If

            ' Create the enum type.
            Dim EnInst As System.Enum
            EnInst = System.Enum.Parse(EnumType, XmlPropVal)

            '' Set it!
            Cfg.Properties.Item(XmlPropName).Value = EnInst

        Else
            Throw New System.Exception("Invalid type name" + _
                                        XmlPropType + " for " + _
                                        XmlPropName + " in " + _
                                        m_CurrentSMPropFile + _
                                        " unable to process type")
        End If
    End Sub


    Private Sub ProcessVCConfiguration(ByVal VCCfg As VCConfiguration, _
                                       ByVal XmlDoc As XmlDocument)

        ' Get the configuration nodes list
        Dim Nodes As XmlNodeList = _
                            XmlDoc.GetElementsByTagName("Configuration")

        ' Get the VCProjectEngine assembly as that's what I'll need for the
        ' reflection.  This way, I'm doing it once instead of each time
        ' through on a tool.
        Dim VCProjEngAssem As System.Reflection.Assembly
        VCProjEngAssem = System.Reflection.Assembly.Load _
                            ("Microsoft.VisualStudio.VCProjectEngine")

        ' Loop through the nodes looking for the matching configuration.
        Dim Node As XmlNode
        For Each Node In Nodes
            If (VCCfg.ConfigurationName = Node("ConfigName").InnerText) Then
                ' Get the Tools node in order to grind through the tools.
                Node = Node("Tools")

                Dim ToolNode As XmlNode
                For Each ToolNode In Node.ChildNodes
                    ProcessVCTool(VCCfg, _
                                  VCProjEngAssem, _
                                  ToolNode)
                Next

                ' We're cooked so I can pop out of the loop.
                Exit For

            End If
        Next


    End Sub

    Const g_VCProjectEngineName As String = "Microsoft.VisualStudio.VCProjectEngine."

    Private Sub ProcessVCTool(ByVal VCCfg As VCConfiguration, _
                              ByVal VCProjEngAssem As [Assembly], _
                              ByVal Node As XmlNode)

        ' Get the name of the tool were about to do.
        Dim ToolStr As String
        ToolStr = Node("ToolName").InnerText

        ' See if I can create the specified tool.

        ' The variable that'll hold the actual tool I'll be bapping
        ' to set properties on through late binding reflection.
        Dim ToolObjInst As Object

        ' Most of the tools I'll create through reflection, but as I
        ' need to support setting properties on the VCConfiguration,
        ' I have to special case this.  I'd prefer not to but key
        ' settings (whole program optimization!) is set in the
        ' configuration instead of on the tools.
        If ("VCConfiguration" = ToolStr) Then
            ' As the configuration is passed right in, this is easy.
            ToolObjInst = VCCfg
        Else
            ' This should be a tool as described in the VC Project
            ' documentation.

            ' Create the requested tool by asking the VCConfiguration
            ' object for it.
            ToolObjInst = VCCfg.Tools(ToolStr)
            If (Nothing Is ToolObjInst) Then

                ' It's entirely possible for me to not be able to create
                ' the tool.  For example, if it's a makefile project, you
                ' can only create the VCNMakeTool and nothing else.  Since
                ' there's no clean way to handle all possible combinations
                ' and permutations (think about third party projects like
                ' those from ActiveState, there's not much I can do.
                ' The big problem is that there's no way to know if a tool
                ' is actually invalid.  Bummer.
                Exit Sub
            End If
        End If

        ' Now that I have the tool object instance, I need to use
        ' reflection to get the property information.
        Dim ToolType As System.Type

        ' For reflection, I need to use the whole dotted name.  First
        ' I'll try adding the tool name onto the dotted name.  If that
        ' doesn't work, I'll try the name directly in case the user
        ' happened to enter full names.
        ToolType = VCProjEngAssem.GetType(g_VCProjectEngineName + ToolStr)

        If (Nothing Is ToolType) Then
            ToolType = VCProjEngAssem.GetType(ToolStr)
            If (Nothing Is ToolType) Then
                Throw New ApplicationException("Invalid tool type " + _
                                               ToolStr + " in " + _
                                               m_CurrentSMPropFile + _
                                               " Unable to create type.")
            End If
        End If

        ' Armed with the tool and the type, get the properties
        ' node so I can whip through and set them.
        Dim PropertiesNode As XmlNode
        PropertiesNode = Node("Properties")

        ' Iterate through each property for this tool.
        Dim CurrProp As XmlNode
        For Each CurrProp In PropertiesNode.ChildNodes

            Dim XmlPropName As String
            Dim XmlPropType As String
            Dim XmlPropAttr As String
            Dim XmlPropVal As String

            Dim NameNode As XmlNode

            ' Snarf up the values so I don't have to keep
            ' hitting them.  One function call is always
            ' better than a bunch...
            NameNode = CurrProp("PropertyName")
            XmlPropName = NameNode.InnerText
            XmlPropType = CurrProp("PropertyType").InnerText
            XmlPropVal = CurrProp("PropertyValue").InnerText

            ' Before anything else, check that this setting applies
            ' to the current configuration.
            If (NameNode.Attributes.Count > 0) Then
                Dim XmlAppAttr As String
                XmlAppAttr = NameNode.Attributes("Type").InnerText


                If (("EXE" = XmlAppAttr) And _
                            ((VCCfg.ConfigurationType <> _
                                ConfigurationTypes.typeApplication))) Then
                    ' My kingdom for a continue statement in VB.NET!!
                    GoTo JamInAContinueDangNabIt
                ElseIf (("DLL" = XmlAppAttr) And _
                            ((VCCfg.ConfigurationType <> _
                                ConfigurationTypes.typeDynamicLibrary))) Then
                    ' My kingdom for a continue statement in VB.NET!!
                    GoTo JamInAContinueDangNabIt
                ElseIf (Not ("DLL" = XmlAppAttr)) And _
                            (Not ("EXE" = XmlAppAttr)) Then

                    Throw New _
                        ApplicationException("Invalid Project Attribute " + _
                                             XmlAppAttr + " in " + _
                                             XmlPropName + " in " + _
                                             m_CurrentSMPropFile)

                End If

            End If

            ' Try and create the property itself before I do anything
            ' else.  If I can't get the property, there's not much
            ' I can do.
            Dim ToolPropInfo As PropertyInfo
            ToolPropInfo = ToolType.GetProperty(XmlPropName)
            If (Nothing Is ToolPropInfo) Then
                Throw New ApplicationException("Invalid property name " + _
                                               XmlPropName + " for " + _
                                               ToolStr + " in " + _
                                               m_CurrentSMPropFile + _
                                               " unable to create property.")
            End If

            ' The property type drives everything.
            If ("String" = XmlPropType) Then
                Dim FinalSettingStr As String
                Dim XmlStringOpAttr As String

                ' Get the attribute, which will determine if this is
                ' an overwrite or append operation.
                XmlStringOpAttr = CurrProp("PropertyType"). _
                                    Attributes("OpType").InnerText
                If ("Overwrite" = XmlStringOpAttr) Then
                    ' The easy case.
                    FinalSettingStr = XmlPropVal
                ElseIf ("Append" = XmlStringOpAttr) Then
                    ' I've got to get the value so I can append to it.
                    FinalSettingStr = ToolPropInfo.GetValue(ToolObjInst, _
                                                            Nothing)
                    FinalSettingStr = FinalSettingStr + XmlPropVal
                Else
                    Throw New ApplicationException("Invalid string operation " + _
                                                    XmlStringOpAttr + " in " + _
                                                    XmlPropName + " in " + _
                                                    m_CurrentSMPropFile + _
                                                    "unable to process string")
                End If

                ' Set it!
                ToolPropInfo.SetValue(ToolObjInst, FinalSettingStr, Nothing)

            ElseIf ("Boolean" = XmlPropType) Then
                ' Determine the value.
                Dim BoolVal As Boolean = False
                If (XmlPropVal = "1") Then
                    BoolVal = True
                End If
                ' Set the value.
                ToolPropInfo.SetValue(ToolObjInst, BoolVal, Nothing)

            ElseIf ("Enum" = XmlPropType) Then

                ' For the enums, I need to get the Name attribute out
                ' of the XML node as I'll create an enum of that type.
                Dim XmlAttrType As String
                XmlAttrType = CurrProp("PropertyType"). _
                                        Attributes("Name").InnerText

                Dim EnumType As System.Type
                ' As creating the tool, I try adding the dotted
                ' complete path first before trying just the name
                ' itself.
                EnumType = VCProjEngAssem. _
                                GetType(g_VCProjectEngineName + XmlAttrType)
                If (Nothing Is EnumType) Then
                    EnumType = VCProjEngAssem.GetType(XmlAttrType)
                    If (Nothing Is EnumType) Then
                        Throw New ApplicationException("Invalid enum type " + _
                                                      XmlAttrType + " for " + _
                                                      XmlPropName + " in " + _
                                                      m_CurrentSMPropFile + _
                                                      " unable to create enum")
                    End If
                End If

                ' Create the enum type.
                Dim EnInst As System.Enum
                EnInst = System.Enum.Parse(EnumType, XmlPropVal)

                ' Set it!
                ToolPropInfo.SetValue(ToolObjInst, EnInst, Nothing)

            Else

                Throw New ApplicationException("Invalid type name" + _
                                               XmlPropType + " for " + _
                                               ToolStr + " in " + _
                                               m_CurrentSMPropFile + _
                                               " unable to process type")
            End If

JamInAContinueDangNabIt:
        Next
    End Sub

End Class
