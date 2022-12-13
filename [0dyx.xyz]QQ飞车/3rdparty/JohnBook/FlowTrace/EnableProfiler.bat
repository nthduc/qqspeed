@REM -------------------------------------------------------------------
@REM Debugging Applications for Microsoft .NET and Windows Applications
@REM Copyright © 2002-2003 John Robbins
@REM -------------------------------------------------------------------

@set Cor_Enable_Profiling=0x1
@regsvr32 ..\Output\FlowTrace.dll
@@set COR_PROFILER={D3799FA3-13EE-4BAB-A263-843C14397DE4}

@@title FlowTrace Code Profiler Enabled
