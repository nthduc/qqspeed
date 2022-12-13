
#pragma once

#pragma warning(disable : 4786)

#if !defined(VFS_PORTABLE)
#  include "teniodll.h"
#  define IFILESYS TComponentImpl<IFileSys, CFileSys, COMPONENTID_VFS_FILESYS, 1>
#  define RELEASEFUNC_ADDON void ComponentFinalize() {}
#else
#  define IFILESYS IFileSys
#  define RELEASEFUNC_ADDON
#endif

#  include "VFS.h"

NAMESPACEBEGIN

    
struct rootdata_t
{
    rootdata_t(const TCHAR* _dir, const TCHAR* _vdir)
       : dir(_dir), vdir(_vdir)
    {
	}
    stl_string dir;
    stl_string vdir;
};

struct openrootdata_t
{
	openrootdata_t(const TCHAR* _dir, int _mode)
		: dir(_dir), mode(_mode)
	{
	}
	int mode;
	stl_string dir;
};

struct findfiledata_t
{
	ROOT_h hroot;
	openrootdata_t *proot;
	bool recursive;
	HANDLE hfind;
	stl_string startpath;
	stl_string parentpath;
	stl_string path;
	int prefixpathlen;
	stl_list<stl_string> s;
};

   
class CFileSys : public IFILESYS
{
    typedef stl_map<VROOT_h,rootdata_t*> MAPVROOT2ROOTDATA;
    typedef MAPVROOT2ROOTDATA::iterator  MAPVROOT2ROOTDATAITER;
	typedef stl_map<ROOT_h,openrootdata_t*> MAPROOT2OPENROOTDATA;
	typedef MAPROOT2OPENROOTDATA::iterator MAPROOT2OPENROOTDATAITER;
    typedef stl_map<stl_string,IRWFilter*> MAPEXT2RWFILTER;
    typedef MAPEXT2RWFILTER::iterator   MAPEXT2RWFILTERITER;
	typedef stl_map<FIND_h,findfiledata_t*> MAPFIND2FINDFILEDATA;
	typedef MAPFIND2FINDFILEDATA::iterator MAPFIND2FINDFILEDATAITER;

public:
    CFileSys();
    ~CFileSys();

	RELEASEFUNC_ADDON

    virtual int Init(IFileSysMgr* filesysmgr);

    virtual void Release();

	virtual void SetCaseSense(bool enable) { m_bCaseSense=enable; }
	virtual bool IsCaseSense() {return m_bCaseSense; }

    virtual VROOT_h MountRoot(const TCHAR* rootdir, const TCHAR* vdir);
	virtual void UnMountAll();
    virtual FILE_h OpenFile(const TCHAR* filepath, int tag);
    virtual FILE_h OpenFile(VROOT_h hroot, const TCHAR* filepath, long lparam, int tag);
	virtual IBuffer* GetFileBuffer(FILE_h hfile);
	virtual int CloseFile(FILE_h hfile);

	virtual int SetCacheSize(int cachesize);


	virtual ROOT_h OpenRoot(const TCHAR* rootdir, int mode);
	virtual int CloseRoot(ROOT_h hroot);

	virtual int CopyFile(ROOT_h hsrc, const TCHAR* srcpath, ROOT_h htrg, const TCHAR* trgpath);
	virtual int MoveFile(ROOT_h hsrc, const TCHAR* srcpath, ROOT_h htrg, const TCHAR* trgpath);
	virtual IBuffer* ReadFile(ROOT_h hsrc, const TCHAR* srcpath);
	virtual int WriteFile(ROOT_h htrg, const TCHAR* trgpath, IBuffer* pbuffer);
	virtual int DelFile(ROOT_h htrg, const TCHAR* trgpath);
	virtual int DelTree(ROOT_h htrg, const TCHAR* startpath);

	virtual int GetPathType(ROOT_h hroot, const TCHAR* rpath);

	virtual FIND_h FindFirstFile(ROOT_h hroot, const TCHAR* startpath, bool recursive=true);
	virtual bool FindNextFile(FIND_h hfind);
	virtual int FindClose(FIND_h hfind);
	virtual ROOT_h GetFindRoot(FIND_h hfind);
	virtual const TCHAR* GetFindPathName(FIND_h hfind);
	virtual const TCHAR* GetRelativeFindPathName(FIND_h hfind);

	virtual int  MakeDir(ROOT_h hroot, const TCHAR* rpath);

    virtual bool IsBuildinFileMapper();
    virtual bool IsBuildinHandleAlloc();

    virtual int RegisterRWFilter(const TCHAR* ext, IRWFilter* pfilter);
	virtual IRWFilter* GetFileRWFilter(const TCHAR* path);

protected:

    VROOT_h GetRealFilePath(const TCHAR* filepath, TCHAR* realfilepath);

	IBuffer* OpenFileIndirect(rootdata_t* proot, const TCHAR* filepath, int tag);

protected:

    MAPVROOT2ROOTDATA m_mapVRoot2RootData;
	MAPROOT2OPENROOTDATA m_mapRoot2OpenRootData;
	MAPFIND2FINDFILEDATA m_mapFind2FindFileData;

	IRWFilter* m_pDefaultRWFilter;

    bool m_bUseBuildinHandleAlloc;
    IHandleAlloc* m_pHandleAlloc;

    bool m_bUseBuildinFileMapper;
    IFileMapper*  m_pFileMapper;

	bool m_bUseBuildinBufferCache;
	IBufferCache* m_pBufferCache;

    MAPEXT2RWFILTER m_mapExt2RWFilter;

    IFileSysMgr*  m_pFileSysMgr;

    bool m_bInit;

	bool m_bCaseSense;
}; 


NAMESPACEEND