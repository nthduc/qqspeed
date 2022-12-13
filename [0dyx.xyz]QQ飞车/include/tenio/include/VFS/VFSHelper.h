//#include"vfs_i.h"
#if !defined(VFS_PORTABLE)
#  include "teniodll.h"
#  define IFILESYSMGR TComponentImpl<IFileSysMgr, CFileSysMgr, COMPONENTID_VFS_FILESYSMGR, 1>
#  define RELEASEFUNC_ADDON void ComponentFinalize() {}
#else
#  define IFILESYSMGR IFileSysMgr
#  define RELEASEFUNC_ADDON
#endif

#  include "../vfs.h"
#  include "vfs_i.h"

using namespace VFS;

NAMESPACEBEGIN

//expert use

extern "C" {

void VFSHelperInit(void);

//ÐéÄâ¸üÐÂ
ROOT_h OpenRootUpdata(const TCHAR* rootpath, int mode=VFS_OPENROOT_MODE_READ);
VFS_HANDLE OpenFileUpdata(ROOT_h hroot,TCHAR *pFileName);
bool ReadFileUpdata(VFS_HANDLE handle,size_t size,
 						   size_t count,void *pReadBuffer,int buffercount);
bool WriteFileUpdata(VFS_HANDLE handle,size_t size,
						   size_t count,void *pReadBuffer,int buffercount);
int  GetFilePosUpdata(VFS_HANDLE handle);
int  GetFileSizeUpdata(VFS_HANDLE handle);
bool FileSeekUpdata(VFS_HANDLE handle,int offset,int origin);

FIND_h FindFirstFile(ROOT_h hroot, const TCHAR* startpath, bool recursive=true);
bool FindNextFile(FIND_h hfind);
int FindClose(FIND_h hfind);
FIND_h GetFindRoot(FIND_h hfind);
const TCHAR* GetFindPathName(FIND_h hfind);
const TCHAR* GetRelativeFindPathName(FIND_h hfind);

int  CloseFileUpdata(VFS_HANDLE handle);
int  CloseRootUpdata(VFS_HANDLE hroot);

//ÐéÄâ¶ÁÈ¡
ROOT_h MountRoot(const TCHAR* rootpath, const TCHAR* vdir);
VFS_HANDLE OpenFileVR(VFS_HANDLE hroot,TCHAR *pFileName);

int  GetFilePosVR(VFS_HANDLE handle);
int  GetFileSizeVR(VFS_HANDLE handle);
bool FileSeekVR(VFS_HANDLE handle,int offset,int origin);

bool ReadFileVR(VFS_HANDLE handle,size_t size,
					   size_t count,void *pReadBuffer,int buffercount);

int SetCacheSize(int fscachesize, int vfscachesize);
void SetVFSFirst(bool bfirst=true);

int  CloseFileVR(VFS_HANDLE handle);
int  CloseRootVR(VFS_HANDLE hroot);
}

NAMESPACEEND