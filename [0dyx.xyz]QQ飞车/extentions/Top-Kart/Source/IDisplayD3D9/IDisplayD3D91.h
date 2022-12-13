// IDisplayD3D61.h: interface for the IDisplayD3D6 class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_IDISPLAYD3D61_H__87FF3FE7_91B4_420C_8DD2_A82A61DA6EF7__INCLUDED_)
#define AFX_IDISPLAYD3D61_H__87FF3FE7_91B4_420C_8DD2_A82A61DA6EF7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Display.h"
#include "RenderD3D9.h"
#include "XFont\\XFontGl.h"
#include <stack>

#define ARGB(a,r,g,b)		((DWORD)((((a)&0xff)<<24)|(((r)&0xff)<<16)|(((g)&0xff)<<8)|((b)&0xff)))


struct stClipState
{
	int  ox;
	int  oy;
	int  width;
	int  height;
	bool enable;

	stClipState()
	{
		ox=0;
		oy=0;
		width=0;
		height=0;
		enable=false;
	}

	stClipState(int x,int y,int w,int h,bool flag=false)
	{
		ox=x;
		oy=y;
		width=w;
		height=h;
		enable=flag;
	}

};

class IDisplayD3D9 : public IDisplay  
{
public:
	IDisplayD3D9();
	virtual ~IDisplayD3D9();

public:
	//Status function
	virtual BOOL	IsWindowed()					{ return _d3d9.IsWin() ? TRUE : FALSE;};
	virtual WORD	GetDisplayWidth()				{ return D3D9::m_Width;};
	virtual WORD	GetDisplayHeight()				{ return D3D9::m_Height;};
	
	//Creation/destruction methods
	virtual HRESULT	CreateFullScreenDisplay(HWND hWnd, int nScreenType);
	virtual HRESULT CreateWindowedDisplay(HWND hWnd, int nScreenType);
	virtual HRESULT	UpdateBounds();
	virtual HRESULT	DestroyObjects();
	virtual HRESULT	RestoreAllSurfaces();

	//Display methods
	virtual HRESULT	ClearScreen(DWORD dwColor);
	virtual HRESULT	BeginScreen();
	virtual HRESULT	UpdateScreen();

	//add+lincoln
	//[2005:1:18] [16:34]
	//增加裁减状态堆栈
	//{{{
	//add+lincoln
	//[2005:1:18] [16:56]
	stClipState GetClipState();
	void	SetClipState( stClipState state);

	//接口
	virtual void	PushClipState();
	virtual void	PopClipState();
	virtual void	PushClipRect(int left, int top, int width, int height);
	//}}}


	//Image utilities
public:
	virtual HIMG	SelectImage( TImage *pImage);
	virtual HIMG	GenImageHandle( int w, int h, int attrib);
	virtual void	ReleaseImageHandle( HIMG hImg);

//-	virtual void	DrawImage( int left, int top, HIMG hImg);
	virtual void	UpdateImage( HIMG hImg, int left, int top, TImage *pImage);

		//mike, added [11/27/2003]
	virtual int		GetImageWidth(HIMG hImg);
	virtual int		GetImageHeight(HIMG hImg);
//-	virtual void	DrawImageColor(int left, int top, HIMG hImg, DWORD dwColor);

	//czy, added [12/16/2003]
	//image特效
	virtual void	DrawImage( int left, int top, HIMG hImg, DWORD effect=0, DWORD dwColor=0xffffffff);
	virtual void	SetImageCenter( float centerX, float centerY);
	virtual void	SetImageScale( float scaleX, float scaleY);
	virtual void	SetImageRotate( float degree);

	//czy, added [3/8/2004]
	virtual bool	EnableTexLinear( bool flag);
	//czy, added [1/8/2004]
	virtual HIMG	SnapScreen( int left, int top, int width, int height);
	virtual TImage* SnapScreen2( int left, int top, int width, int height);
	
	virtual void	SetClipRect( int left, int top, int width, int height);
	virtual void	GetClipRect( int&left, int&top, int&width, int&height);
	
	virtual void	EnableClipRect( bool flag);	
	virtual bool	IsClipEnable();	



	virtual void	SetMaxTex( int size);
public:
	virtual void	DrawPixel(int x, int y, DWORD dwColor);
	virtual void	DrawLine(int x1, int y1, int x2, int y2, DWORD dwColor);
	virtual void	DrawPolygon( int numPoints, const float* xy, DWORD dwColor);
	virtual void	DrawPolygon( int numPoints, const float* xy, DWORD* dwColor);
	virtual void	DrawBar(int left, int top, int width, int height, DWORD dwColor);
	virtual void	DrawBox(int left, int top, int width, int height, DWORD dwColor);
	virtual void    DrawImageArc(int left,int top,int angle,int rot,HIMG hImg,DWORD effect = 0,DWORD dwColor=0xffffffff);
public:
	//size为 12, 16, 24时显示效果较好. 显示单行
	virtual void	DrawText( int sx, int sy, const char* strText, DWORD dwColor, int size,UINT nFlag = 0);
	//czy, added [12/16/2003]
	virtual void	DrawTextEx( int left, int top, int width, int height, 
								const char* str, int size, int space, DWORD dwColor, UINT flags);
	
	virtual void	DrawCtrlText( int left, int top, int width, int height, 
								  const char* str, int size, int space, UINT flags);
	virtual DWORD	SetCtrlTextColor( int colorIdx, DWORD dwColor);

	virtual int		SetFont( int fontIdx);
	virtual void	ClearCtrl( const char* ctrlStr, char* result);
	
	virtual int		EstimateLine( const char* str, int width, int size, int space);
	virtual void	EstimateLine( const char* str, int size, int space, int& width, int& height);
	//设置行距，并返回设置前的行距。缺省行距为0。
	//virtual int		SetRowSpace( int);
	//virtual int		GetRowSpace();

	virtual DWORD   GetCtrlTextColor( int colorIdx );
	virtual void    DrawPixels( int ox, int oy, TImage *pimage );
    
    HIMG    GenRotateImageHandle(TImage *pImage, int nArf, int nCenterx=0, int nCentery=0){return 0;}
    HIMG    GenScaleImageHandle(TImage *pImage, float fscalex, float fscaley){return 0;}
	
	bool    SaveGsoFile(HIMG hImg, char* szFileName){return false;};
private:
	XFontD3D		font10_[2]; //font 10x10
    XFontD3D		font12_[2];	//font 12x12
    XFontD3D		font16_[2];	//font 16x16
	XFontD3D		font24_[2];	//font 24x24
	CRenderD3D9		_d3d9;	
	
	DWORD	ctrlTextColor_[MAX_TEXT_COLOR];
	int		fontType_;


	//add+lincoln
	//[2005:1:18] [16:34]
	//增加裁减状态堆栈
	//{{{
	//add+lincoln
	//[2005:1:18] [16:56]	
	std::stack<stClipState> m_ClipMatrix;
	//}}}

};

extern	IDisplayD3D9* display3D_;			// display device

#endif // !defined(AFX_IDISPLAYD3D61_H__87FF3FE7_91B4_420C_8DD2_A82A61DA6EF7__INCLUDED_)
