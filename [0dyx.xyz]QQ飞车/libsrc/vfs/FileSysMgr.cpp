
#include "FileSysMgr.h"

NAMESPACEBEGIN

#if !defined(VFS_PORTABLE)
	DEFINE_FACTORY_METHOD(CFileSysMgr);
#endif


static ILogSys* s_pLogSys = NULL;

#define ERRORLOG(msg) if (s_pLogSys) s_pLogSys->printf(_T("%s(%d) : error %s\n"),__FILE__,__LINE__,msg);
#define ERRORLOG1(msg,p1) if (s_pLogSys) s_pLogSys->printf(_T("%s(%d) : error "##msg"\n"),__FILE__,__LINE__,p1);
#define ERRORLOG2(msg,p1,p2) if (s_pLogSys) s_pLogSys->printf(_T("%s(%d) : error "##msg"\n"),__FILE__,__LINE__,p1,p2);
#define ERRORLOG3(msg,p1,p2,p3) if (s_pLogSys) s_pLogSys->printf(_T("%s(%d) : error "##msg"\n"),__FILE__,__LINE__,p1,p2,p3);

    
CFileSysMgr::CFileSysMgr()
{
    m_pGeneralFileSys = NULL;
    m_pHandleAlloc = NULL;
    m_pVFSFileMapper = NULL;
	m_pFSFileMapper = NULL;
	m_pVFSBufferCache = NULL;
	m_pFSBufferCache = NULL;
    m_bVFSFirst = false;
	m_bCaseSense = false;
}   
 
CFileSysMgr::~CFileSysMgr()
{	
	IFileSys* pfilesys = GetVFileSysByExt(_T("vfs")); 
	if (pfilesys) pfilesys->Release();

	if (m_pGeneralFileSys)
	{
		m_pGeneralFileSys->Release();
	}

	if (m_pVFSBufferCache)
	{
		m_pVFSBufferCache->Release();
	}

	if (m_pFSBufferCache)
	{
		m_pFSBufferCache->Release();
	}

	if (m_pVFSFileMapper)
	{
		m_pVFSFileMapper->Release();
	}

	if (m_pFSFileMapper)
	{
		m_pFSFileMapper->Release();
	}

	if (m_pHandleAlloc)
	{
		m_pHandleAlloc->Release();
	}

}    

int CFileSysMgr::Init()
{
    do
    {
        m_pHandleAlloc = ICreateHandleAlloc(s_pLogSys);
        if (m_pHandleAlloc==NULL) break;

        m_pVFSFileMapper = ICreateFileMapper(s_pLogSys);
        if (m_pVFSFileMapper==NULL) break;

		m_pFSFileMapper = ICreateFileMapper(s_pLogSys);
		if (m_pFSFileMapper==NULL) break;

		m_pFSBufferCache = ICreateBufferCache(m_pHandleAlloc,m_pFSFileMapper,0);
		if (m_pFSBufferCache==NULL) break;

		m_pVFSBufferCache = ICreateBufferCache(m_pHandleAlloc,m_pVFSFileMapper,0);
		if (m_pVFSBufferCache==NULL) break;
        
        m_pGeneralFileSys = ICreateGeneralFileSys(this,s_pLogSys);
        if (m_pGeneralFileSys==NULL) break;

		IFileSys* pfilesys = ICreateVFileSys(this,s_pLogSys);
		if (pfilesys==NULL) break;

		RegisterFileSys(pfilesys,_T("vfs"));

        m_bVFSFirst = false;
    
        return 1;
        
    }while(0);
    
    if (m_pHandleAlloc)
    {
        m_pHandleAlloc->Release();
        m_pHandleAlloc=NULL;
    }
    
    if (m_pGeneralFileSys)
    {
        m_pGeneralFileSys->Release();
        m_pGeneralFileSys=NULL;
    }
    
    if (m_pVFSFileMapper)
    {
        m_pVFSFileMapper->Release();
        m_pVFSFileMapper=NULL;
    }

	if (m_pFSFileMapper)
	{
		m_pFSFileMapper->Release();
		m_pFSFileMapper=NULL;
	}

	if (m_pFSBufferCache)
	{
		m_pFSBufferCache->Release();
		m_pFSBufferCache=NULL;
	}

	if (m_pVFSBufferCache)
	{
		m_pVFSBufferCache->Release();
		m_pVFSBufferCache=NULL;
	}

    return 0;
}

void CFileSysMgr::Release()
{
#if defined(VFS_PORTABLE)
	delete this;
#else
	Free();
#endif
}    

int CFileSysMgr::RegisterFileSys(IFileSys* filesys, const TCHAR* rootext)
{
	if (filesys==NULL || rootext==NULL) 
	{
		ERRORLOG(_T("param error!"));
		return 0;
	}

    m_mapExtName2FileSys[rootext] = filesys;
    
    VECFILESYSITER iter;
    for (iter=m_vecVFSFileSys.begin(); iter!=m_vecVFSFileSys.end(); ++iter)
    {
        if (*iter==filesys) return 1;
    }
    m_vecVFSFileSys.push_back(filesys);
    return 1;
}   

int CFileSysMgr::UnRegisterFileSys(IFileSys* filesys)
{
	if (filesys==NULL)
	{
		ERRORLOG(_T("param error!"));
		return 0;
	}

    MAPEXTNAME2FILESYSITER iter;
    for (iter=m_mapExtName2FileSys.begin(); iter!=m_mapExtName2FileSys.end(); )
    {
        if (iter->second==filesys)
        {    
            iter = m_mapExtName2FileSys.erase(iter);
        }    
        else
        {
            ++iter;
        }
    }

	return 1;
}

int CFileSysMgr::UnRegisterAllFileSys()
{
	m_mapExtName2FileSys.clear();
	return 1;
}

IFileSys* CFileSysMgr::GetVFileSysByExt(const TCHAR* extname)
{
	if (extname==NULL)
	{
		ERRORLOG(_T("param error!"));
		return NULL;
	}

    MAPEXTNAME2FILESYSITER iter;
    iter = m_mapExtName2FileSys.find(extname);
    if (iter!=m_mapExtName2FileSys.end())
    {
        return iter->second;
    }

    return NULL;
}    

IFileSys* CFileSysMgr::GetVFileSysByHandle(VFS_HANDLE handle)
{
	if (handle==VFS_INVALID_HANDLE_VALUE)
	{
		ERRORLOG(_T("param error!"));
		return NULL;
	}

	MAPHANDLE2FILESYSITER iter;
	iter = m_mapHandle2FileSys.find(handle);
	if (iter!=m_mapHandle2FileSys.end())
	{
		return iter->second;
	}
	return NULL;
}

IFileSys* CFileSysMgr::GetGeneralFileSys()
{
    return m_pGeneralFileSys;
}

void CFileSysMgr::SetCaseSense(bool enable)
{
    VECFILESYSITER iter;
    for (iter=m_vecVFSFileSys.begin(); iter!=m_vecVFSFileSys.end(); ++iter)
    {
		IFileSys* pfilesys = (*iter);
        if (pfilesys) pfilesys->SetCaseSense(enable);
    }

	if (m_pGeneralFileSys) m_pGeneralFileSys->SetCaseSense(enable);

	m_bCaseSense = enable;
}

bool CFileSysMgr::IsCaseSense()
{
	return m_bCaseSense;
}


VROOT_h CFileSysMgr::MountRoot(const TCHAR* rootpath, const TCHAR* vdir)
{
	if (rootpath==NULL || vdir==NULL)
	{
		ERRORLOG(_T("param error!"));
		return VFS_INVALID_HANDLE_VALUE;
	}

    TCHAR fixedrootpath[MAX_PATH];
    TCHAR fixedvpath[MAX_PATH];

	GetFullPathName(rootpath,MAX_PATH,fixedrootpath,NULL);
    
    FixDirName(vdir,fixedvpath);
    
    VROOT_h hroot = VFS_INVALID_HANDLE_VALUE;

	int len = _tcslen(fixedrootpath);
	if (len==0) return VFS_INVALID_HANDLE_VALUE;
	if (fixedrootpath[len-1]==_T('\\') || fixedrootpath[len-1]==_T('/'))
		fixedrootpath[len-1]=_T('\0');
	int type = VFSNAME::GetPathType(fixedrootpath);
    
    if (type==VFS_PATH_DIRECTORY)
    {
		FixDirName(fixedrootpath);
        hroot = m_pGeneralFileSys->MountRoot(fixedrootpath,fixedvpath);        
		m_mapHandle2FileSys[hroot] = m_pGeneralFileSys;
    }   
    else if (type==VFS_PATH_FILE)
    {
        TCHAR extname[MAX_PATH];
        FixPathName(fixedrootpath);
        GetPathExtName(fixedrootpath,extname);
        IFileSys* pfilesys = GetVFileSysByExt(extname);
        if (pfilesys)
        {
            hroot = pfilesys->MountRoot(fixedrootpath,fixedvpath);
            //m_mapVRoot2FileSys[hroot] = pfilesys;
			m_mapHandle2FileSys[hroot] = pfilesys;
        }
    }     
    return hroot;
}   

int CFileSysMgr::SetCacheSize(int fscachesize, int vfscachesize)
{
	int retc=0;
	if (m_pFSBufferCache) retc+=m_pFSBufferCache->SetCacheSize(fscachesize);
	if (m_pVFSBufferCache) retc+=m_pVFSBufferCache->SetCacheSize(vfscachesize);
	return 1;
}

IBuffer* CFileSysMgr::GetFileBuffer(FILE_h hfile)
{
	if (hfile==VFS_INVALID_HANDLE_VALUE)
	{
		ERRORLOG(_T("param error!"));
		return NULL;
	}

	IFileSys* pfilesys = GetFileSysByHandle(hfile);
	if (pfilesys) 
	{
		return pfilesys->GetFileBuffer(hfile);
	}
	return NULL;
}

 
void CFileSysMgr::UnMountAll()
{
	VECFILESYSITER iter;
	for (iter=m_vecVFSFileSys.begin(); iter!=m_vecVFSFileSys.end(); ++iter)
	{
		(*iter)->UnMountAll();
	}
	m_pGeneralFileSys->UnMountAll();
}    

ROOT_h CFileSysMgr::OpenRoot(const TCHAR* rootpath, int mode)
{
	if (rootpath==NULL)
	{
		ERRORLOG(_T("param error!"));
		return VFS_INVALID_HANDLE_VALUE;
	}

    TCHAR fixedrootpath[MAX_PATH];

	GetFullPathName(rootpath,MAX_PATH,fixedrootpath,NULL);
    
    ROOT_h hroot = VFS_INVALID_HANDLE_VALUE;

	int len = _tcslen(fixedrootpath);
	if (len==0) return VFS_INVALID_HANDLE_VALUE;
	if (fixedrootpath[len-1]=='\\' || fixedrootpath[len-1]=='/')
		fixedrootpath[len-1]='\0';


	int type = VFSNAME::GetPathType(fixedrootpath);

	int choose;

	if (type==VFS_PATH_FILE) 
	{
		choose=1;
	}
	else if (type==VFS_PATH_DIRECTORY)
	{
		_tcscat(fixedrootpath,_T("\\"));
		choose=0;
	}
	else if (type==VFS_PATH_NOTEXIST 
		     && (mode==VFS_OPENROOT_MODE_WRITE_ALWAYS || mode==VFS_OPENROOT_MODE_WRITE_NEW)
		   )
	{
		TCHAR extname[MAX_PATH];
		GetPathExtName(fixedrootpath,extname);
		IFileSys* pfilesys = GetVFileSysByExt(extname);
		if (pfilesys==NULL)
		{
			_tcscat(fixedrootpath,_T("\\"));
			MkDirEx(fixedrootpath);
			choose=0;
		}
		else
		{
			MkDirEx(fixedrootpath);
			choose=1;
		}
	}
	else
	{
		return VFS_INVALID_HANDLE_VALUE;
	}
    
    if (choose==0)
    {
		FixDirName(fixedrootpath);		
        hroot = m_pGeneralFileSys->OpenRoot(fixedrootpath,mode);        
		m_mapHandle2FileSys[hroot] = m_pGeneralFileSys;
    }   
    else if (choose==1)
    {
        TCHAR extname[MAX_PATH];
        FixPathName(fixedrootpath);
        GetPathExtName(fixedrootpath,extname);
        IFileSys* pfilesys = GetVFileSysByExt(extname);
        if (pfilesys)
        {
            hroot = pfilesys->OpenRoot(fixedrootpath,mode);
			m_mapHandle2FileSys[hroot] = pfilesys;
        }
    }

    return hroot;
}

int CFileSysMgr::CloseRoot(ROOT_h hroot)
{
	if (hroot==VFS_INVALID_HANDLE_VALUE)
	{
		ERRORLOG(_T("param error!"));
		return 0;
	}

	MAPHANDLE2FILESYSITER iter;
	iter = m_mapHandle2FileSys.find(hroot);
	if (iter!=m_mapHandle2FileSys.end())
	{
		if (iter->second->CloseRoot(hroot))
		{
			m_mapHandle2FileSys.erase(hroot);
			return 1;
		}
	}
	return 0;
}

/*
int CFileSysMgr::CopyFile(FIND_h hfindsrc, ROOT_h htrg, const TCHAR* trgpath)
{
	if (hfindsrc==VFS_INVALID_HANDLE_VALUE || htrg==VFS_INVALID_HANDLE_VALUE || trgpath==NULL)
	{
		ERRORLOG(_T("param error!"));
		return 0;
	}

	IFileSys * psrcfilesys = GetFileSysByHandle(hfindsrc);
	IFileSys * ptrgfilesys = GetFileSysByHandle(htrg);
	if (psrcfilesys==NULL || ptrgfilesys==NULL) return 0;

	if (psrcfilesys==ptrgfilesys)
	{
		return psrcfilesys->CopyFile(hfindsrc,htrg,trgpath);
	}
	else
	{
		//directory => path
		TCHAR newtrgpath[MAX_PATH];
		TCHAR fixedtrg[MAX_PATH];
		VFSNAME::FixPathName(trgpath,fixedtrg);

		VFSNAME::GetRelativeTrgFileName(ptrgfilesys->GetPathType(htrg,fixedtrg),
							psrcfilesys->GetRelativeFindPathName(hfindsrc),
							fixedtrg,newtrgpath
						);

		IBuffer* pbuffer = psrcfilesys->ReadFile(hfindsrc);
		if (pbuffer==NULL) return 0;
		if (0==ptrgfilesys->WriteFile(htrg,newtrgpath,pbuffer))
		{
			pbuffer->Release();
			return 0;
		}
		pbuffer->Release();
		return 1;
	}
}
*/

int CFileSysMgr::CopyFile(ROOT_h hsrc, const TCHAR* srcpath, ROOT_h htrg, const TCHAR* trgpath)
{
	if (hsrc==VFS_INVALID_HANDLE_VALUE || srcpath==NULL || htrg==VFS_INVALID_HANDLE_VALUE || trgpath==NULL)
	{
		ERRORLOG(_T("param error!"));
		return 0;
	}

	IFileSys * psrcfilesys = GetFileSysByHandle(hsrc);
	IFileSys * ptrgfilesys = GetFileSysByHandle(htrg);
	if (psrcfilesys==NULL || ptrgfilesys==NULL) return 0;

	if (psrcfilesys==ptrgfilesys)
	{
		return psrcfilesys->CopyFile(hsrc,srcpath,htrg,trgpath);
	}
	else
	{
		//directory => path
		TCHAR fixedsrc[MAX_PATH];
		TCHAR fixedtrg[MAX_PATH];
		TCHAR newtrgpath[MAX_PATH];
		VFSNAME::FixPathName(srcpath,fixedsrc);
		VFSNAME::FixPathName(trgpath,fixedtrg);
		VFSNAME::GetTrgFileName(ptrgfilesys->GetPathType(htrg,fixedtrg),fixedsrc,fixedtrg,newtrgpath);

		IBuffer* pbuffer = psrcfilesys->ReadFile(hsrc,fixedsrc);
		if (pbuffer==NULL) return 0;
		if (0==ptrgfilesys->WriteFile(htrg,newtrgpath,pbuffer))
		{
			pbuffer->Release();
			return 0;
		}
		pbuffer->Release();
		return 1;
	}	
}

int CFileSysMgr::MoveFile(ROOT_h hsrc, const TCHAR* srcpath, ROOT_h htrg, const TCHAR* trgpath)
{
	if (hsrc==VFS_INVALID_HANDLE_VALUE || srcpath==NULL || htrg==VFS_INVALID_HANDLE_VALUE || trgpath==NULL)
	{
		ERRORLOG(_T("param error!"));
		return 0;
	}

	IFileSys * psrcfilesys = GetFileSysByHandle(hsrc);
	IFileSys * ptrgfilesys = GetFileSysByHandle(htrg);
	if (psrcfilesys==NULL || ptrgfilesys==NULL) return 0;

	if (psrcfilesys==ptrgfilesys)
	{
		return psrcfilesys->CopyFile(hsrc,srcpath,htrg,trgpath);
	}
	else
	{
		//directory => path
		TCHAR fixedsrc[MAX_PATH];
		TCHAR fixedtrg[MAX_PATH];
		TCHAR newtrgpath[MAX_PATH];
		VFSNAME::FixPathName(srcpath,fixedsrc);
		VFSNAME::FixPathName(trgpath,fixedtrg);
		VFSNAME::GetTrgFileName(ptrgfilesys->GetPathType(htrg,fixedtrg),fixedsrc,fixedtrg,newtrgpath);

		IBuffer* pbuffer = psrcfilesys->ReadFile(hsrc,fixedsrc);
		if (pbuffer==NULL) return 0;
		if (0==ptrgfilesys->WriteFile(htrg,newtrgpath,pbuffer))
		{
			pbuffer->Release();
			return 0;
		}
		pbuffer->Release();

		if (0==psrcfilesys->DelFile(hsrc,srcpath))
		{
			return 0;
		}
		return 1;
	}	
}

/*
IBuffer* CFileSysMgr::ReadFile(FIND_h hfindsrc)
{
	if (hfindsrc==VFS_INVALID_HANDLE_VALUE)
	{
		ERRORLOG(_T("param error!"));
		return NULL;
	}

	IFileSys * psrcfilesys = GetFileSysByHandle(hfindsrc);
	if (psrcfilesys==NULL) return NULL;
	return psrcfilesys->ReadFile(hfindsrc);
}
*/

IBuffer* CFileSysMgr::ReadFile(ROOT_h hsrc, const TCHAR* srcpath)
{
	if (hsrc==VFS_INVALID_HANDLE_VALUE || srcpath==NULL)
	{
		ERRORLOG(_T("param error!"));
		return NULL;
	}

	IFileSys * psrcfilesys = GetFileSysByHandle(hsrc);
	if (psrcfilesys==NULL) return NULL;
	return psrcfilesys->ReadFile(hsrc,srcpath);
}

int CFileSysMgr::WriteFile(ROOT_h htrg, const TCHAR* trgpath,IBuffer* pbuffer)
{
	if (htrg==VFS_INVALID_HANDLE_VALUE || trgpath==NULL || pbuffer==NULL)
	{
		ERRORLOG(_T("param error!"));
		return 0;
	}

	IFileSys * psrcfilesys = GetFileSysByHandle(htrg);

	if (psrcfilesys==NULL) return NULL;

	return psrcfilesys->WriteFile(htrg,trgpath,pbuffer);
}

int CFileSysMgr::DelFile(ROOT_h htrg, const TCHAR* trgpath)
{
	if (htrg==VFS_INVALID_HANDLE_VALUE || trgpath==NULL)
	{
		ERRORLOG(_T("param error!"));
		return 0;
	}

	IFileSys * pfilesys = GetFileSysByHandle(htrg);
	if (pfilesys==NULL)
	{
		return 0;
	}

	return pfilesys->DelFile(htrg,trgpath);
}

int CFileSysMgr::DelTree(ROOT_h htrg, const TCHAR* trgpath)
{
	if (htrg==VFS_INVALID_HANDLE_VALUE || trgpath==NULL)
	{
		ERRORLOG(_T("param error!"));
		return 0;
	}

	IFileSys * pfilesys = GetFileSysByHandle(htrg);
	if (pfilesys==NULL)
	{
		return 0;
	}

	return pfilesys->DelTree(htrg,trgpath);
}

FIND_h CFileSysMgr::FindFirstFile(ROOT_h hroot, const TCHAR* startpath, bool recursive)
{
	if (hroot==VFS_INVALID_HANDLE_VALUE || startpath==NULL)
	{
		ERRORLOG(_T("param error!"));
		return VFS_INVALID_HANDLE_VALUE;
	}

	MAPHANDLE2FILESYSITER iter;
	iter = m_mapHandle2FileSys.find(hroot);
	if (iter!=m_mapHandle2FileSys.end())
	{
		FIND_h hfind = iter->second->FindFirstFile(hroot,startpath,recursive);
		if (hfind!=VFS_INVALID_HANDLE_VALUE)
		{
			m_mapHandle2FileSys[hfind]=iter->second;
		}
		return hfind;
	}

	return VFS_INVALID_HANDLE_VALUE;
}

bool CFileSysMgr::FindNextFile(FIND_h hfind)
{
	if (hfind==VFS_INVALID_HANDLE_VALUE)
	{
		ERRORLOG(_T("param error!"));
		return false;
	}

	MAPHANDLE2FILESYSITER iter;
	iter = m_mapHandle2FileSys.find(hfind);
	if (iter!=m_mapHandle2FileSys.end())
	{
		return iter->second->FindNextFile(hfind);
	}

	return false;
}

int CFileSysMgr::FindClose(FIND_h hfind)
{
	if (hfind==VFS_INVALID_HANDLE_VALUE)
	{
		ERRORLOG(_T("param error!"));
		return 0;
	}

	MAPHANDLE2FILESYSITER iter;
	iter = m_mapHandle2FileSys.find(hfind);
	if (iter!=m_mapHandle2FileSys.end())
	{
		int ret = iter->second->FindClose(hfind);
		m_mapHandle2FileSys.erase(iter);
		return ret;
	}

	return 0;
}

FIND_h CFileSysMgr::GetFindRoot(FIND_h hfind)
{
	if (hfind==VFS_INVALID_HANDLE_VALUE)
	{
		ERRORLOG(_T("param error!"));
		return NULL;
	}

	MAPHANDLE2FILESYSITER iter;
	iter = m_mapHandle2FileSys.find(hfind);
	if (iter!=m_mapHandle2FileSys.end())
	{
		return iter->second->GetFindRoot(hfind);
	}

	return NULL;
}

const TCHAR* CFileSysMgr::GetFindPathName(FIND_h hfind)
{
	if (hfind==VFS_INVALID_HANDLE_VALUE)
	{
		ERRORLOG(_T("param error!"));
		return NULL;
	}

	MAPHANDLE2FILESYSITER iter;
	iter = m_mapHandle2FileSys.find(hfind);
	if (iter!=m_mapHandle2FileSys.end())
	{
		const TCHAR* p = iter->second->GetFindPathName(hfind);
		return p;
	}

	return NULL;
}
 
const TCHAR* CFileSysMgr::GetRelativeFindPathName(FIND_h hfind)
{
	MAPHANDLE2FILESYSITER iter;
	iter = m_mapHandle2FileSys.find(hfind);
	if (iter!=m_mapHandle2FileSys.end())
	{
		const TCHAR *p = iter->second->GetRelativeFindPathName(hfind);
		return p;
	}
	return NULL;
}

IFileSys* CFileSysMgr::GetFileSysByHandle(VFS_HANDLE handle)
{
	if (handle==VFS_INVALID_HANDLE_VALUE)
	{
		ERRORLOG(_T("param error!"));
		return NULL;
	}

	MAPHANDLE2FILESYSITER iter;
	iter = m_mapHandle2FileSys.find(handle);
	if (iter==m_mapHandle2FileSys.end()) return NULL;

	return iter->second;
}

FILE_h CFileSysMgr::OpenFile(const TCHAR* filepath, int tag)
{
	if (filepath==NULL)
	{
		ERRORLOG(_T("param error!"));
		return VFS_INVALID_HANDLE_VALUE;
	}

    FILE_h hfile = VFS_INVALID_HANDLE_VALUE;
    
    TCHAR fixedfilepath[MAX_PATH];
    FixPathName(filepath,fixedfilepath);
	
	if (!IsCaseSense())
	{
		StringToLower(fixedfilepath);
	}
    
    bool vfslast=false;

    for (;;)
    {    
        if (m_bVFSFirst || vfslast)
        {
            if (m_pVFSFileMapper==NULL) return hfile;
            
            long lparam;
            VROOT_h hroot = m_pVFSFileMapper->FindFile(fixedfilepath,&lparam);
            if (hroot!=VFS_INVALID_HANDLE_VALUE)
            {
                IFileSys* pfilesys = GetFileSysByHandle(hroot);
                if (pfilesys)
                {
                   hfile = pfilesys->OpenFile(hroot,fixedfilepath,lparam,tag|VFS_OF_TAG_STANDARDPATHNAME);
				   if (hfile!=VFS_INVALID_HANDLE_VALUE)
				   {
					   m_mapHandle2FileSys[hfile]=pfilesys;
				   }
                }
            }
            
            if (vfslast) break;
        }    
        
        if (hfile==VFS_INVALID_HANDLE_VALUE)
        {
			long lparam;
			VROOT_h hroot = m_pFSFileMapper->FindFile(fixedfilepath,&lparam);
		    if (hroot!=VFS_INVALID_HANDLE_VALUE)
			{
			    hfile = m_pGeneralFileSys->OpenFile(hroot,fixedfilepath,lparam,tag|VFS_OF_TAG_STANDARDPATHNAME);
			}
			else
			{
				hfile = m_pGeneralFileSys->OpenFile(fixedfilepath, tag|VFS_OF_TAG_STANDARDPATHNAME);
			}
		//	hroot = m_pFSFileMapper->FindFile(fixedfilepath);
		    if (hfile!=VFS_INVALID_HANDLE_VALUE)
			{
			   m_mapHandle2FileSys[hfile]=m_pGeneralFileSys;
			}
        }
        
        if (hfile==VFS_INVALID_HANDLE_VALUE && !m_bVFSFirst)
        {
            vfslast = true; 
        }
        else
        {
            break;
        }
    }
    
    return hfile;
}   
 
void CFileSysMgr::CloseFile(FILE_h hfile)
{
	if (hfile==VFS_INVALID_HANDLE_VALUE)
	{
		ERRORLOG(_T("param error!"));
		return ;
	}

	MAPHANDLE2FILESYSITER iter = m_mapHandle2FileSys.find(hfile);
	if (iter==m_mapHandle2FileSys.end())
	{
		ERRORLOG(_T("file handle not exist"));
		return ;
	}
	if (0==iter->second->CloseFile(hfile))
	{
		m_mapHandle2FileSys.erase(iter);
	}
}    

void CFileSysMgr::SetVFSFirst(bool bfirst)
{
    m_bVFSFirst = bfirst;
}    

IHandleAlloc* CFileSysMgr::GetHandleAlloc()
{
    return m_pHandleAlloc;
}
    
IFileMapper* CFileSysMgr::GetVFSFileMapper()
{
    return m_pVFSFileMapper;
}

IFileMapper* CFileSysMgr::GetFSFileMapper()
{
	return m_pFSFileMapper;
}

IBufferCache* CFileSysMgr::GetVFSBufferCache()
{
	return m_pVFSBufferCache;
}

IBufferCache* CFileSysMgr::GetFSBufferCache()
{
	return m_pFSBufferCache;
}


IFileSysMgr* ICreateFileSysMgr(ILogSys* pLogSys)
{
	s_pLogSys = pLogSys;

	IFileSysMgr* pFileSysMgr = static_cast<IFileSysMgr*>(new CFileSysMgr());

	if (pFileSysMgr==NULL)
	{
		ERRORLOG(_T("filesys manager interface create error"));
		return NULL;
	}

	if (0==pFileSysMgr->Init())
	{
		ERRORLOG(_T("filesys manager interface init error"));
		pFileSysMgr->Release();
		return NULL;
	}

	return pFileSysMgr;
}    


NAMESPACEEND