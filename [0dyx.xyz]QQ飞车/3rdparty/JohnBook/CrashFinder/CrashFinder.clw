; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
LastClass=CChildFrame
LastTemplate=CDialog
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "crashfinder.h"
LastPage=0

ClassCount=3
Class1=CChildFrame
Class2=CCrashFinderEditView

ResourceCount=9
Resource1=IDR_CRASHFTYPE
Resource2=IDD_ABOUTBOX
Resource3=AFX_IDD_NEWTYPEDLG
Resource4=IDD_BINARYPROPERTIES
Resource5=IDD_FINDCRASH
Resource6=IDD_APPOPTIONS
Resource7=IDD_IGNOREDDLG
Resource8=IDD_DIALOG1
Class3=CIgnoreDlg
Resource9=IDR_MAINFRAME

[CLS:CChildFrame]
Type=0
HeaderFile=ChildFrm.h
ImplementationFile=ChildFrm.cpp
LastObject=CChildFrame

[CLS:CCrashFinderEditView]
Type=0
HeaderFile=CrashFinderEditView.h
ImplementationFile=CrashFinderEditView.cpp

[MNU:IDR_CRASHFTYPE]
Type=1
Class=?
Command1=ID_FILE_NEW
Command2=ID_FILE_OPEN
Command3=ID_FILE_CLOSE
Command4=ID_FILE_SAVE
Command5=ID_FILE_SAVE_AS
Command6=ID_FILE_MRU_FILE1
Command7=ID_APP_EXIT
Command8=ID_EDIT_COPY
Command9=ID_EDIT_FIND_CRASH
Command10=ID_EDIT_ADD_IMAGE
Command11=ID_EDIT_REMOVE_IMAGE
Command12=ID_EDIT_IMAGE_PROPERTIES
Command13=ID_EDIT_IGNORED_IMAGE
Command14=ID_VIEW_TOOLBAR
Command15=ID_VIEW_STATUS_BAR
Command16=ID_VIEW_OPTIONS
Command17=ID_WINDOW_CASCADE
Command18=ID_WINDOW_TILE_HORZ
Command19=ID_WINDOW_ARRANGE
Command20=ID_APP_ABOUT
CommandCount=20

[TB:IDR_MAINFRAME]
Type=1
Class=?
Command1=ID_FILE_NEW
Command2=ID_FILE_OPEN
Command3=ID_FILE_SAVE
Command4=ID_EDIT_COPY
Command5=ID_EDIT_FIND_CRASH
Command6=ID_EDIT_ADD_IMAGE
Command7=ID_EDIT_REMOVE_IMAGE
Command8=ID_EDIT_IMAGE_PROPERTIES
Command9=ID_APP_ABOUT
CommandCount=9

[MNU:IDR_MAINFRAME]
Type=1
Class=?
Command1=ID_FILE_NEW
Command2=ID_FILE_OPEN
Command3=ID_FILE_MRU_FILE1
Command4=ID_APP_EXIT
Command5=ID_EDIT_IGNORED_IMAGE
Command6=ID_VIEW_TOOLBAR
Command7=ID_VIEW_STATUS_BAR
Command8=ID_VIEW_OPTIONS
Command9=ID_APP_ABOUT
CommandCount=9

[ACL:IDR_MAINFRAME]
Type=1
Class=?
Command1=ID_EDIT_ADD_IMAGE
Command2=ID_EDIT_COPY
Command3=ID_EDIT_FIND_CRASH
Command4=ID_FILE_NEW
Command5=ID_FILE_OPEN
Command6=ID_EDIT_REMOVE_IMAGE
Command7=ID_FILE_SAVE
Command8=ID_EDIT_COPY
Command9=ID_NEXT_PANE
Command10=ID_PREV_PANE
CommandCount=10

[DLG:IDD_ABOUTBOX]
Type=1
Class=?
ControlCount=7
Control1=IDC_BIGICON,static,1342177283
Control2=IDC_STATIC,static,1342308353
Control3=IDC_STATIC,static,1342177287
Control4=IDC_HINTS,edit,1352730756
Control5=IDOK,button,1342242817
Control6=IDC_STATIC,static,1342308353
Control7=IDC_APPNAME,static,1342308353

[DLG:IDD_APPOPTIONS]
Type=1
Class=?
ControlCount=4
Control1=IDC_SHOWFULLPATHS,button,1342242819
Control2=IDC_CONFIRMDELETIONS,button,1342242819
Control3=IDOK,button,1342242817
Control4=IDCANCEL,button,1342242816

[DLG:IDD_BINARYPROPERTIES]
Type=1
Class=?
ControlCount=5
Control1=IDC_STATIC,static,1342308352
Control2=IDC_LOADADDREDIT,edit,1350631552
Control3=IDOK,button,1342242817
Control4=IDCANCEL,button,1342242816
Control5=IDC_STATIC,static,1342308352

[DLG:IDD_FINDCRASH]
Type=1
Class=?
ControlCount=14
Control1=IDC_STATIC,static,1342308352
Control2=IDC_HEXEDIT,edit,1350631552
Control3=IDC_FIND,button,1342242817
Control4=IDCANCEL,button,1342242816
Control5=IDC_STATIC,static,1342177287
Control6=IDC_ADDRESS,button,1342242819
Control7=IDC_MODULE,button,1342242819
Control8=IDC_FUNCTION,button,1342242819
Control9=IDC_FNDISPLACEMENT,button,1342242819
Control10=IDC_SOURCEFILE,button,1342242819
Control11=IDC_SOURCELINE,button,1342242819
Control12=IDC_SRCDISPLACEMENT,button,1342242819
Control13=IDC_ONELINE,button,1342242819
Control14=IDC_DETAILS,edit,1353779396

[DLG:AFX_IDD_NEWTYPEDLG]
Type=1
Class=?
ControlCount=5
Control1=IDC_STATIC,static,1342177280
Control2=AFX_IDC_LISTBOX,listbox,1352728577
Control3=IDOK,button,1342242817
Control4=IDCANCEL,button,1342242816
Control5=ID_HELP,button,1342242816

[DLG:IDD_DIALOG1]
Type=1
ControlCount=2
Control1=IDOK,button,1342242817
Control2=IDCANCEL,button,1342242816

[DLG:IDD_IGNOREDDLG]
Type=1
Class=CIgnoreDlg
ControlCount=5
Control1=IDOK,button,1342242817
Control2=IDCANCEL,button,1342242816
Control3=IDC_IGNORELIST,listbox,1352728923
Control4=IDC_ADD,button,1342242816
Control5=IDC_DELETE,button,1342242816

[CLS:CIgnoreDlg]
Type=0
HeaderFile=IgnoreDlg.h
ImplementationFile=IgnoreDlg.cpp
BaseClass=CDialog
Filter=D
VirtualFilter=dWC
LastObject=CIgnoreDlg

