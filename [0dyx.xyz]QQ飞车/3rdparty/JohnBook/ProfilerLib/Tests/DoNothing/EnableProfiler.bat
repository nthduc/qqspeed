@REM////////////////////////////////////////////////////////////////////
@REM Debugging Applications for Microsoft .NET and Microsoft Windows
@REM Copyright (c) 1997-2003 John Robbins -- All rights reserved.
@REM////////////////////////////////////////////////////////////////////

@set Cor_Enable_Profiling=0x1
@regsvr32 /s ..\..\..\Output\DoNothing.dll
@@set COR_PROFILER={D27965C4-B361-4711-A7BE-465F71A97D87}

@echo on
@set Cor_Enable_Profiling
@@set COR_PROFILER
@@title DoNothing Code Profiler Enabled
