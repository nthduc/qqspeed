
#pragma once

#pragma warning(disable : 4786)

#if !defined(VFS_PORTABLE)
#  include "teniodll.h"
#  define IFILESYSMGR TComponentImpl<IFileSysMgr, CFileSysMgr, COMPONENTID_VFS_FILESYSMGR, 1>
#  define RELEASEFUNC_ADDON void ComponentFinalize() {}
#else
#  define IFILESYSMGR IFileSysMgr
#  define RELEASEFUNC_ADDON
#endif

#  include "VFS.h"

NAMESPACEBEGIN


class CFileSysMgr : public IFILESYSMGR
{
    typedef stl_map<stl_string,IFileSys*> MAPEXTNAME2FILESYS;
    typedef MAPEXTNAME2FILESYS::iterator  MAPEXTNAME2FILESYSITER;

	typedef stl_map<VFS_HANDLE,IFileSys*> MAPHANDLE2FILESYS;
	typedef MAPHANDLE2FILESYS::iterator   MAPHANDLE2FILESYSITER;

    typedef stl_vector<IFileSys*>         VECFILESYS;
    typedef VECFILESYS::iterator          VECFILESYSITER;
        
public:

    CFileSysMgr();
    virtual ~CFileSysMgr(); 
    
    virtual int Init();
    virtual void Release();

	RELEASEFUNC_ADDON
        
    virtual int RegisterFileSys(IFileSys* filesys, const TCHAR* rootext);
    virtual int UnRegisterFileSys(IFileSys* filesys);
    virtual int UnRegisterAllFileSys();
    
    IFileSys* GetVFileSysByExt(const TCHAR* extname);
	IFileSys* GetVFileSysByHandle(VFS_HANDLE handle);
    IFileSys* GetGeneralFileSys();

	virtual void SetCaseSense(bool enable);
	virtual bool IsCaseSense();

	//
    virtual VROOT_h MountRoot(const TCHAR* rootpath, const TCHAR* vdir);
    virtual void UnMountAll();
    virtual IFileSys* GetFileSysByHandle(VFS_HANDLE handle);
    virtual FILE_h OpenFile(const TCHAR* filepath, int mode);
	virtual IBuffer* GetFileBuffer(FILE_h hfile);
    virtual void CloseFile(FILE_h hfile);
	virtual int SetCacheSize(int fscachesize, int vfscachesize);


	//
	virtual ROOT_h OpenRoot(const TCHAR* rootpath, int mode);
	virtual int CloseRoot(ROOT_h hroot);

	//
	virtual int CopyFile(ROOT_h hsrc, const TCHAR* srcpath, ROOT_h htrg, const TCHAR* trgpath);
	virtual int MoveFile(ROOT_h hsrc, const TCHAR* srcpath, ROOT_h htrg, const TCHAR* trgpath);
	virtual IBuffer* ReadFile(ROOT_h hsrc, const TCHAR* srcpath);
	virtual int WriteFile(ROOT_h htrg, const TCHAR* trgpath,IBuffer* pbuffer);
	virtual int DelFile(ROOT_h htrg, const TCHAR* trgpath);
	virtual int DelTree(ROOT_h htrg, const TCHAR* trgpath);

	virtual FIND_h FindFirstFile(ROOT_h hroot, const TCHAR* startpath, bool recursive=true);
	virtual bool FindNextFile(FIND_h find);
	virtual int FindClose(FIND_h find);
	virtual FIND_h GetFindRoot(FIND_h hfind);
	virtual const TCHAR* GetFindPathName(FIND_h hfind);
	virtual const TCHAR* GetRelativeFindPathName(FIND_h hfind);
    
    virtual void SetVFSFirst(bool bfirst);
    
    virtual IHandleAlloc* GetHandleAlloc();
    virtual IFileMapper* GetVFSFileMapper();

	virtual IFileMapper* GetFSFileMapper();

	virtual IBufferCache* GetVFSBufferCache();
	virtual IBufferCache* GetFSBufferCache();
    
protected:

    IFileSys*  m_pGeneralFileSys;
    VECFILESYS m_vecVFSFileSys;
    MAPEXTNAME2FILESYS m_mapExtName2FileSys;
	MAPHANDLE2FILESYS m_mapHandle2FileSys;

    IHandleAlloc* m_pHandleAlloc;

    IFileMapper*  m_pVFSFileMapper;
	IBufferCache* m_pVFSBufferCache;

	IFileMapper*  m_pFSFileMapper;
	IBufferCache* m_pFSBufferCache;

    bool m_bVFSFirst;

	bool m_bCaseSense;
};
 

NAMESPACEEND