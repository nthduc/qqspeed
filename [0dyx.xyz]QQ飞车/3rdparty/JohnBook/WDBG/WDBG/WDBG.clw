; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
LastClass=CBreakpointsDlg
LastTemplate=CDialog
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "WDBG.h"
LastPage=0

ClassCount=17
Class1=CWDBGApp
Class2=CWDBGProjDoc
Class3=CWDBGLogView
Class4=CMainFrame

ResourceCount=10
Resource1=IDD_MEMTODISPLAY
Resource2=IDD_BREAKPOINTS
Class5=CChildFrame
Class6=CAboutDlg
Class7=CDBGChildFrame
Class8=CWDBGChildFrame
Resource3=IDR_MAINFRAME
Class9=CProjectSettings
Class10=CDocNotifyWnd
Resource4=IDD_THREADPICKER
Class11=CMemoryDisplayDlg
Resource5=IDR_WDBGTYPE
Class12=CDisasmDisplayDlg
Resource6=IDD_DISASMTODISPLAY
Class13=CSymbolsDisplayDlg
Resource7=IDD_SYMTODISPLAY
Class14=CCallStackDisplayDlg
Class15=CThreadPickerDlg
Resource8=IDD_ABOUTBOX
Class16=CBreakpointsDlg
Resource9=IDD_PROJECTSETTINGS
Class17=CNewEditBPDlg
Resource10=IDD_NEWEDITBP

[CLS:CWDBGApp]
Type=0
HeaderFile=WDBG.h
ImplementationFile=WDBG.cpp
Filter=N
LastObject=CWDBGApp
BaseClass=CWinApp
VirtualFilter=AC

[CLS:CWDBGProjDoc]
Type=0
HeaderFile=WDBGProjDoc.h
ImplementationFile=WDBGProjDoc.cpp
Filter=N
BaseClass=CDocument
VirtualFilter=DC
LastObject=CWDBGProjDoc

[CLS:CWDBGLogView]
Type=0
HeaderFile=WDBGLogView.h
ImplementationFile=WDBGLogView.cpp
Filter=C
LastObject=CWDBGLogView
BaseClass=CEditView
VirtualFilter=VWC


[CLS:CMainFrame]
Type=0
HeaderFile=MainFrm.h
ImplementationFile=MainFrm.cpp
Filter=T


[CLS:CChildFrame]
Type=0
HeaderFile=ChildFrm.h
ImplementationFile=ChildFrm.cpp
Filter=M
LastObject=CChildFrame


[CLS:CAboutDlg]
Type=0
HeaderFile=WDBG.cpp
ImplementationFile=WDBG.cpp
Filter=D

[DLG:IDD_ABOUTBOX]
Type=1
Class=CAboutDlg
ControlCount=5
Control1=IDC_STATIC,static,1342177283
Control2=IDC_STATIC,static,1342308480
Control3=IDC_STATIC,static,1342308352
Control4=IDOK,button,1342373889
Control5=IDC_STATIC,static,1342308352

[MNU:IDR_MAINFRAME]
Type=1
Class=CMainFrame
Command1=ID_FILE_OPEN
Command2=ID_FILE_PRINT_SETUP
Command3=ID_FILE_MRU_FILE1
Command4=ID_APP_EXIT
Command5=ID_VIEW_TOOLBAR
Command6=ID_VIEW_STATUS_BAR
Command7=ID_APP_ABOUT
CommandCount=7

[TB:IDR_MAINFRAME]
Type=1
Class=CMainFrame
Command1=ID_FILE_OPEN
Command2=ID_FILE_SAVE
Command3=ID_EDIT_COPY
Command4=ID_FILE_PRINT
Command5=ID_DEBUG_GO
Command6=ID_DEBUG_BREAK
Command7=ID_DEBUG_STOP
Command8=ID_APP_ABOUT
CommandCount=8

[MNU:IDR_WDBGTYPE]
Type=1
Class=CWDBGLogView
Command1=ID_FILE_CLOSE
Command2=ID_FILE_SAVE
Command3=ID_FILE_SAVE_AS
Command4=ID_FILE_PRINT
Command5=ID_FILE_PRINT_PREVIEW
Command6=ID_FILE_PRINT_SETUP
Command7=ID_APP_EXIT
Command8=ID_EDIT_UNDO
Command9=ID_EDIT_CUT
Command10=ID_EDIT_COPY
Command11=ID_EDIT_PASTE
Command12=ID_EDIT_PROJECT
Command13=ID_VIEW_TOOLBAR
Command14=ID_VIEW_STATUS_BAR
Command15=ID_VIEW_CALLSTACK
Command16=ID_VIEW_DISASSEMBLY
Command17=ID_VIEW_MEMORY
Command18=ID_VIEW_REGISTERS
Command19=ID_VIEW_SYMBOLS
Command20=ID_DEBUG_GO
Command21=ID_DEBUG_STOP
Command22=ID_DEBUG_BREAK
Command23=ID_DEBUG_BREAKPOINTS
Command24=ID_WINDOW_NEW
Command25=ID_WINDOW_CASCADE
Command26=ID_WINDOW_TILE_HORZ
Command27=ID_WINDOW_ARRANGE
Command28=ID_APP_ABOUT
CommandCount=28

[ACL:IDR_MAINFRAME]
Type=1
Class=CMainFrame
Command1=ID_EDIT_COPY
Command2=ID_FILE_NEW
Command3=ID_FILE_OPEN
Command4=ID_FILE_PRINT
Command5=ID_FILE_SAVE
Command6=ID_EDIT_PASTE
Command7=ID_EDIT_UNDO
Command8=ID_EDIT_CUT
Command9=ID_DEBUG_GO
Command10=ID_NEXT_PANE
Command11=ID_PREV_PANE
Command12=ID_EDIT_COPY
Command13=ID_EDIT_PASTE
Command14=ID_EDIT_CUT
Command15=ID_EDIT_UNDO
CommandCount=15

[CLS:CDBGChildFrame]
Type=0
HeaderFile=DBGChildFrame.h
ImplementationFile=DBGChildFrame.cpp
BaseClass=CMDIChildWnd
Filter=M

[CLS:CWDBGChildFrame]
Type=0
HeaderFile=WDBGChildFrame.h
ImplementationFile=WDBGChildFrame.cpp
BaseClass=CMDIChildWnd
Filter=M
VirtualFilter=mfWC
LastObject=CWDBGChildFrame

[DLG:IDD_PROJECTSETTINGS]
Type=1
Class=CProjectSettings
ControlCount=7
Control1=IDC_STATIC,static,1342308352
Control2=IDC_EXEPROGRAM,edit,1350631552
Control3=IDC_STATIC,static,1342308352
Control4=IDC_CMDLINEOPTS,edit,1350631552
Control5=IDC_STOPONLDRBP,button,1342242819
Control6=IDOK,button,1342242817
Control7=IDCANCEL,button,1342242816

[CLS:CProjectSettings]
Type=0
HeaderFile=ProjectSettings.h
ImplementationFile=ProjectSettings.cpp
BaseClass=CDialog
Filter=D
VirtualFilter=dWC
LastObject=CProjectSettings

[CLS:CDocNotifyWnd]
Type=0
HeaderFile=DocNotifyWnd.h
ImplementationFile=DocNotifyWnd.cpp
BaseClass=generic CWnd
Filter=W
LastObject=CDocNotifyWnd

[DLG:IDD_MEMTODISPLAY]
Type=1
Class=CMemoryDisplayDlg
ControlCount=10
Control1=IDC_STATIC,static,1342308352
Control2=IDC_ADDRESS,edit,1350631552
Control3=IDC_STATIC,button,1342177287
Control4=IDC_BYTE,button,1342177289
Control5=IDC_WORD,button,1342177289
Control6=IDC_DWORD,button,1342177289
Control7=IDC_STATIC,static,1342308352
Control8=IDC_LENGTH,edit,1350639744
Control9=IDOK,button,1342242817
Control10=IDCANCEL,button,1342242816

[CLS:CMemoryDisplayDlg]
Type=0
HeaderFile=MemoryDisplayDlg.h
ImplementationFile=MemoryDisplayDlg.cpp
BaseClass=CDialog
Filter=D
VirtualFilter=dWC
LastObject=CMemoryDisplayDlg

[DLG:IDD_DISASMTODISPLAY]
Type=1
Class=CDisasmDisplayDlg
ControlCount=6
Control1=IDC_STATIC,static,1342308352
Control2=IDC_ADDRESS,edit,1350631552
Control3=IDC_STATIC,static,1342308352
Control4=IDC_INSTRUCTIONCOUNT,edit,1350639744
Control5=IDOK,button,1342242817
Control6=IDCANCEL,button,1342242816

[CLS:CDisasmDisplayDlg]
Type=0
HeaderFile=DisasmDisplayDlg.h
ImplementationFile=DisasmDisplayDlg.cpp
BaseClass=CDialog
Filter=D
LastObject=CDisasmDisplayDlg
VirtualFilter=dWC

[DLG:IDD_SYMTODISPLAY]
Type=1
Class=CSymbolsDisplayDlg
ControlCount=4
Control1=IDC_STATIC,static,1342308352
Control2=IDC_MODCOMBO,combobox,1344340227
Control3=IDOK,button,1342242817
Control4=IDCANCEL,button,1342242816

[CLS:CSymbolsDisplayDlg]
Type=0
HeaderFile=SymbolsDisplayDlg.h
ImplementationFile=SymbolsDisplayDlg.cpp
BaseClass=CDialog
Filter=D
VirtualFilter=dWC
LastObject=CSymbolsDisplayDlg

[CLS:CCallStackDisplayDlg]
Type=0
HeaderFile=CallStackDisplayDlg.h
ImplementationFile=CallStackDisplayDlg.cpp
BaseClass=CDialog
Filter=D
VirtualFilter=dWC
LastObject=CCallStackDisplayDlg

[DLG:IDD_THREADPICKER]
Type=1
Class=CThreadPickerDlg
ControlCount=4
Control1=IDC_STATIC,static,1342308352
Control2=IDC_THREADCOMBO,combobox,1344340227
Control3=IDOK,button,1342242817
Control4=IDCANCEL,button,1342242816

[CLS:CThreadPickerDlg]
Type=0
HeaderFile=ThreadPickerDlg.h
ImplementationFile=ThreadPickerDlg.cpp
BaseClass=CDialog
Filter=D
VirtualFilter=dWC
LastObject=CThreadPickerDlg

[DLG:IDD_BREAKPOINTS]
Type=1
Class=CBreakpointsDlg
ControlCount=6
Control1=IDC_BPLIST,listbox,1352728913
Control2=IDNEW,button,1342242817
Control3=IDEDIT,button,1342242817
Control4=IDREMOVE,button,1342242817
Control5=IDOK,button,1342242817
Control6=IDCANCEL,button,1342242816

[CLS:CBreakpointsDlg]
Type=0
HeaderFile=BreakpointsDlg.h
ImplementationFile=BreakpointsDlg.cpp
BaseClass=CDialog
Filter=D
VirtualFilter=dWC
LastObject=IDC_BPLIST

[DLG:IDD_NEWEDITBP]
Type=1
Class=CNewEditBPDlg
ControlCount=8
Control1=IDC_STATIC,static,1342308352
Control2=IDC_BPTYPE,combobox,1344339971
Control3=IDC_STATIC,static,1342308352
Control4=IDC_BPLOCATION,edit,1350631552
Control5=IDC_PASSCOUNTLABEL,static,1342308352
Control6=IDC_PASSCOUNTEDIT,edit,1350639744
Control7=IDOK,button,1342242817
Control8=IDCANCEL,button,1342242816

[CLS:CNewEditBPDlg]
Type=0
HeaderFile=NewEditBPDlg.h
ImplementationFile=NewEditBPDlg.cpp
BaseClass=CDialog
Filter=D
VirtualFilter=dWC
LastObject=CNewEditBPDlg

