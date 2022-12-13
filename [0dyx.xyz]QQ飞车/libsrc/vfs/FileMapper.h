
#pragma once

#pragma warning(disable : 4786)

#if !defined(VFS_PORTABLE)
#  include "teniodll.h"
#  define IFILEMAPPER TComponentImpl<IFileMapper, CFileMapper, COMPONENTID_VFS_FILEMAPPER, 1>
#  define RELEASEFUNC_ADDON void ComponentFinalize() {}
#else
#  define IFILEMAPPER IFileMapper
#  define RELEASEFUNC_ADDON
#endif

#  include "VFS.h"

NAMESPACEBEGIN


struct mapper_data_t
{
	mapper_data_t(VROOT_h _hroot, long _lparam=0) 
		: hroot(_hroot), lparam(_lparam) 
	{}
    VROOT_h hroot;
    long  lparam;
};

class CFileMapper : public IFILEMAPPER
{
	typedef stl_map<stl_string,mapper_data_t*> MAPFILE2MAPPERDATA;
	typedef MAPFILE2MAPPERDATA::iterator MAPFILE2MAPPERDATAITER;
public:
	CFileMapper() {}
	~CFileMapper() {Reset();}

	RELEASEFUNC_ADDON
		
    virtual int MapFile(const TCHAR* filepath, VROOT_h hroot, long lparam=0);
    virtual VROOT_h FindFile(const TCHAR* filepath, long* lparam=NULL);
	
    virtual void Reset();
    virtual void Release();

protected:
	MAPFILE2MAPPERDATA m_mapFile2MapperData;
};


NAMESPACEEND