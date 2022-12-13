/*
 *	xGif.cpp
 *	gif image format functions.
 *	
 *	Jim(LiuZiJian), 2003.6.26.
 *	    Created for library "xImage".
 *	
 */
#include "Engine.h"
#include "gif89a.h"
#include "xGif.h"



#define MAX_CACHE_FILES		32

// Jim, added [10/27/2003]
// 装载 GIF 文件的 CACHE.
//{{{
struct  GIFCACHE
{
	CHAR	szFileName[_MAX_FNAME];	// 文件名称
	DWORD	dwFileID;				// 文件ID
	DWORD	dwRef;					// 引用次数
	IDXANI	*pAniData;				// 储存读入的文件数据

	// --------------------
	GIFCACHE()
	{
		Init();
	}
	void Init()
	{
		memset(this, 0, sizeof(GIFCACHE));
	}
};
//}}}


GIFCACHE	l_GifCache[MAX_CACHE_FILES];


static void display_frame_image(SLONG frame, BYTE *buffer, BYTE *image_plane, LPCGLOBAL_INFO gi, LPCFRAME fm);
static IDXANI* directly_load_idxani_from_gif(char *filename);
static void directly_destroy_idxani(IDXANI **ani);
static DWORD filename_to_id(char *name);

/////////////////////////////////////////////////////////////////////////////
// Load GIF 

int	init_gif_loader()
{
	int i;
	for(i=0; i<MAX_CACHE_FILES; i++)
	{
		l_GifCache[i].Init();
	}
	return 0;
}

void free_gif_loader()
{
	int i;
	for(i=0; i<MAX_CACHE_FILES; i++)
	{
		if(l_GifCache[i].dwRef > 0)
		{
			if(l_GifCache[i].pAniData)
			{
				directly_destroy_idxani(&l_GifCache[i].pAniData);
			}
		}
		l_GifCache[i].Init();
	}
}



IDXANI* load_idxani_from_gif(char *filename, BOOL bForceFromFile /*= TRUE*/)
{
	if(bForceFromFile)
	{
		return directly_load_idxani_from_gif(filename);
	}
	else
	{
		//
		// 寻找CACHE内已有的数据,查找是否有这一文件
		//
		int i, exist_index = -1, empty_index = -1;
		DWORD id = filename_to_id(filename);
		for(i=0; i<MAX_CACHE_FILES; i++)
		{
			if(l_GifCache[i].dwRef == 0)
			{
				empty_index = i;
				continue;
			}
			if(l_GifCache[i].dwFileID != id)
				continue;
			if(0 == strcmp((const char *)l_GifCache[i].szFileName, (const char*)filename))
			{
				exist_index = i;
			}
		}

		//
		// 如果存在已有文件, 则对数据进行引用
		//
		if(exist_index >= 0)
		{
			l_GifCache[exist_index].dwRef ++;
			return l_GifCache[exist_index].pAniData;
		}

		//
		// 不存在已有文件, 需要载入
		//
		IDXANI *pAni = directly_load_idxani_from_gif(filename);

		if(empty_index >= 0 && pAni)
		{
			l_GifCache[empty_index].dwFileID = filename_to_id(filename);
			l_GifCache[empty_index].dwRef = 1;
			l_GifCache[empty_index].pAniData = pAni;
			strcpy(l_GifCache[empty_index].szFileName, (const char*)filename);
		}

		return pAni;
	}
}

void destroy_idxani(IDXANI **ani)
{
	int i, exist_index = -1;
	for(i=0; i<MAX_CACHE_FILES; i++)
	{
		if(l_GifCache[i].dwRef == 0)
			continue;

		if(l_GifCache[i].pAniData == *ani)
		{
			exist_index = i;
			break;
		}
	}

	if(exist_index >= 0)
	{
		l_GifCache[i].dwRef--;
		if(l_GifCache[i].dwRef == 0)
		{
			directly_destroy_idxani(&l_GifCache[i].pAniData);
			l_GifCache[i].Init();
		}
	}
	else
	{
		directly_destroy_idxani(ani);
	}
}

//////////////////////////////////////////////////////////////////////////
// Static functions
static IDXANI* directly_load_idxani_from_gif(char *filename)
{
    LPCGLOBAL_INFO gi;
    CGif89a gif;
    LPCFRAME fm;
    IDXANI *tmp = NULL;
    UINT    i;
    BYTE *image_plane = NULL;
    SLONG plane_width, plane_height, plane_size;
    BYTE *image_overlapped = NULL;
    
    // 打开并读取 GIF 文件
    //Jim, fixed bug here. [7/18/2003]
    //{{{
    //Jim, new version
    if(TRUE != gif.open(filename, TRUE))
    {
        goto _error_exit;
    }
    //Jim, old version
    //gif.open(filename, TRUE);
    //}}}

    if(!gif)
    {
        goto _error_exit;
    }

    // 创建临时 IDXANI 
    if(NULL == (tmp = (IDXANI*)malloc(sizeof(IDXANI))))
    {
        goto _error_exit;
    }
    memset(tmp, 0, sizeof(IDXANI));

    // 设定全局数据，包含动画图像宽度，高度，帧数，背景色。
    gi = gif.getGlobalInfo();
    tmp->width = gi->scrWidth;
    tmp->height = gi->scrHeight;
    tmp->image_frames = gi->frames;
    tmp->background_color = gi->BKColorIdx;

    // Jim, added for make each frame has full image. [8/4/2003]
    //{{{
    plane_width = tmp->width;
    plane_height = tmp->height;
    plane_size = sizeof(BYTE) * tmp->width * tmp->height;
    if(NULL == (image_plane = (BYTE*)malloc(plane_size)))
    {
        goto _error_exit;
    }
    memset(image_plane, gi->BKColorIdx, plane_size);
    //}}}
    
    
    if(gi->gFlag) //有全局调色板
    {	
        if(NULL == (tmp->global_palette = (BYTE *)malloc(sizeof(BYTE) * 769)))
        {
            goto _error_exit;
        }
        memset(tmp->global_palette, 0, sizeof(BYTE) * 769);
        tmp->global_palette[0] = 3; //色盘类型，3表示RGB
        memcpy(&tmp->global_palette[1], gi->gColorTable, gi->gSize * 3);
    }

    // 申请内存，用于存放每一帧图像
    if(NULL == (tmp->frame_image = (IDXIMG **)malloc(sizeof(IDXIMG*) * tmp->image_frames)))
    {
        goto _error_exit;
    }
    memset(tmp->frame_image, 0, sizeof(IDXIMG*) * tmp->image_frames);

    // 顺序读入每一帧图像，同时记录最小延时间隔等用来计算表现帧数
    // 计算表现帧数方法：
    //     表现帧数 = 没有延时的图像帧数 + 有延时的所有图像帧的延时时间之和 / 图像帧之间的最小延时
    // 播放时，原本没有延时的图像帧和最小延时的图像帧将是等同的。
    //
    WORD min_delay_time;    // 图像帧之间的最小延时，以10ms为单位
    ULONG total_delay_time; // 有延时的所有图像帧的延时时间之和
    UINT no_delay_frames;   // 没有延时的图像帧数

    total_delay_time = 0;
    min_delay_time = 65535;
    no_delay_frames = 0;
    for(i=0; i<gi->frames; i++)
    {
        if(NULL == (fm = gif.getNextFrame()))
        {
            goto _error_exit;
        }
        if(NULL == (tmp->frame_image[i] = (IDXIMG*)malloc(sizeof(IDXIMG))))
        {
            goto _error_exit;
        }
        memset(tmp->frame_image[i], 0, sizeof(IDXIMG));
        tmp->frame_image[i]->width = fm->imageWidth;
        tmp->frame_image[i]->height = fm->imageHeight;
        tmp->frame_image[i]->left = fm->imageLPos;
        tmp->frame_image[i]->top = fm->imageTPos;

        if(fm->ctrlExt.active)
        {
            // 获得动画影像帧的最小播放延时时间
            if(fm->ctrlExt.delayTime == 0)
            {
                // 本帧没有延时
                no_delay_frames ++;
            }
            else
            {
                // 累计总延时时间
                total_delay_time += fm->ctrlExt.delayTime;
                if(fm->ctrlExt.delayTime < min_delay_time)
                    min_delay_time = fm->ctrlExt.delayTime;
            }

            // 如果有透明色，则设定影像的透明色
            if(fm->ctrlExt.trsFlag)
            {
                tmp->frame_image[i]->is_trans = 1;
                tmp->frame_image[i]->trans_color = fm->ctrlExt.trsColorIndex;
            }

        }
        else
        {
            // 本帧没有延时
            no_delay_frames ++;
        }

        // 设定影像帧的调色板
        if(fm->lFlag)
        {
            // 有局部调色板，处理局部调色板
            if(NULL == (tmp->frame_image[i]->palette = (BYTE *)malloc(sizeof(BYTE)*769)))
            {
                goto _error_exit;
            }
            memset(tmp->frame_image[i]->palette, 0, sizeof(BYTE) * 769);
            tmp->frame_image[i]->palette[0] = 3; //色盘类型，3表示RGB
            memcpy(&tmp->frame_image[i]->palette[1], fm->pColorTable, fm->lSize * 3);
        }
        else
        {
            // 没有局部调色板，则将本帧的调色板指向全局调色板
            tmp->frame_image[i]->palette = tmp->global_palette;
        }

        // 设定影像的图像数据资料
        //if(fm->interlaceFlag) // 数据交错
        //{
        //}
        //else
        {
            // Jim, changed. [8/4/2003]
            //{{{
            /*
            ULONG image_size;
            image_size = sizeof(BYTE) * fm->imageWidth * fm->imageHeight;
            if(NULL == (tmp->frame_image[i]->data = (BYTE*)malloc(image_size)))
            {
                goto _error_exit;
            }
            memcpy(tmp->frame_image[i]->data, fm->dataBuf, image_size);
            */

            if(NULL == (tmp->frame_image[i]->data = (BYTE*)malloc(plane_size)))
            {
                goto _error_exit;
            }
            display_frame_image(i, tmp->frame_image[i]->data, image_plane, gi, fm);
            tmp->frame_image[i]->is_trans = TRUE;
            tmp->frame_image[i]->trans_color = fm->ctrlExt.trsColorIndex;
            tmp->frame_image[i]->width = (WORD)plane_width;
            tmp->frame_image[i]->height = (WORD)plane_height;
            tmp->frame_image[i]->left = 0;
            tmp->frame_image[i]->top = 0;
            //}}}

        }
    }

    // 计算并设定动画IDXANI的表现帧数
    tmp->present_frames = no_delay_frames;
    if(total_delay_time > 0)
    {
        tmp->present_frames += total_delay_time / min_delay_time;
    }
    if(NULL == (tmp->frame_index = (UINT*)malloc(sizeof(UINT) * tmp->present_frames)))
    {
        goto _error_exit;
    }
    memset(tmp->frame_index, 0, sizeof(UINT) * tmp->present_frames);


    if(tmp->present_frames == tmp->image_frames)
    {
        // 表现帧数和实际帧数相同，顺序对应即可
        for(i=0; i<tmp->present_frames; i++)
        {
            tmp->frame_index[i] = i;
        }
    }
    else
    {
        // 再次轮询每帧图像，以设定动画IDXANI的表现帧
        UINT present_frame, frame_count;
        UINT k;

        present_frame = 0;
        for(i=0; i<gi->frames; i++)
        {
            if(NULL == (fm = gif.getNextFrame()))
            {
                goto _error_exit;
            }
            
            if(fm->ctrlExt.active)
            {
                if(fm->ctrlExt.delayTime == 0)
                {
                    // 本帧没有延时
                    tmp->frame_index[present_frame++] = i;
                }
                else
                {
                    // 将本帧延时折算为动画帧
                    frame_count = fm->ctrlExt.delayTime / min_delay_time;
                    for(k=0; k<frame_count; k++)
                    {
                        tmp->frame_index[present_frame++] = i;
                    }
                }
            }
            else
            {
                // 本帧没有延时
                tmp->frame_index[present_frame++] = i;
            }
        }
    }

    // 操作完毕，关闭GIF文件
    gif.close();

	// Jim, added [10/14/2003]
	//{{{
	if(image_plane)
	{
		free(image_plane);
		image_plane = NULL;
	}
	//}}}
	

    return  tmp;

_error_exit:
    gif.close();
    if(tmp) destroy_idxani(&tmp);
    if(image_plane) { free(image_plane); image_plane = NULL; }
    return NULL;
}


static void directly_destroy_idxani(IDXANI **ani)
{
    if(*ani)
    {
        IDXANI *p = *ani;

        // 释放每个实际影像帧的图像(**frame_image)
        if(p->frame_image)
        {
            // 依次释放每帧图像
            UINT i;
            for(i=0; i<p->image_frames; i++)
            {
                if(p->frame_image[i])
                {
                    // 释放该帧影像数据
                    if(p->frame_image[i]->data)
                    {
                        free(p->frame_image[i]->data);
                        p->frame_image[i]->data = NULL;
                    }

                    // 如果该帧存在局部色盘，释放色盘
                    if( (p->frame_image[i]->palette != NULL)
                        && (p->frame_image[i]->palette != p->global_palette) )
                    {
                        free(p->frame_image[i]->palette);
                        p->frame_image[i]->palette = NULL;
                    }

                    // 释放该帧其他数据
                    free(p->frame_image[i]);
                    p->frame_image[i] = NULL;
                }
            }

            // 释放动画帧图像指针列表
            free(p->frame_image);
            p->frame_image = NULL;
        }

        // 释放表现帧对应的实际影像帧索引表(*frame_index)所占用内存
        if(p->frame_index)
        {
            free(p->frame_index);
            p->frame_index = NULL;
        }

        // 释放动画全局色盘
        if(p->global_palette)
        {
            free(p->global_palette);
            p->global_palette = NULL;
        }

        // 释放
        free(p);

        //全部释放完毕，将动画指针设成空
        *ani = NULL;
    }
}


/////////////////////////////////////////////////////////////////////////////
// static functions
static void __specific_dest_redraw_frame_image(BYTE *dest, SLONG dest_width, LPCFRAME fm)
{
    SLONG x, y;
    BYTE *src = fm->dataBuf;

    if(fm->ctrlExt.trsFlag)
    {
        BYTE trs;
        trs = (BYTE)fm->ctrlExt.trsColorIndex;
        for(y = 0; y < fm->imageHeight; y ++)
        {
            for(x = 0; x < fm->imageWidth; x ++)
            {
                if( *src++ != trs )
                {
                    *(dest+x) = *(src-1);
                }
            }
            dest += dest_width;
        }
    }
    else
    {
        for(y = 0; y < fm->imageHeight; y ++)
        {
            memcpy(dest, src, fm->imageWidth);
            src += fm->imageWidth;
            dest += dest_width;
        }
    }
}

static void __specific_dest_clear_frame_image(BYTE *dest, SLONG dest_width, LPCFRAME fm, BYTE color)
{
    SLONG y;
    BYTE *src = fm->dataBuf;

    for(y = 0; y < fm->imageHeight; y ++)
    {
        memset(dest, color, fm->imageWidth);
        dest += dest_width;
    }
}

static void display_frame_image(SLONG frame, BYTE *buffer, BYTE *image_plane, LPCGLOBAL_INFO gi, LPCFRAME fm)
{
    SLONG plane_width, plane_height, plane_size;
    BYTE *dest, *src, *plane;

    plane_width = gi->scrWidth;
    plane_height = gi->scrHeight;
    plane_size = sizeof(BYTE) * plane_width * plane_height;
    plane = image_plane + plane_width * fm->imageTPos + fm->imageLPos;
    dest = buffer + plane_width * fm->imageTPos + fm->imageLPos;
    src = fm->dataBuf;

    // Jim, added. [8/4/2003]
    //{{{
    // Disposal Method - Indicates the way in which the graphic is to
    // be treated after being displayed.
    //
    // Values :    0 -   No disposal specified. The decoder is
    //                   not required to take any action.
    //             1 -   Do not dispose. The graphic is to be left
    //                   in place.
    //             2 -   Restore to background color. The area used by the
    //                   graphic must be restored to the background color.
    //             3 -   Restore to previous. The decoder is required to
    //                   restore the area overwritten by the graphic with
    //                   what was there prior to rendering the graphic.
    //          4-7 -    To be defined.
    //}}}
    if(fm->ctrlExt.active)
    {
        // 如果图像是第0帧，并且是透明模式，则将背景清除为透明色
        if(frame == 0 && fm->ctrlExt.trsFlag)
        {
            memset(image_plane, fm->ctrlExt.trsColorIndex, plane_size);
        }

        // 依据不同的处理模式，来决定显示图像的方式
        switch(fm->ctrlExt.disposalMethod)
        {
            //   No disposal specified. The decoder is
            //   not required to take any action.
        case 0:
            __specific_dest_redraw_frame_image(plane, plane_width, fm );
            memcpy(buffer, image_plane, plane_size);
            break;

            //
            //  Do not dispose. The graphic is to be left
            //  in place.
            //
        case 1: 
            __specific_dest_redraw_frame_image(plane, plane_width, fm );
            memcpy(buffer, image_plane, plane_size);
            break;

            //
            // Restore to background color. The area used by the
            // graphic must be restored to the background color.
            //
        case 2:
            __specific_dest_redraw_frame_image(plane, plane_width, fm );
            memcpy(buffer, image_plane, plane_size);
            __specific_dest_clear_frame_image(plane, plane_width, fm, gi->BKColorIdx);
            break;

            //
            //   Restore to previous. The decoder is required to
            //   restore the area overwritten by the graphic with
            //   what was there prior to rendering the graphic.
            //
        case 3:
            memcpy(buffer, image_plane, plane_size);
            __specific_dest_redraw_frame_image(dest, plane_width, fm );
            break;

        default:    // To be defined.
            memcpy(buffer, image_plane, plane_size);
            break;
        }
    }
    else
    {
        SLONG y;
        for(y = 0; y < fm->imageHeight; y ++)
        {
            memcpy(dest, src, fm->imageWidth);
            dest += plane_width;
            src += fm->imageWidth;
        }
        memcpy(buffer, image_plane, plane_size);
    }
}


// Jim, added [10/28/2003]
//{{{
//FUNCTION  :   convert a string filename to a 32bit ID.
//PARAMETER :   USTR *name          -> filename
//RETURN    :   ULONG               -> 32 bit ID of the filename.
static DWORD filename_to_id(char *name)
{
    DWORD	s;
    int	i;

    for (s=i=0; name[i]; i++)
       s = (s+(i+1)*name[i]) % 0x8000000bu * 0xffffffefu;
    return s^0x12345678;
}
//}}}



//add+lincoln
//[2004:11:8] [10:01]
//将中间格式的IDXANI数据的第iframe帧转换为类似tga文件的buffer
//返回值：0-ok,其他-error
EXPORT  SLONG   FNBACK  get_buffer_from_idxanim(IDXANI* pIdxAim, SLONG iframe, SLONG *w, SLONG *h,
												USTR **rgb_buffer, USTR **alpha_buffer)
{
    


	IDXIMG* pIdxImg=NULL;
	BYTE* pPalleteBuf=NULL;

    USTR    *image = NULL, *alpha = NULL;
    SLONG   image_width, image_height, image_size, alpha_size;
	
	UCHR    color_idx,trans_idx;

	UCHR    r=0, g=0, b=0, a=0xff;
	SLONG   x,y,pix_offset;

    SLONG   result;
    
    result = 0;

	//没有传入有效的IDXANI数据指针
    if(NULL==pIdxAim)
    {
        result = -1;
        goto error;
    }

	//帧数不正确
	if(iframe>=(SLONG)pIdxAim->image_frames||iframe<0)
    {
        result = -1;
        goto error;
    }

	//取得该帧的图像数据指针
	pIdxImg=pIdxAim->frame_image[iframe];
	if(NULL==pIdxImg)
    {
        result = -1;
        goto error;
    }

	//取得调色板数据指针
	pPalleteBuf=pIdxImg->palette+1;
	if(NULL==pPalleteBuf)
    {
        result = -1;
        goto error;
    }

	//取得透明色索引
	if(pIdxImg->is_trans)
	{
		trans_idx=pIdxImg->trans_color;
	}
	else 
	{
		trans_idx=-1;	 
	}


    // 取得影像大小
    image_width = pIdxImg->width;
    image_height = pIdxImg->height;

	//计算生产的buffer数据的大小
    image_size = image_width * image_height * 3;
    alpha_size = image_width * image_height;

    // 为影像的图像数据分配内存
    if(NULL == (image = (USTR *)malloc(image_size)) )
    {
        result = -1;
        goto error;
    }
    memset(image, 0, image_size);

	// 为影像的Alpha通道数据分配内存
    if(NULL == (alpha = (USTR *)malloc(alpha_size)) )
    {
        result = -1;
        goto error;
    }
    memset(alpha, 0xff, alpha_size);

	//逐点转换和拷贝数据
	for(y = image_height-1; y >= 0; y--)
    {
        for(x=0; x<image_width; x++)
        {
			
            pix_offset = y * image_width + x;

			color_idx=pIdxImg->data[pix_offset];

			r=pPalleteBuf[color_idx*3+0];
			g=pPalleteBuf[color_idx*3+1];
			b=pPalleteBuf[color_idx*3+2];
			a=(color_idx==trans_idx)?0x00:0xff;		

            image[3 * pix_offset + 0] = r;
            image[3 * pix_offset + 1] = g;
            image[3 * pix_offset + 2] = b;
            alpha[pix_offset] = a;
        }
    }


    *w = image_width;
    *h = image_height;
    *rgb_buffer = image;
    *alpha_buffer = alpha;
    return  0;

error:

    if(image) { free(image); image = NULL; }
    if(alpha) { free(alpha); alpha = NULL; }
    return  result;
}

