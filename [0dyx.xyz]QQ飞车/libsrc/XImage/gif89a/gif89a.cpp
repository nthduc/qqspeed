#include <fstream.h>
#include <string.h>
#include "gif89a.h"

CGif89a::CGif89a()
{	opened = FALSE;
	error = FALSE;
}

CGif89a::CGif89a(LPCSTR fileName,BOOL inMem)
{	if(open(fileName,inMem))
	{	opened = TRUE;
		error = FALSE;
	}
	else
	{	opened = FALSE;
		error = TRUE;
	}
}

CGif89a::~CGif89a()
{	close();
}

BOOL CGif89a::operator!()
{	return error;
}

BOOL CGif89a::open(LPCSTR fileName,BOOL b)
{	char cc[4];
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
	ifs.open(fileName,ios::binary);
	if(!ifs)
		return FALSE;
	ifs.read(cc,3);
	if(strncmp(cc,"GIF",3) != 0)
		goto error;
	ifs.read(version,3);
	version[3] = 0;
	if(strncmp(version,"89a",3) > 0)
		goto error;
	ifs.read((char*)&gInfo.scrWidth,2);
	ifs.read((char*)&gInfo.scrHeight,2);
	if(!ifs.good())
		goto error;
	ifs.read((char*)&be,1);
	if((be&0x80) != 0)
		gInfo.gFlag = TRUE;
	else
		gInfo.gFlag = FALSE;
	gInfo.colorRes = ((be&0x70)>>4)+1;
	if(gInfo.gFlag)
	{	if((be&0x08) != 0)
			gInfo.gSort = TRUE;
		else
			gInfo.gSort = FALSE;
		gInfo.gSize = 1;
		gInfo.gSize <<= ((be&0x07)+1);
	}
	ifs.read((char*)&be,1);
	gInfo.BKColorIdx = be;
	ifs.read((char*)&be,1);
	gInfo.pixelAspectRatio = be;
	if(!ifs.good())
		goto error;
	if(gInfo.gFlag)
	{	ifs.read((char*)gColorTable,gInfo.gSize*3);
		gInfo.gColorTable = gColorTable;
	}
	else
		gInfo.gColorTable = NULL;
	dataStart = ifs.tellg();
	if((gInfo.frames = checkFrames(ifs)) == 0)
		goto error;
	if(inMem)
	{	if((allFrames = new FRAME[gInfo.frames]) == NULL)
			goto error;
		ZeroMemory(allFrames,sizeof(FRAME)*gInfo.frames);
		if(!getAllFrames(ifs))
		{	delete[] allFrames;
			allFrames = NULL;
			goto error;
		}
		ifs.close();
	}
	opened = TRUE;
	return TRUE;
error:
	ifs.close();
	return FALSE;
}

UINT CGif89a::checkFrames(ifstream& ifs)
{	BYTE be;
	BOOL fileEnd = FALSE;
	UINT frames=0;
	streampos pos = ifs.tellg();
	while(!ifs.eof() && !fileEnd)
	{	ifs.read((char*)&be,1);
		switch(be)
		{	case 0x21:
				ifs.read((char*)&be,1);
				switch(be)
				{	case 0xf9:
					case 0xfe:
					case 0x01:
					case 0xff:
						while(!ifs.eof())
						{	ifs.read((char*)&be,1);
							if(be == 0)
								break;
							ifs.seekg(be,ios::cur);
						}
						break;
					default:
						return 0;
				}
				break;
			case 0x2c:
			{	BYTE bp;
				BOOL lFlag=FALSE;
				UINT lSize=1;
				frames++;
				ifs.seekg(8,ios::cur);
				ifs.read((char*)&bp,1);
				if((bp&0x80) != 0)
					lFlag = TRUE;
				lSize <<= ((bp&0x07)+1);
				if(lFlag)
					ifs.seekg(lSize*3,ios::cur);
				if(!ifs.good())
					return 0;
				ifs.read((char*)&be,1);
				while(!ifs.eof())
				{	ifs.read((char*)&be,1);
					if(be == 0)
						break;
					ifs.seekg(be,ios::cur);
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
	ifs.seekg(pos);
	return frames;
}

BOOL CGif89a::getAllFrames(ifstream& ifs)
{	BYTE be;
	BOOL fileEnd = FALSE;
	FRAME *pf = allFrames;
	streampos pos = ifs.tellg();
	int i;
	while(!ifs.eof() && !fileEnd)
	{	ifs.read((char*)&be,1);
		switch(be)
		{	case 0x21:
				ifs.read((char*)&be,1);
				switch(be)
				{	case 0xf9:
						while(!ifs.eof())
						{	ifs.read((char*)&be,1);
							if(be == 0)
								break;
							if(be == 4)
							{	ctrlExt.active = TRUE;
								ifs.read((char*)&be,1);
								ctrlExt.disposalMethod = (be&0x1c)>>2;
								if((be&0x02) != 0)
									ctrlExt.userInputFlag = TRUE;
								else
									ctrlExt.userInputFlag = FALSE;
								if((be&0x01) != 0)
									ctrlExt.trsFlag = TRUE;
								else
									ctrlExt.trsFlag = FALSE;
								ifs.read((char*)&ctrlExt.delayTime,2);
								ifs.read((char*)&be,1);
								ctrlExt.trsColorIndex = be;
							}
							else
								ifs.seekg(be,ios::cur);
						}
						break;
					case 0xfe:
					case 0x01:
					case 0xff:
						while(!ifs.eof())
						{	ifs.read((char*)&be,1);
							if(be == 0)
								break;
							ifs.seekg(be,ios::cur);
						}
						break;
					default:
						goto error;
				}
				break;
			case 0x2c:
			{	BYTE bp;
				ifs.read((char*)&pf->imageLPos,2);
				ifs.read((char*)&pf->imageTPos,2);
				ifs.read((char*)&pf->imageWidth,2);
				ifs.read((char*)&pf->imageHeight,2);
				ifs.read((char*)&bp,1);
				if((bp&0x80) != 0)
					pf->lFlag = TRUE;
				if((bp&0x40) != 0)
					pf->interlaceFlag = TRUE;
				if((bp&0x20) != 0)
					pf->sortFlag = TRUE;
				pf->lSize = 1;
				pf->lSize <<= ((bp&0x07)+1);
				if(pf->lFlag)
				{	if((pf->pColorTable = new BYTE[pf->lSize*3]) == NULL)
						goto error;
					ifs.read((char*)pf->pColorTable,pf->lSize*3);
				}
				if(!ifs.good())
					goto error;
				if(!extractData(pf,ifs))
					goto error;
				if(ctrlExt.active)
				{	pf->ctrlExt = ctrlExt;
					ctrlExt.active = FALSE;
				}
				pf++;
				break;
			}
			case 0x3b:
				fileEnd = TRUE;
				break;
			case 0x00:
				break;
			default:
				goto error;
		}
	}
	ifs.seekg(pos);
	return TRUE;
error:
	pf = allFrames;
	for(i=0;i<(int)gInfo.frames;i++)
	{	if(pf->pColorTable != NULL)
		{	delete[] pf->pColorTable;
			pf->pColorTable = NULL;
		}
		if(pf->dataBuf != NULL)
		{	delete[] pf->dataBuf;
			pf->dataBuf = NULL;
		}
	}
	return FALSE;
}

LPCFRAME CGif89a::getNextFrame()
{	if(inMem)
	{	FRAME* p =  allFrames+curIndex;
		curIndex++;
		if(curIndex >= gInfo.frames)
			curIndex = 0;
		return p;
	}
	else
	{	BYTE be;
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
		{	ifs.read((char*)&be,1);
			switch(be)
			{	case 0x21:
					ifs.read((char*)&be,1);
					switch(be)
					{	case 0xf9:
							while(!ifs.eof())
							{	ifs.read((char*)&be,1);
								if(be == 0)
									break;
								if(be == 4)
								{	ctrlExt.active = TRUE;
									ifs.read((char*)&be,1);
									ctrlExt.disposalMethod = (be&0x1c)>>2;
									if((be&0x02) != 0)
										ctrlExt.userInputFlag = TRUE;
									else
										ctrlExt.userInputFlag = FALSE;
									if((be&0x01) != 0)
										ctrlExt.trsFlag = TRUE;
									else
										ctrlExt.trsFlag = FALSE;
									ifs.read((char*)&ctrlExt.delayTime,2);
									ifs.read((char*)&be,1);
									ctrlExt.trsColorIndex = be;
								}
								else
									ifs.seekg(be,ios::cur);
							}
							break;
						case 0xfe:
						case 0x01:
						case 0xff:
							while(!ifs.eof())
							{	ifs.read((char*)&be,1);
								if(be == 0)
									break;
								ifs.seekg(be,ios::cur);
							}
							break;
						default:
							goto error;
					}
					break;
				case 0x2c:
				{	BYTE bp;
					ifs.read((char*)&curFrame.imageLPos,2);
					ifs.read((char*)&curFrame.imageTPos,2);
					ifs.read((char*)&curFrame.imageWidth,2);
					ifs.read((char*)&curFrame.imageHeight,2);
					ifs.read((char*)&bp,1);
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
						ifs.read((char*)curFrame.pColorTable,curFrame.lSize*3);
					if(!ifs.good())
						goto error;
					if(!extractData(&curFrame,ifs))
						goto error;
					curFrame.ctrlExt = ctrlExt;
					if(ctrlExt.active = TRUE)
						ctrlExt.active = FALSE;
					return &curFrame;
				}
				case 0x3b:
					ifs.seekg(dataStart);
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

BOOL CGif89a::initStrTable(STRING_TABLE_ENTRY* strTable,UINT rootSize)
{	UINT i;
	unsigned char *cc;
	for(i=0;i<rootSize;i++)
	{	if((cc = new unsigned char[2]) == NULL)
			goto error;
		cc[0] = i,cc[1] = 0;
		strTable[i].p = cc;
		strTable[i].len = 1;
	}
	return TRUE;
error:
	for(i=0;i<rootSize;i++)
		if(strTable[i].p != NULL)
		{	delete[] strTable[i].p;
			strTable[i].p = NULL;
		}
	return FALSE;
}

BOOL CGif89a::addStrTable(STRING_TABLE_ENTRY* strTable,UINT addIdx,UINT idx,unsigned char c)
{	unsigned char *cc;
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

BOOL CGif89a::extractData(FRAME* f,ifstream& ifs)
{	STRING_TABLE_ENTRY *strTable;
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
	{	delete[] strTable;
		return FALSE;
	}
	ifs.read((char*)&be,1);
	codeSizeBK = codeSize = be+1;
	rootSize = 1;
	rootSize <<= be; 
	tableIndex = rootSize+2;
	if(!initStrTable(strTable,rootSize))
		goto error;

begin:
	if(remainInBuf<=4 && !readOK)
	{	for(i=0;i<remainInBuf;i++)
			buf[i] = buf[bufIndex+i];
		bufIndex = 0;
		ifs.read((char*)&be,1);
		if(be != 0)
		{	ifs.read((char*)(buf+remainInBuf),be);
			remainInBuf += be;
		}
		else
			readOK = TRUE;
		if(!ifs.good())
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
	{	outP[outIndex++] = *strTable[code].p;
		oldCode = code;
	}
	for(;;)
	{	if(remainInBuf<=4 && !readOK)
		{	for(i=0;i<remainInBuf;i++)
				buf[i] = buf[bufIndex+i];
			bufIndex = 0;
			ifs.read((char*)&be,1);
			if(be != 0)
			{	ifs.read((char*)(buf+remainInBuf),be);
				remainInBuf += be;
			}
			else
				readOK = TRUE;
			if(!ifs.good())
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
		{	codeSize = codeSizeBK;
			for(i=rootSize;i<4096;i++)
				if(strTable[i].p != NULL)
				{	delete strTable[i].p;
					strTable[i].p = NULL;
					strTable[i].len = 0;
				}
			tableIndex = rootSize+2;
			goto begin;
		}
		else if(code == rootSize+1)
			break;
		else
		{	unsigned char *p = strTable[code].p;
			int l = strTable[code].len;
			unsigned char c;
			if(p != NULL)
			{	c = *p;
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
			{	p = strTable[oldCode].p;
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
		if(strTable[i].p != NULL)
		{	delete strTable[i].p;
			strTable[i].p = NULL;
		}
	delete[] strTable;
	return TRUE;
error:
	for(i=0;i<4096;i++)
		if(strTable[i].p != NULL)
		{	delete strTable[i].p;
			strTable[i].p = NULL;
		}
	delete[] strTable;
	delete[] f->dataBuf;
	f->dataBuf = NULL;
	return FALSE;
}

LPCGLOBAL_INFO CGif89a::getGlobalInfo()
{	return &gInfo;
}

void CGif89a::close()
{	if(opened)
	{	opened = FALSE;
		if(inMem && allFrames != NULL)
		{	FRAME* pf = allFrames;
			for(UINT i=0;i<gInfo.frames;i++)
			{	if(pf->pColorTable != NULL)
				{	delete[] pf->pColorTable;
					pf->pColorTable = NULL;
				}
				if(pf->dataBuf != NULL)
				{	delete[] pf->dataBuf;
					pf->dataBuf = NULL;
				}
			}
			delete[] allFrames;
			allFrames = NULL;
		}
		if(!inMem)
		{	if(curFrame.pColorTable != NULL)
			{	delete[] curFrame.pColorTable;
				curFrame.pColorTable = NULL;
			}
			if(curFrame.dataBuf != NULL)
			{	delete[] curFrame.dataBuf;
				curFrame.dataBuf = NULL;
			}
			ifs.close();
		}
	}
}

LPCSTR CGif89a::getVer()
{	return version;
}
