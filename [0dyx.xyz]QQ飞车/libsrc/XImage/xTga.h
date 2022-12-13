/*
**    VTGA.H
**    TGA file functions header.
**    ZJian,2000/7/10.
*/
#ifndef _VTGA_H_INCLUDE_
#define _VTGA_H_INCLUDE_      1

//add+lincoln
//[2004:11:8] [14:45]
#include "xImage.h"

/*
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
*/

//add+lincoln
//[2004:11:8] [14:30]
/*
EXPORT  SLONG   FNBACK  TGA_load_file(char *filename, SLONG *w, SLONG *h, 
                                      USTR **image_buffer, USTR **alpha_buffer);
*/


#endif//_VTGA_H_INCLUDE_

