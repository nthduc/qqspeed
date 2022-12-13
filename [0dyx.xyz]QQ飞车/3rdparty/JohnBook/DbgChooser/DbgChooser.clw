; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
LastClass=CDbgChooserDlg
LastTemplate=CDialog
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "DbgChooser.h"

ClassCount=4
Class1=CDbgChooserApp
Class2=CDbgChooserDlg
Class3=CAboutDlg

ResourceCount=4
Resource1=IDD_ABOUTBOX
Resource2=IDR_MAINFRAME
Resource3=IDD_DBGCHOOSER_DIALOG
Class4=CCfgDbgChooser
Resource4=IDD_CONFIGUREDBGCHOOSER

[CLS:CDbgChooserApp]
Type=0
HeaderFile=DbgChooser.h
ImplementationFile=DbgChooser.cpp
Filter=N
LastObject=CDbgChooserApp

[CLS:CDbgChooserDlg]
Type=0
HeaderFile=DbgChooserDlg.h
ImplementationFile=DbgChooserDlg.cpp
Filter=D
LastObject=CDbgChooserDlg
BaseClass=CDialog
VirtualFilter=dWC

[CLS:CAboutDlg]
Type=0
HeaderFile=about.h
ImplementationFile=about.cpp
BaseClass=CDialog
LastObject=CAboutDlg

[DLG:IDD_ABOUTBOX]
Type=1
Class=CAboutDlg
ControlCount=9
Control1=IDC_STATIC,static,1342177283
Control2=IDC_STATIC,static,1342308480
Control3=IDC_STATIC,static,1342308352
Control4=IDOK,button,1342373889
Control5=IDC_STATIC,static,1342308480
Control6=IDC_STATIC,static,1342308352
Control7=IDC_STATIC,static,1342308352
Control8=IDC_STATIC,static,1342308352
Control9=IDC_STATIC,static,1342308352

[DLG:IDD_DBGCHOOSER_DIALOG]
Type=1
Class=CDbgChooserDlg
ControlCount=5
Control1=IDC_VCSEVENDEBUGGER,button,1342242816
Control2=IDC_VCDEBUGGER,button,1342242816
Control3=IDC_WINDBGDEBUGGER,button,1342242816
Control4=IDC_DRWATSONDEBUGGER,button,1342242816
Control5=IDC_DONOTDEBUG,button,1342242816

[DLG:IDD_CONFIGUREDBGCHOOSER]
Type=1
Class=CCfgDbgChooser
ControlCount=12
Control1=IDC_STATIC,static,1342308352
Control2=IDC_VCSEVENDBGCONFIG,edit,1350631552
Control3=IDC_STATIC,static,1342308352
Control4=IDC_VCDBGCONFIG,edit,1350631552
Control5=IDC_STATIC,static,1342308352
Control6=IDC_WINDBGCONFIG,edit,1350631552
Control7=IDC_STATIC,static,1342308352
Control8=IDC_DRWATSONDBGCONFIG,edit,1350631552
Control9=IDOK,button,1342242817
Control10=IDCANCEL,button,1342242816
Control11=IDHELP,button,1342242816
Control12=IDC_STATIC,static,1342312448

[CLS:CCfgDbgChooser]
Type=0
HeaderFile=CfgDbgChooser.h
ImplementationFile=CfgDbgChooser.cpp
BaseClass=CDialog
Filter=D
VirtualFilter=dWC
LastObject=CCfgDbgChooser

