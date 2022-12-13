#include "Top-KartPCH.h"
//XFontLib.cpp		by czy

#include "XFontLib.h"
#include "XBmp.h"
#include <cassert>

//从Bmp文件中获得点阵
void XFontLib::GetHzMatFromBmp( UINT Hz, BYTE* d){
	int nSize= m_FontInf.nSize;
	
	int x0,y0;
	int c0Range=m_FontInf.hzCode->c0Last - m_FontInf.hzCode->c0First+1;		//区码 Range
	int c1Range=m_FontInf.hzCode->c1Last - m_FontInf.hzCode->c1First+1;		//位码 Range
	int nAscLn=256/c1Range+2;	//lines that ascii used 
	
	if (Hz<=0xff){		//Ascii?
		x0=Hz%c1Range * nSize;					//location of Asc
		y0=Hz/c1Range * nSize;
	}else{				// 汉字
		int c0=Hz/256,  c1=Hz%256;				//区码,位码
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

//根据指定的XFont字体调入相应的Bmp文件
HRESULT XFontLib::Init( XFontInf* pFontInf)
{

	//modify+lincoln
	//[2004:12:23] [10:05]
	//增加错误处理
	//{{{
	memset( cache_, 0, sizeof(cache_));

	m_FontInf = *pFontInf;				//record Font information

	return m_Bmp.Load( pFontInf->sFileName);	//load Bmp file
	//}}}

}

//释放字体点阵Bmp文件
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

//获取指定字符的 CFontHz 结构的指针
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
		//字符外矩形的高度
		tmp->h=gm.gmBlackBoxY;
		//字符原点和下个字符原点之间的水平距离
		tmp->width=gm.gmCellIncX;
		//字符原点的左坐标
		tmp->kx=-gm.gmptGlyphOrigin.x;
		//字符原点的上坐标
		tmp->ky=-m_Ascent+gm.gmptGlyphOrigin.y;
*/

	return &fontHz_;
}