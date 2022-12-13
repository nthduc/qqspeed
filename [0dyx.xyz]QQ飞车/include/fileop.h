/* -------------------------------------------------------------------------
//	文件名		：	fileop.h
//	创建者		：	FishHong
//	创建时间	：	2005-10-21 下午 03:15:27
//	功能描述	：	
//
//	$Id: $
// -----------------------------------------------------------------------*/
#ifndef __FILEOP_H__
#define __FILEOP_H__
// -------------------------------------------------------------------------
#include "stdlib.h"
#include "windef.h"

#pragma  warning(disable:4267)

inline BOOL GetModulePath(HMODULE hModule, LPSTR ModulePath, UINT Size = MAX_PATH)
{
	char szModuleName[MAX_PATH];
	DWORD dwLen = GetModuleFileName(hModule, szModuleName, MAX_PATH);
	if ( dwLen > 0 )
	{
		char driver[4], fpath[MAX_PATH], fname[MAX_PATH], ext[MAX_PATH];
		_splitpath(szModuleName, driver, fpath, fname, ext);

		char szModulePath[MAX_PATH] = "";
		_makepath(szModulePath, driver, fpath, NULL, NULL);
		strncpy(ModulePath, szModulePath, __min(Size, strlen(szModulePath)+1));
		return TRUE;
	}
	return FALSE;
}

inline BOOL FullName(LPCSTR FileName, LPSTR FullName, UINT Size = MAX_PATH)
{
	char szModulePath[MAX_PATH] = "";
	if ( !GetModulePath(NULL, szModulePath) )
		return FALSE;
	char szFullName[MAX_PATH] = "";
	_makepath(szFullName, NULL, szModulePath, FileName, NULL);
	strncpy(FullName, szFullName, __min(Size, strlen(szFullName)));
	return TRUE;
}

inline bool FileExists(LPCSTR fname)
{
	WIN32_FIND_DATA fData;
	HANDLE hFind = FindFirstFile(fname, &fData);
	if ( hFind == INVALID_HANDLE_VALUE )
		return false;
	FindClose(hFind);
	return true;
}

inline bool DirectoryExists(LPCSTR dir)
{
	char szFilter[MAX_PATH] = "\0";
	_makepath(szFilter, NULL, dir, "*.*", NULL);
	return FileExists(szFilter);
}

inline void CreateMultiDirectory(LPCSTR fpath)
{
	char szPath[MAX_PATH] = "\0";
	strcpy(szPath, fpath);

	if ( DirectoryExists(szPath) )
		return;
	int i = strlen(szPath) - 1;
	while(szPath[i] == '\\' || szPath[i] == '/' )
	{
		szPath[i] = 0;
		i--;
	}
	if ( !CreateDirectory(szPath, NULL) )
	{
		char szDriver[4] = "\0", szDir[MAX_PATH] = "\0";
		_splitpath(szPath, szDriver, szDir, NULL, NULL);
		if ( strlen(szDir) > 0 )
		{
			char szParentDir[MAX_PATH] = "\0";
			_makepath(szParentDir, szDriver, szDir, NULL, NULL);
			CreateMultiDirectory(szParentDir);
			CreateDirectory(szPath, NULL);
		}
	}
}

// add by hover
inline void CopyString( char *pdest, int maxbuf, const char *psrc )
{
	if( psrc == NULL )
	{
		pdest[0] = 0;
		return;
	}
	strncpy( pdest, psrc, maxbuf );
	pdest[maxbuf-1] = 0;

	int len = strlen( pdest );
	if( pdest[len-1] == '\n' ) pdest[len-1] = '\0';
}

// -------------------------------------------------------------------------
//	$Log: $

#endif /* __FILEOP_H__ */
