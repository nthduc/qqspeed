// VFSHelper.cpp: implementation of the CVFSHelper class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "vfstool.h"
#include "VFSHelper.h"
#include "vfs_i.h"
#include "fileop.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

BOOL MakeVFSFromDir(LPCSTR lpszPath, LPCSTR lpszName)
{
	IFileSysMgr* pMgr = ICreateFileSysMgr();
	if ( !pMgr )
	{
		LogText("ICreateFileSysMgr Failed\n");
		return FALSE;
	}
	VROOT_h hRoot1 = pMgr->OpenRoot(lpszPath);
	if ( hRoot1 == VFS_INVALID_HANDLE_VALUE )
	{
		LogText("OpenRoot(%d) Failed\n", lpszPath);
		return FALSE;
	}

	VROOT_h hRoot2 = pMgr->OpenRoot(lpszName, VFS_OPENROOT_MODE_WRITE_ALWAYS);
	if ( hRoot2 == VFS_INVALID_HANDLE_VALUE )
	{
		LogText("OpenRoot(%d) Failed\n", lpszName);
		return FALSE;
	}

	FIND_h hFind = pMgr->FindFirstFile(hRoot1, "");
	if ( hFind != VFS_INVALID_HANDLE_VALUE )
	{
		while(1)
		{
			LPCSTR lpszFileName = pMgr->GetFindPathName(hFind);
			if ( !pMgr->CopyFile(hRoot1, lpszFileName, hRoot2, lpszFileName) )
			{
				LogText("CopyFile(%d) Failed\n", lpszFileName);
			}
			if ( !pMgr->FindNextFile(hFind) )
				break;
		}
	}
	pMgr->FindClose(hFind);
	pMgr->CloseRoot(hRoot1);
	pMgr->CloseRoot(hRoot2);
	return TRUE;
}