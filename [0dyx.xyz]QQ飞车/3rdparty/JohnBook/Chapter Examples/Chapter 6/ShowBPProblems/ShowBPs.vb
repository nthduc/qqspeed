'-----------------------------------------------------------------------
' Debugging Applications for Microsoft .NET and Microsoft Windows
' Copyright © 1997-2003 John Robbins -- All rights reserved.
'-----------------------------------------------------------------------
Imports System.Threading

Public Class ShowBPsForm
    Inherits System.Windows.Forms.Form

#Region " Windows Form Designer generated code "

    Public Sub New()
        MyBase.New()

        'This call is required by the Windows Form Designer.
        InitializeComponent()

        'Add any initialization after the InitializeComponent() call

    End Sub

    'Form overrides dispose to clean up the component list.
    Public Overloads Sub Dispose()
        MyBase.Dispose()
        If Not (components Is Nothing) Then
            components.Dispose()
        End If
    End Sub
    Private WithEvents btnSkipBreaks As System.Windows.Forms.Button
    Private WithEvents edtOutput As System.Windows.Forms.TextBox
    Private WithEvents btnConditionalBreaks As System.Windows.Forms.Button

    'Required by the Windows Form Designer
    Private components As System.ComponentModel.Container

    'NOTE: The following procedure is required by the Windows Form Designer
    'It can be modified using the Windows Form Designer.  
    'Do not modify it using the code editor.
    <System.Diagnostics.DebuggerStepThroughAttribute()> Private Sub InitializeComponent()
        Me.edtOutput = New System.Windows.Forms.TextBox()
        Me.btnConditionalBreaks = New System.Windows.Forms.Button()
        Me.btnSkipBreaks = New System.Windows.Forms.Button()
        Me.SuspendLayout()
        '
        'edtOutput
        '
        Me.edtOutput.Location = New System.Drawing.Point(8, 80)
        Me.edtOutput.Multiline = True
        Me.edtOutput.Name = "edtOutput"
        Me.edtOutput.ScrollBars = System.Windows.Forms.ScrollBars.Vertical
        Me.edtOutput.Size = New System.Drawing.Size(224, 168)
        Me.edtOutput.TabIndex = 1
        Me.edtOutput.Text = "edtOutput"
        '
        'btnConditionalBreaks
        '
        Me.btnConditionalBreaks.Location = New System.Drawing.Point(8, 48)
        Me.btnConditionalBreaks.Name = "btnConditionalBreaks"
        Me.btnConditionalBreaks.Size = New System.Drawing.Size(224, 23)
        Me.btnConditionalBreaks.TabIndex = 2
        Me.btnConditionalBreaks.Text = "&Conditional Breakpoints"
        '
        'btnSkipBreaks
        '
        Me.btnSkipBreaks.Location = New System.Drawing.Point(8, 16)
        Me.btnSkipBreaks.Name = "btnSkipBreaks"
        Me.btnSkipBreaks.Size = New System.Drawing.Size(224, 23)
        Me.btnSkipBreaks.TabIndex = 0
        Me.btnSkipBreaks.Text = "&Skip Breakpoints"
        '
        'ShowBPsForm
        '
        Me.AutoScaleBaseSize = New System.Drawing.Size(5, 13)
        Me.ClientSize = New System.Drawing.Size(242, 255)
        Me.Controls.AddRange(New System.Windows.Forms.Control() {Me.btnConditionalBreaks, Me.edtOutput, Me.btnSkipBreaks})
        Me.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedDialog
        Me.MaximizeBox = False
        Me.Name = "ShowBPsForm"
        Me.StartPosition = System.Windows.Forms.FormStartPosition.CenterScreen
        Me.Text = "Conditional BP Examples"
        Me.ResumeLayout(False)

    End Sub

#End Region


    Private m_TotalSkipPresses As Integer

    Private Sub ShowBPsForm_Load(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles MyBase.Load, MyBase.Load
        ' Always clear the edit control when first popping up.
        edtOutput.Clear()
        ' Initialize globals.
        m_TotalSkipPresses = 0
    End Sub

Private Sub btnSkipBreaks_Click(ByVal sender As System.Object, _
                                ByVal e As System.EventArgs) _
                                Handles btnSkipBreaks.Click
    Dim i As Integer

        ' Clear the output edit control.
    edtOutput.Clear()

    m_TotalSkipPresses += 1

    edtOutput.Text = "Total presses: " + _
                     m_TotalSkipPresses.ToString() + _
                     vbCrLf

    For i = 1 To 10
        ' Append each character to the output edit box.
        edtOutput.Text += i.ToString() + vbCrLf
        ' Force the output edit box to update on each iteration.
        edtOutput.Update()
    Next
End Sub

    Private Sub btnConditionalBreaks_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles btnConditionalBreaks.Click
        Dim i As Integer = 0
        Dim j As Integer = 0

        ' Clearn the output edit control.
        edtOutput.Clear()

        ' Both are on one line to show how BPs can apply to part of a line.
        For i = 1 To 5 : For j = 1 To 5
                ' Do the output
                edtOutput.Text += "i = " + i.ToString() + " j = " + j.ToString() + vbCrLf
                ' For the output to show up.
                edtOutput.Update()
            Next j
        Next i
    End Sub

End Class
