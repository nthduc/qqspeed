'' This macro will remove the SuperSaver commands and the toolbar.
Imports EnvDTE
Imports System.Diagnostics
Imports VSLangProj
Imports Microsoft.VisualStudio.VCProjectEngine
Imports System.Reflection
Imports Microsoft.Office.Core
Imports Extensibility
Imports Microsoft.Win32

Public Module SSRemove

    Sub RemoveSuperSaverBarsAndCommands()

        ' Get rid of the AddIn if it's loaded.
        Try
            Dim It As AddIn
            It = DTE.AddIns.Item("SuperSaver")
            If (Not (Nothing Is It)) Then
                MsgBox("You must first unload the SuperSaver Add In manually" + _
                        " and restart VS.NET before running this macro", , _
                        "SuperSaver Command Removal Macro")
                Exit Sub
            End If
        Catch
        End Try


        ' Get rid of the command.
        Dim Cmds As Commands = DTE.Commands
        Try
            Dim Cmd As Command = Cmds.Item("SuperSaver.SuperSaverSaveAll")
            If (Not (Nothing Is Cmd)) Then
                Cmd.Delete()
            End If
        Catch
        End Try

        Try
            Dim Cmd As Command = Cmds.Item("SuperSaver.SuperSaverSave")
            If (Not (Nothing Is Cmd)) Then
                Cmd.Delete()
            End If
        Catch
        End Try
        ' Get rid of the command bar.
        Try
            Dim CmdBars As _CommandBars = DTE.CommandBars
            Dim CmdBar As CommandBar = CmdBars("SuperSaver")
            Cmds.RemoveCommandBar(CmdBar)
        Catch
        End Try

    End Sub

End Module
