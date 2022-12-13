/*
**  VPSD.H
**  PSD file support functions header.
**  
**  ZJian,2000.11.29.
*/
#ifndef _VPSD_H_INCLUDE_
#define _VPSD_H_INCLUDE_    1

//add+lincoln
//[2004:11:8] [14:29]
#include "xImage.h"


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


//delete+lincoln
//[2004:11:8] [14:23]
/*

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

*/

//delete+lincoln
//[2004:11:8] [14:26]
/*
// P R O T O T Y P E S ////////////////////////////////////////////////////////////////////////////////////////////

EXPORT  PSDFILE *   FNBACK  open_psd_file(char *filename,SLONG *layers,SLONG *xl,SLONG *yl);
EXPORT  SLONG       FNBACK  read_psd_layer_info(SLONG layer,SLONG *sx,SLONG *sy,SLONG *xl,SLONG *yl,PSDFILE *f);
EXPORT  SLONG       FNBACK  read_psd_layer_image(SLONG layer,USTR *rgb_buffer,USTR *alpha_buffer,PSDFILE *f);
EXPORT  SLONG       FNBACK  read_psd_graph_image(USTR *rgb_buffer, PSDFILE *f);
EXPORT  void        FNBACK  close_psd_file(PSDFILE *f);
EXPORT  void        FNBACK  set_psd_transparency_level(SLONG level);
*/


#endif//PSD_H_INCLUDE
