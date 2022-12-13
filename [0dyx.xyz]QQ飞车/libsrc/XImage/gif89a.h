/*
**  gif89a.h
**
**  
*/
#ifndef _GIF89A_H_
#define _GIF89A_H_

#include <stdio.h>
//#include <fstream.h>
#include <windows.h>

//add+lincoln
//[2004:11:8] [12:44]
#define _VIRTUAL_FILE_  

#ifdef _VIRTUAL_FILE_ 

#include "kernel.h"
#include "vfilewrap.h"

#define FILEPT		FILE_V
#define FILEOPEN	vfopen
#define FILECLOSE	vfclose
#define FILESEEK	vfseek
#define FILETELL	vftell
#define FILEREAD	vfread
#define FILEWRITE   vfwrite
#define FILEPRINTF  vfprintf
#define FILEEOF		vfeof
#else
#define FILEPT		FILE
#define FILEOPEN	fopen
#define FILECLOSE	fclose
#define FILESEEK	fseek
#define FILETELL	ftell
#define FILEREAD	fread
#define FILEWRITE   fwrite
#define FILEPRINTF  fprintf
#define FILEEOF		feof
#endif

typedef struct               //ͼ����չ����
{    
    BOOL active;             //���ṹ�е����������Ƿ����
    UINT disposalMethod;     //���������ɺ��ԣ�
    BOOL userInputFlag;      //�Ƿ��ڴ��û�����
    BOOL trsFlag;            //�Ƿ���͸��ɫ
    WORD delayTime;          //��ʱʱ�䣨��λ1/100�룩
    UINT trsColorIndex;      //͸��ɫ��ɫ������
}GCTRLEXT;

typedef struct                //һ֡ͼ��Ĳ���
{    
    WORD imageLPos;          //ͼ������ص��߼���Ļ�ľ��루��λ���أ�
    WORD imageTPos;          //ͼ���ϱ��ص��߼���Ļ�ľ��루��λ���أ�
    WORD imageWidth;         //ͼ��Ŀ�ȣ���λ���أ�
    WORD imageHeight;        //ͼ��ĸ߶ȣ���λ���أ�
    BOOL lFlag;              //�Ƿ��оֲ���ɫ�壨����������ɫ������Ƿ���Ч��
    BOOL interlaceFlag;      //ͼ�������Ƿ񽻴�
    BOOL sortFlag;           //�ֲ���ɫ�������Ƿ���������
    UINT lSize;              //�ֲ���ɫ���С���ж��ٸ�ʵ����ڣ�
    BYTE *pColorTable;       //ָ��ֲ���ɫ���ָ�루256����ڣ�ÿ��������ֽڣ�
    BYTE *dataBuf;           //ͼ������ָ��
    GCTRLEXT ctrlExt;        //ͼ����չ��������͸�������Ͷ����йأ�
}FRAME;
typedef FRAME *LPFRAME;
typedef const FRAME *LPCFRAME;

typedef struct                //GIF�ļ���ȫ�ֲ���
{    
    UINT frames;             //�ļ���ͼ��֡��
    WORD scrWidth,scrHeight; //�߼���Ļ�Ŀ�Ⱥ͸߶ȣ���λ���أ�
    BOOL gFlag;              //�Ƿ���ȫ�ֵ�ɫ�壨����������ɫ������Ƿ���Ч��
    UINT colorRes;           //ɫ�ʷֱ��ʣ���ʹ�ã�
    BOOL gSort;              //ȫ�ֵ�ɫ���Ƿ���������
    UINT gSize;              //ȫ�ֵ�ɫ���С���ж��ٸ�ʵ����ڣ�
    UINT BKColorIdx;         //����ɫ�ĵ�ɫ������
    UINT pixelAspectRatio;   //���س������
    BYTE *gColorTable;       //ָ��ȫ�ֵ�ɫ���ָ�루256����ڣ�ÿ��������ֽڣ�
} GLOBAL_INFO;
typedef GLOBAL_INFO *LPGLOBAL_INFO;
typedef const GLOBAL_INFO *LPCGLOBAL_INFO;

typedef struct
{	UINT len;
	unsigned char* p;
} STRING_TABLE_ENTRY;

class CGif89a
{
private:
	//ifstream ifs;               //���ڶ��ļ����ļ���
	//streampos dataStart;        //�����ļ�����ͼ�����ݿ�ʼ�ĵط�
	FILEPT* fp;
	int  dataStart;
	//
	char version[4];            //�汾�ַ���
	BOOL error;                 //��ʵ����������ʱ�Ƿ����ı�־
	BOOL opened;                //�Ƿ��ڴ�״̬
	BOOL inMem;                 //ͼ�������Ƿ�һ�ζ����ڴ�
	BYTE gColorTable[256*3];    //ȫ�ֵ�ɫ��
	BYTE lColorTable[256*3];    //�ֲ���ɫ�壨inMemΪ��ʱ�ã�
	FRAME *allFrames;           //ָ������ͼ��֡��ָ�루inMemΪ��ʱ�ã�
	UINT curIndex;              //��ǰ֡��������inMemΪ��ʱ�ã�
private:
	//UINT checkFrames(ifstream&);
	//BOOL getAllFrames(ifstream&);
	//BOOL extractData(FRAME* f,ifstream&);
	UINT checkFrames(void);
	BOOL getAllFrames(void);
	BOOL extractData(FRAME* f);

	BOOL initStrTable(STRING_TABLE_ENTRY* strTable,UINT rootSize);
	BOOL addStrTable(STRING_TABLE_ENTRY* strTable,UINT addIdx,UINT idx,unsigned char c);
private:
	GLOBAL_INFO gInfo;          //GIF�ļ���ȫ�ֲ���
	FRAME curFrame;             //��ǰ֡�Ĳ�����inMemΪ��ʱ�ã�
	GCTRLEXT ctrlExt;           //ͼ����չ��������������ʱ��ʱʹ�ã�
public:
	CGif89a();
	CGif89a(LPCSTR fileName,BOOL inMem);
	~CGif89a();
	BOOL operator!();
	BOOL open(LPCSTR fileName,BOOL inMem);
	void close();
	LPCSTR getVer();
	LPCFRAME getNextFrame();
	LPCGLOBAL_INFO getGlobalInfo();
};

#endif//_GIF89A_H_

