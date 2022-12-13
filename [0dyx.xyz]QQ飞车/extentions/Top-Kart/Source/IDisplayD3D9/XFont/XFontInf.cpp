//XFontInf.cpp		by	czy
//描述汉字库信息结构，使用Bmp文件存储汉字字形。
//#include	"StdAfx.h"
#include "..\Engine.h"
#include "xfontInf.h"
//o---------------------o
//|		hz char set		|
//o---------------------o
XCharSet GB2312 = {
	GB2312_CHARSET,
		0xa1,0xfe,
		0xa1,0xfe
};

XCharSet GBKcode = {
	GB2312_CHARSET,
		0x81,0xfe,
		0x40,0xfe
};

//GBK 点阵字库信息, 明流字体
XFontInf GBK10  =   {	"res\\GBK10.bmp",	10,	&GBKcode};	//12*10 点阵字库信息
XFontInf GBK12	=	{	"res\\GBK12.bmp",	12,	&GBKcode};	//12*12 点阵字库信息
XFontInf GBK16	=	{	"res\\GBK16.bmp",	16, &GBKcode};	//16*16 点阵字库信息
XFontInf GBK24	=	{	"res\\GBK24.bmp",	24,	&GBKcode};	//24*24 点阵字库信息

//GBK 点阵字库信息, 方正粗圆字体
XFontInf GBK10Y =   {	"res\\GBK10Y.bmp",	10,	&GBKcode};	//10*10 点阵字库信息
XFontInf GBK12Y	=	{	"res\\GBK12Y.bmp",	12,	&GBKcode};	//12*12 点阵字库信息
XFontInf GBK16Y	=	{	"res\\GBK16Y.bmp",	16, &GBKcode};	//16*16 点阵字库信息
XFontInf GBK24Y	=	{	"res\\GBK24Y.bmp",	24,	&GBKcode};	//24*24 点阵字库信息

//GB2312 点阵字库信息
//XFontInf FontInf12={	"res\\HZK12.bmp",	12,	&GB2312};	//12*12 点阵字库信息
XFontInf GB12	=	{	"res\\HZK12.bmp",	12,	&GB2312};	//12*12 点阵字库信息
XFontInf GB16	=	{	"res\\HZK16.bmp",	16,	&GB2312};	//16*16 点阵字库信息
XFontInf GB24	=	{	"res\\HZK24.bmp",	24,	&GB2312};	//24*24 点阵字库信息

//24*24 宋体点阵字库信息	目前不用
XFontInf FontInf24S={	"HZK24S.bmp",		24,	&GB2312};

