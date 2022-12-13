/*
 *	xGif.h
 *  GIF file support functions header.
 *
 *  Jim, 2003.6.4.
 *
 */
#ifndef _XGIF_H_
#define _XGIF_H_


//add+lincoln
//[2004:11:8] [14:17]
#include "xImage.h"


//delete+lincoln
//[2004:11:8] [14:17]
/*

/////////////////////////////////////////////////////////////////////////////
// Indexed Color Image (256ɫͼ��)
#ifndef __IDXIMG_STRUCT__
#define __IDXIMG_STRUCT__
typedef struct tagIDXIMG
{
    WORD    left;           //��Ӧ�����߼�����ƫ������
    WORD    top;            //��Ӧ�����߼�����ƫ������
    WORD    width;          //ͼ����
    WORD    height;         //ͼ��߶�
    BYTE    is_trans;       //�Ƿ���͸��ɫ
    BYTE    trans_color;    //͸��ɫ����ֵ
    BYTE    *data;          //Ӱ������(256ɫ)
    BYTE    *palette;       //ɫ��(1+256*RGB,769�ֽ�)
                            //ǰ��1�ֽڱ�ʾɫ������:
                            //����ʱ��3,��ʾRGB
                            //ת������Ӧ�Կ���Ϊ2,��ʾhicolor
} IDXIMG, *LPIDXIMG;
#endif//__IDXFRM_STRUCT__


/////////////////////////////////////////////////////////////////////////////
// Indexed Color Ani (256ɫͼ�񶯻�)
#ifndef __IDXANI_STRUCT__
#define __IDXANI_STRUCT__
typedef struct tagIDXANI 
{
    WORD    width;              //�����߼����
    WORD    height;             //�����߼��߶�
    BYTE    background_color;   //����ɫ����
    UINT    present_frames;     //��������֡��
    UINT    image_frames;       //ʵ��Ӱ��֡��
    BYTE    palette_type;       //ɫ������
                                //����ʱ��3,��ʾRGB
                                //ת������Ӧ�Կ���Ϊ2,��ʾhicolor
    UINT    *frame_index;       //ÿ������֡��Ӧ��ʵ��Ӱ��֡������
    BYTE    *global_palette;    //ȫ��ɫ��(1 + 256 * RGB, 769�ֽ�)
                                //ǰ��1�ֽڱ�ʾɫ������:
                                //����ʱ��3,��ʾRGB
                                //ת������Ӧ�Կ���Ϊ2,��ʾhicolor
    IDXIMG  **frame_image;      //ÿ��ʵ��Ӱ��֡��ͼ��
} IDXANI, *LPIDXANI;
#endif//__IDXANI_STRUCT__


/////////////////////////////////////////////////////////////////////////////
// Functions
IDXANI* load_idxani_from_gif(char *filename, BOOL bForceFromFile = TRUE);
void destroy_idxani(IDXANI **ani);

// Jim, added [10/28/2003]
//{{{
int		init_gif_loader();
void	free_gif_loader();
//}}}


*/


#endif//_XGIF_H_

