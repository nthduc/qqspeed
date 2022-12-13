@echo off
@echo copying library files and header files ..................................
if exist debug\KernelD.lib  xcopy debug\KernelD.lib ..\..\Lib /y/r  > nul
if exist release\Kernel.lib xcopy release\Kernel.lib ..\..\Lib /y/r  > nul
@echo copy files successfully.
