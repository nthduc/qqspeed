
#pragma once

NAMESPACEBEGIN


class IFileSysMgr PUBLICBASECLASS
{
public:
    virtual int Init() = 0;
    virtual void Release() = 0;
    
    virtual int RegisterFileSys(IFileSys* filesys, const TCHAR* rootext)=0;
    virtual int UnRegisterFileSys(IFileSys* filesys)=0;
    virtual int UnRegisterAllFileSys()=0;
    
    virtual IFileSys* GetVFileSysByExt(const TCHAR* extname)=0;
	virtual IFileSys* GetVFileSysByHandle(VFS_HANDLE handle)=0;
    virtual IFileSys* GetGeneralFileSys()=0;

    virtual IHandleAlloc* GetHandleAlloc()=0;
    virtual IFileMapper* GetVFSFileMapper()=0;
	virtual IFileMapper* GetFSFileMapper()=0;
	virtual IBufferCache* GetVFSBufferCache()=0;
	virtual IBufferCache* GetFSBufferCache()=0;

	virtual void SetCaseSense(bool enable)=0;
	virtual bool IsCaseSense()=0;
    
	//virtual access

    virtual VROOT_h MountRoot(const TCHAR* rootpath, const TCHAR* vdir)=0;
    virtual void UnMountAll()=0;
    virtual IFileSys* GetFileSysByHandle(VFS_HANDLE handle)=0;

    virtual FILE_h OpenFile(const TCHAR* filepath, int tag=VFS_OF_TAG_NONE)=0;
	virtual IBuffer* GetFileBuffer(FILE_h hfile)=0;
    virtual void CloseFile(FILE_h hfile)=0;

	virtual int SetCacheSize(int fscachesize, int vfscachesize)=0;

    virtual void SetVFSFirst(bool bfirst=true)=0;

	
	// normal access

	virtual ROOT_h OpenRoot(const TCHAR* rootpath, int mode=VFS_OPENROOT_MODE_READ)=0;
	virtual int CloseRoot(ROOT_h hroot)=0;

	virtual int CopyFile(ROOT_h hsrc, const TCHAR* srcpath, ROOT_h htrg, const TCHAR* trgpath)=0;
	virtual int MoveFile(ROOT_h hsrc, const TCHAR* srcpath, ROOT_h htrg, const TCHAR* trgpath)=0;
	virtual IBuffer* ReadFile(ROOT_h hsrc, const TCHAR* srcpath)=0;
	virtual int WriteFile(ROOT_h htrg, const TCHAR* trgpath,IBuffer* pbuffer)=0;
	virtual int DelFile(ROOT_h htrg, const TCHAR* trgpath)=0;
	virtual int DelTree(ROOT_h htrg, const TCHAR* trgpath)=0;

	virtual FIND_h FindFirstFile(ROOT_h hroot, const TCHAR* startpath, bool recursive=true)=0;
	virtual bool FindNextFile(FIND_h hfind)=0;
	virtual int FindClose(FIND_h hfind)=0;
	virtual FIND_h GetFindRoot(FIND_h hfind)=0;
	virtual const TCHAR* GetFindPathName(FIND_h hfind)=0;
	virtual const TCHAR* GetRelativeFindPathName(FIND_h hfind)=0;

};

NAMESPACEEND
