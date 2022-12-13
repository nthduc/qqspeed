/*----------------------------------------------------------------------
Debugging Applications for Microsoft .NET and Microsoft Windows
Copyright © 1997-2003 John Robbins -- All rights reserved.
----------------------------------------------------------------------*/

#pragma once

#define _CRTDBG_MAP_ALLOC

//#define _ATL_DEBUG_INTERFACES

#ifndef STRICT
#define STRICT
#endif

#define _WIN32_WINNT 0x501
#define WINVER 0x501

#define _ATL_APARTMENT_THREADED
#define _ATL_NO_AUTOMATIC_NAMESPACE

#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS	// some CString constructors will be explicit

// turns off ATL's hiding of some common and often safely ignored warning messages
#define _ATL_ALL_WARNINGS

#include <atlbase.h>
#include <atlcom.h>
#include <atlwin.h>
#include <atltypes.h>
#include <atlctl.h>
#include <atlhost.h>

#include <WinAble.h>
#include <math.h>
#include "BugslayerUtil.h"

#include <crtdbg.h>

using namespace ATL;