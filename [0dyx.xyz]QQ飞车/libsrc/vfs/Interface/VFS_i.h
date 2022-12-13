
#pragma once

#if defined(VFS_PORTABLE)

	#ifdef VFS_EXPORTS
	#define VFS_API __declspec(dllexport)
	#else
	#define VFS_API __declspec(dllimport)
	#endif

	#define PUBLICBASECLASS

	#define VFSNAME VFS

#else

	#include "../teniointerface.h"
	#include "../component_i.h"

	#define PUBLICBASECLASS : public IComponent 

	#define VFSNAME Tenio

#endif

#define NAMESPACEBEGIN  namespace VFSNAME {
#define NAMESPACEEND }


#if !defined(NULL)
#define NULL ((void *)0)
#endif

#define VFS_FAILED(s)  ((int)(s)<=0)
#define VFS_SUCCEEDED(s) ((int)(s)>0)


NAMESPACEBEGIN


typedef unsigned int VFS_HANDLE;
typedef VFS_HANDLE FILE_h;
typedef VFS_HANDLE VROOT_h;
typedef VFS_HANDLE ROOT_h;
typedef VFS_HANDLE FIND_h;
typedef VFS_HANDLE CACHEMGR_h;
typedef VFS_HANDLE CACHE_h;

enum 
{
	VFS_OF_TAG_NONE = 0,
	VFS_OF_TAG_NOFILTER = 0x4000,
	VFS_OF_TAG_STANDARDPATHNAME = 0x8000
};

enum
{
	VFS_OPENROOT_MODE_READ = 0,
	VFS_OPENROOT_MODE_WRITE_EXIST = 1,
	VFS_OPENROOT_MODE_WRITE_ALWAYS = 2,
	VFS_OPENROOT_MODE_WRITE_NEW = 3
};

enum
{
	VFS_INVALID_HANDLE_VALUE=-1,
	VFS_HANDLE_VALUE_START=0
};

enum
{
	VFS_PATH_DIRECTORY,
    VFS_PATH_FILE,
	VFS_PATH_NOTEXIST
};

enum
{
	VFS_FILEATTR_FILE = 1,
	VFS_FILEATTR_DIR = 2
};

enum 
{
	VFS_FILE_HEAD_ID = ' sfv'
};

class IFileMapper;
class IBuffer;
class IBufferCache;
class IFileSysMgr;
class IFileSys;
class IHandleAlloc;
class IRWFilter;

struct ILogSys
{
	virtual int printf(const TCHAR* fmt, ...)=0;
};

#if defined(VFS_PORTABLE)
//expert use
extern "C" {

VFS_API IBuffer* ICreateBuffer(int usedsize,ILogSys* pLogSys=NULL);
VFS_API IFileMapper* ICreateFileMapper(ILogSys* pLogSys=NULL);
VFS_API IFileSys* ICreateGeneralFileSys(IFileSysMgr* pFileSysMgr, ILogSys* pLogSys=NULL);
VFS_API IHandleAlloc* ICreateHandleAlloc(ILogSys* pLogSys=NULL);
VFS_API IFileSys* ICreateVFileSys(IFileSysMgr* pFileSysMgr, ILogSys* pLogSys=NULL);
VFS_API IRWFilter* ICreateZlibRWFilter(ILogSys* pLogSys=NULL);
VFS_API IBufferCache* ICreateBufferCache(IHandleAlloc* phandlealloc, IFileMapper* pfilemapper, int cachesize, ILogSys* pLogSys=NULL);
//normal use
VFS_API ILogSys* ICreateLogFileSys(ILogSys* pLogSys=NULL);
VFS_API IFileSysMgr* ICreateFileSysMgr(ILogSys* pLogSys=NULL);

}


#else

TENIO_MAKE_TYPEID_MAP(IFileSys, COMPONENTID_VFS_FILESYS)
//TENIO_MAKE_TYPEID_MAP(IFileSys, COMPONENTID_VFS_VFILESYS)
TENIO_MAKE_TYPEID_MAP(IFileSysMgr, COMPONENTID_VFS_FILESYSMGR)
TENIO_MAKE_TYPEID_MAP(IFileMapper, COMPONENTID_VFS_FILEMAPPER)
TENIO_MAKE_TYPEID_MAP(IBuffer, COMPONENTID_VFS_BUFFER)
TENIO_MAKE_TYPEID_MAP(IBufferCache, COMPONENTID_VFS_BUFFERCACHE)
TENIO_MAKE_TYPEID_MAP(IHandleAlloc, COMPONENTID_VFS_HANDLEALLOC)
TENIO_MAKE_TYPEID_MAP(IRWFilter, COMPONENTID_VFS_ZLIBRWFILTER)

#endif



NAMESPACEEND

#include "FileMapper_i.h"
#include "HandleAlloc_i.h"
#include "Buffer_i.h"
#include "BufferCache_i.h"
#include "RWFilter_i.h"
#include "FileSys_i.h"
#include "FileSysMgr_i.h"
