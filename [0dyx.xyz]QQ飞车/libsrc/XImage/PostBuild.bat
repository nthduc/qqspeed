@echo off
@echo copying library files and header files ..................................
if exist debug\xImageD.lib  xcopy debug\xImageD.lib	..\..\Lib /y/r  > nul
if exist release\xImage.lib xcopy release\xImage.lib ..\..\Lib /y/r  > nul
if exist xImage.h xcopy xImage.h ..\..\Include /y/r  > nul
if exist xGif.h   xcopy xGif.h	..\..\Include /y/r  > nul
@echo copy files successfully.
