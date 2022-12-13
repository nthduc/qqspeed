'-----------------------------------------------------------------------
' Debugging Applications for Microsoft .NET and Microsoft Windows
' Copyright © 1997 - 2003 John Robbins
'------------------------------------------------------------------------
Imports Microsoft.Office.Core
imports Extensibility
imports System.Runtime.InteropServices
Imports EnvDTE

<GuidAttribute("411FE98A-B4EB-494C-97BB-629A21F6BC73"), _
               ProgIdAttribute("SettingsMaster")> _
Public Class Connect

    Implements Extensibility.IDTExtensibility2
    Implements EnvDTE.IDTCommandTarget


    Private m_ApplicationObject As EnvDTE.DTE = Nothing
    Private m_AddInInstance As EnvDTE.AddIn = Nothing
    Private m_SetMast As SettingsMaster = Nothing

    Public Sub OnBeginShutdown(ByRef custom As System.Array) _
                Implements Extensibility.IDTExtensibility2.OnBeginShutdown
    End Sub

    Public Sub OnAddInsUpdate(ByRef custom As System.Array) _
                Implements Extensibility.IDTExtensibility2.OnAddInsUpdate
    End Sub

    Public Sub OnStartupComplete(ByRef custom As System.Array) _
                Implements Extensibility.IDTExtensibility2.OnStartupComplete
    End Sub

    Public Sub OnDisconnection(ByVal RemoveMode As Extensibility.ext_DisconnectMode, _
                               ByRef custom As System.Array) _
                Implements Extensibility.IDTExtensibility2.OnDisconnection
        m_SetMast.Shutdown()
        m_SetMast = Nothing
    End Sub

    Public Sub OnConnection(ByVal Application As Object, _
                            ByVal ConnectMode As Extensibility.ext_ConnectMode, _
                            ByVal AddInInst As Object, _
                            ByRef Custom As System.Array) _
                Implements Extensibility.IDTExtensibility2.OnConnection

        m_ApplicationObject = CType(Application, EnvDTE.DTE)
        m_AddInInstance = CType(AddInInst, EnvDTE.AddIn)

        If (ConnectMode = ext_ConnectMode.ext_cm_UISetup) Then
            ' New up the class that does all the work.
            m_SetMast = New SettingsMaster(m_ApplicationObject, _
                                           m_AddInInstance)

            ' Do the work to set up the commands, toolbars, etc.
            m_SetMast.SetupCommandsAndToolbars()

        ElseIf (ConnectMode = ext_ConnectMode.ext_cm_AfterStartup) Or _
               (ConnectMode = ext_ConnectMode.ext_cm_Startup) Then

            If (Nothing Is m_SetMast) Then
                m_SetMast = New SettingsMaster(m_ApplicationObject, _
                                               m_AddInInstance)
            End If
        End If

    End Sub

    Public Sub OnQueryStatus(ByVal CommandName As String, _
                             ByVal NeededText As EnvDTE.vsCommandStatusTextWanted, _
                             ByRef Status As EnvDTE.vsCommandStatus, _
                             ByRef CommandText As Object) _
            Implements EnvDTE.IDTCommandTarget.QueryStatus

        Status = m_SetMast.QueryStatus(CommandName, NeededText)

    End Sub

    Public Sub Exec(ByVal CommandName As String, _
                    ByVal ExecuteOption As EnvDTE.vsCommandExecOption, _
                    ByRef VarIn As Object, _
                    ByRef VarOut As Object, _
                    ByRef Handled As Boolean) _
            Implements EnvDTE.IDTCommandTarget.Exec

        m_SetMast.Exec(CommandName, _
                       ExecuteOption, _
                       VarIn, _
                       VarOut, _
                       Handled)
    End Sub

End Class
