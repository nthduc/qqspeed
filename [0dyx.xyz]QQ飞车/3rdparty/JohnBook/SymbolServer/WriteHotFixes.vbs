''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''
' Debugging Microsoft .NET and Windows Applications
' Copyright 1997 - 2002 by John Robbins
'
' Writes out the hot fixes applied to a machine to a file called
' HOTFIXES.TXT.
' I have to do this because JScript and WMI DO NOT get along when it
' comes to passing array/safearray parameters.
''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''

const HKEY_CLASSES_ROOT  = &H80000000
const HKEY_CURRENT_USER  = &H80000001
const HKEY_LOCAL_MACHINE = &H80000002
const HKEY_USERS         = &H80000003
const strHotFixKey = _
            "SOFTWARE\Microsoft\Windows NT\CurrentVersion\HotFix"
const strOutFile = "HotFixes.TXT"

set WshNetwork     = WScript.CreateObject ( "WScript.Network" )
dim strComputer
strComputer = WshNetwork.ComputerName

' Create up all the WMI stuff.
' Do the SWbemLocator.
set wmiLocator = CreateObject ( "WbemScripting.SWbemLocator" )
' Make the connection to the server and get to the correct namespace.
set NameSpace = wmiLocator.ConnectServer ( strComputer, "root\default" )
' Get the Registry Provider.
set objRegistry = NameSpace.Get ( "StdRegProv" )

' Hold the return value from EnumKey.
dim lcRC
' Holds the array of strings.
dim aKeys

' Do the enumeration.
lcRc = objRegistry.EnumKey ( HKEY_LOCAL_MACHINE , strHotFixKey , aKeys )

if ( 0 = lcRC ) then
    ' Got the keys enumerated.  Now open the file and write them out.
    set fso = CreateObject ( "Scripting.FileSystemObject" )
    set OutFile = fso.CreateTextFile ( strOutFile , true )

    dim Str
    for each Str in aKeys
        dim StartChr
        StartChr = Left ( Str , 1 )
        if ( "Q" = StartChr ) then
            OutFile.Write Str + " "
        end if
    next
else
    WScript.Echo "Unable to enumerate the hot fix key!!"
end if
