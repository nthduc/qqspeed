/*----------------------------------------------------------------------
Debugging Applications for Microsoft .NET and Microsoft Windows
Copyright © 1997-2003 John Robbins -- All rights reserved.
----------------------------------------------------------------------*/

#ifndef _PCH_H
#define _PCH_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#define WIN32_LEAN_AND_MEAN
#define _WIN32_WINNT 0x501
#define WINVER 0x501

// 4201 nonstandard extension used : nameless struct/union
// 4115 named type definition in parentheses
#pragma warning ( disable : 4201 4115 )
#include <windows.h>
#include <tchar.h>
#include <stdlib.h>
#include <process.h>
#include <PSAPI.h>
#include <tlhelp32.h>
#include <stddef.h>
#include <string.h>
#include "BugslayerUtil.h"

#endif  // _PCH_H

