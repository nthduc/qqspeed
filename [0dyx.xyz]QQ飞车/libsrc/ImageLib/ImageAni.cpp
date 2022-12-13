/*
 *	ImageAni.cpp
 *	image animation functions.
 *
 *	Jim, 2003.12.16.
 *
 *	˵��: Ӱ�񶯻����ܺ���
 *
 */
#include "Engine.h"
#include "Display.h"
#include "ImageLib.h"
#include "GameImageLib.h"
#include "vfilewrap.h"

//add+lincoln
//[2004:11:8] [12:44]
#include "xImage.h"


#define ANI_COPYRIHGT		MAKEFOURCC('Q','Q','F',0x1a)
#define ANI_FILE_ID			MAKEFOURCC('I','A','N','I')
#define ANI_FILE_VER		0x00010000


struct TAniFileHeader
{
	ULONG   copyright;      //copyright 'QQF'+0x1a
	ULONG   id;             //file id 'IARR'
	ULONG   version;        //file version
	ULONG   head_size;      //size of the file head
	//
	int		ani_width;		// �������Ⱥ͸߶�
	int		ani_height;		//
	int		plane_width;	// ������ʽӰ���Ⱥ͸߶�
	int		plane_height;	//
	int		frames;			// Ӱ��֡��
};


static	void	InitAniFileHeader(TAniFileHeader *pHeader);

//add+lincoln
//[2004:11:8] [12:05]
static TImageAni * LoadGsaAni(CHAR *szFileName);
static TImageAni * LoadGifAni(CHAR *szFileName,int nSplitSize=0);

//add+lincoln
//[2004:11:8] [15:52]
static int	ComputeBestAniSize(int w, int h);
static double	ComputeWasteArea(int w, int h, int square_size);

//////////////////////////////////////////////////////////////////////////
// main functions

TImageAni * CreateImageAni(int ani_width, int ani_height, 
						   int plane_width, int plane_height,
						   int frames)
{
	TImageAni *tmp = NULL;
	int cx, cy, size, blocks;
	
	cx = (plane_width + ani_width - 1) / ani_width;
	cy = (plane_height + ani_height -1) / ani_height;
	blocks = frames * cx * cy;
	size = sizeof(TImageAni) + (blocks - 1) * sizeof(TImage *);
	tmp = (TImageAni*)malloc(size);
	if(NULL == tmp)
		return NULL;
	
	memset(tmp, 0, size);
	tmp->ani_width = ani_width;
	tmp->ani_height = ani_height;
	tmp->plane_width = plane_width;
	tmp->plane_height = plane_height;
	tmp->width_blocks = cx;
	tmp->height_blocks = cy;
	tmp->frames = frames;

	return tmp;
}


void DestroyImageAni(TImageAni **pImageAni)
{
	if(!pImageAni)
		return;
	
	TImageAni *p = *pImageAni;
	if(p)
	{
		int blocks;
		int index;

		blocks = p->frames * p->width_blocks * p->height_blocks;
		for(index = 0; index < blocks; index ++)
		{
			if(p->image[index])
			{
				free(p->image[index]);
				p->image[index] = 0;
			}
		}

		free(p);
	}

	*pImageAni = NULL;
}


TImageAni * LoadImageAni(CHAR *szFileName)
{

	size_t len=strlen(szFileName);	
	
	if(len<5) return NULL;
	
	//gsa
	if(!strcmp(szFileName+len-4,".gsa"))
	{

		return LoadGsaAni(szFileName);
	}
	//gif
	else if(!strcmp(szFileName+len-4,".gif"))
	{
		return LoadGifAni(szFileName);
	}
	
	return NULL;

}


BOOL SaveImageAni(CHAR *szFileName, TImageAni *pImageAni)
{
	if(!szFileName || !pImageAni)
		return FALSE;

	// ���ļ�׼��д��
	FILE *fp = NULL;
	if(NULL == (fp = fopen(szFileName, "wb")))
	{
		//error open file
		goto _error;
	}

	// д���ļ�ͷ
	TAniFileHeader fh;
	InitAniFileHeader(&fh);
	fh.ani_width = pImageAni->ani_width;
	fh.ani_height = pImageAni->ani_height;
	fh.plane_width = pImageAni->plane_width;
	fh.plane_height = pImageAni->plane_height;
	fh.frames = pImageAni->frames;
	fwrite(&fh, 1, sizeof(fh), fp);

	// д�붯��Ӱ������
	int frame, x, y, block_index;
	TImageAni *p;
	p = pImageAni;
	block_index = 0;
	for(frame=0; frame<p->frames; frame++)
	{
		for(y=0; y<p->height_blocks; y++)
		{
			for(x=0; x<p->width_blocks; x++)
			{
				if(!p->image[block_index])
				{
					//error no image
					goto _error;
				}
				NakedWriteImage(p->image[block_index], fp);
				block_index ++;
			}
		}
	}
	
	// д��ɹ�, �ر��ļ�
	if(fp) { fclose(fp); fp = NULL; }

    return TRUE;

	// ������
_error:
	if(fp) { fclose(fp); fp = NULL; }
	return FALSE;
}


BOOL IsImageAniFile(CHAR *szFileName)
{
	TAniFileHeader fh;
	FILE_V* fp = NULL;
	size_t read_len;

	if(NULL == (fp = vfopen(szFileName, "rb")) )
	{
		return FALSE;
	}

	memset(&fh, 0, sizeof(fh));
	read_len = vfread(&fh, 1, sizeof(fh), fp);
	if(read_len != sizeof(fh))
	{
		vfclose(fp);
		return FALSE;
	}
	if(fh.copyright != ANI_COPYRIHGT || fh.id != ANI_FILE_ID)
	{
		vfclose(fp);
		return FALSE;
	}
	vfclose(fp);

	return TRUE;
}


//////////////////////////////////////////////////////////////////////////
// static functions

static	void	InitAniFileHeader(TAniFileHeader *pHeader)
{
	if(!pHeader)
		return;

	memset(pHeader, 0, sizeof(TAniFileHeader));
	pHeader->copyright = ANI_COPYRIHGT;
	pHeader->id = ANI_FILE_ID;
	pHeader->version = ANI_FILE_VER;
	pHeader->head_size = sizeof(TAniFileHeader);
}



//add+lincoln
//[2004:11:8] [12:00]
//LoadImageAni	--	load from all files(gsa,gif,tga,etc)
//LoadGsaAni	--	load from gsa 
//LoadGifAni	--	load from gif 

static TImageAni * LoadGsaAni(CHAR *szFileName)
{
	FILE_V* fp = NULL;
	TImageAni *tmp = NULL;

	// ���ļ�׼����ȡ
	if(NULL == (fp = vfopen(szFileName, "rb")))
	{
		goto _error;
	}

	// ��ȡ�ļ�ͷ
	TAniFileHeader fh;
	size_t read_len;
	read_len = vfread(&fh, 1, sizeof(fh), fp);
	if(read_len != sizeof(fh))
	{
		//head error
		goto _error;
	}
	if(fh.copyright != ANI_COPYRIHGT)
	{
		//copyright error
		goto _error;
	}
	if(fh.id != ANI_FILE_ID)
	{
		//file format error
		goto _error;
	}

	// �����ļ�ͷ�а�����Ӱ����Ϣ����Ӱ������
	tmp = CreateImageAni(fh.ani_width, fh.ani_height, fh.plane_width, fh.plane_height, fh.frames);
	if(NULL == tmp)
	{
		//create image array error
		goto _error;
	}

	// ��ȡӰ������
	TImage *img;
	int frame, x, y, block_index;
	block_index = 0;
	for(frame=0; frame<tmp->frames; frame++)
	{
		for(y=0; y<tmp->height_blocks; y++)
		{
			for(x=0; x<tmp->width_blocks; x++)
			{
				if( !NakedReadImage(&img, fp))
				{
					//read image error
					goto _error;
				}
				tmp->image[block_index++] = img;
				img = NULL;
			}
		}
	}

	// ��ȡ�ɹ�, �ر��ļ�
	if(fp) { vfclose(fp); fp = NULL;	}

	return tmp;

	// ��������
_error:
	if(fp) { vfclose(fp); fp = NULL; }
	DestroyImageAni(&tmp);

	return NULL;
}



//add+lincoln
//[2005:1:28] [11:39]
//nSplitSize: (����:not split),(0:default split),(����:split size),
static TImageAni * LoadGifAni(CHAR *szFileName,int nSplitSize/*=0*/)
{
  
    
    SLONG   total_frames, curr_frame;
    SLONG image_width, image_height;
    USTR *rgb_buffer = NULL, *alpha_buffer = NULL;
	TImage *pImage = NULL;
	TImageAni *	pImageAni=NULL;

	
	IDXANI* pGifAim=load_idxani_from_gif(szFileName);

	if(!pGifAim)
	{
		goto _error_exit;
	}
	
    image_width=pGifAim->width;
	image_height=pGifAim->height;
	total_frames=pGifAim->image_frames;

    // ��һת���ļ�
    curr_frame = 0;
    for(curr_frame=0; curr_frame<total_frames; curr_frame++)
    {

        
		if(0 != get_buffer_from_idxanim(pGifAim, curr_frame, &image_width, &image_height, &rgb_buffer, &alpha_buffer))
        {
            goto _error_exit;
        }
		{
			for( int y=0; y<image_height; y++ )
			{
				for( int x=0; x<image_width; x++ )
				{
					int i = (y*image_width + x)*3;
					rgb_buffer[i] = 255 - rgb_buffer[i];
					rgb_buffer[i+1] = 255 - rgb_buffer[i+1];
					rgb_buffer[i+2] = 255 - rgb_buffer[i+2];
				}
			}
		}

		// �ڶ����һ֡��ʱ��, ����Ӱ�񶯻�
		if(0 == curr_frame)
		{
            
			// ����ͼ�񶯻�
			SLONG ani_width, ani_height, ani_size;

			//Ĭ���зִ�С
			if(nSplitSize==0)
			{
				ani_size = ComputeBestAniSize(image_width, image_height);

				if(ani_size >= image_width && ani_size >= image_height )
				{
					ani_width = image_width;
					ani_height = image_height;
				}
				else
				{
					ani_width = ani_size;
					ani_height = ani_size;
				}
			}
			//���з�
			else if(nSplitSize<0)
			{
				ani_width = image_width;
				ani_height = image_height;
			 
			}
			//ָ���зִ�С
			else 
			{

				ani_size = nSplitSize;

				if(ani_size >= image_width && ani_size >= image_height )
				{
					ani_width = image_width;
					ani_height = image_height;
				}
				else
				{
					ani_width = ani_size;
					ani_height = ani_size;
				}

			}

			pImageAni = CreateImageAni(ani_width, ani_height, image_width, image_height, total_frames);
			if(NULL == pImageAni)
			{
				goto _error_exit;
			}

		}
	

		// ת����֡ͼ��
		TDualImage dimage;
		int x, y;
		int block_index;
		block_index = pImageAni->width_blocks * pImageAni->height_blocks * curr_frame;
		for(y=0; y<pImageAni->height_blocks; y++)
		{
			for(x=0; x<pImageAni->width_blocks; x++)
			{
				dimage.CreateImage(pImageAni->ani_width, pImageAni->ani_height, imageAttrib4444);
				
				dimage.GetBufferImage(image_width, image_height, rgb_buffer, x * pImageAni->ani_width,
					y * pImageAni->ani_height, alpha_buffer);
				pImageAni->image[block_index++] = DuplicateImage(dimage.m_pImage);
				dimage.FreeMemory();
			}
		}
        if(rgb_buffer) { free(rgb_buffer); rgb_buffer = NULL; }
        if(alpha_buffer) { free(alpha_buffer); alpha_buffer = NULL; }
    }

	if(rgb_buffer) { free(rgb_buffer); rgb_buffer = NULL; }
    if(alpha_buffer) { free(alpha_buffer); alpha_buffer = NULL; }
	return pImageAni;


_error_exit:
	
	DestroyImageAni(&pImageAni);
    if(rgb_buffer) { free(rgb_buffer); rgb_buffer = NULL; }
    if(alpha_buffer) { free(alpha_buffer); alpha_buffer = NULL; }
    
	return NULL;

}


//////////////��������////////////////////////////////////////////////////////////

//x,yΪtgaͼ��Ŀ�ȣ��߶�
//square_sizeΪÿ��С��Ĵ�С
//����һ��ֵ��ȫ��������ЩС��ʱ���˷��������
static double	ComputeWasteArea(int w, int h, int square_size)
{
	int x_blocks, y_blocks;
	double waste_area;
	x_blocks = (w + square_size - 1) / square_size;
	y_blocks = (h + square_size - 1) / square_size;
	waste_area = x_blocks * y_blocks * square_size * square_size - w * h;
	return waste_area;
}

//����ͼ���С������һ���ߴ�Ŀ����ָ�
//�ڸ������и��ߴ���ѡ����С�˷�����ĳߴ�
static int	ComputeBestAniSize(int w, int h)
{
	static int sz[] = {32, 64, 128, 256, 512};
	int i, best;
	double min_waste, waste;

	min_waste = w * h * 100;
	best = 0;
	for(i = 0; i < sizeof(sz)/sizeof(sz[0]); i++)
	{
		waste = ComputeWasteArea(w, h, sz[i]);
        if(waste <= min_waste)
		{
			min_waste = waste;
			best = i;
		}
	}
	return sz[best];
}

