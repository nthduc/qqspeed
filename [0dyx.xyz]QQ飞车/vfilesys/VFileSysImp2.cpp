////////////////////////////////////////////////
//     FileName:    VFileSysImp2.cpp
//     Author:      Fish
//     Date:        2006-12-13 10:32:39
//     Description: []
////////////////////////////////////////////////

#include "stdafx.h"
#include ".\vfilesysimp2.h"
#include "Vfilesys.h"
#include "txtCtrlr.h"
#include "common.h"
#include "fileop.h"
#include "VFSHelper.h"

CVFileSysImp2::CVFileSysImp2(void)
{
}

CVFileSysImp2::~CVFileSysImp2(void)
{
	AppUnInit();
}

//IGameService
STDMETHODIMP CVFileSysImp2::AppInit(IGameApp* App)
{
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
				LPCSTR pszRoot = pszItem;
				char strVFS[MAX_PATH] = "";
				_makepath(strVFS, NULL, NULL, strVFS, ".VFS");
				if ( FileExists(strVFS) )
				{
					pszRoot = strVFS;
				}
				VROOT_h hRoot = m_VFSHelper.MountRoot(pszRoot, pszItem);
				if ( hRoot == VFS_INVALID_HANDLE_VALUE )
				{
					LogText("MountRoot(%s) Failed\n", pszItem);
				}
				else
				{
					m_vecRoot.push_back(hRoot);
				}
			}
		}
	}
	return S_OK;
}

STDMETHODIMP CVFileSysImp2::AppUnInit()
{
	return S_OK;
}

FILE_V*	CVFileSysImp2::vfopen( const char* filename, const char* mode )	
{
	std::vector<VROOT_h>::iterator it = m_vecRoot.begin();
	for ( ; it != m_vecRoot.end(); ++it )
	{
		VFS_HANDLE hFile = m_VFSHelper.OpenFileVR((VFS_HANDLE)*it, (char*)filename);
		if ( VFS_SUCCEEDED(hFile) )
		{
			return (FILE_V*)hFile;
		}
	}
	return NULL;
}

int	CVFileSysImp2::vfclose( FILE_V* stream )							
{
	m_VFSHelper.CloseFileVR((VFS_HANDLE)stream);
	return 0;
}

size_t	CVFileSysImp2::vfread( void* buffer, size_t size, size_t count, FILE_V* stream )		
{
	return m_VFSHelper.ReadFileVR((VFS_HANDLE)stream, size, count, buffer, size * count);
}

size_t	CVFileSysImp2::vfwrite( const void *buffer, size_t size, size_t count, FILE_V *stream )	
{
	return 0;
}

int	CVFileSysImp2::vfseek( FILE_V *stream, long offset, int origin )	
{
	return m_VFSHelper.FileSeekVR((VFS_HANDLE)stream, offset, origin);
}

long CVFileSysImp2::vftell( FILE_V *stream )			
{
	return m_VFSHelper.GetFilePosVR((VFS_HANDLE)stream);
}

int	CVFileSysImp2::vfeof( FILE_V *stream )			
{
	return m_VFSHelper.GetFilePosVR((VFS_HANDLE)stream) >= m_VFSHelper.GetFileSizeVR((VFS_HANDLE)stream);
}

bool CVFileSysImp2::vfexists(const char* filename)		
{
	std::vector<VROOT_h>::iterator it = m_vecRoot.begin();
	for ( ; it != m_vecRoot.end(); ++it )
	{
		FIND_h hFind = m_VFSHelper.FindFirstFile(*it, filename);
		if ( VFS_SUCCEEDED(hFind) )
		{
			return true;
		}
	}
	return false;
}
size_t CVFileSysImp2::vfsize(FILE_V* stream)			
{
	return m_VFSHelper.GetFileSizeVR((VFS_HANDLE)stream);
}

