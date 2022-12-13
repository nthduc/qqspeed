#ifndef OV_GFILE_H /* general file operation. */
#define OV_GFILE_H

/*********************************************
#FUNCTION:	This head file definite the general file operation.
#AUTHOR:	Niezhiming
#DATE:		15-5-2002
**********************************************/


#ifdef __cplusplus
extern "C"
{
#endif

#define GF_MAP_CHAR_FROM		'\\'
#define GF_MAP_CHAR_TO			'_'

#define GF_MODE_READ			0x01
#define GF_MODE_WRITE			0x02

#define GF_MODE_CREATE			0x10
#define GF_MODE_TRUNCATE		0x20
#define GF_MODE_EXCL			0x40

#define GF_MAX_NAME			4096
#define GF_MAX_PATH			4096

#ifdef WIN32
typedef struct _stat			GFSTAT;
typedef struct _stat			*LPGFSTAT;
#else
typedef struct stat			GFSTAT;
typedef struct stat			*LPGFSTAT;
#endif

int gfpath_to_name(char* pszBuff, int iBuff, const char* pszPath, const char* pszPrefix);

int gfopen(const char* pszPath, int iMode);
int gfclose(int iFile);

int gfdelete(const char* pszPath);

int gfwrite(int iFile, const char* pszBuff, int iSize);
int gfread(int iFile, char* pszBuff, int iSize);

int gfload(int iFile, char** ppszBuff, int* piSize);
int gflload(const char* pszPath, char** ppszBuff, int* piSize);

int gfsync(int iFile);

int gflock(int iFile, int iOff, int iLen, int fBlock);
int gfunlock(int iFile, int iOff, int iLen);

int gfseek(int iFile, int iOff, int iOrigin);
int gfsize(int iFile);
int gftruncate(int iFile, int iSize);

int gfexist(const char* pszPath);
int gflsize(const char* pszPath);

int gfmkdir(const char* pszPath);
int gfcopy(const char* pszDst, const char* pszSrc, int fReplaceExist);

int gflstat(const char* pszPath, GFSTAT* pstStat);
int gfstat(int iFile, GFSTAT* pstStat);

#ifdef __cplusplus
}
#endif

#endif	/* OV_GFILE_H */
