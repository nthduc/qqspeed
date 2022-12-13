//����XFontTex��
//��Ҫ������ַ�д�������ϣ��������������Թ�render
//�ڲ�����ʹ������ķ�ʽ������"�������ʹ�á�Ƶ�ʶ�̬����
//���������Ժ󣬻��Զ�������õ��ַ��滻��
//��Ҫ����GetChar(), ������ڻ����У���ʹ��FontLib�õ��ַ�����

#ifndef XFONTTEX_H
#define XFONTTEX_H

#include <windows.h>
#include "XFontLib.h"

//�ɴ��һ�����ֵ�������Ϣ
struct XFontNode{
	unsigned int c;		//type+char
	DWORD	left,top;	//�������е��������
	DWORD	w,h;		//w��h
	XFontNode	*next, *prev;	//a Loop link list with a False Head, ;	//˫������:
};

class XFontTex{
public:
	XFontTex():	pTexture_(0)			{};
	~XFontTex()							{ Release(); };

	//��ʼ��: mult same size font can use 1 fontTex
	//  pFontInf:	���ֿ���Ϣ
	//	lpds:		ʹ�õ�����Buffer
	//	TexHigce:   ����buffer��С
	HRESULT	Init( int texSize, int hzSize);
	//HRESULT	Init( void* pTexture, DWORD texSize, int hzSize);
	
	void Release();		//�ͷŻ�������

	//����ַ�c����Ϣ
	XFontNode*	GetChar( XFontLib* pFontLib,		//ʹ�õĺ��ֿ�
						 unsigned int c,
						 int fontType);

	//reset cache
	void ClearCache();
	void* getTex()			{ return pTexture_;	}

private:
	void	Move2Head( XFontNode* cur);
private:
	void*	pTexture_;				//texture	����d3d �� openGL ����ͼ
	
	DWORD	blockSize_;				//���С(ÿ����һ���ַ���Ŀǰ���С�����ִ�С�������п��ܴ��ں��֣�
	DWORD	texSize_;				//�����С

	XFontNode	*m_List;			//��������: ��αͷ�ڵ��ѭ��˫����
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

//�����ڱ�ĵط�ʵ�����������
void DrawChar2Tex( void* pTexture,DWORD x,DWORD y,WORD color,CFontHz *zfontdata);
void* CreateFontTex( int texSize);

#endif