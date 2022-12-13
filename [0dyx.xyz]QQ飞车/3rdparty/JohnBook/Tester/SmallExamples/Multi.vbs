'-----------------------------------------------------------------------
' Debugging Applications for Microsoft .NET and Microsoft Windows
' Copyright © 1997-2003 John Robbins -- All rights reserved.
'-----------------------------------------------------------------------
' Another WSH script to show the window notification handlers.

' You can run this one at the same time as Handler.vbs so that you
' can see multiple scripts handling notifications at the same time.

' RUN THIS SCRIPT WITH CSCRIPT.EXE!  If you use WSCRIPT.EXE, the
' WScript.Echo output goes to message boxes which can screw up the
' focus for Handler.vbs

' Constants for the TNotification.AddNotification routine.
const antDestroyWindow    = 1
const antCreateWindow     = 2
const antCreateAndDestroy = 3

const ansExactMatch       = 0
const ansBeginMatch       = 1
const ansAnyLocMatch      = 2


' The notification variable.
dim Notifier
' Create the notification handler.
set Notifier = _
        WScript.CreateObject ( "Tester.TNotify"      , _
                               "NotepadNotification"     )

' Add the notifications that I want.  For this demo, I want both destroy
' and create.  See the TNotify object for all the gyrations.
Notifier.AddNotification  antCreateAndDestroy , _
                          ansAnyLocMatch      , _
                          "Notepad"

MsgBox "Ho humm, waiting around for notifications from Windows with 'Notepad' in their title.  Press OK to end this script!"

' Disconnect the notifications.  If you don't do this in WSH, the class
' terminate never gets called so the notification is still active in the
' notification table.
WScript.DisconnectObject Notifier

set Notifier = Nothing

WScript.Quit

sub NotepadNotificationCreateWindow ( tWin )
    WScript.Echo ( "Multiple watch reports : A window with Notepad in the caption was created!!" )
end sub

sub NotepadNotificationDestroyWindow ( )
    WScript.Echo ( "Multiple watch reports : A window with Notepad in the caption has gone away...." )
end sub
