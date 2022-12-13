/*
**  xImage.h
**  some image file support functions.
**
**  Jim(LiuZiJian), 2003.6.25.
**
*/
#ifndef _XIMAGE_H_
#define _XIMAGE_H_

//add+lincoln
//[2004:11:8] [12:44]
#define _VIRTUAL_FILE_  

#ifdef _VIRTUAL_FILE_ 

#include "kernel.h"
#include "ximage.h"
#include "vfilewrap.h"

#define FILEPT		FILE_V
#define FILEOPEN	vfopen
#define FILECLOSE	vfclose
#define FILESEEK	vfseek
#define FILETELL	vftell
#define FILEREAD	vfread
#define FILEWRITE   vfwrite
#define FILEPRINTF  vfprintf
#define FILEEOF		vfeof
#else
#define FILEPT		FILE
#define FILEOPEN	fopen
#define FILECLOSE	fclose
#define FILESEEK	fseek
#define FILETELL	ftell
#define FILEREAD	fread
#define FILEWRITE   fwrite
#define FILEPRINTF  fprintf
#define FILEEOF		feof
#endif


//add+lincoln
enum
{
    ERRNO_OK                =   0,
    ERRNO_FILEOPEN          =   -1,
    ERRNO_MEMORY            =   -2,
    ERRNO_FORMATMISMATCH    =   -3,
};

//
// S T R U C T S //////////////////////////////////////////////////////////////////////////////////////////////////
//

/////////////////////////////////////////////////////////////////////////////
// PSD FILE

//delete+lincoln
//[2004:11:8] [14:23]
/*
typedef struct tagCOLOR_MODE_SECTION
{
   ULONG lLength;       // the length of color_mode data in this part
   //// variable color_mode data
   //// color data such as palette ....
} COLOR_MODE_SECTION,*LPCOLOR_MODE_SECTION;


typedef struct tagIMAGE_RESOURCE_SECTION
{
   ULONG lLength;      // the lenght of IMAGE_RESOURCE 
   //// variable image_resource data
} IMAGE_RESOURCE_SECTION,*LPIMAGE_RESOURCE_SECTION;


typedef struct tagLAYER_MASK_INFO_SECTION
{
    ULONG lLength;      // the lenght of LAYER AND MASK INFORMATION 
    //// variable layer information
    //// variable mask information
} LAYER_MASK_INFO_SECTION,*LPLAYER_MASK_INFO_SECTION;


typedef struct tagLAYER_INFO
{
   ULONG lLength;      // the lenght  of LAYER INFORMATION 
   //// variable layer_structure
   //// variable pixel data  
} LAYER_INFO,*LPLAYER_INFO;


#pragma pack(push)
#pragma pack(2)
typedef struct tagLAYER_STRUCT
{
   SHINT iCount;        // number of layer 
                        // if iCount<0 take absolute value
   //// variable LAYER RECORD
} LAYER_STRUCT,*LPLAYER_STRUCT;
#pragma pack(pop)




#pragma pack(push)
#pragma pack(1)
typedef struct tagLAYER_MASK_INFO
{
   SLONG lLength;
   UHINT iColorSpace;
   SLONG lColorComponent1;
   SLONG lColorComponent2;
   UHINT iOpacity;
   UCHR  bKind;
   UCHR  bFiller;
} LAYER_MASK_INFO,*LPLAYER_MASK_INFO;
#pragma pack(pop)

*/


#pragma pack(push)
#pragma pack(2)
typedef struct tagCHANNEL_INFO
{
   SHINT iChannelId;
   SLONG lLength;       // Lenght of following channel data
   USTR *pbChannel;
} CHANNEL_INFO,*LPCHANNEL_INFO;
#pragma pack(pop)

#pragma pack(push)
#pragma pack(1)
typedef struct tagPSD_HEAD
{
   USTR  abKeyWork[4];  //"8BPS"
   UHINT iVersion;      // version
   UHINT iReserved0;
   UHINT iReserved1;
   UHINT iReserved2;
   UHINT iChannels;
   ULONG lRows;         //height
   ULONG lColumns;      //width
   UHINT iDepth;
   UHINT iMode;
} PSD_HEAD,*LPPSD_HEAD;
#pragma pack(pop)

#pragma pack(push)
#pragma pack(1)
typedef struct tagLAYER_RECORDS
{
   SLONG lTop;
   SLONG lLeft;
   SLONG lBottom;
   SLONG lRight;

   UHINT iChannelNum;  // the num of channel in this layer
   
   CHANNEL_INFO *ptChannelInfo;

   USTR  abKeyWork[4];  // always be 8BIM
   USTR  abBendMode[4]; // 
   UCHR  bOpacity;
   UCHR  bClipping;
   UCHR  bFlag;
   UCHR  bFiller;       // always be zero
   SLONG lExtraSize;    // use this flag dump all left data
   // ......
} LAYER_RECORDS,*LPLAYER_RECORDS;
#pragma pack(pop)

typedef struct  tagPSDFILE
{
    FILEPT          *fp;
    SLONG           offset;
    SLONG           layers;
    SLONG           graph_offset;
    PSD_HEAD        psd_head;
    LAYER_RECORDS   *layer_record;
    SLONG           *layer_offset;
} PSDFILE, *LPPSDFILE;



//add+lincoln
//[2004:11:8] [12:52]
//
//GIF FILE//////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
// Indexed Color Image (256色图像)
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



/////////////////////////////////////////////////////////////////////////////
// Indexed Color Ani (256色图像动画)
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


//
// P R O T O T Y P E S ////////////////////////////////////////////////////////////////////////////////////////////
//
// PSD FILE -------------------------------
EXPORT  PSDFILE *   FNBACK  open_psd_file(char *filename,SLONG *layers,SLONG *xl,SLONG *yl);
EXPORT  SLONG       FNBACK  read_psd_layer_info(SLONG layer,SLONG *sx,SLONG *sy,SLONG *xl,SLONG *yl,PSDFILE *f);
EXPORT  SLONG       FNBACK  read_psd_layer_image(SLONG layer,USTR *rgb_buffer,USTR *alpha_buffer,PSDFILE *f);
EXPORT  SLONG       FNBACK  read_psd_graph_image(USTR *rgb_buffer, PSDFILE *f);
EXPORT  void        FNBACK  close_psd_file(PSDFILE *f);
EXPORT  void        FNBACK  set_psd_transparency_level(SLONG level);

// BMP FILE -------------------------------
EXPORT  SLONG	    FNBACK  BMP_load_file(CHAR *filename, SLONG *w, SLONG *h, USTR **rgb_buffer);
EXPORT  SLONG       FNBACK  BMP_save_file(char *filename, SLONG w, SLONG h, USTR *rgb_buffer);

// TGA FILE -------------------------------
EXPORT  SLONG       FNBACK  TGA_load_file(char *filename, SLONG *w, SLONG *h,
                                          USTR **rgb_buffer, USTR **alpha_buffer);

// PCX FILE -------------------------------
EXPORT  SLONG       FNBACK  PCX_load_file(char *filename, SLONG *w, SLONG *h, USTR **rgb_buffer);

// GIF FILE -------------------------------

//modify+lincoln
//[2004:11:8] [11:55]
EXPORT  int init_gif_loader();
EXPORT  void free_gif_loader();
EXPORT  IDXANI* load_idxani_from_gif(char *filename, BOOL bForceFromFile = TRUE);

//add+lincoln
//[2004:11:8] [11:51]
EXPORT  SLONG   FNBACK  get_buffer_from_idxanim(IDXANI* pIdxAim, SLONG iframe, SLONG *w, SLONG *h,
												USTR **rgb_buffer, USTR **alpha_buffer);

//add+lincoln
//[2005:1:24] [15:44]
//抖动指定图像
EXPORT  SLONG       FNBACK  dither_buffer_image(unsigned char *pData,int w,int h,int bpp);


#endif//_XIMAGE_H_



