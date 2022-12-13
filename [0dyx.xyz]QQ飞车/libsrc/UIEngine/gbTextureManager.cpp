//#include "../stdafx.h"
#include "gbTextureManager.h"
#include "NiSourceTexture.h"
#include "NiDynamicTexture.h"
#include "NiRenderedTexture.h"
#include "NiDX9Renderer.h"
#include "NiDX9SourceTextureData.h"
#include "Utility.h"
#include "NiDevImageQuantizer.h"
#include "gbTextureCodecEx.h"

#define REMOVED_TEXTURE 1000
namespace GraphicsUtilities
{
	//////////////////////////////////////////////////////////////////////////
	// Graphics utilities

	enum
	{
		A_MASK_1555	= 0x8000,
		R_MASK_1555 = 0x7c00,
		G_MASK_1555 = 0x03e0,
		B_MASK_1555 = 0x001f,

		A_MASK_4444 = 0xf000,
		R_MASK_4444 = 0x0f00,
		G_MASK_4444 = 0x00f0,
		B_MASK_4444 = 0x000f,

		//add+lincoln
		//[2005:4:22] [13:45]
		//增加对8888格式的处理
		//{{{
		A_MASK_8888 = 0xff000000,
		R_MASK_8888 = 0x00ff0000,
		G_MASK_8888 = 0x0000ff00,
		B_MASK_8888 = 0x000000ff,
		//}}}
	};


	USHORT	ComposeHi1555(BYTE a, BYTE r, BYTE g, BYTE b)
	{
		return (USHORT)
			( (((USHORT)a<<8) & A_MASK_1555) 
			| (((USHORT)r<<7) & R_MASK_1555) 
			| (((USHORT)g<<2) & G_MASK_1555) 
			| (((USHORT)b>>3) & B_MASK_1555) 
			);
	}

	USHORT	ComposeHi4444(BYTE a, BYTE r, BYTE g, BYTE b)
	{
		return (USHORT)
			( (((USHORT)a<<8) & A_MASK_4444) 
			| (((USHORT)r<<4) & R_MASK_4444) 
			| (((USHORT)g   ) & G_MASK_4444) 
			| (((USHORT)b>>4) & B_MASK_4444) );
	}

	ULONG   ComposeHi8888(BYTE a, BYTE r, BYTE g, BYTE b)
	{
		return (ULONG)
			( ((ULONG)a<<24) 
			| ((ULONG)r<<16)
			| ((ULONG)g<<8)
			| ((ULONG)b<<0)
			);
	}


	void	ReduceHi1555(USHORT hi, BYTE *a, BYTE *r, BYTE *g, BYTE *b)
	{
		*a = (BYTE)( (hi & A_MASK_1555) ? 255 : 0);
		*r = (BYTE)( (hi & R_MASK_1555) >> 7 );
		*g = (BYTE)( (hi & G_MASK_1555) >> 2 );
		*b = (BYTE)( (hi & B_MASK_1555) << 3 );
	}

	void	ReduceHi4444(USHORT hi, BYTE *a, BYTE *r, BYTE *g, BYTE *b)
	{
		*a = (BYTE)( (hi & A_MASK_4444) >> 8 );
		*r = (BYTE)( (hi & R_MASK_4444) >> 4 );
		*g = (BYTE)( (hi & G_MASK_4444)      );
		*b = (BYTE)( (hi & B_MASK_4444) << 4 );
	}

	//add+lincoln
	//[2005:4:22] [13:45]
	//增加对8888格式的处理
	//{{{
	void	ReduceHi8888(ULONG hi, BYTE *a, BYTE *r, BYTE *g, BYTE *b)
	{
		*a = (BYTE)( (hi & A_MASK_8888) >> 24 );
		*r = (BYTE)( (hi & R_MASK_8888) >> 16 );
		*g = (BYTE)( (hi & G_MASK_8888) >> 8  );
		*b = (BYTE)( (hi & B_MASK_8888) >> 0 );
	}
	//}}}
}

gbTextureManager* gbTextureManager::ms_pManager = NULL;
gbTextureManager* gbTextureManager::GetManager()
{
	return ms_pManager?ms_pManager:(new gbTextureManager());
}

gbTextureManager::gbTextureManager( ) 
{
	ms_pManager = this;
}

gbTextureManager::~gbTextureManager()
{
	for(unsigned int i = 0;i<m_TexturePool.GetSize();i++)
	{
		ReleaseTexture(i);
		delete m_TexturePool.GetAt(i);
	}
	m_TexturePool.RemoveAll();
}

bool gbTextureManager::Load( const char *path,unsigned int& TextureID )
{
	TextureRect* ptexRect = new TextureRect();
	ptexRect->strFileName.assign(path) ;
	for ( int i = 0 ; i < m_TexturePool.GetSize() ; ++i)
	{
         TextureRect* pATexRect = m_TexturePool.GetAt(i) ;
         if (pATexRect->strFileName == ptexRect->strFileName)
         {
             TextureID = i ;
			 return false ;
         }
	}

	char* pcExt = strrchr(path, '.');
	if (toupper(pcExt[1]) == 'D' && toupper(pcExt[2]) == 'D' &&	toupper(pcExt[3]) == 'S')
	{
		NiTexture::FormatPrefs kFormatPerfs; 
		kFormatPerfs.m_ePixelLayout = NiTexture::FormatPrefs::COMPRESSED;
		kFormatPerfs.m_eMipMapped = NiTexture::FormatPrefs::NO ;
		ptexRect->spTexture = NiSourceTexture::Create( path, kFormatPerfs );
	}
	else
		ptexRect->spTexture = NiSourceTexture::Create(path);

	NiSourceTexture* pTex = NiDynamicCast(NiSourceTexture,ptexRect->spTexture);
	pTex->LoadPixelDataFromFile();
	if(ptexRect->spTexture)
	{

		//ptexRect->spScreenTexture = NiNew NiScreenTexture(ptexRect->spTexture);
		//ptexRect->spScreenTexture->RemoveAllScreenRects();
		//ptexRect->uiIndex = ptexRect->spScreenTexture->AddNewScreenRect((short)0,(short)0,(USHORT)ptexRect->spTexture->GetWidth(),
		//		(USHORT)ptexRect->spTexture->GetHeight(),(USHORT)0,(USHORT)0);
		//ptexRect->spScreenTexture->MarkAsChanged(NiScreenTexture::EVERYTHING_MASK);
		//TextureID = m_TexturePool.AddFirstEmpty(ptexRect) ;

		ptexRect->bIsChanged = false;
		ptexRect->iLeft = 0;
		ptexRect->iTop = 0;
		ptexRect->iWidth = ptexRect->spTexture->GetWidth();
		ptexRect->iHeight = ptexRect->spTexture->GetHeight();
		ptexRect->dwColor = 0xffffffff;
		TextureID = m_TexturePool.AddFirstEmpty(ptexRect) ;
	}
	else
	{
		TextureID = INVALID_IMGID;
	}
	return true;
}

unsigned int gbTextureManager::GetHeight(unsigned int TextureID)
{	
	return GetTextureRect(TextureID)->spTexture->GetHeight();
}

unsigned int gbTextureManager::GetWidth(unsigned int TextureID)
{
	return GetTextureRect(TextureID)->spTexture->GetWidth();
}

bool gbTextureManager::ReleaseTexture(unsigned int TextureID)
{
    TextureRect* prect = GetTextureRect(TextureID);
	if(NULL == prect)
		return true;
	prect->spTexture = NULL;
	m_TexturePool.SetAt(TextureID,NULL);
	return true;
}

TextureRect* gbTextureManager::GetTextureRect(unsigned int TextureID)
{
	if(TextureID >= m_TexturePool.GetSize())
		return NULL;
	return m_TexturePool.GetAt(TextureID);
}

bool gbTextureManager::CreateSourceTexture(unsigned int uiWidth, unsigned int uiHeight,int  attrib,unsigned int& TextureID)
{
	TextureRect* ptexRect = new TextureRect();

	NiPixelDataPtr pPixel = NiNew NiPixelData(uiWidth,uiHeight,NiPixelFormat::BGRA4444);
	ptexRect->spTexture = NiSourceTexture::Create(pPixel);
	if(ptexRect->spTexture)
	{
		//ptexRect->spScreenTexture = NiNew NiScreenTexture(ptexRect->spTexture);
		//ptexRect->spScreenTexture->RemoveAllScreenRects();
		//ptexRect->uiIndex = ptexRect->spScreenTexture->AddNewScreenRect((short)0,(short)0,(USHORT)ptexRect->spTexture->GetWidth(),
		//	(USHORT)ptexRect->spTexture->GetHeight(),(USHORT)0,(USHORT)0);
		//ptexRect->spScreenTexture->MarkAsChanged(NiScreenTexture::EVERYTHING_MASK);
		//TextureID = m_TexturePool.AddFirstEmpty(ptexRect) ;
		ptexRect->bIsChanged = false;
		ptexRect->iLeft = 0;
		ptexRect->iTop = 0;
		ptexRect->iWidth = ptexRect->spTexture->GetWidth();
		ptexRect->iHeight = ptexRect->spTexture->GetHeight();
		ptexRect->dwColor = 0xffffffff;
		TextureID = m_TexturePool.AddFirstEmpty(ptexRect) ;
	}	
	else
	{
		TextureID = INVALID_IMGID;
	}
	//Log("TextureID:%d\n",TextureID);
	return true;
}

IMGID gbTextureManager::CreateRenderImage(int uiWidth, int uiHeight)
{
	TextureRect* ptexRect = new TextureRect();
	unsigned int TextureID;
	NiTexture::FormatPrefs kFormatPerfs; 
	kFormatPerfs.m_ePixelLayout = NiTexture::FormatPrefs::TRUE_COLOR_32;
	kFormatPerfs.m_eMipMapped = NiTexture::FormatPrefs::NO;
	kFormatPerfs.m_eAlphaFmt = NiTexture::FormatPrefs::SMOOTH;
	ptexRect->spTexture = NiRenderedTexture::Create(uiWidth,uiHeight,NiRenderer::GetRenderer(),kFormatPerfs);

	if(ptexRect->spTexture)
	{
		ptexRect->bIsChanged = false;
		ptexRect->iLeft = 0;
		ptexRect->iTop = 0;
		ptexRect->iWidth = ptexRect->spTexture->GetWidth();
		ptexRect->iHeight = ptexRect->spTexture->GetHeight();
		ptexRect->dwColor = 0xffffffff;
		TextureID = m_TexturePool.AddFirstEmpty(ptexRect) ;
	}	
	else
	{
		TextureID = INVALID_IMGID;
	}
	//Log("TextureID:%d\n",TextureID);
	return TextureID;
}



void gbTextureManager::UpdateSourceTexture(unsigned int uiTop,unsigned int uiLeft,TBitImage* pImg,unsigned int& TextureID)
{
	TextureRect* pImgRect = GetTextureRect(TextureID);
	NiSourceTexture* pTex = NiDynamicCast(NiSourceTexture,pImgRect->spTexture);
	pTex->SetStatic(false);
	NiPixelDataPtr spPixdata = pTex->GetSourcePixelData();
	int nBytes = pImg->pitch / pImg->w;
	BYTE a, r, g, b;
	switch(pImg->attrib) {
	case 0://1555
		for (int y = 0; y < pImg->h; y++)
		{
			for (int x = 0; x < pImg->w; x++)
			{
				USHORT* lpPtr_16 = (USHORT*)((BYTE *)pImg->data + y*pImg->pitch + x*nBytes);
				USHORT* pPix = (USHORT*)(*spPixdata)(uiLeft + x,uiTop + y);
				GraphicsUtilities::ReduceHi1555(*lpPtr_16,&a,&r,&g,&b);
				*pPix = GraphicsUtilities::ComposeHi4444(a,r,g,b);
			}
		}
		break;
	case 1://4444
		for (int y = 0; y < pImg->h; y++)
		{
			for (int x = 0; x < pImg->w; x++)
			{
				USHORT* lpPtr_16 = (USHORT*)((BYTE *)pImg->data + y*pImg->pitch + x*nBytes);
				USHORT* pPix = (USHORT*)(*spPixdata)(uiLeft + x,uiTop + y);
				*pPix = *lpPtr_16;
			}
		}
		break;
	case 2://8888
		for (int y = 0; y < pImg->h; y++)
		{
			for (int x = 0; x < pImg->w; x++)
			{
				BYTE* pPixAddress = (BYTE*)(*((UINT*)pImg->data));
				UINT* lpPtr_16 = (UINT*)((BYTE *)pPixAddress + y*pImg->pitch + x*nBytes);
				USHORT* pPix = (USHORT*)(*spPixdata)(uiLeft + x,uiTop + y);
				GraphicsUtilities::ReduceHi8888(*lpPtr_16,&a,&r,&g,&b);
				*pPix = GraphicsUtilities::ComposeHi4444(a,r,g,b);
			}
		}
		break;
	}
	pImgRect->bIsChanged = true;
	spPixdata->MarkAsChanged();
}

void gbTextureManager::UpdateSourceTexture(unsigned int uiTop,unsigned int uiLeft,TBitImage* pImg, unsigned int& TextureID ,RECT *rcRegion )
{
	TextureRect* pImgRect = GetTextureRect(TextureID);
	NiSourceTexture* pTex = NiDynamicCast(NiSourceTexture,pImgRect->spTexture);
	
//	pTex->SetStatic(false);
	pTex->SetStatic(true);

	NiPixelDataPtr spPixdata = pTex->GetSourcePixelData();
	int nBytes = pImg->pitch / pImg->w;

	if ( !rcRegion )
	{
		return ;
	}

    if( ( pImg->w < rcRegion->right)||( pImg->h < rcRegion->bottom ) )
    {
		return ;
    }

	BYTE a, r, g, b;
	switch(pImg->attrib) 
	{
	case 0://1555
		for (int y = rcRegion->top ; y <= rcRegion->bottom ; y++)
		{
			for (int x = rcRegion->left; x <= rcRegion->right ; x++)
			{
				USHORT* lpPtr_16 = (USHORT*)((BYTE *)pImg->data + y*pImg->pitch + x*nBytes);
				USHORT* pPix = (USHORT*)(*spPixdata)(uiLeft + x,uiTop + y);
				GraphicsUtilities::ReduceHi1555(*lpPtr_16,&a,&r,&g,&b);
				*pPix = GraphicsUtilities::ComposeHi4444(a,r,g,b);
			}
		}
		break;
	case 1://4444
		for (int y = rcRegion->top ; y <= rcRegion->bottom ; y++)
		{
			for (int x = rcRegion->left; x <= rcRegion->right ; x++)
			{
				USHORT* lpPtr_16 = (USHORT*)((BYTE *)pImg->data + y*pImg->pitch + x*nBytes);
				USHORT* pPix = (USHORT*)(*spPixdata)(uiLeft + x,uiTop + y);
				*pPix = *lpPtr_16;
			}
		}
		break;
	case 2://8888
		for (int y = rcRegion->top ; y <= rcRegion->bottom ; y++)
		{
			for (int x = rcRegion->left; x <= rcRegion->right ; x++)
			{
				BYTE* pPixAddress = (BYTE*)(*((UINT*)pImg->data));
				UINT* lpPtr_16 = (UINT*)((BYTE *)pPixAddress + (uiTop + y)*pImg->pitch + (uiLeft + x)*nBytes);
				USHORT* pPix = (USHORT*)(*spPixdata)(x,y);
				GraphicsUtilities::ReduceHi8888(*lpPtr_16,&a,&r,&g,&b);
				*pPix = GraphicsUtilities::ComposeHi4444(a,r,g,b);
			}
		}
		break;
	}

	pImgRect->bIsChanged = true;
	spPixdata->MarkAsChanged();
}
IMGID gbTextureManager::MakeGrayImage(IMGID ImageID)
{
	IMGID TextureID;
	TextureRect* pSrcImgRect = GetTextureRect(ImageID);
	NiSourceTexture* pSrcTex = NULL;
	if(NiIsKindOf(NiSourceTexture,pSrcImgRect->spTexture))
		pSrcTex = NiDynamicCast(NiSourceTexture,pSrcImgRect->spTexture);
	else
	{
		assert(!("创建黑白图时 传入的贴图类型不符合类型要求！"));
		return INVALID_IMGID;
	}
	NiPixelDataPtr spSrcPixData = pSrcTex->GetSourcePixelData();

	const NiPixelFormat* pkFmt = &(spSrcPixData->GetPixelFormat());
	unsigned int uiStride = spSrcPixData->GetPixelStride();

	TextureRect* ptexRect = new TextureRect();
	NiPixelDataPtr spPixData = NULL;

	if(uiStride == 0)
	{
		NiTextureCodec* pCodec = NiNew NiTextureCodec();
		spSrcPixData = pCodec->ExpandCompressedData(*spSrcPixData);
		spPixData = NiNew NiPixelData(*spSrcPixData);
		pkFmt = &(spSrcPixData->GetPixelFormat());
		uiStride = spSrcPixData->GetPixelStride();
		NiDelete pCodec;
	}
	else
		spPixData = NiNew NiPixelData(*spSrcPixData);
	NiSourceTexture* pTex = NiSourceTexture::Create(spPixData);
	ptexRect->spTexture = pTex;

	if(ptexRect->spTexture)
	{
		pTex->SetStatic(false);
		ptexRect->bIsChanged = false;
		ptexRect->iLeft = 0;
		ptexRect->iTop = 0;
		ptexRect->iWidth = ptexRect->spTexture->GetWidth();
		ptexRect->iHeight = ptexRect->spTexture->GetHeight();
		ptexRect->dwColor = 0xffffffff;
		TextureID = m_TexturePool.AddFirstEmpty(ptexRect) ;

		//update image
		unsigned int uiAv,uiAlpha,uiPixelValue;
		switch(uiStride) {
			case 1:
				for (int y = 0; y < ptexRect->iHeight; y++)
				{
					for (int x = 0; x < ptexRect->iWidth; x++)
					{
						BYTE* pSrcPix = (*spSrcPixData)(x, y);
						BYTE* pPix = (*spPixData)(x, y);
						uiAv = (*pSrcPix)& pkFmt->GetMask(NiPixelFormat::COMP_RED)>>pkFmt->GetShift(NiPixelFormat::COMP_RED);
						uiAv += ((*pSrcPix)& pkFmt->GetMask(NiPixelFormat::COMP_GREEN))>>pkFmt->GetShift(NiPixelFormat::COMP_GREEN);
						uiAv += (*pSrcPix)& pkFmt->GetMask(NiPixelFormat::COMP_BLUE)>>pkFmt->GetShift(NiPixelFormat::COMP_BLUE);
						uiAlpha = (*pSrcPix)& pkFmt->GetMask(NiPixelFormat::COMP_ALPHA);
						uiAv = (UINT)(uiAv/3);

						uiPixelValue = uiAv	<< pkFmt->GetShift(NiPixelFormat::COMP_RED);
						uiPixelValue |= uiAv<< pkFmt->GetShift(NiPixelFormat::COMP_GREEN);
						uiPixelValue |= uiAv<< pkFmt->GetShift(NiPixelFormat::COMP_BLUE);
						uiPixelValue |= uiAlpha;
						*pPix = uiPixelValue;
						pSrcPix++;
						pPix++;
					}
				}
				break;
			case 2:
				for (int y = 0; y < ptexRect->iHeight; y++)
				{
					for (int x = 0; x < ptexRect->iWidth; x++)
					{
						USHORT* pSrcPix = (USHORT*)(*spSrcPixData)(x, y);
						USHORT* pPix = (USHORT*)(*spPixData)(x, y);
						uiAv = (*pSrcPix)& pkFmt->GetMask(NiPixelFormat::COMP_RED)>>pkFmt->GetShift(NiPixelFormat::COMP_RED);
						uiAv += ((*pSrcPix)& pkFmt->GetMask(NiPixelFormat::COMP_GREEN))>>pkFmt->GetShift(NiPixelFormat::COMP_GREEN);
						uiAv += (*pSrcPix)& pkFmt->GetMask(NiPixelFormat::COMP_BLUE)>>pkFmt->GetShift(NiPixelFormat::COMP_BLUE);
						uiAlpha = (*pSrcPix)& pkFmt->GetMask(NiPixelFormat::COMP_ALPHA);
						uiAv = (UINT)(uiAv/3);

						uiPixelValue = uiAv	<< pkFmt->GetShift(NiPixelFormat::COMP_RED);
						uiPixelValue |= uiAv<< pkFmt->GetShift(NiPixelFormat::COMP_GREEN);
						uiPixelValue |= uiAv<< pkFmt->GetShift(NiPixelFormat::COMP_BLUE);
						uiPixelValue |= uiAlpha;
						*pPix = uiPixelValue;
						pSrcPix++;
						pPix++;
					}
				}
				break;
			case 4:
				for (int y = 0; y < ptexRect->iHeight; y++)
				{
					for (int x = 0; x < ptexRect->iWidth; x++)
					{
						UINT* pSrcPix = (UINT*)(*spSrcPixData)(x, y);
						UINT* pPix = (UINT*)(*spPixData)(x, y);
						uiAv = (*pSrcPix)& pkFmt->GetMask(NiPixelFormat::COMP_RED)>>pkFmt->GetShift(NiPixelFormat::COMP_RED);
						uiAv += ((*pSrcPix)& pkFmt->GetMask(NiPixelFormat::COMP_GREEN))>>pkFmt->GetShift(NiPixelFormat::COMP_GREEN);
						uiAv += (*pSrcPix)& pkFmt->GetMask(NiPixelFormat::COMP_BLUE)>>pkFmt->GetShift(NiPixelFormat::COMP_BLUE);
						uiAlpha = (*pSrcPix)& pkFmt->GetMask(NiPixelFormat::COMP_ALPHA);
						uiAv = (UINT)(uiAv/3);

						uiPixelValue = uiAv	<< pkFmt->GetShift(NiPixelFormat::COMP_RED);
						uiPixelValue |= uiAv<< pkFmt->GetShift(NiPixelFormat::COMP_GREEN);
						uiPixelValue |= uiAv<< pkFmt->GetShift(NiPixelFormat::COMP_BLUE);
						uiPixelValue |= uiAlpha;
						*pPix = uiPixelValue;
						pSrcPix++;
						pPix++;
					}
				}
				break;
		}

		NiTexture::RendererData * pRenderData = pTex->GetRendererData();
		NiDX9TextureData* pDx9Data = NiSmartPointerCast(NiDX9TextureData,pRenderData);
		spPixData->MarkAsChanged();
		pDx9Data->GetAsSourceTexture()->Update();
	}
	else
	{
		TextureID = INVALID_IMGID;
	}
	pTex->SetStatic(true);
	return TextureID;
}

DWORD gbTextureManager::GetPixelColor(IMGID ImageID, unsigned int uiX, unsigned int uiY)
{
	TextureRect* pSrcImgRect = NULL ;
	pSrcImgRect = GetTextureRect(ImageID); 
	if (pSrcImgRect == NULL)
	{
       	return 0 ;
	}
    if ((pSrcImgRect->iWidth < uiX )||(pSrcImgRect->iHeight < uiY))
    {
		return 0;
    } 

	NiSourceTexture* pSrcTex = NULL;
	if(NiIsKindOf(NiSourceTexture,pSrcImgRect->spTexture))
		pSrcTex = NiDynamicCast(NiSourceTexture,pSrcImgRect->spTexture);
	else
	{
		assert(!("得到贴图点alpha时 传入的贴图类型不符合类型要求！"));
		return 0.0f;
	}
	NiPixelDataPtr spSrcPixData = pSrcTex->GetSourcePixelData();
	const NiPixelFormat* pkFmt = &(spSrcPixData->GetPixelFormat());
	unsigned int uiStride = spSrcPixData->GetPixelStride() ; 

	NiPalette* pTexPalette = NULL ;
	NiPalette::PaletteEntry* pTexPaletteEntry = NULL;
	pTexPalette	=  spSrcPixData->GetPalette() ;
	if (pTexPalette)
	{
         pTexPaletteEntry = pTexPalette->GetEntries();
	}

	if(uiStride == 0)
	{
		//decompress time
		gbTextureCodecEx* pCodec = NiNew gbTextureCodecEx();
		DWORD Pix = pCodec->ExpandCompressedPixel(*spSrcPixData,uiX,uiY);
		return Pix ;
	}

	DWORD dwPixelColor = 0;
	switch(uiStride) 
	{
			case 1:
				{
					BYTE* pSrcPix = (*spSrcPixData)(uiX, uiY);
					if (pTexPaletteEntry)
					{
						dwPixelColor  = pTexPaletteEntry[*pSrcPix].m_ucRed   ; dwPixelColor<<=8;
						dwPixelColor |= pTexPaletteEntry[*pSrcPix].m_ucGreen ; dwPixelColor<<=8;
						dwPixelColor |= pTexPaletteEntry[*pSrcPix].m_ucBlue  ; dwPixelColor<<=8;
						dwPixelColor |= pTexPaletteEntry[*pSrcPix].m_ucAlpha ;
					}
				}
				break;
			case 2:
				{
					USHORT* pSrcPix = (USHORT*)(*spSrcPixData)(uiX, uiY);					
					unsigned int r = (*pSrcPix)>>11;
					unsigned int g = (*pSrcPix)&& 0xf81f >>5 ;
					unsigned int b = (*pSrcPix)&& 0xffe0 ;
					dwPixelColor  = r  ; dwPixelColor<<=8;
					dwPixelColor |= g  ; dwPixelColor<<=8;
					dwPixelColor |= b  ; dwPixelColor<<=8;
					dwPixelColor |= 0xff000000 ;
				}
				break;
			case 4:
				{
					UINT* pSrcPix  = (UINT*)(*spSrcPixData)(uiX, uiY);
					dwPixelColor   = (DWORD)(*pSrcPix)<<8;
					unsigned int a = (*pSrcPix)>>24 & 0xffffff00 ;
                    dwPixelColor |= a ;
				}
				break;
	}

	return dwPixelColor;
}

float gbTextureManager::GetPixelAlpha(IMGID ImageID,unsigned int uiX,unsigned int uiY)
{
	TextureRect* pSrcImgRect = GetTextureRect(ImageID);
	NiSourceTexture* pSrcTex = NULL;
	if(NiIsKindOf(NiSourceTexture,pSrcImgRect->spTexture))
		pSrcTex = NiDynamicCast(NiSourceTexture,pSrcImgRect->spTexture);
	else
	{
		assert(!("得到贴图点alpha时 传入的贴图类型不符合类型要求！"));
		return 0.0f;
	}
	NiPixelDataPtr spSrcPixData = pSrcTex->GetSourcePixelData();
	const NiPixelFormat* pkFmt = &(spSrcPixData->GetPixelFormat());
	unsigned int uiStride = spSrcPixData->GetPixelStride();
	if(uiStride == 0)
	{
		//decompress time
		gbTextureCodecEx* pCodec = NiNew gbTextureCodecEx();
		UINT Pix = pCodec->ExpandCompressedPixel(*spSrcPixData,uiX,uiY);
		NiPixelFormat PxFormat =  NiPixelFormat::RGBA32;
		UINT uiMax = ((0xffffffff)& PxFormat.GetMask(NiPixelFormat::COMP_ALPHA))>>PxFormat.GetShift(NiPixelFormat::COMP_ALPHA);
		UINT uiAlpha = ((Pix)& PxFormat.GetMask(NiPixelFormat::COMP_ALPHA))>>PxFormat.GetShift(NiPixelFormat::COMP_ALPHA);
		return (float(uiAlpha))/float(uiMax);
	}

	UINT uiMax ;
	uiMax= ((0xffffffff)& pkFmt->GetMask(NiPixelFormat::COMP_ALPHA))>>pkFmt->GetShift(NiPixelFormat::COMP_ALPHA);	 
	if(uiMax == 0)
		return 0.0f;

	UINT uiAlpha = 0;
	switch(uiStride) {
			case 1:
					{
						BYTE* pSrcPix = (*spSrcPixData)(uiX, uiY); 
						uiAlpha = ((*pSrcPix)& pkFmt->GetMask(NiPixelFormat::COMP_ALPHA))>>pkFmt->GetShift(NiPixelFormat::COMP_ALPHA);
					}
				break;
			case 2:
					{
						USHORT* pSrcPix = (USHORT*)(*spSrcPixData)(uiX, uiY);		
						uiAlpha = ((*pSrcPix)& pkFmt->GetMask(NiPixelFormat::COMP_ALPHA))>>pkFmt->GetShift(NiPixelFormat::COMP_ALPHA);
					}
				break;
			case 4:
					{
						UINT* pSrcPix = (UINT*)(*spSrcPixData)(uiX, uiY);
						uiAlpha = ((*pSrcPix)& pkFmt->GetMask(NiPixelFormat::COMP_ALPHA)) >> pkFmt->GetShift(NiPixelFormat::COMP_ALPHA);
					}
				break;
	}

	return (float(uiAlpha))/float(uiMax);
}

IMGID gbTextureManager::CreateWhite()
{
	unsigned int texID;
	CreateSourceTexture(WHITE_SIZE,WHITE_SIZE,0,texID);
	TextureRect* pImgRect = GetTextureRect(texID);
	NiSourceTexture* pTex = NiDynamicCast(NiSourceTexture,pImgRect->spTexture);
	pTex->SetStatic(false);
	NiPixelDataPtr spPixdata = pTex->GetSourcePixelData();
	for (int y = 0; y < WHITE_SIZE; y++)
	{
		for (int x = 0; x < WHITE_SIZE; x++)
		{
			USHORT* pPix = (USHORT*)(*spPixdata)(x, y);
			*pPix = 0xffff;
		}
	}
	NiTexture::RendererData * pRenderData = pImgRect->spTexture->GetRendererData();
	NiDX9TextureData* pDx9Data = NiSmartPointerCast(NiDX9TextureData,pRenderData);
	spPixdata->MarkAsChanged();
	pDx9Data->GetAsSourceTexture()->Update();

	return texID;
}

void gbTextureManager::OnLostDevice()
{
	for(unsigned int i = 0;i<m_TexturePool.GetSize();i++)
	{
		TextureRect* pRect = m_TexturePool.GetAt(i);
		if(pRect)
		{
			if(NiIsKindOf(NiRenderedTexture,pRect->spTexture))
			{
				pRect->spTexture = NULL;
				pRect->bIsChanged = true;
			}
		}
	}
	m_TexturePool.RemoveAll();
}

void gbTextureManager::OnResetDevice()
{
	for(unsigned int i = 0;i<m_TexturePool.GetSize();i++)
	{
		TextureRect* pRect = m_TexturePool.GetAt(i);
		if(pRect)
		{
			if(pRect->spTexture == NULL&&pRect->bIsChanged)
			{
				NiTexture::FormatPrefs kFormatPerfs; 
				kFormatPerfs.m_ePixelLayout = NiTexture::FormatPrefs::TRUE_COLOR_32;
				kFormatPerfs.m_eMipMapped = NiTexture::FormatPrefs::NO;
				kFormatPerfs.m_eAlphaFmt = NiTexture::FormatPrefs::SMOOTH;
				pRect->spTexture = NiRenderedTexture::Create(pRect->iWidth,pRect->iHeight,NiRenderer::GetRenderer(),kFormatPerfs);
			}
		}
	}
	m_TexturePool.RemoveAll();
}

void gbTextureManager::ClearSourceTexture( unsigned int& TextureID , DWORD dwColor )
{
	TextureRect* pImgRect = GetTextureRect(TextureID);
	NiSourceTexture* pTex = NiDynamicCast(NiSourceTexture,pImgRect->spTexture);
	pTex->SetStatic(false);
	NiPixelDataPtr spPixdata = pTex->GetSourcePixelData();

	for (unsigned int y = 0; y < pTex->GetHeight(); y++)
	{
		for (unsigned int x = 0; x < pTex->GetWidth(); x++)
		{
			USHORT* pPix = (USHORT*)(*spPixdata)( x, y );
			*pPix = (USHORT)dwColor ;
		}
	}

	pImgRect->bIsChanged = true;
	spPixdata->MarkAsChanged();
}

bool  gbTextureManager::HaveAlphaChannels(IMGID ImageID) 
{
	TextureRect* pSrcImgRect = GetTextureRect(ImageID);
	NiSourceTexture* pSrcTex = NULL;
	if(NiIsKindOf(NiSourceTexture,pSrcImgRect->spTexture))
		pSrcTex = NiDynamicCast(NiSourceTexture,pSrcImgRect->spTexture);
	else
	{
		assert(!("得到贴图点alpha时 传入的贴图类型不符合类型要求！"));
		return 0.0f;
	}
	NiPixelDataPtr spSrcPixData = pSrcTex->GetSourcePixelData();
	const NiPixelFormat* pkFmt = &(spSrcPixData->GetPixelFormat());
    if ( pkFmt->GetMask(NiPixelFormat::COMP_ALPHA) != 0 )
    {
		return true ;
    } 
    return false ;

}