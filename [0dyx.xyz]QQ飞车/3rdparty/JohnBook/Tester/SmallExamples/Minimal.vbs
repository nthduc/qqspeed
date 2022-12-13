'-----------------------------------------------------------------------
' Debugging Applications for Microsoft .NET and Microsoft Windows
' Copyright © 1997-2003 John Robbins -- All rights reserved.
'-----------------------------------------------------------------------
' A minimal VBScript Tester example. It just starts Notepad, enters a
' few lines of text, and closes Notepad.

' Get the system and input objects.
Dim tSystem
Dim tInput
Dim tWin
Set tSystem = WScript.CreateObject ( "Tester.TSystem" )
Set tInput = WScript.CreateObject ( "Tester.TInput" )

' Start Notepad.
tSystem.Execute "NOTEPAD.EXE"

' Wait a few seconds.
tSystem.Sleep 3.0

' Try to find Notepad's main window.
Set tWin = tSystem.FindTopTWindowByTitle ( "Untitled - Notepad" )
If ( tWin Is Nothing ) Then
    MsgBox "Unable to find Notepad!"
    WScript.Quit
End If

' Ensure that Notepad is in the foreground.
tWin.SetForegroundTWindow

' Type something.
tInput.PlayInput "Be all you can be!~~~"
' Play it again, Sam.
tInput.PlayInput "Put on your boots and parachutes....~~~"
' Third time's the charm.
tInput.PlayInput "Silver wings upon their chests.....~~~"

' Wait 3 seconds.
tSystem.Sleep 3.0

' End Notepad.
tInput.PlayInput "%FX"
tSystem.Sleep 2.0
tInput.PlayInput "{TAB}~"

' Script is done!

