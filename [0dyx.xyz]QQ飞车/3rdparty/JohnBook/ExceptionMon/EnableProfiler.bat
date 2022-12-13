@REM////////////////////////////////////////////////////////////////////
@REM Debugging Applications for Microsoft .NET and Microsoft Windows
@REM Copyright (c) 1997-2003 John Robbins -- All rights reserved.
@REM////////////////////////////////////////////////////////////////////

@set Cor_Enable_Profiling=0x1
@regsvr32 ..\Output\ExceptionMon.dll
@@set COR_PROFILER={F6F3B5B7-4EEC-48f6-82F3-A9CA97311A1D}

@@title ExceptionMon Code Profiler Enabled
