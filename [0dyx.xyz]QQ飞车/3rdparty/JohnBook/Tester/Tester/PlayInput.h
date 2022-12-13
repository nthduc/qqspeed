/*----------------------------------------------------------------------
Debugging Applications for Microsoft .NET and Microsoft Windows
Copyright © 1997-2003 John Robbins -- All rights reserved.
------------------------------------------------------------------------
The PlayInput function.

I tried to make this ready for internationalization, but I have not
tested this with different keyboard layouts.  If you find that it is not
ready, I would appreciate if you would let me know!
----------------------------------------------------------------------*/

#ifndef _PLAYINPUT_H
#define _PLAYINPUT_H

#ifdef __cplusplus
extern "C" {
#endif

/*//////////////////////////////////////////////////////////////////////
// The return defines for PlayInput
//////////////////////////////////////////////////////////////////////*/
#define PI_SUCCESS        0
#define PI_INVALIDPARAMS  1
#define PI_PARSEERROR     2
#define PI_ERROR          3
#define PI_CRASH          5

/*----------------------------------------------------------------------
FUNCTION        :   PlayInput
DISCUSSION      :
    This function takes a string that corresponds to the input to play.
The string format follows the Visual Basic SendKey statement rules.
For the first version of this function does not support the repeat keys
construct, "{key number}".
    If the string parses properly, the keys get sent through the
SendInput function.
    The caller is responsible for ensuring that the system is set up to
properly handle the upcoming keystrokes.  Needless to say, this function
could prove quite dangerous.
PARAMETERS      :
    szString  - The string to parse.
    puiErrPos - The pointer that will recieve the position of the error
                character in the string.  If the functions succeeds,
                this is undefined.
RETURNS         :
    PI_SUCCESS       - The piRetBuff holds the parsed string.
    PI_INVALIDPARAMS - One or more of the parameters were invalid.
    PI_PARSEERROR    - szString has an invalid character in it.
    PI_ERROR         - There was a completely unexpected problem.
    PI_CRASH         - There was a crash inside the function.
----------------------------------------------------------------------*/
UINT __stdcall PlayInput ( LPCTSTR szString , LPUINT puiErrPos  ) ;

/*----------------------------------------------------------------------
FUNCTION        :   SetUseAbsoluteScreenCoordinates
DISCUSSION      :
Sets the global option to use absolute screen coordinates for
playback items using coordinates.
PARAMETERS      :
bUseAbsolute - If FALSE, virtual screen coordinates will be used.
If TRUE, absolute screen coordinates will be used.
RETURNS         :
The previous setting.
----------------------------------------------------------------------*/
BOOL __stdcall SetUseAbsoluteScreenCoordinates ( BOOL bUseAbsolute ) ;

/*----------------------------------------------------------------------
FUNCTION        :   GetUseAbsoluteScreenCoordinates
DISCUSSION      :
Gets the global option to use absolute screen coordinates for
playback items using coordinates.
PARAMETERS      :
None.
RETURNS         :
TRUE  - Absolute screen coordinates are in use.
FALSE - Virtual screen coordinates are in use.
----------------------------------------------------------------------*/
BOOL __stdcall GetUseAbsoluteScreenCoordinates ( void ) ;

/*----------------------------------------------------------------------
FUNCTION        :   SetMultiMonitor
DISCUSSION      :
    Sets the global option to use support multi monitor systems or not.
PARAMETERS      :
    bUseMM - TRUE, coordinates reflect the whole virtual screen.
             FALSE, only use the primary monitor.
RETURNS         :
    The previous setting.
----------------------------------------------------------------------*/
BOOL __stdcall SetMultiMonitor ( BOOL bUseMM ) ;

/*----------------------------------------------------------------------
FUNCTION        :   GetMultiMonitor
DISCUSSION      :
    Gets the global option indicating if multi monitor support is
enabled.
PARAMETERS      :
    None.
RETURNS         :
    TRUE  - Multi monitor usage is on.
    FALSE - It's off.
----------------------------------------------------------------------*/
BOOL __stdcall GetMultiMonitor ( void ) ;


#ifdef __cplusplus
}
#endif

#endif  // _PLAYINPUT_H


