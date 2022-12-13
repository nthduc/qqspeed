/*
 *	Display.cpp
 *  DirectDraw Display functions.
 *
 *  Jim, 2003.9.10.
 */
#include <windows.h>
#include "stdio.h"
#include "Display.h"
//#include "IDisplayD3D91.h"


//////////////////////////////////////////////////////////////////////////
// TImage functions

TImage*	CreateImage(int w, int h, int attrib)
{
	TImage *tmp = NULL;
	int nBytes = 2;
	
	if(0 == w || 0 == h)
	{
		return NULL;
	}
	
	if ( attrib == imageAttrib8888 )
	{
	    nBytes = 4;
	}    
	if(NULL == (tmp = (TImage*)malloc(sizeof(TImage) + w * h * nBytes - 4)))	
	{
		return NULL;
	}

	tmp->w = w;
	tmp->h = h;
	tmp->pitch = w * nBytes;
	tmp->attrib = attrib;

	//modify+lincoln
	//去处图像切割后的黑边
	//{{{
	//memset(tmp->data, 0, tmp->pitch);
	memset(tmp->data, 0, w * h * nBytes);
	//}}}
	

	return tmp;
}

void	DestroyImage(TImage **pImage)
{
	if(*pImage)
	{
		free(*pImage);
		*pImage = NULL;
	}
}

TImage*	DuplicateImage(TImage *pImage)
{
	TImage *tmp = NULL;

	if(!pImage)
	{
		return NULL;
	}

	tmp = CreateImage(pImage->w, pImage->h, pImage->attrib);
	memcpy(tmp->data, pImage->data, tmp->pitch * pImage->h);

	return tmp;
}

void	ClearImage(TImage *pImage)
{
	if(pImage)
	{
		memset(pImage->data, 0, pImage->pitch * pImage->h);
	}
}

void	BltImage(TImage *pDestImage, int left, int top, TImage *pSrcImage)
{
	if(!pDestImage || !pSrcImage)
	{
		return;
	}

	if(pDestImage->attrib != pSrcImage->attrib)
	{
		return;
	}

	int y;
	unsigned char *psrc, *pdest;
	int nBytes = pDestImage->pitch / pDestImage->w;
	psrc = pSrcImage->data + pSrcImage->pitch * (pSrcImage->h - 1);
	pdest = pDestImage->data + pDestImage->pitch * (top + pSrcImage->h - 1)
		+ left * nBytes;
	for(y = pSrcImage->h-1; y>=0; y--)
	{
		memcpy(pdest, psrc, pSrcImage->w * nBytes);
		psrc -= pSrcImage->w * nBytes;
		pdest -= pDestImage->w * nBytes;
	}
}

