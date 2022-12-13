//关于XFontTex，
//将要输出的字符写到纹理上，返回纹理坐标以供render
//内部缓冲使用链表的方式，根据"最近最少使用”频率动态调整
//缓冲满了以后，会自动将最不常用的字符替换掉
//主要函数GetChar(), 如果不在缓存中，将使用FontLib得到字符点阵

#ifndef XFONTTEX_H
#define XFONTTEX_H

#include <windows.h>
#include "XFontLib.h"

//可存放一个汉字的纹理信息
struct XFontNode{
	unsigned int c;		//type+char
	DWORD	left,top;	//在纹理中的起点坐标
	DWORD	w,h;		//w、h
	XFontNode	*next, *prev;	//a Loop link list with a False Head, ;	//双向链表:
};

class XFontTex{
public:
	XFontTex():	pTexture_(0)			{};
	~XFontTex()							{ Release(); };

	//初始化: mult same size font can use 1 fontTex
	//  pFontInf:	汉字库信息
	//	lpds:		使用的纹理Buffer
	//	TexHigce:   纹理buffer大小
	HRESULT	Init( int texSize, int hzSize);
	//HRESULT	Init( void* pTexture, DWORD texSize, int hzSize);
	
	void Release();		//释放缓冲链表

	//获得字符c的信息
	XFontNode*	GetChar( XFontLib* pFontLib,		//使用的汉字库
						 unsigned int c,
						 int fontType);

	//reset cache
	void ClearCache();
	void* getTex()			{ return pTexture_;	}

private:
	void	Move2Head( XFontNode* cur);
private:
	void*	pTexture_;				//texture	保存d3d 或 openGL 的贴图
	
	DWORD	blockSize_;				//块大小(每块存放一个字符。目前块大小＝汉字大小。将来有可能大于汉字）
	DWORD	texSize_;				//纹理大小

	XFontNode	*m_List;			//缓冲链表: 带伪头节点的循环双链表
};

class XFontTexSet{
public:
	XFontTex*	getFontTex( int hzSize, int& texWid, int& texHei);
	void Release(){fontTex10_.Release(); fontTex12_.Release(); fontTex16_.Release(); fontTex24_.Release();};
private:
	XFontTex fontTex10_;
	XFontTex fontTex12_;
	XFontTex fontTex16_;
	XFontTex fontTex24_;
};

extern XFontTexSet fontTexSet;

//必须在别的地方实现这个函数。
void DrawChar2Tex( void* pTexture,DWORD x,DWORD y,WORD color,CFontHz *zfontdata);
void* CreateFontTex( int texSize);

#endif