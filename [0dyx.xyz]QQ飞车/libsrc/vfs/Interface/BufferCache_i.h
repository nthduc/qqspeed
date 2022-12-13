

#pragma once

NAMESPACEBEGIN


class IBufferCache PUBLICBASECLASS
{
public:
    virtual int Init(IHandleAlloc* phandlealloc, IFileMapper* pfilemapper, int cachesize)=0;
    virtual void Release()=0;
	virtual int Clear()=0;
	virtual int SetCacheSize(int cachesize)=0;
	virtual CACHE_h AddCache(const TCHAR* vpath, VROOT_h hroot, long lparam, IBuffer* pbuffer)=0;
	virtual IBuffer* GetBuffer(CACHE_h hcache)=0;
	virtual int IncRefCount(CACHE_h hcache)=0;
	virtual int DecRefCount(CACHE_h hcache)=0;
	virtual CACHEMGR_h GetHandle()=0;
};

NAMESPACEEND
