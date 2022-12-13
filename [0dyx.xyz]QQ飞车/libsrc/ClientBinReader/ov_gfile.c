#include <stdio.h>
#include "ov_os.h"
#include "ov_gfile.h"

/* szPath can't be null. */
int gfpath_to_name(char* pszBuff, int iBuff, const char* pszPath, const char* pszPrefix)
{
	int i;
	int iLen;
	char* pszPtr;
	int iPrefix;
	int iPath;
	
	assert( pszBuff && pszPath && iBuff>0 );

	pszBuff[0]	=	0;

#ifdef WIN32 
	iPath	=	GetShortPathName(pszPath, NULL, 0);
#else
	iPath	=	strlen(pszPath);
#endif

	if( 0==iPath )
		return -1;

	if( (char*)0==pszPrefix )
		iPrefix	=	0;
	else
		iPrefix	=	strlen(pszPrefix);

	iLen	=	iPath + iPrefix;

	if( iLen>=iBuff )
		return -1;

	if( (char*)0==pszPrefix )
		pszPtr	=	pszBuff;
	else
	{
		strcpy(pszBuff, pszPrefix);
		pszPtr	=	pszBuff + iPrefix;
	}

#ifdef WIN32 
	if(0==GetShortPathName(pszPath, pszPtr, iBuff-iPrefix))
		return -1;
#else
	strcpy(pszPtr, pszPath);
#endif
	/* do mapping. */
	i	=	0;

	while(pszPtr[i])
	{
		if( GF_MAP_CHAR_FROM==pszPtr[i] )
			pszPtr[i]	=	GF_MAP_CHAR_TO;
		else
			pszPtr[i]	=	tolower(pszPtr[i]);

		i++;
	}

	pszPtr[i]	=	0;

	return 0;
}

int gfopen(const char* pszPath, int iMode)
{
#ifdef WIN32
	int iAccess;
	int iCreate;
	int iFile;
	SECURITY_ATTRIBUTES sa;

	iAccess	=	0;
	iCreate	=	0;

	sa.nLength		=	sizeof(sa);
	sa.bInheritHandle	=	FALSE;
	sa.lpSecurityDescriptor	=	0;

	if( iMode & GF_MODE_READ )
		iAccess	|=	GENERIC_READ;
	if( iMode & GF_MODE_WRITE )
		iAccess |=	GENERIC_WRITE;

	if( iMode & GF_MODE_CREATE )
	{
		if(iMode & GF_MODE_EXCL)
			iCreate	=	CREATE_NEW;
		else if( iMode & GF_MODE_TRUNCATE )
			iCreate	=	CREATE_ALWAYS;
		else
			iCreate	=	OPEN_ALWAYS;
	}
	else
	{
		if(iMode & GF_MODE_TRUNCATE)
			iCreate	=	TRUNCATE_EXISTING;
		else
			iCreate	=	OPEN_EXISTING;
	}

#ifdef WINNT
	sa.lpSecurityDescriptor	=	os_get_sd();
#endif

	iFile	=	(int)CreateFile( pszPath, (DWORD)iAccess, FILE_SHARE_READ | FILE_SHARE_WRITE,
					&sa,(DWORD) iCreate, FILE_FLAG_WRITE_THROUGH,NULL );

#ifdef WINNT
	os_free_sd(sa.lpSecurityDescriptor);
#endif

	if( INVALID_HANDLE_VALUE==(HANDLE)iFile )
		return -1;
	else
		return iFile;

#else
	int iAccess;
	int iCreate;
	int iFile;

	iAccess	=	0;
	iCreate	=	0;

	if( ( iMode & GF_MODE_READ ) &&
		( iMode & GF_MODE_WRITE ) )
		iAccess =	O_RDWR;
	else if( iMode & GF_MODE_READ )
		iAccess	=	O_RDONLY;
	else if( iMode & GF_MODE_WRITE )
		iAccess	=	O_WRONLY;
	else
		return -1;

	if( iMode & GF_MODE_CREATE )
		iCreate	|=	O_CREAT;

	if( iMode & GF_MODE_TRUNCATE )
		iCreate |=	O_TRUNC;

	if( iMode & GF_MODE_EXCL )
		iCreate |=	O_EXCL;

	return open(pszPath, iAccess | iCreate, 0666);
#endif

}

int gfclose(int iFile)
{
#ifdef WIN32
	if( -1==iFile )
		return 0;

	if(CloseHandle((HANDLE)iFile))
		return 0;
	else
		return -1;
#else
	if( -1==iFile )
		return 0;

	return close(iFile);
#endif
}


int gfdelete(const char* pszPath)
{
#ifdef WIN32
	if(DeleteFile(pszPath))
		return 0;
	else
		return -1;
#else
	return unlink(pszPath);
#endif
}


int gfwrite(int iFile, const char* pszBuff, int iSize)
{
#ifdef WIN32
	DWORD dwWritten;

	if( iSize<=0 )
		return -1;

	if(WriteFile((HANDLE)iFile, pszBuff, (DWORD)iSize, &dwWritten, NULL))
		return (int)dwWritten;
	else
		return -1;
#else
	return write(iFile, pszBuff, (size_t)iSize);
#endif
}

int gfread(int iFile, char* pszBuff, int iSize)
{
#ifdef WIN32
	DWORD dwRead;

	if( iSize<=0 )
		return -1;
	
	if(ReadFile((HANDLE)iFile, pszBuff, (DWORD)iSize, &dwRead, NULL))
		return (int)dwRead;
	else
		return -1;
#else
	return read( iFile, pszBuff, (size_t) iSize);
#endif
}

int gfload(int iFile, char** ppszBuff, int* piSize)
{
	int iSize;
	int iRead;
	int iRet;
	char* pszData;
	
	assert( ppszBuff && piSize );

	iSize	=	gfsize(iFile);

	if( iSize<0 )
		return -1;

	if( 0==iSize )
	{
		*ppszBuff	=	NULL;
		*piSize		=	0;
		
		return 0;
	}

	*piSize	=	iSize;

	pszData	=	(char*) calloc(1, iSize+1);

	if( NULL==pszData )
		return -1;

	iRead	=	gfread(iFile, pszData, iSize);

	if( iRead==iSize )
	{
		*ppszBuff	=	pszData;
		pszData[iSize]	=	'\0';
		iRet		=	0;
	}
	else
	{
		free(pszData);
		iRet		=	-1;
	}

	return iRet;
}

int gflload(const char* pszPath, char** ppszBuff, int* piSize)
{
	int iFile;
	int iRet;

	iFile	=	gfopen(pszPath, GF_MODE_READ);

	if( -1==iFile )
		return -1;

	iRet	=	gfload(iFile, ppszBuff, piSize);

	gfclose(iFile);

	return iRet;
}

int gfsync(int iFile)
{
#ifdef WIN32
	if( FlushFileBuffers((HANDLE)iFile) )
		return 0;
	else
		return -1;
#else
	return fsync(iFile);
#endif
}

int gflock(int iFile, int iOff, int iLen, int fBlock)
{
#ifdef WIN32
	int iRet;
	
	iRet	=	LockFile((HANDLE)iFile, (DWORD)iOff, 0, (DWORD)iLen, 0);

	if( iRet )
		return 0;
	else
		return -1;
#else
	int iRet;
	struct flock lock;

	lock.l_type	=	F_WRLCK;
	lock.l_whence	=	SEEK_SET;
	lock.l_start	=	iOff;
	lock.l_len	=	iLen;

	if( fBlock )
	{
		iRet	=	fcntl(iFile, F_SETLKW, &lock);
		return iRet;
	}
	else
	{
		iRet	=	fcntl(iFile, F_SETLK, &lock);
		return iRet;
	}
#endif
}

int gfunlock(int iFile, int iOff, int iLen)
{
#ifdef WIN32
	int iRet;

	iRet	=	UnlockFile((HANDLE)iFile, (DWORD)iOff, 0, (DWORD)iLen, 0);

	if( iRet )
		return 0;
	else
		return -1;
#else
	struct flock lock;

	lock.l_type	=	F_UNLCK;
	lock.l_whence	=	SEEK_SET;
	lock.l_start	=	iOff;
	lock.l_len	=	iLen;

	return fcntl(iFile, F_SETLK, &lock);
#endif
}

int gfseek(int iFile, int iOff, int iOrigin)
{
	int iMethod;
	int iRet;

#ifdef WIN32

	switch(iOrigin)
	{
	case SEEK_SET:
		iMethod	=	FILE_BEGIN;
		break;
	case SEEK_CUR:
		iMethod	=	FILE_CURRENT;
		break;
	case SEEK_END:
		iMethod	=	FILE_END;
		break;
	default:
		return -1;
	}
	
	iRet	=	(int) SetFilePointer((HANDLE)iFile, iOff, 0, iMethod);

	if( -1==iRet )
		return -1;
	else
		return iRet;
#else
	return lseek(iFile, iOff, iOrigin);
#endif
}

int gfsize(int iFile)
{
#ifdef WIN32
	return (int) GetFileSize((HANDLE)iFile, (DWORD*)0);
#else
	struct stat stInfo;
	
	if( -1==fstat(iFile, &stInfo) )
		return -1;

	return (int)stInfo.st_size;
#endif
}

int gflsize(const char* pszPath)
{
#ifdef WIN32
	struct _stat stInfo;

	if( -1==_stat(pszPath, &stInfo) )
		return -1;

	return (int)stInfo.st_size;
#else
	struct stat stInfo;
	
	if( -1==stat(pszPath, &stInfo) )
		return -1;

	return (int)stInfo.st_size;
#endif
}

int gftruncate(int iFile, int iSize)
{
#ifdef WIN32
	if( -1==gfseek(iFile, iSize, SEEK_SET) )
		return -1;

	if( SetEndOfFile((HANDLE)iFile) )
		return 0;
	else
		return -1;
#else
	if( -1==gfseek(iFile, iSize, SEEK_SET) )
		return -1;

	return ftruncate(iFile, iSize);
#endif
}

int gfexist(const char* pszPath)
{
#ifdef WIN32
	if(-1==GetFileAttributes(pszPath))
		return -1;
	else
		return 0;
#else
	struct stat stInfo;

	if( -1==stat(pszPath, &stInfo) )
		return -1;
	else
		return 0;
#endif
}

int gfmkdir(const char* pszPath)
{
#ifdef WIN32
	if( CreateDirectory(pszPath, NULL) )
		return 0;
	else
		return -1;
#else
	return mkdir(pszPath, S_IRWXU | S_IRWXG);
#endif
}

int gfcopy(const char* pszDst, const char* pszSrc, int fReplaceExist)
{
#ifdef WIN32
	if( CopyFile(pszSrc, pszDst, fReplaceExist) )
		return 0;
	else
		return -1;
#else
	char szBuff[0x1000];
	FILE* fpSrc;
	FILE* fpDst;
	int iRead;
	int iWrite;
	int iRet=0;

	if( 0==fReplaceExist )	/* make sure the file is nonexist. */
	{
		fpDst	=	fopen(pszDst, "rb");

		if( fpDst )
		{
			fclose(fpDst);
			return -1;
		}
	}

	fpSrc	=	fopen(pszSrc, "rb");
	if( (FILE*)0==fpSrc )
		return -1;

	fpDst	=	fopen(pszDst, "wb");
	if( (FILE*)0==fpDst )
	{
		fclose(fpSrc);
		return -1;
	}

	do
	{
		iRead	=	fread(szBuff, 1, 0x1000, fpSrc);

		if( iRead>0 )
		{
			iWrite	=	fwrite(szBuff, 1, iRead, fpDst);

			if( iWrite!=iRead )
			{
				iRet	=	-1;
				break;
			}
		}
	}
	while( iRead>0 );

	fclose(fpSrc);
	fclose(fpDst);

	return iRet;
#endif
}

int gflstat(const char* pszPath, GFSTAT* pstStat)
{
#ifdef WIN32
	return _stat(pszPath, pstStat);
#else
	return stat(pszPath, pstStat);
#endif
}

int gfstat(int iFile, GFSTAT* pstStat)
{
#ifdef WIN32
	return _fstat(iFile, pstStat);
#else
	return fstat(iFile, pstStat);
#endif
}
