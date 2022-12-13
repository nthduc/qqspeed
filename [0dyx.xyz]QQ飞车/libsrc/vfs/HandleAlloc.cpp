

#include "HandleAlloc.h"


NAMESPACEBEGIN

#if !defined(VFS_PORTABLE)
	DEFINE_FACTORY_METHOD(CHandleAlloc);
#endif


static ILogSys* s_pLogSys = NULL;

#define ERRORLOG(msg) if (s_pLogSys) s_pLogSys->printf(_T("%s(%d) : error %s\n"),__FILE__,__LINE__,msg);
#define ERRORLOG1(msg,p1) if (s_pLogSys) s_pLogSys->printf(_T("%s(%d) : error "##msg"\n"),__FILE__,__LINE__,p1);
#define ERRORLOG2(msg,p1,p2) if (s_pLogSys) s_pLogSys->printf(_T("%s(%d) : error "##msg"\n"),__FILE__,__LINE__,p1,p2);
#define ERRORLOG3(msg,p1,p2,p3) if (s_pLogSys) s_pLogSys->printf(_T("%s(%d) : error "##msg"\n"),__FILE__,__LINE__,p1,p2,p3);


CHandleAlloc::CHandleAlloc()
{
	m_hLastHandle=VFS_HANDLE_VALUE_START;
	m_lMutex = 0;
}

CHandleAlloc::~CHandleAlloc()
{
}

void CHandleAlloc::Reset()
{
	simple_mutex mutex(&m_lMutex);

	m_vecFreeHandle.clear();
	m_hLastHandle=VFS_HANDLE_VALUE_START;
}

unsigned int CHandleAlloc::AllocHandle()
{
	simple_mutex mutex(&m_lMutex);

	unsigned int handle;
	if (m_vecFreeHandle.size())
	{
		HANDLESETITER iter = m_vecFreeHandle.end()-1;
		handle = *iter;
		m_vecFreeHandle.pop_back();
	}
	else
	{
		handle = m_hLastHandle++;
	}
	return handle;
}

void CHandleAlloc::FreeHandle(unsigned int handle)
{
	simple_mutex mutex(&m_lMutex);

	if (handle==VFS_INVALID_HANDLE_VALUE)
	{
		ERRORLOG(_T("param error!"));
		return ;
	}

	if (handle==m_hLastHandle-1)
	{
		m_hLastHandle--;
	}
	else
	{
		m_vecFreeHandle.push_back(handle);
	}
}


void CHandleAlloc::Release()
{
	simple_mutex mutex(&m_lMutex);

#if defined(VFS_PORTABLE)
	delete this;
#else
	Free();
#endif
}

IHandleAlloc* ICreateHandleAlloc(ILogSys* pLogSys)
{
	s_pLogSys = pLogSys;

	IHandleAlloc* pHandle = static_cast<IHandleAlloc*>(new CHandleAlloc());
	if (pHandle==NULL)
	{
		ERRORLOG(_T("handle alloc interface create error!"));
		return NULL;
	}

	return pHandle;
}


NAMESPACEEND