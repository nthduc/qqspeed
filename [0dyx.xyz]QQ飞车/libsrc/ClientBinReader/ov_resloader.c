
#include "ov_os.h"
#include "ov_res.h"
#include "ov_gfile.h"
#include "ov_resloader.h"
#include "vfile.h"

int rl_comp_func(const void* pv1, const void* pv2)
{
	return (*(int*)pv1) - (*(int*)pv2);
}

int rl_check_head(LPRESHEAD pstHead, int iData)
{
	assert( pstHead );

	if( RES_MAGIC!=pstHead->iMagic )
		return -1;

	if( RES_VERSION!=pstHead->iVersion )
		return -1;

	if( pstHead->iUnit<=0 )
		return -1;

	if( pstHead->iCount<0 )
		return -1;

	if( sizeof(RESHEAD) + pstHead->iUnit*pstHead->iCount!=iData )
		return -1;

	return 0;
}

void rl_set_head(LPRESHEAD pstHead, int iCount, int iUnit)
{
	assert( pstHead );

	pstHead->iMagic	=	RES_MAGIC;
	pstHead->iVersion=	RES_VERSION;
	pstHead->iCount =	iCount;
	pstHead->iUnit	=	iUnit;
}

int rl_sload(const char* pszPath, char* pszBuff, int iBuff, int iUnit)
{
	LPRESHEAD pstHead;
	char* pszData;
	char* pszSrc;
	char* pszDst;
	int iData;
	int iCount;
	int iSrcUnit;
	int i;

	if( gflload(pszPath, &pszData, &iData)<0 || !pszData )
		return -1;	/* can not read. */

	pstHead	=	(LPRESHEAD)pszData;

	if( rl_check_head(pstHead, iData)<0 || 
	    pstHead->iUnit>iUnit || pstHead->iCount*iUnit>iBuff )
	{
		free(pszData);
		return -1;
	}
	
	iCount	=	pstHead->iCount;
	iSrcUnit=	pstHead->iUnit;
	pszSrc	=	pszData + sizeof(RESHEAD);
	pszDst	=	pszBuff;

	for(i=0; i<iCount; i++)
	{
		memcpy(pszDst, pszSrc, iSrcUnit);
		
		pszDst	+=	iUnit;
		pszSrc	+=	iSrcUnit;
	}

	free( pszData );

	return iCount;
}


int gfloadvf(CVFile* pFile, char** ppszBuff, int* piSize)
{
	int iSize;
	int iRead;
	int iRet;
	char* pszData;
	
	assert( ppszBuff && piSize );
	
	/*
	iSize	=	gfsize(iFile);
	*/

	iSize = pFile->GetFileSize();

	if( iSize < 0 )
	{
		return -1;
	}

	if( 0 == iSize )
	{
		*ppszBuff	=	NULL;
		*piSize		=	0;
		
		return 0;
	}
	
	*piSize	=	iSize;
	
	pszData	=	(char*) calloc(1, iSize+1);
	
	if( NULL==pszData )
	{
		return -1;
	}
	/*
	iRead	=	gfread(iFile, pszData, iSize);
	*/
	
	bool bRead = pFile->Read(pszData,iSize);

	if( bRead )
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

int gflloadvf(const char* pszPath, char** ppszBuff, int* piSize)
{
	int iFile;
	int iRet;
	
	CVFile* pFile = CVFileMgr::GetSingletonPtr()->OpenFile(pszPath,CVFile::READ);

	if(NULL == pFile)
	{
		return -1;
	}
	
	/*
	iFile	=	gfopen(pszPath, GF_MODE_READ);
	
	if( -1==iFile )
		return -1;
	
	iRet	=	gfload(iFile, ppszBuff, piSize);
	*/

	iRet	=	gfloadvf(pFile, ppszBuff, piSize);
	
	/*
	gfclose(iFile);
	*/

	pFile->Close();

	return iRet;
}

int rl_cload(void** ppvBuff, const char* pszPath, int *piBuff, int *piUnit)
{
	RESHEAD stHead;
	char* pszData;
	char* pszSrc;
	char* pszDst;
	int iFile;
	int iData;
	int iCount;
	int iSrcUnit;
	int i;

	iFile	=	gfopen(pszPath, GF_MODE_READ);

	if( -1==iFile )
		return -1;

	if( sizeof(stHead)!=gfread(iFile, (char*)&stHead, sizeof(stHead)) )
	{
		gfclose(iFile);
		return -1;
	}

	iData	=	stHead.iCount * stHead.iUnit + sizeof(stHead);

	if( rl_check_head(&stHead, iData)<0 )
	{
		gfclose(iFile);
		return -1;
	}

	if( piBuff )
		*piBuff	=	stHead.iCount * stHead.iUnit;

	if( piUnit )
		*piUnit	=	stHead.iUnit;

	pszData	=	(char*) malloc(stHead.iCount*stHead.iUnit);

	if( NULL==pszData )
	{
		gfclose(iFile);
		return -1;
	}

	iData	=	gfread(iFile, pszData, stHead.iCount*stHead.iUnit);

	gfclose(iFile);

	if( iData!=stHead.iCount*stHead.iUnit )
	{
		free(pszData);
		return -1;
	}
	else
	{
		*ppvBuff	=	pszData;
		return 0;
	}
}

int rl_sloadvf(const char* pszPath, char* pszBuff, int iBuff, int iUnit)
{
	LPRESHEAD pstHead;
	char* pszData;
	char* pszSrc;
	char* pszDst;
	int iData;
	int iCount;
	int iSrcUnit;
	int i;

	if( gflloadvf(pszPath, &pszData, &iData)<0 || !pszData )
		return -1;	/* can not read. */

	pstHead	=	(LPRESHEAD)pszData;

	if( rl_check_head(pstHead, iData)<0 || 
	    pstHead->iUnit>iUnit || pstHead->iCount*iUnit>iBuff )
	{
		free(pszData);
		return -1;
	}
	
	iCount	=	pstHead->iCount;
	iSrcUnit=	pstHead->iUnit;
	pszSrc	=	pszData + sizeof(RESHEAD);
	pszDst	=	pszBuff;

	for(i=0; i<iCount; i++)
	{
		memcpy(pszDst, pszSrc, iSrcUnit);
		
		pszDst	+=	iUnit;
		pszSrc	+=	iSrcUnit;
	}

	free( pszData );

	return iCount;
}

int rl_cloadvf(void** ppvBuff, const char* pszPath, int *piBuff, int *piUnit)
{
	RESHEAD stHead;
	char* pszData;
	char* pszSrc;
	char* pszDst;
	int iFile;
	int iData;
	int iCount;
	int iSrcUnit;
	int i;

	CVFile* pFile = CVFileMgr::GetSingletonPtr()->OpenFile(pszPath,CVFile::READ);

	if(NULL == pFile)
	{
		return -1;
	}

	/*
	if( sizeof(stHead)!=gfread(iFile, (char*)&stHead, sizeof(stHead)) )
	{
		gfclose(iFile);
		return -1;
	}
	*/

	if(!pFile->Read(&stHead,sizeof(stHead)))
	{
		pFile->Close();
		return -1;
	}


	iData	=	stHead.iCount * stHead.iUnit + sizeof(stHead);

	if( rl_check_head(&stHead, iData)<0 )
	{
		pFile->Close();
		return -1;
	}

	if( piBuff )
		*piBuff	=	stHead.iCount * stHead.iUnit;

	if( piUnit )
		*piUnit	=	stHead.iUnit;

	pszData	=	(char*) malloc(stHead.iCount*stHead.iUnit);

	if( NULL==pszData )
	{
		pFile->Close();
		return -1;
	}

	/*
	iData	=	gfread(iFile, pszData, stHead.iCount*stHead.iUnit);

	gfclose(iFile);
	*/

	bool bRead = pFile->Read(pszData,stHead.iCount*stHead.iUnit);

	pFile->Close();

	if( !bRead )
	{
		free(pszData);
		return -1;
	}
	else
	{
		*ppvBuff	=	pszData;
		return 0;
	}
}
