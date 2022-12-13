
#pragma once

#include <cassert>
#include <memory>
#include <cstdlib>
#include <malloc.h>
#include <direct.h>

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#include <tchar.h>

#include "vfs_i.h"

#if !defined(INVALID_SET_FILE_POINTER)
#define INVALID_SET_FILE_POINTER (DWORD)-1
#endif

#pragma warning(disable:4786)

#include <string>
#include <vector>
#include <list>
#include <map>
#include <set>

NAMESPACEBEGIN

#if !defined(stl_string)
#if defined(UNICODE)
   #define stl_string std::wstring
#else
   #define stl_string std::string
#endif
#endif

#if !defined(stl_vector)
#define stl_vector std::vector
#endif

#if !defined(stl_list)
#define stl_list std::list
#endif

#if _MSC_VER>=1400
	#if !defined(stl_map)
		#define stl_map stdex::hash_map 
		#pragma message("map use hash")
	#endif
#else
	#if !defined(stl_map)
		#define stl_map std::map 
		#pragma message("map use rb tree")
	#endif
#endif

#if !defined(stl_set)
#define stl_set std::set
#endif

#if !defined(stl_pair)
#define stl_pair std::pair
#endif

#if !defined(stl_make_pair)
#define stl_make_pair std::make_pair
#endif
	
extern int FixDirName(const TCHAR* srcdir, TCHAR* fixeddir);
extern int FixPathName(const TCHAR* srcpath, TCHAR* fixedpath);
extern int FixDirName(TCHAR* fixeddir);
extern int FixPathName(TCHAR* fixedpath);
extern int GetPathExtName(const TCHAR* pathname, TCHAR* extname);
extern int GetPathType(const TCHAR* path);
extern bool MkDirEx( const TCHAR* path );
extern int GetTrgFileName(int type, const TCHAR* srcpath, const TCHAR* trgpath, 
						  TCHAR* newtrgpath
					   );
extern void StringToLower(TCHAR* str);

//extern int GetRelativeTrgFileName(int type, const TCHAR* relativesrcpath, 
//						   const TCHAR* trgpath, TCHAR* newtrgpath
//					   );
extern int TrimSpace(TCHAR* str);

struct simple_mutex
{
	long* pmutex;

	simple_mutex(long* _pmutex)	: pmutex(_pmutex) 
	{
		while(InterlockedExchange(pmutex,1)) ::Sleep(0);
	}

	~simple_mutex()
	{
		InterlockedExchange(pmutex,0);
	}
};

#if !defined(VFS_PORTABLE)

extern  IBuffer* ICreateBuffer(int usedsize,ILogSys* pLogSys=NULL);
extern  IFileMapper* ICreateFileMapper(ILogSys* pLogSys=NULL);
extern  IFileSys* ICreateGeneralFileSys(IFileSysMgr* pFileSysMgr, ILogSys* pLogSys=NULL);
extern  IHandleAlloc* ICreateHandleAlloc(ILogSys* pLogSys=NULL);
extern  IFileSys* ICreateVFileSys(IFileSysMgr* pFileSysMgr, ILogSys* pLogSys=NULL);
extern  IRWFilter* ICreateZlibRWFilter(ILogSys* pLogSys=NULL);
extern  IBufferCache* ICreateBufferCache(IHandleAlloc* phandlealloc, IFileMapper* pfilemapper, int cachesize, ILogSys* pLogSys=NULL);

//normal use
extern  ILogSys* ICreateLogFileSys(ILogSys* pLogSys=NULL);
extern  IFileSysMgr* ICreateFileSysMgr(ILogSys* pLogSys=NULL);

#endif


NAMESPACEEND