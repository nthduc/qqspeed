/*----------------------------------------------------------------------
Debugging Applications for Microsoft .NET and Microsoft Windows
Copyright © 1997-2003 John Robbins -- All rights reserved.
----------------------------------------------------------------------*/

#ifndef _VERBOSE_H
#define _VERBOSE_H

/*//////////////////////////////////////////////////////////////////////
// Necessary Includes
//////////////////////////////////////////////////////////////////////*/

/*//////////////////////////////////////////////////////////////////////
// Constants & Defines
//////////////////////////////////////////////////////////////////////*/

/*//////////////////////////////////////////////////////////////////////
// Typedefs
//////////////////////////////////////////////////////////////////////*/

/*//////////////////////////////////////////////////////////////////////
// Prototypes
//////////////////////////////////////////////////////////////////////*/

/*----------------------------------------------------------------------
FUNCTION        :   SetVerbose
DISCUSSION      :
    Turns on or off the verbose flag.
PARAMETERS      :
    TRUE  - Turns Verbose on.
    FALSE - Turns Verbose off.
RETURNS         :
    Nothing.
----------------------------------------------------------------------*/
void SetVerbose ( BOOL bVerbose ) ;

/*----------------------------------------------------------------------
FUNCTION        :   VerboseOut
DISCUSSION      :
    Prints the message only if verbose is on.
PARAMETERS      :
    szFmt - The format string.  Conforms to the printf standard.
    ...   - Fill for the format string.
RETURNS         :
    Nothing.
----------------------------------------------------------------------*/
void VerboseOut ( LPCTSTR szFmt , ... ) ;

/*----------------------------------------------------------------------
FUNCTION        :   WarningOut
DISCUSSION      :
    Ensures the message is seen even if output is redirected.
PARAMETERS      :
    szFmt - The format string.  Conforms to the printf standard.
    ...   - Fill for the format string.
RETURNS         :
    Nothing.
----------------------------------------------------------------------*/
void WarningOut ( LPCTSTR szFmt , ... ) ;

#endif  // _VERBOSE_H


