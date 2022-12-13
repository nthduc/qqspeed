#pragma once

#include "limits.h"
#include "stdio.h"
#pragma  warning(disable:4218)
#pragma  warning(disable:4267)
#pragma  warning(disable:4018)

template<class T>
inline UINT typelen(T* Data, int MaxLen = UINT_MAX)
{
	UINT i = 0;
	for ( ; i < MaxLen; i++ )
	{
		if ( 0 == Data[i] )
		{
			break;
		}
	}
	return i;
}

inline UINT byteslen(const BYTE *byBuffer, UINT MaxLen = UINT_MAX)
{
	return typelen(byBuffer, MaxLen);
}

inline int trim_space( char* str )
{
	int left, right;
	int len = strlen(str);

	for (left=0; left<len; left++)
		if (!(str[left]==' ' || str[left]=='\t' || str[left]=='\n' || str[left]=='\r')) break;

	for (right=len-1; right>left; right--)
		if (!(str[right]==' ' || str[right]=='\t' || str[right]=='\n' || str[right]=='\r')) break;

	len = right-left+1;

	if (len<0) len=0;

	if (len>0) strncpy(str,str+left,len);

	str[len]=0;

	return len;
}

inline void LogText(const char *fmt, ...)
{
	va_list va;
	char szText[1024];
	va_start(va, fmt);
	vsprintf(szText, fmt, va);
	va_end(va);
	OutputDebugString(szText);
}

inline void XTrace(const char *fmt, ...)
{
	va_list va;
	char szText[1024];
	va_start(va, fmt);
	vsprintf(szText, fmt, va);
	va_end(va);
	OutputDebugString("-(trace)");
	OutputDebugString(szText);
	OutputDebugString("\n");
}

inline void XWaring(const char *fmt, ...)
{
	va_list va;
	char szText[1024];
	va_start(va, fmt);
	vsprintf(szText, fmt, va);
	va_end(va);
	OutputDebugString("!(waring)");
	OutputDebugString(szText);
	OutputDebugString("\n");
}

inline void XError(const char *fmt, ...)
{
	va_list va;
	char szText[1024];
	va_start(va, fmt);
	vsprintf(szText, fmt, va);
	va_end(va);
	OutputDebugString("!!(error)");
	OutputDebugString(szText);
	OutputDebugString("\n");
	MessageBox(NULL, szText, "´íÎó", MB_OK | MB_ICONSTOP);
}

inline UINT MyGetTime()
{
	return GetTickCount();
}

#include "string"
using namespace std;
inline string FormatErrorMessage(DWORD dwError)
{
	LPVOID lpMsgBuf;
	FormatMessage( 
		FORMAT_MESSAGE_ALLOCATE_BUFFER | 
		FORMAT_MESSAGE_FROM_SYSTEM | 
		FORMAT_MESSAGE_IGNORE_INSERTS,
		NULL,
		dwError,
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), // Default language
		(LPTSTR) &lpMsgBuf,
		0,
		NULL 
		);
	// Process any inserts in lpMsgBuf.
	// ...
	// Display the string.
	//MessageBox( NULL, (LPCTSTR)lpMsgBuf, "Error", MB_OK | MB_ICONINFORMATION );
	// Free the buffer.
	string strMessage = (LPCSTR)lpMsgBuf;
	LocalFree( lpMsgBuf );
	return strMessage;
}

#define CALL(p, member)	if (p) { p->member; } 
#define CALLEX(p, member, ret) if ( p ) { ret = p->member; }

#include "assert.h"
#include "crtdbg.h"

#if defined(DEBUG) || defined(_DEBUG)

#ifndef ASSERT
#define ASSERT(hr)			_ASSERTE(hr)
#endif

#ifdef assert
#undef assert
#endif
#define assert(hr)	_ASSERTE(hr)

#else

#define ASSERT(x)
#define assert(x)

#endif //defined(DEBUG) || defined(_DEBUG)

#define KK_CHECK(hr)		if ( (hr) != S_OK )	{ assert(0); goto KK_EXIT; }
#define KK_CHECK_BOOL(b)	if ( (b) == FALSE ) { assert(0); goto KK_EXIT; }
#define KK_CHECK_VBOOL(b)	if ( (b) == VARIANT_FALSE ) { assert(0); goto KK_EXIT; }
#define KK_CHECK_POINTER(p)	if ( (p) == NULL ) { assert(0); goto KK_EXIT; }
#define RELEASE(p)			if ( p ) { (p)->Release(); p = NULL; }

#define KK_CHECKEX(hr, expr)		if ( (hr) != S_OK )	{ assert(0); expr; goto KK_EXIT; }
#define KK_CHECK_BOOLEX(b, expr)	if ( (b) == FALSE ) { assert(0); expr; goto KK_EXIT; }
#define KK_CHECK_VBOOLEX(b, expr)	if ( (b) == VARIANT_FALSE ) { assert(0); expr; goto KK_EXIT; }
#define KK_CHECK_POINTEREX(p, expr)	if ( (p) == NULL ) { assert(0); expr; goto KK_EXIT; }
#define RELEASE(p)			if ( p ) { (p)->Release(); p = NULL; }


