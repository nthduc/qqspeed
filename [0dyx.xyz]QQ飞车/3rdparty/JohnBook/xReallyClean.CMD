@ECHO OFF

REM A helper batch file for Build.JS

PUSHD Output
IF EXIST *.ILK DEL /Q *.ILK > nul
IF EXIST *.PDB DEL /Q *.PDB > nul
IF EXIST *.DLL DEL /Q *.DLL > nul
IF EXIST *.EXE DEL /Q *.EXE > nul
IF EXIST *.DD  DEL /Q *.DD > nul
IF EXIST *.SWS DEL /Q *.SWS > nul
IF EXIST *.TLB DEL /Q *.TLB > nul
IF EXIST *.LIB DEL /Q *.LIB > nul
IF EXIST *.EXP DEL /Q *.EXP > nul
IF EXIST *.FLO DEL /Q *.FLO > nul
IF EXIST *.LOG DEL /Q *.LOG > nul
IF EXIST *.P2M DEL /Q *.P2M > nul
IF EXIST *.TRG DEL /Q *.TRG > nul
IF EXIST X     DEL /Q X     > nul
IF EXIST Y     DEL /Q Y     > nul
POPD

PUSHD Output\1033
IF EXIST *.ILK DEL /Q *.ILK > nul
POPD

PUSHD Release
IF EXIST *.ILK DEL /Q *.ILK > nul
IF EXIST *.PDB DEL /Q *.PDB > nul
IF EXIST *.DLL DEL /Q *.DLL > nul
IF EXIST *.EXE DEL /Q *.EXE > nul
IF EXIST *.DD  DEL /Q *.DD > nul
IF EXIST *.SWS DEL /Q *.SWS > nul
IF EXIST *.TLB DEL /Q *.TLB > nul
IF EXIST *.LIB DEL /Q *.LIB > nul
IF EXIST *.EXP DEL /Q *.EXP > nul
IF EXIST *.FLO DEL /Q *.FLO > nul
IF EXIST *.LOG DEL /Q *.LOG > nul
IF EXIST *.P2M DEL /Q *.P2M > nul
IF EXIST *.MAP DEL /Q *.MAP > nul
IF EXIST *.TRG DEL /Q *.TRG > nul
IF EXIST X     DEL /Q X     > nul
IF EXIST Y     DEL /Q Y     > nul
POPD

PUSHD Release\1033
IF EXIST *.ILK DEL /Q *.ILK > nul
IF EXIST *.PDB DEL /Q *.PDB > nul
POPD

PUSHD WDBG
DEL /Q /S *.PDB *.IDB
POPD

DEL /Q /S BUILDLOG.HTM *.APS *.NCB *.PLG *.BAK > nul

REM - Man, those .NET projects leave turds everywhere!
PUSHD BugslayerUtil.NET
RD /s /q OBJ
POPD
PUSHD BugslayerUtil.NET\Tests\BSUNStackTraceTest
RD /s /q OBJ
POPD
PUSHD BugslayerUtil.NET\Tests\BSUNELTLTest
RD /s /q OBJ
POPD
PUSHD BugslayerUtil.NET\Tests\BSUNBTWTLTest
RD /s /q OBJ
POPD
PUSHD BugslayerUtil.NET\Tests\BSUNAssertTest
RD /s /q OBJ
RD /s /q BIN
POPD
PUSHD ExceptionMon\Tests\ExceptMonTest
RD /s /q OBJ
POPD
PUSHD ExceptionMon\Tests\simpleexp
RD /s /q OBJ
POPD
PUSHD SuperSaver\SuperSaver
RD /s /q OBJ
POPD
PUSHD SettingsMaster\SettingsMaster
RD /s /q OBJ
POPD
PUSHD SimpleToolWindow\SimpleToolWindow
RD /s /q OBJ
POPD
PUSHD FlowTrace\Tests\FlowTest
RD /s /q OBJ
POPD
PUSHD MemStress
RD /s /q OBJ
POPD
PUSHD Tester\Tester\Tests\TesterTester
RD /s /q OBJ
POPD


