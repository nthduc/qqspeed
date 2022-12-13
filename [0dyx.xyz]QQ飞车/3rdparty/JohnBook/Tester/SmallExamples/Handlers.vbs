'-----------------------------------------------------------------------
' Debugging Applications for Microsoft .NET and Microsoft Windows
' Copyright © 1997-2003 John Robbins -- All rights reserved.
'-----------------------------------------------------------------------
' A VBScript test to show the window notification handlers

' Constants for the TNotify.AddNotification routine.
Const antDestroyWindow    = 1
Const antCreateWindow     = 2
Const antCreateAndDestroy = 3

Const ansExactMatch       = 0
Const ansBeginMatch       = 1
Const ansAnyLocMatch      = 2

' Get the system and input objects.

Dim tSystem
Dim tInput
Set tSystem = WScript.CreateObject ( "Tester.TSystem" )
Set tInput = WScript.CreateObject ( "Tester.TInput" )


' The TNotify object variable
Dim Notifier

' Create the TNotify object.
Set Notifier = _
        WScript.CreateObject ( "Tester.TNotify"      , _
                               "NotepadNotification"     )

' Add the notifications that I want. For this demonstration, I want both
' window destroy and window create notifications. See the TNotify source
' code for all the possible notification combinations.
Notifier.AddNotification  antCreateAndDestroy , _
                          ansAnyLocMatch      , _
                          "Notepad"

' Start Notepad.
tSystem.Execute "NOTEPAD.EXE"

' Pause for one seconds.
tSystem.Sleep 1.0

' Because the apartment model isn't thread-safe, I set up the
' notification scheme to use a timer. However, the message can get
' blocked because all the processing is forced to a single thread. This
' function lets you manually check the window create and window destroy
' conditions.
Notifier.CheckNotification

' The message box in the NotepadNotification_CreateWindow event
' procedure blocks, so the code to end Notepad won't execute until the
' message box is cleared.
tInput.PlayInput "%FX"
tSystem.Sleep 1.0

' Again, check notifications.
Notifier.CheckNotification

' Give TNotify a chance to intercept the window destroy message.
tSystem.Sleep 1.0

' Disconnect the notifications. If you don't do this in WSH, the class
' terminate never gets called so the notification is still active in the
' notification table.
WScript.DisconnectObject Notifier

Set Notifier = Nothing

WScript.Quit


Sub NotepadNotificationCreateWindow ( tWin )
    MsgBox ( "Notepad was created!!" )
End Sub

Sub NotepadNotificationDestroyWindow ( )
    MsgBox ( "Notepad has gone away...." )
End Sub

