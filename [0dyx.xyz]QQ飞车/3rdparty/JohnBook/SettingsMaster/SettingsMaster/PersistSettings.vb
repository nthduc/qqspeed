'-----------------------------------------------------------------------
' Debugging Applications for Microsoft .NET and Microsoft Windows
' Copyright © 1997 - 2003 John Robbins
'------------------------------------------------------------------------
Imports Microsoft.Win32
Imports System.Text

Public Class PersistSettings

    Private RegKeyStr As String

    Public Sub New(ByVal ApplicationObject As EnvDTE.DTE)
        RegKeyStr = ApplicationObject.RegistryRoot + _
                     "\AddIns\SettingsMaster\Settings"
    End Sub

    Private Function GetBooleanVal(ByVal StrVal As String) As Boolean
        Dim Key As RegistryKey

        Key = Registry.CurrentUser.CreateSubKey(RegKeyStr)

        Dim Val As Int32
        Val = Key.GetValue(StrVal, 1)

        If (1 = Val) Then
            GetBooleanVal = True
        Else
            GetBooleanVal = False
        End If
    End Function

    Private Sub SetBooleanVal(ByVal StrVal As String, ByVal Val As Boolean)
        Dim Key As RegistryKey

        Key = Registry.CurrentUser.CreateSubKey(RegKeyStr)

        Dim SaveVal As Integer = 0
        If (True = Val) Then
            SaveVal = 1
        End If

        Key.SetValue(StrVal, SaveVal)
    End Sub

    Private Const TBVisInDesignMode As String = _
            "ToolbarVisibleInDesignMode"

    Public Property ToolbarVisibleInDesignMode() As Boolean
        Get
            Return GetBooleanVal(TBVisInDesignMode)
        End Get
        Set(ByVal Value As Boolean)
            SetBooleanVal(TBVisInDesignMode, Value)
        End Set
    End Property

    Private Const SPAfterUpdating As String = _
            "SaveProjAfterUpdating"

    Public Property SaveProjAfterUpdating() As Boolean
        Get
            Return GetBooleanVal(SPAfterUpdating)
        End Get
        Set(ByVal Value As Boolean)
            SetBooleanVal(SPAfterUpdating, Value)
        End Set
    End Property

    Const LanguageCount As String = "LanguageCount"

    Public Property LanguageArray() As Hashtable
        Get
            Dim Hash As Hashtable = New Hashtable()

            Dim Key As RegistryKey
            Key = Registry.CurrentUser.CreateSubKey(RegKeyStr)

            Dim Val As Int32
            Val = Key.GetValue(LanguageCount, 0)
            If (Val > 0) Then
                Dim CurrRegStr As String
                Dim StrVal As String
                Dim i As Int32

                For i = 1 To Val
                    CurrRegStr = "Language" + i.ToString()
                    StrVal = Key.GetValue(CurrRegStr, Nothing)
                    If (StrVal <> Nothing) Then
                        Dim Vals As String() = Split(StrVal, "|")
                        If (Vals.Length = 2) Then
                            Hash.Add(Vals(0), Vals(1))
                        Else
                            Throw New _
                             ApplicationException("Invalid registry in" + _
                                                  CurrRegStr + " missing '|'")
                        End If
                    Else
                        Exit For
                    End If
                Next
            Else
                Throw New _
                    ApplicationException("Missing language count in " + _
                                         RegKeyStr)
            End If

            Return Hash

        End Get

        Set(ByVal Value As Hashtable)

            Dim RegKey As RegistryKey
            RegKey = Registry.CurrentUser.CreateSubKey(RegKeyStr)

            RegKey.SetValue(LanguageCount, Value.Count)

            Dim CurrRegStr As New StringBuilder()
            CurrRegStr.Append("Language")
            Dim RegLen = CurrRegStr.Length

            Dim CurrKey As String
            Dim i As Int32 = 1

            Dim CurrVal As New StringBuilder()

            For Each CurrKey In Value.Keys
                CurrRegStr.Append(i.ToString())
                i += 1

                CurrVal.Remove(0, CurrVal.Length)
                CurrVal.Append(CurrKey)
                CurrVal.Append("|")
                CurrVal.Append(Value(CurrKey))

                RegKey.SetValue(CurrRegStr.ToString(), _
                                CurrVal.ToString())

                CurrRegStr.Remove(RegLen, _
                                  CurrRegStr.Length - RegLen)
            Next
        End Set
    End Property

End Class
