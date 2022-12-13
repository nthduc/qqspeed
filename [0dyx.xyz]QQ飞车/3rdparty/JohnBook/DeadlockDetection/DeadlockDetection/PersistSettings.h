/*----------------------------------------------------------------------
Debugging Applications for Microsoft .NET and Microsoft Windows
Copyright © 1997-2003 John Robbins -- All rights reserved.
------------------------------------------------------------------------
All the persistent storage handling routines
----------------------------------------------------------------------*/

#ifndef _PERSISTSETTINGS_H
#define _PERSISTSETTINGS_H

/*----------------------------------------------------------------------
FUNCTION        :   SetPersistentFilename
DISCUSSION      :
    Sets up this unit by calculating the location of the persistent file
based off an HINSTANCE.  This function MUST be called before any other
in this subsystem.
PARAMETERS      :
    hMod    - The module to use.
RETURNS         :
    None.
----------------------------------------------------------------------*/
void SetPersistentFilename ( HINSTANCE hMod ) ;

/*----------------------------------------------------------------------
FUNCTION        :   GetExtDllFilename
DISCUSSION      :
    Loads the extension DLL filename.
PARAMETERS      :
    szBuff - The buffer to copy the name into.
    uiSize - The size in characters of szBuff.
    szDef  - The default name.
RETURNS         :
    The number of characters copied into szBuff.
----------------------------------------------------------------------*/
int GetExtDllFilename ( LPTSTR szBuff , UINT uiSize , LPCTSTR szDef ) ;

/*----------------------------------------------------------------------
FUNCTION        :   StartInDllMain
DISCUSSION      :
    Returns TRUE if the deadlock detection system is supposed to start
as soon as the DLL is loaded.
PARAMETERS      :
    bDef - The default if the storage does not say.
RETURNS         :
    1 - Start the system as soon as the DLL is loaded.
    0 - Do not start the system until directed.
----------------------------------------------------------------------*/
BOOL StartInDllMain ( BOOL bDef ) ;

/*----------------------------------------------------------------------
FUNCTION        :   GetIgnoreModule
DISCUSSION      :
    Gets the specific module to ignore.
PARAMETERS      :
    iMod  - The module number to ignore.
    szMod - The returned module name.  Assumed to be MAX_PATH
            characters.
RETURNS         :
    1 - Module number was found.
    0 - Module was not found.
----------------------------------------------------------------------*/
BOOL GetIgnoreModule ( int iMod , TCHAR * szMod ) ;

/*----------------------------------------------------------------------
FUNCTION        :   DefaultEventOptions
DISCUSSION      :
    Retrieves the default event logging options.
PARAMETERS      :
    dwDef - The default if the storage does not say.
RETURNS         :
    The default options.
----------------------------------------------------------------------*/
DWORD DefaultEventOptions ( DWORD dwDef ) ;

#endif  // _PERSISTSETTINGS_H


