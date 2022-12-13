// stdafx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//      are changed infrequently
//

#if !defined(AFX_STDAFX_H__D9A49CE6_AA54_4D89_82CB_5F6D12711990__INCLUDED_)
#define AFX_STDAFX_H__D9A49CE6_AA54_4D89_82CB_5F6D12711990__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define VC_EXTRALEAN		// Exclude rarely-used stuff from Windows headers

#include <afxwin.h>         // MFC core and standard components
#include <afxext.h>         // MFC extensions
#include <afxdisp.h>        // MFC Automation classes
#include <afxdtctl.h>		// MFC support for Internet Explorer 4 Common Controls
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>			// MFC support for Windows Common Controls
#endif // _AFX_NO_AFXCMN_SUPPORT

//#include "..\\..\\..\\Template\\tenio_t.h"
//#include "..\\..\\..\\Include\\VFS\\vfs_i.h"

//using namespace Tenio;
//#define TENIO_INI "..\\..\\..\\Tenio.ini"

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.
#define VFS_PORTABLE
#include "../interface/vfs_i.h"
using namespace VFS;

#endif // !defined(AFX_STDAFX_H__D9A49CE6_AA54_4D89_82CB_5F6D12711990__INCLUDED_)
