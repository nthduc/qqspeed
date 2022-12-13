// stdafx.cpp : source file that includes just the standard includes
//	Test.pch will be the pre-compiled header
//	stdafx.obj will contain the pre-compiled type information

#pragma warning(disable:4786)
#include "stdafx.h"


#if defined(_DEBUG) || defined(DEBUG)
#pragma comment(lib, "vfs_d")
#else
#pragma comment(lib, "vfs")
#endif