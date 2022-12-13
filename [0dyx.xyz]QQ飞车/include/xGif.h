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
// Indexed Color Image (256色图像)
#ifndef __IDXIMG_STRUCT__
#define __IDXIMG_STRUCT__
typedef struct tagIDXIMG
{
    WORD    left;           //对应动画逻辑起点的偏移坐标
    WORD    top;            //对应动画逻辑起点的偏移坐标
    WORD    width;          //图像宽度
    WORD    height;         //图像高度
    BYTE    is_trans;       //是否有透明色
    BYTE    trans_color;    //透明色索引值
    BYTE    *data;          //影像资料(256色)
    BYTE    *palette;       //色盘(1+256*RGB,769字节)
                            //前面1字节表示色盘类型:
                            //载入时＝3,表示RGB
                            //转换到对应显卡后为2,表示hicolor
} IDXIMG, *LPIDXIMG;
#endif//__IDXFRM_STRUCT__


/////////////////////////////////////////////////////////////////////////////
// Indexed Color Ani (256色图像动画)
#ifndef __IDXANI_STRUCT__
#define __IDXANI_STRUCT__
typedef struct tagIDXANI 
{
    WORD    width;              //动画逻辑宽度
    WORD    height;             //动画逻辑高度
    BYTE    background_color;   //背景色索引
    UINT    present_frames;     //动画表现帧数
    UINT    image_frames;       //实际影像帧数
    BYTE    palette_type;       //色盘类型
                                //载入时＝3,表示RGB
                                //转换到对应显卡后为2,表示hicolor
    UINT    *frame_index;       //每个表现帧对应的实际影像帧索引表
    BYTE    *global_palette;    //全局色盘(1 + 256 * RGB, 769字节)
                                //前面1字节表示色盘类型:
                                //载入时＝3,表示RGB
                                //转换到对应显卡后为2,表示hicolor
    IDXIMG  **frame_image;      //每个实际影像帧的图像
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

