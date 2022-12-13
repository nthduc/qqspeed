'-----------------------------------------------------------------------
' Debugging Applications for Microsoft .NET and Microsoft Windows
' Copyright © 1997 - 2003 John Robbins
'------------------------------------------------------------------------
Imports EnvDTE
Imports System.ComponentModel
Imports System.Runtime.InteropServices
Imports System.Drawing
Imports System.Windows.Forms
Imports System.Text

<GuidAttribute("7E177BE2-5391-469d-8A4E-325472BC91EE"), _
  ProgId("SettingsMaster.OptionsControl")> _
Public Class SettingsMasterOptions
    Inherits System.Windows.Forms.UserControl
    Implements EnvDTE.IDTToolsOptionsPage

#Region " Windows Form Designer generated code "

    Public Sub New()
        MyBase.New()

        'This call is required by the Windows Form Designer.
        InitializeComponent()

        'Add any initialization after the InitializeComponent() call

    End Sub

    'UserControl overrides dispose to clean up the component list.
    Protected Overloads Overrides Sub Dispose(ByVal disposing As Boolean)
        If disposing Then
            If Not (components Is Nothing) Then
                components.Dispose()
            End If
        End If
        MyBase.Dispose(disposing)
    End Sub

    'Required by the Windows Form Designer
    Private components As System.ComponentModel.IContainer

    'NOTE: The following procedure is required by the Windows Form Designer
    'It can be modified using the Windows Form Designer.
    'Do not modify it using the code editor.
    Friend WithEvents SettingsLabel As System.Windows.Forms.Label
    Friend WithEvents SetBar As System.Windows.Forms.Label
    Friend WithEvents SaveProjCheckBox As System.Windows.Forms.CheckBox
    Friend WithEvents LanguageLabel As System.Windows.Forms.Label
    Friend WithEvents LangSetBar As System.Windows.Forms.Label
    Friend WithEvents LangLabel As System.Windows.Forms.Label
    Friend WithEvents CurrFile As System.Windows.Forms.Label
    Friend WithEvents CurrFileTextBox As System.Windows.Forms.TextBox
    Friend WithEvents LangCombo As System.Windows.Forms.ComboBox
    Friend WithEvents BrowseButton As System.Windows.Forms.Button
    Friend WithEvents EditButton As System.Windows.Forms.Button
    Friend WithEvents OpenFileDialog As System.Windows.Forms.OpenFileDialog
    <System.Diagnostics.DebuggerStepThrough()> Private Sub InitializeComponent()
        Me.SettingsLabel = New System.Windows.Forms.Label()
        Me.SetBar = New System.Windows.Forms.Label()
        Me.SaveProjCheckBox = New System.Windows.Forms.CheckBox()
        Me.LanguageLabel = New System.Windows.Forms.Label()
        Me.LangSetBar = New System.Windows.Forms.Label()
        Me.LangLabel = New System.Windows.Forms.Label()
        Me.CurrFile = New System.Windows.Forms.Label()
        Me.CurrFileTextBox = New System.Windows.Forms.TextBox()
        Me.LangCombo = New System.Windows.Forms.ComboBox()
        Me.BrowseButton = New System.Windows.Forms.Button()
        Me.EditButton = New System.Windows.Forms.Button()
        Me.OpenFileDialog = New System.Windows.Forms.OpenFileDialog()
        Me.SuspendLayout()
        '
        'SettingsLabel
        '
        Me.SettingsLabel.AutoSize = True
        Me.SettingsLabel.Name = "SettingsLabel"
        Me.SettingsLabel.Size = New System.Drawing.Size(45, 13)
        Me.SettingsLabel.TabIndex = 99
        Me.SettingsLabel.Text = "Settings"
        '
        'SetBar
        '
        Me.SetBar.Anchor = ((System.Windows.Forms.AnchorStyles.Top Or System.Windows.Forms.AnchorStyles.Left) _
                    Or System.Windows.Forms.AnchorStyles.Right)
        Me.SetBar.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D
        Me.SetBar.Location = New System.Drawing.Point(0, 8)
        Me.SetBar.Name = "SetBar"
        Me.SetBar.Size = New System.Drawing.Size(416, 2)
        Me.SetBar.TabIndex = 99
        '
        'SaveProjCheckBox
        '
        Me.SaveProjCheckBox.Location = New System.Drawing.Point(16, 24)
        Me.SaveProjCheckBox.Name = "SaveProjCheckBox"
        Me.SaveProjCheckBox.Size = New System.Drawing.Size(368, 24)
        Me.SaveProjCheckBox.TabIndex = 0
        Me.SaveProjCheckBox.Text = "Save solutions/projects after updating settings"
        '
        'LanguageLabel
        '
        Me.LanguageLabel.AutoSize = True
        Me.LanguageLabel.Location = New System.Drawing.Point(0, 56)
        Me.LanguageLabel.Name = "LanguageLabel"
        Me.LanguageLabel.Size = New System.Drawing.Size(121, 13)
        Me.LanguageLabel.TabIndex = 99
        Me.LanguageLabel.Text = "Default Language Files"
        '
        'LangSetBar
        '
        Me.LangSetBar.Anchor = ((System.Windows.Forms.AnchorStyles.Top Or System.Windows.Forms.AnchorStyles.Left) _
                    Or System.Windows.Forms.AnchorStyles.Right)
        Me.LangSetBar.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D
        Me.LangSetBar.Location = New System.Drawing.Point(-8, 64)
        Me.LangSetBar.Name = "LangSetBar"
        Me.LangSetBar.Size = New System.Drawing.Size(416, 2)
        Me.LangSetBar.TabIndex = 99
        '
        'LangLabel
        '
        Me.LangLabel.AutoSize = True
        Me.LangLabel.Location = New System.Drawing.Point(16, 84)
        Me.LangLabel.Name = "LangLabel"
        Me.LangLabel.Size = New System.Drawing.Size(58, 13)
        Me.LangLabel.TabIndex = 9
        Me.LangLabel.Text = "Language:"
        '
        'CurrFile
        '
        Me.CurrFile.AutoSize = True
        Me.CurrFile.Location = New System.Drawing.Point(16, 128)
        Me.CurrFile.Name = "CurrFile"
        Me.CurrFile.Size = New System.Drawing.Size(131, 13)
        Me.CurrFile.TabIndex = 99
        Me.CurrFile.Text = "Current configuration file:"
        '
        'CurrFileTextBox
        '
        Me.CurrFileTextBox.Anchor = ((System.Windows.Forms.AnchorStyles.Top Or System.Windows.Forms.AnchorStyles.Left) _
                    Or System.Windows.Forms.AnchorStyles.Right)
        Me.CurrFileTextBox.Location = New System.Drawing.Point(16, 152)
        Me.CurrFileTextBox.Name = "CurrFileTextBox"
        Me.CurrFileTextBox.ReadOnly = True
        Me.CurrFileTextBox.Size = New System.Drawing.Size(376, 20)
        Me.CurrFileTextBox.TabIndex = 4
        Me.CurrFileTextBox.Text = ""
        '
        'LangCombo
        '
        Me.LangCombo.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList
        Me.LangCombo.Location = New System.Drawing.Point(96, 80)
        Me.LangCombo.Name = "LangCombo"
        Me.LangCombo.Size = New System.Drawing.Size(121, 21)
        Me.LangCombo.Sorted = True
        Me.LangCombo.TabIndex = 1
        '
        'BrowseButton
        '
        Me.BrowseButton.Location = New System.Drawing.Point(208, 120)
        Me.BrowseButton.Name = "BrowseButton"
        Me.BrowseButton.TabIndex = 2
        Me.BrowseButton.Text = "Browse"
        '
        'EditButton
        '
        Me.EditButton.Location = New System.Drawing.Point(296, 120)
        Me.EditButton.Name = "EditButton"
        Me.EditButton.TabIndex = 3
        Me.EditButton.Text = "Edit"
        '
        'OpenFileDialog
        '
        Me.OpenFileDialog.DefaultExt = "XML"
        Me.OpenFileDialog.Filter = "XML Files(*.XML)|*.XML|All Files (*.*)|*.*"
        Me.OpenFileDialog.RestoreDirectory = True
        Me.OpenFileDialog.Title = "Open SettingsMaster Configuration File for"
        '
        'SettingsMasterOptions
        '
        Me.Controls.AddRange(New System.Windows.Forms.Control() {Me.EditButton, Me.BrowseButton, Me.LangCombo, Me.CurrFileTextBox, Me.CurrFile, Me.LangLabel, Me.LanguageLabel, Me.SaveProjCheckBox, Me.SettingsLabel, Me.SetBar, Me.LangSetBar})
        Me.Name = "SettingsMasterOptions"
        Me.Size = New System.Drawing.Size(408, 184)
        Me.ResumeLayout(False)

    End Sub

#End Region

    Dim m_RegSet As PersistSettings = Nothing
    Dim m_FileHash As Hashtable = Nothing
    Dim m_SelIndex As Int32 = 0
    Dim m_DTEObject As DTE = Nothing

    Public Sub GetProperties(ByRef PropertiesObject As Object) _
            Implements IDTToolsOptionsPage.GetProperties

        PropertiesObject = Nothing

    End Sub

    Public Sub OnAfterCreated(ByVal DTEObject As DTE) _
            Implements IDTToolsOptionsPage.OnAfterCreated

        m_DTEObject = DTEObject

        ' Go through and update all the controls to use the right font.
        Dim Props As Properties
        Props = DTEObject.Properties("FontsAndColors", _
                                     "Dialogs and Tool Windows")

        Dim FntName As String = Props.Item("FontFamily").Value
        Dim ObjTemp As Object = Props.Item("FontSize").Value

        Dim FntSize As Int32 = Convert.ToInt32(ObjTemp)

        Dim DlgFont As Font = New Font(FntName, _
                                       FntSize, _
                                       GraphicsUnit.Point)

        Me.Font = DlgFont

        Dim Ctl As Control
        For Each Ctl In Me.Controls
            Ctl.Font = DlgFont
        Next

        ' Resize those controls not autoresizing.
        Dim g As Graphics
        Dim sizeF As SizeF

        g = SaveProjCheckBox.CreateGraphics()
        sizeF = g.MeasureString(SaveProjCheckBox.Text, _
                                SaveProjCheckBox.Font)
        SaveProjCheckBox.Height = Convert.ToInt32(sizeF.Height) + 6
        SaveProjCheckBox.Width = Convert.ToInt32(sizeF.Width) + 20

        g = LangCombo.CreateGraphics()
        sizeF = g.MeasureString("WWWWWWW", _
                                LangCombo.Font)
        LangCombo.Height = Convert.ToInt32(sizeF.Height) + 6
        LangCombo.Width = Convert.ToInt32(sizeF.Width) + 20

        g = CurrFileTextBox.CreateGraphics()
        sizeF = g.MeasureString("XXXXXX", CurrFileTextBox.Font)
        CurrFileTextBox.Height = Convert.ToInt32(sizeF.Height) + 6

        g = BrowseButton.CreateGraphics()
        sizeF = g.MeasureString(BrowseButton.Text, BrowseButton.Font)
        BrowseButton.Height = Convert.ToInt32(sizeF.Height) + 6
        BrowseButton.Width = Convert.ToInt32(sizeF.Width) + 20

        EditButton.Size = BrowseButton.Size

        ' Jiggle the lines so they look good.
        Dim LineHeight As Int32
        LineHeight = SettingsLabel.Bottom - SettingsLabel.Top

        Dim HalfHeight As Int32 = (LineHeight / 2) - 1

        SetBar.Top = SettingsLabel.Top + HalfHeight

        LangSetBar.Top = LanguageLabel.Top + HalfHeight

        ' Get the settings from the registry.
        m_RegSet = New PersistSettings(DTEObject)

        SaveProjCheckBox.Checked = m_RegSet.SaveProjAfterUpdating

        m_FileHash = m_RegSet.LanguageArray
        Debug.Assert(m_FileHash.Count > 0, "m_FileHash.Count > 0")
        If (m_FileHash.Count = 0) Then
            Throw New ArgumentException("Invalid language count in registry!")
        End If

        Dim Key As String
        For Each Key In m_FileHash.Keys
            'LangCombo.Items.Add(Key)
            LangCombo.Items.Add(LanguageGUID.GUIDToName(Key))
        Next

        LangCombo.SelectedIndex = 0
        m_SelIndex = 0
        ' I've always wondered why you can't simply get the first
        ' key of a hash.
        CurrFileTextBox.Text = _
                m_FileHash(LanguageGUID.NameToGUID(LangCombo.Items(0)))

    End Sub

    Public Sub OnHelp() _
            Implements IDTToolsOptionsPage.OnHelp
        MsgBox("Please read Chapter 9 of Debugging Applications for " + _
               "Microsoft .NET and Microsoft Windows by John Robbins " + _
               "for more information about SettingsMaster.", _
               MsgBoxStyle.Information, _
               "SettingsMaster")
    End Sub

    Public Sub OnCancel() _
            Implements IDTToolsOptionsPage.OnCancel

    End Sub

    Public Sub OnOK() _
            Implements IDTToolsOptionsPage.OnOK

        m_RegSet.SaveProjAfterUpdating = SaveProjCheckBox.Checked

        m_RegSet.LanguageArray = m_FileHash

    End Sub

    Private Sub LangCombo_SelectedIndexChanged(ByVal sender As System.Object, _
                                               ByVal e As System.EventArgs) _
            Handles LangCombo.SelectedIndexChanged
        ' A fine case where the event should contain the index changed to!!
        Dim i As Int32 = LangCombo.SelectedIndex
        Dim CurrLangName As String = LangCombo.Items(i)
        Dim PrevLangName As String = LangCombo.Items(m_SelIndex)

        If ("" <> CurrFileTextBox.Text) Then
            m_FileHash(LanguageGUID.NameToGUID(PrevLangName)) = _
                                                    CurrFileTextBox.Text
        End If

        CurrFileTextBox.Text = m_FileHash(LanguageGUID.NameToGUID(CurrLangName))

        m_SelIndex = i
    End Sub

    Private Sub BrowseButton_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles BrowseButton.Click
        OpenFileDialog.Title = OpenFileDialog.Title + " " + _
                               LangCombo.Items(LangCombo.SelectedIndex)

        Dim CurrAsm As System.Reflection.Assembly
        CurrAsm = System.Reflection.Assembly.GetExecutingAssembly()

        Dim StrLoc As StringBuilder = New StringBuilder

        Dim StrTemp As String = CurrAsm.Location.ToLower()
        Dim iPos As Int32 = StrTemp.IndexOf("settingsmaster.dll")

        StrLoc.Append(CurrAsm.Location.Substring(0, iPos))

        OpenFileDialog.InitialDirectory = StrLoc.ToString()

        OpenFileDialog.FileName = CurrFileTextBox.Text

        If (OpenFileDialog.ShowDialog() = DialogResult.OK) Then
            Dim Path As String
            Dim File As String
            Dim aPos As Int32

            aPos = OpenFileDialog.FileName.LastIndexOf("\")

            Path = OpenFileDialog.FileName.Substring(0, aPos + 1)
            File = OpenFileDialog.FileName.Substring(aPos + 1, _
                                        OpenFileDialog.FileName.Length - aPos - 1)

            If ((0 <> String.Compare(Path, StrLoc.ToString(), True)) Or _
                (0 <> String.Compare(File, CurrFileTextBox.Text, True))) Then

                CurrFileTextBox.Text = OpenFileDialog.FileName
                Dim Str As String = LangCombo.Items(m_SelIndex)
                m_FileHash(LanguageGUID.NameToGUID(Str)) = CurrFileTextBox.Text

            End If
        End If

    End Sub

    Private Sub EditButton_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles EditButton.Click
        MsgBox("Please read Chapter 9 of Debugging Applications for " + _
               "Microsoft .NET and Microsoft Windows by John Robbins " + _
               "for more information about SettingsMaster.", _
               MsgBoxStyle.Information, _
               "SettingsMaster")
    End Sub
End Class
