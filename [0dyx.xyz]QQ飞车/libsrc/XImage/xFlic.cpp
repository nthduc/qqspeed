/*
**  xFlic.cpp
**  flic file functions.
**  
**  stephen, 1996.7.
**      created version for 256 color engine.
**  Jack(LiuZiJian), 1999.4.29.
**      modified for 256 color dll.
**  Jack(LiuZiJian), 2001.5.26.
**      modified for make library of 16bit color engine.
**          and it will be compatible with 256 color.
**      modified color range of rgb from 0~63(64) to 0~255(256).
**  
*/
#include "Engine.h"
#include "xFlic.h"


#define	PACK_FILE	FILEPT
#define	pack_fopen	FILEOPEN
#define	pack_fread	FILEREAD
#define	pack_fseek	FILESEEK
#define	pack_fclose	FILECLOSE

#define	log_error(a,b)	


#define FLIC_FILE_HEADER_LENGTH         128
#define FLIC_FRAME_HEADER_LENGTH        16
#define FLIC_CHUNK_HEADER_LENGTH        6


//structs ---------------------------------------------------------------------------------------

/*
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
*/


//locals --------------------------------------------------------------------------------------------------
static int                  SCREEN_X_BYTE=640;
static int                  SCREEN_Y_BYTE=480;
static USTR               * flc_bitmap;
static USTR                 flc_palette[768];
static PACK_FILE          * FlicFp;
static FLIC_FILE_HEADER     FlicFileHeader;
static FLIC_FRAME_HEADER    FlicFrameHeader;
static unsigned char      * FlicFrameBuffer=(unsigned char *)NULL;
static int                  FlicLastFramePos;
static int                  FlicStartFramePos;
static int                  FlicNowFrameCount;
static int                  FrameBufferSize;

//static function prototypes -----------------------------------------------------------------------------
static  void    fnProcessFliLineCompress(unsigned char *buffer);
static  void    fnProcessFliColor(unsigned char *buffer);
static  void    fnProcessFliBrun(unsigned char *buffer);
static  void    fnProcessFliCopy(unsigned char *buffer);
static  void    fnProcessFlcColor(unsigned char *buffer);
static  void    fnProcessFlcLineCompress(unsigned char *buffer);
static  void    fnMoveData2Screen(short int xpos,short int ypos,unsigned char *buffer,unsigned char count);
static  void    fnSetData2Screen(short int xpos,short int ypos,unsigned char data,unsigned char count);
static  void    fnSet2Data2Screen(short int xpos,short int ypos,unsigned short int data,unsigned char count);
static  void    fnSetPart256Palette(short int coloridx,unsigned char *pal,unsigned char count);
static  void    fnSetFull256Palette(unsigned char *pal);
static  void    fnMoveFullScreen(unsigned char *buffer,int len);
static  void    fnSetFullScreen(int len,unsigned char data);
static  void    fnProcessFliBlack(unsigned char *buffer);


//functions --------------------------------------------------------------------------------------------
EXPORT  SLONG   FNBACK  FLIC_open_flic_file(USTR *name,SLONG mode_flag,USTR *memory_plane)
{
    int     frame_size;
    USTR    disp_rec[256];
    
    if(mode_flag==0)
    {
        SCREEN_X_BYTE=640;
        SCREEN_Y_BYTE=480;
    }
    else
    {
        SCREEN_X_BYTE=320;
        SCREEN_Y_BYTE=200;
    }
    FlicFp=pack_fopen((const char *)name,"rb");
    if(FlicFp==NULL)
    {
        sprintf((char *)disp_rec,"File %s open error",name);
        log_error(1, (USTR*)disp_rec);
        return(TTN_ERROR);
    }
    pack_fseek(FlicFp,0L,SEEK_SET);
    pack_fread(&FlicFileHeader,1,FLIC_FILE_HEADER_LENGTH,FlicFp);
    if(FlicFrameBuffer != NULL)
        GlobalFree(FlicFrameBuffer);
    frame_size=(int)FlicFileHeader.Width * (int)FlicFileHeader.Height;
    frame_size=frame_size * (int)FlicFileHeader.Depth / 8;
    FlicFrameBuffer=(unsigned char *)GlobalAlloc(GPTR, frame_size+512);
    FrameBufferSize=frame_size + 512;
    FlicNowFrameCount=0;
    FlicLastFramePos=FLIC_FILE_HEADER_LENGTH;
    flc_bitmap=memory_plane;
    return(FlicFileHeader.TotalFrame);
}
/******************************************************************************/
EXPORT  void    FNBACK  FLIC_close_flic_file(void)
{
    if(FlicFp != NULL)
        pack_fclose(FlicFp);
    if(FlicFrameBuffer!=NULL)
        GlobalFree(FlicFrameBuffer);
    FlicFrameBuffer=NULL;
}
/******************************************************************************/
EXPORT  SHINT   FNBACK  FLIC_read_flic_data(void)
{
    pack_fseek(FlicFp,FlicLastFramePos,SEEK_SET);
    pack_fread(&FlicFrameHeader,1,FLIC_FRAME_HEADER_LENGTH,FlicFp);
    FlicLastFramePos+=FlicFrameHeader.FrameSize;
    while((FlicFrameHeader.Magic[0]==0x00)&&(FlicFrameHeader.Magic[1]==0xf1))
    {
        pack_fseek(FlicFp,FlicLastFramePos,SEEK_SET);
        pack_fread(&FlicFrameHeader,1,FLIC_FRAME_HEADER_LENGTH,FlicFp);
        FlicLastFramePos+=FlicFrameHeader.FrameSize;
    }
    if(((FlicFrameHeader.Magic[0]!=0xfa)&&(FlicFrameHeader.Magic[1]!=0xf1))||
        (FlicFrameHeader.FrameSize+16 > FrameBufferSize))
    {
        pack_fclose(FlicFp);
        return(-1);
    }
    pack_fread(FlicFrameBuffer,1,FlicFrameHeader.FrameSize,FlicFp);
    return(0);
}
/******************************************************************************/
EXPORT  SHINT   FNBACK  FLIC_play_flic_frame(void)
{
    int             old_frame_buffer_index;
    int             frame_buffer_index=0;
    int             chunk_size;
    short int       i;
    short int       chunk_type;
    
    if(FlicFrameHeader.TotalChunk == 0)
        return(0);
    for(i=0;i<FlicFrameHeader.TotalChunk;i++)
    {
        chunk_size=((FLIC_CHUNK_HEADER *)&FlicFrameBuffer[frame_buffer_index])->ChunkSize;
        chunk_type=((FLIC_CHUNK_HEADER *)&FlicFrameBuffer[frame_buffer_index])->ChunkType;
        old_frame_buffer_index=frame_buffer_index;
        frame_buffer_index+=6;
        switch(chunk_type)
        {
        case 0x0b:
            fnProcessFliColor(&FlicFrameBuffer[frame_buffer_index]);
            break;
        case 0x0c:
            fnProcessFliLineCompress(&FlicFrameBuffer[frame_buffer_index]);
            break;
        case 0x0d:
            fnProcessFliBlack(&FlicFrameBuffer[frame_buffer_index]);
            break;
        case 0x0f:
            fnProcessFliBrun(&FlicFrameBuffer[frame_buffer_index]);
            break;
        case 0x04:
            fnProcessFlcColor(&FlicFrameBuffer[frame_buffer_index]);
            break;
        case 0x07:
            fnProcessFlcLineCompress(&FlicFrameBuffer[frame_buffer_index]);
            break;
        case 0x10:
            fnProcessFliCopy(&FlicFrameBuffer[frame_buffer_index]);
            break;
        case 0x12:
            break;
        default:
            printf("%c%c\n",0x07,0x07);
            break;
        }
        frame_buffer_index=old_frame_buffer_index+chunk_size;
    }
    return(0);
}
/******************************************************************************/
EXPORT  void    FNBACK  FLIC_export_palette(USTR *palette)
{
    memcpy(palette,flc_palette,768);
}
/******************************************************************************/
EXPORT  void    FNBACK  FLIC_inport_palette(USTR *palette)
{
    memcpy(flc_palette,palette,768);
}


// static functions ------------------------------------------------------------------------------------------------
static void fnProcessFliLineCompress(unsigned char *buffer)
{
    unsigned char           *usebuffer;
    short int               start_line;
    short int               change_line;
    short int               i,j;
    unsigned char           data;
    unsigned char           count;
    short int               xpos,ypos;
    unsigned short  int     packet_num;
    
    usebuffer=buffer;
    start_line=*(short int *)usebuffer;
    change_line=*(short int *)(usebuffer+2);
    usebuffer+=4;
    ypos=start_line;
    for(i=0;i<change_line;i++)
    {
        ypos=(short int)(start_line+i);
        xpos=0;
        data=*usebuffer++;
        packet_num=data;
        if(packet_num == 0)
            continue;
        for(j=0;j<packet_num;j++)
        {
            data = *usebuffer++;
            xpos = (short)(xpos+data);
            data=*usebuffer++;
            if(data & 0x80)
            {
                count= (unsigned char)((255-data)+1);
                data = *usebuffer++;
                fnSetData2Screen(xpos,ypos,data,count);
                xpos = (short)(xpos+count);
            }
            else
            {
                count=data;
                fnMoveData2Screen(xpos,ypos,usebuffer,count);
                usebuffer+=count;
                xpos =(short)(xpos+count);
            }
        }
    }
}

static void fnProcessFliColor(unsigned char *buffer)
{
    unsigned char   *usebuffer;
    short int       packet_num;
    unsigned char   data;
    short int       i;
    short int       change_color;
    
    usebuffer=buffer;
    packet_num=*(short int *)usebuffer;
    usebuffer+=2;
    change_color=0;
    for(i=0;i<packet_num;i++)
    {
        data=*usebuffer++;
        change_color =(short)(change_color+data);
        data=*usebuffer++;
        if(data == 0)
        {
            fnSetFull256Palette(usebuffer);
            usebuffer+=768;
        }
        else
        {
            fnSetPart256Palette(change_color,usebuffer,data);
            change_color = (short)(change_color+data);
            usebuffer+=(data*3);
        }
    }
}

static void fnProcessFliBrun(unsigned char *buffer)
{
    unsigned char *usebuffer;
    short int     packet_num;
    unsigned char data;
    unsigned char count;
    short int     i;
    short int     j;
    short int     xpos,ypos;
    
    usebuffer=buffer;
    for(i=0;i<FlicFileHeader.Height;i++)
    {
        ypos=i;
        xpos=0;
        data=*usebuffer++;
        packet_num=data;
        for(j=0;j<packet_num;j++)
        {
            data=*usebuffer++;
            if(data & 0x80)
            {
                count= (unsigned char)((255-data)+1);
                fnMoveData2Screen(xpos,ypos,usebuffer,count);
                xpos = (short)(xpos+count);
                usebuffer+=count;
            }
            else
            {
                count=data;
                data=*usebuffer++;
                fnSetData2Screen(xpos,ypos,data,count);
                xpos = (short)(xpos+count);
            }
        }
    }
}

static void fnProcessFliCopy(unsigned char *buffer)
{
    unsigned char *usebuffer;
    int           len;
    
    usebuffer=buffer;
    len=FlicFileHeader.Width;
    len*=FlicFileHeader.Height;
    fnMoveFullScreen(usebuffer,len);
}



static void fnProcessFlcColor(unsigned char *buffer)
{
    unsigned char   *usebuffer;
    short int       packet_num;
    unsigned char   data;
    short int       i;
    short int       change_color;
    
    usebuffer=buffer;
    packet_num=*(short int *)usebuffer;
    usebuffer+=2;
    change_color=0;
    for(i=0;i<packet_num;i++)
    {
        data=*usebuffer++;
        change_color = (short)(change_color+data);
        data=*usebuffer++;
        if(data == 0)
        {
            fnSetFull256Palette(usebuffer);
            usebuffer+=768;
        }
        else
        {
            fnSetPart256Palette(change_color,usebuffer,data);
            change_color =(short)(change_color+data);
            usebuffer+=(data*3);
        }
    }
}

static void fnProcessFlcLineCompress(unsigned char *buffer)
{
    unsigned char   *usebuffer;
    short int       line_count;
    unsigned short int temp;
    short int       i,j;
    unsigned char   data;
    unsigned char   count;
    short int       xpos,ypos;
    short unsigned int packet_num;
    
    usebuffer=buffer;
    line_count=*(short int *)usebuffer;
    temp=*(short int *)(usebuffer+2);
    if(temp & 0x8000)
    {
        ypos=(short)((65535-temp)+1);
        usebuffer+=4;
    }
    else
    {
        ypos=0;
        usebuffer+=2;
    }
    for(i=0;i<line_count;i++)
    {
        xpos=0;
        temp=*(short int *)usebuffer;
        usebuffer+=2;
        if(temp & 0x8000)
        {
            ypos = (short)(ypos+(65535-temp)+1);
            temp = *(short int *)usebuffer;
            usebuffer+=2;
        }
        packet_num=temp;
        for(j=0;j<packet_num;j++)
        {
            data=*usebuffer++;
            xpos =(short)(xpos+data);
            data=*usebuffer++;
            if(data & 0x80)
            {
                count=(unsigned char)((255-data)+1);
                temp=*(short int *)usebuffer;
                usebuffer+=2;
                fnSet2Data2Screen(xpos,(short int)(ypos+i),temp,count);
                xpos=(short)(xpos+count);
                xpos=(short)(xpos+count);
            }
            else
            {
                count=(unsigned char)(data*2);
                fnMoveData2Screen(xpos,(short int)(ypos+i),usebuffer,count);
                usebuffer+=count;
                xpos = (short)(xpos+count);
            }
        }
    }
}


static void fnProcessFliBlack(unsigned char *buffer)
{
    unsigned char *usebuffer;
    int           len;
    
    usebuffer=buffer;
    len=FlicFileHeader.Width;
    len*=FlicFileHeader.Height;
    fnSetFullScreen(len,0x00);
}


//----------------------------
static void  fnMoveData2Screen(short int xpos,short int ypos,unsigned char *buffer,unsigned char count)
{
    int     adr;
    
    adr=(int)ypos*SCREEN_X_BYTE+xpos;
    memcpy(&flc_bitmap[adr],buffer,count);
}

static void fnSetData2Screen(short int xpos,short int ypos,unsigned char data,unsigned char count)
{
    int adr;
    
    adr=(int)ypos*SCREEN_X_BYTE+xpos;
    memset(&flc_bitmap[adr],data,count);
}

static void fnSet2Data2Screen(short int xpos,short int ypos,unsigned short int data,unsigned char count)
{
    int     adr;
    int     i;
    unsigned char d0,d1;
    
    adr=(int)ypos*SCREEN_X_BYTE+xpos;
    d0=(unsigned char)(data & 0x00FF);
    d1=(unsigned char)((data & 0xFF00) >> 8);
    for(i=0;i<count;i++)
    {
        flc_bitmap[adr+i*2]=d0;
        flc_bitmap[adr+i*2+1]=d1;
    }
}

static void fnSetPart256Palette(short int coloridx,unsigned char *pal,unsigned char count)
{
    int     i,j;
    
    for(i=0,j=3*coloridx;i<3*count;i++,j++)
    {
        //geaan, 2001.5.26, changed for hi & true color engine.
        //flc_palette[j]=(unsigned char)(pal[i]>>2);
        flc_palette[j]=(unsigned char)(pal[i]);
    }
}

static void fnSetFull256Palette(unsigned char *pal)
{
    int     i;
    
    for(i=0;i<768;i++)
    {
        //geaan, 2001.5.26, changed for hi & true color engine.
        //flc_palette[i]=(unsigned char)(pal[i]>>2);
        flc_palette[i]=(unsigned char)(pal[i]);
    }
}


static void fnMoveFullScreen(unsigned char *buffer,int len)
{
    memcpy(flc_bitmap,buffer,len);
}


static void fnSetFullScreen(int len, unsigned char data)
{
    memset(flc_bitmap,data,len);
}

