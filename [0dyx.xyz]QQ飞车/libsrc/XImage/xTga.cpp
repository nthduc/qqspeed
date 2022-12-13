/*
**  VTGA.CPP
**  TGA file functions.
**
**  Dixon(LiJun),  2000.6.12.  
**      TGA file format & decode function.
**  ZJian(LiuZiJian),  2000.10.19.
**      Optimized.
**  Jim(LiuZiJian), 2003.6.25.
**      Modified for load image to rgb_buffer.
**
**
*/
#include "Engine.h"
#include "xTga.h"


//
// DEFINES & STRUCTS /////////////////////////////////////////////////////////////////////////////////////////////////
//
#define TGA_UNCOMPRESS        2
#define TGA_RLE_COMPRESS      10
#define TGA16_R(c)          ((UCHR)((((c)&0x7c00)>>7)&0xff))
#define TGA16_G(c)          ((UCHR)((((c)&0x03e0)>>2)&0xff))
#define TGA16_B(c)          ((UCHR)((((c)&0x001f)<<3)&0xff))


#pragma pack(push)
#pragma pack(1)
typedef struct tagTGA_HEAD
{
   UCHR  bIdSize;
   UCHR  bColorMapType;
   UCHR  bImageType;
   UHINT iColorMapStart;
   UHINT iColorMapLength;
   UCHR  bColorMapBits;

   UHINT ixStart;
   UHINT iyStart;
   UHINT iWidth;
   UHINT iHeight;

   UCHR  bBitsPerPixel;
   UCHR  bDescriptor;
} TGA_HEAD,*LPTGA_HEAD;;
#pragma pack(pop)


/********************************************************************************************************************/
/* MAIN FUNCTIONS                                                                                                   */
/********************************************************************************************************************/
EXPORT  SLONG   FNBACK  TGA_load_file(char *filename, SLONG *w, SLONG *h, 
                                      USTR **rgb_buffer, USTR **alpha_buffer)
{
    
    TGA_HEAD tga_head;
    FILEPT   *f=NULL;
    UCHR    r=0, g=0, b=0, a=0xff, t[6]={0};
    UHINT   cc;
    UCHR    count,flag;
    SLONG   i,offset,size, x, y;
    USTR    *image = NULL, *alpha = NULL;
    SLONG   image_width, image_height, image_size, alpha_size;
    SLONG   pix_offset, pix_offset2;
    SLONG   result;
    
    result = ERRNO_OK;

    //
    // 打开文件准备读取
    //
    if(NULL==(f = FILEOPEN((const char *)filename,"rb")) )
    {
        result = ERRNO_FILEOPEN;
        goto error;
    }

    //
    // 读取影像文件头
    //
    FILEREAD(&tga_head, 1, sizeof(TGA_HEAD), f);

    //
    // 取得影像大小
    //
    image_width = tga_head.iWidth;
    image_height = tga_head.iHeight;
    image_size = image_width * image_height * 3;
    alpha_size = image_width * image_height;

    //
    // 为影像的图像数据和Alpha通道数据分配内存
    //
    if(NULL == (image = (USTR *)malloc(image_size)) )
    {
        result = ERRNO_MEMORY;
        goto error;
    }
    memset(image, 0, image_size);

    if(NULL == (alpha = (USTR *)malloc(alpha_size)) )
    {
        result = ERRNO_MEMORY;
        goto error;
    }
    memset(alpha, 0xff, alpha_size);

    //
    // 跳过文件中不关心的数据，直接到影像数据
    //
    FILESEEK(f, tga_head.bIdSize, SEEK_CUR);
    FILESEEK(f, tga_head.iColorMapLength, SEEK_CUR);

    switch(tga_head.bImageType)
    {
    case TGA_UNCOMPRESS:
        {
            switch(tga_head.bBitsPerPixel)
            {
            case 32: //AAAAAAAARRRRRRRRGGGGGGGGBBBBBBBB
                {
                    for(y = image_height-1; y >= 0; y--)
                    {
                        for(x=0; x<image_width; x++)
                        {
                            FILEREAD(&b,1,1,f);
                            FILEREAD(&g,1,1,f);
                            FILEREAD(&r,1,1,f);
                            FILEREAD(&a,1,1,f);

                            pix_offset = y * image_width + x;
                            image[3 * pix_offset + 0] = r;
                            image[3 * pix_offset + 1] = g;
                            image[3 * pix_offset + 2] = b;
                            alpha[pix_offset] = a;
                        }
                    }
                    break;
                }
            case 24: //RRRRRRRRGGGGGGGGBBBBBBBB
                {
                    for(y=image_height-1; y>=0; y--)
                    {
                        for(x=0;x<tga_head.iWidth;x++)
                        {
                            FILEREAD(&b,1,1,f);
                            FILEREAD(&g,1,1,f);
                            FILEREAD(&r,1,1,f);

                            pix_offset = y * image_width + x;
                            image[3 * pix_offset + 0] = r;
                            image[3 * pix_offset + 1] = g;
                            image[3 * pix_offset + 2] = b;
                            alpha[pix_offset] = a;
                        }
                    }
                    break;
                }
            case 15:
            case 16://ARRRRRGG GGGBBBBB
                {
                    for(y=image_height-1; y>=0; y--)
                    {
                        for(x=0; x<image_width; x++)
                        {
                            FILEREAD(&t[0],1,1,f);
                            FILEREAD(&t[1],1,1,f);
                            cc = *(UHINT *)&t[0];
                            r = TGA16_R(cc);
                            g = TGA16_G(cc);
                            b = TGA16_B(cc);

                            pix_offset = y * image_width + x;
                            image[3 * pix_offset + 0] = r;
                            image[3 * pix_offset + 1] = g;
                            image[3 * pix_offset + 2] = b;
                            alpha[pix_offset] = a;
                        }
                    }
                    break;
                }
            default:
                {
                    //sprintf((char *)print_rec,"%s","tga image type not support");
                    //log_error(1,print_rec);
                    result = ERRNO_FORMATMISMATCH;
                    goto error;
                    //break;
                }
            }
        }
        break;

    case TGA_RLE_COMPRESS:
        {
            offset = 0;
            size = image_width * image_height;
            while(offset < size)
            {
                FILEREAD(&count,1,1,f);
                flag = (UCHR)(count & 0x80);
                if(0==flag)
                {
                    flag = (UCHR)(count+1);
                    for(i=0;i<flag;i++)
                    {
                        switch(tga_head.bBitsPerPixel)
                        {
                        case 32:
                            FILEREAD(&b,1,1,f);
                            FILEREAD(&g,1,1,f);
                            FILEREAD(&r,1,1,f);
                            FILEREAD(&a,1,1,f);
                            break;
                        case 24:
                            FILEREAD(&b,1,1,f);
                            FILEREAD(&g,1,1,f);
                            FILEREAD(&r,1,1,f);
                            a = 0xff;
                            break;
                        case 16:
                        case 15:
                            FILEREAD(&t[0],1,1,f);
                            FILEREAD(&t[1],1,1,f);
                            cc = *(UHINT *)&t[0];
                            r = TGA16_R(cc);
                            g = TGA16_G(cc);
                            b = TGA16_B(cc);
                            a = 0xff;
                            break;
                            
                        }

                        if(offset>=size)
                            continue;

                        x = offset % image_width;
                        y = image_height - 1 - offset/image_width;
                        pix_offset = y * image_width + x;
                        image[3 * pix_offset + 0] = r;
                        image[3 * pix_offset + 1] = g;
                        image[3 * pix_offset + 2] = b;
                        alpha[pix_offset] = a;

                        offset++;
                    }
                }
                else
                {
                    flag=(UCHR)(count-127);
                    switch(tga_head.bBitsPerPixel)
                    {
                    case 32:
                        FILEREAD(&b,1,1,f);
                        FILEREAD(&g,1,1,f);
                        FILEREAD(&r,1,1,f);
                        FILEREAD(&a,1,1,f);
                        break;
                    case 24:
                        FILEREAD(&b,1,1,f);
                        FILEREAD(&g,1,1,f);
                        FILEREAD(&r,1,1,f);
                        a = 0xff;
                        break;
                    case 16:
                    case 15:
                        FILEREAD(&t[0],1,1,f);
                        FILEREAD(&t[1],1,1,f);
                        cc = *(UHINT *)&t[0];
                        r = TGA16_R(cc);
                        g = TGA16_G(cc);
                        b = TGA16_B(cc);
                        a = 0xff;
                        break;
                        
                    }
                    for(i=0; i<flag; i++)
                    {
                        if(offset >= size)
                            continue;

                        x = offset % image_width;
                        y = image_height - 1 - offset/image_width;
                        pix_offset = y * image_width + x;
                        image[3 * pix_offset + 0] = r;
                        image[3 * pix_offset + 1] = g;
                        image[3 * pix_offset + 2] = b;
                        alpha[pix_offset] = a;

                        offset++;
                    }
                }
            }
            break;
        }
    default:
        {
            //sprintf((char *)print_rec,"%s","tga image type not support");
            //log_error(1,print_rec);
            result = ERRNO_FORMATMISMATCH;
            goto error;
            //break;
        }
    }
    if(f) { FILECLOSE(f); f = NULL; }

    if(tga_head.bDescriptor  & 0x20)
    {
        //rotate image & alpha top vs bottom
        for(x = image_width - 1; x >= 0; x--)
        {
            for( y = 0; y < image_height / 2; y ++)
            {
                pix_offset = y * image_width + x;
                pix_offset2 = (image_height - 1 - y) * image_width + x;

                r = image[3 * pix_offset + 0];
                g = image[3 * pix_offset + 1];
                b = image[3 * pix_offset + 2];
                a = alpha[pix_offset];

                image[3 * pix_offset + 0] = image[3 * pix_offset2 + 0];
                image[3 * pix_offset + 1] = image[3 * pix_offset2 + 1];
                image[3 * pix_offset + 2] = image[3 * pix_offset2 + 2];
                alpha[pix_offset] = alpha[pix_offset2];

                image[3 * pix_offset2 + 0] = r;
                image[3 * pix_offset2 + 1] = g;
                image[3 * pix_offset2 + 2] = b;
                alpha[pix_offset2] = a;
            }
        }
    }
    if(tga_head.bDescriptor  & 0x10)
    {
        //rotate image & alpha left vs right
        for(y = image_height - 1; y >= 0; y --)
        {
            for(x = 0; x < image_width/2; x ++)
            {
                pix_offset = y * image_width + x;
                pix_offset2 = y * image_width + image_width - 1 - x;

                r = image[3 * pix_offset + 0];
                g = image[3 * pix_offset + 1];
                b = image[3 * pix_offset + 2];
                a = alpha[pix_offset];

                image[3 * pix_offset + 0] = image[3 * pix_offset2 + 0];
                image[3 * pix_offset + 1] = image[3 * pix_offset2 + 1];
                image[3 * pix_offset + 2] = image[3 * pix_offset2 + 2];
                alpha[pix_offset] = alpha[pix_offset2];

                image[3 * pix_offset2 + 0] = r;
                image[3 * pix_offset2 + 1] = g;
                image[3 * pix_offset2 + 2] = b;
                alpha[pix_offset2] = a;
            }
        }
    }

    *w = image_width;
    *h = image_height;
    *rgb_buffer = image;
    *alpha_buffer = alpha;
    return  ERRNO_OK;

error:
    if(f) { FILECLOSE(f); f = NULL; }
    if(image) { free(image); image = NULL; }
    if(alpha) { free(alpha); alpha = NULL; }
    return  result;
}




//add+lincoln
//[2005:1:24] [15:44]
EXPORT  SLONG    FNBACK  dither_buffer_image(unsigned char *pData,int w,int h,int bpp)
{

	if(!pData) return ERRNO_MEMORY;


	for (int y = 0; y < h - 1; y++)
	{

		for (int x = 0; x < w - 1; x++)
		{

			for (int i = 0; i < bpp; i++)
			{

			
			  //计算误差值
			  int ditherValue = *(pData + (y * w + x) * bpp + i) % 16;

			  //计算近似值
			  *(pData + (y * w + x) * bpp + i) -= ditherValue;


			  if (*(pData + ((y + 1) * w + x) * bpp + i)  + ditherValue * 0.375 <= 255)
				 *(pData + ((y + 1) * w + x) * bpp + i)  += ditherValue * 0.375;

     

			  if (*(pData + (y * w + x + 1) * bpp + i) + ditherValue * 0.375 <= 255)
				 *(pData + (y * w + x + 1) * bpp + i)  += ditherValue * 0.375;

     

			  if (*(pData + ((y + 1) * w + x + 1) * bpp + i)  + ditherValue * 0.25 <= 255)
				 *(pData + ((y + 1) * w + x + 1) * bpp + i)  += ditherValue * 0.25;

			  }

		}

	}

	return ERRNO_OK;



}



//============================================================================
