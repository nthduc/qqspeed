#pragma once
//////////////////////////////////////////////////////////////////////////
//		��ͳ��CRT���ַ���������������ȫ����ϵ��̫�ͣ�һֱΪ��ڸ����
//	VC8Ҳ�ǷѾ���˼���������·�װ���ַ���������������function_s��
//	��������ز����Ϸ��ͺ����Եļ�飬���Ҳ����ˡ�Secure template overloads��
//	��Ҳ���ǻ����ַ��������С��ģ�庯����������ʵ�ּ����ϵĴ���
//	�������ƣ���֧�ֶ�̬����ģ�����̬����Ļ���Ҫ�޸Ĵ������ʹ�ð�ȫ������
//		��ӦVC8�ĳ����ɣ��������ǵ��ַ�����������������function_s��ȫ���
//	�汾����غ���ԭ����ο�VC8��MSDN˵����Ϊ��VC6/7�����Ա���ͨ����д��һ
//	����include/safestring.h���ı���֧�ֺ궨�壬�ڵͰ汾����ת����ͳ��CRT�ַ���������
//
//		��ص�safestring.h ���������£�������������ʷ�ֻ�����˳��õģ�
//	���µĿ��Բ�����ӡ�������
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
// ȥ��VC8�������е�����ϵĲ���ȫ���ַ������������ľ��棬 VC6/VC7����Ч�����

//#ifndef _CRT_SECURE_NO_DEPRECATE
//	#define _CRT_SECURE_NO_DEPRECATE 
//#endif
//
//#ifndef _CRT_SECURE_NO_WARNINGS
//	#define _CRT_SECURE_NO_WARNINGS 
//#endif


//////////////////////////////////////////////////////////////////////////
// ͨ������ĺ꿪�أ������޸��ϵĴ��룬VC8�л�ͨ����Secure Template Overloads������
// �����ð�ȫ����������� VC6/VC7����Ч�����
// ע�⣺
//      �������ڲ�����̬�ַ�������ʽ���ַ�����char szBuf[10];��
//		��̬������𲻵����ã�char *szBuf = (char*)malloc(10);�� ------------(1)

#ifdef _CRT_SECURE_CPP_OVERLOAD_STANDARD_NAMES
	#undef _CRT_SECURE_CPP_OVERLOAD_STANDARD_NAMES
#endif
#define _CRT_SECURE_CPP_OVERLOAD_STANDARD_NAMES 1		// ���ڼ��ݾɴ�������ͨ�ַ�������

#ifdef _CRT_SECURE_CPP_OVERLOAD_STANDARD_NAMES_COUNT 
	#undef _CRT_SECURE_CPP_OVERLOAD_STANDARD_NAMES_COUNT 
#endif
#define _CRT_SECURE_CPP_OVERLOAD_STANDARD_NAMES_COUNT 1 // ���ڼ��ݾɴ����д�"n"���ַ�������

//////////////////////////////////////////////////////////////////////////
// ͨ������ĺ꿪�أ�����Ҫ�޸ĺ�����������"_s"��׺��ͬ��VC8�л�ͨ����Secure Template Overloads��
// ���������ð�ȫ����������� VC6/VC7����Ч�����
//	TCHAR szBuf[10];
//	_tcscpy(szBuf, _T("test"));  ==> _tcscpy_s(szBuf, _T("test"));
//
// ע�⣺
//      �������ڲ�����̬�ַ�������ʽ���ַ�����char szBuf[10];��
//		��̬������𲻵����ã�char *szBuf = (char*)malloc(10);�� ------------(2)

#ifdef _CRT_SECURE_CPP_OVERLOAD_SECURE_NAMES
	#undef _CRT_SECURE_CPP_OVERLOAD_SECURE_NAMES
#endif
#define _CRT_SECURE_CPP_OVERLOAD_SECURE_NAMES 1		// ǿ��ʹ�ð�ȫ�ַ�������
	
//////////////////////////////////////////////////////////////////////////
// �������(1)(2)�����������Ҫ�Ķ������ˣ�
//	TCHAR *szBuf = (TCHAR*)malloc(10);
//	_tcscpy(szBuf, _T("test"));  ==> _tcscpy_s(szBuf, 10*sizeof(TCHAR), _T("test"));
//

// Note: VS2005(_MSC_VER=1400)  VS2003(_MSC_VER=1310) VC6(1200)
#if _MSC_VER < 1400 
//{{{
	
	//****����ģ�庯���ͺ꣬������VS2003 �� VC6��VS2005ʹ���Դ��İ�ȫ��****

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
	// TODO �� ���Ը�����Ҫ����ʱ�����µ��ַ�����ͨ�ñ��붨���



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

	// ��Ծ�̬�ַ�������ʽ����ȡԪ�ظ����ķ�������̬�Ĳ�֧��
	// ���磺	TCHAR szArr[20] 
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
// ʹ�÷�����
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
// �ַ������ע�����
// 1���������Դ�Ĵ�С�ͳ��ȣ����Ŀ�ĵĴ�С�ͳ���
// 2��_tcslen �ǲ����ַ����������ģ�����
// 

