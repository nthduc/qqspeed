========================================================================
       MICROSOFT FOUNDATION CLASS LIBRARY : vfstool
========================================================================


AppWizard has created this vfstool application for you.  This application
not only demonstrates the basics of using the Microsoft Foundation classes
but is also a starting point for writing your application.

This file contains a summary of what you will find in each of the files that
make up your vfstool application.

vfstool.dsp
    This file (the project file) contains information at the project level and
    is used to build a single project or subproject. Other users can share the
    project (.dsp) file, but they should export the makefiles locally.

vfstool.h
    This is the main header file for the application.  It includes other
    project specific headers (including Resource.h) and declares the
    CVfstoolApp application class.

vfstool.cpp
    This is the main application source file that contains the application
    class CVfstoolApp.

vfstool.rc
    This is a listing of all of the Microsoft Windows resources that the
    program uses.  It includes the icons, bitmaps, and cursors that are stored
    in the RES subdirectory.  This file can be directly edited in Microsoft
	Visual C++.

vfstool.clw
    This file contains information used by ClassWizard to edit existing
    classes or add new classes.  ClassWizard also uses this file to store
    information needed to create and edit message maps and dialog data
    maps and to create prototype member functions.

res\vfstool.ico
    This is an icon file, which is used as the application's icon.  This
    icon is included by the main resource file vfstool.rc.

res\vfstool.rc2
    This file contains resources that are not edited by Microsoft 
	Visual C++.  You should place all resources not editable by
	the resource editor in this file.



/////////////////////////////////////////////////////////////////////////////

For the main frame window:

MainFrm.h, MainFrm.cpp
    These files contain the frame class CMainFrame, which is derived from
    CFrameWnd and controls all SDI frame features.

res\Toolbar.bmp
    This bitmap file is used to create tiled images for the toolbar.
    The initial toolbar and status bar are constructed in the CMainFrame
    class. Edit this toolbar bitmap using the resource editor, and
    update the IDR_MAINFRAME TOOLBAR array in vfstool.rc to add
    toolbar buttons.
/////////////////////////////////////////////////////////////////////////////

AppWizard creates one document type and one view:

vfstoolDoc.h, vfstoolDoc.cpp - the document
    These files contain your CVfstoolDoc class.  Edit these files to
    add your special document data and to implement file saving and loading
    (via CVfstoolDoc::Serialize).

vfstoolView.h, vfstoolView.cpp - the view of the document
    These files contain your CVfstoolView class.
    CVfstoolView objects are used to view CVfstoolDoc objects.



/////////////////////////////////////////////////////////////////////////////
Other standard files:

StdAfx.h, StdAfx.cpp
    These files are used to build a precompiled header (PCH) file
    named vfstool.pch and a precompiled types file named StdAfx.obj.

Resource.h
    This is the standard header file, which defines new resource IDs.
    Microsoft Visual C++ reads and updates this file.

/////////////////////////////////////////////////////////////////////////////
Other notes:

AppWizard uses "TODO:" to indicate parts of the source code you
should add to or customize.

If your application uses MFC in a shared DLL, and your application is 
in a language other than the operating system's current language, you
will need to copy the corresponding localized resources MFC42XXX.DLL
from the Microsoft Visual C++ CD-ROM onto the system or system32 directory,
and rename it to be MFCLOC.DLL.  ("XXX" stands for the language abbreviation.
For example, MFC42DEU.DLL contains resources translated to German.)  If you
don't do this, some of the UI elements of your application will remain in the
language of the operating system.

/////////////////////////////////////////////////////////////////////////////
