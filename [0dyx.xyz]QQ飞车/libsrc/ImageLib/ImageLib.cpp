/*
 *	ImageLib.cpp
 *	Image library functions.
 *
 *	Jim, 2003.12.12.
 */
#include "Engine.h"
#include "Display.h"
#include "ImageLib.h"
#include "GameImageLib.h"
#include "vfilewrap.h"


extern	IDisplay	*g_pDisplay;

//////////////////////////////////////////////////////////////////////////



IImageLib * CreateImageLib()
{
	return (IImageLib*)(new CGameImageLib());
}


void DestroyImageLib(IImageLib *pImageLib)
{
	if(pImageLib)
	{
		delete (CGameImageLib*)pImageLib;
	}
}


BOOL	writeNullImg( FILE *fp){
	int a=0;	
	fwrite(&a, 1, sizeof(int), fp);
	fwrite(&a, 1, sizeof(int), fp);
	fwrite(&a, 1, sizeof(int), fp);
	fwrite(&a, 1, sizeof(int), fp);
//	fwrite(pImage->data, 1, sizeof(unsigned char) * pImage->h * pImage->pitch, fp);
	
	return TRUE;
}
//////////////////////////////////////////////////////////////////////////
// disk i/o functions

BOOL	NakedWriteImage(TImage *pImage, FILE *fp){
	if( !fp)		return FALSE;
	if(	!pImage )	return writeNullImg( fp);
		
	fwrite(&pImage->w, 1, sizeof(int), fp);
	fwrite(&pImage->h, 1, sizeof(int), fp);
	fwrite(&pImage->pitch, 1, sizeof(int), fp);
	fwrite(&pImage->attrib, 1, sizeof(int), fp);
	fwrite(pImage->data, 1, sizeof(unsigned char) * pImage->h * pImage->pitch, fp);
	
	return TRUE;
}

BOOL	NakedReadImage(TImage **pImage, FILE *fp)
{
	if(!fp || !pImage)
		return FALSE;
	
	int w, h, pitch, attrib;
	fread(&w, 1, sizeof(int), fp);
	fread(&h, 1, sizeof(int), fp);
	fread(&pitch, 1, sizeof(int), fp);
	fread(&attrib, 1, sizeof(int), fp);

    if ( w > 2048 || h > 2048 || w < 0 || h < 0 )
    {
//        LogInfo("读取文件:%s 发生错误 w=%d  h=%d pitch=%d attrib=%d", 
//            fp->_tmpfname, w, h, pitch, attrib);
        *pImage = NULL;
        return FALSE;
    }
	
	TImage *tmp = CreateImage(w, h, attrib);
	if(NULL == tmp)
		return FALSE;
	
	unsigned char *pdata = tmp->data;
	for(int y=0; y<h; y++)
	{
		fread(pdata, 1, pitch, fp);
		pdata += pitch;
	}
	
	*pImage = tmp;
	
	return TRUE;
}

BOOL	NakedReadImage(TImage **pImage, FILE_V* fp)
{
	if(!fp || !pImage)
		return FALSE;
	
	int w, h, pitch, attrib;
	vfread(&w, 1, sizeof(int), fp);
	vfread(&h, 1, sizeof(int), fp);
	vfread(&pitch, 1, sizeof(int), fp);
	vfread(&attrib, 1, sizeof(int), fp);
    
    if ( w > 2048 || h > 2048 )
    {
        *pImage = NULL;
        return FALSE;
    }
	
	TImage *tmp = CreateImage(w, h, attrib);
	if(NULL == tmp)
		return TRUE;
	
	unsigned char *pdata = tmp->data;
	for(int y=0; y<h; y++)
	{
		vfread(pdata, 1, pitch, fp);
		pdata += pitch;
	}
	
	*pImage = tmp;
	
	return TRUE;
}



//////////////////////////////////////////////////////////////////////////
// TDualImage 

TDualImage::TDualImage()
{
	Init();
}


void TDualImage::Init()
{ 
	memset(this, 0, sizeof(TDualImage));
}


void TDualImage::FreeMemory()
{
	DestroyImage(&m_pImage);
    m_pImage = 0 ;
}


BOOL TDualImage::CreateImage(int w, int h, int attrib)
{
	m_pImage = ::CreateImage(w, h, attrib);
	if(NULL == m_pImage)
		return FALSE;
	return TRUE;
}


void TDualImage::GetBufferImage(int bw, int bh, BYTE *rgb_buffer, 
								int left, int top, 
								BYTE *alpha_buffer/* = NULL*/)
{
	enum { nRGBPixelSize = 3, nAlphaPixelSize = 1};

	if(!m_pImage || !rgb_buffer)
		return;

    SLONG over_xl,over_yl;
    SLONG des_sx, des_sy; // for *rgb_buffer & alpha_buffer
    SLONG src_sx, src_sy; // for *m_pImage
    des_sx = MAX(left, 0);
    des_sy = MAX(top, 0);
    over_xl = MIN(left + m_pImage->w, bw) - des_sx;
    over_yl = MIN(top + m_pImage->h, bh) - des_sy;
    if(over_yl <= 0 || over_xl <= 0)
        return;

    src_sx = MAX(-left, 0);
    src_sy = MAX(-top, 0);

    SLONG x,y;
	UCHR r, g, b, a;
	UCHR *pdes;
	USHORT *psrc;

	UINT* psrcex;


	pdes = rgb_buffer + (bw * des_sy + des_sx) * nRGBPixelSize;
	switch(m_pImage->attrib)
	{

	//uncompressed 1555
	case imageAttrib1555:
		if(alpha_buffer)
		{
			BYTE *pdes_alpha;
			pdes_alpha = alpha_buffer + (bw * des_sy + des_sx) * nAlphaPixelSize;
			for(y=0; y<over_yl; y++)
			{
				psrc = (USHORT*)(m_pImage->data + (src_sy+y) * m_pImage->pitch 
					+ src_sx * sizeof(USHORT));
				for(x=0; x<over_xl; x++)
				{
					r = *(pdes + x * nRGBPixelSize + 0);
					g = *(pdes + x * nRGBPixelSize + 1);
					b = *(pdes + x * nRGBPixelSize + 2);
					a = *(pdes_alpha + x * nAlphaPixelSize);
					*psrc++ = ComposeHi1555(a, r, g, b);
				}
				pdes += bw * nRGBPixelSize;
				pdes_alpha += bw * nAlphaPixelSize;
			}
		}
		else
		{
			for(y=0; y<over_yl; y++)
			{
				psrc = (USHORT*)(m_pImage->data + (src_sy+y) * m_pImage->pitch 
					+ src_sx * sizeof(USHORT));
				for(x=0; x<over_xl; x++)
				{
					r = *(pdes + x * nRGBPixelSize + 0);
					g = *(pdes + x * nRGBPixelSize + 1);
					b = *(pdes + x * nRGBPixelSize + 2);
					*psrc++ = ComposeHi1555(255, r, g, b);
				}
				pdes += bw * nRGBPixelSize;
			}
		}
		break;

	//uncompressed 4444
	case imageAttrib4444:
		if(alpha_buffer)
		{
			BYTE *pdes_alpha;
			pdes_alpha = alpha_buffer + (bw * des_sy + des_sx) * nAlphaPixelSize;
			for(y=0; y<over_yl; y++)
			{
				psrc = (USHORT*)(m_pImage->data + (src_sy+y) * m_pImage->pitch 
					+ src_sx * sizeof(USHORT));
				for(x=0; x<over_xl; x++)
				{
					r = *(pdes + x * nRGBPixelSize + 0);
					g = *(pdes + x * nRGBPixelSize + 1);
					b = *(pdes + x * nRGBPixelSize + 2);
					a = *(pdes_alpha + x * nAlphaPixelSize);
					*psrc++ = ComposeHi4444(a, r, g, b);
				}
				pdes += bw * nRGBPixelSize;
				pdes_alpha += bw * nAlphaPixelSize;
			}
		}
		else
		{
			for(y=0; y<over_yl; y++)
			{
				psrc = (USHORT*)(m_pImage->data + (src_sy+y) * m_pImage->pitch 
					+ src_sx * sizeof(USHORT));
				for(x=0; x<over_xl; x++)
				{
					r = *(pdes + x * nRGBPixelSize + 0);
					g = *(pdes + x * nRGBPixelSize + 1);
					b = *(pdes + x * nRGBPixelSize + 2);
					*psrc++ = ComposeHi4444(255, r, g, b);
				}
				pdes += bw * nRGBPixelSize;
			}
		}
		break;
		
	//add+lincoln
	//[2005:3:25] [11:06]
	//uncompressed 8888
	case imageAttrib8888:
		if(alpha_buffer)
		{

			BYTE *pdes_alpha;
			pdes_alpha = alpha_buffer + (bw * des_sy + des_sx) * nAlphaPixelSize;
			for(y=0; y<over_yl; y++)
			{
				psrcex = (UINT*)(m_pImage->data + (src_sy+y) * m_pImage->pitch 
					+ src_sx * sizeof(UINT));
				for(x=0; x<over_xl; x++)
				{
					r = *(pdes + x * nRGBPixelSize + 0);
					g = *(pdes + x * nRGBPixelSize + 1);
					b = *(pdes + x * nRGBPixelSize + 2);
					a = *(pdes_alpha + x * nAlphaPixelSize);
					*psrcex++ = ComposeHi8888(a, r, g, b);
				}
				pdes += bw * nRGBPixelSize;
				pdes_alpha += bw * nAlphaPixelSize;
			}
		}
		else
		{
			for(y=0; y<over_yl; y++)
			{
				psrcex = (UINT*)(m_pImage->data + (src_sy+y) * m_pImage->pitch 
					+ src_sx * sizeof(UINT));
				for(x=0; x<over_xl; x++)
				{
					r = *(pdes + x * nRGBPixelSize + 0);
					g = *(pdes + x * nRGBPixelSize + 1);
					b = *(pdes + x * nRGBPixelSize + 2);
					*psrcex++ = ComposeHi8888(255, r, g, b);
				}
				pdes += bw * nRGBPixelSize;
			}
		}
		break;

	}
	return;
}


void TDualImage::OperImageSize(int *sx, int *sy, int *ex, int *ey)
{
    int image_sx, image_sy, image_ex, image_ey;
    int x, y;

	if(!m_pImage) 
		return;
    
    if( (*ex) > m_pImage->w ) (*ex) = m_pImage->w;
    if( (*ey) > m_pImage->h ) (*ey) = m_pImage->h;
    if( (*sx) < 0 ) (*sx) = 0;
    if( (*sy) < 0 ) (*sy) = 0;
    image_sx = (*ex);
    image_sy = (*ey);
    image_ex = (*sx);
    image_ey = (*sy);
	switch(m_pImage->attrib)
	{
	case imageAttrib1555:
		for(y=(*sy);y<(*ey);y++)
		{
			USHORT *hi = (USHORT*)&m_pImage->data[y * m_pImage->pitch + (*sx) * 2];
			for(x=(*sx);x<(*ex);x++)
			{
				BYTE a, r, g, b;
				ReduceHi1555(*hi++, &a, &r, &g, &b);
				if(a)
				{
					if(x<image_sx) image_sx=x;
					if(x>image_ex) image_ex=x;
					if(y<image_sy) image_sy=y;
					if(y>image_ey) image_ey=y;
				}
			}
		}
		break;

	case imageAttrib4444:
		for(y=(*sy);y<(*ey);y++)
		{
			USHORT *hi = (USHORT*)&m_pImage->data[y * m_pImage->pitch + (*sx) * 2];
			for(x=(*sx);x<(*ex);x++)
			{
				BYTE a, r, g, b;
				ReduceHi4444(*hi++, &a, &r, &g, &b);
				if(a)
				{
					if(x<image_sx) image_sx=x;
					if(x>image_ex) image_ex=x;
					if(y<image_sy) image_sy=y;
					if(y>image_ey) image_ey=y;
				}
			}
		}
		break;
	}

    (*sx)=image_sx;
    (*sy)=image_sy;
    (*ex)=image_ex;
    (*ey)=image_ey;
}


TImage*	TDualImage::CopyImage(int sx, int sy, int xl, int yl)
{
    TImage *pDestImage = NULL;
	
    if(NULL == (pDestImage = (TImage*)::CreateImage(xl, yl, m_pImage->attrib)))
        return NULL;
	
	BYTE *s, *p;
	s = m_pImage->data + sy * m_pImage->pitch + sx * 2;
	p = pDestImage->data;
    for(int y = 0; y < yl; y ++)
    {
		memcpy(p, s, xl * 2);
		s += m_pImage->pitch;
		p += pDestImage->pitch;
    }
	
    return pDestImage;
}


TImage* TDualImage::CopyVisibleImage(int *sx, int *sy, int *ex, int *ey)
{
    int view_sx, view_sy;
    int view_ex, view_ey;
    TImage *tmp = NULL;

	if(! m_pImage)
		return NULL;

    view_sx = max(*sx, 0);
    view_sy = max(*sy, 0);
    view_ex = min(*ex, m_pImage->w);
    view_ey = min(*ey, m_pImage->h);
    OperImageSize(&view_sx, &view_sy, &view_ex, &view_ey);
	
    if(view_sy > view_ey || view_sx > view_ex)
    {
		return NULL;
    }
    else
    {
        tmp = CopyImage(view_sx, view_sy, view_ex - view_sx + 1, view_ey - view_sy + 1);
        (*sx) = view_sx;
        (*sy) = view_sy;
    }
    return tmp;
}



void TDualImage::FreeHandle()
{
	if(m_hImage)
	{
		g_pDisplay->ReleaseImageHandle(m_hImage);
		m_hImage = 0;
	}
}


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
