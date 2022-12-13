@ECHO OFF
SETLOCAL
REM A simple wrapper for the Build.JS script.

REM Clear out any command args that might be left around.
SET COMMANDARG=

REM Get the DEVENV command line set up.
IF "%1"=="" SET COMMANDARG=/build
IF /i "%1"=="build" SET COMMANDARG=/build
IF /i "%1"=="/build" SET COMMANDARG=/build
IF /i "%1"=="rebuild" SET COMMANDARG=/rebuild
IF /i "%1"=="/rebuild" SET COMMANDARG=/rebuild
IF /i "%1"=="clean" SET COMMANDARG=/clean
IF /i "%1"=="/clean" SET COMMANDARG=/clean
IF /i "%1"=="reallyclean" SET COMMANDARG=/reallyclean
IF /i "%1"=="/reallyclean" SET COMMANDARG=/reallyclean
IF /i "%1"=="/?" GOTO InvalidArg
IF /i "%1"=="?" GOTO InvalidArg
IF "%COMMANDARG%"=="" GOTO InvalidArg

CSCRIPT //NOLOGO BUILD.JS %COMMANDARG% Utilities
GOTO End


:InvalidArg
@ECHO Allowed args: [/]build [/]rebuild [/]clean [/]reallyclean

:End
ENDLOCAL
