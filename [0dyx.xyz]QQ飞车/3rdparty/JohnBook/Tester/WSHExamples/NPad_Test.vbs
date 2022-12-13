''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''
' Debugging Applications for Microsoft .NET and Microsoft Windows
' Copyright © 1997-2003 John Robbins -- All rights reserved.
''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''
' This Windows Script Hosting (WSH) file shows how to automate an
' application with the Tester object.
'
' At the time of this writing, WSH script debugging is not the most
' complete thing in the world.  While the script debugger help mentions
' a "Stop" keyword that will break into the debugger, WSH just seems to
' ignore it.  Consequently, the only way to debug a WSH script is to
' have a message box executed first and while the box is up, switch to
' the script debugger and select "Break at Next Statement" from the
' "Debug" menu.
'
' When debugging with Tester, be very careful that you keep the
' appropriate window at the top of the Z-order.  If you do not, then
' you will be sending keystrokes to whatever window is active, which is
' probably not good.  Just watch it carefully.  In this script, I always
' specifically set the focus before calling TInput.PlayInput.
'
' This script uses the WScript.Echo method to indicate status and
' states.  If you run it with WSCRIPT.EXE, you will get message boxed
' to death.  Use CSCRIPT.EXE.  By using CSCRIPT.EXE you can also
' redirect output so you can compare multiple runs.
''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''
option explicit

''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''
' Constants that are used by the notepad script.
const k_DEBUGMSG_ONE = _
"Here's your chance to debug this script!"
const k_DEBUGMSG_TWO = _
"Press OK to continue the show"
const k_DEBUGMSG_THREE = _
"Don't press any keys or use the mouse after pressing OK!"
const k_SCRIPTTITLE = "Tester (Notepad Example)"

const k_UNABLE_TO_START = "Unable to start and find -> "

const k_NOTEPAD_ALREADY_RUNNING = _
                           "Notepad is already running, cannot continue"

const k_SAVE_FILE_FAILED = "Save file failed!"
const k_OPEN_FILE_FAILED = "Open file failed!"

' The program that this script will run.
const k_PROGRAM = "Notepad.exe"
const k_UNTITLED_TITLE = "Untitled - Notepad"

' The name of the file saved to the script directory.
const k_INTRO_FILENAME = "Intro.txt"


''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''
' Script execution starts here!
''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''
' To get a chance to actually debug, do the message box as the very
' first thing.
MsgBox k_DEBUGMSG_ONE & vbCrLf & _
            k_DEBUGMSG_TWO & vbCrLf & _
            k_DEBUGMSG_THREE _
       , , _
       k_SCRIPTTITLE

' Execute the "main"
main

' This quits the script.
Wscript.Quit
''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''
' Script execution ends here!
''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''

''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''
' The "main" for this script.  Put all the actual script specific stuff
' here.  Support subroutines appear right after this function, but
' before the reusable ones.
''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''
sub main ( )

    ' The main notepad window object.
    dim twinMainWin
    ' The TInput object.
    dim tInput
    ' The TSystem object.
    dim tSystem

    ' Initialize the objects that I will use throught this routine.
    ' Create the system object.
    set tSystem = WScript.CreateObject ( "Tester.TSystem" )
    ' Create the input object.
    set tInput = WScript.CreateObject ( "Tester.TInput" )

    ' Before I get to carried away, make sure another copy of notepad
    ' is not running.
    On Error Resume Next
    set twinMainWin = Nothing
    set twinMainWin = tSystem.FindTopWindowByTitle ( k_UNTITLED_TITLE )
    On Error GoTo 0
    if ( not ( twinMainWin is nothing ) ) then
        WScript.Echo k_NOTEPAD_ALREADY_RUNNING
        exit sub
    end if

    ' First, just try and start notepad.
    set twinMainWin = StartAndFindProgram ( k_PROGRAM , _
                                            k_UNTITLED_TITLE )

    ' Double check that I got the window I was after.  If not, quit.
    if ( twinMainWin is nothing ) then
        WScript.Echo k_UNABLE_TO_START & k_UNTITLED_TITLE
        exit sub
    end if

    ' Now that notepad got started, send some text to it.
    PlayIntroText tSystem , twinMainWin , tInput

    ' Get the name of the script directory, because that it where I will
    ' save the intro file to.
    Dim iNameStart
    iNameStart = InStrRev ( WScript.ScriptFullName , WScript.ScriptName)

    Dim strTextFile
    strTextFile = Mid ( WScript.ScriptFullName , 1 , iNameStart - 1 )

    ' Add the filename on the end.
    strTextFile = strTextFile & k_INTRO_FILENAME

    ' Save the file.
    if ( false = SaveFile ( twinMainWin , strTextFile , true ) ) then
        WScript.Echo k_SAVE_FILE_FAILED
        exit sub
    end if

    ' Do a file new to create a new file.
    tSystem.Sleep 2.0
    twinMainWin.SetForegroundTWindow
    tInput.PlayInput k_FILE_NEW_MENU_KEYS
    tSystem.Sleep 1.0

    ' Now reopen the file.
    if ( false = OpenFile ( tWinMainWin , strTextFile ) ) then
        WScript.Echo k_OPEN_FILE_FAILED
        exit sub
    end if

    ' Do the next test.  Do some edit menu stuff.
    DoEditMenuTest tSystem , twinMainWin , tInput

    ' Clear out the current file.
    twinMainWin.SetForegroundTWindow
    tInput.PlayInput k_FILE_NEW_MENU_KEYS
    tSystem.Sleep 1.0

    ' Tell the prompt that I don't want to save the changes.
    tInput.PlayInput k_BUTTON_NO_ACCEL
    tSystem.Sleep 1.0

    ' Do the nifty help about.
    DoHelpAbout tSystem , twinMainWin , tInput

    ' Kill it!
    tInput.PlayInput k_CLOSE_APPLICATION_ACCEL_KEY
end sub

' A helper routine to send the introduction text to notepad.
' tSys   - The TSystem object to use.
' tWin   - The TWindow to set the focus to.
' tInput - The TInput object to use.
sub PlayIntroText ( tSys , tWin , tInput )

twin.SetForegroundTWindow
tInput.PlayInput "Hello, from Tester!!~"
tInput.PlayInput "The Debugging Applications for Microsoft .NET and "
tInput.PlayInput "Microsoft Windows~Automated Testing Utility!~"
tInput.PlayInput "======================================="
tInput.PlayInput "==========================~~"
tSys.Sleep 1.0

twin.SetForegroundTWindow
tInput.PlayInput "Tester is an ActiveX control that allows you to~"
tInput.PlayInput "easily control another application so that you~"
tInput.PlayInput "can get better testing and code coverage during~"
tInput.PlayInput "your unit tests.~~"
tSys.Sleep 1.0

twin.SetForegroundTWindow
tInput.PlayInput "While Tester might not replace a full blown regression~"
tInput.PlayInput "testing tool, it is perfect for quick and dirty tests.~"
tInput.PlayInput "One of my favorite uses is to just get my applications~"
tInput.PlayInput "started with a test file open.  That way, I don't have~"
tInput.PlayInput "to give myself carpel-tunnel syndrome typing the same~"
tInput.PlayInput "things in time after time.  By making Tester a COM~"
tInput.PlayInput "object, you can use programmable COM hosts so that~"
tInput.PlayInput "you can do more than if you just used a recording tool.~~"
tSys.Sleep 1.0

twin.SetForegroundTWindow
tInput.PlayInput "Myself, I am partial to using the Windows Script Host~"
tInput.PlayInput "{(}WSH{)} as the controller tool.  It is getting better~"
tInput.PlayInput "with each release and future releases will address all~"
tInput.PlayInput "the shortcomings.~~"
tSys.Sleep 1.0

twin.SetForegroundTWindow
tInput.PlayInput "This version of Tester, along with TestRec the recorder~"
tInput.PlayInput "application support keystoke and mouse operation playback!~"
tInput.PlayInput "If you have any suggestions or, better yet, new objects,~"
tInput.PlayInput "please feel free to contact me at www.wintellect.com!~~"
tSys.Sleep 1.0

twin.SetForegroundTWindow
tInput.PlayInput "Sit back and relax.  After a pause of 10 seconds, this~"
tInput.PlayInput "script will run NOTEPAD through it's paces so you can~"
tInput.PlayInput "get an idea what you can do with Tester!~~"

dim i
for i = 1 to 10
    tInput.PlayInput i
    tSys.Sleep 1.0
    tInput.PlayInput "{BACKSPACE}"
next
tInput.PlayInput "{BACKSPACE}"

twin.SetForegroundTWindow
tInput.PlayInput "~ON WITH THE SHOW!~~"
tSys.Sleep 1.0

end sub

' A helper routine to do some gyrations on the edit menu..
' tSys   - The TSystem object to use.
' tWin   - The TWindow to set the focus to.
' tInput - The TInput object to use.
sub DoEditMenuTest ( tSys , tWin , tInput )

' Move back to the beginning of the text.
tWin.SetForegroundTWindow
tInput.PlayInput "^{HOME}"
tSys.Sleep 1.0

' Select all the text.  ^+{END} = CTRL+SHIFT+END
tWin.SetForegroundTWindow
'tInput.PlayInput "^+{END}"
tInput.PlayInput "{SHIFT DOWN}{CTRL DOWN}{END}{SHIFT UP}{CTRL UP}"
tSys.Sleep 1.0

' Copy the text to the clipboard using accelerator keys.
tWin.SetForegroundTWindow
tInput.PlayInput k_EDIT_CUT_ACCEL_KEY
tSys.Sleep 1.0

' Let the user know what I just did.
tWin.SetForegroundTWindow
tInput.PlayInput "I just copied everything to the clipboard.  "
tInput.PlayInput "I am about to paste it back."
tInput.PlayInput "+{HOME}"
tSys.Sleep 3.0

tWin.SetForegroundTWindow
tInput.PlayInput k_EDIT_PASTE_ACCEL_KEY
tSys.Sleep 1.0
end sub

' The cheesy global variable used to communicate between the handler,
' HelpAboutNotification_CreateWindow, and the DoHelpAbout routine.
dim g_SeenAbout
g_SeenAbout = false

sub DoHelpAbout ( tSys , tWin , tInput )

    dim Notifier
    ' Create the notification handler.
    set Notifier = _
            WScript.CreateObject ( "Tester.TNotify"       , _
                                   "HelpAboutNotification"   )

    ' Set the notification.
    Notifier.AddNotification 2 , 2 , "About Notepad"

    ' Send the help about keystokes.
    tInput.PlayInput k_HELP_ABOUT_MENU_KEYS

    ' Wait around a bit.
    tSys.Sleep 2.0

    ' Double check the notifications.  I must do this because the
    ' time to do the notification is so short the WM_TIMER might
    ' not get through.
    Notifier.CheckNotification

    ' Disconnect the events stuff.
    WScript.DisconnectObject Notifier

    if ( false = g_SeenAbout ) then
        WScript.Echo "Help About dialog not started!"
    else
        ' It's there so get rid of it!
        tInput.PlayInput "~"
    end if
    set Notifier = nothing

end sub

' Here's the notification handler set in DoHelpAbout.
sub HelpAboutNotificationCreateWindow ( tWin )
    g_SeenAbout = true
end sub



''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''
''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''
' Reusable functions and constants are below.
' You can clip these out and use them in your Tester scripts.
''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''
''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''

''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''
' The American English string constants
''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''
''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''
' The File Menu Constants
''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''
' The menu keystrokes
public const k_FILE_MENU_KEY            = "%f"
public const k_FILE_NEW_MENU_KEYS       = "%fn"
public const k_FILE_OPEN_MENU_KEYS      = "%fo"
public const k_FILE_SAVE_MENU_KEYS      = "%fs"
public const k_FILE_SAVE_AS_MENU_KEYS   = "%fa"
public const k_FILE_PRINT_MENU_KEYS     = "%fp"
public const k_FILE_EXIT_MENU_KEYS      = "%fx"

' The accelerators
public const k_FILE_NEW_ACCEL_KEY       = "^n"
public const k_FILE_OPEN_ACCEL_KEY      = "^o"
public const k_FILE_SAVE_ACCEL_KEY      = "^s"
public const k_FILE_PRINT_ACCEL_KEY     = "^p"

''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''
' The Edit Menu constants
''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''
public const k_EDIT_MENU_KEY            = "%e"
public const k_EDIT_UNDO_MENU_KEYS      = "%eu"
public const k_EDIT_CUT_MENU_KEYS       = "%et"
public const k_EDIT_COPY_MENU_KEYS      = "%ec"
public const k_EDIT_PASTE_MENU_KEYS     = "%ep"
public const k_EDIT_SELECT_ALL_MENU_KEYS = "%el"
public const k_EDIT_FIND_MENU_KEYS      = "%ef"
public const k_EDIT_FIND_NEXT_MENU_KEYS = "%en"
public const k_EDIT_REPLACE_MENU_KEYS   = "%ee"

public const k_EDIT_UNDO_ACCEL_KEY      = "^z"
public const k_EDIT_CUT_ACCEL_KEY       = "^x"
public const k_EDIT_COPY_ACCEL_KEY      = "^c"
public const k_EDIT_PASTE_ACCEL_KEY     = "^v"
public const k_EDIT_SELECT_ALL_ACCEL_KEY = "^a"
public const k_EDIT_FIND_ACCEL_KEY      = "^f"
public const k_EDIT_FIND_NEXT_ACCEL_KEY = "{F3}"
public const k_EDIT_REPLACE_ACCEL_KEY   = "^h"

public const k_CLOSE_APPLICATION_ACCEL_KEY = "%{F4}"

''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''
' The Window Menu constants
''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''
public const k_WINDOW_MENU_KEY          = "%w"
public const k_WINDOW_NEW_MENU_KEYS     = "%wn"
public const k_WINDOW_CASCADE_MENU_KEYS = "%wc"
public const k_WINDOW_TILE_MENU_KEYS    = "%wt"
public const k_WINDOW_ARRANGE_ICONS_MENU_KEYS = "%wa"

public const k_NEXT_MDICHILD_WINDOW_ACCEL_KEY = "^{F6}"
public const k_CLOSE_MDICHILD_WINDOW_ACCEL_KEY = "^{F4}"

''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''
' The Help Menu constants
''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''
public const k_HELP_MENU_KEY            = "%h"
public const k_HELP_ABOUT_MENU_KEYS     = "%ha"

public const k_HELP_ACCEL_KEY           = "{F1}"

''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''
' Button text constants.
''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''
public const k_BUTTON_OK_TEXT           = "&Ok"
public const k_BUTTON_YES_TEXT          = "&Yes"
public const k_BUTTON_NO_TEXT           = "&No"
public const k_BUTTON_CANCEL_TEXT       = "&Cancel"

public const k_BUTTON_YES_ACCEL         = "&y"
public const k_BUTTON_NO_ACCEL          = "&n"
public const k_BUTTON_OK_ACCEL          = "&o"
public const k_BUTTON_CANCEL_ACCEL      = "&c"

''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''
' Common window titles.
''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''
' The "Save As" dialog box.
public const k_TITLE_SAVE_AS            = "Save As"
' The first child window title inside the "Save As" dialog.
public const k_TITLE_SAVE_IN            = "Save &in:"
' The "Open" dialog box.
public const k_TITLE_OPEN               = "Open"
' The first child window title inside the "Open" dialog.
public const k_TITLE_LOOK_IN            = "Look &in:"

''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''
' Error strings and codes used by the reusable functions.
''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''
const k_INVALID_PARAMS_NUMBER = 1
const k_INVALID_PARAMS_STRING = "Invalid Parameter(s)"

''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''
' This function starts the specified program and returns the TWindow
' object for the main window.  The first instance of the specified
' window is returned so if there are multiple instances of the
' application already running, this can return the wrong one.
' strProg  - The name of the program to start.  This can include command
'            line options and such.
' strTitle - The title bar of the application after starting the app in
'            strProg.
''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''
function StartAndFindProgram ( strProg , strTitle )

    ' Check the paramters.
    if ( ( "" = strProg ) or ( "" = strTitle ) ) then
        ' Error out.
        err.raise vbObjectError + k_INVALID_PARAMS_NUMBER , _
                  "StartAndFindProgram"                   , _
                  k_INVALID_PARAMS_STRING
    end if

    ' Assume the worst.
    set StartAndFindProgram = nothing

    ' Params are good.  Try and start the program.
    dim WshShell
    set WshShell = WScript.CreateObject ( "WScript.Shell" )

    WshShell.Run ( strProg )

    set WshShell = nothing

    ' Try and find the window.
    dim twinRet
    dim tSys

    ' Create the TSystem object.
    set tSys = WScript.CreateObject ( "Tester.TSystem" )

    ' Pause for a bit to allow the window to pop up.
    tSys.Sleep 1.0

    ' Hunt down the window.
    set twinRet = tSys.FindTopWindowByTitle ( strTitle )

    ' Get rid of the system object.
    set tSys = nothing

    ' return whatever was found.  This could be "nothing"
    set StartAndFindProgram = twinRet

end function

''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''
' The function to save a file.
' tWin          - The window to send the ALT+F+S input to.
' strFilename   - The filename for the saved file.
' bOverwrite    - True to overwrite the any existing file.
'
' Returns
' True  - The file was saved.
' False - There was a problem and the state is probably screwed up.
''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''
function SaveFile ( tWin , strFilename , bOverwrite )

    ' A temporary TSystem object.  I need it to pause a few moments.
    dim tSys
    ' A temporary TInput object.
    dim tInput

    set tSys = WScript.CreateObject ( "Tester.TSystem" )
    set tInput = WScript.CreateObject ( "Tester.TInput" )

    ' Set the forground window the the tWin passed in.
    tWin.SetForegroundTWindow

    ' Pause a bit.
    tSys.Sleep 1.0

    ' Send the File|Save to the window.
    tInput.PlayInput k_FILE_SAVE_MENU_KEYS

    ' Pause a bit and then take a look at the window that popped up.
    tSys.Sleep 1.0

    dim tWinCurr

    set tWinCurr = tSys.GetForegroundTWindow

    ' If the title is not "Save As", then I don't know how to continue.
    if ( k_TITLE_SAVE_AS <> tWinCurr.Caption ) then
        SaveFile = false
        exit function
    end if

    ' Do a little enumeration of the child windows to ensure this is
    ' really the good ol' Save As dialog.  I do this by checking the
    ' first child window caption.
    dim twinChildCol

    set twinChildCol = tWinCurr.EnumChildren

    if ( k_TITLE_SAVE_IN <> twinChildCol(1).Caption ) then
        SaveFile = false
        exit function
    end if

    ' The return boolean.
    dim bRet
    bRet = false

    ' The cursor is sitting in the right spot so just play the keys
    tInput.PlayInput strFilename

    ' Pause and play the enter key.
    tSys.Sleep 1.0
    tInput.PlayInput "~"
    tSys.Sleep 1.0

    ' The file might already exist, so take a check for the Save As
    ' dialog.
    set tWinCurr = tSys.GetForegroundTWindow

    if ( k_TITLE_SAVE_AS = tWinCurr.Caption ) then
        ' Enum this windows children.  If the first two are &Yes and
        ' &No, then overwrite the function if the user wants.
        set twinChildCol = tWinCurr.EnumChildren
        if ( ( k_BUTTON_YES_TEXT = twinChildCol(1).Caption ) And _
             ( k_BUTTON_NO_TEXT  = twinChildCol(2).Caption )       )then
             if ( true = bOverwrite ) then
                ' Overwrite it!
                tInput.PlayInput k_BUTTON_YES_ACCEL
                bRet = True
             end if
        end if
    end if

    ' Check if tWinCurr is equal to tWin.  If they are equal, then the
    ' file was a new file.
    if ( tWin.hWnd = tWinCurr.hWnd ) then
        bRet = True
    end if

    ' End it.
    SaveFile = bRet
end function

''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''
' The function to open a file.  Before calling this function, make sure
' the application is ready to recieve a new file.  For MDI applications,
' that should require any work, but SDI documents might need to be
' cleared or saved.
' tWin          - The window to send the ALT+F+O input to.
' strFilename   - The filename for the file to open.
'
' Returns
' True  - The file was opened.
' False - There was a problem and the state is probably screwed up.
''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''
function OpenFile ( tWin , strFilename )

    ' A temporary TSystem object.  I need it to pause a few moments.
    dim tSys
    ' A temporary TInput object.
    dim tInput

    set tSys = WScript.CreateObject ( "Tester.TSystem" )
    set tInput = WScript.CreateObject ( "Tester.TInput" )

    ' Set the forground window the the tWin passed in.
    tWin.SetForegroundTWindow

    ' Pause a bit.
    tSys.Sleep 1.0

    ' Send the File|Open keystrokes.
    tInput.PlayInput k_FILE_OPEN_MENU_KEYS

    ' Pause a bit and then take a look at the window that popped up.
    tSys.Sleep 1.0

    dim tWinCurr

    set tWinCurr = tSys.GetForegroundTWindow

    ' If the title is not "Save As", then I don't know how to continue.
    if ( k_TITLE_OPEN <> tWinCurr.Caption ) then
        OpenFile = false
        exit function
    end if

    ' Do a little enumeration of the child windows to ensure this is
    ' really the good ol' Save As dialog.  I do this by checking the
    ' first child window caption.
    dim twinChildCol

    set twinChildCol = tWinCurr.EnumChildren

    if ( k_TITLE_LOOK_IN <> twinChildCol(1).Caption ) then
        OpenFile = false
        exit function
    end if

    ' The return boolean.
    dim bRet
    bRet = false

    ' The cursor is sitting in the right spot so just play the keys
    tInput.PlayInput strFilename

    tSys.Sleep 1.0

    tInput.PlayInput "~"

    tSys.Sleep 1.0

    ' Get the window with focus.
    set tWinCurr = tSys.GetForegroundTWindow

    ' Check if tWinCurr is equal to tWin.  If they are equal, then the
    ' file file was opened.
    if ( tWin.hWnd = tWinCurr.hWnd ) then
        bRet = true
    end if

    OpenFile = bRet

end function
