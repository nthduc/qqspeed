
#include "FileSys.h"

NAMESPACEBEGIN

#if !defined(VFS_PORTABLE)
	DEFINE_FACTORY_METHOD(CFileSys);
#endif

    
static ILogSys* s_pLogSys = NULL;

#define ERRORLOG(msg) if (s_pLogSys) s_pLogSys->printf(_T("%s(%d) : error %s\n"),__FILE__,__LINE__,msg);
#define ERRORLOG1(msg,p1) if (s_pLogSys) s_pLogSys->printf(_T("%s(%d) : error "##msg"\n"),__FILE__,__LINE__,p1);
#define ERRORLOG2(msg,p1,p2) if (s_pLogSys) s_pLogSys->printf(_T("%s(%d) : error "##msg"\n"),__FILE__,__LINE__,p1,p2);
#define ERRORLOG3(msg,p1,p2,p3) if (s_pLogSys) s_pLogSys->printf(_T("%s(%d) : error "##msg"\n"),__FILE__,__LINE__,p1,p2,p3);


CFileSys::CFileSys() 
	: m_pFileMapper(NULL), m_bInit(false), m_pHandleAlloc(NULL),  m_pFileSysMgr(NULL),
	  m_pDefaultRWFilter(NULL), m_pBufferCache(NULL), m_bCaseSense(false)
{
}

 
CFileSys::~CFileSys()
{
    MAPVROOT2ROOTDATAITER iter2;
	for (iter2=m_mapVRoot2RootData.begin(); iter2!=m_mapVRoot2RootData.end(); ++iter2)
	{
		delete iter2->second;
	}
	m_mapVRoot2RootData.clear();

	MAPROOT2OPENROOTDATAITER iter3;
	for (iter3=m_mapRoot2OpenRootData.begin(); iter3!=m_mapRoot2OpenRootData.end(); ++iter3)
	{
		CloseRoot(iter3->first);
		delete iter3->second;
	}
	m_mapRoot2OpenRootData.clear();

	MAPFIND2FINDFILEDATAITER iter5;
	for (iter5=m_mapFind2FindFileData.begin(); iter5!=m_mapFind2FindFileData.end(); ++iter5)
	{
		delete iter5->second;
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


void CFileSys::Release()
{
#if defined(VFS_PORTABLE)
	delete this;
#else
	Free();
#endif
}
 

int CFileSys::Init(IFileSysMgr* filesysmgr)
{
    m_pFileSysMgr = filesysmgr;

    IHandleAlloc* pHandleAlloc;
	IFileMapper* pFileMapper;
	IBufferCache* pBufferCache;

    if (filesysmgr!=NULL) 
    {
        pHandleAlloc = filesysmgr->GetHandleAlloc();
		pFileMapper = filesysmgr->GetFSFileMapper();
		pBufferCache = filesysmgr->GetFSBufferCache();
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
			pHandleAlloc->Release();
			pFileMapper->Release();
			return 0;
		}

		m_bUseBuildinFileMapper = true;
        m_bUseBuildinHandleAlloc = true;
		m_bUseBuildinBufferCache = true;
	}

	m_pFileMapper = pFileMapper;
	m_pHandleAlloc = pHandleAlloc;
	m_pBufferCache = pBufferCache;

	m_bInit = true;

    return 1;
}


VROOT_h CFileSys::MountRoot(const TCHAR* rootdir, const TCHAR* vdir)
{
    VROOT_h hroot = VFS_INVALID_HANDLE_VALUE;
    
    if (!m_bInit) return hroot;

	if (rootdir==NULL || vdir==NULL)
	{
		ERRORLOG(_T("param error!"));
		return hroot;
	}
    
    TCHAR fixedrootdir[MAX_PATH];
    TCHAR fixedvdir[MAX_PATH];

	GetFullPathName(rootdir,MAX_PATH,fixedrootdir,NULL);
    
    FixDirName(fixedrootdir);
    FixDirName(vdir,fixedvdir);

	if (!IsCaseSense())
	{
		StringToLower(fixedrootdir);
		StringToLower(fixedvdir);
	}
    
    rootdata_t* proot = new rootdata_t(fixedrootdir,fixedvdir);
    if (proot==NULL) return hroot;
    hroot = m_pHandleAlloc->AllocHandle();
    m_mapVRoot2RootData[hroot] = proot;

    return hroot;
}   


void CFileSys::UnMountAll()
{
	if (!m_bInit) return;

	MAPVROOT2ROOTDATAITER iter;
	for (iter=m_mapVRoot2RootData.begin(); iter!=m_mapVRoot2RootData.end();++iter)
	{
		delete iter->second;
		m_pHandleAlloc->FreeHandle(iter->first);
	}
	m_mapVRoot2RootData.clear();
} 


VROOT_h CFileSys::GetRealFilePath(const TCHAR* filepath, TCHAR* realfilepath)
{
	if (filepath==NULL || realfilepath==NULL)
	{
		ERRORLOG(_T("param error!"));
		return VFS_INVALID_HANDLE_VALUE;
	}

    MAPVROOT2ROOTDATAITER iter;
    for (iter=m_mapVRoot2RootData.begin(); iter!=m_mapVRoot2RootData.end();++iter)
    {
        if (iter->second->vdir.length())
        {
            if (!_tcsncmp(filepath,iter->second->vdir.c_str(),iter->second->vdir.length()))
            {
                _tcscpy(realfilepath,iter->second->dir.c_str());
                _tcscat(realfilepath,&filepath[iter->second->vdir.length()]);
                if (VFSNAME::GetPathType(realfilepath)==VFS_PATH_FILE)
				{
					return iter->first;
				}
            }
        }
        else
        {
            _tcscpy(realfilepath,iter->second->dir.c_str());
            _tcscat(realfilepath,filepath);
            if (VFSNAME::GetPathType(realfilepath)==VFS_PATH_FILE)
			{
				return iter->first;
			}
        }
    }
    
    return VFS_INVALID_HANDLE_VALUE;
}


FILE_h CFileSys::OpenFile(const TCHAR* filepath, int tag)
{
	if (!m_bInit) return VFS_INVALID_HANDLE_VALUE;

	if (filepath==NULL)
	{
		ERRORLOG(_T("param error!"));
		return VFS_INVALID_HANDLE_VALUE;
	}

    TCHAR fixedfilepath[MAX_PATH];
    TCHAR realfilepath[MAX_PATH];
    
    FILE_h hfile = VFS_INVALID_HANDLE_VALUE;

    if (!m_bInit) return hfile;

    if (0==(tag&VFS_OF_TAG_STANDARDPATHNAME))
    {
        FixPathName(filepath,fixedfilepath);
    }
	else
	{
		_tcscpy(fixedfilepath, filepath);
	}

	if (!IsCaseSense())
	{
		StringToLower(fixedfilepath);
	}


    VROOT_h hroot;
	long lparam;
    hroot = m_pFileMapper->FindFile(fixedfilepath,&lparam);
    if (hroot!=VFS_INVALID_HANDLE_VALUE)
    {
        return OpenFile(hroot,fixedfilepath,lparam,tag|VFS_OF_TAG_STANDARDPATHNAME);
    }

    hroot = GetRealFilePath(fixedfilepath,realfilepath);
    if (hroot==VFS_INVALID_HANDLE_VALUE)
    {
        ERRORLOG1(_T("vfs [%s] not found"),fixedfilepath);
        return hfile;
    }

    MAPVROOT2ROOTDATAITER iter;
    iter = m_mapVRoot2RootData.find(hroot);
    if (iter==m_mapVRoot2RootData.end())
    {
		ERRORLOG(_T("root not exist"));
        return hfile;
    } 

    IBuffer *pbuffer = OpenFileIndirect(iter->second,realfilepath,tag);

	if (pbuffer==NULL) 
	{
		return VFS_INVALID_HANDLE_VALUE;
	}

	return m_pBufferCache->AddCache(fixedfilepath,hroot,0,pbuffer);
}


FILE_h CFileSys::OpenFile(VROOT_h hroot, const TCHAR* vpath, long lparam, int tag)
{
	if (!m_bInit) return VFS_INVALID_HANDLE_VALUE;

	if (hroot==VFS_INVALID_HANDLE_VALUE || vpath==NULL)
	{
		ERRORLOG(_T("param error!"));
		return VFS_INVALID_HANDLE_VALUE;
	}

	TCHAR realfilepath[MAX_PATH];
	TCHAR fixedvpath[MAX_PATH];

	if (hroot==VFS_INVALID_HANDLE_VALUE) return hroot;

	if (hroot==m_pBufferCache->GetHandle())
	{
		m_pBufferCache->IncRefCount(lparam);
		return lparam;
	}

    MAPVROOT2ROOTDATAITER iter;
    iter = m_mapVRoot2RootData.find(hroot);

    if (iter==m_mapVRoot2RootData.end())
    {
		ERRORLOG(_T("root not exist"));
        return VFS_INVALID_HANDLE_VALUE;
    } 

	rootdata_t* prootdata = iter->second;

	if (0==(tag&VFS_OF_TAG_STANDARDPATHNAME))
	{
		FixPathName(vpath,fixedvpath);
	}
	else
	{
		_tcscpy(fixedvpath,vpath);
	}

	if (!IsCaseSense())
	{
		StringToLower(fixedvpath);
	}

	if (_tcsncmp(prootdata->vdir.c_str(),fixedvpath,prootdata->vdir.length()))
	{
		ERRORLOG2(_T("文件[%s]不在虚路径[%s]上!"),fixedvpath,prootdata->vdir.c_str());
		return VFS_INVALID_HANDLE_VALUE;
	}

	_tcscpy(realfilepath,prootdata->dir.c_str());
	_tcscat(realfilepath,&fixedvpath[prootdata->vdir.length()]);
	
	IBuffer* pbuffer = OpenFileIndirect(iter->second,realfilepath,tag);
	if (pbuffer==NULL)
	{
		return VFS_INVALID_HANDLE_VALUE;
	}

	return m_pBufferCache->AddCache(fixedvpath,hroot,lparam,pbuffer);
}
 

IBuffer* CFileSys::OpenFileIndirect(rootdata_t* prootdata, const TCHAR* realfilepath, int tag)
{
	if (prootdata==NULL || realfilepath==NULL)
	{
		ERRORLOG(_T("param error!"));
		return NULL;
	}

    HANDLE hWinFile = ::CreateFile(
            realfilepath,
            GENERIC_READ,
            FILE_SHARE_READ, 
            NULL,
            OPEN_EXISTING,
			FILE_ATTRIBUTE_NORMAL,
			NULL
        );
    
    if (hWinFile==INVALID_HANDLE_VALUE)
    {
        stl_string error;
        error = _T("文件");
        error += realfilepath;
        error += _T("打开错误!");
        ERRORLOG(error.c_str());
		return NULL;
    }
    
    DWORD size = ::GetFileSize(hWinFile,NULL);
    
    IBuffer* pbuffer = ICreateBuffer(size);
    
    DWORD numtoread;
    if (!::ReadFile(hWinFile,pbuffer->GetBuffer(),pbuffer->GetUsedSize(),&numtoread,NULL)
		|| numtoread!=pbuffer->GetUsedSize()
		)
    {
        stl_string error;
        error = _T("file ");
        error += realfilepath;
        error += _T(" read error");
        ERRORLOG(error.c_str());
        pbuffer->Release();
        ::CloseHandle(hWinFile);
		return NULL;
    }
    
    ::CloseHandle(hWinFile);
	
	return pbuffer;
}    


int CFileSys::CloseFile(FILE_h hfile)
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


IBuffer* CFileSys::GetFileBuffer(FILE_h hfile)
{
	if (NULL==m_pBufferCache) return NULL;

	if (hfile==VFS_INVALID_HANDLE_VALUE)
	{
		ERRORLOG(_T("param error!"));
		return NULL;
	}

	return m_pBufferCache->GetBuffer(hfile);
}

int CFileSys::SetCacheSize(int cachesize)
{
	if (NULL==m_pBufferCache) return 0;

	return m_pBufferCache->SetCacheSize(cachesize);
}

ROOT_h CFileSys::OpenRoot(const TCHAR* rootdir, int mode)
{
	if (!m_bInit) return VFS_INVALID_HANDLE_VALUE;

	if (rootdir==NULL)
	{
		ERRORLOG(_T("param error!"));
		return VFS_INVALID_HANDLE_VALUE;
	}

	TCHAR fixedrootdir[MAX_PATH];

	GetFullPathName(rootdir,MAX_PATH,fixedrootdir,NULL);

	FixDirName(fixedrootdir);

	if (!IsCaseSense())
	{
		StringToLower(fixedrootdir);
	}

	openrootdata_t* prootdata = new openrootdata_t(fixedrootdir,mode);
	ROOT_h hroot = m_pHandleAlloc->AllocHandle();
	m_mapRoot2OpenRootData[hroot] = prootdata;
	return hroot;
}


int CFileSys::CloseRoot(ROOT_h hroot)
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
		ERRORLOG(_T("root not exist"));
		return 0;
	}

	openrootdata_t* prootdata = iter->second;

	m_pHandleAlloc->FreeHandle(iter->first);

	m_mapRoot2OpenRootData.erase(iter);

	if (prootdata) delete prootdata;

	return 1;
}

int CFileSys::DelFile(ROOT_h htrg, const TCHAR* trgpath)
{
	if (!m_bInit) return 0;

	if (trgpath==NULL || htrg==VFS_INVALID_HANDLE_VALUE)
	{
		ERRORLOG(_T("param error!"));
		return 0;
	}

	MAPROOT2OPENROOTDATAITER iter;
	iter = m_mapRoot2OpenRootData.find(htrg);
	if (iter==m_mapRoot2OpenRootData.end())
	{
		ERRORLOG(_T("src root not found"));
		return 0;
	}

	openrootdata_t* proot = iter->second;

	if (proot->mode==VFS_OPENROOT_MODE_READ)
	{
		ERRORLOG(_T("no right to del file"));
		return 0;
	}

	TCHAR fixedpath[MAX_PATH];
	_tcscpy(fixedpath,proot->dir.c_str());
	_tcscat(fixedpath,trgpath);

	FixPathName(fixedpath);

	return (0==::remove(fixedpath))?1:0;
}

int CFileSys::DelTree(ROOT_h htrg, const TCHAR* startpath)
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
		ERRORLOG(_T("src root not found"));
		return 0;
	}

	openrootdata_t* proot = iter->second;

	if (proot->mode==VFS_OPENROOT_MODE_READ)
	{
		ERRORLOG(_T("no right to del file"));
		return 0;
	}

	TCHAR fixedpath[MAX_PATH];
	TCHAR dirpath[MAX_PATH];
	_tcscpy(fixedpath,proot->dir.c_str());
	_tcscat(fixedpath,startpath);
	FixDirName(fixedpath);
	_tcscpy(dirpath,fixedpath);
	
	char path[MAX_PATH];
	wsprintf(path,_T("%s*.*"),fixedpath);
	WIN32_FIND_DATA find;
	HANDLE handle = ::FindFirstFile(path,&find);
	stl_list<stl_string> subdir;
	stl_list<stl_string> dirdellist;
	if (handle!=INVALID_HANDLE_VALUE)
	{
		BOOL bfind ;
		do 
		{
			if (find.dwFileAttributes&FILE_ATTRIBUTE_DIRECTORY)
			{
				if (find.cFileName[0]!='.')
				{
					wsprintf(path,_T("%s%s\\"),fixedpath,find.cFileName);
					subdir.push_back(path);
				}
			}
			else
			{
				wsprintf(path,_T("%s%s"),fixedpath,find.cFileName);
				if (0!=::remove(path))
				{
					ERRORLOG(_T("del file error"));
					::FindClose(handle);
					return 0;
				}
			}

			bfind = ::FindNextFile(handle,&find);

			while (bfind==FALSE && subdir.size())
			{
				_tcscpy(fixedpath,subdir.back().c_str());
				dirdellist.push_front(fixedpath);
				wsprintf(path,_T("%s*.*"),fixedpath);
				subdir.pop_back();
				::FindClose(handle);
				handle = ::FindFirstFile(path,&find);
				if (handle==INVALID_HANDLE_VALUE)
				{
					continue;
				}
				bfind=TRUE;
				break;
			}
		} while(bfind);

		::FindClose(handle);

		for (stl_list<stl_string>::iterator iter=dirdellist.begin();
		     iter!=dirdellist.end(); ++iter
		   )
		 {
			 const TCHAR* dir = (*iter).c_str();
			 if (0!=::_trmdir(dir))
			 {
				 return 0;
			 }
		
		 }
	}

	return (0==::_trmdir(dirpath))?1:0;
}


FIND_h CFileSys::FindFirstFile(ROOT_h hroot, const TCHAR* startpath, bool recursive)
{ 
	if (!m_bInit) return 0;

	if (hroot==VFS_INVALID_HANDLE_VALUE || startpath==NULL)
	{
		ERRORLOG(_T("param error!"));
		return VFS_INVALID_HANDLE_VALUE;
	}

	MAPROOT2OPENROOTDATAITER iter;
	iter = m_mapRoot2OpenRootData.find(hroot);
	if (iter==m_mapRoot2OpenRootData.end())
	{
		ERRORLOG(_T("root not exist"));
		return VFS_INVALID_HANDLE_VALUE;
	}

	FIND_h findresult = VFS_INVALID_HANDLE_VALUE;

	openrootdata_t* proot = iter->second;

	TCHAR path[MAX_PATH];
	TCHAR parentpath[MAX_PATH];
	TCHAR _startpath[MAX_PATH];

	_tcscpy(_startpath,proot->dir.c_str());
	_tcscat(_startpath,startpath);
	FixDirName(_startpath);

	int prefixpathlen = _tcslen(_startpath);
	parentpath[0] = 0;
	wsprintf(path,_T("%s*.*"),_startpath);

	WIN32_FIND_DATA finddata;
	
	BOOL bFind;
	
	findfiledata_t* pfinddata = new findfiledata_t();	

	for(;;)
	{
		HANDLE hfind = ::FindFirstFile(path,&finddata);

		if (hfind==INVALID_HANDLE_VALUE)
		{
			delete pfinddata;
			return VFS_INVALID_HANDLE_VALUE;
		}

		bFind = TRUE;

		while(finddata.dwFileAttributes&FILE_ATTRIBUTE_DIRECTORY)
		{
			if (finddata.cFileName[0]!='.') 
			{
				wsprintf(path,_T("%s%s%s\\"),_startpath,parentpath,finddata.cFileName);
			    pfinddata->s.push_back(&path[prefixpathlen]);
			}
			bFind=::FindNextFile(hfind,&finddata);
			if (!bFind) break;
		}

		if (bFind)
		{
			pfinddata->hroot = hroot;
			pfinddata->proot = proot;
			pfinddata->recursive = recursive;
			pfinddata->hfind = hfind;
			pfinddata->startpath = _startpath;
			pfinddata->parentpath = parentpath;
			pfinddata->path = _startpath;
			pfinddata->path += parentpath;
			pfinddata->path += finddata.cFileName;
			findresult=m_pHandleAlloc->AllocHandle();
			m_mapFind2FindFileData[findresult]=pfinddata;
			return findresult;
		}

		::FindClose(hfind);

		if (recursive)
		{	
			if (pfinddata->s.size()!=0)
			{
				_tcscpy(parentpath,pfinddata->s.front().c_str());
				wsprintf(path,_T("%s%s*.*"),_startpath,parentpath);
				pfinddata->s.pop_front();
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
	
	return VFS_INVALID_HANDLE_VALUE;
}


bool CFileSys::FindNextFile(FIND_h hfind)
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

	findfiledata_t* pfindfiledata = iter->second;

	BOOL bFind;

	TCHAR parentpath[MAX_PATH];
	TCHAR path[MAX_PATH];
	
	const TCHAR* startpath = pfindfiledata->startpath.c_str();

	_tcscpy(parentpath,pfindfiledata->parentpath.c_str());
	wsprintf(path,_T("%s%s*.*"),startpath,parentpath);

	WIN32_FIND_DATA finddata;

	int prefixlen = pfindfiledata->startpath.length();

	for(;;)
	{
		bFind = ::FindNextFile(pfindfiledata->hfind,&finddata);

		if (bFind)
		{
			while(finddata.dwFileAttributes&FILE_ATTRIBUTE_DIRECTORY)
			{
				if (finddata.cFileName[0]!='.') 
				{
					wsprintf(path,_T("%s%s%s\\"),startpath,parentpath,finddata.cFileName);
					pfindfiledata->s.push_back(&path[prefixlen]);
				}
				bFind=::FindNextFile(pfindfiledata->hfind,&finddata);
				if (!bFind) break;
			}
		}

		if (bFind)
		{
			pfindfiledata->parentpath = parentpath;
			pfindfiledata->path = startpath;
			pfindfiledata->path += parentpath;
			pfindfiledata->path += finddata.cFileName;
			return true;
		}

		::FindClose(pfindfiledata->hfind);

		if (pfindfiledata->recursive)
		{	
			if (pfindfiledata->s.size()!=0)
			{
				_tcscpy(parentpath,pfindfiledata->s.front().c_str());
				wsprintf(path,_T("%s%s*.*"),startpath,parentpath);
				pfindfiledata->s.pop_front();
				
				pfindfiledata->hfind = ::FindFirstFile(path,&finddata);

				if (pfindfiledata->hfind==INVALID_HANDLE_VALUE)
				{
					return false;
				}

				if (0==(finddata.dwFileAttributes&FILE_ATTRIBUTE_DIRECTORY))
				{
					pfindfiledata->parentpath = parentpath;
					pfindfiledata->path = startpath;
					pfindfiledata->path += parentpath;
					pfindfiledata->path += finddata.cFileName;
					return true;
				}
			}
			else
			{
				return false;
			}
		}
		else
		{
			return false;
		}
	}
	return false;
}


int CFileSys::FindClose(FIND_h hfind)
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
		return 0;
	}
	delete iter->second;
	m_pHandleAlloc->FreeHandle(hfind);
	m_mapFind2FindFileData.erase(iter);
	return 1;
}

int CFileSys::MakeDir(ROOT_h hroot, const TCHAR* rpath)
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
		return 0;
	}

	if (iter->second->mode==VFS_OPENROOT_MODE_WRITE_EXIST ||
		iter->second->mode==VFS_OPENROOT_MODE_WRITE_ALWAYS ||
		iter->second->mode==VFS_OPENROOT_MODE_WRITE_NEW
		)
	{
		if (VFSNAME::MkDirEx(rpath))
			return 1;
	}

	return 0;
}

ROOT_h CFileSys::GetFindRoot(FIND_h hfind)
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
		return 0;
	}

	return iter->second->hroot;
}

const char* CFileSys::GetFindPathName(FIND_h hfind)
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
		return 0;
	}

	const TCHAR* findpath = iter->second->path.c_str();

	openrootdata_t* proot = iter->second->proot;

	if (proot==NULL)
	{
		ERRORLOG("find root invalid");
		return 0;
	}

	int prefixlen = proot->dir.length();

	return &findpath[prefixlen];
}

const char* CFileSys::GetRelativeFindPathName(FIND_h hfind)
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
		return 0;
	}

	const TCHAR* findpath = iter->second->path.c_str();

	int prefixlen = iter->second->startpath.length();

	return &findpath[prefixlen];
}
/*
int CFileSys::CopyFile(FIND_h hfind, ROOT_h htrg, const TCHAR* trgpath)
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
		return 0;
	}

	findfiledata_t* psrcfinddata = iter1->second;

	MAPROOT2OPENROOTDATAITER iter2;
	iter2 = m_mapRoot2OpenRootData.find(htrg);
	if (iter2==m_mapRoot2OpenRootData.end())
	{
		ERRORLOG(_T("target root not found"));
		return 0;
	}

	openrootdata_t * ptrgrootdata = iter2->second;

	if (ptrgrootdata->mode!=VFS_OPENROOT_MODE_WRITE_EXIST &&
		ptrgrootdata->mode!=VFS_OPENROOT_MODE_WRITE_ALWAYS &&
		ptrgrootdata->mode!=VFS_OPENROOT_MODE_WRITE_NEW
	)
	{
		ERRORLOG(_T("target path couldn't write"));
		return 0;
	}

	TCHAR _srcpath[MAX_PATH];
	TCHAR _trgpath[MAX_PATH];
	_tcscpy(_trgpath,ptrgrootdata->dir.c_str());
	_tcscat(_srcpath,psrcfinddata->path.c_str());
	_tcscat(_trgpath,trgpath);
	FixPathName(_trgpath);
	
	VFSNAME::MkDirEx(_trgpath);

	SetFileAttributes(_trgpath,FILE_ATTRIBUTE_ARCHIVE);
	if (::CopyFile(_srcpath,_trgpath,FALSE)) return 1;
	return 0;	
}
*/

int CFileSys::CopyFile(ROOT_h hsrc, const TCHAR* srcpath, ROOT_h htrg, const TCHAR* trgpath)
{
	if (!m_bInit) return 0;

	if (hsrc==VFS_INVALID_HANDLE_VALUE || srcpath==NULL || htrg==VFS_INVALID_HANDLE_VALUE || trgpath==NULL)
	{
		ERRORLOG(_T("param error!"));
		return 0;
	}

	MAPROOT2OPENROOTDATAITER iter;
	openrootdata_t *psrcrootdata, *ptrgrootdata;
	iter = m_mapRoot2OpenRootData.find(hsrc);
	if (iter==m_mapRoot2OpenRootData.end())
	{
		ERRORLOG(_T("source root not found"));
		return 0;
	}

	psrcrootdata = iter->second;
	iter = m_mapRoot2OpenRootData.find(htrg);
	if (iter==m_mapRoot2OpenRootData.end())
	{
		ERRORLOG(_T("target root not found"));
		return 0;
	}

	ptrgrootdata = iter->second;
	if (ptrgrootdata->mode!=VFS_OPENROOT_MODE_WRITE_EXIST &&
		ptrgrootdata->mode!=VFS_OPENROOT_MODE_WRITE_ALWAYS &&
		ptrgrootdata->mode!=VFS_OPENROOT_MODE_WRITE_NEW
	)
	{
		ERRORLOG(_T("target path couldn't write"));
		return 0;
	}

	TCHAR fixedsrcpath[MAX_PATH];
	TCHAR fixedtrgpath[MAX_PATH];
	
	if (psrcrootdata->dir.length() || ptrgrootdata->dir.length())
	{
		_tcscpy(fixedsrcpath,psrcrootdata->dir.c_str());
		_tcscpy(fixedtrgpath,ptrgrootdata->dir.c_str());
		_tcscat(fixedsrcpath,srcpath);
		_tcscat(fixedtrgpath,trgpath);
		FixPathName(fixedsrcpath);
		FixPathName(fixedtrgpath);
		srcpath = fixedsrcpath;
		trgpath = fixedtrgpath;
	}
	
	VFSNAME::MkDirEx(trgpath);

	SetFileAttributes(trgpath,FILE_ATTRIBUTE_ARCHIVE);
	if (::CopyFile(srcpath,trgpath,FALSE)) return 1;
	return 0;
}

int CFileSys::MoveFile(ROOT_h hsrc, const TCHAR* srcpath, ROOT_h htrg, const TCHAR* trgpath)
{
	if (!m_bInit) return 0;

	if (hsrc==VFS_INVALID_HANDLE_VALUE || htrg==VFS_INVALID_HANDLE_VALUE || srcpath==NULL || trgpath==NULL)
	{
		ERRORLOG(_T("param error!"));
		return 0;
	}

	MAPROOT2OPENROOTDATAITER iter;
	openrootdata_t *psrcrootdata, *ptrgrootdata;
	iter = m_mapRoot2OpenRootData.find(hsrc);
	if (iter==m_mapRoot2OpenRootData.end())
	{
		ERRORLOG(_T("source root not found"));
		return 0;
	}

	psrcrootdata = iter->second;
	iter = m_mapRoot2OpenRootData.find(htrg);
	if (iter==m_mapRoot2OpenRootData.end())
	{
		ERRORLOG(_T("target root not found"));
		return 0;
	}

	ptrgrootdata = iter->second;
	if (ptrgrootdata->mode!=VFS_OPENROOT_MODE_WRITE_EXIST &&
		ptrgrootdata->mode!=VFS_OPENROOT_MODE_WRITE_ALWAYS &&
		ptrgrootdata->mode!=VFS_OPENROOT_MODE_WRITE_NEW
	)
	{
		ERRORLOG(_T("target path couldn't write"));
		return 0;
	}
	
	TCHAR fixedsrcpath[MAX_PATH];
	TCHAR fixedtrgpath[MAX_PATH];

	if (psrcrootdata->dir.length())
	{
		_tcscpy(fixedsrcpath,psrcrootdata->dir.c_str());
		_tcscat(fixedsrcpath,srcpath);
		FixPathName(fixedsrcpath);
		srcpath = fixedsrcpath;
	}
	
	if (ptrgrootdata->dir.length())
	{
		_tcscpy(fixedtrgpath,ptrgrootdata->dir.c_str());
		_tcscat(fixedtrgpath,trgpath);
		FixPathName(fixedtrgpath);
		trgpath = fixedtrgpath;
	}

	VFSNAME::MkDirEx(trgpath);

	::SetFileAttributes(trgpath,FILE_ATTRIBUTE_ARCHIVE);

	if (psrcrootdata->mode==VFS_OPENROOT_MODE_READ)
	{
		if (::CopyFile(srcpath,trgpath,FALSE)) return 1;
	}
	else
	{
		if (::MoveFile(srcpath,trgpath)) return 1;
	}
	
	return 0;
}

/*
IBuffer* CFileSys::ReadFile(FIND_h hfind)
{
	if (hfind==VFS_INVALID_HANDLE_VALUE)
	{
		ERRORLOG(_T("param error!"));
		return NULL;
	}

	MAPFIND2FINDFILEDATAITER iter1;
	iter1 = m_mapFind2FindFileData.find(hfind);
	if (iter1==m_mapFind2FindFileData.end())
	{
		return NULL;
	}

	findfiledata_t* psrcfinddata = iter1->second;

    HANDLE hWinFile = ::CreateFile(
            psrcfinddata->path.c_str(),
            GENERIC_READ,
            FILE_SHARE_READ, //FILE_SHARE_READ
            NULL,
            OPEN_ALWAYS,
			FILE_ATTRIBUTE_NORMAL,
			NULL
        );
    
    if (hWinFile==INVALID_HANDLE_VALUE)
    {
        ERRORLOG1(_T("file [%s] open error!"),psrcfinddata->path.c_str());
        return NULL;
    }
    
    DWORD size = ::GetFileSize(hWinFile,NULL);

	IBuffer* pbuffer = ICreateBuffer(size);
    
    DWORD numtoread;
    if (!::ReadFile(hWinFile,pbuffer->GetBuffer(),pbuffer->GetUsedSize(),&numtoread,NULL)
		|| numtoread!=pbuffer->GetUsedSize()
		)
    {
        ERRORLOG1(_T("file [%s] read error"),psrcfinddata->path.c_str());
        pbuffer->Release();
        ::CloseHandle(hWinFile);
        return NULL;
    }
    
    ::CloseHandle(hWinFile);

	return pbuffer;
}
*/

IBuffer* CFileSys::ReadFile(ROOT_h hsrc, const TCHAR* srcpath)
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
		ERRORLOG(_T("src root not found"));
		return NULL;
	}

	openrootdata_t* proot = iter->second;

	TCHAR _srcpath[MAX_PATH];
	_tcscpy(_srcpath,proot->dir.c_str());
	_tcscat(_srcpath,srcpath);
	FixPathName(_srcpath);

    HANDLE hWinFile = ::CreateFile(
            _srcpath,
            GENERIC_READ,
            FILE_SHARE_READ, //FILE_SHARE_READ
            NULL,
            OPEN_EXISTING,
			FILE_ATTRIBUTE_NORMAL,
			NULL
        );
    
    if (hWinFile==INVALID_HANDLE_VALUE)
    {
        ERRORLOG1(_T("file [%s] open error!"),_srcpath);
        return NULL;
    }
    
    DWORD size = ::GetFileSize(hWinFile,NULL);

	IBuffer* pbuffer = ICreateBuffer(size);
    
    DWORD numtoread;
    if (!::ReadFile(hWinFile,pbuffer->GetBuffer(),pbuffer->GetUsedSize(),&numtoread,NULL)
		|| numtoread!=pbuffer->GetUsedSize()
		)
    {
        ERRORLOG1(_T("file [%s] read error!"),_srcpath);
		pbuffer->Release();
        ::CloseHandle(hWinFile);
        return NULL;
    }
    
    ::CloseHandle(hWinFile);
	return pbuffer;
}


int CFileSys::WriteFile(ROOT_h htrg, const TCHAR* trgpath, IBuffer* pbuffer)
{
	if (htrg==VFS_INVALID_HANDLE_VALUE || trgpath==NULL || pbuffer==NULL)
	{
		ERRORLOG(_T("param error!"));
		return 0;
	}

	if (pbuffer==NULL || pbuffer->GetBuffer()==NULL || pbuffer->GetUsedSize()<=0)
		return 0;

	MAPROOT2OPENROOTDATAITER iter;
	iter = m_mapRoot2OpenRootData.find(htrg);
	if (iter==m_mapRoot2OpenRootData.end())
	{
		ERRORLOG(_T("src root not found"));
		return 0;
	}

	openrootdata_t* proot = iter->second;

	TCHAR _trgpath[MAX_PATH];
	_tcscpy(_trgpath,proot->dir.c_str());
	_tcscat(_trgpath,trgpath);
	FixPathName(_trgpath);

	VFSNAME::MkDirEx(_trgpath);

    HANDLE hWinFile = ::CreateFile(
            _trgpath,
            GENERIC_WRITE,
            0, //FILE_SHARE_READ
            NULL,
            OPEN_ALWAYS,
			FILE_ATTRIBUTE_NORMAL,
			NULL
        );
    
    if (hWinFile==INVALID_HANDLE_VALUE)
    {
        ERRORLOG1(_T("file [%s] open error!"),_trgpath);
        return 0;
    }
    
    DWORD numwrite;
    if (!::WriteFile(hWinFile,pbuffer->GetBuffer(),pbuffer->GetUsedSize(),&numwrite,NULL)
		|| numwrite!=pbuffer->GetUsedSize()
		)
    {
        ERRORLOG1(_T("file [%s] read error!"),_trgpath);
        ::CloseHandle(hWinFile);
        return 0;
    }
    
    ::CloseHandle(hWinFile);
	return 1;    
}


bool CFileSys::IsBuildinFileMapper()
{
    return m_bUseBuildinFileMapper;
}


bool CFileSys::IsBuildinHandleAlloc()
{
    return m_bUseBuildinHandleAlloc;
}


int CFileSys::GetPathType(ROOT_h hroot, const TCHAR* rpath)
{
	if (hroot==VFS_INVALID_HANDLE_VALUE || rpath==NULL)
	{
		ERRORLOG(_T("param error!"));
		return VFS_PATH_NOTEXIST;
	}

	MAPROOT2OPENROOTDATAITER iter;
	iter = m_mapRoot2OpenRootData.find(hroot);
	if (iter==m_mapRoot2OpenRootData.end())
	{
		return VFS_PATH_NOTEXIST;
	}

	TCHAR realpath[MAX_PATH];
	wsprintf(realpath,_T("%s%s"),iter->second->dir.c_str(),rpath);
	FixPathName(realpath);
	
	return VFSNAME::GetPathType(realpath);
}

int CFileSys::RegisterRWFilter(const TCHAR* ext, IRWFilter* prwfilter)
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


IRWFilter* CFileSys::GetFileRWFilter(const TCHAR* path)
{
	if (path==NULL)
	{
		return m_pDefaultRWFilter;
	}

	TCHAR extname[MAX_PATH];
	GetPathExtName(path,extname);

	MAPEXT2RWFILTERITER iter = m_mapExt2RWFilter.find(extname);
	if (iter==m_mapExt2RWFilter.end())
	{
		return m_pDefaultRWFilter;
	}

	return iter->second;	
}


IFileSys* ICreateGeneralFileSys(IFileSysMgr* pFileSysMgr, ILogSys* pLogSys)
{
    s_pLogSys = pLogSys;

    IFileSys* pfilesys = static_cast<IFileSys*>(new CFileSys());
    
    if (pfilesys==NULL)
    {
        ERRORLOG(_T("general file system interface create error"));
        return NULL;        
    }
    
    if (pfilesys->Init(pFileSysMgr)==0)
    {
        ERRORLOG(_T("general file system interface init failed"));
        pfilesys->Release();
        return NULL;
    }

    return pfilesys;
}

 

NAMESPACEEND