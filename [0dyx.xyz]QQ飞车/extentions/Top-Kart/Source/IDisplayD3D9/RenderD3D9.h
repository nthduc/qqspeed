// RenderD3D6.h: interface for the CRenderD3D6 class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RENDERD3D6_H__1A6287A6_DB4F_4E82_8CD6_3A9EDD4C71A8__INCLUDED_)
#define AFX_RENDERD3D6_H__1A6287A6_DB4F_4E82_8CD6_3A9EDD4C71A8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "d3d9wrapper.h"
//#include "gfx.h"

class CRenderD3D9  
{
public :
	
	HRESULT CreateDisplay();
	HRESULT Release();
	
	HRESULT BeginSence();
	HRESULT EndSence();

	void SetClipRect(int left,int top,int width,int height);
	void EnableClip(bool bEnable);


	HRESULT PrepareState();
	HRESULT RestoreState();
	
	HIMG	SelectImage( TImage *pImage,bool Transform);
	HIMG	SelectImage( TImage *pImage,int left,int top,bool Transform);
	HIMG	GenImageHandle( int w, int h, int attrib,bool Transform);
	void	ReleaseImageHandle( HIMG hImg);


	void	UpdateImage( HIMG hImg, int left, int top, int width,int height,TImage *pImage,int sleft,int stop);

	int		GetImageWidth(HIMG hImg);
	int		GetImageHeight(HIMG hImg);

	
	void	DrawImage(int left, int top, HIMG hImg, DWORD effect=0, DWORD dwColor=0xffffffff);
	void	DrawImageEx(int left, int top, HIMG hImg, DWORD effect=0, DWORD dwColor=0xffffffff);
	
	void	SetImageCenter(float centerX, float centerY);
	void	SetImageScale(float scaleX, float scaleY);
	void	SetImageRotate(float degree);
	void	SetImageCenterEx(HIMG hImg,float centerX,float centerY);

	void    GetImageScale(float & scaleX, float & scaleY);

	void	DrawPixel(int x, int y, DWORD dwColor);
	void	DrawLine(int x1, int y1, int x2, int y2, DWORD dwColor);
	void	DrawPolygon( int numPoints, const float* xy, DWORD dwColor);
	void	DrawPolygon( int numPoints, const float* xy, DWORD* dwColor);
	void	DrawBar(int left, int top, int width, int height, DWORD dwColor);
	void	DrawBox(int left, int top, int width, int height, DWORD dwColor);
	void	DrawImageArc(int left,int top,int angle,int rot,HIMG hImg,DWORD effect, DWORD dwColor);


	HWND    GetHWND(){ return m_hWnd; };
	bool    IsWin()   {return m_bWin; };
	
public :
	void GetClipRect(int& left,int &top,int &width,int &height);
	bool IsClipEnable();
	HRESULT ClearScreen(DWORD dwColor);
	void EnableTextureLinear(bool bEnable);
	bool IsTextureLinear(){return m_bLinear; };

	CRenderD3D9();
	virtual ~CRenderD3D9();
	bool m_bOK;

protected:
	
	void Init2DRenderState();
	HRESULT CreateDirect3D();


	HWND           m_hWnd;
	bool           m_bWin;
	RECT           m_rcScreenRect;
	RECT           m_rcViewportRect;
	bool           m_bLinear;

	ID3DXSprite*    m_Sprite;
};

#endif // !defined(AFX_RENDERD3D6_H__1A6287A6_DB4F_4E82_8CD6_3A9EDD4C71A8__INCLUDED_)
