// VFileSysImp.cpp : Implementation of CVFileSysImp
#include "stdafx.h"
#include "Vfilesys.h"
#include "VFileSysImp.h"
#include "vfile.h"
#include "txtCtrlr.h"
#include "common.h"

/////////////////////////////////////////////////////////////////////////////
// CVFileSysImp

STDMETHODIMP CVFileSysImp::AppInit(IGameApp* App)
{
	static CVFileMgr s_vfs;
	CtxtCtrlr txtCtrl;
	if ( txtCtrl.Open("pc.txt") )
	{
		while(1)
		{
			if ( !txtCtrl.Next() )
				break;
			CtxtRecordset* pRS = txtCtrl.GetRecordset();
			if ( pRS->GetFieldCount() > 0 )
			{
				LPCSTR pszItem = pRS->GetField(0);
				s_vfs.AddPackage(pszItem);
			}
		}
	}
	return S_OK;
}

STDMETHODIMP CVFileSysImp::AppUnInit()
{
	return S_OK;
}

FILE_V*	CVFileSysImp::vfopen	( const char* filename, const char* mode )	
{
	return FileToHandle(::vfopen(filename, mode));
}

int		CVFileSysImp::vfclose	( FILE_V* stream )							
{
	HandleToFile(stream)->Close();
	return 0;
}

size_t	CVFileSysImp::vfread	( void* buffer, size_t size, size_t count, FILE_V* stream )		
{
	bool bRet = HandleToFile(stream)->Read(buffer, size * count);
	return bRet ? size * count : 0;
}

size_t	CVFileSysImp::vfwrite	( const void *buffer, size_t size, size_t count, FILE_V *stream )	
{
	bool bRet = HandleToFile(stream)->Write((void*)buffer, size * count);
	return bRet ? size * count : 0;
}
int		CVFileSysImp::vfseek	( FILE_V *stream, long offset, int origin )	
{
	return HandleToFile(stream)->Seek(offset, origin);
}
long	CVFileSysImp::vftell	( FILE_V *stream )			
{
	return HandleToFile(stream)->TellPos();
}

int		CVFileSysImp::vfeof	( FILE_V *stream )			
{
	return HandleToFile(stream)->TellPos() == HandleToFile(stream)->GetFileSize();
}
bool	CVFileSysImp::vfexists	(const char* filename)		
{
	return CVFileMgr::GetSingleton().IsFileExist(filename);
}
size_t	CVFileSysImp::vfsize	(FILE_V* stream)			
{
	return HandleToFile(stream)->GetFileSize();
}
FILE_V*	CVFileSysImp::FileToHandle(CVFile* pVFile)
{
	return (FILE_V*)pVFile;
}
CVFile* CVFileSysImp::HandleToFile(FILE_V* stream)
{
	return (CVFile*)stream;
}