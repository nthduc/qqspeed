/*----------------------------------------------------------------------
Debugging Applications for Microsoft .NET and Microsoft Windows
Copyright © 1997-2003 John Robbins -- All rights reserved.
------------------------------------------------------------------------
The javascript file that takes care of inserting an operating
system's binaries and symbols into a symbol server

A WORD OF WARNING:
Because of the way I have to spawn SYMCHK.EXE, if you use CTRL+C to
stop execution, SYMCHK.EXE might not stop.  If you stop execution,
issue a 'KILL SYMCHK.EXE' to kill it.
----------------------------------------------------------------------*/

/*//////////////////////////////////////////////////////////////////////
// Necessary globals.
// I'll never understand why these aren't first class object like
// WScript.
//////////////////////////////////////////////////////////////////////*/
var WSHSHELL = WScript.CreateObject ( "WScript.Shell" ) ;
var FSO = WScript.CreateObject ( "Scripting.FileSystemObject" ) ;

/*//////////////////////////////////////////////////////////////////////
// Project globals.
//////////////////////////////////////////////////////////////////////*/
// The minimum set of OS data you want to do.
var g_WorkToDo = new Array
(
    new SymbolsToProcess ( "%SYSTEMROOT%"   ,   // Start directory.
                           "*.dll"          ,   // Searching wildcard.
                           "DllBinLog.TXT"  ,   // Binary logging file.
                           "DllSymLog.TXT"   )  // Symbol logging file.
) ;

// The EXE set of work.
var g_ExeWork = new SymbolsToProcess ( "%SYSTEMROOT%"   ,
                                       "*.exe"          ,
                                       "ExeBinLog.TXT"  ,
                                       "ExeSymLog.TXT"   ) ;

// Add your own work sets here for new Microsoft applications such as
// SQL server.  Add anything here as an array like is done in
// g_WorkToDo.
var g_AdditionalWork = null ;

/*//////////////////////////////////////////////////////////////////////
// Execution start here!
//////////////////////////////////////////////////////////////////////*/
var RetVal = main ( ) ;
WScript.Quit ( RetVal ) ;
/*//////////////////////////////////////////////////////////////////////
// Execution ends here!
//////////////////////////////////////////////////////////////////////*/

/*//////////////////////////////////////////////////////////////////////
// Functions start here.
//////////////////////////////////////////////////////////////////////*/

// The entry point.
function main ( )
{
    var RetValue = 0 ;

    var Args = new CmdLineArgs ( ""     ,
                                 false  ,
                                 false  ,
                                 false  ,
                                 false  ,
                                 false  ,
                                 false   ) ;

    // Parse up the command line args.
    var ParseRes = ParseCommandLine ( Args ) ;
    if ( true == ParseRes )
    {
        // Get all the OS info so I can pass it as parameters to
        // SYMSTORE.
        var OSInfo = new OSVersionInfo ( "" , "" , "" ) ;
        if ( true == GetOSInfo ( OSInfo ) )
        {
            if ( true == Args.DebugScript )
            {
                WScript.Echo ( "OS Version Info :" ) ;
                WScript.Echo ( "  Product       : " + OSInfo.Product ) ;
                WScript.Echo ( "  Version       : " + OSInfo.Version ) ;
                WScript.Echo ( "  Comment       : " + OSInfo.Comment ) ;
                WScript.Echo ( "\n" ) ;
            }

            // Am I supposed to do EXEs?
            if ( true == Args.DoEXEs )
            {
                // Poink those onto the end.
                g_WorkToDo.push ( g_ExeWork ) ;
            }

            // Tack on any extra work subsequent users might want.
            if ( null != g_AdditionalWork )
            {
                g_WorkToDo = g_WorkToDo.concat ( g_AdditionalWork ) ;
            }

            var SymStoreErr = "\Failure executing SYMSTORE.EXE."
            var SymChkErr = "\nFailure executing SYMCHK.EXE."

            for ( i = 0 ; i < g_WorkToDo.length ; i++ )
            {
                if ( false == Args.SkipBinaries )
                {
                    RetValue = ExecuteSymStore ( Args            ,
                                                 OSInfo          ,
                                                 g_WorkToDo[ i ]  ) ;
                }
                if ( 0 == RetValue )
                {
                    if ( false == Args.SkipSymbols )
                    {
                        RetValue = ExecuteSymChk ( Args            ,
                                                   OSInfo          ,
                                                   g_WorkToDo[ i ]  ) ;
                    }
                    if ( 0 != RetValue )
                    {
                        WScript.Echo ( SymChkErr ) ;
                        break ;
                    }
                }
                else
                {
                    WScript.Echo ( SymStoreErr ) ;
                    break ;
                }

            }
        }
        else
        {
            WScript.Echo ( "\nUnable to get OS version information." ) ;
            RetValue = 2 ;
        }
    }
    else
    {
        ShowHelp ( ) ;
        if ( false == Args.JustShowHelp )
        {
            WScript.Echo ( "\nInvalid command line argument" ) ;
            RetValue = 3 ;
        }
    }
    return ( 3 ) ;
}

// The class that holds the command line ags.
function CmdLineArgs ( SymbolServer ,
                       DoEXEs       ,
                       DoVerbose    ,
                       JustShowHelp ,
                       DebugScript  ,
                       SkipBinaries ,
                       SkipSymbols   )
{
    this.SymbolServer = SymbolServer ;
    this.DoEXEs       = DoEXEs ;
    this.DoVerbose    = DoVerbose ;
    this.JustShowHelp = JustShowHelp ;
    this.DebugScript  = DebugScript ;
    this.SkipBinaries = SkipBinaries ;
    this.SkipSymbols  = SkipSymbols ;
}

// The class that holds the OS version info.
function OSVersionInfo ( Product , Version , Comment )
{
    this.Product = Product ;
    this.Version = Version ;
    this.Comment = Comment ;
}

// The class that holds the work to do.
function SymbolsToProcess ( StartingDir ,
                            WildCard    ,
                            BinaryLog   ,
                            SymbolsLog   )
{
    // The starting directory.  This can be an environment variable.
    this.StartingDir = StartingDir ;
    // The wild card to do.
    this.WildCard    = WildCard ;
    // The log file for binarys matched in the wild card.
    this.BinaryLog   = BinaryLog ;
    // The log file for symbols matched in the wild card.
    this.SymbolsLog  = SymbolsLog ;
}

// Execute SYMSTORE.EXE.  Returns 0 if everything went well.
function ExecuteSymStore ( Args , OSInfo , SymsToDo )
{
    // Build up the input file string.
    var InputFiles =
            WSHSHELL.ExpandEnvironmentStrings ( SymsToDo.StartingDir ) ;
    InputFiles += "\\" ;
    InputFiles += SymsToDo.WildCard ;

    // Delete any old input file.  SYMSTORE.EXE just keeps appending.
    if ( FSO.FileExists ( SymsToDo.BinaryLog ) )
    {
        FSO.DeleteFile ( SymsToDo.BinaryLog ) ;
    }

    // Build up the command to execute.
    var Cmd = "SYMSTORE add /r /f " ;
    Cmd += InputFiles ;
    Cmd += " /s " ;
    Cmd += Args.SymbolServer ;
    Cmd += " /t \"" ;
    Cmd += OSInfo.Product ;
    Cmd += "\" /v \"" ;
    Cmd += OSInfo.Version ;
    Cmd += "\" /c \"" ;
    Cmd += OSInfo.Comment ;
    Cmd += "\" /d \"" ;
    Cmd += SymsToDo.BinaryLog ;
    Cmd += "\"" ;

    if ( true == Args.DoVerbose )
    {
        Cmd += " /o" ;
    }

    WScript.Echo ( "Executing SYMSTORE.EXE on " + SymsToDo.WildCard ) ;
    WScript.Echo ( "   Log written to " + SymsToDo.BinaryLog ) ;

    var CmdRetVal = 0 ;
    if ( true == Args.DebugScript )
    {
        WScript.Echo ( "\nCommand Line:" ) ;
        WScript.Echo ( Cmd ) ;
        WScript.Echo ( "\n" ) ;
    }
    else
    {
        CmdRetVal = WSHSHELL.Run ( Cmd , 0 , true ) ;
    }
    return ( CmdRetVal ) ;
}

// Executes SYMCHK.EXE.  Returns 0 if life is good.
function ExecuteSymChk ( Args , OSInfo , SymsToDo )
{
    // Build up the input file string.
    var InputFiles =
            WSHSHELL.ExpandEnvironmentStrings ( SymsToDo.StartingDir ) ;
    InputFiles += "\\" ;
    InputFiles += SymsToDo.WildCard ;

    var SymPath = "SRV*"
    SymPath += Args.SymbolServer ;
    SymPath += "*" ;
    SymPath += "http://msdl.microsoft.com/download/symbols" ;

    // Delete any old input file.  SYMSTORE.EXE just keeps appending.
    if ( FSO.FileExists ( SymsToDo.SymbolsLog ) )
    {
        FSO.DeleteFile ( SymsToDo.SymbolsLog ) ;
    }

    var ComSpec = WSHSHELL.ExpandEnvironmentStrings ( "%comspec%" ) ;

    // Build up the command to execute.
    var Cmd = ComSpec + " /C SYMCHK /r " ;
    Cmd += InputFiles ;
    Cmd += " /s " ;
    Cmd += SymPath ;
    Cmd += " > " ;
    Cmd += SymsToDo.SymbolsLog ;

    WScript.Echo ( "Executing SYMCHK.EXE on " + SymsToDo.WildCard ) ;
    WScript.Echo ( "   Log written to " + SymsToDo.SymbolsLog ) ;

    var CmdRetVal = 0 ;
    if ( true == Args.DebugScript )
    {
        WScript.Echo ( "\nCommand Line:" ) ;
        WScript.Echo ( Cmd ) ;
        WScript.Echo ( "\n" ) ;
    }
    else
    {
        CmdRetVal = WSHSHELL.Run ( Cmd , 0 , true ) ;

        // Since I'm spawning SYMCHK through CMD.EXE, I'll always get
        // a return value of 1.  In order to determine if the execution
        // worked, I'll have to look at the output file.  Ugly, but
        // it works.
        var InFile = FSO.OpenTextFile ( SymsToDo.SymbolsLog ,
                                        1                   ,
                                        false                ) ;
        var InStr = null ;
        try
        {
            InStr = InFile.ReadLine ( ) ;
        }
        catch ( e )
        {
            InStr = null ;
        }
        InFile.Close ( ) ;

        var Re = /SYMCHK/ ;
        if ( null != InStr )
        {
            if ( null == InStr.match ( Re ) )
            {
                CmdRetVal = 1 ;
            }
            else
            {
                CmdRetVal = 0 ;
            }
        }
        else
        {
            CmdRetVal = 0 ;
        }
    }
    return ( CmdRetVal ) ;
}

function GetOSInfo ( OSInfo )
{
    var RetValue = false ;
    OSInfo.Product = GetOSName ( ) ;
    OSInfo.Version = GetOSServicePack ( ) ;

    // At least start the comment string off with the date this command
    // was run.
    var d = new Date ( ) ;
    OSInfo.Comment += ( d.getMonth ( ) + 1 ) + "/" ;
    OSInfo.Comment += d.getDate ( ) + "/" ;
    OSInfo.Comment += d.getYear ( ) ;

    var HotFixStr = GetHotFixesApplied ( ) ;
    if ( "" != HotFixStr )
    {
        OSInfo.Comment += " " ;
        OSInfo.Comment += HotFixStr ;
    }

    if ( ( "" == OSInfo.Product ) ||
         ( "" == OSInfo.g_Version )   )
    {
        RetValue = false ;
    }
    else
    {
        RetValue = true ;
    }
    return ( RetValue ) ;

}
// Returns the OSName as a string.
function GetOSName ( )
{
    var ResStr = "" ;
    try
    {
        var ProdPath = "HKLM\\SOFTWARE\\Microsoft\\Windows NT" ;
        ProdPath = ProdPath + "\\CurrentVersion\\ProductName" ;

        ResStr = WSHSHELL.RegRead ( ProdPath ) ;
    }
    catch ( e )
    {
        ResStr = "" ;
    }
    return ( ResStr ) ;
}

// Returns the OS SP level as a string.
function GetOSServicePack ( )
{
    var ResStr = "" ;
    try
    {
        var ProdPath = "HKLM\\SOFTWARE\\Microsoft\\Windows NT" ;
        ProdPath = ProdPath + "\\CurrentVersion\\CSDVersion" ;

        ResStr = WSHSHELL.RegRead ( ProdPath ) ;
    }
    catch ( e )
    {
        ResStr = "" ;
    }
    return ( ResStr ) ;
}

// Returns the list of hotfixes applied as a string.
function GetHotFixesApplied ( )
{
    // Yes, this is pretty dumb.  JScript will not properly handle the
    // WMI, StdRegProv.EnumKey method that returns an variant array.
    // However, VBScript does.  I'll spawn off the VBScript file and
    // generate a text file that contains the list.  Yes, this is really
    // ugly.
    var FileName = "HotFixes.TXT" ;
    var Cmd = "WriteHotFixes.vbs" ;
    var Str = "" ;

    WSHSHELL.Run ( Cmd , 0 , true ) ;

    if ( true == FSO.FileExists ( FileName ) )
    {
        // The file exits so read it into the comment variable.
        var InFile = FSO.OpenTextFile ( FileName , 1 , false );
        var InStr = InFile.ReadAll ( ) ;
        Str += InStr ;
        InFile.Close ( ) ;
        InFile = null ;
        // I no longer need the file.
        FSO.DeleteFile ( FileName ) ;
    }
    return ( Str ) ;
}

// Parses up the command line options.  This returns true if the command
// line options were correct.
function ParseCommandLine ( Args )
{
    // Do the easy cases.
    if ( 0 == WScript.Arguments.length )
    {
        Args.JustShowHelp = true ;
        return ( false ) ;
    }

    // The return value for this function.
    var RetVal = true ;

    // The first argument is the symbol server and is the only required
    // argument.
    Args.SymbolServer = WScript.Arguments(0).toUpperCase ( ) ;

    // Do some regex action to validate this looks like a symbol server
    // path.
    var Re = /\\\\[A-Za-z0-9]+\\[A-Za-z0-9]+/ ;

    var Pos = Args.SymbolServer.search ( Re ) ;
    if ( Pos != -1 )
    {
        // The pattern matches, so now look for the -e and -v.
        var CurrArgIndex = 1 ;
        for ( CurrArgIndex = 1                        ;
              CurrArgIndex < WScript.Arguments.length ;
              CurrArgIndex++                           )
        {
            var Arg = WScript.Arguments(CurrArgIndex).toUpperCase ( ) ;
            switch ( Arg.substring ( 0 ,1 ) )
            {
                case '-'    :
                case '?'    :
                    switch ( Arg.substring ( 1 , 2 ) )
                    {
                        case 'E'    :
                            Args.DoEXEs = true ;
                            break ;
                        case 'V'    :
                            Args.DoVerbose = true ;
                            break ;
                        case 'D'    :
                            Args.DebugScript = true ;
                            break ;
                        case '?'    :
                            Args.JustShowHelp = true ;
                            RetVal = false ;
                            break ;
                        case 'B'    :
                            Args.SkipBinaries = true ;
                            break ;
                        case 'S'    :
                            Args.SkipSymbols = true ;
                            break ;
                        default     :
                            RetVal = false ;
                            return ( RetVal ) ;
                            break ;
                    }
                    break ;
                default     :
                    RetVal = false ;
                    return ( RetVal ) ;
                    break ;
            }

        }
    }
    else
    {
        RetVal = false ;
    }
    if ( true == Args.DebugScript )
    {
        WScript.Echo ( "Command line args :" ) ;
        WScript.Echo ( "  Symbol server   : " + Args.SymbolServer ) ;
        WScript.Echo ( "  Do EXEs         : " + Args.DoEXEs ) ;
        WScript.Echo ( "  Do Verbose      : " + Args.DoVerbose ) ;
        WScript.Echo ( "  Just Show Help  : " + Args.JustShowHelp ) ;
        WScript.Echo ( "  Skip Binaries   : " + Args.SkipBinaries ) ;
        WScript.Echo ( "  Skip Symbols    : " + Args.SkipSymbols ) ;
        WScript.Echo ( "\n" ) ;
    }
    return ( RetVal ) ;
}

// Displays the help for this program.
function ShowHelp ( )
{
var HelpString =
"OSsyms - Version 1.0 - Copyright 2002-2003 by John Robbins\n" ;
HelpString +=
"   Debugging Applications for Microsoft .NET and Microsoft Windows\n\n" ;
HelpString +=
"   Fills your symbol server with the OS binaries and symbols.\n" ;
HelpString +=
"   Run this on each time you apply a service pack/hotfix to get the";
HelpString +=
" perfect\n   symbols while debugging and for mini dumps.\n"
HelpString +=
"   SYMSTORE.EXE and SYMCHK.EXE must be in the path.\n\n" ;
HelpString +=
" Usage : OSsyms <symbol server> [-e|-v|-b|-s|-d]\n\n" ;
HelpString +=
"   <symbol server> - The symbol server in \\\\server\\share format.\n";
HelpString +=
"   -e              - Do EXEs as well as DLLs.\n" ;
HelpString +=
"   -v              - Do verbose output.\n" ;
HelpString +=
"   -d              - Debug the script. (Shows what would execute.)\n" ;
HelpString +=
"   -b              - Don't add the binaries to the symbol store.\n" ;
HelpString +=
"   -s              - Don't add the symbols to the symbol store.\n" ;
HelpString +=
"                     (Not recommended)\n" ;
    WScript.Echo ( HelpString ) ;
}
