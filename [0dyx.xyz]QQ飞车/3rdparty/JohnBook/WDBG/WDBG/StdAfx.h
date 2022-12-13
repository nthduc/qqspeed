/*----------------------------------------------------------------------
Debugging Applications for Microsoft .NET and Microsoft Windows
Copyright © 1997-2003 John Robbins -- All rights reserved.
----------------------------------------------------------------------*/
#ifndef _STDAFX_H
#define _STDAFX_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define VC_EXTRALEAN
#define _WIN32_WINNT 0x501
#define WINVER 0x501

#include <stdlib.h>
#include <errno.h>

#include <afxwin.h>
#include <afxext.h>
#include <afxdtctl.h>
#include <afxtempl.h>

#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>
#endif // _AFX_NO_AFXCMN_SUPPORT

#pragma warning ( push , 3 )
#include <vector>
#include <map>
using namespace std ;
#pragma warning ( pop )

#include "BugslayerUtil.h"
#ifdef new
#undef new
#endif

#include "CPUHelp.h"
#include "DebugInterface.h"
#include "LocalAssist.h"

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // _STDAFX_H
