#ifndef _VFSHELPER_H_
#define _VFSHELPER_H_

#define _WIN32_WINNT	0x500

#pragma warning(disable : 4786)

#include "Template/teniopointer_t.h"
#include "Include/vfs/vfs_i.h"
using namespace Tenio;

#include <string>
#include <list>
#include <map>
#include <set>

#if !defined(stl_string)
#if defined(UNICODE)
   #define stl_string std::wstring
#else
   #define stl_string std::string
#endif
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

struct OpenFileData
{
	int hroot;
	int hfile;
	int    m_nfilesize;
	int    m_npos;
	IBuffer *pBuffer;
	stl_string s_filename;
};

class CVFSHelper
{

public:

	CVFSHelper();
	~CVFSHelper();

	//虚拟更新
	ROOT_h      OpenRootUpdata(const TCHAR* rootpath, int mode=VFS_OPENROOT_MODE_READ);
	VFS_HANDLE  OpenFileUpdata(ROOT_h hroot,TCHAR *pFileName);
	size_t        ReadFileUpdata(VFS_HANDLE handle,size_t size,
 						   size_t count,void *pReadBuffer,int buffercount);
	bool        WriteFileUpdata(FILE_h handle,size_t size,
						   size_t count,void *pReadBuffer,int buffercount);
	int         GetFilePosUpdata(VFS_HANDLE handle);
	int         GetFileSizeUpdata(VFS_HANDLE handle);
	int         FileSeekUpdata(VFS_HANDLE handle,int offset,int origin);

	FIND_h      FindFirstFile(ROOT_h hroot, const TCHAR* startpath, bool recursive=true);
	bool        FindNextFile(FIND_h hfind);
	int         FindClose(FIND_h hfind);
	FIND_h      GetFindRoot(FIND_h hfind);
	const TCHAR* GetFindPathName(FIND_h hfind);
	const TCHAR* GetRelativeFindPathName(FIND_h hfind);
	BOOL         CopyTree(ROOT_h hsrc,TCHAR *pPathName,ROOT_h hdest);
	int			 CopyFile(ROOT_h hsrc, const TCHAR* srcpath, 
						ROOT_h htrg, const TCHAR* trgpath);
	int			 MoveFile(ROOT_h hsrc, const TCHAR* srcpath,
						ROOT_h htrg, const TCHAR* trgpath);

	int			DelFile(ROOT_h htrg, const TCHAR* trgpath);
	int			DelTree(ROOT_h htrg, const TCHAR* trgpath);


	int         CloseFileUpdata(VFS_HANDLE handle);
	int         CloseRootUpdata(VFS_HANDLE hroot);

	//虚拟读取
	ROOT_h   MountRoot(const TCHAR* rootpath, const TCHAR* vdir);
	void     UnMountAll();
	VFS_HANDLE   OpenFileVR(VFS_HANDLE hroot,TCHAR *pFileName);

	int    GetFilePosVR(VFS_HANDLE handle);
	int    GetFileSizeVR(VFS_HANDLE handle);
	int   FileSeekVR(VFS_HANDLE handle,int offset,int origin);

	size_t   ReadFileVR(VFS_HANDLE handle,size_t size,
					   size_t count,void *pReadBuffer,int buffercount);

	IBuffer *ReadFile(FILE_h hfile);

	int    SetCacheSize(int fscachesize, int vfscachesize);
	void   SetVFSFirst(bool bfirst=true);

	int    CloseFileVR(VFS_HANDLE handle);

	//打包
	BOOL CompressVfs(TCHAR *pStrFile,TCHAR *pDestFile);
	//解包
	BOOL UnCompressVfs(TCHAR *pStrFile,TCHAR *pDestFile);

private:	
	void SetFilePos(VFS_HANDLE handle,int Pos);
	VFS_HANDLE AddOpenFile(ROOT_h hroot,TCHAR *pFileName);

	typedef stl_map<VFS_HANDLE,OpenFileData*> MAPFILE2OPENFILEDATA;
	typedef MAPFILE2OPENFILEDATA::iterator MAPFILE2OPENFILEDATAITER;

	MAPFILE2OPENFILEDATA m_mapFile2OpenFileData;
	TTenioPtr<IFileSysMgr> pFileSysMgr;
	IHandleAlloc *pHandleAlloc;

};

#endif
//NAMESPACEEND