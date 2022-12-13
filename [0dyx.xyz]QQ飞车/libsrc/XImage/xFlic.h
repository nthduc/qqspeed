/*
**  vflic.h
**  flic file functions header.
**
**  zjian,2001.5.26.
*/
#ifndef _VFLIC_H_INCLUDE_
#define _VFLIC_H_INCLUDE_

//add+lincoln
//[2004:11:8] [14:28]
#include "xImage.h"


#define OPEN_FLIC_MODE_640X480      0
#define OPEN_FLIC_MODE_320X200      1


#pragma pack(push)
#pragma pack(1)

typedef struct
{
    int           FileSize;      //  1~4
    unsigned char Magic[2];      //  5~6
    short int     TotalFrame;    //  7~8
    short int     Width;         //  9~10
    short int     Height;        // 11~12
    short int     Depth;         // 13~14
    short int     Flag;          // 15~16
    short int     Speed;         // 17~18
    char          dummy[110];    // 19~128
} FLIC_FILE_HEADER;

typedef struct
{
    int           FrameSize;     // 1~4
    unsigned char Magic[2];      // 5~6
    short int     TotalChunk;    // 7~8
    char          dummy[8];      // 9~16
} FLIC_FRAME_HEADER;

typedef struct
{
    int           ChunkSize;     // 1~4
    short int     ChunkType;     // 5~6
} FLIC_CHUNK_HEADER;

#pragma pack(pop)

//delete+lincoln
//[2004:11:8] [14:28]
/*
EXPORT  SLONG   FNBACK  FLIC_open_flic_file(USTR *name,SLONG mode_flag,USTR *memory_plane);
EXPORT  void    FNBACK  FLIC_close_flic_file(void);
EXPORT  SHINT   FNBACK  FLIC_read_flic_data(void);
EXPORT  SHINT   FNBACK  FLIC_play_flic_frame(void);
EXPORT  void    FNBACK  FLIC_export_palette(USTR *palette);
*/

#endif//_VFLIC_H_INCLUDE_
