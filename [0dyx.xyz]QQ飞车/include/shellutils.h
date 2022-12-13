#ifndef __SHELLUTILS_H__
#define __SHELLUTILS_H__

inline BOOL SHDeleteFile(LPCSTR file)
{
	char szBuffer[MAX_PATH];
	memset(szBuffer, 0, MAX_PATH);
	strcpy(szBuffer, file);

	SHFILEOPSTRUCT fo;
	memset(&fo, 0, sizeof(fo));
	fo.wFunc = FO_DELETE;
	fo.pFrom = szBuffer;
	fo.fFlags = FOF_NOCONFIRMATION | FOF_NOERRORUI | FOF_SILENT;
	int op = SHFileOperation(&fo);
	return op == 0;
}

inline BOOL SHRenameFile(LPCSTR src, LPCSTR dst)
{
	char szBufferSrc[MAX_PATH];
	memset(szBufferSrc, 0, MAX_PATH);
	strcpy(szBufferSrc, src);

	char szBufferDst[MAX_PATH];
	memset(szBufferDst, 0, MAX_PATH);
	strcpy(szBufferDst, dst);

	SHFILEOPSTRUCT fo;
	memset(&fo, 0, sizeof(fo));
	fo.wFunc = FO_RENAME;
	fo.pFrom = szBufferSrc;
	fo.pTo = szBufferDst;
	fo.fFlags = FOF_NOCONFIRMATION | FOF_NOERRORUI | FOF_SILENT;
	int op = SHFileOperation(&fo);
	return op == 0;	
}

#endif