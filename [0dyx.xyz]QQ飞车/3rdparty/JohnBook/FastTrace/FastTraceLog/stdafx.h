/*----------------------------------------------------------------------
Debugging Applications for Microsoft .NET and Microsoft Windows
Copyright © 1997-2003 John Robbins -- All rights reserved.
----------------------------------------------------------------------*/
#pragma once

#define WIN32_LEAN_AND_MEAN
#include <stdio.h>
#include <stdlib.h>
#include <tchar.h>
#include <windows.h>

#include "JString.h"

// Tell StrSafe to link against the lib and use the LIB and only
// character counted functions.
//#define STRSAFE_LIB
//#define STRSAFE_NO_CB_FUNCTIONS
//#include "StrSafe.h"

#include "BugslayerUtil.h"
#include "JSimpleColl.h"


