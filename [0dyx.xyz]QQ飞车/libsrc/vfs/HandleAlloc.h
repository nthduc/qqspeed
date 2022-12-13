
#pragma once

#pragma warning(disable : 4786)

#if !defined(VFS_PORTABLE)
#  include "teniodll.h"
#  define IHANDLEALLOC TComponentImpl<IHandleAlloc, CHandleAlloc, COMPONENTID_VFS_HANDLEALLOC, 1>
#  define RELEASEFUNC_ADDON void ComponentFinalize() {}
#else
#  define IHANDLEALLOC IHandleAlloc
#  define RELEASEFUNC_ADDON
#endif

#  include "VFS.h"

NAMESPACEBEGIN

    
class CHandleAlloc : public IHANDLEALLOC
{
	typedef stl_vector<unsigned int> HANDLESET;
	typedef HANDLESET::iterator HANDLESETITER;

public:
	CHandleAlloc();
	~CHandleAlloc();
	
	RELEASEFUNC_ADDON

	virtual void Reset();
	virtual void Release();

    virtual unsigned int AllocHandle();
    virtual void FreeHandle(unsigned int handle);

protected:
	HANDLESET m_vecFreeHandle;
	unsigned int m_hLastHandle;
	long      m_lMutex;
};


NAMESPACEEND