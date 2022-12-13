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

#include <windows.h>
#include <tchar.h>
#include <stdlib.h>
#include <process.h>
#include <PSAPI.h>
#include <tlhelp32.h>
#pragma warning ( push , 3 )
#include <vector>
using namespace std ;
#pragma warning ( pop )

#include "CriticalSection.h"
#include "BugslayerUtil.h"
#include "LocalAssist.h"

#endif  // _PCH_H

