/*----------------------------------------------------------------------
Debugging Applications for Microsoft .NET and Microsoft Windows
Copyright © 1997-2003 John Robbins -- All rights reserved.
----------------------------------------------------------------------*/
#ifndef _STDAFX_H
#define _STDAFX_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define WIN32_LEAN_AND_MEAN

// Turn off :
// the 'xx' : unreferenced inline function has been removed
#pragma warning ( push , 3 )
#pragma warning ( disable : 4514 )
#include <stdio.h>
#include <windows.h>
#include <tchar.h>
#include <psapi.h>
#pragma warning ( default : 4514 )
#pragma warning ( pop )

#include <BugslayerUtil.h>

#endif // _STDAFX_H
