//:XFontLib.h		by czy
//Bmp汉字库
#ifndef XFONTLIB_H
#define XFONTLIB_H

#include <windows.h>
#include "XBmp.h"
#include "XFontInf.h"

//保存某字符的位图和其它信息
struct CFontHz{
	UINT	c;							//字符的ASCII码(包括双字节)
	int		ColorCnt;					//2 or 16
	BYTE	*lpData;					//字符位图的指针
//	DWORD	dwSize;						//字符位图空间大小
	DWORD	w,h;						//字符位图的宽度,高度
	int		nextCharOffset;				//下一个字符的偏移
	DWORD   kx,ky;						//字符位图的原点之间的距离,原点的X,Y坐标
};

class XFontLib{
public:
	XFontLib()	{};
	~XFontLib()	{	Release();	};

	//根据指定的XFont字体调入相应的Bmp文件
	HRESULT Init( XFontInf* pFontInf);
	void Release();

	//获取指定字符的位图信息
	CFontHz *GetChar(UINT c);
private:
	//释放字体点阵Bmp文件


	//从Bmp文件中获得点阵
	void GetHzMat(  UINT Hz,			//	Hz:	汉字
					BYTE* d);			//	d:	正方形点阵，	
	void GetHzMatFromBmp( UINT Hz, BYTE* d);

	XFontInf	m_FontInf;			//保存Bmp字库信息
//	XBmp		m_Bmp;				//保存Bmp字库的内存Bmp文件
	XBmpFile	m_Bmp;				//改为使用保存Bmp字库的外存Bmp文件!!!
	
	CFontHz		fontHz_;			//为指定字符生成的位图信息
	BYTE		m_HzMatrix[32*32];	//为指定字符生成的汉字点阵, 最大不要超过32*32

	BYTE*		cache_[65536];		//save Hanzi Mat which is used
};

#endif