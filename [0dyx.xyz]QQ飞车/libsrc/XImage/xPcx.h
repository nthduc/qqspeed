/*
**    VPCX.H
**    PCX functions header.
**    ZJian,2000/7/12.
*/
#ifndef _VPCX_H_INCLUDE_
#define _VPCX_H_INCLUDE_      1

//add+lincoln
//[2004:11:8] [14:29]
#include "xImage.h"

/*
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
*/

//delete+lincoln
//[2004:11:8] [14:29]
/*
EXPORT SLONG FNBACK  PCX_load_file(char *filename, SLONG *w, SLONG *h, USTR **rgb_buffer);
*/


#endif//_VPCX_H_INCLUDE_
