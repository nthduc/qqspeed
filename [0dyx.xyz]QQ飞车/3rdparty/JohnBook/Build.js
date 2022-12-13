/*//////////////////////////////////////////////////////////////////////
The build script for all the utilities and unit tests from
Debugging Applications for Microsoft .NET and Microsoft Windows
Copyright © 1997-2003 by John Robbins

You have to have DEVENV.EXE and the framework tools in your path!  Run
<VS.NET Install Dir>\Common7\Tools\VSVARS32.BAT to get the proper
environment variables set up.

Valid args: CMD [Utilities|Tests|Both]
- CMD is required
- If one of [Utilities|Tests|Both] is not specified Utilities is
  assumed.

CMD =
    /build          - Builds the projects.
    /rebuild        - Rebuilds all the projects.
    /clean          - Cleans based on the projects.
    /reallyclean    - Scrubs everything that's built.
    /?              - Does a little help action.

Utilities = Builds just the utilities.
Tests = Builds just the unit tests
Both = Builds both utilities and unit tests.
//////////////////////////////////////////////////////////////////////*/

/*//////////////////////////////////////////////////////////////////////
// The following globals are constants
//////////////////////////////////////////////////////////////////////*/
var k_OUTPUTFILENAME = "Build.OUT" ;
var WSHSHELL = WScript.CreateObject ( "WScript.Shell" ) ;
var FSO = WScript.CreateObject ( "Scripting.FileSystemObject" ) ;

// The command I have to run to properly really clean everything.
var k_REALLYCLEANCMD = "xReallyClean.CMD" ;

/*//////////////////////////////////////////////////////////////////////
// The following globals are changed based on the build type.
//////////////////////////////////////////////////////////////////////*/
// The development environment command to execute.
var BuildOperationType = "/Build" ;
// The targets to do. 1=Utilities Only, 2=Tests Only, 3=Both
var TargetsToDo = 0x1 ;
// Flag to indicate reallyclean.
var IsReallyClean = false ;
// The prompt to show the user.
var PromptText = "**UNINITIALIZED**" ;

/*//////////////////////////////////////////////////////////////////////
// Set up all the arrays of data I'll need.
//////////////////////////////////////////////////////////////////////*/

/*//////////////////////////////////////////////////////////////////////
// All the Special Utility build targets.
// Order is "ProjectLocation"   ,
//          "ProjectName"       ,
//          "ConfigToBuild"     ,
//          "SpecialPrompt"
//////////////////////////////////////////////////////////////////////*/
var SpecialUtilityCmdTgts = new Array
(
    new CommandTarget ( "JFX"           ,
                        "JFX"           ,
                        "Release BSU"   ,
                        null             ) ,
    new CommandTarget ( "JFX"           ,
                        "JFX"           ,
                        "Debug BSU"     ,
                        null             ) ,
    new CommandTarget ( "SymbolEngine"  ,
                        "SymbolEngine"  ,
                        "Release BSU"   ,
                        null             ) ,
    new CommandTarget ( "SymbolEngine"  ,
                        "SymbolEngine"  ,
                        "Debug BSU"     ,
                        null             )
) ;

/*//////////////////////////////////////////////////////////////////////
// The common utility projects.  All projects in this list assume the
// following.
// 1.  The directory appears one level below the directory this script
//     resides.
// 2.  There's a VS.NET solution in same name as the directory that will
//     build everything for that project.
// 3.  There's a Debug and a Release configuration in that project.
//////////////////////////////////////////////////////////////////////*/
var StandardUtilityTgts = new Array
(
    "JFX"                   ,
    "SymbolEngine"          ,
    "BugslayerUtil"         ,
    "BugslayerUtil.NET"     ,
    "CrashFinder"           ,
    "PDB2MAP"               ,
//    "TraceSrv"              ,
    "DeadlockDetection"     ,
    "SWS"                   ,
    "ProfilerLib"           ,
    "ExceptionMon"          ,
    "DbgChooser"            ,
    "MinDBG"                ,
    "WDbg"                  ,
    "BSU_ExpEval_AddIn"     ,
    "SimpleToolWindow"      ,
    "SuperSaver"            ,
    "SettingsMaster"        ,
    "FlowTrace"             ,
    "XPExceptMon"           ,
    "Tester"                ,
    "FastTrace"             ,
    "MemStress"
) ;

/*//////////////////////////////////////////////////////////////////////
// The test harnesses for all the utilities
// Order is "ProjectLocation"   ,
//          "ProjectName"       ,
//          "ConfigToBuild"     ,
//          "SpecialPrompt"
//////////////////////////////////////////////////////////////////////*/
var TestCmdTgts = new Array
(
    new CommandTarget ( "BugslayerUtil\\Tests"          ,
                        "Tests"                         ,
                        "Release"                       ,
                        "BugslayerUtil.DLL Tests Release" ) ,
    new CommandTarget ( "BugslayerUtil\\Tests"          ,
                        "Tests"                         ,
                        "Debug"                         ,
                        "BugslayerUtil.DLL Tests Debug"  ) ,
    new CommandTarget ( "SymbolEngine\\Tests\\SymClassTests" ,
                        "SymClassTests"                      ,
                        "Release"                            ,
                        "SymbolEngine SymClassTests Release"  ) ,
    new CommandTarget ( "SymbolEngine\\Tests\\SymClassTests" ,
                        "SymClassTests"                      ,
                        "Debug"                              ,
                        "SymbolEngine SymClassTests Debug"    ) ,
    new CommandTarget ( "SymbolEngine\\Tests\\SymLookup" ,
                        "SymLookup"                      ,
                        "Release"                        ,
                        "SymbolEngine SymLookup Release"  ) ,
    new CommandTarget ( "SymbolEngine\\Tests\\SymLookup" ,
                        "SymLookup"                      ,
                        "Debug"                          ,
                        "SymbolEngine SymLookup Debug"    ) ,
    new CommandTarget ( "BugslayerUtil.NET\\Tests\\BSUNAssertTest" ,
                        "BSUNAssertTest"                           ,
                        "Debug"                                    ,
                        "BugslayerUtil.NET BSUNAssertTest Debug"    ) ,
    new CommandTarget ( "BugslayerUtil.NET\\Tests\\BSUNBTWTLTest" ,
                        "BSUNBTWTLTest"                           ,
                        "Debug"                                   ,
                        "BugslayerUtil.NET BSUNBTWTLTest Debug"    ) ,
    new CommandTarget ( "BugslayerUtil.NET\\Tests\\BSUNELTLTest" ,
                        "BSUNELTLTest"                           ,
                        "Debug"                                  ,
                        "BugslayerUtil.NET BSUNELTLTest Debug"    ) ,
    new CommandTarget ( "BugslayerUtil.NET\\Tests\\BSUNStackTraceTest",
                        "BSUNStackTraceTest"                          ,
                        "Release"                                     ,
                        "BugslayerUtil.NET BSUNStackTraceTest Release"),
    new CommandTarget ( "BugslayerUtil.NET\\Tests\\BSUNStackTraceTest",
                        "BSUNStackTraceTest"                          ,
                        "Debug"                                       ,
                        "BugslayerUtil.NET BSUNStackTraceTest Release"),
//    new CommandTarget ( "TraceSrv\\DCOMTest"            ,
//                        "DCOMTest"                      ,
//                        "Debug"                         ,
//                        "TraceSrv DCOMTest Test Debug"   ) ,
    new CommandTarget ( "DeadlockDetection"             ,
                        "DeadlockDetectionTests"        ,
                        "Release"                       ,
                        "DeadlockDetection Tests Release" ) ,
    new CommandTarget ( "DeadlockDetection"             ,
                        "DeadlockDetectionTests"        ,
                        "Debug"                         ,
                        "DeadlockDetection Tests Debug"  ) ,
    new CommandTarget ( "SWS\\Tests\\SimpleSWSTest"     ,
                        "SimpleSWSTest"                 ,
                        "Debug"                         ,
                        "SWS SimpleSWSTest Test Debug"   ) ,
    new CommandTarget ( "SWS\\Tests\\MultiDLLs"         ,
                        "MultiDlls"                     ,
                        "Debug"                         ,
                        "SWS MultiDlls Test Debug"       ) ,
    new CommandTarget ( "SWS\\Tests\\MultiThreads"      ,
                        "MultiThreads"                  ,
                        "Debug"                         ,
                        "SWS Multithreads Test Debug"           ) ,
    new CommandTarget ( "ProfilerLib\\Tests\\DoNothing" ,
                        "DoNothing"                     ,
                        "Debug"                         ,
                        "ProfilerLib DoNothing Test Debug" ) ,
    new CommandTarget ( "ProfilerLib\\Tests\\DoNothing" ,
                        "DoNothing"                     ,
                        "Release"                       ,
                        "ProfilerLib DoNothing Test Release" ) ,
    new CommandTarget ( "ExceptionMon\\Tests\\SimpleExp"    ,
                        "SimpleExp"                         ,
                        "Release"                           ,
                        "ExceptionMonitor SimpleExp Release"  ) ,
    new CommandTarget ( "ExceptionMon\\Tests\\SimpleExp"    ,
                        "SimpleExp"                         ,
                        "Debug"                             ,
                        "ExceptionMonitor SimpleExp Debug"   ) ,
    new CommandTarget ( "ExceptionMon\\Tests\\ExceptMonTest"    ,
                        "ExceptMonTest"                         ,
                        "Release"                               ,
                        "ExceptionMonitor ExceptMonTest Release"  ) ,
    new CommandTarget ( "ExceptionMon\\Tests\\ExceptMonTest"    ,
                        "ExceptMonTest"                         ,
                        "Debug"                                 ,
                        "ExceptionMonitor ExceptMonTest Debug"   ) ,
    new CommandTarget ( "BSU_ExpEval_AddIn\\Tests\\EEAddInTest" ,
                        "EEAddInTest"                           ,
                        "Debug"                                 ,
                        "Expression Evaluator AddIn Test Debug"  ) ,
    new CommandTarget ( "XPExceptMon\\Tests\\XPExceptMonTests"  ,
                        "XPExceptMonTests"                      ,
                        "Debug"                                 ,
                        "XP Vector EH Test Debug"                ) ,
    new CommandTarget ( "XPExceptMon\\Tests\\XPExceptMonTests"  ,
                        "XPExceptMonTests"                      ,
                        "Release"                               ,
                        "XP Vector EH Test Release"              ) ,
    new CommandTarget ( "Tester\\Tester\\Tests"                 ,
                        "Tests"                                 ,
                        "Debug"                                 ,
                        "Tester DLL Tests  Debug"                ) ,
    new CommandTarget ( "Tester\\Tester\\Tests"                 ,
                        "Tests"                                 ,
                        "Release"                               ,
                        "Tester DLL Tests  Release"              ) ,
    new CommandTarget ( "Tester\\TNotifyHlp\\Tests\\TNHTest"    ,
                        "TNHTest"                               ,
                        "Debug"                                 ,
                        "TNotify DLL Tests  Debug"               ) ,
    new CommandTarget ( "Tester\\TNotifyHlp\\Tests\\TNHTest"    ,
                        "TNHTest"                               ,
                        "Release"                               ,
                        "TNotify DLL Tests  Debug"               ) ,
    new CommandTarget ( "FastTrace\\FastTrace\\Tests\\FTSimpTest" ,
                        "FTSimpTest"                              ,
                        "Debug"                                   ,
                        "FastTrace Tests Debug"                    ) ,
    new CommandTarget ( "FastTrace\\FastTrace\\Tests\\FTSimpTest" ,
                        "FTSimpTest"                              ,
                        "Release"                                 ,
                        "FastTrace Tests Release"                  ) ,
    new CommandTarget ( "MemStress\\Tests\\MemStressDemo"       ,
                        "MemStressDemo"                         ,
                        "Debug"                                 ,
                        "MemStressDemo  Debug"                   ) ,
    new CommandTarget ( "AppVerifierDemo"           ,
                        "AppVerifierDemo"           ,
                        "Release"                   ,
                        "AppVerifierDemo Release"    )
) ;

/*//////////////////////////////////////////////////////////////////////
// Execution starts here.
//////////////////////////////////////////////////////////////////////*/

// Check to see if the right stuff is in the path.
CheckEnvironmentAndParseParams ( ) ;

if ( 0x01 == ( TargetsToDo & 0x01 ) )
{
    WScript.Echo ( "***************************************" ) ;
    WScript.Echo ( PromptText + " All Utilities" ) ;
    WScript.Echo ( "***************************************" ) ;
    WScript.Echo ( "********Special Utility Projects*******" ) ;

    for ( i = 0 ; i < SpecialUtilityCmdTgts.length ; i++ )
    {
        ExecuteOperation2 ( BuildOperationType                         ,
                            PromptText                                 ,
                            SpecialUtilityCmdTgts[ i ].ProjectLocation ,
                            SpecialUtilityCmdTgts[ i ].ProjectName     ,
                            SpecialUtilityCmdTgts[ i ].ConfigToBuild   ,
                            SpecialUtilityCmdTgts[ i ].SpecialPrompt  );
    }

    WScript.Echo ( "************Utility Projects***********" ) ;
    for ( j = 0 ; j < StandardUtilityTgts.length ; j++ )
    {
        ExecuteOperation2 ( BuildOperationType       ,
                            PromptText               ,
                            StandardUtilityTgts[ j ] ,
                            StandardUtilityTgts[ j ] ,
                            "Release"                ,
                            null                      ) ;

        ExecuteOperation2 ( BuildOperationType       ,
                            PromptText               ,
                            StandardUtilityTgts[ j ] ,
                            StandardUtilityTgts[ j ] ,
                            "Debug"                  ,
                            null                      ) ;

    }
}

if ( 0x02 == ( TargetsToDo & 0x02 ) )
{
    WScript.Echo ( "***************************************" ) ;
    WScript.Echo ( PromptText + " All Unit Tests" ) ;
    WScript.Echo ( "***************************************" ) ;
    for (i = 0; i < TestCmdTgts.length; i++)
    {
        ExecuteOperation2 ( BuildOperationType               ,
                            PromptText                       ,
                            TestCmdTgts[ i ].ProjectLocation ,
                            TestCmdTgts[ i ].ProjectName     ,
                            TestCmdTgts[ i ].ConfigToBuild   ,
                            TestCmdTgts[ i ].SpecialPrompt    ) ;
    }
}

// Delete any build output laying around.
if ( FSO.FileExists ( k_OUTPUTFILENAME ) )
{
    FSO.DeleteFile ( k_OUTPUTFILENAME ) ;
}

// For reallyclean, I need to do some extra work.
if ( true == IsReallyClean )
{
     WSHSHELL.Run ( k_REALLYCLEANCMD , 0 , true ) ;
}

WScript.Echo ( "***************************************" ) ;
WScript.Echo ( PromptText + " completed successfully!" ) ;
WScript.Echo ( "***************************************" ) ;
WScript.Quit ( 0 ) ;

/*//////////////////////////////////////////////////////////////////////
// Functions Start Here
//////////////////////////////////////////////////////////////////////*/

function CheckEnvironmentAndParseParams ( )
{
    var VCInstDir = WSHSHELL.ExpandEnvironmentStrings("%VCINSTALLDIR%");
    if ( "%VCINSTALLDIR%" == VCInstDir )
    {
        WScript.Echo("#!#!#!#!#!#!#!#!#!#!#!#!#!#!#!#!#!#!#!#!#!#!#!#!");
        WScript.Echo("VS.NET environment variables not set." ) ;
        WScript.Echo("Run VCVARS32.BAT to initialize this command prompt.");
        WScript.Echo("#!#!#!#!#!#!#!#!#!#!#!#!#!#!#!#!#!#!#!#!#!#!#!#!\n" );

        ShowUsage ( ) ;
        WScript.Quit ( 1 ) ;
    }

    // Get all the arguments to the script.
    if ( false == ProcessesArguments ( ) )
    {
        ShowUsage ( ) ;
        WScript.Quit ( 1 ) ;
    }
}

// Types a text file to the screen because CSCRIPT.EXE is too stupid
// to call TYPE.  Sheez.
function TypeFile ( TxtFile )
{
    if ( false == IsCSCRIPT ( ) )
    {
        // Running under WSCRIPT so don't want to do each and every line
        // through a message box.
        WSHSHELL.Run ( "notepad.exe " + TxtFile , 1 , false ) ;
    }
    else
    {
        var TxtFileObj = FSO.OpenTextFile ( TxtFile , 1 , false ) ;
        while ( ! TxtFileObj.AtEndOfStream )
        {
            var StrLine = TxtFileObj.ReadLine ( ) ;
            WScript.Echo ( StrLine ) ;
        }
    }
}

// Returns TRUE if CSCRIPT.EXE is executing the script.
function IsCSCRIPT ( )
{
    var RegPtrn = /cscript\.exe/i ;
    var UpStr = WScript.FullName.toUpperCase ( ) ;
    var Res = WScript.FullName.match ( RegPtrn ) ;
    if ( Res != null )
    {
        return ( true ) ;
    }
    else
    {
        return ( FALSE ) ;
    }
}

// Takes care of doing an individual build.  If the build fails, the
// function will exit the script.
function ExecuteOperation2 ( BuildOp         ,
                             PromptTxt       ,
                             ProjectLocation ,
                             ProjectName     ,
                             ConfigToBuild   ,
                             SpecialPrompt    )
{
    var ProjLoc = ProjectLocation + "\\" + ProjectName ;

    // Delete any build output laying around.
    if ( FSO.FileExists ( k_OUTPUTFILENAME ) )
    {
        FSO.DeleteFile ( k_OUTPUTFILENAME ) ;
    }

    // Echo what we're about to do.
    if ( null == SpecialPrompt )
    {
        WScript.Echo ( PromptTxt + " " +
                       ProjectName + " " +
                       ConfigToBuild ) ;
    }
    else
    {
        WScript.Echo ( PromptTxt + " " + SpecialPrompt ) ;
    }

    var CmdStr = "devenv.exe /safemode " + BuildOp + " \"" +
                 ConfigToBuild + "\"" +
                 " /out " +
                 k_OUTPUTFILENAME + " " +
                 ProjectLocation + "\\" +
                 ProjectName + ".SLN" ;

    //WScript.Echo ( CmdStr ) ;

    var iRunRet = WSHSHELL.Run ( CmdStr , 8 , true ) ;

    if ( 1 == iRunRet )
    {
        WScript.Echo ( "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!" ) ;
        WScript.Echo ( "!!!!!!!!!!!! BUILD FAILED !!!!!!!!!!!!!" ) ;
        WScript.Echo ( "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!" ) ;
        TypeFile ( k_OUTPUTFILENAME ) ;
        WScript.Quit ( 1 ) ;
    }

    return ( iRunRet ) ;
}

// Shows acceptable command line options.
function ShowUsage ( )
{
WScript.Echo ( "Build.js - Builds the utilities" ) ;
WScript.Echo ( " BUILD.JS CMD [Utilities|Tests|Both]" ) ;
WScript.Echo ( " CMD = " ) ;
WScript.Echo ( "    /?            - This help" ) ;
WScript.Echo ( "    /build        - Builds all the code" ) ;
WScript.Echo ( "    /rebuild      - Rebuilds the code" ) ;
WScript.Echo ( "    /clean        - Project clean" ) ;
WScript.Echo ( "    /reallyclean  - Scrubs all sparkly clean" ) ;
WScript.Echo ( " [Utilities|Tests|Both]" ) ;
WScript.Echo ( "    Utilities     - Just the utilities" ) ;
WScript.Echo ( "                    (default if none specified" ) ;
WScript.Echo ( "    Tests         - Just the unit tests" ) ;
WScript.Echo ( "    Both          - The whole shebang!" ) ;
WScript.Echo ( " " ) ;
WScript.Echo("DEVENV.EXE and .NET Framework tool must be in the path.");
WScript.Echo ( "  Run VSVARS32.BAT to get tools set up." ) ;
}

// The constructor for my command target object.
function CommandTarget ( ProjectLocation ,
                         ProjectName     ,
                         ConfigToBuild   ,
                         SpecialPrompt    )
{
    this.ProjectLocation = ProjectLocation ;
    this.ProjectName     = ProjectName     ;
    this.ConfigToBuild   = ConfigToBuild   ;
    this.SpecialPrompt   = SpecialPrompt   ;
}

// Takes care of processing the command line args.
function ProcessesArguments ( )
{
    // Do the easy cases.
    if ( ( 0 == WScript.Arguments.length   ) ||
         ( "/?" == WScript.Arguments ( 0 ) ) ||
         ( WScript.Arguments.length > 2    )   )
    {
        return ( false ) ;
    }

    // Upper case the argument to make comparison easier.
    var CurrArg = WScript.Arguments(0).toUpperCase ( ) ;
    if ( "/BUILD" == CurrArg )
    {
        BuildOperationType = "/Build" ;
        PromptText = "Building" ;
    }
    else if ( "/REBUILD" == CurrArg )
    {
        BuildOperationType = "/Rebuild" ;
        PromptText = "Rebuilding" ;
    }
    else if ( "/CLEAN" == CurrArg )
    {
        BuildOperationType = "/Clean" ;
        PromptText = "Cleaning" ;
    }
    else if ( "/REALLYCLEAN" == CurrArg )
    {
        BuildOperationType = "/Clean"
        PromptText = "Really cleaning"
        IsReallyClean = true ;
    }
    else
    {
        return ( false ) ;
    }

    if ( 1 == WScript.arguments.length )
    {
        TargetsToDo = 0x01 ;
    }
    else
    {
        CurrArg = WScript.Arguments(1).toUpperCase ( ) ;

        if ( "UTILITIES" == CurrArg )
        {
            TargetsToDo = 0x01 ;
        }
        else if ( "TESTS" == CurrArg )
        {
            TargetsToDo = 0x02 ;
        }
        else if ( "BOTH" == CurrArg )
        {
            TargetsToDo = 0x03 ;
        }
        else
        {
            return ( FALSE ) ;
        }
    }

    return ( true ) ;
}

