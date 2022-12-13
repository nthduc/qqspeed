#include "Top-KartPCH.h"
//XFontLib.cpp		by czy

#include "XFontLib.h"
#include "XBmp.h"
#include <cassert>

//��Bmp�ļ��л�õ���
void XFontLib::GetHzMatFromBmp( UINT Hz, BYTE* d){
	int nSize= m_FontInf.nSize;
	
	int x0,y0;
	int c0Range=m_FontInf.hzCode->c0Last - m_FontInf.hzCode->c0First+1;		//���� Range
	int c1Range=m_FontInf.hzCode->c1Last - m_FontInf.hzCode->c1First+1;		//λ�� Range
	int nAscLn=256/c1Range+2;	//lines that ascii used 
	
	if (Hz<=0xff){		//Ascii?
		x0=Hz%c1Range * nSize;					//location of Asc
		y0=Hz/c1Range * nSize;
	}else{				// ����
		int c0=Hz/256,  c1=Hz%256;				//����,λ��
		y0=(c0-m_FontInf.hzCode->c0First+nAscLn)*nSize;	//location of Hanzi
		x0=(c1-m_FontInf.hzCode->c1First)*nSize;
	}
	
	for(int y=0; y<nSize; y++)					//get Matrix from Bmp file
		for(int x=0; x<nSize; x++)
			d[y*nSize+x]= m_Bmp.Get(x0+x, y0+y);	
}

void XFontLib::GetHzMat(UINT Hz, BYTE* d){
	assert(Hz>=0 && Hz<65536);
	
	int nSize= m_FontInf.nSize;

	if( cache_[Hz]){		//in cache?
		memcpy( d, cache_[Hz], nSize*nSize);
		return;
	}

	GetHzMatFromBmp( Hz, d);		

	cache_[Hz] = new BYTE[nSize*nSize];
	memcpy( cache_[Hz], d, nSize*nSize);
}

//����ָ����XFont���������Ӧ��Bmp�ļ�
HRESULT XFontLib::Init( XFontInf* pFontInf)
{

	//modify+lincoln
	//[2004:12:23] [10:05]
	//���Ӵ�����
	//{{{
	memset( cache_, 0, sizeof(cache_));

	m_FontInf = *pFontInf;				//record Font information

	return m_Bmp.Load( pFontInf->sFileName);	//load Bmp file
	//}}}

}

//�ͷ��������Bmp�ļ�
void XFontLib::Release(){
	m_Bmp.Release();
	for ( int i=0; i<65536;i++ )
	{
		if(cache_[i])
		{
			delete cache_[i];
			cache_[i] = NULL;
		}
	}
}

//��ȡָ���ַ��� CFontHz �ṹ��ָ��
CFontHz *XFontLib::GetChar( UINT c){
	memset( &fontHz_, 0, sizeof(fontHz_));		//clear all
	fontHz_.ColorCnt = m_Bmp.ColorCnt();			//2color / 16 color
	GetHzMat( c, m_HzMatrix);						//get hanzi metrix

	fontHz_.c = c;
	if ( c<=0xFF)			//Asc
		fontHz_.nextCharOffset= m_FontInf.nSize/2;
	else
		fontHz_.nextCharOffset= m_FontInf.nSize;

	fontHz_.h = m_FontInf.nSize;
	fontHz_.w = m_FontInf.nSize;
	fontHz_.kx = 0;
	fontHz_.ky = 0;
	fontHz_.lpData = m_HzMatrix;

/*		tmp->w=(gm.gmBlackBoxX+3)&0xfffffffc;
		//�ַ�����εĸ߶�
		tmp->h=gm.gmBlackBoxY;
		//�ַ�ԭ����¸��ַ�ԭ��֮���ˮƽ����
		tmp->width=gm.gmCellIncX;
		//�ַ�ԭ���������
		tmp->kx=-gm.gmptGlyphOrigin.x;
		//�ַ�ԭ���������
		tmp->ky=-m_Ascent+gm.gmptGlyphOrigin.y;
*/

	return &fontHz_;
}