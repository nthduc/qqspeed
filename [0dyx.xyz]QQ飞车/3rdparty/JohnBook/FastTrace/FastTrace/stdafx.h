/*----------------------------------------------------------------------
Debugging Applications for Microsoft .NET and Microsoft Windows
Copyright © 1997-2003 John Robbins -- All rights reserved.
----------------------------------------------------------------------*/
#pragma once

#define _WIN32_WINNT 0x501
#define WINVER 0x501
#define WIN32_LEAN_AND_MEAN

#include <windows.h>
#include <tchar.h>
#include <malloc.h>

// Tell StrSafe to link against the lib and use the LIB and only
// character counted functions.
#define STRSAFE_LIB
#define STRSAFE_NO_CB_FUNCTIONS
#include "StrSafe.h"

#include "BugslayerUtil.h"

#include "JSimpleColl.h"

