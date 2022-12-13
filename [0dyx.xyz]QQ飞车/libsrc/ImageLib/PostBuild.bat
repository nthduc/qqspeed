@echo off
@echo copying library files and header files ..................................
if exist debug\ImageLibD.lib  xcopy debug\ImageLibD.lib	..\..\Lib /y/r  > nul
if exist release\ImageLib.lib xcopy release\ImageLib.lib ..\..\Lib /y/r  > nul
if exist ImageLib.h			  xcopy ImageLib.h           ..\..\Include /y/r  > nul 
if exist IDImage.h			  xcopy IDImage.h            ..\..\Include /y/r  > nul 
@echo copy files successfully.
