//#if	0
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

EXPORT  SLONG   FNBACK  BMP_save_file(UHINT *buffer,SLONG xl,SLONG yl,char *filename)
{
    FILEPT *fp=NULL;
    BITMAPFILEHEADER bithead;
    BITMAPINFOHEADER bitinfo;
    int headsize,pitch,i,j,s;
    unsigned long color;
    unsigned long offset;
    
    if(!buffer) return TTN_NOT_OK;
    
    pitch=(xl*3+3)&0xfffffffc;
    s=pitch-xl*3;
    if((fp=FILEOPEN(filename,"wb"))==NULL) 
    {
        sprintf((char *)print_rec,"Can't open bmp file %s",filename);
        log_error(1,print_rec);
        return -1;
    }
    headsize=sizeof(bithead)+sizeof(bitinfo);
    bithead.bfType='MB';
    bithead.bfSize=headsize+pitch*yl;
    bithead.bfReserved1=bithead.bfReserved2=0;
    bithead.bfOffBits=headsize;
    FILEWRITE(&bithead,1,sizeof(bithead),fp);
    bitinfo.biSize=sizeof(bitinfo);
    bitinfo.biWidth=xl;
    bitinfo.biHeight=yl;
    bitinfo.biPlanes=1;
    bitinfo.biBitCount=24;
    bitinfo.biCompression=BI_RGB;
    bitinfo.biSizeImage=0;
    bitinfo.biXPelsPerMeter=72;
    bitinfo.biYPelsPerMeter=72;
    bitinfo.biClrUsed=0;
    bitinfo.biClrImportant=0;
    FILEWRITE(&bitinfo,1,sizeof(bitinfo),fp);
    for(i=yl-1;i>=0;i--)
    {
        for(j=0;j<xl;j++)
        {
            offset = i * xl + j;
            color=hi2true(buffer[offset]);
            FILEWRITE(&color,1,3,fp);
        }
        FILEWRITE(&color,1,s,fp);
    }
    if(fp) FILECLOSE(fp);
    return 0;
}


static  UCHR    tga_trans_level = 128;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

EXPORT  void    FNBACK  TGA_set_transparency_level(SLONG level)
{
    if(level>=0 && level<255)
        tga_trans_level = (UCHR)level;
    else
        tga_trans_level = (UCHR)128;
}



//==================================================================
//EXPORT  BMP*    FNBACK  TGA_load_file(char *filename);
//EXPORT  ABMP*   FNBACK  TGA_load_file_with_alpha(char *filename);
EXPORT ABMP* FNBACK TGA_load_file_with_alpha(char *filename)
{
    TGA_HEAD tga_head;
    FILEPT *f=NULL;
    ABMP *abmp=NULL;
    UCHR r=0,g=0,b=0,a=0xff,t[6]={0};
    UHINT x,y,cc;
    UCHR count,flag;
    SLONG i,offset,size;
    PIXEL color;
    
    if(NULL==(f=FILEOPEN(filename,"rb")))
    {
        sprintf((char *)print_rec,"Can't open tga file %s",filename);
        log_error(1,print_rec);
        goto error;
    }
    // read file head
    FILEREAD(&tga_head,1,sizeof(TGA_HEAD),f);
    if(NULL==(abmp=create_abitmap(tga_head.iWidth,tga_head.iHeight)))
    {
        sprintf((char *)print_rec,"%s","memory alloc error");
        log_error(1,print_rec);
        goto error;
    }
    // skip to file data 
    FILESEEK(f,tga_head.bIdSize,SEEK_CUR);
    FILESEEK(f,tga_head.iColorMapLength,SEEK_CUR);
    switch(tga_head.bImageType)
    {
    case TGA_UNCOMPRESS:
        {
            switch(tga_head.bBitsPerPixel)
            {
            case 32: //AAAAAAAARRRRRRRRGGGGGGGGBBBBBBBB
                for(y=0;y<tga_head.iHeight;y++)
                {
                    for(x=0;x<tga_head.iWidth;x++)
                    {
                        FILEREAD(&b,1,1,f);
                        FILEREAD(&g,1,1,f);
                        FILEREAD(&r,1,1,f);
                        FILEREAD(&a,1,1,f);
                        *((PIXEL *)abmp->line[abmp->h-1-y]+x)=true2hi((r<<16)|(g<<8)|b);
                        abmp->line[abmp->h-1-y][abmp->w*SIZEOFPIXEL+x]=a;
                    }
                }
                break;
            case 24: //RRRRRRRRGGGGGGGGBBBBBBBB
                for(y=0;y<tga_head.iHeight;y++)
                {
                    for(x=0;x<tga_head.iWidth;x++)
                    {
                        FILEREAD(&b,1,1,f);
                        FILEREAD(&g,1,1,f);
                        FILEREAD(&r,1,1,f);
                        *((PIXEL *)abmp->line[abmp->h-1-y]+x)=true2hi((r<<16)|(g<<8)|b);
                        abmp->line[abmp->h-1-y][abmp->w*SIZEOFPIXEL+x]=a;
                    }
                }
                break;
            case 15:
            case 16://ARRRRRGG GGGBBBBB
                for(y=0;y<tga_head.iHeight;y++)
                {
                    for(x=0;x<tga_head.iWidth;x++)
                    {
                        FILEREAD(&t[0],1,1,f);
                        FILEREAD(&t[1],1,1,f);
                        cc=*(UHINT *)&t[0];
                        r=TGA16_R(cc);
                        g=TGA16_G(cc);
                        b=TGA16_B(cc);
                        *((PIXEL *)abmp->line[abmp->h-1-y]+x)=true2hi((r<<16)|(g<<8)|b);
                        abmp->line[abmp->h-1-y][abmp->w*sizeof(PIXEL)+x]=a;
                    }
                }
                break;
            default:
                sprintf((char *)print_rec,"%s","tga image type not support");
                log_error(1,print_rec);
                goto error;
                break;
            }
        }
        break;
    case TGA_RLE_COMPRESS:
        {
            offset=0;
            size=tga_head.iHeight*tga_head.iWidth;
            while(offset<size)
            {
                FILEREAD(&count,1,1,f);
                flag = (UCHR)(count & 0x80);
                if(0==flag)
                {
                    flag = (UCHR)(count+1);
                    for(i=0;i<flag;i++)
                    {
                        switch(tga_head.bBitsPerPixel)
                        {
                        case 32:
                            FILEREAD(&b,1,1,f);
                            FILEREAD(&g,1,1,f);
                            FILEREAD(&r,1,1,f);
                            FILEREAD(&a,1,1,f);
                            break;
                        case 24:
                            FILEREAD(&b,1,1,f);
                            FILEREAD(&g,1,1,f);
                            FILEREAD(&r,1,1,f);
                            a=255;
                            break;
                        case 16:
                        case 15:
                            FILEREAD(&t[0],1,1,f);
                            FILEREAD(&t[1],1,1,f);
                            cc=*(UHINT *)&t[0];
                            r=TGA16_R(cc);
                            g=TGA16_G(cc);
                            b=TGA16_B(cc);
                            a=255;
                            break;
                            
                        }
                        y=(UHINT)(offset/tga_head.iWidth);
                        x=(UHINT)(offset-y*tga_head.iWidth);
                        *((PIXEL *)abmp->line[abmp->h-1-y]+x)=true2hi((r<<16)|(g<<8)|b);
                        abmp->line[abmp->h-1-y][abmp->w*SIZEOFPIXEL+x]=a;

                        if(offset>=size)
                            continue;
                        offset++;
                    }
                }
                else
                {
                    flag=(UCHR)(count-127);
                    switch(tga_head.bBitsPerPixel)
                    {
                    case 32:
                        FILEREAD(&b,1,1,f);
                        FILEREAD(&g,1,1,f);
                        FILEREAD(&r,1,1,f);
                        FILEREAD(&a,1,1,f);
                        break;
                    case 24:
                        FILEREAD(&b,1,1,f);
                        FILEREAD(&g,1,1,f);
                        FILEREAD(&r,1,1,f);
                        a=255;
                        break;
                    case 16:
                    case 15:
                        FILEREAD(&t[0],1,1,f);
                        FILEREAD(&t[1],1,1,f);
                        cc=*(UHINT *)&t[0];
                        r=TGA16_R(cc);
                        g=TGA16_G(cc);
                        b=TGA16_B(cc);
                        a=255;
                        break;
                        
                    }
                    color=true2hi((r<<16)|(g<<8)|b);
                    for(i=0;i<flag;i++)
                    {
                        if(offset>=size)
                            continue;
                        y=(UHINT)(offset/tga_head.iWidth);
                        x=(UHINT)(offset-y*tga_head.iWidth);
                        *((PIXEL *)abmp->line[abmp->h-1-y]+x)=true2hi((r<<16)|(g<<8)|b);
                        abmp->line[abmp->h-1-y][abmp->w*sizeof(PIXEL)+x]=a;
                        offset++;
                    }
                }
            }
        }
        break;
    default:
        sprintf((char *)print_rec,"%s","tga image type not support");
        log_error(1,print_rec);
        goto error;
        break;
    }
    if(f) FILECLOSE(f);

    if(tga_head.bDescriptor  & 0x20)
    {
        rotate_abitmap_top_bottom(abmp);
    }
    if(tga_head.bDescriptor  & 0x10)
    {
        rotate_abitmap_left_right(abmp);
    }

    return(abmp);
error:
    if(f) FILECLOSE(f);
    if(abmp) destroy_abitmap(&abmp);
    return(NULL);
}



EXPORT BMP* FNBACK TGA_load_file(char *filename)
{
    TGA_HEAD tga_head;
    FILEPT *f=NULL;
    BMP *bmp=NULL;
    PIXEL color;
    UCHR r=0,g=0,b=0,a=0xff,t[6]={0};
    UHINT x,y,cc;
    UCHR count,flag;
    SLONG offset,size,i;
    
    if(NULL==(f=FILEOPEN(filename,"rb")))
    {
        sprintf((char *)print_rec,"Can't open tga file %s",filename);
        log_error(1,print_rec);
        goto error;
    }
    // read file head
    FILEREAD(&tga_head,1,sizeof(TGA_HEAD),f);
    if(NULL==(bmp=create_bitmap(tga_head.iWidth,tga_head.iHeight)))
    {
        sprintf((char *)print_rec,"%s","memory alloc error");
        log_error(1,print_rec);
        goto error;
    }
    // skip to file data 
    FILESEEK(f,tga_head.bIdSize,SEEK_CUR);
    FILESEEK(f,tga_head.iColorMapLength,SEEK_CUR);
    switch(tga_head.bImageType)
    {
    case TGA_UNCOMPRESS:
        {
            switch(tga_head.bBitsPerPixel)
            {
            case 32: //AAAAAAAARRRRRRRRGGGGGGGGBBBBBBBB
                for(y=0;y<tga_head.iHeight;y++)
                {
                    for(x=0;x<tga_head.iWidth;x++)
                    {
                        FILEREAD(&b,1,1,f);
                        FILEREAD(&g,1,1,f);
                        FILEREAD(&r,1,1,f);
                        FILEREAD(&a,1,1,f);
                        if(a>=tga_trans_level)
                        {
                            if(r<8 && g<8 && b<8)
                                b=8;
                        }
                        else
                        {
                            r=g=b=0;
                        }
                        *((PIXEL *)bmp->line[bmp->h-1-y]+x)=true2hi((r<<16)|(g<<8)|b);
                    }
                }
                break;
            case 24: //RRRRRRRRGGGGGGGGBBBBBBBB
                for(y=0;y<tga_head.iHeight;y++)
                {
                    for(x=0;x<tga_head.iWidth;x++)
                    {
                        FILEREAD(&b,1,1,f);
                        FILEREAD(&g,1,1,f);
                        FILEREAD(&r,1,1,f);
                        a=255;
                        if(a>=tga_trans_level)
                        {
                            if(r<8 && g<8 && b<8)
                                b=8;
                        }
                        else
                        {
                            r=g=b=0;
                        }
                        *((PIXEL *)bmp->line[bmp->h-1-y]+x)=true2hi((r<<16)|(g<<8)|b);
                    }
                }
                break;
            case 15:
            case 16://ARRRRRGG GGGBBBBB
                for(y=0;y<tga_head.iHeight;y++)
                {
                    for(x=0;x<tga_head.iWidth;x++)
                    {
                        FILEREAD(&t[0],1,1,f);
                        FILEREAD(&t[1],1,1,f);
                        cc=*(UHINT *)&t[0];
                        r=TGA16_R(cc);
                        g=TGA16_G(cc);
                        b=TGA16_B(cc);
                        a=255;
                        if(a>=tga_trans_level)
                        {
                            if(r<8 && g<8 && b<8)
                                b=8;
                        }
                        else
                        {
                            r=g=b=0;
                        }
                        *((PIXEL *)bmp->line[bmp->h-1-y]+x)=true2hi((r<<16)|(g<<8)|b);
                    }
                }
                break;
            default:
                sprintf((char *)print_rec,"%s","tga image type not support");
                log_error(1,print_rec);
                goto error;
                break;
            }
        }
        break;
    case TGA_RLE_COMPRESS:
        {
            offset=0;
            size=tga_head.iHeight*tga_head.iWidth;
            while(offset<size)
            {
                FILEREAD(&count,1,1,f);
                flag = (UCHR)(count & 0x80);
                if(0==flag)
                {
                    flag = (UCHR)(count+1);
                    for(i=0;i<flag;i++)
                    {
                        switch(tga_head.bBitsPerPixel)
                        {
                        case 32:
                            FILEREAD(&b,1,1,f);
                            FILEREAD(&g,1,1,f);
                            FILEREAD(&r,1,1,f);
                            FILEREAD(&a,1,1,f);
                            break;
                        case 24:
                            FILEREAD(&b,1,1,f);
                            FILEREAD(&g,1,1,f);
                            FILEREAD(&r,1,1,f);
                            a=255;
                            break;
                        case 16:
                        case 15:
                            FILEREAD(&t[0],1,1,f);
                            FILEREAD(&t[1],1,1,f);
                            cc=*(UHINT *)&t[0];
                            r=TGA16_R(cc);
                            g=TGA16_G(cc);
                            b=TGA16_B(cc);
                            a=255;
                            break;
                            
                        }
                        if(a>=tga_trans_level)
                        {
                            if(r<8 && g<8 && b<8)
                                b=8;
                        }
                        else
                        {
                            r=g=b=0;
                        }
                        y=(UHINT)(offset/tga_head.iWidth);
                        x=(UHINT)(offset-y*tga_head.iWidth);
                        *((PIXEL *)bmp->line[bmp->h-1-y]+x)=true2hi((r<<16)|(g<<8)|b);
                        if(offset>=size)
                            continue;
                        offset++;
                    }
                }
                else
                {
                    flag=(UCHR)(count-127);
                    switch(tga_head.bBitsPerPixel)
                    {
                    case 32:
                        FILEREAD(&b,1,1,f);
                        FILEREAD(&g,1,1,f);
                        FILEREAD(&r,1,1,f);
                        FILEREAD(&a,1,1,f);
                        break;
                    case 24:
                        FILEREAD(&b,1,1,f);
                        FILEREAD(&g,1,1,f);
                        FILEREAD(&r,1,1,f);
                        a=255;
                        break;
                    case 16:
                    case 15:
                        FILEREAD(&t[0],1,1,f);
                        FILEREAD(&t[1],1,1,f);
                        cc=*(UHINT *)&t[0];
                        r=TGA16_R(cc);
                        g=TGA16_G(cc);
                        b=TGA16_B(cc);
                        a=255;
                        break;
                        
                    }
                    if(a>=tga_trans_level)
                    {
                        if(r<8 && g<8 && b<8)
                            b=8;
                    }
                    else
                    {
                        r=g=b=0;
                    }
                    color=true2hi((r<<16)|(g<<8)|b);
                    for(i=0;i<flag;i++)
                    {
                        if(offset>=size)
                            continue;
                        y=(UHINT)(offset/tga_head.iWidth);
                        x=(UHINT)(offset-y*tga_head.iWidth);
                        *((PIXEL *)bmp->line[bmp->h-1-y]+x)=true2hi((r<<16)|(g<<8)|b);
                        offset++;
                    }
                }
            }
        }
        break;
    default:
        sprintf((char *)print_rec,"%s","tga image type not support");
        log_error(1,print_rec);
        goto error;
        break;
    }
    if(f) FILECLOSE(f);
    if(tga_head.bDescriptor & 0x20)
    {
        rotate_bitmap_top_bottom(bmp);
    }
    if(tga_head.bDescriptor & 0x10)
    {
        rotate_bitmap_left_right(bmp);
    }
    return(bmp);
error:
    if(f) FILECLOSE(f);
    destroy_bitmap(&bmp);
    return(NULL);
}



//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//#endif//0
