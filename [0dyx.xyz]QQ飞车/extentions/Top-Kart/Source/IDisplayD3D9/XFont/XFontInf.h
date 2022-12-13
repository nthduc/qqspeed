//XFontInf.h	by	czy
//描述汉字库信息结构，使用Bmp文件存储汉字字形。

#ifndef XFONTINF_H
#define XFONTINF_H

struct XCharSet {
	unsigned long	charSetID; 
	//汉字由2个字节组成
	unsigned char	c0First,c0Last;		//第1字节的起始值和终止值
	unsigned char	c1First,c1Last;		//第2字节的起始值和终止值
};

//汉字库信息结构
struct XFontInf{		
	char*	sFileName;		//Bmp文件名
	int		nSize;			//汉字点阵的宽度和高度
	XCharSet*	hzCode;		//汉字编码方式：GB2312,GBK, Unicode,Big5...
	//汉字由2个字节组成
//	unsigned char	c0First,c0Last;		//第1字节的起始值和终止值
//	unsigned char	c1First,c1Last;		//第2字节的起始值和终止值
};

/*extern XFontInf FontInf24;
extern XFontInf FontInf16;
extern XFontInf FontInf12;
*/
extern XFontInf GBK10, GBK12,	GBK16,	GBK24;
extern XFontInf GBK10Y, GBK12Y, GBK16Y, GBK24Y;


#endif


