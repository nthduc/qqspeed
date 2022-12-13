/*----------------------------------------------------------------------
Debugging Applications for Microsoft .NET and Microsoft Windows
Copyright © 1997-2003 John Robbins -- All rights reserved.
----------------------------------------------------------------------*/

#ifndef _TNOTIFYHLP_H
#define _TNOTIFYHLP_H

#ifdef __cplusplus
extern "C" {
#endif

/*//////////////////////////////////////////////////////////////////////
                            Special Defines
//////////////////////////////////////////////////////////////////////*/
// The defines that set up how the functions or classes are exported or
// imported.
#ifndef TNOTIFYHLP_DLLINTERFACE
#ifdef BUILDING_TNOTIFYHLP_DLL
#define TNOTIFYHLP_DLLINTERFACE __declspec ( dllexport )
#else
#define TNOTIFYHLP_DLLINTERFACE __declspec ( dllimport )
#endif  // BUILDING_TNOTIFYHLP_DLL
#endif  // TNOTIFYHLP_DLLINTERFACE

/*//////////////////////////////////////////////////////////////////////
                                Defines
//////////////////////////////////////////////////////////////////////*/
// The defines that set the type of notification desired.
// Just window destruction.
#define ANTN_DESTROYWINDOW      0x1
// Just window creation.
#define ANTN_CREATEWINDOW       0x2
// Both creation and destruction.
#define ANTN_CREATEANDDESTROY   (ANTN_DESTROYWINDOW | ANTN_CREATEWINDOW)

// The defines that set the searching type when adding a title to get
// notifications on.
// The string must match exactly the window title.
#define ANTS_EXACTMATCH  0
// The string must be in the beginning of the window title.
#define ANTS_BEGINMATCH  1
// The string can appear anywhere in the window title.
#define ANTS_ANYLOCMATCH 2

/*----------------------------------------------------------------------
FUNCTION        :   AddNotifyTitle
DISCUSSION      :
    Adds the specified notification title to the list of windows to
watch for and notify on.  The type of notification is set with the
ANTN_* defines above.  The window title is searched according to the
iSearchType settings.
    There are a limited number of slots for notifications so this
function can fail.  See the TOTAL_NOTIFY_SLOTS in TNotifyHlp.cpp for the
exact number.
PARAMETERS      :
    iNotifyType - The type of notification as specified by the ANTN_*
                  defins above.
    iSearchType - The way to indicate a match.  See the ANT_* defines
                  above.
    szTitle     - The name to look for.  The longest name supported is
                  MAX_PATH characters long.
RETURNS         :
    INVALID_HANDLE_VALUE - The item could not be added.
    Otherwise            - The item was added and the returned handle
                           must be passed to all other functions.
----------------------------------------------------------------------*/
HANDLE TNOTIFYHLP_DLLINTERFACE __stdcall
    AddNotifyTitle ( int     iNotifyType ,
                     int     iSearchType ,
                     LPCTSTR szString     ) ;

/*----------------------------------------------------------------------
FUNCTION        :   RemoveNotifyTitle
DISCUSSION      :
    This removes a previously added notification title.
PARAMETERS      :
    hItem - A previously added item.
RETURNS         :
    None.
----------------------------------------------------------------------*/
void TNOTIFYHLP_DLLINTERFACE __stdcall
    RemoveNotifyTitle ( HANDLE hItem ) ;

/*----------------------------------------------------------------------
FUNCTION        :   CheckNotifyCreateTitle
DISCUSSION      :
    Returns the HWND if the title in hItem has been created.  A return
of 0 means the item was not created.
PARAMETERS      :
    hItem - A previously added item.
RETURNS         :
    !NULL - The item has been "triggered."
    NULL  - The item has not been triggered.
----------------------------------------------------------------------*/
HWND TNOTIFYHLP_DLLINTERFACE __stdcall
    CheckNotifyCreateTitle ( HANDLE hItem ) ;

/*----------------------------------------------------------------------
FUNCTION        :   CheckNotifyDestroyTitle
DISCUSSION      :
    Returns TRUE if the item was destroyed, FALSE otherwise.
PARAMETERS      :
    hItem - A previously added item.
RETURNS         :
    TRUE  - The item has been destroyed.
    FALSE - The item has not been destroyed
----------------------------------------------------------------------*/
BOOL TNOTIFYHLP_DLLINTERFACE __stdcall
    CheckNotifyDestroyTitle ( HANDLE hItem ) ;

#ifdef __cplusplus
}
#endif

#endif  // _TNOTIFYHLP_H


