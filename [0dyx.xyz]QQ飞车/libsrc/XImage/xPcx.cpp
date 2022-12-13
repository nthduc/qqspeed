/*
**  PCX.CPP
**  PCX functions.
**
**  ZJian(LiuZiJian), 2000.7.16.
**      256 color PCX support.
**  ZJian(LiuZiJian), 2000.9.2.
**      16M color PCX support. Special thanks to Dixon(LiJun).
**  Jim(LiuZiJian), 2003.6.25.
**      Modified for read image file to a rgb_buffer.
**  
*/
#include "Engine.h"
#include "xPcx.h"


//
// STRUCTURES ////////////////////////////////////////////////////////////////////////////////////////////////////////
//
#pragma pack(push)
#pragma pack(1)
typedef struct tagPCX_HEAD
{
   UCHR  manufacturer;
   UCHR  version;
   UCHR  encoding;
   UCHR  bitsPerPixel;
   SHINT xMin;
   SHINT yMin;
   SHINT xMax;
   SHINT yMax;
   SHINT hResolution;
   SHINT vResolution;
   UCHR  palette[48];
   UCHR  videoMode;
   UCHR  colorPlanes;
   SHINT bytesPerLines;
   SHINT paletteType;
   SHINT shResolution;
   SHINT svResolution;
   UCHR  filler[54];
} PCX_HEAD,*LPPCX_HEAD;
#pragma pack(pop)


//
// LOCALS ////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
static PCX_HEAD pcx_head;
static USTR *pcx_file_buffer = NULL;
static USTR  pcx_palette[768];

static SLONG decode_256_color_pcx_image(USTR *image_buffer, SLONG image_width, SLONG image_height);
static SLONG decode_true_color_pcx_image(USTR *image_buffer, SLONG image_width, SLONG image_height);


/********************************************************************************************************************/
/* MAIN FUNCTIONS                                                                                                   */
/********************************************************************************************************************/
EXPORT SLONG FNBACK  PCX_load_file(char *filename, SLONG *w, SLONG *h, USTR **rgb_buffer)
{
    enum
    {
        ERRNO_OK                =   0,
        ERRNO_FILEOPEN          =   -1,
        ERRNO_FORMATMISMATCH    =   -2,
        ERRNO_MEMORY            =   -3,
    };
    FILEPT* fp = NULL;
    USTR *image_buffer = NULL;
    SLONG image_width, image_height, image_size;
    UCHR ch;
    SLONG pcx_file_size;
    SLONG result;
    
    result = ERRNO_OK;

    //
    // 打开影像文件准备读取
    //
    if(NULL == (fp = FILEOPEN(filename,"rb")))
    {
        result = ERRNO_FILEOPEN;
        goto _error_exit;
    }

    //
    // 读取影像文件头并作必要检查
    //
    FILEREAD(&pcx_head, 1, sizeof(PCX_HEAD), fp);
    if( (0x0a != pcx_head.manufacturer) || (1 != pcx_head.encoding) )
    {
        result = ERRNO_FORMATMISMATCH;
        goto _error_exit;
    }
    if(8 != pcx_head.bitsPerPixel)
    {
        result = ERRNO_FORMATMISMATCH;
        goto _error_exit;
    }

    //
    // 计算影像大小并创建影像内存缓冲区
    //
    image_width = pcx_head.xMax-pcx_head.xMin+1;
    image_height = pcx_head.yMax-pcx_head.yMin+1;
    image_size = image_width * image_height * 3;
    if(NULL == (image_buffer = (USTR*)malloc(image_size)))
    {
        result = ERRNO_MEMORY;
        goto _error_exit;
    }
    memset(image_buffer, 0, image_size);

    //
    // 读取影像文件数据
    //
	//modify+lincoln
	//[2004:11:8] [15:26]
    //pcx_file_size = filelength(fileno(fp));
	pcx_file_size = vfsize(fp);



    if(NULL == (pcx_file_buffer = (UCHR *)malloc(pcx_file_size)))
    {
        result = ERRNO_MEMORY;
        goto _error_exit;
    }
    FILEREAD(pcx_file_buffer, 1, pcx_file_size, fp);

    switch(pcx_head.bitsPerPixel*pcx_head.colorPlanes)
    {
    case 8:// 256 colors 
        //
        // 读取256色盘(色盘为256*3=768字节，前面有一个leadcode=0x0c)
        //
        FILESEEK(fp,-769L,SEEK_END);
        FILEREAD(&ch, 1, 1, fp);
        if(0x0c != ch) //check palette flag
        {
            result = ERRNO_FORMATMISMATCH;
            goto _error_exit;
        }
        FILEREAD(&pcx_palette,1, 768, fp);

        //
        // 文件读取成功，关闭文件。
        //
        if(fp) { FILECLOSE(fp); fp = NULL; }

        //
        // 开始解压缩文件影像资料
        //
        if(0 != decode_256_color_pcx_image(image_buffer, image_width, image_height))
        {
            result = ERRNO_MEMORY;
            goto _error_exit;
        }
        break;

    case 24:// true color
        //
        // 文件读取成功，关闭文件。
        //
        if(fp) { FILECLOSE(fp); fp = NULL; }

        //
        // 开始解压缩文件影像资料
        //
        if(0 != decode_true_color_pcx_image(image_buffer, image_width, image_height))
        {
            result = ERRNO_MEMORY;
            goto _error_exit;
        }
        break;

    default:
        result = ERRNO_FORMATMISMATCH;
        goto _error_exit;
    }

    if(pcx_file_buffer) { free(pcx_file_buffer); pcx_file_buffer = NULL; }

    *w = image_width;
    *h = image_height;
    *rgb_buffer = image_buffer;

    return result;

_error_exit:
    if(pcx_file_buffer) { free(pcx_file_buffer); pcx_file_buffer = NULL; }
    if(fp) { FILECLOSE(fp); fp = NULL; }
    if(image_buffer) { free(image_buffer); image_buffer = NULL; }
    return  result;
}


/********************************************************************************************************************/
/* STATIC FUNCTIONS                                                                                                 */
/********************************************************************************************************************/
static  SLONG   decode_256_color_pcx_image(USTR *image_buffer, SLONG image_width, SLONG image_height)
{
    SLONG source_idx = 0, target_idx = 0;
    UCHR data=0, count=0, c;
    UCHR *decode_buffer=NULL;
    SLONG i,decode_y=0;
    USTR *image_ptr = image_buffer;
    
    if(NULL == (decode_buffer = (UCHR *)malloc(image_width * 3 + 64)))
        return  -1;

    while(decode_y < image_height)
    {
        data = pcx_file_buffer[source_idx++];
        if(0xc0 != (data & 0xc0))
        {
            decode_buffer[target_idx++] = data;
        }
        else
        {
            count=(unsigned char)(data&0x3f);
            data=pcx_file_buffer[source_idx++];
            memset(&decode_buffer[target_idx],data,count);
            target_idx = target_idx + count;
        }

        if(target_idx >= image_width)  // RGB decode 1 line OK
        {
            target_idx = 0;
            for(i=0; i<image_width;i++)
            {
                c = decode_buffer[i];
                *image_ptr++ = pcx_palette[c+c+c+0];    //r
                *image_ptr++ = pcx_palette[c+c+c+1];    //g
                *image_ptr++ = pcx_palette[c+c+c+2];    //b
            }
            decode_y++;
        }
    }

    if(decode_buffer) { free(decode_buffer); decode_buffer = NULL;}

    return  0;
}



static SLONG decode_true_color_pcx_image(USTR *image_buffer, SLONG image_width, SLONG image_height)
{
    SLONG source_idx=0,target_idx=0;
    UCHR data=0,count=0,r,g,b;
    UCHR *decode_buffer=NULL;
    SLONG i,decode_y=0;
    USTR *image_ptr = image_buffer;
    
    if(NULL == (decode_buffer = (UCHR *)malloc(image_width*3 + 64)))
        return  -1;

    while(decode_y < image_height)
    {
        data = pcx_file_buffer[source_idx++];
        if(0xc0 != (data&0xc0))
        {
            decode_buffer[target_idx++]=data;
        }
        else
        {
            count = (unsigned char)(data & 0x3f);
            data = pcx_file_buffer[source_idx++];
            memset(&decode_buffer[target_idx], data, count);
            target_idx = target_idx+count;
        }
        if(target_idx >= image_width * 3)  // RGB decode 1 line OK
        {
            target_idx = 0;
            for(i=0; i<image_width; i++)
            {
                r = decode_buffer[i];
                g = decode_buffer[i + image_width];
                b = decode_buffer[i + image_width + image_width];
                *image_ptr++ = r;
                *image_ptr++ = g;
                *image_ptr++ = b;
            }
            decode_y++;
        }
    }

    if(decode_buffer) { free(decode_buffer); decode_buffer = NULL;}
    return  0;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

