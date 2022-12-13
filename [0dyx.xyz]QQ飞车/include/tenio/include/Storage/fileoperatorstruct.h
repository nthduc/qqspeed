#ifndef __FILEOPERATORSTRUCT_H__
#define __FILEOPERATORSTRUCT_H__

#define TENIO_FILEHASHLEN	16

namespace Tenio
{
	typedef struct _tgaLISTFILEINFO
	{
		TCHAR szFileName[MAX_PATH];
		BOOL  bIsMd5Valid;
		BYTE  byarMD5Hash[TENIO_FILEHASHLEN];
		BOOL  bIsFile;
		BOOL  bIsDirectory;
		DWORD dwSize;
	}LISTFILEINFO;
};

#endif