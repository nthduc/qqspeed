/*----------------------------------------------------------------------
Debugging Applications for Microsoft .NET and Microsoft Windows
Copyright © 1997-2003 John Robbins -- All rights reserved.
----------------------------------------------------------------------*/

#include "stdafx.h"
#include "resource.h"

// The module attribute causes DllMain, DllRegisterServer and DllUnregisterServer to be automatically implemented for you
[ module(dll, uuid = "{A521073B-A3E0-442B-8831-51906958B07A}",
		 name = "Tester",
		 helpstring = "Tester 1.0 Type Library",
		 resource_name = "IDR_TESTER") ];
