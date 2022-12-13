//:Bmp.h		by czy
//Bmp文件的内存映像，作为汉字库使用	支持黑白和16色（灰度）模式，16色用于反锯齿


#ifndef XBMP_H
#define XBMP_H
#include "windows.h"
#include <cstdio>

#pragma pack( push, 1 )
struct BmpFileHead 
{		//Bmp文件头
	char	identifier[2];
	DWORD	file_size;
	DWORD	reserved;
	DWORD	bitmap_data_offset;
	DWORD	bitmap_header_size;
	DWORD	width;
	DWORD	height;
	WORD	planes;
	WORD	bits_per_pixel;
	DWORD	compression;
	DWORD	bitmap_data_size;
	DWORD	hresolution;
	DWORD	vresolution;
	DWORD	colors;
	DWORD	important_colors;
	DWORD	palette[2];
	//bitmap data
};
#pragma pack( pop )

//Bmp文件的内存映像，作为汉字库使用	支持黑白和16色（灰度）模式，16色用于反锯齿
class XBmp
{
	int m_ColorCnt;					//色数
	int m_nWidth, m_nHeight;		//宽度，
	int m_nPitch;

	BYTE *m_pData;					//颜色数据
	BmpFileHead *m_pBmp;			//文件头

public:
	XBmp();
	~XBmp();

	void Release();

	//从磁盘调入Bmp文件
	void Load(char* sFileName);		

	//把Bmp文件存入磁盘文件
	void Save(char* sFileName);		

	//生成空的内存Bmp
	void CreateBlank(int ColorCnt, int nWidth, int nHeight);	

	int	 FileLen(){ return m_pBmp->file_size; }

	//2色(黑白)  或  16色（灰度）
	int	 ColorCnt(){ return m_ColorCnt; }

	int	 Width(){ return m_nWidth; }

	int	 Height(){ return m_nHeight; }

	//将Bmp的位置x,y设为nColor
	void Set(int x, int y, int nColor);		

	//从Bmp的位置x,y取出颜色值
	int  Get(int x, int y);
};

//Bmp文件的内存映像，作为汉字库使用	支持黑白和16色（灰度）模式，16色用于反锯齿
class XBmpFile
{
	int m_ColorCnt;					//色数
	int m_nWidth, m_nHeight;		//宽度，
	int m_nPitch;
	
//	BYTE *m_pData;					//颜色数据
//	BmpFileHead *m_pBmp;			//文件头
	int dataOffset_;
	FILE* fp_;						//文件句柄，用于读取
	
	BYTE	ReadByte( int pos);

public:
	XBmpFile();
	~XBmpFile();
	void Release();
	
	//modify+lincoln
	//[2004:12:23] [9:38]
	//增加返回值，处理从磁盘调入Bmp文件失败的情况
	//{{{
	HRESULT Load(char* sFileName);		
	//}}}
	
	//生成空的内存Bmp
//	void CreateBlank(int ColorCnt, int nWidth, int nHeight);	
	
//	int	 FileLen(){ return m_pBmp->file_size; }
	//2色(黑白)  或  16色（灰度）
	int	 ColorCnt(){ return m_ColorCnt; }
	int	 Width(){ return m_nWidth; }
	int	 Height(){ return m_nHeight; }
	
	//从Bmp的位置x,y取出颜色值
	int  Get(int x, int y);
};
#endif
