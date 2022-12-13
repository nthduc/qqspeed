/*
**  gif89a.cpp
**  gif89a functions.
**
**  Jim(LiuZiJian), 2003.6.26.
**      Add in library "xImage".
**      Special thanks to cproom.
**      [cproom]
**      +    e-mail:cproom@163.net
**      +    http://cproom.yeah.net
**      +    ͨѶ��ַ������ʡ������̫����·202��(��ɽ΢���ӹ�˾)�������գ�
**      +    �ʱࡡ����710065
**      +    �绰������(029)-8234582
**  
**
**    
*/
//#include <fstream.h>
#include <string.h>
#include "gif89a.h"

// ��  �ܣ����캯��
// ��  ������
// ����ֵ����
// ˵  �������ڹ���һ���յ�ʵ�����Ա��Ժ���open()�������ļ���
CGif89a::CGif89a()
{	
    opened = FALSE;
	error = FALSE;
}


// ��  �ܣ����캯��
// ��  ����fileName:Ҫ�򿪵��ļ����ַ���
//         inMem:�Ƿ�ͼ������һ�ζ����ڴ�
// ����ֵ����
// ˵  �����ڲ�����open()��������ο�open()������
CGif89a::CGif89a(LPCSTR fileName,BOOL inMem)
{
    if(open(fileName,inMem))
	{	
		opened = TRUE;
		error = FALSE;
	}
	else
	{	
		opened = FALSE;
		error = TRUE;
	}
}

// ��  �ܣ���������
// ��  ������
// ����ֵ����
// ˵  �����ڲ�����close()����������������
CGif89a::~CGif89a()
{	
    close();
}

// ��  �ܣ��жϹ����Ƿ�ɹ�
// ��  ������
// ����ֵ��Ϊ������ɹ���Ϊ����ʧ�ܡ�
// ˵  �����ڹ��캯������֮��һ��Ҫ���ô˺���ȷ�������Ƿ�ɹ���
BOOL CGif89a::operator!()
{	
    return error;
}

// ��  �ܣ����ļ�
// ��  ����fileName:Ҫ�򿪵��ļ����ַ���
//         inMem:�Ƿ�ͼ������һ�ζ����ڴ�
// ����ֵ��Ϊ����ɹ���Ϊ����ʧ��
// ˵  �������inMemΪ�棬������ͼ��֡����һ��ȫ�������ڴ棬Ȼ��ر��ļ����˷�ʽ
//         �����ļ���С����Ҫ��ȡ���ݽϿ��������˷�ʽռ���ڴ�϶ࡣ
//         ���inMemΪ�٣��򱣳��ļ�Ϊ��״̬���Ժ���Ҫʱ���ļ��������ݡ��˷�ʽ��
//         ���ļ��ϴ󣬶Զ�ȡ���ݲ�Ҫ��ܿ��������˷�ʽռ���ڴ���١�
//         ʹ����������ǰ��Ӧ���ȵ��ô˺�����
//         ��������GIF�ļ�������Ҫ���Ϊ�ϸ񣬶��ڴ�Internet�����ص�GIF�ļ������δ
//         ���������أ���򿪽�ʧ�ܡ�
BOOL CGif89a::open(LPCSTR fileName,BOOL b)
{
    char cc[4];
	BYTE be;
	BOOL fileEnd = FALSE;

	inMem = b;
	allFrames = NULL;
	curIndex = 0;
	curFrame.pColorTable = NULL;
	curFrame.dataBuf = NULL;
	ctrlExt.active = FALSE;
	if(opened)
		return FALSE;

	// Jim, changed [10/14/2003]
	//{{{
    //ifs.open(fileName,ios::binary | ios::nocreate);
	//if(!ifs) return FALSE;
	fp = FILEOPEN(fileName, "rb");
	if(NULL == fp)
		return FALSE;
	//}}}


    // [GIF File Header]
    //--------------------------
    // char Signature[3];
    // char Version[3];
    //--------------------------
    //
	//ifs.read(cc,3);
	FILEREAD(cc, 1, 3, fp);

	if(strncmp(cc,"GIF",3) != 0)
		goto error;

	//ifs.read(version,3);
	FILEREAD(version, 1, 3, fp);

	version[3] = 0;
	if(strncmp(version,"89a",3) > 0)
		goto error;

    // [Logical Screen Descriptor]
    //----------------------------
    // unsigned short LogicalScreenWidth;
    // unsigned short LogicalScreenHeight;
    // byte PackedFields;
    // byte BackgroundColorIndex;
    // byte PixelAspectRatio;
    //----------------------------
    //
    // PackedFields:
    //
    // byte GlobalColorTableFlag : 1;
    // byte ColorResolution : 3;
    // byte SortFlag : 1;
    // byte SizeOfGlobalColorTable : 3;     // means 2 ^ (1+n)
    //----------------------------
    //

	//ifs.read((char*)&gInfo.scrWidth,2);
	FILEREAD((char*)&gInfo.scrWidth, 1, 2, fp);

	//ifs.read((char*)&gInfo.scrHeight,2);
	FILEREAD((char*)&gInfo.scrHeight,1, 2, fp);

	//if(!ifs.good())
	//	goto error;
	if(FILEEOF(fp))
		goto error;

	//ifs.read((char*)&be,1);
	FILEREAD((char*)&be, 1, 1, fp);

	if((be&0x80) != 0)
		gInfo.gFlag = TRUE;
	else
		gInfo.gFlag = FALSE;
	gInfo.colorRes = ((be&0x70)>>4)+1;
	if(gInfo.gFlag)
	{	
        if((be&0x08) != 0)
			gInfo.gSort = TRUE;
		else
			gInfo.gSort = FALSE;
		gInfo.gSize = 1;
		gInfo.gSize <<= ((be&0x07)+1);
	}

	//ifs.read((char*)&be,1);
	FILEREAD((char*)&be,1,1,fp);

	gInfo.BKColorIdx = be;

	//ifs.read((char*)&be,1);
	FILEREAD((char*)&be,1,1,fp);

	gInfo.pixelAspectRatio = be;

	//if(!ifs.good())
	//	goto error;
	if(FILEEOF(fp))
		goto error;

    //
    // [Global Color Table]
    // 
	if(gInfo.gFlag)
	{	
        //ifs.read((char*)gColorTable,gInfo.gSize*3);
        FILEREAD((char*)gColorTable,1,gInfo.gSize*3,fp);

		gInfo.gColorTable = gColorTable;
	}
	else
    {
		gInfo.gColorTable = NULL;
    }

	//
	//dataStart = ifs.tellg();
	dataStart = FILETELL(fp);

	//if((gInfo.frames = checkFrames(ifs)) == 0)
	if((gInfo.frames = checkFrames()) == 0)
		goto error;

	if(inMem)
	{	
        if((allFrames = new FRAME[gInfo.frames]) == NULL)
			goto error;
		ZeroMemory(allFrames,sizeof(FRAME)*gInfo.frames);

		//if(!getAllFrames(ifs))
		if(!getAllFrames())
		{	
            delete[] allFrames;
			allFrames = NULL;
			goto error;
		}
		//ifs.close();
		if(fp) { FILECLOSE(fp); fp = NULL; }
	}
	opened = TRUE;
	return TRUE;

error:
	//ifs.close();
	if(fp) { FILECLOSE(fp); fp = NULL; }
	return FALSE;
}



// ����  ������ļ���ͼ��֡����
// ����  ��ifs:���ļ��������á�
// ����ֵ���ļ���ͼ��֡����
UINT CGif89a::checkFrames()
//UINT CGif89a::checkFrames(ifstream& ifs)
{	
    BYTE be;
	BOOL fileEnd = FALSE;
	UINT frames=0;

	//streampos pos = ifs.tellg();
	long pos = FILETELL(fp);

	//while(!ifs.eof() && !fileEnd)
	while(!FILEEOF(fp) && !fileEnd)
	{	
        //ifs.read((char*)&be,1);
        FILEREAD((char*)&be,1,1,fp);

		switch(be)
		{
        case 0x21:
            {
                //ifs.read((char*)&be,1);
                FILEREAD((char*)&be,1,1,fp);

                switch(be)
                {	
                case 0xf9:
                case 0xfe:
                case 0x01:
                case 0xff:
                    //while(!ifs.eof())
                    while(!FILEEOF(fp))
                    {	
						//ifs.read((char*)&be,1);
						FILEREAD((char*)&be,1,1,fp);

						if(be == 0)
							break;

						//ifs.seekg(be,ios::cur);
						FILESEEK(fp, be,SEEK_CUR);
                    }
                    break;
                default:
                    return 0;
                }
                break;
            }

        case 0x2c:
			{
                BYTE bp;
                BOOL lFlag=FALSE;
                UINT lSize=1;
                frames++;

                //ifs.seekg(8,ios::cur);
                FILESEEK(fp, 8, SEEK_CUR);

                //ifs.read((char*)&bp,1);
                FILEREAD((char*)&bp,1,1,fp);

                if((bp&0x80) != 0)
                    lFlag = TRUE;
                lSize <<= ((bp&0x07)+1);
                if(lFlag)
				{
                    //ifs.seekg(lSize*3,ios::cur);
                    FILESEEK(fp, lSize*3, SEEK_CUR);
				}

                //if(!ifs.good())
                  //  return 0;
				if(FILEEOF(fp))
					return 0;

                //ifs.read((char*)&be,1);
                FILEREAD((char*)&be,1,1,fp);

                //while(!ifs.eof())
                while(!FILEEOF(fp))
                {	
                    //ifs.read((char*)&be,1);
                    FILEREAD((char*)&be,1,1,fp);
                    if(be == 0)
                        break;

                    //ifs.seekg(be,ios::cur);
                    FILESEEK(fp, be, SEEK_CUR);
                }
                break;
			}

        case 0x3b:
            fileEnd = TRUE;
            break;

        case 0x00:
            break;

        default:
            return 0;
		}
	}
	//ifs.seekg(pos);
	FILESEEK(fp, pos, SEEK_SET);

	return frames;
}


// ����  ��������ͼ��֡���ݶ����ڴ档
// ����  ��ifs:���ļ��������á�
// ����ֵ�������Ƿ�ɹ���Ϊ��ɹ���Ϊ��ʧ�ܡ�
//BOOL CGif89a::getAllFrames(ifstream& ifs)
BOOL CGif89a::getAllFrames()
{
    BYTE be;
	BOOL fileEnd = FALSE;
	FRAME *pf = allFrames;

	//streampos pos = ifs.tellg();
	long pos = FILETELL(fp);
	int i;

	//while(!ifs.eof() && !fileEnd)
	while(!FILEEOF(fp) && !fileEnd)
	{	
        //ifs.read((char*)&be,1);
        FILEREAD((char*)&be,1,1,fp);

		switch(be)
		{	
        case 0x21:
            {
                //ifs.read((char*)&be,1);
                FILEREAD((char*)&be,1,1,fp);
                switch(be)
                {
                    // 
                    // [Graphic Control Extension]
                    // -----------------------------------------------------------
                    // byte ExtensionIntroducer;    // fixed with 0x21
                    // byte GraphicControlLabel;    // fixed with 0xf9
                    //
                    // byte BlockSize;              // fixed with 4
                    // byte PackedFields;
                    // unsigned short DelayTime;
                    // byte TransparentColorIndex;
                    //
                    // byte BlockTerminator;        // fixed with 0
                    // -----------------------------------------------------------
                    //
                    // <PackedFields>
                    // 
                    // byte Reserved : 3;
                    // byte DisposalMethod : 3;
                    // byte UserInputFlag : 1;
                    // byte TransparentColorFlag : 1;
                    //
                    // -----------------------------------------------------------
                    //
                case 0xf9:
                    //while(!ifs.eof())
                    while(!FILEEOF(fp))
                    {	
                        //ifs.read((char*)&be,1);
                        FILEREAD((char*)&be,1,1,fp);
                        if(be == 0)
                            break;
                        if(be == 4)
                        {
                            ctrlExt.active = TRUE;

                            //ifs.read((char*)&be,1);
                            FILEREAD((char*)&be,1,1,fp);

                            ctrlExt.disposalMethod = (be&0x1c)>>2;
                            if((be&0x02) != 0)
                                ctrlExt.userInputFlag = TRUE;
                            else
                                ctrlExt.userInputFlag = FALSE;
                            if((be&0x01) != 0)
                                ctrlExt.trsFlag = TRUE;
                            else
                                ctrlExt.trsFlag = FALSE;

                            //ifs.read((char*)&ctrlExt.delayTime,2);
                            FILEREAD((char*)&ctrlExt.delayTime,1,2,fp);

                            //ifs.read((char*)&be,1);
                            FILEREAD((char*)&be,1,1,fp);

                            ctrlExt.trsColorIndex = be;
                        }
                        else
                        {
                            //ifs.seekg(be,ios::cur);
                            FILESEEK(fp, be, SEEK_CUR);
                        }
                    }
                    break;

                    // [Comment Extension]
                    // --------------------------------------------------------------
                    // byte ExtensionIntroducer;        // fixed with 0x21
                    // byte CommentLabel;               // fixed with 0xfe
                    //
                    // byte CommentData[1~255];
                    //
                    // byte BlockTerminator;            // fixed with 0
                    // --------------------------------------------------------------
                    //
                case 0xfe:

                    // [Plain Text Extension]
                    // --------------------------------------------------------------
                    // byte ExtensionIntroducer;        // fixed with 0x21
                    // byte PlainTextLabel;             // fixed with 0x01
                    //
                    // byte BlockSize;                  // fixed with 12
                    // unsigned short TextGridLeftPosition;
                    // unsigned short TextGridTopPosition;
                    // unsigned short TextGridWidth;
                    // unsigned short TextGridHeight;
                    // byte CharacterCellWidth;
                    // byte CharacterCellHeight;
                    // byte TextForgroundColorIndex;
                    // byte TextBackgroundColorIndex;
                    //
                    // byte PlainTextData[1~255];
                    //
                    // byte BlockTerminator;            // fixed with 0
                    // --------------------------------------------------------------
                    //
                case 0x01:

                    // [Application Extension]
                    // --------------------------------------------------------------
                    // byte ExtensionIntroducer;        // fixed with 0x21
                    // byte ExtensionLabel;             // fixed with 0xff
                    //
                    // byte BlockSize;                  // fixed with 11
                    // char ApplicationIdentifier[8];
                    // char ApplicationAuthenticationCode[3];
                    //
                    // byte ApplicationData[N];
                    //
                    // byte BlockTerminator;            // fixed with 0
                    // --------------------------------------------------------------
                    //
                case 0xff:
                    //while(!ifs.eof())
                    while(!FILEEOF(fp))
                    {	
                        //ifs.read((char*)&be,1);
                        FILEREAD((char*)&be,1,1,fp);
                        if(be == 0)
                            break;

                        //ifs.seekg(be,ios::cur);
                        FILESEEK(fp, be, SEEK_CUR);
                    }
                    break;
                    
                default:
                    goto error;
                }
                break;
            }
            
            // 
            // [Image Descriptor]
            // --------------------------------------------------------------
            // byte ImageSeperator;                 //fixed with 0x2c
            // unsigned short ImageLeftPosition;
            // unsigned short ImageTopPosition;
            // unsigned short ImageWidth;
            // unsigned short ImageHeight;
            // byte PackedFields;
            // --------------------------------------------------------------
            //<Packed Fields>
            // 
            //  byte LocalColorTableFlag : 1;
            //  byte InterlaceFlag : 1;
            //  byte SortFlag : 1;
            //  byte Reserved : 2;
            //  byte SizeOfLocalColorTable : 3;
            // --------------------------------------------------------------
            //
        case 0x2c:
            {	
                BYTE bp;
                //ifs.read((char*)&pf->imageLPos,2);
                FILEREAD((char*)&pf->imageLPos,1,2,fp);
                //ifs.read((char*)&pf->imageTPos,2);
                FILEREAD((char*)&pf->imageTPos,1,2,fp);
                //ifs.read((char*)&pf->imageWidth,2);
                FILEREAD((char*)&pf->imageWidth,1,2,fp);
                //ifs.read((char*)&pf->imageHeight,2);
                FILEREAD((char*)&pf->imageHeight,1,2,fp);
                //ifs.read((char*)&bp,1);
                FILEREAD((char*)&bp,1,1,fp);

                if((bp&0x80) != 0)
                    pf->lFlag = TRUE;
                if((bp&0x40) != 0)
                    pf->interlaceFlag = TRUE;
                if((bp&0x20) != 0)
                    pf->sortFlag = TRUE;
                pf->lSize = 1;
                pf->lSize <<= ((bp&0x07)+1);
                if(pf->lFlag)
                {	
                    if((pf->pColorTable = new BYTE[pf->lSize*3]) == NULL)
                        goto error;
                    //ifs.read((char*)pf->pColorTable,pf->lSize*3);
                    FILEREAD((char*)pf->pColorTable,1,pf->lSize*3,fp);
                }
                //if(!ifs.good())
                  //  goto error;
                if(FILEEOF(fp))
                    goto error;

                //if(!extractData(pf,ifs))
                if(!extractData(pf))
                    goto error;

                if(ctrlExt.active)
                {	
					pf->ctrlExt = ctrlExt;
					ctrlExt.active = FALSE;
                }
                pf++;
                break;
            }

            //
            // [GIF Trailer]
            // -----------------------------------------------
            // byte Description;        // fixed with 0x3b
            // -----------------------------------------------
        case 0x3b:
            fileEnd = TRUE;
            break;

            //
            // [Block Terminator]
            // -----------------------------------------------
            // byte BlockSize;          // fixed with 0x00
            // -----------------------------------------------
            //
        case 0x00:
            break;

        default:
            goto error;
        }
    }
	//ifs.seekg(pos);
	FILESEEK(fp, pos, SEEK_SET);
	return TRUE;

error:
	pf = allFrames;
	for(i=0;i<(int)gInfo.frames;i++)
	{	
        if(pf->pColorTable != NULL)
		{	
            delete[] pf->pColorTable;
			pf->pColorTable = NULL;
		}
		if(pf->dataBuf != NULL)
		{	
            delete[] pf->dataBuf;
			pf->dataBuf = NULL;
		}

		// Jim, added [10/14/2003]
		//{{{
		pf++;
		//}}}
	}

	// Jim, added [10/14/2003]
	//{{{
	delete[] allFrames;
	allFrames = NULL;
	//}}}

	return FALSE;
}

// ��  �ܣ�ȡ��һ֡ͼ��
// ��  ������
// ����ֵ��ָ��FRAME�ṹ��ָ�룬������ͼ�����ݣ�������,�ɹ�ʱ���ط�0ֵ��ʧ��ʱ����NULL��
// ˵  ����ÿ�ε��÷���һ֡�µ����ݣ�ͼ���������Ѿ���ѹ�ġ�����ȡ���һ֡���ݺ���
//         �ε��ô˺��������ص�һ֡�����ݣ����ѭ����ʹ�÷��ص�����ʱҪע����ֲ�����
//         �ر��������Ƿ񽻴�
//         ���ͼ��֡������1��ѭ�����ô˺����ɵõ�����GIF�ļ��е�����ͼ��֡���ݡ�
//         һ��Ҫ��鷵��ֵ�Ƿ���Ч��
LPCFRAME CGif89a::getNextFrame()
{	
    if(inMem)
	{	
        FRAME* p =  allFrames+curIndex;
		curIndex++;
		if(curIndex >= gInfo.frames)
			curIndex = 0;
		return p;
	}
	else
	{
        BYTE be;
		BOOL fileEnd = FALSE;
		if(curFrame.pColorTable != NULL)
		{	delete[] curFrame.pColorTable;
			curFrame.pColorTable = NULL;
		}
		if(curFrame.dataBuf != NULL)
		{	delete[] curFrame.dataBuf;
			curFrame.dataBuf = NULL;
		}
		ZeroMemory(&curFrame,sizeof(FRAME));
		while(TRUE)
		{
            //ifs.read((char*)&be,1);
            FILEREAD((char*)&be,1,1,fp);

			switch(be)
			{
            case 0x21:
                {
                    //ifs.read((char*)&be,1);
                    FILEREAD((char*)&be,1,1,fp);

                    switch(be)
                    {
                    case 0xf9:
                        //while(!ifs.eof())
                        while(!FILEEOF(fp))
                        {	
							//ifs.read((char*)&be,1);
							FILEREAD((char*)&be,1,1,fp);

							if(be == 0)
								break;
							if(be == 4)
							{	
								ctrlExt.active = TRUE;

								//ifs.read((char*)&be,1);
								FILEREAD((char*)&be,1,1,fp);

								ctrlExt.disposalMethod = (be&0x1c)>>2;
								if((be&0x02) != 0)
									ctrlExt.userInputFlag = TRUE;
								else
									ctrlExt.userInputFlag = FALSE;
								if((be&0x01) != 0)
									ctrlExt.trsFlag = TRUE;
								else
									ctrlExt.trsFlag = FALSE;

								//ifs.read((char*)&ctrlExt.delayTime,2);
								FILEREAD((char*)&ctrlExt.delayTime,1,2,fp);

								//ifs.read((char*)&be,1);
								FILEREAD((char*)&be,1,1,fp);

								ctrlExt.trsColorIndex = be;
							}
							else
							{
								//ifs.seekg(be,ios::cur);
								FILESEEK(fp, be, SEEK_CUR);
							}
                        }
                        break;
                    case 0xfe:
                    case 0x01:
                    case 0xff:
                        //while(!ifs.eof())
                        while(!FILEEOF(fp))
                        {	
							//ifs.read((char*)&be,1);
							FILEREAD((char*)&be,1,1,fp);
							if(be == 0)
								break;
							//ifs.seekg(be,ios::cur);
							FILESEEK(fp, be, SEEK_CUR);
                        }
                        break;
                    default:
                        goto error;
                    }
                    break;
                }
                
            case 0x2c:
                {	
                    BYTE bp;
                    //ifs.read((char*)&curFrame.imageLPos,2);
                    //ifs.read((char*)&curFrame.imageTPos,2);
                    //ifs.read((char*)&curFrame.imageWidth,2);
                    //ifs.read((char*)&curFrame.imageHeight,2);
                    //ifs.read((char*)&bp,1);
                    FILEREAD((char*)&curFrame.imageLPos,1,2,fp);
                    FILEREAD((char*)&curFrame.imageTPos,1,2,fp);
                    FILEREAD((char*)&curFrame.imageWidth,1,2,fp);
                    FILEREAD((char*)&curFrame.imageHeight,1,2,fp);
                    FILEREAD((char*)&bp,1,1,fp);

                    if((bp&0x80) != 0)
                        curFrame.lFlag = TRUE;
                    if((bp&0x40) != 0)
                        curFrame.interlaceFlag = TRUE;
                    if((bp&0x20) != 0)
                        curFrame.sortFlag = TRUE;
                    curFrame.lSize = 1;
                    curFrame.lSize <<= ((bp&0x07)+1);
                    if((curFrame.pColorTable = new BYTE[curFrame.lSize*3]) == NULL)
                        goto error;
                    if(curFrame.lFlag)
					{
                        //ifs.read((char*)curFrame.pColorTable,curFrame.lSize*3);
                        FILEREAD((char*)curFrame.pColorTable,1,curFrame.lSize*3,fp);
					}

                    //if(!ifs.good())
                      //  goto error;
					if(FILEEOF(fp))
						goto error;

                    //if(!extractData(&curFrame,ifs))
                    if(!extractData(&curFrame))
                        goto error;

                    curFrame.ctrlExt = ctrlExt;
                    if(ctrlExt.active = TRUE)
                        ctrlExt.active = FALSE;
                    return &curFrame;
                }
                
            case 0x3b:
                //ifs.seekg(dataStart);
                FILESEEK(fp, dataStart, SEEK_SET);
                break;
                
            case 0x00:
                break;
                
            default:
                goto error;
            }
        }
        return &curFrame;
error:
        if(curFrame.pColorTable != NULL)
        {	delete[] curFrame.pColorTable;
        curFrame.pColorTable = NULL;
        }
        if(curFrame.dataBuf != NULL)
        {	delete[] curFrame.dataBuf;
        curFrame.dataBuf = NULL;
        }
        return NULL;
    }
}

// ����  ����ʼ���ַ�����
// ����  ��strTable:ָ���ַ������ָ�롣
//         rootSize:��ʼ�����������
// ����ֵ�������Ƿ�ɹ���Ϊ��ɹ���Ϊ��ʧ�ܡ�
BOOL CGif89a::initStrTable(STRING_TABLE_ENTRY* strTable,UINT rootSize)
{
    UINT i;
    unsigned char *cc;
    for(i=0;i<rootSize;i++)
    {	
        if((cc = new unsigned char[2]) == NULL)
            goto error;
        cc[0] = i,cc[1] = 0;
        strTable[i].p = cc;
        strTable[i].len = 1;
    }
    return TRUE;
error:
    for(i=0;i<rootSize;i++)
    {
        if(strTable[i].p != NULL)
        {	
            delete[] strTable[i].p;
            strTable[i].p = NULL;
        }
    }
    return FALSE;
}


// ����  �����ַ�����������һ�
// ����  ��strTable:ָ���ַ������ָ�롣
//         addIdx:���ӵ����������
//         idx:���ڹ���Ҫ���ӵ��ַ��������������
//         c:���ڹ���Ҫ���ӵ��ַ������ַ���
// ����ֵ�������Ƿ�ɹ���Ϊ��ɹ���Ϊ��ʧ�ܡ�
BOOL CGif89a::addStrTable(STRING_TABLE_ENTRY* strTable,UINT addIdx,UINT idx,unsigned char c)
{	
    unsigned char *cc;
	UINT l = strTable[idx].len;
	if(addIdx >= 4096)
		return FALSE;
	if((cc = new unsigned char[l+2]) == NULL)
		return FALSE;
	for(UINT i=0;i<l;i++)
		cc[i] = strTable[idx].p[i];
	cc[l] = c;
	cc[l+1] = 0;
	strTable[addIdx].p = cc;
	strTable[addIdx].len = strTable[idx].len +1;
	return TRUE;
}

//BOOL CGif89a::extractData(FRAME* f,ifstream& ifs)
BOOL CGif89a::extractData(FRAME* f)
{	
    STRING_TABLE_ENTRY *strTable;
	UINT codeSize,rootSize,tableIndex,codeSizeBK;
	int remainInBuf = 0,i;
	UINT bufIndex = 0,outIndex = 0;
	UINT bitIndex = 0;
	DWORD code,oldCode;
	BYTE be,*outP;
	BYTE buf[262];
	BOOL readOK = FALSE;
	UINT bufLen = f->imageWidth*f->imageHeight;
	if((strTable = new STRING_TABLE_ENTRY[4096]) == NULL)
		return FALSE;
	ZeroMemory(strTable,sizeof(STRING_TABLE_ENTRY)*4096);
	outP = f->dataBuf = new BYTE[bufLen];
	if(f->dataBuf == NULL)
	{	
		delete[] strTable;
		return FALSE;
	}

	//ifs.read((char*)&be,1);
	FILEREAD((char*)&be,1,1,fp);

	codeSizeBK = codeSize = be+1;
	rootSize = 1;
	rootSize <<= be; 
	tableIndex = rootSize+2;
	if(!initStrTable(strTable,rootSize))
		goto error;

begin:
	if(remainInBuf<=4 && !readOK)
	{	
        for(i=0;i<remainInBuf;i++)
			buf[i] = buf[bufIndex+i];
		bufIndex = 0;

		//ifs.read((char*)&be,1);
		FILEREAD((char*)&be,1,1,fp);

		if(be != 0)
		{	
			//ifs.read((char*)(buf+remainInBuf),be);
			FILEREAD((char*)(buf+remainInBuf),1,be,fp);
			remainInBuf += be;
		}
		else
			readOK = TRUE;

		//if(!ifs.good())
		//	goto error;
		if(FILEEOF(fp))
			goto error;
	}
	if(remainInBuf<=4)
		if(remainInBuf<=0 || codeSize > (remainInBuf*8-bitIndex))
			goto done;
	code = *((DWORD*)(buf+bufIndex));
	code <<= 32-codeSize-bitIndex;
	code >>= 32-codeSize;
	bitIndex += codeSize;
	bufIndex += bitIndex/8;
	remainInBuf -= bitIndex/8;
	bitIndex %= 8;
	if(code >= rootSize+1)
		goto error;
	if(code == rootSize)
		goto begin;
	else
	{	
		outP[outIndex++] = *strTable[code].p;
		oldCode = code;
	}
	for(;;)
	{	
        if(remainInBuf<=4 && !readOK)
		{	
            for(i=0;i<remainInBuf;i++)
				buf[i] = buf[bufIndex+i];
			bufIndex = 0;

			//ifs.read((char*)&be,1);
			FILEREAD((char*)&be,1,1,fp);

			if(be != 0)
			{	
				//ifs.read((char*)(buf+remainInBuf),be);
				FILEREAD((char*)(buf+remainInBuf),1,be,fp);
				remainInBuf += be;
			}
			else
				readOK = TRUE;

			//if(!ifs.good())
			//	goto error;
			if(FILEEOF(fp))
				goto error;
		}
		if(remainInBuf<=4)
			if(remainInBuf<=0 || codeSize > (remainInBuf*8-bitIndex))
				break;
		code = *((DWORD*)(buf+bufIndex));
		code <<= 32-codeSize-bitIndex;
		code >>= 32-codeSize;
		bitIndex += codeSize;
		bufIndex += bitIndex/8;
		remainInBuf -= bitIndex/8;
		bitIndex %= 8;
		if(code == rootSize)
		{	
            codeSize = codeSizeBK;
			for(i=rootSize;i<4096;i++)
            {
				if(strTable[i].p != NULL)
				{	
					delete strTable[i].p;
					strTable[i].p = NULL;
					strTable[i].len = 0;
				}
            }
			tableIndex = rootSize+2;
			goto begin;
		}
		else if(code == rootSize+1)
			break;
		else
		{	
            unsigned char *p = strTable[code].p;
			int l = strTable[code].len;
			unsigned char c;
			if(p != NULL)
			{	
                c = *p;
				if(outIndex+l <= bufLen)
					for(i=0;i<l;i++)
						outP[outIndex++] = *p++;
				else
					goto error;
				if(!addStrTable(strTable,tableIndex++,oldCode,c))
					goto error;
				oldCode = code;
			}
			else
			{	
                p = strTable[oldCode].p;
				l = strTable[oldCode].len;
				c = *p;
				if(outIndex+l+1 <= bufLen)
				{	for(i=0;i<l;i++)
						outP[outIndex++] = *p++;
					outP[outIndex++] = c;
				}
				else
					goto error;
				if(!addStrTable(strTable,tableIndex++,oldCode,c))
					goto error;
				oldCode = code;
			}
			if(tableIndex == (((UINT)1)<<codeSize) && codeSize != 12)
				codeSize++;
		}
	}
done:
	for(i=0;i<4096;i++)
    {
		if(strTable[i].p != NULL)
		{	
			delete strTable[i].p;
			strTable[i].p = NULL;
		}
    }
	delete[] strTable;
	return TRUE;
error:
	for(i=0;i<4096;i++)
    {
		if(strTable[i].p != NULL)
		{	
			delete strTable[i].p;
			strTable[i].p = NULL;
		}
    }
	delete[] strTable;
	delete[] f->dataBuf;
	f->dataBuf = NULL;
	return FALSE;
}

LPCGLOBAL_INFO CGif89a::getGlobalInfo()
{	
    return &gInfo;
}

void CGif89a::close()
{	
    if(opened)
	{	
        opened = FALSE;
		if(inMem && allFrames != NULL)
		{	
            FRAME* pf = allFrames;
			for(UINT i=0;i<gInfo.frames;i++)
			{	
				if(pf->pColorTable != NULL)
				{	
					delete[] pf->pColorTable;
					pf->pColorTable = NULL;
				}
				if(pf->dataBuf != NULL)
				{	
					delete[] pf->dataBuf;
					pf->dataBuf = NULL;
				}
				// Jim, added [10/14/2003]
				//{{{
				pf++;
				//}}}
			}
			delete[] allFrames;
			allFrames = NULL;
		}
		if(!inMem)
		{	
            if(curFrame.pColorTable != NULL)
			{	
				delete[] curFrame.pColorTable;
				curFrame.pColorTable = NULL;
			}
			if(curFrame.dataBuf != NULL)
			{	
				delete[] curFrame.dataBuf;
				curFrame.dataBuf = NULL;
			}
			//ifs.close();
			if(fp) {FILECLOSE(fp); fp = NULL;}
		}
	}
}


LPCSTR CGif89a::getVer()
{	
    return version;
}



/////////////////////////////////////////////////////////////////////////////
// Examples :
// Jim,  [7/5/2003]
/*
void example_for_show_gif_image(void)
{
    char *  filename="pic\\6.gif";
    CGif89a gif;
    LPCGLOBAL_INFO gi;
    SLONG   gif_width, gif_height;
    BMP     *gif_bitmap = NULL;
    UINT    n, n3;
    UCHR    r, g, b;
    PIXEL   palette[256];
    UINT    bkcolor_index;
    
    gif.open(filename, TRUE);
    if(!gif)
        break;
    gi = gif.getGlobalInfo();
    
    //{
    log_error(1, "GIF:%s", filename);
    log_error(1, "�߼���Ļ���=%d",gi->scrWidth);
    log_error(1, "�߼���Ļ�߶�=%d",gi->scrHeight);
    log_error(1, "ͼ��֡��=%d",gi->frames);
    log_error(1, "����ɫ�ĵ�ɫ������=%d",gi->BKColorIdx);
    if(gi->gFlag)
    {	
        log_error(1, "ȫ�ֵ�ɫ��=��");
        log_error(1, "ȫ�ֵ�ɫ�������=%d",gi->gSize);
        if(gi->gSort)
            log_error(1, "ȫ�ֵ�ɫ���Ƿ���������=��");
        else
            log_error(1, "ȫ�ֵ�ɫ���Ƿ���������=��");
    }
    else
    {
        log_error(1, "ȫ�ֵ�ɫ��=��");
    }
    //}
    
    gif_width = gi->scrWidth;
    gif_height = gi->scrHeight;
    bkcolor_index = gi->BKColorIdx;
    gif_bitmap = create_bitmap(gif_width, gif_height);
    if(NULL == gif_bitmap)
        return;
    get_bitmap(0, 0, gif_width, gif_height, gif_bitmap, screen_channel0);
    if(gi->gFlag) //��ȫ�ֵ�ɫ��
    {
        for(n=0; n<gi->gSize; n++)
        {
            n3 = n+n+n;
            r = gi->gColorTable[n3+0];
            g = gi->gColorTable[n3+1];
            b = gi->gColorTable[n3+2];
            palette[n] = rgb2hi(r, g, b);
        }
    }
    
    for(UINT i=0; i<gi->frames; i++)
    {
        LPCFRAME fm;
        SLONG   x, y, offset;
        BYTE    ch;
        
        fm = gif.getNextFrame();
        if(fm == NULL)
            continue;
        
        //{
        log_error(1, "��%d֡", i+1);
        log_error(1, "ͼ���ȣ�%d",fm->imageWidth);
        log_error(1,"ͼ��߶ȣ�%d",fm->imageHeight);
        log_error(1,"ͼ�����Ͻ�X����=%d",fm->imageLPos);
        log_error(1,"ͼ�����Ͻ�Y����=%d",fm->imageTPos);
        if(fm->interlaceFlag)
            log_error(1,"�����Ƿ񽻴�=��");
        else
            log_error(1,"�����Ƿ񽻴�=��");
        if(fm->lFlag)
        {	
            log_error(1,"�Ƿ��оֲ���ɫ��=��");
            log_error(1,"�ֲ���ɫ�������=%d",fm->lSize);
            if(fm->sortFlag)
                log_error(1,"�ֲ���ɫ���Ƿ���������=��");
            else
                log_error(1,"�ֲ���ɫ���Ƿ���������=��");
        }
        else
            log_error(1,"�Ƿ��оֲ���ɫ��=��");
        if(fm->ctrlExt.active)
        {	
            log_error(1,"����ʽ=%d",fm->ctrlExt.disposalMethod);
            if(fm->ctrlExt.userInputFlag)
                log_error(1,"�Ƿ�ȴ��û�����=��");
            else
                log_error(1,"�Ƿ�ȴ��û�����=��");
            log_error(1,"��ʱ�����룩=%d",fm->ctrlExt.delayTime*10);
            if(fm->ctrlExt.trsFlag)
                log_error(1,"�Ƿ���͸��ɫ=��");
            else
                log_error(1,"�Ƿ���͸��ɫ=��");
        }
        //}
        
        if(fm->lFlag)   //�оֲ���ɫ��
        {
            for(n = 0; n < fm->lSize; n++)
            {
                n3 = n + n + n;
                r = fm->pColorTable[n3 + 0];
                g = fm->pColorTable[n3 + 1];
                b = fm->pColorTable[n3 + 2];
                palette[n] = rgb2hi(r, g, b);
            }
        }
        
        offset = 0;
        for(y = fm->imageTPos; y < fm->imageTPos + fm->imageHeight; y++)
        {
            for(x = fm->imageLPos; x < fm->imageLPos + fm->imageWidth; x++)
            {
                ch = fm->dataBuf[offset + x];
                if(fm->ctrlExt.trsFlag) //��ͼ����չ����
                {
                    if(ch != fm->ctrlExt.trsColorIndex)
                    {
                        put_pixel(x, y, palette[ch], gif_bitmap);
                    }
                }
                else
                {
                    put_pixel(x, y, palette[ch], gif_bitmap);
                }
            }
            offset += fm->imageWidth;
        }
        
        put_bitmap(0, 0, gif_bitmap, screen_channel0);
    }
    destroy_bitmap(&gif_bitmap);
    gif.close();
}
*/
/////////////////////////////////////////////////////////////////////////////

