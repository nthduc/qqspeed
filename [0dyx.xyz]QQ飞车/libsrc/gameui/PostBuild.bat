@echo off
@echo copying library files and header files ..................................

if exist debug\GameUID.lib  xcopy debug\GameUID.lib ..\..\Lib /y/r  > nul
if exist release\GameUI.lib xcopy release\GameUI.lib ..\..\Lib /y/r  > nul

@echo copy files successfully.
