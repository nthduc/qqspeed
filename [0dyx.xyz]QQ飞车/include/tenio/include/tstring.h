#ifndef __TSTRING_H__
#define __TSTRING_H__

#include <string>
#include <fstream>
#include <tchar.h>

#ifndef tstring
	#ifdef _UNICODE
		#define tstring		wstring
		#define tifstream	wifstream		
		#define tofstream	wofstream	
		#define tfstream	wfstream	
	#else
		#define tstring string
		#define tifstream	ifstream		
		#define tofstream	ofstream	
		#define tfstream	fstream	
	#endif
#endif

#endif