#pragma once
//////////////////////////////////////////////////////////////////////////
//		传统的CRT的字符串操作函数，安全保障系数太低，一直为人诟病。
//	VC8也是费劲心思，采用了新封装的字符串操作函数集（function_s）
//	来进行相关参数合法和合理性的检查，并且采用了“Secure template overloads”
//	（也就是基于字符串数组大小的模板函数）机制来实现兼容老的代码
//	（有限制，不支持动态分配的），动态分配的还是要修改代码才能使用安全函数。
//		适应VC8的潮流吧，建议我们的字符串操作函数都采用function_s安全风格
//	版本，相关函数原型请参考VC8的MSDN说明。为了VC6/7都可以编译通过，写了一
//	个“include/safestring.h”的编译支持宏定义，在低版本还是转调传统的CRT字符串函数。
//
//		相关的safestring.h 的内容如下，看这样定义合适否，只定义了常用的，
//	有新的可以不断添加…………
//
//												 [6/7/2007] coolhomli
//
//////////////////////////////////////////////////////////////////////////

#include <tchar.h>
#include <stdio.h>
#include <stdarg.h>
#include <string.h>

namespace Tenio
{

#ifdef __cplusplus
extern "C++"
{
//////////////////////////////////////////////////////////////////////////
// 去掉VC8编译器中的针对老的不安全的字符串操作函数的警告， VC6/VC7中无效会忽略

//#ifndef _CRT_SECURE_NO_DEPRECATE
//	#define _CRT_SECURE_NO_DEPRECATE 
//#endif
//
//#ifndef _CRT_SECURE_NO_WARNINGS
//	#define _CRT_SECURE_NO_WARNINGS 
//#endif


//////////////////////////////////////////////////////////////////////////
// 通过下面的宏开关，无需修改老的代码，VC8中会通过“Secure Template Overloads”机制
// 来调用安全的替代函数， VC6/VC7中无效会忽略
// 注意：
//      仅仅限于操作静态字符数组形式的字符串（char szBuf[10];）
//		动态分配的起不到作用（char *szBuf = (char*)malloc(10);） ------------(1)

#ifdef _CRT_SECURE_CPP_OVERLOAD_STANDARD_NAMES
	#undef _CRT_SECURE_CPP_OVERLOAD_STANDARD_NAMES
#endif
#define _CRT_SECURE_CPP_OVERLOAD_STANDARD_NAMES 1		// 用于兼容旧代码中普通字符串函数

#ifdef _CRT_SECURE_CPP_OVERLOAD_STANDARD_NAMES_COUNT 
	#undef _CRT_SECURE_CPP_OVERLOAD_STANDARD_NAMES_COUNT 
#endif
#define _CRT_SECURE_CPP_OVERLOAD_STANDARD_NAMES_COUNT 1 // 用于兼容旧代码中带"n"类字符串函数

//////////////////////////////////////////////////////////////////////////
// 通过下面的宏开关，则需要修改函数名，加上"_s"后缀，同样VC8中会通过“Secure Template Overloads”
// 机制来调用安全的替代函数， VC6/VC7中无效会忽略
//	TCHAR szBuf[10];
//	_tcscpy(szBuf, _T("test"));  ==> _tcscpy_s(szBuf, _T("test"));
//
// 注意：
//      仅仅限于操作静态字符数组形式的字符串（char szBuf[10];）
//		动态分配的起不到作用（char *szBuf = (char*)malloc(10);） ------------(2)

#ifdef _CRT_SECURE_CPP_OVERLOAD_SECURE_NAMES
	#undef _CRT_SECURE_CPP_OVERLOAD_SECURE_NAMES
#endif
#define _CRT_SECURE_CPP_OVERLOAD_SECURE_NAMES 1		// 强制使用安全字符串函数
	
//////////////////////////////////////////////////////////////////////////
// 针对上面(1)(2)的情况，就需要改动代码了：
//	TCHAR *szBuf = (TCHAR*)malloc(10);
//	_tcscpy(szBuf, _T("test"));  ==> _tcscpy_s(szBuf, 10*sizeof(TCHAR), _T("test"));
//

// Note: VS2005(_MSC_VER=1400)  VS2003(_MSC_VER=1310) VC6(1200)
#if _MSC_VER < 1400 
//{{{
	
	//****以下模板函数和宏，适用于VS2003 和 VC6，VS2005使用自带的安全库****

	#ifndef errno_t
		typedef int errno_t;
	#endif

	// secure funtion returns 
	#define EINVAL          22
	#define ERANGE          34
	#define ESUCCESS		0

	//////////////////////////////////////////////////////////////////////////
	// general implementation of _tcscpy_s
	//
	//	ReturnValue: ESUCCESS if successful
	//
	//	Error Conditions
	//	strDestination  nCount	strSource  ReturnValue  ContentsOfstrDestination  
	//	NULL			any			any			EINVAL		not modified
	//	any				any			NULL		EINVAL		not modified
	//	any			  0/too small	any			ERANGE		not modified

	template<typename strtype, typename conststrtype>
	inline errno_t __cdecl _tcscpy_s_impl(
		strtype strDestination, int nCount, conststrtype strSource)	
	{	
		errno_t eReturn = ESUCCESS;

		// the resulting string is terminated with a null character("\0"). 
		if (strDestination != NULL && nCount > 0 && strSource != NULL) 
		{
			int nSafeCount = nCount - 1;
			int nSourceCount = _tcslen(strSource);
			if (nSafeCount < nSourceCount) 
			{
				eReturn = ERANGE;
			}
			else
			{
				nSafeCount = nSourceCount;
				_tcsncpy(strDestination, strSource, nSafeCount);
				strDestination[nSafeCount] = _T('\0');
			}
		}
		else
		{
			eReturn = (nCount == 0) ? ERANGE : EINVAL;
		}
		
		return eReturn;
	}

	//////////////////////////////////////////////////////////////////////////
	// general implementation of _tcsncpy_s
	//
	//	ReturnValue: ESUCCESS if successful
	//
	//	Error Conditions
	//	strDestination  nCount	strSource  ReturnValue  ContentsOfstrDestination  
	//	NULL			any			any			EINVAL		not modified
	//	any				any			NULL		EINVAL		not modified
	//	any				0			any			EINVAL		not modified
	//	not NULL		too small	any			EINVAL		strDest[0] set to 0

	template<typename strtype, typename conststrtype>
	inline errno_t __cdecl _tcsncpy_s_impl(
		strtype strDestination, int nCount, conststrtype strSource, int nSourceCount)	
	{		
		errno_t eReturn = ESUCCESS;

		// the resulting string is terminated with a null character("\0"). 
		if (strDestination != NULL && nCount > 0 && strSource != NULL && nSourceCount > 0) 
		{
			int nSafeCount = nCount - 1;
			if (nSafeCount < nSourceCount) 
			{
				eReturn = EINVAL;
				strDestination[0] = _T('\0');
			}
			else
			{
				nSafeCount = nSourceCount;
				_tcsncpy(strDestination, strSource, nSafeCount);
				strDestination[nSafeCount] = _T('\0');
			}
		}
		else
		{
			eReturn = EINVAL;
		}

		return eReturn;
	}

	//////////////////////////////////////////////////////////////////////////
	// general implementation of _tcscat_s
	//
	//	ReturnValue: ESUCCESS if successful
	//
	//	Error Conditions
	//	strDestination		nCount	strSource  ReturnValue  ContentsOfstrDestination  
	//	NULL / unterminated	any		any			EINVAL		not modified
	//	any					any		NULL		EINVAL		not modified
	//	any				0/too small	any			ERANGE		not modified

	template<typename strtype, typename conststrtype>
	inline errno_t __cdecl _tcscat_s_impl(
		strtype strDestination, int nCount, conststrtype strSource)	
	{		
		errno_t eReturn = ESUCCESS;

		// the resulting string is terminated with a null character("\0"). 
		if (strDestination != NULL && nCount > 0 && strSource != NULL) 
		{
			int nSourceCount = _tcslen(strSource);
			int nDestCount = _tcslen(strDestination);
			int nNeedTotalCount = nDestCount + nSourceCount + 1;

			if (nCount < nNeedTotalCount) 
			{
				eReturn = ERANGE;
			}
			else
			{
				int nSafeAppendCount = nSourceCount;
				_tcsncat(strDestination, strSource, nSafeAppendCount);
				strDestination[nNeedTotalCount - 1] = _T('\0');
			}
		}
		else
		{
			eReturn = (nCount == 0) ? ERANGE : EINVAL;
		}

		return eReturn;
	}

	//////////////////////////////////////////////////////////////////////////
	// general implementation of _tcsncat_s
	//
	//	ReturnValue: ESUCCESS if successful
	//
	//	Error Conditions
	//	strDestination		nCount	strSource  ReturnValue  ContentsOfstrDestination  
	//	NULL / unterminated	any		any			EINVAL		not modified
	//	any					any		NULL		EINVAL		not modified
	//	any				0/too small	any			ERANGE		not modified

	template<typename strtype, typename conststrtype>
	inline errno_t __cdecl _tcsncat_s_impl(
		strtype strDestination, int nCount, conststrtype strSource, int nSourceCount)	
	{		
		errno_t eReturn = ESUCCESS;

		// the resulting string is terminated with a null character("\0"). 
		if (strDestination != NULL && nCount > 0 && strSource != NULL && nSourceCount > 0) 
		{			
			int nDestCount = _tcslen(strDestination);
			int nNeedTotalCount = nDestCount + nSourceCount + 1;

			if (nCount < nNeedTotalCount) 
			{
				eReturn = ERANGE;
			}
			else
			{
				int nSafeAppendCount = nSourceCount;
				_tcsncat(strDestination, strSource, nSafeAppendCount);
				strDestination[nNeedTotalCount - 1] = _T('\0');
			}
		}
		else
		{
			eReturn = (nCount == 0) ? ERANGE : EINVAL;
		}

		return eReturn;
	}

	//////////////////////////////////////////////////////////////////////////
	// general implementation of _tcslwr_s
	//
	//	ReturnValue: ESUCCESS if successful
	//
	//	Error Conditions
	//	strDestination	nCount	  ReturnValue  ContentsOfstrDestination  
	//	NULL  			any			EINVAL		not modified
	//	any				0/too small		ERANGE		not modified

	template<typename strtype>
	inline errno_t __cdecl _tcslwr_s_impl(
		strtype strDestination, int nCount)	
	{
		errno_t eReturn = ESUCCESS;

		if (strDestination != NULL && nCount > 0) 
		{
			int nSafeCount = nCount - 1;
			if (nSafeCount < (int)_tcslen(strDestination)) 
			{
				eReturn = ERANGE;
			}
			else
			{
				_tcslwr(strDestination);
			}			
		}
		else
		{
			eReturn = (nCount == 0) ? ERANGE : EINVAL;
		}

		return eReturn;
	}

	//////////////////////////////////////////////////////////////////////////
	// general implementation of _stprintf_s
	//	
	//	ReturnValue: The number of characters written(not counting _T('\0') )
	//
	//	Error Conditions
	//	buffer	nCount	format  ReturnValue  ContentsOfstrDestination  
	//	NULL  	any		any		  -1			not modified
	//	any		any		NULL	  -1			not modified

#define _STPRINTF_S_IMPL()\
	int nWrittenCount = -1;\
	if(buffer != NULL && nCount > 0 && format != NULL)\
	{\
		int nSafeCount = nCount - 1;\
		va_list arglist; \
		va_start(arglist, format); \
\
		nWrittenCount = _vsntprintf(\
			buffer,\
			nSafeCount,\
			format,\
			arglist);\
\
		va_end(arglist);\
\
	buffer[nSafeCount] = _T('\0');\
	}\
	return nWrittenCount;

	// TODO:
	//////////////////////////////////////////////////////////////////////////
	// general implementation of _sntprintf_s
	//	
	//	ReturnValue: The number of characters written(not counting _T('\0') )
	//
	//	Error Conditions
	//	buffer	nCount	format  ReturnValue  ContentsOfstrDestination  
	//	NULL  	any		any		  -1			not modified
	//	any		any		NULL	  -1			not modified

#define _SNTPRINTF_S_IMPL()\
	int nWrittenCount = -1;\
	if(buffer != NULL && nCount > 0 && nMaxCountStore > 0 && format != NULL)\
	{\
		int nSafeCount = (nCount - 1) < nMaxCountStore ? (nCount - 1) : nMaxCountStore;\
		va_list arglist; \
		va_start(arglist, format); \
\
		nWrittenCount = _vsntprintf(\
			buffer,\
			nSafeCount,\
			format,\
			arglist);\
\
		va_end(arglist);\
\
		buffer[nSafeCount] = _T('\0');\
	}\
	return nWrittenCount;

	// TODO:
	//////////////////////////////////////////////////////////////////////////
	// general implementation of _vsntprintf_s
	//	
	//	ReturnValue: The number of characters written(not counting _T('\0') )
	//
	//	Error Conditions
	//	buffer	nCount	format  ReturnValue  ContentsOfstrDestination  
	//	NULL  	any		any		  -1			not modified
	//	any		any		NULL	  -1			not modified

	template<typename strtype, typename conststrtype>
	inline int __cdecl _vsntprintf_s_impl(
		strtype buffer, int nCount, int nMaxCountStore, conststrtype format,va_list argptr)
	{
		int nWrittenCount = -1;

		if (buffer != NULL && nCount > 0 && nMaxCountStore > 0 && format != NULL && argptr != NULL) 
		{
			int nSafeCount = (nCount - 1) < nMaxCountStore ? (nCount - 1) : nMaxCountStore;

			nWrittenCount = _vsntprintf(buffer, nSafeCount, format, argptr);

			buffer[nSafeCount] = _T('\0');
		}

		return nWrittenCount;
	}

	//////////////////////////////////////////////////////////////////////////
	// TODO ： 可以根据需要，随时增加新的字符串的通用编译定义宏



	//////////////////////////////////////////////////////////////////////////
	#ifdef _UNICODE

		__declspec(selectany) 
			errno_t (*wcscpy_s)(wchar_t*, int, const wchar_t*) = _tcscpy_s_impl;

		__declspec(selectany) 
			errno_t (*wcsncpy_s)(wchar_t*, int, const wchar_t*, int) = _tcsncpy_s_impl;

		__declspec(selectany) 
			errno_t (*wcscat_s)(wchar_t*, int, const wchar_t*) = _tcscat_s_impl;

		__declspec(selectany) 
			errno_t (*wcsncat_s)(wchar_t*, int, const wchar_t*, int) = _tcsncat_s_impl;

		__declspec(selectany) 
			errno_t (*wcslwr_s)(wchar_t*, int) = _tcslwr_s_impl;

		inline	int __cdecl swprintf_s(
			wchar_t* buffer, int nCount,const wchar_t* format, ...) 
		{
			// the resulting string is terminated with a null character("\0"). 
			_STPRINTF_S_IMPL();
		}

		inline	int __cdecl swnprintf_s(
			wchar_t* buffer, int nCount, int nMaxCountStore, const wchar_t* format, ...) 
		{
			// the resulting string is terminated with a null character("\0"). 
			_SNTPRINTF_S_IMPL();
		}

		__declspec(selectany) 
			int (*_vswnprintf_s)(wchar_t*, int, int, const wchar_t*, va_list) = _vsntprintf_s_impl;


		#define _tcscpy_s wcscpy_s
		#define _tcsncpy_s wcsncpy_s
		#define _tcscat_s wcscat_s
		#define _tcsncat_s wcsncat_s
		#define _tcslwr_s wcslwr_s
		#define _stprintf_s swprintf_s
		#define _sntprintf_s swnprintf_s
		#define _vsntprintf_s _vswnprintf_s
		
	#else //  _MBCS
	
		__declspec(selectany) 
			errno_t (*strcpy_s)(char*, int, const char*) = _tcscpy_s_impl;

		__declspec(selectany) 
			errno_t (*strncpy_s)(char*, int, const char*, int) = _tcsncpy_s_impl;

		__declspec(selectany) 
			errno_t (*strcat_s)(char*, int, const char*) = _tcscat_s_impl;

		__declspec(selectany) 
			errno_t (*strncat_s)(char*, int, const char*, int) = _tcsncat_s_impl;

		__declspec(selectany) 
			errno_t (*strlwr_s)(char*, int) = _tcslwr_s_impl;

		inline	int __cdecl sprintf_s(
			char* buffer, int nCount,const char* format, ...) 
		{
			// the resulting string is terminated with a null character("\0"). 
			_STPRINTF_S_IMPL();
		}

		inline	int __cdecl snprintf_s(
			char* buffer, int nCount, int nMaxCountStore, const char* format, ...) 
		{
			// the resulting string is terminated with a null character("\0"). 
			_SNTPRINTF_S_IMPL();
		}

		__declspec(selectany) 
			int (*_vsnprintf_s)(char*, int, int, const char*, va_list) = _vsntprintf_s_impl;


		#define _tcscpy_s strcpy_s
		#define _tcsncpy_s strncpy_s
		#define _tcscat_s strcat_s
		#define _tcsncat_s  strncat_s
		#define _tcslwr_s strlwr_s
		#define _stprintf_s sprintf_s
		#define _sntprintf_s snprintf_s
		#define _vsntprintf_s _vsnprintf_s
		
	#endif // _UNICODE ,  _MBCS

	// 针对静态字符数组形式，获取元素个数的方法，动态的不支持
	// 比如：	TCHAR szArr[20] 
	//			sizeof(szArr) = 40 bytes
	//			_countof(szArr) = 20 elements
	#define _countof(_Array) (sizeof(_Array) / sizeof(_Array[0]))

//}}}
#endif // _MSC_VER < 1400

	#define _tcscount(bytesize) bytesize/sizeof(TCHAR)

	#define _tcssize(count) count*sizeof(TCHAR)


}//extern "C++"
#endif // __cplusplus

};// Tenio

//////////////////////////////////////////////////////////////////////////
// 使用范例：
//
//	TCHAR szBuffer[100] = {0};
//
//	_tcsncpy_s(szBuffer, _countof(szBuffer), _T("mice"), 4);
//
//	_tcsncat_s(szBuffer, _countof(szBuffer), _T("mice"), 4);
//
//	_sntprintf_s(szBuffer, _countof(szBuffer), _countof(szBuffer) - 1,
//				 _T("name=%s%d"), _("Tenio"), 1400);
//
//
// 
//////////////////////////////////////////////////////////////////////////
// 字符串编程注意事项：
// 1、清楚输入源的大小和长度，输出目的的大小和长度
// 2、_tcslen 是不含字符串结束符的！！！
// 

