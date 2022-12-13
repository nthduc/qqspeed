
#pragma once

NAMESPACEBEGIN


class IFileSys PUBLICBASECLASS
{
public:
    virtual int Init(IFileSysMgr* filesysmgr)=0;
    virtual void Release()=0;

	virtual void SetCaseSense(bool enable)=0;
	virtual bool IsCaseSense()=0;
    
    virtual VROOT_h MountRoot(const TCHAR* rootpath, const TCHAR* vpath)=0;
	virtual void UnMountAll()=0;
    virtual FILE_h OpenFile(const TCHAR* filepath, int tag=VFS_OF_TAG_NONE)=0;
    virtual FILE_h OpenFile(VROOT_h hroot, const TCHAR* filepath, long lparam, int tag=VFS_OF_TAG_NONE)=0;
	virtual IBuffer* GetFileBuffer(FILE_h hfile)=0;
    virtual int CloseFile(FILE_h hfile)=0;
	virtual int SetCacheSize(int cachesize)=0;
	
	virtual ROOT_h OpenRoot(const TCHAR* rootpath, int mode)=0;
	virtual int CloseRoot(ROOT_h hroot)=0;

	virtual int CopyFile(ROOT_h hsrc, const TCHAR* srcpath, ROOT_h htrg, const TCHAR* trgpath)=0;
	virtual int MoveFile(ROOT_h hsrc, const TCHAR* srcpath, ROOT_h htrg, const TCHAR* trgpath)=0;
	virtual IBuffer* ReadFile(ROOT_h hsrc, const TCHAR* srcpath)=0;
	virtual int WriteFile(ROOT_h htrg, const TCHAR* trgpath, IBuffer* pbuffer)=0;
	virtual int DelFile(ROOT_h htrg, const TCHAR* trgpath)=0;
	virtual int DelTree(ROOT_h htrg, const TCHAR* startpath)=0;

	virtual int GetPathType(ROOT_h hroot, const TCHAR* rpath)=0;

	virtual FIND_h FindFirstFile(ROOT_h hroot, const TCHAR* startpath, bool recursive=true)=0;
	virtual bool FindNextFile(FIND_h find)=0;
	virtual int FindClose(FIND_h hfind)=0;
	virtual FIND_h GetFindRoot(FIND_h hfind)=0;
	virtual const TCHAR* GetFindPathName(FIND_h hfind)=0;
	virtual const TCHAR* GetRelativeFindPathName(FIND_h hfind)=0;

	virtual int  MakeDir(ROOT_h hroot, const TCHAR* rpath)=0;

    virtual int RegisterRWFilter(const TCHAR* ext, IRWFilter* pfilter)=0;
	virtual IRWFilter* GetFileRWFilter(const TCHAR* path)=0;
    
    virtual bool IsBuildinFileMapper()=0;
    virtual bool IsBuildinHandleAlloc()=0;
};


NAMESPACEEND
