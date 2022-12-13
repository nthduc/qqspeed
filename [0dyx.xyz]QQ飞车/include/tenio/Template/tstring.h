#ifndef __TSTRING_H__
#define __TSTRING_H__

#include <string>
#include <tchar.h>

#ifndef tstring
	#ifdef _UNICODE
		#define tstring wstring
	#else
		#define tstring string
	#endif
#endif

#endif