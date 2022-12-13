// stdafx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//      are changed infrequently
//

#if !defined(AFX_STDAFX_H__17968A3A_0755_42E5_9AF7_0F8E5C363882__INCLUDED_)
#define AFX_STDAFX_H__17968A3A_0755_42E5_9AF7_0F8E5C363882__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define VC_EXTRALEAN		// Exclude rarely-used stuff from Windows headers

#define _WIN32_WINNT	0x500
#include <afxwin.h>         // MFC core and standard components
#include <afxext.h>         // MFC extensions
#include <afxcview.h>
#include <afxdisp.h>        // MFC Automation classes
#include <afxdtctl.h>		// MFC support for Internet Explorer 4 Common Controls
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>			// MFC support for Windows Common Controls
#endif // _AFX_NO_AFXCMN_SUPPORT

#define VFS_PORTABLE
#include "vfs/vfs_i.h"
using namespace VFS;

#include "common.h"
#include "log.h"
//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STDAFX_H__17968A3A_0755_42E5_9AF7_0F8E5C363882__INCLUDED_)
