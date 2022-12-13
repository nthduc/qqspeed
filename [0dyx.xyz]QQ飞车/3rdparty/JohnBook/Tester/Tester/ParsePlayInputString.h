/*----------------------------------------------------------------------
Debugging Applications for Microsoft .NET and Microsoft Windows
Copyright © 1997-2003 John Robbins -- All rights reserved.
----------------------------------------------------------------------*/

#ifndef _PARSEPLAYINPUTSSTRING_H
#define _PARSEPLAYINPUTSSTRING_H

/*//////////////////////////////////////////////////////////////////////
// Includes
//////////////////////////////////////////////////////////////////////*/

// Include error returns from PlayKeys.
#include "PlayInput.h"

/*----------------------------------------------------------------------
FUNCTION        :   ParsePlayInputString
DISCUSSION      :
    Given a string, parses it and returns the VK_* keycodes and mouse
movement, clicks, and double clicks for each item in the string.  For
the most part, this follows the same conventions as the Visual Basic
SendKeys rules.
    The differences are
        - The repeat keys specifier, "{h 10}" is not supported.
        - Mouse input is:
            {MOVETO x , y}
            {BTNDOWN btn , x , y}
            {BTNUP btn , x , y}
            {CLICK btn , x , y}
            {DBLCLICK btn , x , y}
                btn = LEFT, RIGHT, MIDDLE (X1 and X2 are not supported)
                x   = The x coordinate.
                y   = The y coordinate.
        - To have CTRL, ALT, and SHIFT set up for use with the mouse,
          use {CTRL DOWN}, {CTRL UP}, {ALT DOWN}, {ALT UP}, {SHIFT DOWN}
          and {SHIFT UP}.
PARAMETERS      :
    szString  - The string to parse.
    uiErrChar - The character that caused the error.  Undefined if no
                error.
    pRetKeys  - The return buffer.  This memory is allocated with new
                and is up to the caller to delete [] or it will be
                leaked.
    uiCount   - The number of items in piRetBuff.
RETURNS         :
    PI_SUCCESS       - The piRetBuff holds the parsed string.
    PI_INVALIDPARAMS - One or more of the parameters were invalid.
    PI_PARSEERROR    - The input string was not properly formatted.
                         piErrChar points to the character.
    PI_ERROR         - There was a completely unexpected problem.
----------------------------------------------------------------------*/
UINT ParsePlayInputString ( IN  LPCTSTR  szString  ,
                            OUT UINT &   uiErrChar ,
                            OUT PINPUT & pRetKeys  ,
                            OUT UINT &   uiCount    ) ;

#endif  // _PARSEPLAYINPUTSSTRING_H


