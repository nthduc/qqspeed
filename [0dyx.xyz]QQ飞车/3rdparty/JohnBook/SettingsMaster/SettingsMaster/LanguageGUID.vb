'-----------------------------------------------------------------------
' Debugging Applications for Microsoft .NET and Microsoft Windows
' Copyright © 1997 - 2003 John Robbins
'------------------------------------------------------------------------
Module Wintellect

    Public Class LanguageGUID

        Public Shared Function GUIDToName(ByVal Str As String) As String
            Select Case Str
                Case EnvDTE.CodeModelLanguageConstants.vsCMLanguageCSharp
                    GUIDToName = "C#"
                Case EnvDTE.CodeModelLanguageConstants.vsCMLanguageIDL
                    GUIDToName = "IDL"
                Case EnvDTE.CodeModelLanguageConstants.vsCMLanguageMC
                    GUIDToName = "MC++"
                Case EnvDTE.CodeModelLanguageConstants.vsCMLanguageVB
                    GUIDToName = "VB.NET"
                Case EnvDTE.CodeModelLanguageConstants.vsCMLanguageVC
                    GUIDToName = "VC++"
                Case Else
                    Debug.Assert(False)
                    GUIDToName = "**UNKNOWN LANGUAGE!!**"
            End Select
        End Function

        Public Shared Function NameToGUID(ByVal Str As String) As String
            Select Case Str
                Case "C#"
                    NameToGUID = EnvDTE.CodeModelLanguageConstants.vsCMLanguageCSharp
                Case "IDL"
                    NameToGUID = EnvDTE.CodeModelLanguageConstants.vsCMLanguageIDL
                Case "MC++"
                    NameToGUID = EnvDTE.CodeModelLanguageConstants.vsCMLanguageMC
                Case "VB.NET"
                    NameToGUID = EnvDTE.CodeModelLanguageConstants.vsCMLanguageVB
                Case "VC++"
                    NameToGUID = EnvDTE.CodeModelLanguageConstants.vsCMLanguageVC
                Case Else
                    Debug.Assert(False)
                    NameToGUID = "**UNKNOWN LANGUAGE!!**"
            End Select
        End Function

        Public Shared Function IsKnownGUID(ByVal Str As String) As Boolean
            Select Case Str
                Case EnvDTE.CodeModelLanguageConstants.vsCMLanguageCSharp
                    IsKnownGUID = True
                Case EnvDTE.CodeModelLanguageConstants.vsCMLanguageIDL
                    IsKnownGUID = True
                Case EnvDTE.CodeModelLanguageConstants.vsCMLanguageMC
                    IsKnownGUID = True
                Case EnvDTE.CodeModelLanguageConstants.vsCMLanguageVB
                    IsKnownGUID = True
                Case EnvDTE.CodeModelLanguageConstants.vsCMLanguageVC
                    IsKnownGUID = True
                Case Else
                    IsKnownGUID = False
            End Select
        End Function

    End Class

End Module
