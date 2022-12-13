/*
**  XPSD.CPP
**  PSD file support functions.
**
**  Dixon(LiJun), 2000.6.12.
**      PSD file format & decode function.
**
**  ZJian(LiuZiJian), 2000.8.30.
**      Added in for game sdk.
**
**  ZJian(LiuZiJian), 2000.11.30.
**      (1) Optimized for read psd image layer by layer.
**      (2) Support multi PSD file exist at the same time.
**
**  Jim(LiuZiJian), 2003.6.25.
**      Modified for load layer image to rgb_buffer.
**
**
*/
#include "Engine.h"
#include "xPsd.h"

// S T R U C T S //////////////////////////////////////////////////////////////////////////////////////////////////
/*
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


typedef struct  tagPSDFILE
{
    FILEPT            *fp;
    SLONG           offset;
    SLONG           layers;
    SLONG           graph_offset;
    PSD_HEAD        psd_head;
    LAYER_RECORDS   *layer_record;
    SLONG           *layer_offset;
} PSDFILE, *LPPSDFILE;
*/


// D E F I N E S //////////////////////////////////////////////////////////////////////////////////////////////////////
#define R_CHANNEL               0
#define G_CHANNEL               1
#define B_CHANNEL               2
#define TRANSPARENCY            -1


#define CHUNK_PSD_HEAD                      1
#define CHUNK_COLOR_MODE_SECTION            2
#define CHUNK_IMAGE_RESOURCE_SECTION        3
#define CHUNK_LAYER_MASK_INFO_SECTION       4
#define CHUNK_LAYER_INFO                    5
#define CHUNK_LAYER_STRUCT                  6
#define CHUNK_LAYER_RECORDS                 7
#define CHUNK_CHANNEL_INFO                  8
#define CHUNK_LAYER_MASK_INFO               9

// G L O B A L S //////////////////////////////////////////////////////////////////////////////////////////////////////

static  UCHR    psd_trans_level=128;


// P R O T O T Y P E S ///////////////////////////////////////////////////////////////////////////////////////////////
static  void    change_u4_turn(ULONG *pl);
static  void    change_u2_turn(UHINT *pi);
static  SLONG   read_psd_chunk(void *buffer, SLONG chunk_type, FILEPT *f);
static  void    decode_channel(USTR *pbDes,USTR *pbScr,SLONG xl,SLONG yl);
static  void    decode_image_data(FILEPT *f,USTR *pbChannel,UHINT *piLength,SLONG xl,SLONG yl);

// S T A T I C   F U N C T I O N S ///////////////////////////////////////////////////////////////////////////////////
static  void    change_u4_turn(ULONG *pl)
{
    *pl=(*pl>>24)|((*pl>>8)&0xFF00)|((*pl<<8)&0x00FF0000)|(*pl<<24);
}

static  void    change_u2_turn(UHINT *pi)
{
    *pi=(UHINT)((*pi>>8)|(*pi<<8));
}

static  SLONG   read_psd_chunk(void *buffer, SLONG chunk_type, FILEPT* f)
{
    SLONG   offset;

    offset = 0;
    switch(chunk_type)
    {
    case CHUNK_PSD_HEAD:
        {
            PSD_HEAD *psd_head=(PSD_HEAD *)buffer;

            FILEREAD(psd_head,1,sizeof(PSD_HEAD),f);
            change_u2_turn(&psd_head->iVersion);
            change_u2_turn(&psd_head->iChannels);
            change_u4_turn(&psd_head->lRows);
            change_u4_turn(&psd_head->lColumns);
            change_u2_turn(&psd_head->iDepth);
            change_u2_turn(&psd_head->iMode);

            offset = sizeof(PSD_HEAD);
        }
        break;
    case CHUNK_COLOR_MODE_SECTION:
        {
            COLOR_MODE_SECTION *pt = (COLOR_MODE_SECTION *)buffer;

            FILEREAD(&pt->lLength,4,1,f);
            change_u4_turn(&pt->lLength);

            offset = 4;
        }
        break;
    case CHUNK_IMAGE_RESOURCE_SECTION:
        {
            IMAGE_RESOURCE_SECTION *pt = (IMAGE_RESOURCE_SECTION *)buffer;

            FILEREAD(&pt->lLength,4,1,f);
            change_u4_turn(&pt->lLength);

            offset = 4;
        }
        break;
    case CHUNK_LAYER_MASK_INFO_SECTION:
        {
            LAYER_MASK_INFO_SECTION *pt = (LAYER_MASK_INFO_SECTION *)buffer;

            FILEREAD(&pt->lLength,4,1,f);
            change_u4_turn(&pt->lLength);

            offset = 4;
        }
        break;
    case CHUNK_LAYER_INFO:
        {
            LAYER_INFO *pt = (LAYER_INFO *)buffer;

            FILEREAD(&pt->lLength,4,1,f);
            change_u4_turn(&pt->lLength);

            offset = 4;
        }
        break;
    case CHUNK_LAYER_STRUCT:
        {
            LAYER_STRUCT *pt = (LAYER_STRUCT *)buffer;

            FILEREAD(&pt->iCount,2,1,f);
            change_u2_turn((UHINT *)&pt->iCount);

            offset = 2;
        }
        break;
    case CHUNK_LAYER_RECORDS:
        {
            LAYER_RECORDS *pt = (LAYER_RECORDS *)buffer;

            FILEREAD(&pt->lTop,4,1,f);
            change_u4_turn((ULONG *)&pt->lTop);
            offset += 4;

            FILEREAD(&pt->lLeft,4,1,f);
            change_u4_turn((ULONG *)&pt->lLeft);
            offset += 4;

            FILEREAD(&pt->lBottom,4,1,f);
            change_u4_turn((ULONG *)&pt->lBottom);
            offset += 4;

            FILEREAD(&pt->lRight,4,1,f);
            change_u4_turn((ULONG *)&pt->lRight);
            offset += 4;

            FILEREAD(&pt->iChannelNum,2,1,f);
            change_u2_turn(&pt->iChannelNum);
            offset += 2;

            if(pt->ptChannelInfo!=NULL)
            {
                free(pt->ptChannelInfo);
                pt->ptChannelInfo=NULL;
            }
            pt->ptChannelInfo=(CHANNEL_INFO *)malloc(sizeof(CHANNEL_INFO)*pt->iChannelNum);
            for(SLONG i=0;i<pt->iChannelNum;i++)
                pt->ptChannelInfo[i].pbChannel=NULL;
            for(i=0; i<pt->iChannelNum;i++)
            {
                CHANNEL_INFO *pci = (CHANNEL_INFO *)&pt->ptChannelInfo[i];
                FILEREAD(&pci->iChannelId,2,1,f);
                change_u2_turn((UHINT *)&pci->iChannelId);
                offset += 2;

                FILEREAD(&pci->lLength,4,1,f);
                change_u4_turn((ULONG *)&pci->lLength);
                offset += 4;
            }
            FILEREAD(pt->abKeyWork,4,1,f);
            offset += 4;

            FILEREAD(pt->abBendMode,4,1,f);
            offset += 4;

            FILEREAD(&pt->bOpacity,1,1,f);
            offset += 1;

            FILEREAD(&pt->bClipping,1,1,f);
            offset += 1;

            FILEREAD(&pt->bFlag,1,1,f);
            offset += 1;

            FILEREAD(&pt->bFiller,1,1,f);
            offset += 1;

            FILEREAD(&(pt->lExtraSize),4,1,f);
            offset += 4;

            // skip extra size
            change_u4_turn((ULONG *)&pt->lExtraSize);
            FILESEEK(f,pt->lExtraSize,SEEK_CUR);
            offset += pt->lExtraSize;
        }
        break;
    case CHUNK_CHANNEL_INFO:
        {
            CHANNEL_INFO *pt = (CHANNEL_INFO *)buffer;

            FILEREAD(&pt->iChannelId,2,1,f);
            change_u2_turn((UHINT *)&pt->iChannelId);
            offset += 2;

            FILEREAD(&pt->lLength,4,1,f);
            change_u4_turn((ULONG *)&pt->lLength);
            offset += 4;
        }
        break;
    case CHUNK_LAYER_MASK_INFO:
        {
            LAYER_MASK_INFO *pt = (LAYER_MASK_INFO *)buffer;

            FILEREAD(&pt->lLength,4,1,f);
            change_u4_turn((ULONG *)&pt->lLength);
            offset += 4;

            FILEREAD(&pt->iColorSpace,2,1,f);
            change_u2_turn((UHINT *)&pt->iColorSpace);
            offset += 2;

            FILEREAD(&pt->lColorComponent1,4,1,f);
            change_u4_turn((ULONG *)&pt->lColorComponent1);
            offset += 4;

            FILEREAD(&pt->lColorComponent2,4,1,f);
            change_u4_turn((ULONG *)&pt->lColorComponent2);
            offset += 4;

            FILEREAD(&pt->iOpacity,2,1,f);
            change_u2_turn((UHINT *)&pt->iOpacity);
            offset += 2;

            FILEREAD(&pt->bKind,1,1,f);
            offset += 1;

            FILEREAD(&pt->bFiller,1,1,f);
            offset += 1;
        }
        break;
    default:
        break;
    }
    return (offset);
}


static  void    decode_channel(USTR *pbDes,USTR *pbScr,SLONG xl,SLONG yl)
{
    USTR  bCount,*pbD,*pbS;
    SLONG x,y,lOffsetD,lOffsetS,i;
    SHINT iCompression;// two byte compress flag
    UHINT *piLength=NULL;

    iCompression=*(SHINT *)pbScr;
    change_u2_turn((UHINT *)&iCompression);
    pbScr+=2;
    
    pbD=pbDes;
    pbS=pbScr;
    switch(iCompression)
    {
    case 0x00:  // raw data
        memcpy(pbD,pbS,xl*yl);
        break;
    case 0x01:
        {
            piLength=(UHINT *)malloc(sizeof( UHINT)*yl);
            for(i=0;i<yl;i++)
            {
                piLength[i]=*(SHINT *)pbS;
                change_u2_turn((UHINT *)&piLength[i]);
                pbS+=2;
            }
            
            x=0;
            y=0;
            while(y<yl)
            {
                x=lOffsetD=lOffsetS=0;
                while(x<piLength[y])
                {
                    
                    bCount=pbS[lOffsetS];
                    lOffsetS++;
                    x++;
                    if(bCount<0x80)
                    {
                        bCount++;
                        memcpy(&pbD[lOffsetD],&pbS[lOffsetS],bCount);
                        lOffsetD+=bCount;
                        lOffsetS+=bCount;
                        x+=bCount;
                    }
                    else if(bCount>0x80)
                    {
                        bCount=(unsigned char)(0XFF-bCount+2);
                        memset(&pbD[lOffsetD],pbS[lOffsetS],bCount);
                        lOffsetD+=bCount;
                        lOffsetS++;
                        x++;
                    }
                    else
                    {
                        continue;
                    }
                }
                pbD+=xl;
                pbS+=piLength[y];
                y++;
            }
        }
        break;
    default:
        {
            //sprintf((char *)print_rec,"unknown psd compress format");
            //log_error(1,print_rec);
        }
        break;
    }
    if(piLength) { free(piLength); piLength = NULL; }
    return;
}

static  void    decode_image_data(FILEPT* f,USTR *pbChannel,UHINT *piLength,SLONG xl,SLONG yl)
{
    USTR bCount,bChar;
    SLONG lSize=0;
    SLONG i;
    SLONG lOffset=0;
    SLONG x,y; 
    USTR *pbMemory = NULL;
    USTR *pbPoint = NULL, *pbBackPoint = NULL;
    USTR *pbS = NULL;
    
    for(i=0;i<yl;i++)
        lSize+=piLength[i];
    pbMemory=(USTR *)malloc(lSize);
    FILEREAD(pbMemory,1,lSize,f);
    
    //Jack, 2002.3.28.
    /*
    pbS = pbChannel;
    x = 0;
    y = 0;
    lCount = 0;
    lSize = 0;
    while(y<yl)
    {
        x=0;
        lOffset=0;
        pbS=pbChannel;
        pbS+=y*xl;
        pbPoint=pbMemory;
        while( (x<piLength[y])&&(lOffset<xl) )
        {
            bCount=pbMemory[0];
            pbMemory++;

            x++;
            if(bCount<0x80)
            {
                bCount++;
                for(SLONG i=0;i<bCount;i++)
                {
                    bChar=pbMemory[0];
                    pbMemory++;
                    
                    pbS[0]=bChar;
                    pbS++;
                    x++;
                    lOffset++;
                    
                    lCount++;
                    lSize++;
                }
                
            }
            else if(bCount>0x80)
            {
                bCount=(unsigned char)(0XFF-bCount+2);

                bChar=pbMemory[0];
                pbMemory++;
                
                memset(pbS,bChar,bCount);
                pbS+=bCount;
                x++;
                lOffset+=bCount;
                
                lCount++;
                lSize+=bCount;
            }
            else
            {
                continue;
            }
        }
        pbMemory=pbPoint;
        pbMemory+=piLength[y];

        y++;
    }
    */

    pbS = pbChannel;
    pbPoint = pbMemory;
    x = 0;
    y = 0;
    lSize = 0;
    while(y<yl)
    {
        x = 0;
        lOffset = 0;
        pbS = pbChannel + y*xl;
        pbBackPoint = pbPoint;
        while( (x<piLength[y])&&(lOffset<xl) )
        {
            bCount=pbPoint[0];
            pbPoint++;

            x++;
            if(bCount<0x80)
            {
                bCount++;
                for(SLONG i=0;i<bCount;i++)
                {
                    bChar=pbPoint[0];
                    pbPoint++;
                    
                    pbS[0]=bChar;
                    pbS++;
                    x++;
                    lOffset++;
                    
                    lSize++;
                }
            }
            else if(bCount>0x80)
            {
                bCount=(unsigned char)(0XFF-bCount+2);

                bChar=pbPoint[0];
                pbPoint++;
                
                memset(pbS,bChar,bCount);
                pbS+=bCount;
                x++;
                lOffset+=bCount;
                
                lSize+=bCount;
            }
            else
            {
                continue;
            }
        }
        pbPoint = pbBackPoint + piLength[y];

        y++;
    }

    //Jack, 2002.3.28.
    //found memory leak of pbMemory.
    if(pbMemory)
    {
        free(pbMemory);
        pbMemory = NULL;
    }

    return;
}



// F U N C T I O N S ////////////////////////////////////////////////////////////////////////////////////////////////
EXPORT  PSDFILE *   FNBACK  open_psd_file(char *filename,SLONG *layers,SLONG *xl,SLONG *yl)
{
    PSDFILE *f=NULL;
    COLOR_MODE_SECTION tColorMode;
    IMAGE_RESOURCE_SECTION tImageResource; 
    LAYER_MASK_INFO_SECTION tLayer;
    LAYER_INFO tLayerInfo;
    LAYER_STRUCT tLayerStruct;
    SLONG current_layer_offset;
    SLONG i,j;

    if(NULL == (f=(PSDFILE *)malloc(sizeof(PSDFILE))))
    {
        goto error;
    }
    f->fp = NULL;
    f->offset = 0;
    f->layer_record=NULL;
    f->layers=0;
    f->graph_offset=0;

    if(NULL == (f->fp = FILEOPEN((const char *)filename,(const char *)"rb")) )
    {
        //sprintf((char *)print_rec,"open file %s error",filename);
        //log_error(1,print_rec);
        goto error;
    }

    read_psd_chunk((void *)&f->psd_head,CHUNK_PSD_HEAD,f->fp);
    if(! ((f->psd_head.abKeyWork[0]=='8') && (f->psd_head.abKeyWork[1]=='B') 
        && (f->psd_head.abKeyWork[2]=='P') && (f->psd_head.abKeyWork[3]=='S')) )
    {
        //sprintf((char *)print_rec,"psd file format error");
        //log_error(1,print_rec);
        goto error;
    }

    read_psd_chunk((void *)&tColorMode,CHUNK_COLOR_MODE_SECTION,f->fp);
    FILESEEK(f->fp,tColorMode.lLength,SEEK_CUR);
    
    read_psd_chunk((void *)&tImageResource,CHUNK_IMAGE_RESOURCE_SECTION,f->fp);
    FILESEEK(f->fp,tImageResource.lLength,SEEK_CUR);
    
    read_psd_chunk((void *)&tLayer,CHUNK_LAYER_MASK_INFO_SECTION,f->fp);
    {
        read_psd_chunk((void *)&tLayerInfo,CHUNK_LAYER_INFO,f->fp);
        {
            read_psd_chunk((void *)&tLayerStruct,CHUNK_LAYER_STRUCT,f->fp);
            {
                f->layers = abs(tLayerStruct.iCount);
                f->layer_record = (LAYER_RECORDS *)malloc(sizeof(LAYER_RECORDS)*f->layers);
                for(i=0; i<f->layers; i++)
                    f->layer_record[i].ptChannelInfo = NULL;
                for(i=0; i<f->layers; i++)
                    read_psd_chunk((void*)&f->layer_record[i],CHUNK_LAYER_RECORDS,f->fp);

                current_layer_offset = FILETELL(f->fp);
                f->layer_offset = (SLONG *)malloc(sizeof(SLONG)*f->layers);
                for(i=0; i<f->layers; i++)
                {
                    f->layer_offset[i] = current_layer_offset;
                    for(j=0; j<f->layer_record[i].iChannelNum; j++)
                    {
                        //FILESEEK(f->fp,f->layer_record[j].ptChannelInfo[i].lLength,SEEK_CUR);
                        current_layer_offset += f->layer_record[i].ptChannelInfo[j].lLength;
                    }
                }
            }
        }
        //LAYER_MASK_INFO tLayermaskInfo;
        //read_psd_chunk((void *)&tLayermaskInfo,CHUNK_LAYER_MASK_INFO,f->fp);
    }

    f->graph_offset = sizeof(PSD_HEAD)
        + sizeof(COLOR_MODE_SECTION) + tColorMode.lLength
        + sizeof(IMAGE_RESOURCE_SECTION) + tImageResource.lLength
        + sizeof(LAYER_MASK_INFO_SECTION) + tLayer.lLength;

    *layers = f->layers;
    *xl = f->psd_head.lColumns;
    *yl = f->psd_head.lRows;
    return(f);

error:
    if(f)
    {
        if(f->fp) FILECLOSE(f->fp);
        free(f);
        f = NULL;
    }
    return NULL;
}


EXPORT  SLONG       FNBACK  read_psd_layer_info(SLONG layer,SLONG *sx,SLONG *sy,SLONG *xl,SLONG *yl,PSDFILE *f)
{
    if(layer < 0) return(-1);
    if(layer >= f->layers) return(-2);
    *sx = f->layer_record[layer].lLeft;
    *sy = f->layer_record[layer].lTop;
    *xl = f->layer_record[layer].lRight - f->layer_record[layer].lLeft;
    *yl = f->layer_record[layer].lBottom - f->layer_record[layer].lTop;
    return 0;
}


EXPORT  SLONG       FNBACK  read_psd_layer_image(SLONG layer,USTR *rgb_buffer,USTR *alpha_buffer,PSDFILE *f)
{
    SLONG offset;
    SLONG i;
    USTR  *pbTempmemory=NULL;
    SLONG w,h;
    SLONG x,y;
    USTR  r,g,b,t;

    if(layer < 0) return(-1);
    if(layer >= f->layers) return(-2);

    //(1) read layer channels data from file ------------------------------------------------------------------------
    offset = f->layer_offset[layer];
    FILESEEK(f->fp,offset,SEEK_SET);

    w = f->layer_record[layer].lRight - f->layer_record[layer].lLeft; 
    h = f->layer_record[layer].lBottom - f->layer_record[layer].lTop;
    for(i=0; i<f->layer_record[layer].iChannelNum; i++)
    {
        switch(f->layer_record[layer].ptChannelInfo[i].iChannelId)
        {
        case R_CHANNEL:
        case G_CHANNEL:
        case B_CHANNEL:
        case TRANSPARENCY:
            if(pbTempmemory!=NULL)
            {
                free(pbTempmemory);
                pbTempmemory=NULL;
            }
            pbTempmemory=(USTR *)malloc(f->layer_record[layer].ptChannelInfo[i].lLength);
            if(pbTempmemory==NULL)
                goto error;
            FILEREAD(pbTempmemory,1,f->layer_record[layer].ptChannelInfo[i].lLength,f->fp);
            if(f->layer_record[layer].ptChannelInfo[i].pbChannel)
            {
                free(f->layer_record[layer].ptChannelInfo[i].pbChannel);
                f->layer_record[layer].ptChannelInfo[i].pbChannel=NULL;
            }
            f->layer_record[layer].ptChannelInfo[i].pbChannel=(USTR *)malloc(w*h);
            decode_channel(f->layer_record[layer].ptChannelInfo[i].pbChannel,pbTempmemory,w,h);
            break;
        default:
            // skip unknow channel data
            FILESEEK(f->fp,f->layer_record[layer].ptChannelInfo[i].lLength,SEEK_CUR);
            break;
        }
    }
    if(pbTempmemory)
    {
        free(pbTempmemory);
        pbTempmemory = NULL;
    }

    //(2) merge channels for our use -----------------------------------------------------------------------
    r=g=b=t=0;
    offset=0;
    for(y=0; y<h; y++)
    {
        for(x=0; x<w; x++)
        {
            t=0xff;
            for(i=0; i<f->layer_record[layer].iChannelNum; i++)  
            {
                switch(f->layer_record[layer].ptChannelInfo[i].iChannelId)
                {
                case R_CHANNEL:
                    r = f->layer_record[layer].ptChannelInfo[i].pbChannel[offset];
                    break;
                case G_CHANNEL:
                    g = f->layer_record[layer].ptChannelInfo[i].pbChannel[offset];  
                    break;
                case B_CHANNEL:
                    b = f->layer_record[layer].ptChannelInfo[i].pbChannel[offset];
                    break;
                case TRANSPARENCY:
                    t = f->layer_record[layer].ptChannelInfo[i].pbChannel[offset];
                    break;
                }   
            }
            if(NULL == alpha_buffer)
            {
                if(t>=psd_trans_level)
                {
                    if((r<8)&&(g<8)&&(b<8))
                    {
                        b=8; 
                    }
                }              
                else  
                {
                    r=g=b=0;
                }

                rgb_buffer[offset*3 + 0] = r;
                rgb_buffer[offset*3 + 1] = g;
                rgb_buffer[offset*3 + 2] = b;
                offset++;
            }
            else
            {
                /*
                if(t)
                {
                    if((r<8)&&(g<8)&&(b<8))
                    {
                        b=8; 
                    }
                }              
                else  
                {
                    r=g=b=0;
                }
                */
                //ZJian,2000.12.15.
                //32 is a 8-grade alpha limit value
                if(t<32)
                {
                    r=g=b=t=0;
                }
                else if(t>=255-32)
                {
                    if((r<8)&&(g<8)&&(b<8))
                    {
                        r=g=0;
                        b=8;
                    }
                    t=255;
                }

                rgb_buffer[offset*3 + 0] = r;
                rgb_buffer[offset*3 + 1] = g;
                rgb_buffer[offset*3 + 2] = b;
                alpha_buffer[offset]=t;
                offset++;
            }
        }
    }

    //(3) free layer channels data memory ------------------------------------------------------------------------------
    for(i=0; i<f->layer_record[layer].iChannelNum; i++)
    {
        if(f->layer_record[layer].ptChannelInfo[i].pbChannel)
        {
            free(f->layer_record[layer].ptChannelInfo[i].pbChannel);
            f->layer_record[layer].ptChannelInfo[i].pbChannel = NULL;
        }
    }

    // finisehd
    return 0;
error:
    if(pbTempmemory) 
    { 
        free(pbTempmemory);
        pbTempmemory = NULL;
    }
    return -3;
}


EXPORT  SLONG   FNBACK  read_psd_graph_image(USTR *rgb_buffer, PSDFILE *f)
{
    SLONG offset;
    SLONG i,j,xl,yl;
    SLONG channels;
    UCHR r,g,b,t;
    USTR **image_channel=NULL;
    UHINT *length=NULL;
    UHINT compress_flag;


    //(1) read datas from file ----------------------------------------------------------------
    offset = f->graph_offset;
    FILESEEK(f->fp, offset, SEEK_SET);

    yl=f->psd_head.lRows;
    xl=f->psd_head.lColumns;
    channels=f->psd_head.iChannels;

    image_channel = (USTR **)malloc(sizeof(USTR *)*channels);
    for(i=0;i<channels;i++)
    {
        if(NULL == (image_channel[i] = (USTR*)malloc(xl*yl)))
        {
            //sprintf((char *)print_rec,"memory alloc error");
            //log_error(1,print_rec);
            goto error;
        }
    }
    
    FILEREAD(&compress_flag,2,1,f->fp);
    change_u2_turn((UHINT *)&compress_flag);
    if(compress_flag)
    {
        length = (UHINT *)malloc(sizeof(UHINT)*yl*channels);
        for(i=0;i<yl*channels;i++)
        {
            FILEREAD(&length[i],1,2,f->fp);
            change_u2_turn((UHINT *)&length[i]);
        }
        for(i=0;i<channels;i++)
        {
            decode_image_data(f->fp,(USTR *)image_channel[i],&length[yl*i],xl,yl);
        }
    }
    else
    {
        for(i=0;i<channels;i++)
        {
            FILEREAD(image_channel[i],1,xl*yl,f->fp);
        }
    }          

    offset=0;
    // we assert it's have 4 channels, r,g,b,t.
    if(channels>=4)
    {
        for(i=0;i<xl;i++)  
        {
            for(j=0;j<yl;j++)  
            {
                r = image_channel[0][offset];
                g = image_channel[1][offset];
                b = image_channel[2][offset];
                t = image_channel[3][offset];
                if(t>=psd_trans_level)
                {
                    if((r<8)&&(g<8)&&(b<8))
                    {
                        b=8; 
                    }
                }              
                else  
                {
                    r=g=b=0;
                }

                rgb_buffer[offset*3 + 0] = r;
                rgb_buffer[offset*3 + 1] = g;
                rgb_buffer[offset*3 + 2] = b;
                offset++;
            }  
        }
    }

    if(image_channel)
    {
        for(i=0;i<channels;i++)
        {
            if(image_channel[i]) free(image_channel[i]);
        }
        free(image_channel);
        image_channel=NULL;
    }
    if(length)
    {
        free(length);
        length=NULL;
    }
    return 0;
error:
    if(image_channel)
    {
        for(i=0;i<channels;i++)
        {
            if(image_channel[i]) free(image_channel[i]);
        }
        free(image_channel);
        image_channel=NULL;
    }
    if(length)
    {
        free(length);
        length=NULL;
    }
    return -1;
}


EXPORT  void        FNBACK  close_psd_file(PSDFILE *f)
{
    SLONG i,j;
    if(f)
    {
        if(f->layer_record)
        {
            for(i=0;i<f->layers;i++)
            {
                if(f->layer_record[i].ptChannelInfo)
                {
                    for(j=0; j<f->layer_record[i].iChannelNum;j++)
                    {
                        if(f->layer_record[i].ptChannelInfo[j].pbChannel)
                        {
                            free(f->layer_record[i].ptChannelInfo[j].pbChannel);
                            f->layer_record[i].ptChannelInfo[j].pbChannel=NULL;
                        } 
                    }
                    free(f->layer_record[i].ptChannelInfo);
                }
            }
            free(f->layer_record);
            f->layer_record=NULL;
        }
        if(f->layer_offset)
        {
            free(f->layer_offset);
            f->layer_offset=NULL;
        }
        if(f->fp) 
        { 
            FILECLOSE(f->fp); 
            f->fp=NULL; 
        }
        free(f);
    }
}



EXPORT  void    FNBACK  set_psd_transparency_level(SLONG level)
{
    if(level>=0 && level<255)
        psd_trans_level = (UCHR)level;
    else
        psd_trans_level = (UCHR)128;
}

