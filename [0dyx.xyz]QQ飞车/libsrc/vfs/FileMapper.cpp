
#include "FileMapper.h"

NAMESPACEBEGIN

#if !defined(VFS_PORTABLE)
	DEFINE_FACTORY_METHOD(CFileMapper);
#endif


static ILogSys* s_pLogSys = NULL;

#define ERRORLOG(msg) if (s_pLogSys) s_pLogSys->printf(_T("%s(%d) : error %s\n"),__FILE__,__LINE__,msg);
#define ERRORLOG1(msg,p1) if (s_pLogSys) s_pLogSys->printf(_T("%s(%d) : error "##msg"\n"),__FILE__,__LINE__,p1);
#define ERRORLOG2(msg,p1,p2) if (s_pLogSys) s_pLogSys->printf(_T("%s(%d) : error "##msg"\n"),__FILE__,__LINE__,p1,p2);
#define ERRORLOG3(msg,p1,p2,p3) if (s_pLogSys) s_pLogSys->printf(_T("%s(%d) : error "##msg"\n"),__FILE__,__LINE__,p1,p2,p3);



int CFileMapper::MapFile(const TCHAR* filepath, VROOT_h hroot, long lparam)
{
	if (filepath==NULL || hroot==VFS_INVALID_HANDLE_VALUE)
	{
		ERRORLOG(_T("param error!"));
		return 0;
	}

	MAPFILE2MAPPERDATAITER iter;
	iter = m_mapFile2MapperData.find(filepath);
	if (iter!=m_mapFile2MapperData.end())
	{
		iter->second->hroot=hroot;
		iter->second->lparam=lparam;
	}
	else
	{
		mapper_data_t* pfinddata = new mapper_data_t(hroot,lparam);
		if (pfinddata==NULL)
		{
			ERRORLOG(_T("memory not enough"));
			return 0;
		}
		m_mapFile2MapperData[filepath]=pfinddata;
	}
	return 1;
}

VROOT_h CFileMapper::FindFile(const TCHAR* filepath, long* lparam)
{
	if (filepath==NULL)
	{
		ERRORLOG(_T("param error!"));
		return VFS_INVALID_HANDLE_VALUE;
	}

	MAPFILE2MAPPERDATAITER iter;
	iter = m_mapFile2MapperData.find(filepath);
	if (iter!=m_mapFile2MapperData.end())
	{
		if (lparam) *lparam = iter->second->lparam;
		return iter->second->hroot;	
	}
	return VFS_INVALID_HANDLE_VALUE;
}

void CFileMapper::Reset()
{
	MAPFILE2MAPPERDATAITER iter;
	for (iter=m_mapFile2MapperData.begin(); iter!=m_mapFile2MapperData.end(); ++iter)
	{
		delete iter->second;
	}
	m_mapFile2MapperData.clear();
}

void CFileMapper::Release()
{
#if defined(VFS_PORTABLE)
	delete this;
#else
	Free();
#endif
}

IFileMapper* ICreateFileMapper(ILogSys* pLogSys)
{
	s_pLogSys = pLogSys;
	IFileMapper* pfilemapper = static_cast<IFileMapper*>(new CFileMapper());
	if (pfilemapper==NULL)
	{
		ERRORLOG(_T("create filemapper interface fail!"));
		return NULL;
	}
	return pfilemapper;
}


NAMESPACEEND