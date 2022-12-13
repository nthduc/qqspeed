/*
**  XBMP.H
**  BMP file support functions header.
**
**	Jim(LiuZiJian), 2003.6.24.
**		Created.	
**
*/
#ifndef _XBMP_H_
#define _XBMP_H_

//add+lincoln
//[2004:11:8] [15:14]
#include "xImage.h"


/*
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
*/


//delete+lincoln]
//[2004:11:8] [14:19]
/*
EXPORT  SLONG	    FNBACK  BMP_load_file(CHAR *filename, SLONG *w, SLONG *h, USTR **rgb_buffer);
EXPORT  SLONG       FNBACK  BMP_save_file(char *filename, SLONG w, SLONG h, USTR *rgb_buffer);
*/

#endif//_XBMP_H_
