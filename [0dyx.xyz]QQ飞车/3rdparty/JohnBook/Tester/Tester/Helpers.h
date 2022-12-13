/*----------------------------------------------------------------------
Debugging Applications for Microsoft .NET and Microsoft Windows
Copyright © 1997-2003 John Robbins -- All rights reserved.
----------------------------------------------------------------------*/
#ifndef _HELPERS_H
#define _HELPERS_H

// The forward interface declarations to make everyone happy.
// Forward interface declarations necessary.
[idl_quote ( "interface ITWindow ;" ) ] ;
__interface ITWindow ;
[idl_quote ( "interface ITWindows ;" ) ] ;
__interface ITWindows ;

#include "TWindow.h"
#include "TWindows.h"


// A most useful typedef.
typedef CSimpleArray< HWND > HWNDARRAY ;

// Takes the array of HWNDS and builds them into an ITWindows object.
HRESULT BuildITWindowsFromHwndArray ( HWNDARRAY &  aHwnds ,
                                      ITWindows ** pVal    ) ;

#endif  // _HELPERS_H