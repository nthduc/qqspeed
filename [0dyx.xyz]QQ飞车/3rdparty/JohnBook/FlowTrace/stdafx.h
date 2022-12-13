/*----------------------------------------------------------------------
Debugging Applications for Microsoft .NET and Windows Applications
Copyright © 2002-2003 John Robbins
----------------------------------------------------------------------*/
#pragma once

#define WIN32_LEAN_AND_MEAN
#define _WIN32_WINNT 0x0500
#include <windows.h>
#include <tchar.h>
#include <stdio.h>
#include "ProfilerLib.h"

// Why won't this compile at warning level 4 in a release build?  Sheez!
// Also, the XTREE file has unreachable code warnings on the following
// lines:  1132, 1112, 1114, 1117 so I have to disable that one for the
// complete project in release builds.  Grumble, grumble....
#ifdef NDEBUG
#pragma warning ( disable : 4702 )
#endif
#pragma warning ( push , 3 )
#include <map>
using namespace std ;
#pragma warning ( pop )

#include <BugslayerUtil.h>

// Utilize the sexy (and safe!) string functions.
// Use the library.
#define STRSAFE_LIB
// Only use character count functions.
#define STRSAFE_NO_CB_FUNCTIONS
#include <strsafe.h>




