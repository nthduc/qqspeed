
#pragma once 

#pragma warning(disable : 4786)

#if !defined(VFS_PORTABLE)
#  include "teniodll.h"
#  define IFILESYS TComponentImpl<IFileSys, CVFileSys, COMPONENTID_VFS_VFILESYS, 1>
#  define RELEASEFUNC_ADDON void ComponentFinalize() {}
#else
#  define IFILESYS IFileSys
#  define RELEASEFUNC_ADDON
#endif

#  include "VFS.h"

NAMESPACEBEGIN


#if !defined(u32)
#define u32 unsigned int
#endif

#if !defined(u8)
#define u8 unsigned char
#endif

#pragma pack(push,1)

//#define VFILENAME_MAXLEN 32

#define VFS_DEF_BLOCKSIZE 512

#define VFS_FREEHEADTAG 'free'
#define VFS_HEADTAG 'head'
#define VFS_TAILTAG 'tail'

struct vfdt_bin1_t
{
	u32 head_tag;
    stl_string filename;
};

struct vfdt_bin2_t
{
    u32 origin_size;
    u32 saved_size;
    u32 fileattr;
    u32 first_block_offset;
    u32 parent_offset;
    u32 next_sibling_offset;
    u32 prev_sibling_offset;
    u32 first_child_offset;
    u8  reserved[16];
	u32 tail_tag;
};


struct vfdt_t
{
    vfdt_t() { parent=first_child=next_sibling=prev_sibling=NULL; memset(&bin2,0,sizeof(bin2)); bin1.head_tag=VFS_HEADTAG; bin1.filename=""; bin2.tail_tag=VFS_TAILTAG; }
    vfdt_t* parent;
    vfdt_t* next_sibling;
    vfdt_t* prev_sibling;
    vfdt_t* first_child; 
    int idx;
	vfdt_bin1_t bin1;
	vfdt_bin2_t bin2;
};

struct vfs_bin_t
{
    u32 id;
    u32 version;
    u32 blocksize;
    u32 fat_rwfilter_id;
    u32 freeblock_num;
    u32 first_freeblock_offset;
    u32 fdtroot_offset;
    u8 reserved[128];
};

struct vfs_t
{
    typedef stl_list<vfdt_t*> fdtlist_t;
    typedef fdtlist_t::iterator fdtlistiter_t;

    vfs_t()  
    {
        memset(&bin,0,sizeof(bin));
    }

    ~vfs_t() 
    { 
        for (fdtlistiter_t iter=fdt_list.begin();iter!=fdt_list.end(); iter++)
        {
            if (*iter) delete *iter;
        }
    }

    void clear_fdtlist()
    {
        for (fdtlistiter_t iter=fdt_list.begin();iter!=fdt_list.end(); iter++)
        {
            if (*iter) delete *iter;
        }
        fdt_list.clear();
    }
    
    vfs_bin_t   bin;
    fdtlist_t   fdt_list;
};


struct file_block_t
{
	u32 head_tag;
    u32 cont_block_num;
    u32 next_block_offset;
    u32 real_data_size;
};

struct free_block_t
{
	u32 head_tag;
    u32 cont_block_num;
    u32 next_block_offset;
};

#pragma pack(pop)

struct vrootdata_t
{
    vrootdata_t(HANDLE _hwinfile, vfs_t* _vfs, const TCHAR* _path, const TCHAR* _vdir) :
        hwinfile(_hwinfile), vfs(_vfs), path(_path), vdir(_vdir){}
  
    ~vrootdata_t() 
    {
        if (vfs) delete vfs;
        if (hwinfile) CloseHandle(hwinfile);
    }
    
    HANDLE hwinfile;
    vfs_t* vfs;

    stl_string path;
    stl_string vdir;
};

struct vopenrootdata_t
{
    vopenrootdata_t(HANDLE _hwinfile, vfs_t* _vfs, const TCHAR* _path, int _mode) 
        : hwinfile(_hwinfile), vfs(_vfs), path(_path), mode(_mode) 
    {
    }
  
    ~vopenrootdata_t() 
    {
        if (vfs) delete vfs;
        if (hwinfile) CloseHandle(hwinfile);
    }
    
    HANDLE hwinfile;
    vfs_t* vfs;

    stl_string path;
    int    mode;
};

struct vfindfiledata_t
{
    ROOT_h hroot;
    vopenrootdata_t* proot;
    vfdt_t* pnextsibfdt;
	stl_string startpath;
    stl_string path;
    stl_string dir;
    stl_list< stl_pair<stl_string,vfdt_t*> > s;
    bool recursive;
};


class CVFileSys : public IFILESYS
{
    typedef stl_map<VROOT_h,vrootdata_t*> MAPVROOT2ROOTDATA;
    typedef MAPVROOT2ROOTDATA::iterator  MAPVROOT2ROOTDATAITER;
    typedef stl_map<ROOT_h,vopenrootdata_t*> MAPROOT2OPENROOTDATA;
    typedef MAPROOT2OPENROOTDATA::iterator MAPROOT2OPENROOTDATAITER;
    typedef stl_map<stl_string,IRWFilter*> MAPEXT2RWFILTER;
    typedef MAPEXT2RWFILTER::iterator   MAPEXT2RWFILTERITER;
    typedef stl_map<ROOT_h,vfindfiledata_t*> MAPFIND2FINDFILEDATA;
    typedef MAPFIND2FINDFILEDATA::iterator MAPFIND2FINDFILEDATAITER;

public:

    CVFileSys();
    ~CVFileSys();

	RELEASEFUNC_ADDON

    virtual int Init(IFileSysMgr* filesysmgr);
    virtual void Release();

	virtual void SetCaseSense(bool enable) { m_bCaseSense=enable; }
	virtual bool IsCaseSense() { return m_bCaseSense; }

    virtual VROOT_h MountRoot(const TCHAR* rootpath, const TCHAR* vdir);
    virtual void UnMountAll();

    virtual FILE_h OpenFile(const TCHAR* filepath, int tag);
    virtual FILE_h OpenFile(VROOT_h hroot, const TCHAR* filepath, long lparam, int tag);

    virtual int CloseFile(FILE_h hfile);
    
    virtual IBuffer* GetFileBuffer(FILE_h hfile);

	virtual int SetCacheSize(int cachesize);
	

    virtual ROOT_h OpenRoot(const TCHAR* rootpath,int mode);
    virtual int CloseRoot(ROOT_h hroot);

    virtual int CopyFile(ROOT_h hsrc, const TCHAR* srcpath, ROOT_h htrg, const TCHAR* trgpath);
    virtual int MoveFile(ROOT_h hsrc, const TCHAR* srcpath, ROOT_h htrg, const TCHAR* trgpath);
    virtual IBuffer* ReadFile(ROOT_h hsrc, const TCHAR* srcpath);
    virtual int WriteFile(ROOT_h htrg, const TCHAR* trgpath, IBuffer* pbuffer);
    virtual int DelFile(ROOT_h htrg, const TCHAR* trgpath);
	virtual int DelTree(ROOT_h htrg, const TCHAR* startpath);

    virtual int GetPathType(ROOT_h hroot, const TCHAR* path);

	virtual FIND_h FindFirstFile(ROOT_h hroot, const TCHAR* startpath, bool recursive=true);
    virtual bool FindNextFile(FIND_h hfind);
    virtual int FindClose(FIND_h hfind);
	virtual ROOT_h GetFindRoot(FIND_h hfind);
    virtual const TCHAR* GetFindPathName(FIND_h hfind);
	virtual const TCHAR* GetRelativeFindPathName(FIND_h hfind);

	virtual int  MakeDir(ROOT_h hroot, const TCHAR* rpath);

    virtual bool IsBuildinFileMapper();
    virtual bool IsBuildinHandleAlloc();

    virtual int RegisterRWFilter(const TCHAR* ext, IRWFilter* prwfilter);
    virtual IRWFilter* GetFileRWFilter(const TCHAR* path);

protected:

    int FDT_Link2Offset(vfs_t* pvfs);
    int FDT_Offset2Link(vfs_t* pvfs);

    int ReadHead(HANDLE hwinfile, vfs_t* pvfs);
    int WriteHead(HANDLE hwinfile, vfs_t* pvfs);

    int ReadFDT(HANDLE hwinfile, vfs_t* pvfs);
    int WriteFDT(HANDLE hwinfile, vfs_t* pvfs);

	int FreeFDT(vopenrootdata_t* proot, vfdt_t* pfdt, bool bfile=true);
    int FreeFileBlock(vfs_t* pvfs, HANDLE hwinfile, int file_block_offset);

    int MapVRoot(VROOT_h hroot, vrootdata_t* prootdata, const TCHAR* fixedvdir);

    vfdt_t* OpenFileFdt(vopenrootdata_t * proot, const TCHAR* path, bool createable);
    
    IBuffer* ReadRawFile(vopenrootdata_t* proot, vfdt_t* pfdt);
    int WriteRawFile(vopenrootdata_t* proot, vfdt_t* pfdt, IBuffer* buffer);

    IBuffer* ReadFile(vopenrootdata_t* proot, vfdt_t* pfdt);
    int WriteFile(vopenrootdata_t* proot, vfdt_t* pfdt, IBuffer* pbuffer);

    int CopyFile(vopenrootdata_t* psrcroot, vfdt_t* psrcfdt, const TCHAR* srcpath,
          vopenrootdata_t* ptrgroot, const TCHAR* trgpath
        );

    int MoveFile(vopenrootdata_t* psrcroot, vfdt_t* psrcfdt, const TCHAR* srcpath,
          vopenrootdata_t* ptrgroot, const TCHAR* trgpath
        );

    int DelFile(vopenrootdata_t* proot, vfdt_t* pfdt);

    int GetPathType(vopenrootdata_t* proot, const TCHAR* path);

protected:

    MAPVROOT2ROOTDATA m_mapVRoot2RootData;
    MAPROOT2OPENROOTDATA m_mapRoot2OpenRootData;
    MAPFIND2FINDFILEDATA m_mapFind2FindFileData;

    bool m_bUseBuildinHandleAlloc;
    IHandleAlloc* m_pHandleAlloc;

    bool m_bUseBuildinFileMapper;
    IFileMapper*  m_pFileMapper;

	bool m_bUseBuildinBufferCache;
	IBufferCache* m_pBufferCache;

    MAPEXT2RWFILTER m_mapExt2RWFilter;
    IRWFilter* m_pDefaultRWFilter;

    IFileSysMgr*  m_pFileSysMgr;

    bool m_bInit;

	bool m_bCaseSense;
};


NAMESPACEEND