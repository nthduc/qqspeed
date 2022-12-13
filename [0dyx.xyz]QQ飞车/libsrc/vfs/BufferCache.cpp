
#include "BufferCache.h"

NAMESPACEBEGIN

#if !defined(VFS_PORTABLE)
	DEFINE_FACTORY_METHOD(CBufferCache);
#endif


static ILogSys* s_pLogSys = NULL;

#define ERRORLOG(msg) if (s_pLogSys) s_pLogSys->printf(_T("%s(%d) : error %s\n"),__FILE__,__LINE__,msg);
#define ERRORLOG1(msg,p1) if (s_pLogSys) s_pLogSys->printf(_T("%s(%d) : error "##msg"\n"),__FILE__,__LINE__,p1);
#define ERRORLOG2(msg,p1,p2) if (s_pLogSys) s_pLogSys->printf(_T("%s(%d) : error "##msg"\n"),__FILE__,__LINE__,p1,p2);
#define ERRORLOG3(msg,p1,p2,p3) if (s_pLogSys) s_pLogSys->printf(_T("%s(%d) : error "##msg"\n"),__FILE__,__LINE__,p1,p2,p3);


CBufferCache::CBufferCache()
{
	m_pHandleAlloc = NULL;
	m_pFileMapper = NULL;
	m_nCacheCurSize = m_nCacheMaxSize = 0;
	m_hSelfHandle = VFS_INVALID_HANDLE_VALUE;
	m_bInit = false;
	m_lMutex = 0;
}

CBufferCache::~CBufferCache()
{
	Clear();
}
    
int CBufferCache::Init(IHandleAlloc* phandlealloc, IFileMapper* pfilemapper, int cachesize)
{
	if (m_bInit) return 0;
	if (phandlealloc==NULL || pfilemapper==NULL) return 0;
	m_pHandleAlloc = phandlealloc;
	m_pFileMapper = pfilemapper;
	m_nCacheMaxSize = (cachesize>=0)?cachesize:0;
	m_nCacheCurSize = 0;
	m_hSelfHandle = m_pHandleAlloc->AllocHandle();
	m_bInit=true;
	return 1;
}

void CBufferCache::Release()
{
#if defined(VFS_PORTABLE)
	delete this;
#else
	Free();
#endif
}

int CBufferCache::SetCacheSize(int cachesize)
{
	simple_mutex mutex(&m_lMutex);

	if (!m_bInit) return 0;
	m_nCacheMaxSize = (cachesize>=0)?cachesize:0;
	return AdjustCacheBySizeLimit();
}

int CBufferCache::AdjustCacheBySizeLimit()
{
	if (m_nCacheCurSize<=m_nCacheMaxSize) return 0;

	int releasecount=0;

	for (LRULISTITER iter=m_lstLRU.begin(); iter!=m_lstLRU.end(); )
	{
		CACHE_h hcache = (*iter);

		MAPCACHE2CACHEDATAITER iter2 = m_mapCache2CacheData.find(hcache);

		if (iter2!=m_mapCache2CacheData.end())
		{
			cachedata_t* pcache = iter2->second;
			if (pcache->refcount>0) { iter++; continue; }
			VROOT_h hroot = pcache->hroot;
			long lparam = pcache->lparam;
			IBuffer* pbuffer = pcache->pbuffer;
			m_pFileMapper->MapFile(pcache->vpath.c_str(),hroot,lparam);
			m_nCacheCurSize-=pbuffer->GetUsedSize();
			if (m_nCacheCurSize<0) m_nCacheCurSize=0;
			pbuffer->Release();
			delete iter2->second;
			m_mapCache2CacheData.erase(iter2);
			releasecount++;
			iter = m_lstLRU.erase(iter);
			if (m_nCacheCurSize<=m_nCacheMaxSize) break;
		}
		else
		{
			iter = m_lstLRU.erase(iter);
		}
	}

	return releasecount;
}

int CBufferCache::ActiveCache(CACHE_h hcache)
{
	for (LRULISTITER iter=m_lstLRU.begin(); iter!=m_lstLRU.end(); iter++)
	{
		if (*iter==hcache)
		{
			m_lstLRU.erase(iter);
			m_lstLRU.push_back(hcache);
			return 1;
		}
	}
	return 0;
}

CACHE_h CBufferCache::AddCache(const TCHAR* vpath, VROOT_h hroot, long lparam, IBuffer* pbuffer)
{
	simple_mutex mutex(&m_lMutex);

	if (vpath==NULL || pbuffer==NULL || hroot==VFS_INVALID_HANDLE_VALUE)
	{
		ERRORLOG(_T("param error!"));
		return VFS_INVALID_HANDLE_VALUE;
	}

	if (!m_bInit) return 0;

	int buffersize = pbuffer->GetUsedSize();

	if (hroot==m_hSelfHandle)
	{	//already in cache
		ActiveCache(lparam);
		return VFS_INVALID_HANDLE_VALUE;
	}

	cachedata_t* pcachedata = new cachedata_t(vpath,hroot,lparam,pbuffer);
	if (pcachedata==NULL)
	{
		return VFS_INVALID_HANDLE_VALUE;
	}

	lparam = m_pHandleAlloc->AllocHandle();

	if (0==m_pFileMapper->MapFile(vpath,m_hSelfHandle,lparam))
	{
		m_pHandleAlloc->FreeHandle(lparam);
		delete pcachedata;
		return VFS_INVALID_HANDLE_VALUE;
	}

	m_lstLRU.push_back(lparam);

	m_mapCache2CacheData[lparam] = pcachedata;

	m_nCacheCurSize+=buffersize;

	AdjustCacheBySizeLimit();

	return lparam;	
}

int CBufferCache::Clear()
{
	simple_mutex mutex(&m_lMutex);

	MAPCACHE2CACHEDATAITER iter;
	for (iter=m_mapCache2CacheData.begin(); iter!=m_mapCache2CacheData.end(); ++iter)
	{
		VROOT_h hroot = iter->second->hroot;
		long lparam = iter->second->lparam;
		IBuffer* pbuffer = iter->second->pbuffer;
		m_pFileMapper->MapFile(iter->second->vpath.c_str(),hroot,lparam);
		pbuffer->Release();
		delete iter->second;
	}
	m_mapCache2CacheData.clear();
	m_lstLRU.clear();
	m_nCacheCurSize=0;
	return 1;
}

IBuffer* CBufferCache::GetBuffer(CACHE_h hcache)
{
	simple_mutex mutex(&m_lMutex);

	MAPCACHE2CACHEDATAITER iter;
	iter = m_mapCache2CacheData.find(hcache);
	if (iter!=m_mapCache2CacheData.end())
	{
		ActiveCache(hcache);
		//iter->second->refcount++;
		return iter->second->pbuffer;
	}
	return NULL;
}

int CBufferCache::IncRefCount(CACHE_h hcache)
{
	simple_mutex mutex(&m_lMutex);

	MAPCACHE2CACHEDATAITER iter;
	iter = m_mapCache2CacheData.find(hcache);
	if (iter!=m_mapCache2CacheData.end())
	{
		iter->second->refcount++;
		return iter->second->refcount;
	}
	return 0;
}

int CBufferCache::DecRefCount(CACHE_h hcache)
{
	simple_mutex mutex(&m_lMutex);

	MAPCACHE2CACHEDATAITER iter;
	iter = m_mapCache2CacheData.find(hcache);
	if (iter!=m_mapCache2CacheData.end())
	{
		iter->second->refcount--;
		if (iter->second->refcount<0) iter->second->refcount=0;
		int refc = iter->second->refcount;
		AdjustCacheBySizeLimit();
		return refc;
	}
	return 0;
}


IBufferCache* ICreateBufferCache(IHandleAlloc* phandlealloc, IFileMapper* pfilemapper, int cachesize, ILogSys* pLogSys)
{
	s_pLogSys = pLogSys;

    IBufferCache* pcache = static_cast<IBufferCache*>(new CBufferCache());
	if (pcache==NULL)
	{
		ERRORLOG(_T("buffercache interface create error"));
		return NULL;
	}

    if (0==pcache->Init(phandlealloc,pfilemapper,cachesize))
    {
        pcache->Release();
		ERRORLOG(_T("buffercache interface init failed"));
        return NULL;
    }
    return pcache;    
}


NAMESPACEEND