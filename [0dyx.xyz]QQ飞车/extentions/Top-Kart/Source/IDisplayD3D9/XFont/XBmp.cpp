//:Bmp.cpp		by czy
#include "stdio.h"
#include "assert.h"
#include "stdlib.h"
#include "..\Engine.h"
#include "XBmp.h"

XBmp::XBmp()
{
	m_pBmp= NULL;
}

XBmp::~XBmp()
{
	Release();
};

void XBmp::Release()
{
	if (m_pBmp!=NULL)
		free(m_pBmp);
	m_pBmp= NULL;
};


//��Bmp�ڴ�ӳ���������ļ�
void XBmp::Save(char* sFileName)
{
	FILE *pFile=fopen(sFileName, "wb");
	assert(pFile);
	fwrite(m_pBmp, 1, FileLen(), pFile);
	fclose(pFile);
}

//��Bmp��λ��x,yȡ����ɫֵ
int  XBmp::Get(int x, int y)
{
	if(2==m_ColorCnt)
	{
		//2 color
		BYTE b= m_pData[(m_nHeight-1-y)*m_nPitch + x/8];	//ע��Bmpĩ�д�����ǰ�棡
		return (b & (0x80>>(x%8)))!=0;
	}
	else
	{						
		//16 color
		BYTE b= m_pData[(m_nHeight-1-y)*m_nPitch + x/2];
		return (x%2)? b & 0x0f : b>>4;
	}
}

//��Bmp��λ��x,y��ΪnColor
void XBmp::Set(int x, int y, int nColor)
{
	if(0==nColor) return;		//not good
	if(2==m_ColorCnt)
	{			
		//2 color
		BYTE& b= m_pData[(m_nHeight-1-y)*m_nPitch + x/8];
		b |= 0x80>>(x%8);
	}
	else if(16==m_ColorCnt)
	{		
		//16 color
		nColor &=0x0f;
		BYTE& b= m_pData[(m_nHeight-1-y)*m_nPitch + x/2];
		if(x%2)
			b = (b & 0xf0)| nColor;
		else
			b = (b & 0x0f)| nColor<<4;
	}
}

//========2 Color Bmp example========================================
BmpFileHead BmpCopyC2=
{
	'B','M',
	0x203e,				//file size;
	0,
	0x3e,
	0x28,
	0x100, 0x100,		//width, height
	1,1,
	0,					//non compression
	0x2000,				//data size
	0x0ec4,
	0x0ec4,
	0,0,
	0, 0x00ffffff		//palette
};

//========16 Color Bmp example========================================
BmpFileHead BmpCopyC16 =
{
	'B','M',
	0x0276,				//file size;
	0,
	0x76,				//bitmap_data_offset
	0x28,
	0x20, 0x20,			//width, height
	1,4,				//planes, bits_per_pixel;
	0,					//non compression
	0x0200,				//data size
	0x0ec4,
	0x0ec4,
	0,0,
	0, 0x00ffffff		//palette
};	

//���ɿյ��ڴ�Bmp
void XBmp::CreateBlank(int ColorCnt, int nWidth, int nHeight)
{
	Release();
	
	nWidth= ((nWidth+31)/32)*32;		//4 Byte ����
	
	BmpFileHead BmpHead;
	if (2==ColorCnt)
		BmpHead=BmpCopyC2;
	else if(16==ColorCnt)
		BmpHead=BmpCopyC16;
	else assert(false);;
	
	int	nFileLen=(nWidth*nHeight*BmpHead.bits_per_pixel/8+BmpHead.bitmap_data_offset);
	//allocate memory for Bmp
	m_pBmp= (BmpFileHead*)malloc(nFileLen);
	memset(m_pBmp, 0, nFileLen);
	//fill Bmp head
	memcpy(m_pBmp, &BmpHead, sizeof(BmpHead));
	
	//set Bmp infomation
	BmpFileHead *p= m_pBmp;
	p->bitmap_data_size= nWidth*nHeight*BmpHead.bits_per_pixel/8;
	p->file_size= nFileLen;
	p->width= nWidth;
	p->height= nHeight;
	//record Bmp member var	
	m_nWidth=p->width;
	m_nHeight=p->height;
	m_nPitch=m_nWidth*BmpHead.bits_per_pixel/8;		
	m_pData=p->bitmap_data_offset+ (BYTE*)m_pBmp;
	m_ColorCnt=ColorCnt;
	
	//set palette
	if(16==ColorCnt)
	{
		for(int i=0; i<16; i++)
		{
			m_pBmp->palette[i]=i<<12 | i<<4 | i<<20;	//green 0-15
		}
	}
}

//�Ӵ��̵���Bmp�ļ�
void XBmp::Load(char* sFileName)
{
	Release();
	
	FILE *pFile=fopen(sFileName, "rb");		assert(pFile);
	
	fseek(pFile, 0, SEEK_END);  int nFileLen=ftell(pFile);		//get File length
	m_pBmp= (BmpFileHead*)malloc(nFileLen);
	
	//read file 
	fseek(pFile, 0, SEEK_SET);
	fread(m_pBmp, 1, nFileLen, pFile);
	fclose(pFile);
	
	//set Bmp information
	BmpFileHead *p= m_pBmp;
	m_nWidth=p->width;
	m_nHeight=p->height;
	m_nPitch=m_nWidth* p->bits_per_pixel/8;		
	m_pData=p->bitmap_data_offset+ (BYTE*)m_pBmp;
	m_ColorCnt= 1<<p->bits_per_pixel;
}

//o-------------------------o
//|       XBmpFile			|
//o-------------------------o
XBmpFile::XBmpFile(){
	fp_ = 0;
}

XBmpFile::~XBmpFile(){
	Release();
};

void XBmpFile::Release(){
	if ( fp_ != 0)
	{
		fclose( fp_);
		fp_ = NULL;
	}
};

//��Bmp��λ��x,yȡ����ɫֵ
int XBmpFile::Get(int x, int y){
	if(2==m_ColorCnt){
		//2 color
		int pos = (m_nHeight-1-y)*m_nPitch + x/8;	//ע��Bmpĩ�д�����ǰ�棡
		BYTE b= ReadByte( pos);
		return (b & (0x80>>(x%8)))!=0;
	}else{						
		//16 color
		int pos = (m_nHeight-1-y)*m_nPitch + x/2;
		BYTE b= ReadByte( pos);
		return (x%2)? b & 0x0f : b>>4;
	}
}

BYTE XBmpFile::ReadByte( int pos){
	BYTE b;
	fseek( fp_, pos+dataOffset_, SEEK_SET);
	fread( &b, 1,1, fp_);
	return b;
}


//modify+lincoln
//[2004:12:23] [9:38]
//���ӷ���ֵ������Ӵ��̵���Bmp�ļ�ʧ�ܵ����
HRESULT XBmpFile::Load(char* sFileName)
{
	
	Release();
	
	fp_ = fopen(sFileName, "rb");		
	
	if(!fp_) return S_FALSE;
	
//	fseek(pFile, 0, SEEK_END);  int nFileLen=ftell(pFile);		//get File length
	BmpFileHead* pBmpHead = new BmpFileHead;//(BmpFileHead*)malloc(nFileLen);
	
	//read file 
	//fseek( fp_, 0, SEEK_SET);
	fread( pBmpHead, 1, sizeof(BmpFileHead), fp_);
	//fclose(pFile);
	
	//set Bmp information
	BmpFileHead *p= pBmpHead;
	m_nWidth	= p->width;
	m_nHeight	= p->height;
	m_nPitch	= m_nWidth * p->bits_per_pixel/8;		
	m_ColorCnt	= 1<<p->bits_per_pixel;
	dataOffset_ = p->bitmap_data_offset;

	delete pBmpHead;

	return S_OK;
}