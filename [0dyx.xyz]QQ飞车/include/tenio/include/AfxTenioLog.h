// AfxTenio.h: interface for the AfxTenio class.
//			author : robinyang
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_AFXTENIO_H__620EA01A_9D44_465B_95F8_57E4DCF636E0__INCLUDED_)
#define AFX_AFXTENIO_H__620EA01A_9D44_465B_95F8_57E4DCF636E0__INCLUDED_

#ifdef _AFX_TENIO_LOG_

#pragma warning(disable:4786)

#include "wtypes.h"
#include "windows.h"
#include "tchar.h"

#include "../Template/tenio_t.h"
#include "../Template/teniopointer_t.h"
#include "DebugTools/Logobject_i.h"

using namespace Tenio;

static TTenioPtr<ILogObject, FALSE> logObject;	//默认Tenio.ini在同目录下, 该对象是Singleton的全局唯一实例， 进程内唯一

inline void __cdecl TenioTrace(DWORD dwComponentID, DWORD dwInstanceID, LPCTSTR lpszFormat, ...)
{
	if(logObject != NULL)
	{
		va_list argList;
		va_start(argList, lpszFormat);

		logObject->TraceStringV(1, dwComponentID, dwInstanceID, lpszFormat, argList);

		va_end( argList );
	}

}

inline void __cdecl TenioDump(DWORD dwComponentID, DWORD dwInstanceID, struct _EXCEPTION_POINTERS *pExceptionInfo)
{
	if(logObject != NULL)
	{
		logObject->Dump(dwComponentID, dwInstanceID, pExceptionInfo);
	}
}

inline void __cdecl TenioMeomory(DWORD dwComponentID, DWORD dwInstanceID, char* pBuf, DWORD dwSize, LPCTSTR lpszDesription)
{
	if(logObject != NULL)
	{
		logObject->TraceMemory(dwComponentID, dwInstanceID, pBuf, dwSize, lpszDesription);
	}
}

inline void __cdecl TenioAssert(DWORD dwComponentID, DWORD dwInstanceID, LPCSTR lpszFileName, int nLine)
{
	if(logObject != NULL)
	{
		// 99 is alert level
		logObject->TraceString(99, dwComponentID, dwInstanceID, _T("(Assert)  File : %s; Line : %d"), lpszFileName, nLine);
	}
}

#define AfxTenioTrace						TenioTrace
#define AfxTenioDump						TenioDump
#define AfxTenioMemory						TenioMeomory
#define AfxTenioAssert						TenioAssert

// for tenio component obj 
#define TENTRACE0(sz)						TenioTrace(GetComponentID(), (DWORD)this, _T("%s"), sz);
#define TENTRACE1(sz, p1)					TenioTrace(GetComponentID(), (DWORD)this, sz, p1);
#define TENTRACE2(sz, p1, p2)				TenioTrace(GetComponentID(), (DWORD)this, sz, p1, p2);
#define TENTRACE3(sz, p1, p2, p3)			TenioTrace(GetComponentID(), (DWORD)this, sz, p1, p2, p3);

#define TENDUMP(e)							TenioDump(GetComponentID(), (DWORD)this, e);
#define TENMEMORY(p, n, sz)					TenioMeomory(GetComponentID(), (DWORD)this, p, n, sz);

#define TENASSERT(obj)						if(!obj) TenioAssert(GetComponentID(), (DWORD)this, __FILE__, __LINE__);

// all user common
#define TENIO_TRACE0(sz)					TenioTrace(0, (DWORD)this, _T("%s"), sz);
#define TENIO_TRACE1(sz, p1)				TenioTrace(0, (DWORD)this, sz, p1);
#define TENIO_TRACE2(sz, p1, p2)			TenioTrace(0, (DWORD)this, sz, p1, p2);
#define TENIO_TRACE3(sz, p1, p2, p3)		TenioTrace(0, (DWORD)this, sz, p1, p2, p3);

#define TENIO_DUMP(e)						TenioDump(0, (DWORD)this, e);
#define TENIO_MEMORY(p, n, sz)				TenioMeomory(0, (DWORD)this, p, n, sz);

#define TENIO_ASSERT(obj)					if(!obj) TenioAssert(0, (DWORD)this, __FILE__, __LINE__);

#else   // _AFX_TENIO_LOG_

inline void __cdecl TenioTrace(DWORD dwComponentID, DWORD dwInstanceID, LPCTSTR lpszFormat, ...){};
inline void __cdecl TenioDump(DWORD dwComponentID, DWORD dwInstanceID, struct _EXCEPTION_POINTERS *pExceptionInfo = NULL){};
inline void __cdecl TenioMeomory(DWORD dwComponentID, DWORD dwInstanceID, char* pBuf, DWORD dwSize, LPCTSTR lpszDesription){};
inline void __cdecl TenioAssert(DWORD dwComponentID, DWORD dwInstanceID, LPCSTR lpszFileName, int nLine){};

#define AfxTenioTrace						1 ? (void)0 : TenioTrace
#define AfxTenioDump						1 ? (void)0 : TenioDump
#define AfxTenioMemory						1 ? (void)0 : TenioMeomory
#define AfxTenioAssert						1 ? (void)0 : TenioAssert

// for tenio component obj 
#define TENTRACE0(sz)
#define TENTRACE1(sz, p1)
#define TENTRACE2(sz, p1, p2)
#define TENTRACE3(sz, p1, p2, p3)

#define TENDUMP(e)
#define TENMEMORY(p, n, sz)

#define TENASSERT(obj)

// all user common
#define TENIO_TRACE0(sz)
#define TENIO_TRACE1(sz, p1)
#define TENIO_TRACE2(sz, p1, p2)
#define TENIO_TRACE3(sz, p1, p2, p3)

#define TENIO_DUMP(e)
#define TENIO_MEMORY(p, n, sz)

#define TENIO_ASSERT(obj)

#endif // _AFX_TENIO_LOG_

#endif // !defined(AFX_AFXTENIO_H__620EA01A_9D44_465B_95F8_57E4DCF636E0__INCLUDED_)
