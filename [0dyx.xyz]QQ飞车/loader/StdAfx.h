// stdafx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//      are changed infrequently
//

#if !defined(AFX_STDAFX_H__CAD4E1BB_FCFA_49C0_86FE_8E86FC246614__INCLUDED_)
#define AFX_STDAFX_H__CAD4E1BB_FCFA_49C0_86FE_8E86FC246614__INCLUDED_

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

#include <afxsock.h>		// MFC socket extensions

#include <atlbase.h>
#include <afxocc.h>

typedef enum tagServerEntity
{
	eQQServer	= 0,
	eDirServer	= 1,
	eGameServer = 2,
	eShopServer = 3,
}ServerEntity;

typedef enum tagLoginResult
{
	eLoginSuccessed			= 0,
	eLoginFailed			= 1,
	eLoginTimeout			= 2,
	eLoginRequireReg		= 3,
	eLoginCanceled			= 4,
	eLoginIdentityLimit		= 5,
	eLoginVersionLimit		= 6,
	eLoginVersionRecommend	= 7,
	eLoginFileCracked		= 8,
}LoginResult;

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STDAFX_H__CAD4E1BB_FCFA_49C0_86FE_8E86FC246614__INCLUDED_)
