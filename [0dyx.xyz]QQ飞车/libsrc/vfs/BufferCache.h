
#pragma once

#pragma warning(disable : 4786)

#if !defined(VFS_PORTABLE)
#  include "teniodll.h"
#  define IBUFFERCACHE TComponentImpl<IBufferCache, CBufferCache, COMPONENTID_VFS_BUFFERCACHE, 1>
#  define RELEASEFUNC_ADDON void ComponentFinalize() {}
#else
#  define IBUFFERCACHE IBufferCache
#  define RELEASEFUNC_ADDON
#endif

#  include "VFS.h"


NAMESPACEBEGIN


struct cachedata_t
{
	cachedata_t(const TCHAR* _vpath, VROOT_h _hroot, long _lparam, IBuffer* _pbuffer):
		vpath(_vpath), hroot(_hroot), lparam(_lparam), pbuffer(_pbuffer), refcount(1) {}
	stl_string vpath;
	VROOT_h  hroot;
	long     lparam;
	IBuffer* pbuffer;
	int      refcount;
};

    
class CBufferCache : public IBUFFERCACHE
{
	typedef stl_map<CACHE_h,cachedata_t*> MAPCACHE2CACHEDATA;
	typedef MAPCACHE2CACHEDATA::iterator MAPCACHE2CACHEDATAITER;
	typedef stl_list<CACHE_h> LRULIST;
	typedef LRULIST::iterator LRULISTITER;

public:
	CBufferCache();
	~CBufferCache();

	RELEASEFUNC_ADDON

    virtual int Init(IHandleAlloc* phandlealloc, IFileMapper* pfilemapper, int cachesize);
    virtual void Release();
	virtual int Clear();
	virtual int SetCacheSize(int cachesize);
	virtual CACHE_h AddCache(const TCHAR* vpath, VROOT_h hroot, long lparam, IBuffer* pbuffer);
	virtual IBuffer* GetBuffer(CACHE_h hcache);
	virtual int IncRefCount(CACHE_h hcache);
	virtual int DecRefCount(CACHE_h hcache);
	virtual CACHEMGR_h GetHandle() {return m_hSelfHandle;}

protected:
	
	int AdjustCacheBySizeLimit();
	int ActiveCache(CACHE_h hcache);
			
protected:
	CACHEMGR_h m_hSelfHandle;
	int m_nCacheMaxSize;
	int m_nCacheCurSize;
	LRULIST m_lstLRU;
	MAPCACHE2CACHEDATA m_mapCache2CacheData;
	IHandleAlloc* m_pHandleAlloc;
	IFileMapper*  m_pFileMapper;
	bool m_bInit;
	long m_lMutex;
};



NAMESPACEEND