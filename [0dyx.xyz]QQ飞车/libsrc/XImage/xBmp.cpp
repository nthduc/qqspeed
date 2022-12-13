/*
**  xBmp.cpp
**  BMP file functions.
**
**  Jim(LiuZiJian), 2003.6.24.
**		Created for library "XImage".
**
**
*/
#include "Engine.h"
#include "xBmp.h"


#define	log_error(a,b)	

#pragma pack (push)
#pragma pack (2)
typedef struct tagBITMAPHEADER
{
    UHINT type;
    ULONG size;
    ULONG reserved;
    ULONG off_bits;
    ULONG head_size;
    ULONG width;
    ULONG height;
    UHINT planes;
    UHINT bit; 
} BITMAPHEADER,*LPBITMAPHEADER;;
#pragma pack (pop)



EXPORT  SLONG	FNBACK  BMP_load_file(CHAR *filename, SLONG *w, SLONG *h, USTR **rgb_buffer)
{
	enum
	{	
		ERRNO_OK				=	0,		//����
		ERRNO_FILEOPEN			=	-1,		//���ļ�����
		ERRNO_FORMATMISMATCH	=	-2,		//��ʽ����
		ERRNO_MEMORY			=	-3,		//�ڴ����
	};
	//
    BITMAPHEADER	bmph;
    FILEPT *	fp=NULL;
	SLONG	result, i, j;
	USTR	*image_buffer = NULL, *image_ptr = NULL;
	SLONG	image_width, image_height, image_size;
	BYTE *	line_buffer = NULL, *line_ptr = NULL;
	SLONG	pitch, offset;
    
	result = ERRNO_OK;

	//
	// ���ļ�׼����ȡ
	//
    if (NULL == (fp = FILEOPEN(filename,"rb"))) 
    {
		result = ERRNO_FILEOPEN;
		goto _error_exit;
    }

	//
	// ��ȡӰ���ļ�ͷ
	//
    FILEREAD(&bmph,1,sizeof(bmph),fp);
    if (bmph.type != 19778 || (bmph.bit != 24 && bmph.bit !=8)) 
    {
		result = ERRNO_FORMATMISMATCH;
		goto _error_exit;
    }

    //
    // ���Ӱ���С������Ӱ���ڴ�
    //
    image_width = bmph.width;
    image_height = bmph.height;
    image_size = image_width * image_height * 3;
    if(NULL == (image_buffer = (USTR*)malloc(image_size)))
    {
        result = ERRNO_MEMORY;
        goto _error_exit;
    }
    memset(image_buffer, 0, image_size);

    //
    // ��ȡӰ������
    //
    if(8 == bmph.bit)	// 8λ��256ɫBMPͼ��
    {
        BITMAPINFOHEADER bitinfo;
        RGBQUAD	pal[256];
        SLONG	pal_size, i, j;

        //
        // ��ȡӰ����Ϣ����ͷ
        //
        FILESEEK(fp, sizeof(BITMAPFILEHEADER), SEEK_SET);
        FILEREAD(&bitinfo, 1, sizeof(bitinfo), fp);
        
		//
		// ��ȡ��ɫ��
		//
        if(bitinfo.biClrUsed)
        {
            pal_size = bitinfo.biClrUsed * sizeof(RGBQUAD);
        }
        else
        {
			pal_size = (bitinfo.biBitCount == 8) ? 256 * sizeof(RGBQUAD) : 0;
        }
        FILEREAD(&pal, 1, pal_size, fp);

		//
		// ���ж�ȡӰ�����ݣ�������Ӱ���ڴ���
		//
        pitch = (image_width + 3) & 0xfffffffc;
        line_buffer = (BYTE*)malloc(pitch);
        for(i = image_height-1; i>=0; i--)
        {
            offset = i * image_width * 3;
            image_ptr = image_buffer + offset;
            FILEREAD(line_buffer, 1, pitch, fp);
            for(j=0; j<image_width; j++)
			{
				*image_ptr++ = pal[line_buffer[j]].rgbRed;
				*image_ptr++ = pal[line_buffer[j]].rgbGreen;
				*image_ptr++ = pal[line_buffer[j]].rgbBlue;
			}
        }
        if(line_buffer) {free(line_buffer); line_buffer = NULL; }
        if(fp) { FILECLOSE(fp); fp = NULL; }
    }
	else //24λ���BMPͼ��(0xAARRGGBB)
	{
        //
        // ����Ӱ������
        //
        FILESEEK(fp, bmph.off_bits, SEEK_SET);

		//
		// ���ж�ȡӰ�����ݣ�������Ӱ���ڴ���
        // TrueColor = 0xAARRGGBB
		//
		BYTE r, g, b;
        pitch = (image_width * 3 +  3) & 0xfffffffc;
        line_buffer = (BYTE*)malloc(pitch);
        for(i = image_height-1; i>=0; i--)
        {
            offset = i * image_width * 3;
            image_ptr = image_buffer + offset;
            FILEREAD(line_buffer, 1, pitch, fp);
            for(j=0; j<image_width; j++)
			{
				b = line_buffer[j*3+0];
				g = line_buffer[j*3+1];
				r = line_buffer[j*3+2];

                *image_ptr++ = r;
                *image_ptr++ = g;
                *image_ptr++ = b;

                //offset = i * image_width + j;
                //image_buffer[offset * 3 + 0] = r;
                //image_buffer[offset * 3 + 1] = g;
                //image_buffer[offset * 3 + 2] = b;

				//*(image_ptr+j*3+0) = r;
				//*(image_ptr+j*3+1) = g;
				//*(image_ptr+j*3+2) = b;
			}
        }
        if(line_buffer) {free(line_buffer); line_buffer = NULL; }
        if(fp) { FILECLOSE(fp); fp = NULL; }
	}

	*w = image_width;
	*h = image_height;
	*rgb_buffer = image_buffer;
	return	result;

_error_exit:
	if(fp) { FILECLOSE(fp); }
	if(image_buffer) { free(image_buffer); image_buffer = NULL; }
	if(line_buffer) { free(line_buffer); line_buffer = NULL; }
	return	result;
}


EXPORT  SLONG FNBACK  BMP_save_file(char *filename, SLONG w, SLONG h, USTR *rgb_buffer)
{
	enum
	{
		ERRNO_NOIMAGE			=	-1,
		ERRNO_FILEOPEN			=	-2,
		ERRNO_MEMORY			=	-3,		//�ڴ����
	};
    BITMAPFILEHEADER bithead;
    BITMAPINFOHEADER bitinfo;
    FILEPT	*fp = NULL;
    SLONG	headsize, pitch, append;
	SLONG	result;

    if(!rgb_buffer)
	{
		result = ERRNO_NOIMAGE;
		goto _error_exit;
	}

    pitch = (w*3 + 3) & 0xfffffffc;
    append = pitch - w * 3;

	//
	// ���ļ�׼��д
	//
    if(NULL == (fp = FILEOPEN(filename,"wb"))) 
    {
		result = ERRNO_FILEOPEN;
		goto _error_exit;
    }

	//
	// д��Ӱ���ļ�ͷ(BITMAPFILEHEADER)
	//
    headsize = sizeof(bithead) + sizeof(bitinfo);
    bithead.bfType = 'MB';
    bithead.bfSize = headsize + pitch * h;
    bithead.bfReserved1 = bithead.bfReserved2 = 0;
    bithead.bfOffBits = headsize;
    FILEWRITE(&bithead, 1, sizeof(bithead), fp);

	//
	// д��Ӱ����Ϣͷ(BITMAPINFOHEADER)
	//
    bitinfo.biSize = sizeof(bitinfo);
    bitinfo.biWidth = w;
    bitinfo.biHeight = h;
    bitinfo.biPlanes = 1;
    bitinfo.biBitCount = 24;
    bitinfo.biCompression = BI_RGB;
    bitinfo.biSizeImage = 0;
    bitinfo.biXPelsPerMeter = 72;
    bitinfo.biYPelsPerMeter = 72;
    bitinfo.biClrUsed = 0;
    bitinfo.biClrImportant = 0;
    FILEWRITE(&bitinfo, 1, sizeof(bitinfo), fp);

	//
	// ����д��Ӱ����Ϣ
	//
	ULONG	offset;
	SLONG	x, y;
	BYTE*	line_buffer;
	BYTE	r,g,b;

	if(NULL == (line_buffer = (BYTE*)malloc(pitch)))
	{
		result = ERRNO_MEMORY;
		goto _error_exit;
	}
	memset(line_buffer, 0, pitch);

    for(y = h-1; y >= 0; y --)
    {
        for(x = 0; x < w; x ++)
        {
            offset = y * w + x;
			r = rgb_buffer[offset*3 + 0];
			g = rgb_buffer[offset*3 + 1];
			b = rgb_buffer[offset*3 + 2];
			line_buffer[x*3 + 0] = b;
			line_buffer[x*3 + 1] = g;
			line_buffer[x*3 + 2] = r;
        }
		FILEWRITE(line_buffer, 1, pitch, fp);
    }
	if(fp) { FILECLOSE(fp); fp = NULL; }
    return 0;

_error_exit:
	if(fp) { FILECLOSE(fp); fp = NULL; }
	//remove((const char *)filename);

	return	result;
}


