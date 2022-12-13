
#include "VFileSys.h"

NAMESPACEBEGIN

#if !defined(VFS_PORTABLE)
	DEFINE_FACTORY_METHOD(CVFileSys);
#endif



static ILogSys* s_pLogSys = NULL;

#define ERRORLOG(msg) if (s_pLogSys) s_pLogSys->printf(_T("%s(%d) : error %s\n"),__FILE__,__LINE__,msg);
#define ERRORLOG1(msg,p1) if (s_pLogSys) s_pLogSys->printf(_T("%s(%d) : error "##msg"\n"),__FILE__,__LINE__,p1);
#define ERRORLOG2(msg,p1,p2) if (s_pLogSys) s_pLogSys->printf(_T("%s(%d) : error "##msg"\n"),__FILE__,__LINE__,p1,p2);
#define ERRORLOG3(msg,p1,p2,p3) if (s_pLogSys) s_pLogSys->printf(_T("%s(%d) : error "##msg"\n"),__FILE__,__LINE__,p1,p2,p3);


CVFileSys::CVFileSys()
 : m_pFileMapper(NULL), m_bInit(false), m_pHandleAlloc(NULL),  m_pFileSysMgr(NULL),
 m_pDefaultRWFilter(NULL), m_pBufferCache(NULL), m_bCaseSense(false)
{
}

CVFileSys::~CVFileSys()
{
    MAPVROOT2ROOTDATAITER iter1;
	for (iter1=m_mapVRoot2RootData.begin(); iter1!=m_mapVRoot2RootData.end(); ++iter1)
	{
		delete iter1->second;
	}
	m_mapVRoot2RootData.clear();
    
    MAPROOT2OPENROOTDATAITER iter2;
	for (iter2=m_mapRoot2OpenRootData.begin(); iter2!=m_mapRoot2OpenRootData.end(); ++iter2)
	{
		CloseRoot(iter2->first);
//		delete iter2->second;
	}
	m_mapRoot2OpenRootData.clear();

    MAPFIND2FINDFILEDATAITER iter4;
	for (iter4=m_mapFind2FindFileData.begin();iter4!=m_mapFind2FindFileData.end(); ++iter4)
	{
		delete iter4->second;
	}
	m_mapFind2FindFileData.clear();	

	if (m_pDefaultRWFilter)
	{
		m_pDefaultRWFilter->Release();
	}

	if (m_bUseBuildinBufferCache)
	{
		m_pBufferCache->Release();
	}	

	if (m_bUseBuildinFileMapper)
	{
		m_pFileMapper->Release();
	}

    if (m_bUseBuildinHandleAlloc)
	{
		m_pHandleAlloc->Release();
	}
}

void CVFileSys::Release()
{
#if defined(VFS_PORTABLE)
	delete this;
#else
	Free();
#endif
}

int CVFileSys::Init(IFileSysMgr* filesysmgr)
{
    m_pFileSysMgr = filesysmgr;

    IHandleAlloc* pHandleAlloc;
	IFileMapper* pFileMapper;
	IBufferCache* pBufferCache;

    if (filesysmgr!=NULL) 
    {
        pHandleAlloc = filesysmgr->GetHandleAlloc();
		pFileMapper = filesysmgr->GetVFSFileMapper();
		pBufferCache = filesysmgr->GetVFSBufferCache();
		if (pHandleAlloc==NULL || pFileMapper==NULL || pBufferCache==NULL) return 0;

        m_bUseBuildinHandleAlloc = false;
		m_bUseBuildinFileMapper = false;
		m_bUseBuildinBufferCache = false;
    }
	else
	{
        pHandleAlloc = ICreateHandleAlloc(s_pLogSys);

		if (pHandleAlloc==NULL)
		{
			return 0;
		}

		pFileMapper = ICreateFileMapper(s_pLogSys);
    
		if (pFileMapper==NULL)
		{
			pHandleAlloc->Release();
			return 0;                
		}    

		pBufferCache = ICreateBufferCache(pHandleAlloc,pFileMapper,0,s_pLogSys);

		if (pBufferCache==NULL)
		{
			pFileMapper->Release();
			pHandleAlloc->Release();
			return 0;
		}

		m_bUseBuildinFileMapper = true;
        m_bUseBuildinHandleAlloc = true;
		m_bUseBuildinBufferCache = true;
	}

    m_pHandleAlloc = pHandleAlloc;
	m_pFileMapper = pFileMapper;
	m_pBufferCache = pBufferCache;

	m_bInit = true;

	m_pDefaultRWFilter = ICreateZlibRWFilter(s_pLogSys);
	if (m_pDefaultRWFilter==NULL)
	{
		return 0;
	}

    return 1;
}


VROOT_h CVFileSys::MountRoot(const TCHAR* rootpath, const TCHAR* vdir)
{
	if (rootpath==NULL || vdir==NULL)
	{
		ERRORLOG(_T("param error!"));
		return VFS_INVALID_HANDLE_VALUE;
	}

    VROOT_h hroot = VFS_INVALID_HANDLE_VALUE;
    
    if (!m_bInit) return hroot;
    
    TCHAR fixedrootdir[MAX_PATH] = {_T('\0')};
    TCHAR fixedvdir[MAX_PATH] = {_T('\0')};

	GetFullPathName(rootpath,MAX_PATH,fixedrootdir,NULL);
    
    FixPathName(fixedrootdir);
    FixDirName(vdir,fixedvdir);

	if (!IsCaseSense())
	{
		StringToLower(fixedrootdir);
		StringToLower(fixedvdir);
	}

	int pathtype = VFSNAME::GetPathType(fixedrootdir);
	if (pathtype==VFS_PATH_DIRECTORY)
	{
		ERRORLOG1(_T("root [%s] not a fle"),fixedrootdir);
		return hroot;
	}

	MAPVROOT2ROOTDATAITER iter;
	for (iter=m_mapVRoot2RootData.begin();iter!=m_mapVRoot2RootData.end(); ++iter)
	{
		if (!_tcscmp(fixedrootdir,iter->second->path.c_str()))
		{
			ERRORLOG1(_T("root [%s] mount more than once"),fixedrootdir);
			return hroot;
		}
	}
    
	vrootdata_t* proot = NULL;

	HANDLE hwinfile;

	if (pathtype==VFS_PATH_NOTEXIST)
	{
		ERRORLOG1(_T("root [%s] not exist"),fixedrootdir);
		return hroot;
	}

	hwinfile = ::CreateFile(
        fixedrootdir,
        GENERIC_READ,
        FILE_SHARE_READ,
        NULL,
        OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL,
		NULL
    );

	if (hwinfile==INVALID_HANDLE_VALUE)
	{
		ERRORLOG1(_T("file [%s] open error"),fixedrootdir);
		return hroot;
	}			

	//read fdt

	vfs_t *pvfs = new vfs_t();

	if (NULL==pvfs)
	{
		CloseHandle(hwinfile);
		ERRORLOG(_T("memory not enough!"));
		return hroot;
	}

	if (0==ReadHead(hwinfile,pvfs))
	{
		CloseHandle(hwinfile);
		delete pvfs;
		ERRORLOG(_T("read VFS file head error"));
		return hroot;
	}

	if (0==ReadFDT(hwinfile,pvfs))
	{
		CloseHandle(hwinfile);
		delete pvfs;
		ERRORLOG1(_T("read [%s] FDT error"),fixedrootdir);
		return hroot;
	}

	if (0==FDT_Offset2Link(pvfs))
	{
		ERRORLOG1(_T("VFS[%s] fdt relink error!"),fixedrootdir);
		CloseHandle(hwinfile);
		delete pvfs;
		return hroot;
	}

    proot = new vrootdata_t(hwinfile,pvfs,fixedrootdir,fixedvdir);

    if (proot==NULL)
	{
		ERRORLOG(_T("memory not enough"));
		CloseHandle(hwinfile);
		delete pvfs;
		return hroot;
	}

    hroot = m_pHandleAlloc->AllocHandle();
    m_mapVRoot2RootData[hroot] = proot;

	MapVRoot(hroot,proot,fixedvdir);
	
    return hroot;	
}

void CVFileSys::UnMountAll()
{
	if (!m_bInit) return ;
	MAPVROOT2ROOTDATAITER iter;
	for (iter=m_mapVRoot2RootData.begin(); iter!=m_mapVRoot2RootData.end(); ++iter)
	{
		delete iter->second;
		m_pHandleAlloc->FreeHandle(iter->first);
	}
	m_mapVRoot2RootData.clear();
}


struct browser_node_t
{
	browser_node_t(vfdt_t* _parent, vfdt_t* _current, const TCHAR* _parentpath) :
		parent(_parent), current(_current), parentpath(_parentpath) {}
	vfdt_t* parent;
	vfdt_t* current;
	stl_string parentpath;
};

int CVFileSys::MapVRoot(VROOT_h hroot, vrootdata_t* prootdata, const TCHAR* fixedvdir)
{
	if (!m_bInit) return 0;

	if (hroot==VFS_INVALID_HANDLE_VALUE || prootdata==NULL || fixedvdir==NULL)
	{
		ERRORLOG(_T("param error!"));
		return 0;
	}

	//map vfile to file finder
	vfs_t* pvfs = prootdata->vfs;

	if (pvfs->fdt_list.size())
	{
		vfdt_t* pcur = *pvfs->fdt_list.begin(); //root
    
		int offset;
    
		vfdt_t* prev_sibling=NULL;
		vfdt_t* parent = NULL;
    
		TCHAR vfilepath[MAX_PATH] = {_T('\0')};
		TCHAR vcurrentpath[MAX_PATH] = {_T('\0')};
    
		_tcscpy(vcurrentpath,fixedvdir);

		stl_list<browser_node_t*> s;
    
		while(pcur)
		{
			if (pcur->bin2.fileattr&VFS_FILEATTR_FILE)
			{
				wsprintf(vfilepath,_T("%s%s"),vcurrentpath,pcur->bin1.filename.c_str());

				if (!IsCaseSense())
				{
					StringToLower(vfilepath);
				}

				m_pFileMapper->MapFile(vfilepath, hroot, (long)pcur);
			}
			else
			{ // DIR
				offset = pcur->bin2.first_child_offset;
				if (offset)
				{
					s.push_back(new browser_node_t(pcur,pcur->first_child,vcurrentpath));
				}
			}
			
			offset = pcur->bin2.next_sibling_offset;
        
			if (offset)
			{
				pcur=pcur->next_sibling;
			}
			else
			{
				if (s.size())
				{
					browser_node_t* bnode = s.front();                
					pcur = bnode->current;
					parent = bnode->parent;
					wsprintf(vcurrentpath,_T("%s%s\\"),bnode->parentpath.c_str(),parent->bin1.filename.c_str());
					s.pop_front();
					delete bnode;
				}
				else
				{
					break;
				}
			}
		}
	}
	return 1;
}
 

FILE_h CVFileSys::OpenFile(const TCHAR* filepath, int tag)
{
	if (!m_bInit) return VFS_INVALID_HANDLE_VALUE;

	if (filepath==NULL)
	{
		ERRORLOG(_T("param error!"));
		return VFS_INVALID_HANDLE_VALUE;
	}

	TCHAR fixedpath[MAX_PATH] = {_T('\0')};

	if (0==(tag&VFS_OF_TAG_STANDARDPATHNAME))
	{
		FixPathName(filepath,fixedpath);
	}
	else
	{
		_tcscpy(fixedpath,filepath);
	}

	long lparam;

	if (!IsCaseSense())
	{
		StringToLower(fixedpath);
	}

	VROOT_h hroot = m_pFileMapper->FindFile(fixedpath,&lparam);

	if (hroot==VFS_INVALID_HANDLE_VALUE)
	{
		return VFS_INVALID_HANDLE_VALUE;
	}

	return OpenFile(hroot,fixedpath,lparam,tag);
}

FILE_h CVFileSys::OpenFile(VROOT_h hroot, const TCHAR* filepath, long lparam, int tag)
{
	if (hroot==VFS_INVALID_HANDLE_VALUE || filepath==NULL)
	{
		ERRORLOG(_T("param error!"));
		return VFS_INVALID_HANDLE_VALUE;
	}

	FILE_h hfile = VFS_INVALID_HANDLE_VALUE;

	if (!m_bInit) return hfile;

	TCHAR fixedpath[MAX_PATH] = {_T('\0')};

	if (0==(tag&VFS_OF_TAG_STANDARDPATHNAME))
	{
		FixPathName(filepath,fixedpath);
	}
	else
	{
		_tcscpy(fixedpath,filepath);
	}

	if (!IsCaseSense())
	{
		StringToLower(fixedpath);
	}

	if (hroot==VFS_INVALID_HANDLE_VALUE) return hfile;

	if (hroot==m_pBufferCache->GetHandle())
	{
		m_pBufferCache->IncRefCount(lparam);
		return lparam;
	}

	//find root
	MAPVROOT2ROOTDATAITER iter = m_mapVRoot2RootData.find(hroot);
	if (iter==m_mapVRoot2RootData.end())
	{
		ERRORLOG1(_T("hroot [%s] not exist!"),hroot);
		return hfile;
	}

	vrootdata_t * prootdata = iter->second;

	HANDLE hwinfile = prootdata->hwinfile;

	vfdt_t* pfdt = (vfdt_t*)lparam;
	

	DWORD numread;

	file_block_t fileblock;

	u32 loadsize = pfdt->bin2.saved_size;
	u32 curloadsize = 0;

	IBuffer* buffer = NULL;

	if (loadsize)
	{
		buffer = ICreateBuffer(loadsize);
		
		if (buffer==NULL)
		{
			ERRORLOG(_T("create buffer error!"));
			return hfile;
		}

		void* pbuf = buffer->GetBuffer();

		u32 fileoffset = pfdt->bin2.first_block_offset;

		while (loadsize>0)
		{
			DWORD ret = ::SetFilePointer(hwinfile, fileoffset, NULL, FILE_BEGIN);
			if (ret==INVALID_SET_FILE_POINTER)
			{
				ERRORLOG(_T("wrong set file offset"));
				buffer->Release();
				return 0;
			}

			if (!::ReadFile(hwinfile, &fileblock, sizeof(fileblock), &numread, NULL )
				|| numread!=sizeof(fileblock)
				)
			{
				ERRORLOG(_T("read file error!"));
				buffer->Release();
				return hfile;
			}

			if (fileblock.head_tag!=VFS_HEADTAG)
			{
				ERRORLOG(_T("head tag invalid"));
				buffer->Release();
				return 0;
			}

			if (fileblock.real_data_size<=loadsize)
			{
				curloadsize = fileblock.real_data_size;
			}
			else
			{
				break; //error realsize > fdt.saved_size
			}

			if (!::ReadFile(hwinfile,pbuf,curloadsize,&numread,NULL)
				|| numread!=curloadsize
				)
			{
				ERRORLOG(_T("read file error!"));
				buffer->Release();
				return hfile;
			}

			pbuf=((TCHAR*)pbuf)+curloadsize;

			loadsize -= curloadsize;
			if (loadsize==0) break;
		
			fileoffset = fileblock.next_block_offset;
			if (fileoffset==0) break;
		}

		if (loadsize!=0 || fileblock.next_block_offset!=0)
		{
			buffer->Release();
			ERRORLOG(_T("read file error!"));
			return hfile;
		}
		else
		{
			IRWFilter* pfilter = GetFileRWFilter(fixedpath);
			if (pfilter!=NULL)
			{
				IBuffer* outbuf = ICreateBuffer(pfdt->bin2.origin_size,s_pLogSys);
				if (outbuf==NULL)
				{
					buffer->Release();
					return VFS_INVALID_HANDLE_VALUE;
				}

				if (0==pfilter->PostLoad(buffer,outbuf))
				{
					buffer->Release();
					outbuf->Release();
					return VFS_INVALID_HANDLE_VALUE;
				}

				buffer->Release();
				buffer = outbuf;
			}
		}
	}

	if (buffer==NULL)
	{
		return VFS_INVALID_HANDLE_VALUE;
	}

	return m_pBufferCache->AddCache(fixedpath,hroot,lparam, buffer);
}

int CVFileSys::CloseFile(FILE_h hfile)
{
	if (!m_bInit) return 0;

	if (hfile==VFS_INVALID_HANDLE_VALUE)
	{
		ERRORLOG(_T("param error!"));
		return 0;
	}

	int refc = m_pBufferCache->DecRefCount(hfile);

	if (refc==0) return 1;

	return 0;
}

IBuffer* CVFileSys::GetFileBuffer(FILE_h hfile)
{
	if (!m_bInit) return NULL;

	if (hfile==VFS_INVALID_HANDLE_VALUE)
	{
		ERRORLOG(_T("param error!"));
		return 0;
	}

	return m_pBufferCache->GetBuffer(hfile);
}

int CVFileSys::SetCacheSize(int cachesize)
{
	if (NULL==m_pBufferCache) return 0;
	return m_pBufferCache->SetCacheSize(cachesize);
}

ROOT_h CVFileSys::OpenRoot(const TCHAR* rootpath, int mode)
{
	if (!m_bInit) return VFS_INVALID_HANDLE_VALUE;

	if (rootpath==NULL)
	{
		ERRORLOG(_T("param error!"));
		return VFS_INVALID_HANDLE_VALUE;
	}

	TCHAR fixedpath[MAX_PATH] = {_T('\0')};

	GetFullPathName(rootpath,MAX_PATH,fixedpath,NULL);

	FixPathName(fixedpath);

	if (!IsCaseSense())
	{
		StringToLower(fixedpath);
	}

	ROOT_h hroot = VFS_INVALID_HANDLE_VALUE;

	int pathtype = VFSNAME::GetPathType(fixedpath);

	HANDLE hwinfile;

	bool writehead=false;

	if (mode==VFS_OPENROOT_MODE_READ)
	{
		if (pathtype==VFS_PATH_NOTEXIST)
		{
			ERRORLOG1(_T("open root [%s] not exist"),fixedpath);
			return hroot;
		}

		hwinfile = ::CreateFile(
			fixedpath,
			GENERIC_READ,
			FILE_SHARE_READ,
			NULL,
			OPEN_EXISTING,
			FILE_ATTRIBUTE_NORMAL,
			NULL
		);

		if (hwinfile==INVALID_HANDLE_VALUE)
		{
			ERRORLOG1(_T("root [%s] open error!"),fixedpath);
			return hroot;
		}
	}
	else if (mode==VFS_OPENROOT_MODE_WRITE_ALWAYS)
	{
		hwinfile = ::CreateFile(
            fixedpath,
            GENERIC_READ+GENERIC_WRITE,
            FILE_SHARE_READ,
            NULL,
            OPEN_ALWAYS,
			FILE_ATTRIBUTE_NORMAL,
			NULL
        );
    
		if (hwinfile==INVALID_HANDLE_VALUE)
		{
			ERRORLOG1(_T("root [%s] write open error!"),fixedpath);
			return hroot;
		}

		if (pathtype==VFS_PATH_NOTEXIST)
		{
			writehead = true;
		}
	}
	else if (mode==VFS_OPENROOT_MODE_WRITE_NEW)
	{
		hwinfile = ::CreateFile(
            fixedpath,
            GENERIC_READ+GENERIC_WRITE,
            FILE_SHARE_READ,
            NULL,
            CREATE_ALWAYS,
			FILE_ATTRIBUTE_NORMAL,
			NULL
        );
    
		if (hwinfile==INVALID_HANDLE_VALUE)
		{
			ERRORLOG1(_T("root [%s] write open error!"),fixedpath);
			return hroot;
		}

		writehead = true;
	}
	else if (mode==VFS_OPENROOT_MODE_WRITE_EXIST)
	{
		if (pathtype==VFS_PATH_NOTEXIST)
		{
			ERRORLOG1(_T("root [%s] write open error, root not exist"),fixedpath);
			return hroot;
		}

		hwinfile = ::CreateFile(
            fixedpath,
            GENERIC_READ+GENERIC_WRITE,
            FILE_SHARE_READ,
            NULL,
            OPEN_EXISTING,
			FILE_ATTRIBUTE_NORMAL,
			NULL
        );
    
		if (hwinfile==INVALID_HANDLE_VALUE)
		{
			ERRORLOG1(_T("file [%s] write open error!"),fixedpath);
			return hroot;
		}
	}

	vfs_t *pvfs = new vfs_t();

	if (NULL==pvfs)
	{
		CloseHandle(hwinfile);
		ERRORLOG(_T("memory not enough!"));
		return hroot;
	}

	if (writehead)
	{
		vfs_bin_t& vfsbin = pvfs->bin;

		memset(&vfsbin,0,sizeof(vfsbin));
		vfsbin.id = VFS_FILE_HEAD_ID;
		vfsbin.blocksize = VFS_DEF_BLOCKSIZE;		
		//vfsbin.fdtroot_offset = sizeof(vfsbin);
		
		if (0==WriteHead(hwinfile,pvfs))
		{
			ERRORLOG1(_T("write vfs [%s] heads error"),fixedpath);
			CloseHandle(hwinfile);
			return hroot;
		}
	}
	else
	{
		if (0==ReadHead(hwinfile,pvfs))
		{
			CloseHandle(hwinfile);
			delete pvfs;
			ERRORLOG(_T("read vfs head error!"));
			return hroot;
		}

		if (0==ReadFDT(hwinfile,pvfs))
		{
			CloseHandle(hwinfile);
			delete pvfs;
			ERRORLOG1(_T("read file[%s] fdt error"),fixedpath);
			return hroot;
		}

		if (0==FDT_Offset2Link(pvfs))
		{
			ERRORLOG1(_T("VFS[%s] fdt relink error!"),fixedpath);
			CloseHandle(hwinfile);
			delete pvfs;
			return hroot;
		}
	}

    vopenrootdata_t* proot = new vopenrootdata_t(hwinfile,pvfs,fixedpath,mode);

    if (proot==NULL)
	{
		ERRORLOG(_T("memory not enough"));
		delete proot;
		return hroot;
	}

    hroot = m_pHandleAlloc->AllocHandle();
    m_mapRoot2OpenRootData[hroot] = proot;

    return hroot;	
}

int CVFileSys::ReadHead(HANDLE hwinfile, vfs_t* pvfs)
{
	if (hwinfile==INVALID_HANDLE_VALUE || pvfs==NULL)
	{
		ERRORLOG(_T("param error!"));
		return 0;
	}

	if (INVALID_SET_FILE_POINTER==::SetFilePointer(hwinfile,0,NULL,FILE_BEGIN))
	{
		return 0;
	}

	DWORD numread;

	if (!::ReadFile(hwinfile,&pvfs->bin,sizeof(pvfs->bin),&numread,NULL)
		|| numread!=sizeof(pvfs->bin)
		)
	{
		return 0;
	}
	return 1;
}

int CVFileSys::WriteHead(HANDLE hwinfile, vfs_t* pvfs)
{
	if (hwinfile==INVALID_HANDLE_VALUE || pvfs==NULL)
	{
		ERRORLOG(_T("param error!"));
		return 0;
	}

	if (INVALID_SET_FILE_POINTER==::SetFilePointer(hwinfile,0,NULL,FILE_BEGIN))
	{
		return 0;
	}

	DWORD numwrite;

	if (!::WriteFile(hwinfile,&pvfs->bin,sizeof(vfs_bin_t),&numwrite,NULL)
		|| numwrite!=sizeof(vfs_bin_t)
		)
	{
		return 0;
	}
	return 1;
}

int WriteOneFDT(HANDLE hwinfile, vfdt_bin1_t* pbin1, vfdt_bin2_t* pbin2)
{
	if (pbin1==NULL || pbin2==NULL || hwinfile==INVALID_HANDLE_VALUE)
	{
		ERRORLOG(_T("param error"));
		return 0;
	}

	DWORD numwrite;

	if (pbin1->head_tag!=VFS_HEADTAG || pbin2->tail_tag!=VFS_TAILTAG)
	{
		ERRORLOG(_T("fdt data error, some memory access over range"));
		return 0;
	}

	if (!::WriteFile(hwinfile,&pbin1->head_tag,sizeof(pbin1->head_tag),&numwrite,NULL)
		|| numwrite!=sizeof(pbin1->head_tag)
		)
	{
		ERRORLOG(_T("write file error!"));
		return 0;
	}	

	u32 namelen = pbin1->filename.length()*sizeof(TCHAR);

	if (!::WriteFile(hwinfile,&namelen,sizeof(namelen),&numwrite,NULL)
		|| numwrite!=sizeof(namelen)
		)
	{
		ERRORLOG(_T("write file error!"));
		return 0;
	}

	if (!::WriteFile(hwinfile,pbin1->filename.c_str(),namelen,&numwrite,NULL)
		|| numwrite!=namelen
		)
	{
		ERRORLOG(_T("write file error!"));
		return 0;
	}

	if (!::WriteFile(hwinfile,pbin2,sizeof(*pbin2),&numwrite,NULL)
		|| numwrite!=sizeof(*pbin2)
		)
	{
		ERRORLOG(_T("write file error!"));
		return 0;
	}

	return 1;
}

int CVFileSys::WriteFDT(HANDLE hwinfile, vfs_t* pvfs)
{
	if (hwinfile==INVALID_HANDLE_VALUE || pvfs==NULL)
	{
		ERRORLOG(_T("param error!"));
		return 0;
	}

	u32 fdt_offset = pvfs->bin.fdtroot_offset;

	if (fdt_offset==0) return 1;

	DWORD ret = ::SetFilePointer(hwinfile,fdt_offset,NULL,FILE_BEGIN);
	if (ret==INVALID_SET_FILE_POINTER)
	{
		ERRORLOG(_T("wrong set file offset"));
		return 0;
	}

	DWORD numwrite;

	u32 fdt_count = pvfs->fdt_list.size();

	if (!::WriteFile(hwinfile,&fdt_count,sizeof(fdt_count),&numwrite,NULL)
		|| numwrite!=sizeof(fdt_count)
		)
	{
		ERRORLOG(_T("write fdt count error"));
		return 0;
	}

	if (0==FDT_Link2Offset(pvfs))
	{
		return 0;
	}

	if (fdt_count>0)
	{
		vfs_t::fdtlistiter_t iter = pvfs->fdt_list.begin();
		IRWFilter* pfilter=NULL;
		if ( NULL!=(pfilter=GetFileRWFilter(_T(".?fdt"))) )
		{
			IBuffer* pinbuf = ICreateBuffer(sizeof(vfdt_bin1_t)+sizeof(vfdt_bin2_t)+MAX_PATH*sizeof(TCHAR));
			if (pinbuf==NULL)
			{
				return 0;
			}

			IBuffer* poutbuf = ICreateBuffer(pinbuf->GetBufferSize()*2);
			if (poutbuf==NULL)
			{
				return 0;
			}

			for (int i=0; i<fdt_count; i++, ++iter)
			{
				if ( (*iter)->bin1.head_tag!=VFS_HEADTAG || (*iter)->bin2.tail_tag!=VFS_TAILTAG )
				{
					ERRORLOG(_T("fdt data error, some memory access over range"));
					return 0;
				}

				int tagsize = sizeof((*iter)->bin1.head_tag); 
				int namelen = (*iter)->bin1.filename.length()*sizeof(TCHAR);
				int buflen =  tagsize + namelen + sizeof(u32);

				pinbuf->Realloc(buflen+sizeof(vfdt_bin2_t));
				
				char* pbuf = (char*)pinbuf->GetBuffer();

				memcpy(pbuf,&((*iter)->bin1.head_tag),tagsize);
				memcpy(pbuf+tagsize, &namelen, sizeof(u32));
				memcpy(pbuf+tagsize+sizeof(u32), (*iter)->bin1.filename.c_str(), namelen);
				
				memcpy(pbuf+buflen,&(*iter)->bin2,sizeof(vfdt_bin2_t));

				int outsize = m_pDefaultRWFilter->GetSaveSize(pinbuf);

				if (0==poutbuf->Realloc(outsize))
				{
					return 0;
				}

				pfilter->PreSave(pinbuf,poutbuf);

				u32 savedsize = poutbuf->GetUsedSize();
				DWORD numwrite;

				if (!::WriteFile(hwinfile,&savedsize,sizeof(u32),&numwrite,NULL)
					|| numwrite!=sizeof(u32)
					)
				{
					return 0;
				}

				u32 orignsize = buflen+sizeof(vfdt_bin2_t);

				if (!::WriteFile(hwinfile,&orignsize,sizeof(u32),&numwrite,NULL)
					|| numwrite!=sizeof(u32)
					)
				{
					return 0;
				}

				if (!::WriteFile(hwinfile,poutbuf->GetBuffer(),savedsize,&numwrite,NULL)
					|| numwrite!=savedsize
					)
				{
					return 0;
				}
			}
			pinbuf->Release();
			poutbuf->Release();
		}
		else
		{
			for (int i=0; i<fdt_count; i++,++iter)
			{
				if (VFS_FAILED(WriteOneFDT(hwinfile,&(*iter)->bin1,&(*iter)->bin2)))
				{
					ERRORLOG(_T("WriteFDT error!"));
					return 0;
				}
			}
		}
	}

	return 1;
}

int CVFileSys::ReadFDT(HANDLE hwinfile, vfs_t* pvfs)
{
	if (hwinfile==INVALID_HANDLE_VALUE || pvfs==NULL)
	{
		ERRORLOG(_T("param error!"));
		return 0;
	}

	if (pvfs->bin.fdtroot_offset)
	{//如果存在文件

		DWORD ret = ::SetFilePointer(hwinfile,pvfs->bin.fdtroot_offset,NULL,FILE_BEGIN);
		if (ret==INVALID_SET_FILE_POINTER)
		{
			ERRORLOG(_T("wrong set file offset"));
			return 0;
		}

		DWORD numread;

		u32 fdt_count;

		if (!::ReadFile(hwinfile,&fdt_count,sizeof(fdt_count),&numread,NULL)
			|| numread!=sizeof(fdt_count) || fdt_count<0
			)
		{
			return 0;
		}

		if (fdt_count)
		{
			IRWFilter* pfilter=NULL;
			if ( NULL!=(pfilter=GetFileRWFilter(_T(".?fdt"))) )
			{
				pvfs->clear_fdtlist();

				IBuffer* pinbuf = ICreateBuffer((sizeof(vfdt_bin1_t)+sizeof(vfdt_bin2_t))*2+MAX_PATH*sizeof(TCHAR));
				if (pinbuf==NULL)
				{
					return 0;
				}

				IBuffer* poutbuf = ICreateBuffer(sizeof(vfdt_bin1_t)+sizeof(vfdt_bin2_t)+MAX_PATH*sizeof(TCHAR));
				if (poutbuf==NULL)
				{
					return 0;
				}

				for (int i=0; i<fdt_count; i++)
				{
					u32 savedsize;
					u32 orignsize;
					DWORD numread;

					if (!::ReadFile(hwinfile,&savedsize,sizeof(u32),&numread,NULL)
						|| numread!=sizeof(u32)
						)
					{
						pvfs->clear_fdtlist();
						return 0;
					}

					if (0==pinbuf->Realloc(savedsize))
					{
						pvfs->clear_fdtlist();
						return 0;
					}

					if (!::ReadFile(hwinfile,&orignsize,sizeof(u32),&numread,NULL)
						|| numread!=sizeof(u32)
						)
					{
						pvfs->clear_fdtlist();
						return 0;
					}

					if (0==poutbuf->Realloc(orignsize))
					{
						pvfs->clear_fdtlist();
						return 0;
					}

					if (!::ReadFile(hwinfile,pinbuf->GetBuffer(),savedsize,&numread,NULL)
						|| numread!=savedsize
						)
					{
						pvfs->clear_fdtlist();
						return 0;
					}

					pfilter->PostLoad(pinbuf,poutbuf);

					u8* pbuf = (u8*)poutbuf->GetBuffer();

					vfdt_t * pfdt = new vfdt_t();
					if (pfdt==NULL)
					{
						pvfs->clear_fdtlist();
						return 0;
					}

					pfdt->bin1.head_tag = *((u32*)pbuf);
					pbuf+=sizeof(u32);
					
					if (pfdt->bin1.head_tag!=VFS_HEADTAG)
					{
						delete pfdt;
						pvfs->clear_fdtlist();
						return 0;
					}

					int namelen = *(u32*)pbuf;
					pbuf += 4;
					TCHAR& ch = *(TCHAR*)(pbuf+namelen);
					TCHAR backch = ch;
					ch = _T('\0');
					pfdt->bin1.filename = (TCHAR*)pbuf;
					ch = backch;

					pbuf+=namelen;

					memcpy(&pfdt->bin2,pbuf,sizeof(vfdt_bin2_t));

					if (pfdt->bin2.tail_tag!=VFS_TAILTAG)
					{
						delete pfdt;
						pvfs->clear_fdtlist();
						return 0;
					}

					pvfs->fdt_list.push_back(pfdt);
				}
				pinbuf->Release();
				poutbuf->Release();
			}
			else
			{
				pvfs->clear_fdtlist();

				vfdt_t * pfdt;

				IBuffer* pinbuf = ICreateBuffer(MAX_PATH*sizeof(TCHAR));
				if (pinbuf==NULL)
				{
					return 0;
				}

				for (int i=0; i<fdt_count; i++)
				{
					pfdt = new vfdt_t();

					if (pfdt==NULL)
					{
						pvfs->clear_fdtlist();
						return 0;
					}

					u32 namelen;

					if (!::ReadFile(hwinfile,&pfdt->bin1.head_tag,sizeof(pfdt->bin1.head_tag),&numread,NULL)
						|| numread!=sizeof(pfdt->bin1.head_tag) || pfdt->bin1.head_tag!=VFS_HEADTAG
					    )
					{
						if (pfdt) delete pfdt;
						pvfs->clear_fdtlist();
						return 0;
					}

					if (!::ReadFile(hwinfile,&namelen,sizeof(u32),&numread,NULL)
						|| numread!=sizeof(u32)
						)
					{
						if (pfdt) delete pfdt;
						pvfs->clear_fdtlist();
						return 0;
					}

					if (0==pinbuf->Realloc(namelen+sizeof(TCHAR)))
					{
						if (pfdt) delete pfdt;
						pvfs->clear_fdtlist();
						return 0;
					}

					u8* pbuf = (u8*)pinbuf->GetBuffer();

					if (!::ReadFile(hwinfile,pbuf,namelen,&numread,NULL)
						|| numread!=namelen
						)
					{
						if (pfdt) delete pfdt;
						pvfs->clear_fdtlist();
						return 0;
					}

					*(TCHAR*)(pbuf+namelen) = _T('\0');

					pfdt->bin1.filename = (TCHAR*)pbuf;

					if (!::ReadFile(hwinfile,&pfdt->bin2,sizeof(vfdt_bin2_t),&numread,NULL)
						|| numread!=sizeof(vfdt_bin2_t) || pfdt->bin2.tail_tag!=VFS_TAILTAG
						)
					{
						if (pfdt) delete pfdt;
						pvfs->clear_fdtlist();
						return 0;					
					}

					pvfs->fdt_list.push_back(pfdt);
				}
			}
		}
	}
	return 1;
}

int CVFileSys::FreeFDT(vopenrootdata_t* proot, vfdt_t* pfdt, bool bfile)
{
	if (proot==NULL)
	{
		ERRORLOG(_T("param error!"));
		return 0;
	}

	vfs_t * pvfs = proot->vfs;

	if (bfile)
	{
		if (pfdt==NULL || pfdt->bin2.fileattr&VFS_PATH_DIRECTORY)
		{
			return 0;
		}
	}
	else
	{
		if (pfdt && pfdt->bin2.fileattr&VFS_PATH_FILE)
		{
			return 0;
		}
	}

	vfs_t::fdtlistiter_t iter;
	int i=0;
	stl_vector<vfs_t::fdtlistiter_t> fdt_iter_vec;
	for (iter=pvfs->fdt_list.begin(); iter!=pvfs->fdt_list.end(); ++iter)
	{
		(*iter)->idx=i++;
		fdt_iter_vec.push_back(iter);
	}

	vfdt_t *freelink=NULL, *nextlink;

	if (pfdt)
	{
		pvfs->fdt_list.erase(fdt_iter_vec[pfdt->idx]);
		freelink = pfdt->first_child;
	}
	else
	{
		if (pvfs->fdt_list.size()>0)
			freelink = *pvfs->fdt_list.begin();
	}
	
	stl_vector<vfdt_t*> s;

	//assert( ((pfdt->bin2.fileattr&VFS_PATH_DIRECTORY)!=0) == (NULL!=freelink) );

	while(freelink)
	{
		if (freelink->first_child)
		{
			s.push_back(freelink->first_child);
		}
		nextlink = freelink->next_sibling;
		if (0==FreeFileBlock(pvfs,proot->hwinfile,freelink->bin2.first_block_offset))
		{
			ERRORLOG(_T("free file block error"));
			return 0;
		}
		pvfs->fdt_list.erase(fdt_iter_vec[freelink->idx]);
		delete freelink;
		if (nextlink==NULL && s.size())
		{
			nextlink = s.back();
			s.pop_back();
		}
		freelink=nextlink;
	}

	if (pfdt)
	{
		if (pfdt->prev_sibling)
		{
			pfdt->prev_sibling->next_sibling = pfdt->next_sibling;
			if (pfdt->next_sibling) pfdt->next_sibling->prev_sibling = pfdt->prev_sibling;
		}
		else if (pfdt->parent)
		{
			pfdt->parent->first_child=pfdt->next_sibling;
		}
		else
		{
			if (pfdt->next_sibling) pfdt->next_sibling->prev_sibling = NULL;
		}

		if (0==FreeFileBlock(pvfs,proot->hwinfile,pfdt->bin2.first_block_offset))
		{
			ERRORLOG(_T("free file block error"));
			return 0;
		}

		delete pfdt;
	}
	return 1;
}



int CVFileSys::FreeFileBlock(vfs_t* pvfs, HANDLE hwinfile, int file_block_offset)
{
	if (pvfs==NULL || hwinfile==INVALID_HANDLE_VALUE)
	{
		ERRORLOG(_T("param error!"));
		return 0;
	}

	file_block_t fileblock;
	free_block_t freeblock;

	DWORD numreadwrite;

	while(file_block_offset)
	{
		DWORD ret = ::SetFilePointer(hwinfile, file_block_offset, NULL, FILE_BEGIN);
		if (ret==INVALID_SET_FILE_POINTER)
		{
			ERRORLOG(_T("wrong set file offset"));
			return 0;
		}

		if (!::ReadFile(hwinfile,&fileblock,sizeof(fileblock),&numreadwrite,NULL)
			|| numreadwrite!=sizeof(fileblock)
			)
		{
			ERRORLOG(_T("error read file"));
			return 0;
		}

		if (fileblock.head_tag!=VFS_HEADTAG)
		{
			ERRORLOG(_T("file head tag invalid"));
			return 0;
		}

		freeblock.head_tag = VFS_FREEHEADTAG;
		freeblock.cont_block_num = fileblock.cont_block_num;
		freeblock.next_block_offset = pvfs->bin.first_freeblock_offset;

		ret = ::SetFilePointer(hwinfile, file_block_offset, NULL, FILE_BEGIN);
		if (ret==INVALID_SET_FILE_POINTER)
		{
			ERRORLOG(_T("wrong set file offset"));
			return 0;
		}

		if (!::WriteFile(hwinfile,&freeblock,sizeof(freeblock),&numreadwrite,NULL)
			|| sizeof(freeblock)!=numreadwrite
			)
		{
			return 0;
		}

		pvfs->bin.freeblock_num += freeblock.cont_block_num;
		pvfs->bin.first_freeblock_offset = file_block_offset;
		file_block_offset = fileblock.next_block_offset;
	}

	return 1;
}


int CVFileSys::FDT_Link2Offset(vfs_t* pvfs)
{
	if (NULL==pvfs)
	{
		ERRORLOG(_T("param error!"));
		return 0;
	}

	int fdt_count = pvfs->fdt_list.size();

	if (fdt_count<=0) return 1;

	u32 fdt_offset = pvfs->bin.fdtroot_offset+sizeof(u32);

	vfs_t::fdtlistiter_t pcurfdtiter = pvfs->fdt_list.begin();

	int i;

	vfdt_t* pcurfdt;

	stl_map<vfdt_t*,u32> link;

	u32 curoffset = fdt_offset;

	for (i=0; i<fdt_count; i++, pcurfdtiter++)
	{
		pcurfdt = *pcurfdtiter;
		link[pcurfdt] = curoffset;
		curoffset += sizeof(pcurfdt->bin1.head_tag) 
			        + sizeof(u32) 
					+ pcurfdt->bin1.filename.length()*sizeof(TCHAR) 
					+ sizeof(vfdt_bin2_t);
	}

	pcurfdtiter = pvfs->fdt_list.begin();

	for (i=0; i<fdt_count; i++, ++pcurfdtiter)
	{
		pcurfdt = *pcurfdtiter;

		vfdt_t* parent_link = pcurfdt->parent;
		vfdt_t* first_child_link = pcurfdt->first_child;
		vfdt_t* next_sibling_link = pcurfdt->next_sibling;
		vfdt_t* prev_sibling_link = pcurfdt->prev_sibling;

		stl_map<vfdt_t*,u32>::iterator iter;

		pcurfdt->bin2.parent_offset = pcurfdt->bin2.first_child_offset 
			= pcurfdt->bin2.next_sibling_offset = pcurfdt->bin2.prev_sibling_offset 
			= 0;

		if (parent_link)
		{
			if ((iter=link.find(parent_link))!=link.end())
			{
				pcurfdt->bin2.parent_offset = iter->second;
			}
			else
			{
				ERRORLOG("error fdtlink2offset");
				return 0;
			}
		}

		if (first_child_link) 
		{
			if ((iter=link.find(first_child_link))!=link.end())
			{
				pcurfdt->bin2.first_child_offset = iter->second;
			}
			else
			{
				ERRORLOG("error fdtlink2offset");
				return 0;
			}
		}

		if (next_sibling_link)
		{
			if ((iter=link.find(next_sibling_link))!=link.end())
			{
				pcurfdt->bin2.next_sibling_offset = iter->second;
			}
			else
			{
				ERRORLOG("error fdtlink2offset");
				return 0;
			}
		}

		if (prev_sibling_link)
		{
			if ((iter=link.find(prev_sibling_link))!=link.end())
			{
				pcurfdt->bin2.prev_sibling_offset = iter->second;
			}
			else
			{
				ERRORLOG("error fdtlink2offset");
				return 0;
			}
		}
	}

	return 1;
}

int CVFileSys::FDT_Offset2Link(vfs_t* pvfs)
{
	if (NULL==pvfs)
	{
		ERRORLOG(_T("param error!"));
		return 0;
	}

	u32 fdt_count = pvfs->fdt_list.size();
	if (fdt_count<=0) return 1;

	u32 fdt_offset = pvfs->bin.fdtroot_offset+sizeof(u32);

	vfs_t::fdtlistiter_t pcurfdtiter = pvfs->fdt_list.begin();
	vfdt_t* pcurfdt;

	stl_map<u32,vfdt_t*> offset;
	u32 curoffset=fdt_offset;
	for (;pcurfdtiter!=pvfs->fdt_list.end();++pcurfdtiter)
	{
		pcurfdt = * pcurfdtiter;
		offset[curoffset]=pcurfdt;
		curoffset+= pcurfdt->bin1.filename.length()*sizeof(TCHAR) 
			        + sizeof(pcurfdt->bin1.head_tag)
					+ sizeof(u32)
					+ sizeof(pcurfdt->bin2)
					;		
	}

	for (pcurfdtiter = pvfs->fdt_list.begin(); pcurfdtiter!=pvfs->fdt_list.end(); ++pcurfdtiter)
	{
		pcurfdt = *pcurfdtiter;

		u32 parent_offset = pcurfdt->bin2.parent_offset;
		u32 prev_sibling_offset = pcurfdt->bin2.prev_sibling_offset;
		u32 next_sibling_offset = pcurfdt->bin2.next_sibling_offset;
		u32 first_child_offset = pcurfdt->bin2.first_child_offset;

		if (  (parent_offset && parent_offset<fdt_offset)
			||(first_child_offset && first_child_offset<fdt_offset)
			||(prev_sibling_offset && prev_sibling_offset<fdt_offset)
			||(next_sibling_offset && next_sibling_offset<fdt_offset)
			)
		{
			ERRORLOG(_T("fdt data error!"));
			return 0;
		}

		stl_map<u32,vfdt_t*>::iterator iter;

		pcurfdt->parent = pcurfdt->prev_sibling = pcurfdt->next_sibling = pcurfdt->first_child = NULL;

		if (parent_offset)
		{
			if ((iter=offset.find(parent_offset))!=offset.end())
			{
				pcurfdt->parent = iter->second;
			}
			else
			{
				ERRORLOG(_T("fdt data error"));
				return 0;
			}
		}

		if (prev_sibling_offset)
		{
			if ((iter=offset.find(prev_sibling_offset))!=offset.end())
			{
				pcurfdt->prev_sibling = iter->second;
			}
			else
			{
				ERRORLOG(_T("fdt data error"));
				return 0;
			}
		}
		
		if (next_sibling_offset)
		{
			if ((iter=offset.find(next_sibling_offset))!=offset.end())
			{
				pcurfdt->next_sibling = iter->second;
			}
			else
			{
				ERRORLOG(_T("fdt data error"));
				return 0;
			}
		}
		
		if (first_child_offset)
		{
			if ((iter=offset.find(first_child_offset))!=offset.end())
			{
				pcurfdt->first_child = iter->second;
			}
			else
			{
				ERRORLOG(_T("fdt data error"));
				return 0;
			}
		}
	}

	return 1;
}

int CVFileSys::CloseRoot(ROOT_h hroot)
{
	if (!m_bInit) return 0;

	if (hroot==VFS_INVALID_HANDLE_VALUE)
	{
		ERRORLOG(_T("param error!"));
		return 0;
	}

	MAPROOT2OPENROOTDATAITER iter;
	iter = m_mapRoot2OpenRootData.find(hroot);
	if (iter==m_mapRoot2OpenRootData.end())
	{
		return 0;
	}
	
	vopenrootdata_t* proot = iter->second;

	if ( (proot->mode==VFS_OPENROOT_MODE_WRITE_ALWAYS) ||
		 (proot->mode==VFS_OPENROOT_MODE_WRITE_EXIST) ||
		 (proot->mode==VFS_OPENROOT_MODE_WRITE_NEW)
		)
	{
		if (0==WriteHead(proot->hwinfile,proot->vfs))
		{
			ERRORLOG(_T("write vfs head error"));
			return 0;
		}

		if (0==WriteFDT(proot->hwinfile,proot->vfs))
		{
			ERRORLOG(_T("write vfs fdt error"));
			return 0;
		}
	}

	delete proot;

	m_pHandleAlloc->FreeHandle(iter->first);

	m_mapRoot2OpenRootData.erase(iter);
	
	return 1;
}

IBuffer* CVFileSys::ReadRawFile(vopenrootdata_t* proot, vfdt_t* pfdt)
{
	if (proot==NULL || pfdt==NULL || proot->hwinfile==INVALID_HANDLE_VALUE) 
	{
		ERRORLOG(_T("param error!"));
		return NULL;
	}

	file_block_t fileblock;

	DWORD numread;

	u32 cur_read = 0;
	u32 want_read = pfdt->bin2.saved_size;

	IBuffer* pbuffer = ICreateBuffer(want_read,s_pLogSys);
	if (pbuffer==NULL)
	{
		ERRORLOG(_T("memory not enough"));
		return NULL;
	}

	char* pbuf = (char*)pbuffer->GetBuffer();

	u32 block_offset = pfdt->bin2.first_block_offset;

	for (;block_offset;)
	{
		DWORD ret = ::SetFilePointer(proot->hwinfile,block_offset,NULL,FILE_BEGIN);
		if (ret==INVALID_SET_FILE_POINTER)
		{
			ERRORLOG(_T("wrong set file offset"));
			pbuffer->Release();
			return 0;
		}

		if (!::ReadFile(proot->hwinfile,&fileblock,sizeof(file_block_t),&numread,NULL)
			|| numread!=sizeof(file_block_t)
			)
		{
			ERRORLOG(_T("read file error"));
			pbuffer->Release();
			return NULL;
		}

		if (fileblock.head_tag==VFS_FREEHEADTAG)
		{
			ERRORLOG(_T("file block head tag error"));
			pbuffer->Release();
			return NULL;
		}

		if (fileblock.real_data_size>want_read)
		{
			ERRORLOG(_T("file block size error"));
			pbuffer->Release();
			return NULL;
		}
		else
		{
			cur_read = fileblock.real_data_size;
		}

		if (!::ReadFile(proot->hwinfile,pbuf,cur_read,&numread,NULL)
			|| numread!=cur_read
			)
		{
			ERRORLOG(_T("read file error"));
			pbuffer->Release();
			return NULL;
		}

		pbuf+=cur_read;

		want_read-=cur_read;

		block_offset = fileblock.next_block_offset;		

		if (want_read<=0 || block_offset==0)
		{
			if (block_offset || want_read)
			{
				ERRORLOG(_T("read file error"));
				pbuffer->Release();
				return NULL;
			}
			return pbuffer;
		}
	}
	return NULL;
}

int CVFileSys::WriteRawFile(vopenrootdata_t* proot, vfdt_t* pfdt, IBuffer* pbuffer)
{
	if (proot==NULL || pfdt==NULL || proot->vfs==NULL)
	{
		ERRORLOG(_T("param error!"));
		return 0;
	}

	if (proot->mode==VFS_OPENROOT_MODE_READ)
	{
		ERRORLOG(_T("root couldn't write"));
		return 0;
	}

	u32 file_offset = pfdt->bin2.first_block_offset;

	int saved_size = pbuffer->GetUsedSize();
	char* pbuf = (char*)pbuffer->GetBuffer();

	vfs_t* pvfs = proot->vfs;

	int block_size = pvfs->bin.blocksize;
	if (block_size==0) return 0;
	
	file_block_t file_block;
	free_block_t free_block;

	DWORD numreadwrite;

	int cursavedsize;
	int curblocksize;
	int wantsavesize=saved_size;

	int prev_file_offset = -1;

	u32 curusedblock;
	u32 next_block_offset;
	u32 cont_block_num;

	//write file block

	if (file_offset!=0)
	{
		pfdt->bin2.first_block_offset = file_offset;
	}

	for(;file_offset;)
	{
		DWORD ret = ::SetFilePointer(proot->hwinfile,file_offset,NULL,FILE_BEGIN);
		if (ret==INVALID_SET_FILE_POINTER)
		{
			ERRORLOG(_T("wrong set file offset"));
			return 0;
		}

		if (!::ReadFile(proot->hwinfile,&file_block,sizeof(file_block_t),&numreadwrite,NULL)
			|| numreadwrite != sizeof(file_block_t)
			)
		{
			ERRORLOG(_T("read file error!"));
			return 0;
		}

		if (file_block.head_tag!=VFS_HEADTAG)
		{
			ERRORLOG(_T("file head tag error!"));
			return 0;
		}

		curblocksize = file_block.cont_block_num*block_size-sizeof(file_block_t);

		if (curblocksize > wantsavesize)
		{
			cursavedsize = wantsavesize;
		}
		else
		{
			cursavedsize = curblocksize;
		}

		if (!::WriteFile(proot->hwinfile,pbuf,cursavedsize,&numreadwrite,NULL)
			|| cursavedsize != numreadwrite
			)
		{
			ERRORLOG(_T("write file error!"));
			return 0;
		}

		curusedblock = (cursavedsize + sizeof(file_block_t));
		curusedblock = curusedblock/block_size + ((curusedblock%block_size)?1:0);

		next_block_offset = file_block.next_block_offset;
		cont_block_num = file_block.cont_block_num;

		pbuf+=cursavedsize;
		wantsavesize-=cursavedsize;

		if (wantsavesize==0)
		{
			if (!(file_block.cont_block_num==curusedblock &&
				  file_block.real_data_size==cursavedsize && 
				  file_block.next_block_offset==0
				  )
				)
			{   //与以前数据不同时，才去写
				file_block.head_tag = VFS_HEADTAG;
				file_block.cont_block_num = curusedblock;
				file_block.real_data_size = cursavedsize;
				file_block.next_block_offset = 0;
				DWORD ret = ::SetFilePointer(proot->hwinfile,file_offset,NULL,FILE_BEGIN);
				if (ret==INVALID_SET_FILE_POINTER)
				{
					ERRORLOG(_T("wrong set file offset"));
					return 0;
				}

				if (!::WriteFile(proot->hwinfile,&file_block,sizeof(file_block),&numreadwrite,NULL)
					|| numreadwrite!=sizeof(file_block)
					)
				{
					ERRORLOG(_T("write file error!"));
					return 0;
				}
			}

			int restblock = cont_block_num-curusedblock;

			if (restblock)
			{
				free_block.head_tag = VFS_FREEHEADTAG;
				free_block.cont_block_num = restblock;
				free_block.next_block_offset = pvfs->bin.first_freeblock_offset;
				file_offset += curusedblock*block_size;
				DWORD ret = ::SetFilePointer(proot->hwinfile,file_offset,NULL,FILE_BEGIN);
				if (ret==INVALID_SET_FILE_POINTER)
				{
					ERRORLOG(_T("wrong set file offset"));
					return 0;
				}

				if (!::WriteFile(proot->hwinfile,&free_block,sizeof(free_block),&numreadwrite,NULL)
					|| sizeof(free_block)!=numreadwrite
					)
				{
					ERRORLOG(_T("write file error!"));
					return 0;
				}

				pvfs->bin.first_freeblock_offset=file_offset;
				pvfs->bin.freeblock_num+=restblock;
			}

			while(next_block_offset)
			{
				DWORD ret = ::SetFilePointer(proot->hwinfile,next_block_offset,NULL,FILE_BEGIN);
				if (ret==INVALID_SET_FILE_POINTER)
				{
					ERRORLOG(_T("wrong set file offset"));
					return 0;
				}

				if (!::ReadFile(proot->hwinfile,&file_block,sizeof(file_block),&numreadwrite,NULL)
					|| numreadwrite!=sizeof(file_block)
					)
				{
					ERRORLOG(_T("read file error!"));
					return 0;
				}

				if (file_block.head_tag!=VFS_HEADTAG)
				{
					ERRORLOG(_T("file head tag error"));
					return 0;
				}

				free_block.head_tag = VFS_FREEHEADTAG;
				free_block.cont_block_num = file_block.cont_block_num;
				free_block.next_block_offset = pvfs->bin.first_freeblock_offset;

				ret = ::SetFilePointer(proot->hwinfile,next_block_offset,NULL,FILE_BEGIN);
				if (ret==INVALID_SET_FILE_POINTER)
				{
					ERRORLOG(_T("wrong set file offset"));
					return 0;
				}

				if (!::WriteFile(proot->hwinfile,&free_block,sizeof(free_block),&numreadwrite,NULL)
					|| sizeof(free_block)!=numreadwrite
					)
				{
					ERRORLOG(_T("write file error!"));
					return 0;
				}

				pvfs->bin.first_freeblock_offset = next_block_offset;
				next_block_offset = file_block.next_block_offset;
			}

			return 1;
		}
		else
		{
			if (!(file_block.cont_block_num==curusedblock &&
				  file_block.real_data_size==cursavedsize && 
				  file_block.next_block_offset==next_block_offset
				  )
				)
			{   //与以前数据不同时，才去写
				file_block.head_tag = VFS_HEADTAG;
				file_block.cont_block_num = curusedblock;
				file_block.real_data_size = cursavedsize;
				file_block.next_block_offset = next_block_offset;
				DWORD ret = ::SetFilePointer(proot->hwinfile,file_offset,NULL,FILE_BEGIN);
				if (ret==INVALID_SET_FILE_POINTER)
				{
					ERRORLOG(_T("wrong set file offset"));
					return 0;
				}

				if (!::WriteFile(proot->hwinfile,&file_block,sizeof(file_block),&numreadwrite,NULL)
					|| numreadwrite!=sizeof(file_block)
					)
				{
					ERRORLOG(_T("write file error!"));
					return 0;
				}
			}

			prev_file_offset = file_offset;
			file_offset = next_block_offset;
		}
	}

	//write free block

	if (wantsavesize)
	{
		if (pvfs->bin.freeblock_num > 0) file_offset = pvfs->bin.first_freeblock_offset;
		else file_offset = 0;

		if (wantsavesize==saved_size && file_offset!=0)
		{
			pfdt->bin2.first_block_offset = file_offset;
		}
		else
		{
			if (file_offset!=0 && prev_file_offset!=-1)
			{
				int diffsize = sizeof(file_block_t)-sizeof(u32);
				DWORD ret = ::SetFilePointer(proot->hwinfile, prev_file_offset+diffsize, NULL, FILE_BEGIN);
				if (ret==INVALID_SET_FILE_POINTER)
				{
					ERRORLOG(_T("wrong set file offset"));
					return 0;
				}

				//write next block offset
				if (!::WriteFile(proot->hwinfile,&file_offset,sizeof(file_offset),&numreadwrite,NULL)
					|| numreadwrite!=sizeof(file_offset)
					)
				{
					ERRORLOG(_T("write file error!"));
					return 0;
				}

				prev_file_offset = -1;
			}
		}

		for(;file_offset;)
		{
			DWORD ret = ::SetFilePointer(proot->hwinfile, file_offset, NULL, FILE_BEGIN);
			if (ret==INVALID_SET_FILE_POINTER)
			{
				ERRORLOG(_T("wrong set file offset"));
				return 0;
			}

			if (!::ReadFile(proot->hwinfile,&free_block,sizeof(free_block),&numreadwrite,NULL)
				|| sizeof(free_block)!=numreadwrite
				)
			{
				ERRORLOG(_T("read file error!"));
				return 0;
			}

			if (free_block.head_tag!=VFS_FREEHEADTAG)
			{
				ERRORLOG(_T("free block head tag error!"));
				return 0;
			}

			curblocksize = free_block.cont_block_num*block_size-sizeof(file_block_t);

			if (curblocksize>wantsavesize)
			{
				cursavedsize=wantsavesize;
			}
			else
			{
				cursavedsize=curblocksize;
			}

			int diffsize = sizeof(file_block_t)-sizeof(free_block_t);
			ret = ::SetFilePointer(proot->hwinfile, diffsize, NULL, FILE_CURRENT);
			if (ret==INVALID_SET_FILE_POINTER)
			{
				ERRORLOG(_T("wrong set file offset"));
				return 0;
			}

			if (!::WriteFile(proot->hwinfile,pbuf,cursavedsize,&numreadwrite,NULL)
				|| cursavedsize!=numreadwrite
				)
			{
				ERRORLOG(_T("write file error!"));
				return 0;
			}

			curusedblock = (cursavedsize + sizeof(file_block_t));
			curusedblock = curusedblock/block_size + ((curusedblock%block_size)?1:0);

			next_block_offset = free_block.next_block_offset;
			cont_block_num = free_block.cont_block_num;

			pvfs->bin.freeblock_num-=curusedblock;

			pbuf+=cursavedsize;
			wantsavesize-=cursavedsize;

			if (wantsavesize==0)
			{
				file_block.head_tag = VFS_HEADTAG;
				file_block.cont_block_num = curusedblock;
				file_block.real_data_size = cursavedsize;
				file_block.next_block_offset = 0;
				ret = ::SetFilePointer(proot->hwinfile,file_offset,NULL,FILE_BEGIN);
				if (ret==INVALID_SET_FILE_POINTER)
				{
					ERRORLOG(_T("wrong set file offset"));
					return 0;
				}

				if (!::WriteFile(proot->hwinfile,&file_block,sizeof(file_block),&numreadwrite,NULL)
					|| numreadwrite!=sizeof(file_block)
					)
				{
					ERRORLOG(_T("write file error!"));
					return 0;
				}


				int restblock = cont_block_num-curusedblock;

				if (restblock)
				{
					free_block.head_tag = VFS_FREEHEADTAG;
					free_block.cont_block_num = restblock;
					free_block.next_block_offset = next_block_offset;
					file_offset += curusedblock*block_size;
					DWORD ret = ::SetFilePointer(proot->hwinfile,file_offset,NULL,FILE_BEGIN);
					if (ret==INVALID_SET_FILE_POINTER)
					{
						ERRORLOG(_T("wrong set file offset"));
						return 0;
					}

					if (!::WriteFile(proot->hwinfile,&free_block,sizeof(free_block),&numreadwrite,NULL)
						|| sizeof(free_block)!=numreadwrite
						)
					{
						ERRORLOG(_T("write file error!"));
						return 0;
					}
					pvfs->bin.first_freeblock_offset = file_offset;
				}
				else
				{ 
					pvfs->bin.first_freeblock_offset = next_block_offset;
				}

				return 1;
			}
			else
			{
				file_block.head_tag = VFS_HEADTAG;
				file_block.cont_block_num = cont_block_num;
				file_block.real_data_size = cursavedsize;
				file_block.next_block_offset = next_block_offset;
				ret = ::SetFilePointer(proot->hwinfile,file_offset,NULL,FILE_BEGIN);
				if (ret==INVALID_SET_FILE_POINTER)
				{
					ERRORLOG(_T("wrong set file offset"));
					return 0;
				}

				if (!::WriteFile(proot->hwinfile,&file_block,sizeof(file_block),&numreadwrite,NULL)
					|| numreadwrite!=sizeof(file_block)
					)
				{
					ERRORLOG(_T("write file error!"));
					return 0;
				}

				prev_file_offset = file_offset;
				file_offset = next_block_offset;
				pvfs->bin.first_freeblock_offset = next_block_offset;
			}
		}

		//write new block

		if (wantsavesize)
		{
			u32 t = wantsavesize + sizeof(file_block_t);

			curusedblock = t/block_size + ((t%block_size)?1:0);

			file_block.head_tag = VFS_HEADTAG;
			file_block.cont_block_num = curusedblock;
			file_block.next_block_offset = 0;
			file_block.real_data_size = wantsavesize;

			file_offset = pvfs->bin.fdtroot_offset;

			if (file_offset==0) 
			{
				file_offset = sizeof(pvfs->bin);
				pvfs->bin.fdtroot_offset = file_offset;
			}

			if (wantsavesize==saved_size)
			{
				pfdt->bin2.first_block_offset = file_offset;
			}

			if (INVALID_SET_FILE_POINTER==::SetFilePointer(proot->hwinfile,file_offset,NULL,FILE_BEGIN))
			{
				ERRORLOG(_T("wrong set file offset"));
				return 0;
			}

			if (!::WriteFile(proot->hwinfile,&file_block,sizeof(file_block),&numreadwrite,NULL)
				|| sizeof(file_block)!=numreadwrite
				)
			{
				ERRORLOG(_T("write file error!"));
				return 0;
			}

			if (!::WriteFile(proot->hwinfile,pbuf,wantsavesize,&numreadwrite,NULL)
				|| wantsavesize!=numreadwrite
				)
			{
				ERRORLOG(_T("write file error!"));
				return 0;
			}

			int blockrealsize = curusedblock * block_size;
			wantsavesize = blockrealsize - wantsavesize;

			if (wantsavesize>=0)
			{
				TCHAR tmpbuf[512] = {_T('\0')};
				if (!::WriteFile(proot->hwinfile,tmpbuf,wantsavesize,&numreadwrite,NULL)
					|| wantsavesize!=numreadwrite
					)
				{
					ERRORLOG(_T("write file error!"));
					return 0;
				}
			}

			if (prev_file_offset!=-1)
			{
				DWORD ret = ::SetFilePointer(proot->hwinfile,prev_file_offset+sizeof(u32)*3,NULL,FILE_BEGIN);
				if (ret==INVALID_SET_FILE_POINTER)
				{
					ERRORLOG(_T("wrong set file offset"));
					return 0;
				}

				if (!::WriteFile(proot->hwinfile,&file_offset,sizeof(file_offset),&numreadwrite,NULL)
					|| numreadwrite!=sizeof(file_block)
					)
				{
					ERRORLOG(_T("write file error!"));
					return 0;
				}
			}

			file_offset += blockrealsize;
			pvfs->bin.fdtroot_offset = file_offset;
		}
	}

	pfdt->bin2.origin_size = saved_size;
	pfdt->bin2.saved_size = saved_size;

	return saved_size;
}


int CVFileSys::WriteFile(vopenrootdata_t* proot, vfdt_t* pfdt, IBuffer* pbuffer)
{
	if (proot==NULL || pfdt==NULL || pbuffer==NULL)
	{
		ERRORLOG(_T("param error!"));
		return 0;
	}

	IRWFilter* pfilter = GetFileRWFilter(pfdt->bin1.filename.c_str());
	if (pfilter)
	{
		int size = pfilter->GetSaveSize(pbuffer);

		IBuffer* pbuffer2 = ICreateBuffer(size,s_pLogSys);

		if (pbuffer2==NULL)
		{
			return 0;
		}

		size = pfilter->PreSave(pbuffer,pbuffer2);

		int ret = WriteRawFile(proot,pfdt,pbuffer2);	

		pfdt->bin2.origin_size = pbuffer->GetUsedSize();
		pfdt->bin2.saved_size = pbuffer2->GetUsedSize();

		pbuffer2->Release();

		return size;
	}

	return WriteRawFile(proot,pfdt,pbuffer);
}

IBuffer* CVFileSys::ReadFile(vopenrootdata_t* proot, vfdt_t* pfdt)
{
	if (proot==NULL || pfdt==NULL)
	{
		ERRORLOG(_T("param error!"));
		return NULL;
	}

	IBuffer* pbuffer = ReadRawFile(proot,pfdt);
	if (pbuffer)
	{
		IRWFilter* pfilter = GetFileRWFilter(pfdt->bin1.filename.c_str());
		if (pfilter)
		{
			IBuffer* pbuffer2 = ICreateBuffer(pfdt->bin2.origin_size);
			if (0==pfilter->PostLoad(pbuffer,pbuffer2))
			{
				pbuffer2->Release();
				pbuffer->Release();
				return NULL;
			}
			pbuffer->Release();
			pbuffer=pbuffer2;
		}
	}
	return pbuffer;
}

/*
IBuffer* CVFileSys::ReadFile(FIND_h hfind)
{
	if (hfind==VFS_INVALID_HANDLE_VALUE)
	{
		ERRORLOG(_T("param error!"));
		return NULL;
	}

	MAPFIND2FINDFILEDATAITER iter;
	iter = m_mapFind2FindFileData.find(hfind);
	if (iter==m_mapFind2FindFileData.end())
	{
		ERRORLOG(_T("hfind not found"));
		return NULL;
	}

	vfindfiledata_t * pfindfiledata = iter->second;
	return ReadFile(pfindfiledata->proot,pfindfiledata->pfdt);
}
*/

IBuffer* CVFileSys::ReadFile(ROOT_h hsrc, const TCHAR* srcpath)
{
	if (hsrc==VFS_INVALID_HANDLE_VALUE || srcpath==NULL)
	{
		ERRORLOG(_T("param error!"));
		return NULL;
	}

	MAPROOT2OPENROOTDATAITER iter;
	iter = m_mapRoot2OpenRootData.find(hsrc);
	if (iter==m_mapRoot2OpenRootData.end())
	{
		ERRORLOG(_T("hroot not found"));
		return NULL;
	}

	TCHAR fixedpath[MAX_PATH] = {_T('\0')};
	FixPathName(srcpath,fixedpath);
	srcpath = (const TCHAR*)fixedpath;

	vfdt_t* pfdt = OpenFileFdt(iter->second,srcpath,false);

	if (pfdt==NULL)
	{
		ERRORLOG1(_T("file [%s] not found"), srcpath);
		return NULL;
	}

	return ReadFile(iter->second,pfdt);
}

int CVFileSys::WriteFile(ROOT_h htrg, const TCHAR* trgpath, IBuffer* pbuffer)
{
	if (htrg==VFS_INVALID_HANDLE_VALUE || trgpath==NULL || pbuffer==NULL)
	{
		ERRORLOG(_T("param error!"));
		return 0;
	}

	MAPROOT2OPENROOTDATAITER iter;
	iter = m_mapRoot2OpenRootData.find(htrg);
	if (iter==m_mapRoot2OpenRootData.end())
	{
		ERRORLOG(_T("invalid root handle"));
		return NULL;
	}

	vopenrootdata_t* proot = iter->second;

	TCHAR fixedpath[MAX_PATH] = {_T('\0')};
	FixPathName(trgpath,fixedpath);
	trgpath = (const TCHAR*)fixedpath;

	vfdt_t * pfdt = OpenFileFdt(iter->second,trgpath,true);
	
	if (pfdt==NULL)
	{
		ERRORLOG1(_T("couldn't create path %s"),trgpath);
		return 0;
	}

	return WriteFile(iter->second,pfdt,pbuffer);
}

int CVFileSys::CopyFile(vopenrootdata_t* psrcroot, vfdt_t* psrcfdt, const TCHAR* srcpath,
			  	 vopenrootdata_t* ptrgroot, const TCHAR* trgpath
			   )
{
	if (!m_bInit) return 0;

	if (psrcroot==NULL || psrcfdt==NULL || srcpath==NULL || ptrgroot==NULL || trgpath==NULL)
	{
		ERRORLOG(_T("param error!"));
		return 0;
	}

	IBuffer* pbuffer;

	IRWFilter* psrcfilter = GetFileRWFilter(srcpath);
	IRWFilter* ptrgfilter = GetFileRWFilter(trgpath);

	if (psrcfilter==ptrgfilter)
	{
		pbuffer = ReadRawFile(psrcroot,psrcfdt);
		if (pbuffer==NULL)
		{
			ERRORLOG(_T("read file error!"));
			return 0;
		}

		vfdt_t * pfdt = OpenFileFdt(ptrgroot,trgpath,true);
		if (pfdt==NULL)
		{
			pbuffer->Release();
			ERRORLOG1(_T("create path %s error"),trgpath);
			return 0;
		}

		if (0==WriteRawFile(ptrgroot,pfdt,pbuffer))
		{
			pbuffer->Release();
			ERRORLOG(_T("write file error!"));
			return 0;
		}

		pfdt->bin2.origin_size = psrcfdt->bin2.origin_size;
		pfdt->bin2.saved_size = psrcfdt->bin2.saved_size;
	}
	else
	{
		pbuffer = ReadFile(psrcroot,psrcfdt);
		if (pbuffer==NULL)
		{
			ERRORLOG(_T("read file error!"));
			return 0;
		}

		vfdt_t * pfdt = OpenFileFdt(ptrgroot,trgpath,true);
		if (pfdt==NULL)
		{
			pbuffer->Release();
			ERRORLOG1(_T("create path %s error"),trgpath);
			return 0;
		}

		int saved_size;
		saved_size=WriteFile(ptrgroot,pfdt,pbuffer);
		if (saved_size==0)
		{
			pbuffer->Release();
			ERRORLOG(_T("write file error!"));
			return 0;
		}

		pfdt->bin2.origin_size = pbuffer->GetUsedSize();
		pfdt->bin2.saved_size = saved_size;
	}
	pbuffer->Release();
	return 1;
}

/*
int CVFileSys::CopyFile(FIND_h hfind, ROOT_h htrg, const TCHAR* trgpath)
{
	if (!m_bInit) return 0;

	if (hfind==VFS_INVALID_HANDLE_VALUE || htrg==VFS_INVALID_HANDLE_VALUE || trgpath==NULL)
	{
		ERRORLOG(_T("param error!"));
		return 0;
	}

	MAPFIND2FINDFILEDATAITER iter1;
	iter1 = m_mapFind2FindFileData.find(hfind);
	if (iter1==m_mapFind2FindFileData.end())
	{
		ERRORLOG(_T("invalid find handle"));
		return 0;
	}

	vfindfiledata_t* pfinddata = iter1->second;

	MAPROOT2OPENROOTDATAITER iter2;
	iter2 = m_mapRoot2OpenRootData.find(htrg);
	if (iter2==m_mapRoot2OpenRootData.end())
	{
		ERRORLOG(_T("invalid root handle"));
		return 0;
	}

	vopenrootdata_t* ptrgroot = iter2->second;

	TCHAR fixedpath[MAX_PATH] = {_T('\0')};
	VFSNAME::FixPathName(trgpath,fixedpath);
	TCHAR newtrgpath[MAX_PATH] = {_T('\0')};
	VFSNAME::GetRelativeTrgFileName(GetPathType(htrg,fixedpath),GetRelativeFindPathName(hfind),fixedpath,newtrgpath);

	return CopyFile(pfinddata->proot, pfinddata->pfdt, pfinddata->path.c_str(),ptrgroot,newtrgpath);
}
*/

int CVFileSys::MoveFile(vopenrootdata_t* psrcroot, vfdt_t* psrcfdt, const TCHAR* srcpath,
			   	 vopenrootdata_t* ptrgroot, const TCHAR* trgpath
			   )
{
	if (psrcroot==NULL || psrcfdt==NULL || srcpath==NULL || ptrgroot==NULL || trgpath==NULL)
	{
		ERRORLOG(_T("param error!"));
		return 0;
	}

	TCHAR trgfile[MAX_PATH] = {_T('\0')};
	TCHAR _trgpath[MAX_PATH] = {_T('\0')};
	_tcscpy(_trgpath,trgpath);
	trgpath = _trgpath;

	TCHAR *p = _tcsrchr(_trgpath,_T('\\'));
	if (p!=NULL && *++p!=_T('\0'))
	{
		_tcscpy(trgfile,p);
		*p = _T('\0');
	}
	else
	{
		_tcscpy(trgfile,_trgpath);
		_trgpath[0] = _T('\0');
	}

	if (psrcroot==ptrgroot)
	{
		vfdt_t* parentfdt = OpenFileFdt(ptrgroot,trgpath,false);
		if (parentfdt==NULL && trgpath[0]!=_T('\0'))
		{
			ERRORLOG1(_T("target path %s not found"),trgpath);
			return 0;
		}

		if (parentfdt!=psrcfdt->parent)
		{//no same directory
			if (psrcfdt->prev_sibling)
			{
				psrcfdt->prev_sibling = psrcfdt->next_sibling;
				psrcfdt->next_sibling->prev_sibling = psrcfdt->prev_sibling;
			}
			else if (psrcfdt->parent)
			{
				psrcfdt->parent = psrcfdt->next_sibling;
			}

			if (parentfdt)
			{
				psrcfdt->next_sibling = parentfdt->first_child;
				parentfdt->first_child->prev_sibling = psrcfdt;
				parentfdt->first_child = psrcfdt;
				psrcfdt->parent = parentfdt;
			}
			else
			{
				vfdt_t* rootfdt;
				if (psrcroot->vfs->fdt_list.size()>0)
				{
					rootfdt = *psrcroot->vfs->fdt_list.begin();
				}
				else 
				{
					rootfdt = NULL;
				}
				psrcfdt->parent = NULL;
				psrcfdt->prev_sibling = NULL;
				psrcfdt->next_sibling = rootfdt;
				if (rootfdt) rootfdt->prev_sibling = psrcfdt;

				psrcroot->vfs->fdt_list.push_front(psrcfdt);
			}

		}

		psrcfdt->bin1.filename = trgfile;
	}
	else
	{
		_tcscat(_trgpath,trgfile);

		IBuffer* pbuffer;
		
		IRWFilter* psrcfilter = GetFileRWFilter(srcpath);
		IRWFilter* ptrgfilter = GetFileRWFilter(trgpath);

		if (psrcfilter==ptrgfilter)
		{
			pbuffer = ReadRawFile(psrcroot,psrcfdt);
			if (NULL==pbuffer)
			{
				ERRORLOG1(_T("error read file %s"),srcpath);
				return 0;
			}

			vfdt_t * ptrgfdt = OpenFileFdt(ptrgroot,trgpath,true);
			if (ptrgfdt==NULL)
			{
				pbuffer->Release();
				ERRORLOG1(_T("file %s create error!"),trgpath);
				return 0;
			}
			
			if (0==WriteRawFile(ptrgroot,ptrgfdt,pbuffer))
			{
				pbuffer->Release();
				ERRORLOG1(_T("error write file %s!"),trgpath);
				return 0;
			}

			ptrgfdt->bin2.origin_size = psrcfdt->bin2.origin_size;
			ptrgfdt->bin2.saved_size = psrcfdt->bin2.saved_size;
		}
		else
		{
			pbuffer = ReadFile(psrcroot,psrcfdt);
			if (NULL==pbuffer)
			{
				ERRORLOG1(_T("error read file %s"),srcpath);
				return 0;
			}

			vfdt_t * ptrgfdt = OpenFileFdt(ptrgroot,trgpath,true);
			if (ptrgfdt==NULL)
			{
				pbuffer->Release();
				ERRORLOG1(_T("file %s create error!"),trgpath);
				return 0;
			}
			
			int saved_size;
			saved_size=WriteFile(ptrgroot,ptrgfdt,pbuffer);
			if (saved_size==0)
			{
				pbuffer->Release();
				ERRORLOG1(_T("error write file %s!"),trgpath);
				return 0;
			}

			ptrgfdt->bin2.origin_size = pbuffer->GetUsedSize();
			ptrgfdt->bin2.saved_size = saved_size;
		}

		pbuffer->Release();

		if (0==FreeFDT(psrcroot, psrcfdt))
		{
			ERRORLOG(_T("free fdt error"));
			return 0;
		}
	}

	return 1;
}


int CVFileSys::CopyFile(ROOT_h hsrc, const TCHAR* srcpath, ROOT_h htrg, const TCHAR* trgpath)
{
	if (!m_bInit) return 0;

	if (hsrc==VFS_INVALID_HANDLE_VALUE || srcpath==NULL || htrg==VFS_INVALID_HANDLE_VALUE || trgpath==NULL)
	{
		ERRORLOG(_T("param error!"));
		return 0;
	}

	vopenrootdata_t * psrcroot, * ptrgroot;
	MAPROOT2OPENROOTDATAITER iter;
	iter = m_mapRoot2OpenRootData.find(hsrc);
	if (iter==m_mapRoot2OpenRootData.end())
	{
		ERRORLOG(_T("invalid src root handle"));
		return 0;
	}

	psrcroot = iter->second;

	iter = m_mapRoot2OpenRootData.find(htrg);
	if (iter==m_mapRoot2OpenRootData.end())
	{
		ERRORLOG(_T("invalid trg root handle"));
		return 0;
	}

	ptrgroot = iter->second;

	TCHAR fixedsrcpath[MAX_PATH] = {_T('\0')};
	TCHAR fixedtrgpath[MAX_PATH] = {_T('\0')};
	FixPathName(srcpath,fixedsrcpath);
	FixPathName(trgpath,fixedtrgpath);

	TCHAR newtrgpath[MAX_PATH] = {_T('\0')};

	VFSNAME::GetTrgFileName(GetPathType(htrg,fixedtrgpath),fixedsrcpath,fixedtrgpath,newtrgpath);

	vfdt_t * psrcfdt = OpenFileFdt(psrcroot,fixedsrcpath,false);
	if (psrcfdt==NULL || (psrcfdt->bin2.fileattr&VFS_FILEATTR_DIR))
	{
		ERRORLOG1(_T("file %s not found"),srcpath);
		return 0;
	}

	return CopyFile(psrcroot, psrcfdt, fixedsrcpath, ptrgroot, fixedtrgpath);
}

int CVFileSys::MoveFile(ROOT_h hsrc, const TCHAR* srcpath, ROOT_h htrg, const TCHAR* trgpath)
{
	if (!m_bInit) return 0;

	if (hsrc==VFS_INVALID_HANDLE_VALUE || srcpath==NULL || htrg==VFS_INVALID_HANDLE_VALUE || trgpath==NULL)
	{
		ERRORLOG(_T("param error!"));
		return 0;
	}

	vopenrootdata_t * psrcroot, * ptrgroot;
	MAPROOT2OPENROOTDATAITER iter;
	iter = m_mapRoot2OpenRootData.find(hsrc);
	if (iter==m_mapRoot2OpenRootData.end())
	{
		ERRORLOG(_T("invalid src root handle"));
		return 0;
	}

	psrcroot = iter->second;

	iter = m_mapRoot2OpenRootData.find(htrg);
	if (iter==m_mapRoot2OpenRootData.end())
	{
		ERRORLOG(_T("invalid trg root handle"));
		return 0;
	}

	ptrgroot = iter->second;

	TCHAR fixedsrcpath[MAX_PATH] = {_T('\0')};
	TCHAR fixedtrgpath[MAX_PATH] = {_T('\0')};
	FixPathName(srcpath,fixedsrcpath);
	FixPathName(trgpath,fixedtrgpath);

	TCHAR newtrgpath[MAX_PATH] = {_T('\0')};

	VFSNAME::GetTrgFileName(GetPathType(htrg,fixedtrgpath),fixedsrcpath,fixedtrgpath,newtrgpath);

	vfdt_t * psrcfdt = OpenFileFdt(psrcroot,fixedsrcpath,false);
	if (psrcfdt==NULL || (psrcfdt->bin2.fileattr&VFS_FILEATTR_DIR))
	{
		ERRORLOG1(_T("file %s not found"),srcpath);
		return 0;
	}

	if (psrcroot->mode==VFS_OPENROOT_MODE_READ)
	{
		return CopyFile(psrcroot, psrcfdt, fixedsrcpath, ptrgroot, newtrgpath);
	}
	else
	{
		return MoveFile(psrcroot, psrcfdt, fixedsrcpath, ptrgroot, newtrgpath);
	}
}

int CVFileSys::DelFile(vopenrootdata_t* proot, vfdt_t* pfdt)
{
	if (proot==NULL || pfdt==NULL)
	{
		ERRORLOG(_T("param error!"));
		return 0;
	}

	if (pfdt->bin2.fileattr&VFS_FILEATTR_DIR)
	{
		ERRORLOG(_T("target is directory"));
		return 0;
	}

	if (0==FreeFDT(proot,pfdt))
	{
		ERRORLOG(_T("free fdt error"));
		return 0;
	}

	return 1;
}

int CVFileSys::DelFile(ROOT_h htrg, const TCHAR* trgpath)
{
	if (!m_bInit) return 0;

	if (htrg==VFS_INVALID_HANDLE_VALUE || trgpath==NULL)
	{
		ERRORLOG(_T("param error!"));
		return 0;
	}

	MAPROOT2OPENROOTDATAITER iter;
	iter = m_mapRoot2OpenRootData.find(htrg);
	if (iter==m_mapRoot2OpenRootData.end())
	{
		ERRORLOG(_T("root not found"));
		return 0;
	}

	vopenrootdata_t* proot = iter->second;

	if (proot->mode==VFS_OPENROOT_MODE_READ)
	{
		ERRORLOG(_T("no right to del file"));
		return 0;
	}

	TCHAR fixedpath[MAX_PATH] = {_T('\0')};
	FixPathName(trgpath,fixedpath);

	vfdt_t* pfdt = OpenFileFdt(proot,fixedpath,false);
	if (pfdt==NULL)
	{
		ERRORLOG(_T("del file not exist"));
		return 0;
	}

	if (pfdt->bin2.fileattr&VFS_FILEATTR_DIR)
	{
		ERRORLOG(_T("target is dir"));
		return 0;
	}

	if (0==FreeFDT(proot,pfdt))
	{
		ERRORLOG(_T("free fdt error"));
		return 0;
	}

	return 1;	
}

int CVFileSys::DelTree(ROOT_h htrg, const TCHAR* startpath)
{
	if (!m_bInit) return 0;

	if (htrg==VFS_INVALID_HANDLE_VALUE || startpath==NULL)
	{
		ERRORLOG(_T("param error!"));
		return 0;
	}

	MAPROOT2OPENROOTDATAITER iter;
	iter = m_mapRoot2OpenRootData.find(htrg);
	if (iter==m_mapRoot2OpenRootData.end())
	{
		ERRORLOG(_T("root not found"));
		return 0;
	}

	vopenrootdata_t* proot = iter->second;

	if (proot->mode==VFS_OPENROOT_MODE_READ)
	{
		ERRORLOG(_T("no right to del file"));
		return 0;
	}

	TCHAR fixedpath[MAX_PATH] = {_T('\0')};
	FixPathName(startpath,fixedpath);

	vfdt_t* pfdt;

	if (fixedpath[0]==0 || (fixedpath[1]==0 && fixedpath[0]==_T('\\')) )
	{
		pfdt = NULL;		
	}
	else
	{
		pfdt = OpenFileFdt(proot,fixedpath,false);
		if (pfdt==NULL)
		{
			ERRORLOG(_T("del dir not exist"));
			return 0;
		}

		if (pfdt->bin2.fileattr&VFS_FILEATTR_FILE)
		{
			ERRORLOG(_T("target is file"));
			return 0;
		}
	}

	if (0==FreeFDT(proot,pfdt,false))
	{
		ERRORLOG(_T("free fdt error"));
		return 0;
	}
	return 1;
}


FIND_h CVFileSys::FindFirstFile(ROOT_h hroot, const TCHAR* startpath, bool recursive)
{
	if (!m_bInit) return VFS_INVALID_HANDLE_VALUE;

	if (hroot==VFS_INVALID_HANDLE_VALUE || startpath==NULL)
	{
		ERRORLOG(_T("param error!"));
		return VFS_INVALID_HANDLE_VALUE;
	}

	MAPROOT2OPENROOTDATAITER iter;
	iter = m_mapRoot2OpenRootData.find(hroot);
	if (iter==m_mapRoot2OpenRootData.end())
	{
		ERRORLOG(_T("root not found"));
		return VFS_INVALID_HANDLE_VALUE;
	}

	FIND_h findresult = VFS_INVALID_HANDLE_VALUE;

	vopenrootdata_t* proot = iter->second;

	vfs_t* pvfs = proot->vfs;

	int fdt_count = pvfs->fdt_list.size();

	if (fdt_count<=0) return findresult;

	vfindfiledata_t* pfinddata = new vfindfiledata_t();

	if (pfinddata==NULL)
	{
		ERRORLOG(_T("memory not enough"));
		return VFS_INVALID_HANDLE_VALUE;
	}

	bool bfind = false;

	TCHAR _startdir[MAX_PATH] = {_T('\0')};
	FixDirName(startpath,_startdir);
	TCHAR* startdir;
	if (_startdir[0]==_T('\\'))
	{
		startdir = &_startdir[1];
	}
	else
	{
		startdir = _startdir;
	}

	vfdt_t* pfdt=NULL;
	if (startdir[0]==_T('\0'))
	{
		pfdt = *pvfs->fdt_list.begin();
	}
	else
	{
		pfdt = OpenFileFdt(proot,startdir,false); //*pvfs->fdt_list.begin();
		if (pfdt!=NULL) pfdt = pfdt->first_child;
	}

	if (pfdt==NULL)
	{
		ERRORLOG(_T("start path not found!"));
		return VFS_INVALID_HANDLE_VALUE;
	}

	TCHAR dir[MAX_PATH] = {_T('\0')};
	TCHAR path[MAX_PATH] = {_T('\0')};

	for(;;)
	{
		while (pfdt->bin2.fileattr&VFS_FILEATTR_DIR)
		{
			if (pfdt->first_child) pfinddata->s.push_back(stl_make_pair(stl_string(dir),pfdt));
			pfdt = pfdt->next_sibling;
			if (pfdt==NULL) break;
		}

		if (pfdt)
		{
			//find out
			wsprintf(path,_T("%s%s%s"),startdir,dir,pfdt->bin1.filename.c_str());
			pfinddata->startpath = startpath;
			pfinddata->dir = dir;
			pfinddata->path = path;
			pfinddata->hroot = hroot;
			pfinddata->recursive = recursive;
			pfinddata->proot = proot;
			pfinddata->pnextsibfdt = pfdt?pfdt->next_sibling:NULL;
			findresult = m_pHandleAlloc->AllocHandle();
			m_mapFind2FindFileData[findresult] = pfinddata;
			return findresult;
		}
		else
		{
			if (recursive)
			{
				if (pfinddata->s.size())
				{
					pfdt = pfinddata->s.front().second;
					wsprintf(dir,_T("%s%s\\"),pfinddata->s.front().first.c_str(),
						pfdt->bin1.filename.c_str());
					pfdt = pfdt->first_child;
				}
				else
				{
					delete pfinddata;
					return findresult;
				}
			}
			else
			{
				delete pfinddata;
				return findresult;
			}
		}
	}
	return findresult;
}

bool CVFileSys::FindNextFile(FIND_h hfind)
{
	if (!m_bInit) return false;

	if (hfind==VFS_INVALID_HANDLE_VALUE)
	{
		ERRORLOG(_T("param error!"));
		return false;
	}

	MAPFIND2FINDFILEDATAITER iter;
	iter = m_mapFind2FindFileData.find(hfind);
	if (iter==m_mapFind2FindFileData.end())
	{
		ERRORLOG(_T("hfind not exist"));
		return false;
	}

	vfindfiledata_t* pfindfiledata = iter->second;
	
	vfdt_t* pfdt = pfindfiledata->pnextsibfdt;

	TCHAR dir[MAX_PATH] = {_T('\0')};
	TCHAR path[MAX_PATH] = {_T('\0')};

	_tcscpy(dir,pfindfiledata->dir.c_str());

	for(;;)
	{
		if (pfdt)
		{
			while (pfdt->bin2.fileattr&VFS_FILEATTR_DIR)
			{
				if (pfdt->first_child) pfindfiledata->s.push_back(stl_make_pair(stl_string(dir),pfdt));
				pfdt = pfdt->next_sibling;
				if (pfdt==NULL) break;
			}
		}

		if (pfdt)
		{//found it
			wsprintf(path,_T("%s%s%s"),pfindfiledata->startpath.c_str(),dir,pfdt->bin1.filename.c_str());
			pfindfiledata->path = path;
			pfindfiledata->pnextsibfdt = pfdt?pfdt->next_sibling:NULL;
			pfindfiledata->dir = dir;
			return true;
		}
		else
		{
			if (pfindfiledata->s.size())
			{
				pfdt = pfindfiledata->s.front().second;
				wsprintf(dir,_T("%s%s\\"),pfindfiledata->s.front().first.c_str(), pfdt->bin1.filename.c_str());
				pfindfiledata->s.pop_front();
				pfdt=pfdt->first_child;
				if (pfdt->bin2.fileattr&VFS_FILEATTR_FILE)
				{
					wsprintf(path,_T("%s%s%s"),pfindfiledata->startpath.c_str(),dir,pfdt->bin1.filename.c_str());
					pfindfiledata->path = path;
					pfindfiledata->pnextsibfdt = pfdt?pfdt->next_sibling:NULL;
					pfindfiledata->dir = dir;
					return true;
				}
			}
			else
			{
				return false;
			}
		}
	}
	return false;	
}

int CVFileSys::FindClose(FIND_h hfind)
{
	if (!m_bInit) return 0;

	if (hfind==VFS_INVALID_HANDLE_VALUE)
	{
		ERRORLOG(_T("param error!"));
		return 0;
	}

	MAPFIND2FINDFILEDATAITER iter;
	iter = m_mapFind2FindFileData.find(hfind);
	if (iter==m_mapFind2FindFileData.end())
	{
		ERRORLOG(_T("hfind not exist"));
		return 0;
	}
	delete iter->second;
	m_pHandleAlloc->FreeHandle(hfind);
	m_mapFind2FindFileData.erase(iter);
	return 1;
}

ROOT_h CVFileSys::GetFindRoot(FIND_h hfind)
{
	if (hfind==VFS_INVALID_HANDLE_VALUE)
	{
		ERRORLOG(_T("param error!"));
		return NULL;
	}

	MAPFIND2FINDFILEDATAITER iter;
	iter = m_mapFind2FindFileData.find(hfind);
	if (iter==m_mapFind2FindFileData.end())
	{
		return NULL;
	}
	return iter->second->hroot;
}

const TCHAR* CVFileSys::GetFindPathName(FIND_h hfind)
{
	if (hfind==VFS_INVALID_HANDLE_VALUE)
	{
		ERRORLOG(_T("param error!"));
		return NULL;
	}

	MAPFIND2FINDFILEDATAITER iter;
	iter = m_mapFind2FindFileData.find(hfind);
	if (iter==m_mapFind2FindFileData.end())
	{
		return NULL;
	}
	return iter->second->path.c_str();
}

const TCHAR* CVFileSys::GetRelativeFindPathName(FIND_h hfind)
{
	if (hfind==VFS_INVALID_HANDLE_VALUE)
	{
		ERRORLOG(_T("param error!"));
		return NULL;
	}

	MAPFIND2FINDFILEDATAITER iter;
	iter = m_mapFind2FindFileData.find(hfind);
	if (iter==m_mapFind2FindFileData.end())
	{
		return NULL;
	}

	const TCHAR* path = iter->second->path.c_str();
	return &path[iter->second->startpath.length()];
}

int CVFileSys::MakeDir(ROOT_h hroot, const TCHAR* rpath)
{
	if (hroot==VFS_INVALID_HANDLE_VALUE || rpath==NULL)
	{
		ERRORLOG(_T("param error!"));
		return 0;
	}

	MAPROOT2OPENROOTDATAITER iter;
	iter = m_mapRoot2OpenRootData.find(hroot);
	if (iter==m_mapRoot2OpenRootData.end())
	{
		ERRORLOG(_T("root not found"));
		return VFS_INVALID_HANDLE_VALUE;
	}

	if (iter->second->mode==VFS_OPENROOT_MODE_WRITE_EXIST ||
		iter->second->mode==VFS_OPENROOT_MODE_WRITE_ALWAYS ||
		iter->second->mode==VFS_OPENROOT_MODE_WRITE_NEW
		)
	{
		if (NULL!=OpenFileFdt(iter->second,rpath,true))
			return 1;
	}

	return 0;
}


int CVFileSys::GetPathType(vopenrootdata_t* proot, const TCHAR* path)
{
	if (proot==NULL || path==NULL)
	{
		ERRORLOG(_T("param error!"));
		return VFS_PATH_NOTEXIST;
	}

	vfdt_t* pfdt = OpenFileFdt(proot,path,false);

	if (pfdt!=NULL)
	{
		if (pfdt->bin2.fileattr&VFS_FILEATTR_DIR)
		{
			return VFS_PATH_DIRECTORY;
		}
		else if (pfdt->bin2.fileattr&VFS_FILEATTR_FILE)
		{
			return VFS_PATH_FILE;
		}
	}

	return VFS_PATH_NOTEXIST;	
}


int CVFileSys::GetPathType(ROOT_h hroot, const TCHAR* path)
{
	if (hroot==VFS_INVALID_HANDLE_VALUE || path==NULL)
	{
		ERRORLOG(_T("param error!"));
		return 0;
	}

	MAPROOT2OPENROOTDATAITER iter;
	iter = m_mapRoot2OpenRootData.find(hroot);
	if (iter==m_mapRoot2OpenRootData.end())
	{
		return VFS_PATH_NOTEXIST;
	}

	TCHAR fixedpath[MAX_PATH] = {_T('\0')};
	FixPathName(path,fixedpath);
	path = fixedpath;

	return GetPathType(iter->second,path);
}

bool CVFileSys::IsBuildinFileMapper()
{
	return m_bUseBuildinFileMapper;
}

bool CVFileSys::IsBuildinHandleAlloc()
{
	return m_bUseBuildinHandleAlloc;
}

vfdt_t* CVFileSys::OpenFileFdt(vopenrootdata_t * proot, const TCHAR* path, bool createable)
{
	if (proot==NULL || path==NULL)
	{
		ERRORLOG(_T("param error!"));
		return NULL;
	}

	TCHAR fname[MAX_PATH] = {_T('\0')};

	vfs_t* pvfs = proot->vfs;

	if (pvfs==NULL) return NULL;

	int pos;
	const TCHAR* pcur = path;

	stl_list< stl_pair<stl_string,vfdt_t*> > s;

	TCHAR curpath[MAX_PATH] = {_T('\0')};

	vfdt_t* pcurfdt = NULL;
	
	if (pvfs->fdt_list.size())
		pcurfdt = *pvfs->fdt_list.begin();

	vfdt_t* parent = NULL;
	vfdt_t* prev_sibling = NULL;

	bool bdir;

	for(;;)
	{
		if (*pcur==0) break;
		
		while(*pcur==_T('\\')) pcur++;

		pos = 0;
		while(*pcur!=_T('\\') && *pcur!=_T('\0')) fname[pos++] = *pcur++;

	#	if defined(VFILENAME_MAXLEN)
		if (pos>VFILENAME_MAXLEN)
		{
			ERRORLOG2(_T("path name exceed [%d] characters."),path,VFILENAME_MAXLEN);
			return NULL;
		}
	#	endif

		fname[pos] = _T('\0');

		if (*pcur==0) bdir=false;
		else bdir=true;

		while(pcurfdt)
		{
			int ret = -1;
			if (IsCaseSense())
			{
				ret = _tcscmp(pcurfdt->bin1.filename.c_str(),fname);
			}
			else
			{
				ret = _tcsicmp(pcurfdt->bin1.filename.c_str(),fname);
			}

			if (!ret)
			{
				if (bdir && (pcurfdt->bin2.fileattr&VFS_FILEATTR_DIR))
				{
					_tcscat(curpath,pcurfdt->bin1.filename.c_str());
					_tcscat(curpath,_T("\\"));
					parent = pcurfdt;
					prev_sibling = NULL;
					pcurfdt = pcurfdt->first_child;
					break;
				}

				if ( bdir != (0!=(pcurfdt->bin2.fileattr&VFS_FILEATTR_DIR)) )
				{//error 
					return NULL;
				}

				if (*pcur==0) return pcurfdt;
			}
			prev_sibling=pcurfdt;
			pcurfdt=pcurfdt->next_sibling;
		}

		if (pcurfdt==NULL)
		{
			if (createable)
			{
				pcurfdt = new vfdt_t();
				if (pcurfdt==NULL)
				{
					return NULL;
				}
				pcurfdt->bin1.filename = fname;
				pcurfdt->bin2.fileattr = bdir?VFS_FILEATTR_DIR:VFS_FILEATTR_FILE;
				pcurfdt->parent = parent;
				pcurfdt->prev_sibling = prev_sibling;
				pcurfdt->first_child = NULL;
				pcurfdt->next_sibling = NULL;
				if (prev_sibling)
				{
					prev_sibling->next_sibling=pcurfdt;
				}
				else
				{
					if (parent) parent->first_child=pcurfdt;
				}
				if (bdir) pcurfdt->bin2.fileattr = VFS_FILEATTR_DIR;
				else pcurfdt->bin2.fileattr = VFS_FILEATTR_FILE;

				pvfs->fdt_list.push_back(pcurfdt);

				if (*pcur==_T('\0')) return pcurfdt;	
				
				parent = pcurfdt;
				prev_sibling = NULL;
				pcurfdt = NULL;
			}
			else
			{
				return NULL;
			}
		}
	}
	return NULL;
}

int CVFileSys::RegisterRWFilter(const TCHAR* ext, IRWFilter* prwfilter)
{
	if (ext==NULL || ext[0]==0) 
	{
		m_pDefaultRWFilter = prwfilter;
	}
	else
	{
		m_mapExt2RWFilter[ext]=prwfilter;
	}
	return 1;
}

IRWFilter* CVFileSys::GetFileRWFilter(const TCHAR* path)
{
	if (path==NULL)
	{
		return m_pDefaultRWFilter;
	}

	TCHAR extname[MAX_PATH] = {_T('\0')};
	GetPathExtName(path,extname);

	MAPEXT2RWFILTERITER iter = m_mapExt2RWFilter.find(extname);
	if (iter==m_mapExt2RWFilter.end())
	{
		if (extname[0]!=_T('?'))
		{
			return m_pDefaultRWFilter;
		}
		else
		{
			return NULL;
		}
	}

	return iter->second;	
}



IFileSys* ICreateVFileSys(IFileSysMgr* pFileSysMgr, ILogSys* pLogSys)
{
    s_pLogSys = pLogSys;

    IFileSys* pfilesys = static_cast<IFileSys*>(new CVFileSys());
    
    if (pfilesys==NULL)
    {
        ERRORLOG(_T("vfs inerface create error"));
        return NULL;        
    }
    
    if (pfilesys->Init(pFileSysMgr)==0)
    {
        ERRORLOG(_T("vfs inerface init failed"));
        pfilesys->Release();
        return NULL;
    }

    return pfilesys;
}




NAMESPACEEND