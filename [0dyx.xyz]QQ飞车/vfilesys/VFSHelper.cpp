#include <tchar.h>
#include <windows.h>
#include <assert.h>

#include "VFSHelper.h"

CVFSHelper::CVFSHelper()
{
	TTenioPtr<IFileSysMgr> spFileSysMgr;
	pFileSysMgr->Init();
	pHandleAlloc = pFileSysMgr->GetHandleAlloc();

	assert(pFileSysMgr != NULL&& pHandleAlloc !=NULL);
}

CVFSHelper::~CVFSHelper()
{
}

//¸üÐÂ
ROOT_h CVFSHelper::OpenRootUpdata(const TCHAR* rootpath, int mode)
{
	assert(pFileSysMgr != NULL);
	ROOT_h hroot = pFileSysMgr->OpenRoot(rootpath,mode);

	return hroot;
}

VFS_HANDLE CVFSHelper::OpenFileUpdata(ROOT_h hroot,TCHAR *pFileName)
{
	MAPFILE2OPENFILEDATAITER iter;

	return AddOpenFile(hroot,pFileName);
}

VFS_HANDLE CVFSHelper::AddOpenFile(ROOT_h hroot,TCHAR *pFileName)
{
	FIND_h h_find = pFileSysMgr->FindFirstFile(hroot,"",true);

	if(h_find!=VFS_INVALID_HANDLE_VALUE)
	{
		bool IsFindFile;
		do
		{	
			TCHAR *pSubfilename = (TCHAR *)pFileSysMgr->GetFindPathName(h_find);
			if(_tcscmp(pFileName,pSubfilename)!=0)
			{
				IsFindFile = pFileSysMgr->FindNextFile(h_find);
				continue;
			}

			pFileSysMgr->FindClose(h_find);
			OpenFileData *pFileData = new OpenFileData;
			ROOT_h handle = VFS_INVALID_HANDLE_VALUE;
			if(pFileData == NULL)
			{
				//error("no memory for openfiledata!");
				return false;
			}

			pFileData->hroot = hroot;
			pFileData->hfile = VFS_INVALID_HANDLE_VALUE;
			pFileData->m_npos = 0;
			pFileData->pBuffer = NULL;
			TCHAR *p = _tcschr(pFileName,_T('\\'));
			if(p == NULL)
			{
				pFileData->s_filename = pFileName;
			}
			else if(++*p != 0)
			{
				pFileData->s_filename = p;
			}

			IBuffer *pBuffer = pFileSysMgr->ReadFile(hroot,pFileName);
			if(pBuffer != NULL)
			{
				pFileData->m_nfilesize = pBuffer->GetUsedSize();
				//pFileData->pBuffer = pBuffer;
				pBuffer->Release();

				handle = pHandleAlloc->AllocHandle();
				m_mapFile2OpenFileData[handle] = pFileData;
			}
			else
			{
				return VFS_INVALID_HANDLE_VALUE;
			}

			
 			return handle;
			
		}while(IsFindFile);
	}

	return VFS_INVALID_HANDLE_VALUE;
}

void CVFSHelper::SetFilePos(VFS_HANDLE handle,int Pos)
{
	assert(Pos>=0);

	MAPFILE2OPENFILEDATAITER iter;
	iter = m_mapFile2OpenFileData.find(handle);
	if(iter == m_mapFile2OpenFileData.end())
	{
		return;
	}

	if(Pos <= iter->second->m_nfilesize)
	{
		iter->second->m_npos = Pos;
	}
	else
	{
		iter->second->m_npos = iter->second->m_nfilesize;
	}
}

int CVFSHelper::GetFilePosUpdata(VFS_HANDLE handle)
{
	assert(pFileSysMgr != NULL);

	MAPFILE2OPENFILEDATAITER iter;
	iter = m_mapFile2OpenFileData.find(handle);
	if(iter == m_mapFile2OpenFileData.end())
	{
		return VFS_INVALID_HANDLE_VALUE;
	}

	return iter->second->m_npos;	
}

int CVFSHelper::GetFileSizeUpdata(VFS_HANDLE handle)
{
	assert(pFileSysMgr != NULL);

	MAPFILE2OPENFILEDATAITER iter = m_mapFile2OpenFileData.find(handle);
	if(iter == m_mapFile2OpenFileData.end())
	{
		return NULL;
	}
	else
	{
		return iter->second->m_nfilesize;
	}
}

int CVFSHelper::FileSeekUpdata(VFS_HANDLE handle,int offset,int origin)
{

	assert(pFileSysMgr != NULL);

	MAPFILE2OPENFILEDATAITER iter = m_mapFile2OpenFileData.find(handle);
	if(iter == m_mapFile2OpenFileData.end())
	{
		return -1;
	}	
	
	int npos = GetFilePosUpdata(iter->first);

	if(origin == VFS_SEEK_CUR)
	{
		SetFilePos(iter->first,offset+npos);
	}
	else if(origin == VFS_SEEK_SET)
	{
		SetFilePos(iter->first,0);
		SetFilePos(iter->first,offset);
	}                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                    
	else if(origin == VFS_SEEK_END)
	{
		int size = GetFileSizeUpdata(iter->first);
		if(offset <= size)
		{
			SetFilePos(iter->first,size - offset);
		}
		else
		{
			SetFilePos(iter->first,0);
		}
	}

	return 0;
}

size_t CVFSHelper::ReadFileUpdata(VFS_HANDLE handle,size_t size,
						   size_t count,void *pReadBuffer,int buffercount)
{
	IBuffer *pBuffer = NULL;

	assert(pFileSysMgr != NULL);

	MAPFILE2OPENFILEDATAITER iter;
	iter = m_mapFile2OpenFileData.find(handle);
	if(iter == m_mapFile2OpenFileData.end())
	{
		return 0;
	}


	if(iter->second->pBuffer == NULL)			
	{
		iter->second->pBuffer = pFileSysMgr->ReadFile(iter->second->hroot,
			iter->second->s_filename.c_str());
	}

	pBuffer = iter->second->pBuffer;

	if(pBuffer == NULL)
	{
		return 0;
	}
	

	int MaxSize = iter->second->m_nfilesize = pBuffer->GetUsedSize();
	int nUseSize = count*size<buffercount?count*size:buffercount;
	int pos = GetFilePosUpdata(handle);
	BYTE *pBuf = (BYTE *)pBuffer->GetBuffer();
	if(pBuf == NULL)
	{
		pBuffer->Release();
		iter->second->pBuffer = NULL;
		return false;
	}
	
	size_t sResultsize = 0;
	if(nUseSize + pos < MaxSize)
	{
		memcpy(pReadBuffer,&pBuf[pos],nUseSize);
		SetFilePos(handle,pos + nUseSize);
		sResultsize = nUseSize;
	}
	else
	{
		memcpy(pReadBuffer,&pBuf[pos],MaxSize - pos);
		SetFilePos(handle,MaxSize);
		sResultsize = MaxSize - pos;
	}
	
	pBuffer->Release();
	iter->second->pBuffer = NULL;

	return sResultsize;
}

bool CVFSHelper::WriteFileUpdata(FILE_h handle,size_t size,
						   size_t count,void *pReadBuffer,int buffercount)
{
	IBuffer *pBuffer = NULL;

	assert(pFileSysMgr != NULL);

	MAPFILE2OPENFILEDATAITER iter;
	iter = m_mapFile2OpenFileData.find(handle);
	if(iter == m_mapFile2OpenFileData.end())
	{
		return false;
	}

	if(iter->second->pBuffer == NULL)
	{
		iter->second->pBuffer = pFileSysMgr->ReadFile(iter->second->hroot,
			iter->second->s_filename.c_str());
	}

	pBuffer = iter->second->pBuffer;


    int nUseSize = count*size>buffercount?count*size:buffercount;
	int pos = GetFilePosUpdata(handle);
	int MaxSize = iter->second->m_nfilesize;

	BYTE *pbuf = (BYTE *)pBuffer->GetBuffer();
	if(pos+nUseSize>MaxSize)
	{
		pBuffer->Realloc(pos+nUseSize);
		iter->second->m_nfilesize = pos+nUseSize;
		pBuffer->SetUsedSize(pos+nUseSize);
	}

	memcpy(&pbuf[pos],pReadBuffer,nUseSize);
	if(pos+nUseSize>MaxSize)
	{
		SetFilePos(handle,pos+nUseSize);
	}
	else
	{
		SetFilePos(handle,MaxSize);
	}

	pFileSysMgr->WriteFile(iter->second->hroot,
		iter->second->s_filename.c_str(),pBuffer);

	pBuffer->Release();
	iter->second->pBuffer = NULL;

	return true;
}

FIND_h CVFSHelper::FindFirstFile(ROOT_h hroot, const TCHAR* startpath, bool recursive)
{
	return pFileSysMgr->FindFirstFile(hroot,startpath,recursive);
}

bool CVFSHelper::FindNextFile(FIND_h hfind)
{
	return pFileSysMgr->FindNextFile(hfind);
}

int CVFSHelper::FindClose(FIND_h hfind)
{
	return pFileSysMgr->FindClose(hfind);
}

FIND_h CVFSHelper::GetFindRoot(FIND_h hfind)
{
	return pFileSysMgr->GetFindRoot(hfind);
}

const TCHAR* CVFSHelper::GetFindPathName(FIND_h hfind)
{
	return pFileSysMgr->GetFindPathName(hfind);
}

const TCHAR* CVFSHelper::GetRelativeFindPathName(FIND_h hfind)
{
	return pFileSysMgr->GetRelativeFindPathName(hfind);
}

int CVFSHelper::CloseFileUpdata(VFS_HANDLE handle)
{
	MAPFILE2OPENFILEDATAITER iter;
	iter = m_mapFile2OpenFileData.find(handle);

	if(iter == m_mapFile2OpenFileData.end())
		return false;

	assert(iter->second != NULL);
	
	if(iter->second->pBuffer != NULL)
	{
		iter->second->pBuffer->Release();
	}
	if(iter->second->hfile != VFS_INVALID_HANDLE_VALUE)
	{
		pFileSysMgr->CloseFile(iter->second->hfile);
	}
	
	delete iter->second;
	pHandleAlloc->FreeHandle(iter->first);
	m_mapFile2OpenFileData.erase(iter);
	
	return true;
}

int CVFSHelper::CloseRootUpdata(ROOT_h hroot)
{
	MAPFILE2OPENFILEDATAITER iter;

	for(iter = m_mapFile2OpenFileData.begin();iter!= m_mapFile2OpenFileData.end();iter++)
	{
		if(iter->second->hroot != hroot) continue;

		if(iter->second != NULL)
		{
			if(iter->second->pBuffer != NULL)
			{
				iter->second->pBuffer->Release();
			}
				
			delete iter->second;
		}

		if(iter->first != VFS_INVALID_HANDLE_VALUE)
		{
			pHandleAlloc->FreeHandle(iter->first);
		}
	
		iter = m_mapFile2OpenFileData.erase(iter);
		if(iter == m_mapFile2OpenFileData.end())
		{
			break;
 		}
		iter--;
	}

	pFileSysMgr->CloseRoot(hroot);
	
	return true;
}

//ÐéÄâ¶ÁÈ¡
VFS_HANDLE CVFSHelper::MountRoot(const TCHAR* rootpath, const TCHAR* vdir)
{
	assert(pFileSysMgr != NULL);
	return pFileSysMgr->MountRoot(rootpath,vdir);
}

void CVFSHelper::UnMountAll()
{
	assert(pFileSysMgr != NULL);
	pFileSysMgr->UnMountAll();
}

VFS_HANDLE CVFSHelper::OpenFileVR(VFS_HANDLE hroot,TCHAR *pFileName)
{
	if(pFileName == NULL)
	{
		return VFS_INVALID_HANDLE_VALUE;
	}

	FILE_h hfile = pFileSysMgr->OpenFile(pFileName);
	if(hfile == VFS_INVALID_HANDLE_VALUE)
	{
		return hfile;
	}

	OpenFileData *pFileData = new OpenFileData;
	assert(pFileData != NULL);

	IBuffer *pBuffer = pFileSysMgr->GetFileBuffer(hfile);
	assert(pBuffer != NULL);

	pFileData->hroot = hroot;
	pFileData->hfile = hfile;
	pFileData->m_nfilesize = pBuffer->GetUsedSize();
	pFileData->m_npos = 0;
	pFileData->s_filename = pFileName;
	pFileData->pBuffer = NULL;

	ROOT_h handle = pHandleAlloc->AllocHandle();
	m_mapFile2OpenFileData[handle] = pFileData;
	return handle;
}

int  CVFSHelper::GetFilePosVR(VFS_HANDLE handle)
{
	return GetFilePosUpdata(handle);
}

int CVFSHelper::GetFileSizeVR(VFS_HANDLE handle)
{
	return GetFileSizeUpdata(handle);
}

int CVFSHelper::FileSeekVR(VFS_HANDLE handle,int offset,int origin)
{
	return FileSeekUpdata(handle,offset,origin);
}

size_t CVFSHelper::ReadFileVR(VFS_HANDLE handle,size_t size,
					   size_t count,void *pReadBuffer,int buffercount)
{
	assert(pFileSysMgr != NULL);

	MAPFILE2OPENFILEDATAITER iter;
	iter = m_mapFile2OpenFileData.find(handle);
	if(iter == m_mapFile2OpenFileData.end())
	{
		return 0;
	}


	IBuffer *pBuffer = pFileSysMgr->GetFileBuffer(iter->second->hfile);

	assert(pBuffer != NULL);	

	int MaxSize = iter->second->m_nfilesize = pBuffer->GetUsedSize();
	int nUseSize = count*size<buffercount?count*size:buffercount;
	int pos = GetFilePosUpdata(handle);
	BYTE *pBuf = (BYTE *)pBuffer->GetBuffer();
	if(pBuf == NULL)
	{
		return 0;
	}
	
	size_t sResultSize = 0;
	if(nUseSize + pos < MaxSize)
	{
		memcpy(pReadBuffer,&pBuf[pos],nUseSize);
		SetFilePos(handle,pos + nUseSize);
		sResultSize = nUseSize;
	}
	else
	{
		memcpy(pReadBuffer,&pBuf[pos],MaxSize - pos);
		SetFilePos(handle,MaxSize);
		sResultSize = MaxSize - pos;
	}
	
	return sResultSize;
}

IBuffer *CVFSHelper::ReadFile(FILE_h hfile)
{
	assert(pFileSysMgr != NULL);

	MAPFILE2OPENFILEDATAITER iter;
	iter = m_mapFile2OpenFileData.find(hfile);
	if(iter == m_mapFile2OpenFileData.end())
	{
		return false;
	}


	IBuffer *pBuffer = pFileSysMgr->GetFileBuffer(iter->second->hfile);

	return pBuffer;
}

int CVFSHelper::SetCacheSize(int fscachesize, int vfscachesize)
{
	return pFileSysMgr->SetCacheSize(fscachesize,vfscachesize);
}

void CVFSHelper::SetVFSFirst(bool bfirst)
{
	pFileSysMgr->SetVFSFirst(bfirst);
}

int CVFSHelper::CloseFileVR(VFS_HANDLE handle)
{
	return CloseFileUpdata(handle);
}

int	CVFSHelper::CopyFile(ROOT_h hsrc, const TCHAR* srcpath, 
			ROOT_h htrg, const TCHAR* trgpath)
{
	return pFileSysMgr->CopyFile(hsrc,srcpath,htrg,trgpath);
}

int	 CVFSHelper::MoveFile(ROOT_h hsrc, const TCHAR* srcpath,
			ROOT_h htrg, const TCHAR* trgpath)
{
	return pFileSysMgr->MoveFile(hsrc,srcpath,htrg,trgpath);
}

BOOL CVFSHelper::CompressVfs(TCHAR *pStrFile,TCHAR *pDestFile)
{
	assert(pStrFile != NULL && pDestFile != NULL);

	BOOL nRed = TRUE;

	ROOT_h hsrc = pFileSysMgr->OpenRoot(pStrFile);
	ROOT_h hdest = pFileSysMgr->OpenRoot(pDestFile,VFS_OPENROOT_MODE_WRITE_ALWAYS);

	if(hsrc == VFS_INVALID_HANDLE_VALUE ||
		hdest == VFS_INVALID_HANDLE_VALUE)
	{
		return FALSE;
	}

	BOOL b_find;
	FIND_h h_find = pFileSysMgr->FindFirstFile(hsrc,"",TRUE);

	if(h_find != VFS_INVALID_HANDLE_VALUE)
	{
		do {
			pFileSysMgr->CopyFile(pFileSysMgr->GetFindRoot(h_find),
				pFileSysMgr->GetFindPathName(h_find),hdest,
				pFileSysMgr->GetFindPathName(h_find));
			b_find = pFileSysMgr->FindNextFile(h_find);	
			
		} while(b_find);

		pFileSysMgr->FindClose(h_find);
	}
	else
	{
		nRed = FALSE;
	}

	pFileSysMgr->CloseRoot(hsrc);
	pFileSysMgr->CloseRoot(hdest);

	return nRed;
}

BOOL CVFSHelper::UnCompressVfs(TCHAR *pStrFile,TCHAR *pDestFile)
{
	BOOL nRed = TRUE;
	TCHAR *p = _tcschr(pStrFile,_T('.'));
	if(p != NULL)
	{
		if(_tcscmp(p,".vfs") != 0)
		{
			return FALSE;
		}
	}
	else
	{
		return FALSE;
	}

	return CompressVfs(pStrFile,pDestFile);
}

int CVFSHelper::DelFile(ROOT_h htrg, const TCHAR* trgpath)
{
	return pFileSysMgr->DelFile(htrg,trgpath);
}

int CVFSHelper::DelTree(ROOT_h htrg, const TCHAR* trgpath)
{
	return pFileSysMgr->DelTree(htrg,trgpath);
}

BOOL CVFSHelper::CopyTree(ROOT_h hsrc,TCHAR *pPathName,ROOT_h hdest)
{
	FILE_h hfile = FindFirstFile(hsrc,pPathName,TRUE);

	BOOL bfind;
	do {
		CopyFile(hsrc,GetFindPathName(hfile),
			hdest,GetFindPathName(hfile));
		bfind = FindNextFile(hfile);
	} while(bfind);

	FindClose(hfile);
	return TRUE;
}

