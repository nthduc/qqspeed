// stdafx.cpp : source file that includes just the standard includes
//	vfstool.pch will be the pre-compiled header
//	stdafx.obj will contain the pre-compiled type information

#include "stdafx.h"


#if defined(_DEBUG) || defined(DEBUG)
#pragma comment(lib, "vfs_d")
#else
#pragma comment(lib, "vfs")
#endif