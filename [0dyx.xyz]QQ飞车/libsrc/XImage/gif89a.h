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

typedef struct               //图象扩展参数
{    
    BOOL active;             //本结构中的其它参数是否可用
    UINT disposalMethod;     //处理方法（可忽略）
    BOOL userInputFlag;      //是否期待用户输入
    BOOL trsFlag;            //是否有透明色
    WORD delayTime;          //延时时间（单位1/100秒）
    UINT trsColorIndex;      //透明色调色板索引
}GCTRLEXT;

typedef struct                //一帧图象的参数
{    
    WORD imageLPos;          //图象左边沿到逻辑屏幕的距离（单位像素）
    WORD imageTPos;          //图象上边沿到逻辑屏幕的距离（单位像素）
    WORD imageWidth;         //图象的宽度（单位像素）
    WORD imageHeight;        //图象的高度（单位像素）
    BOOL lFlag;              //是否有局部调色板（决定其他调色板参数是否有效）
    BOOL interlaceFlag;      //图象数据是否交错
    BOOL sortFlag;           //局部调色板数据是否按优先排序
    UINT lSize;              //局部调色板大小（有多少个实际入口）
    BYTE *pColorTable;       //指向局部调色板的指针（256个入口，每个入口三字节）
    BYTE *dataBuf;           //图象数据指针
    GCTRLEXT ctrlExt;        //图象扩展参数（与透明背景和动画有关）
}FRAME;
typedef FRAME *LPFRAME;
typedef const FRAME *LPCFRAME;

typedef struct                //GIF文件的全局参数
{    
    UINT frames;             //文件中图象帧数
    WORD scrWidth,scrHeight; //逻辑屏幕的宽度和高度（单位像素）
    BOOL gFlag;              //是否有全局调色板（决定其他调色板参数是否有效）
    UINT colorRes;           //色彩分辨率（不使用）
    BOOL gSort;              //全局调色板是否按优先排序
    UINT gSize;              //全局调色板大小（有多少个实际入口）
    UINT BKColorIdx;         //背景色的调色板索引
    UINT pixelAspectRatio;   //像素长宽比例
    BYTE *gColorTable;       //指向全局调色板的指针（256个入口，每个入口三字节）
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
	//ifstream ifs;               //用于读文件的文件流
	//streampos dataStart;        //保存文件流中图象数据开始的地方
	FILEPT* fp;
	int  dataStart;
	//
	char version[4];            //版本字符串
	BOOL error;                 //类实例变量创建时是否出错的标志
	BOOL opened;                //是否处于打开状态
	BOOL inMem;                 //图象数据是否一次读入内存
	BYTE gColorTable[256*3];    //全局调色板
	BYTE lColorTable[256*3];    //局部调色板（inMem为假时用）
	FRAME *allFrames;           //指向所有图象帧的指针（inMem为真时用）
	UINT curIndex;              //当前帧的索引（inMem为真时用）
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
	GLOBAL_INFO gInfo;          //GIF文件的全局参数
	FRAME curFrame;             //当前帧的参数（inMem为假时用）
	GCTRLEXT ctrlExt;           //图象扩展参数（读入数据时临时使用）
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

