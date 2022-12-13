@REM////////////////////////////////////////////////////////////////////
@REM Debugging Applications for Microsoft .NET and Microsoft Windows
@REM Copyright (c) 1997-2003 John Robbins -- All rights reserved.
@REM////////////////////////////////////////////////////////////////////

@set Cor_Enable_Profiling=0x1
@regsvr32 <<PUT PROFILER DLL HERE!>>
@@set COR_PROFILER=<<PUT PROFILER GUID HERE!>>

@@title DoNothing Code Profiler Enabled
