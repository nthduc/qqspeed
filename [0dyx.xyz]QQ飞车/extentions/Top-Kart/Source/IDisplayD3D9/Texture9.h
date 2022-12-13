// Texture6.h: interface for the CTexture6 class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TEXTURE6_H__FE17A7C1_506F_4BB8_AFFF_BB04FDE10D46__INCLUDED_)
#define AFX_TEXTURE6_H__FE17A7C1_506F_4BB8_AFFF_BB04FDE10D46__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "d3d9wrapper.h"
#include "HandleMgr.h"

typedef struct _panel_custom {
	float x,y,z;
	float u,v;
}PANEL_CUSTOMVERTEX;

#define PANEL_D3DFVF_CUSTOMVERTEX D3DFVF_XYZ |  D3DFVF_TEX1


typedef struct _panel_custom_t {
	float x,y,z,rhw;
	float u,v;
}PANEL_CUSTOMVERTEX_T;

#define PANEL_D3DFVF_CUSTOMVERTEX_T D3DFVF_XYZRHW |  D3DFVF_TEX1


typedef struct _panel_custom_t_2 {
	float x,y,z,rhw;
	DWORD dwcolor;
}PANEL_CUSTOMVERTEX_T_2;

#define PANEL_D3DFVF_CUSTOMVERTEX_T_2 D3DFVF_XYZRHW | D3DFVF_DIFFUSE


extern void SetVertexShader(DWORD shader);


class CTexture9  
{
public:
	CTexture9();
	virtual ~CTexture9();

public:
	void SetVertex();

	
	
    
	void Render(int left,int top,DWORD dweffect,DWORD dwcolor = 0xffffffff);
	void RenderEx(int left,int top,DWORD dweffect,DWORD dwcolor = 0xffffffff);
	
	int GetTextureWidth(){return m_nTextureWidth;};
	int GetTextureHeight(){return m_nTextureHeight;};
	int GetImageWidth(){return m_nRealWidth;};
	int GetImageHeight(){return m_nRealHeight;};
	
	bool UpdateImage(int dleft,int dtop,int w,int h,TImage* pimage,int sleft,int stop);

	void DrawImageArc(int left,int top,int angle,int rot,HIMG hImg,DWORD effect, DWORD dwColor);

	bool Set(int w,int h,int attrib);
	bool getVMem();
	bool SetUse(bool bTranslate);
	
	void SetCenterPointer(float x,float y);
	void GetCenterPointer(float& x, float& y);


	void SetLeftTop(float left,float top);


	void Release();



	
	//画出贴图的 子矩形
	void	Draw( int x, int y, int w, int h,		//矩形屏幕位置
				int tx, int ty, int tw, int th,DWORD dwcolor);	//矩形贴图位置


//protected:
	void SetTexture();

	//画出贴图的 子矩形
	void Draw( float rect[4][2],					//矩形屏幕位置, 顺时针方向
			   float left, float top, float width, float height);	//矩形贴图位置

	void CoreRender(int left,int top,DWORD dweffect,DWORD dwcolor);
	void SetupVT();
	void CalcTextureAddr();
	bool CheckTextureSize();


	D3DFORMAT                   m_tft;
	LPDIRECT3DTEXTURE9			m_pDT;
	LPDIRECT3DVERTEXBUFFER9		m_pVT;
    bool                        m_btran;
	///
	DWORD                     m_nRealWidth;
	DWORD                     m_nRealHeight;
	DWORD                     m_nTextureWidth;
	DWORD                     m_nTextureHeight;

	float                   m_fTextureCenterX;
	float                   m_fTextureCenterY;

	float                   m_fleft;
	float                   m_ftop;

	int                     m_attrib;
	bool					m_bUsed;
	int                     m_bits;
	float                   m_textaddr[4][2];
	
};

typedef unsigned long HTEX;


class CTexMgr9{
	
	CHandleMgr<CTexture9> handleMgr_;

public:
	
	CTexMgr9()								{}		
	~CTexMgr9()								{}
	void	Release()						{ handleMgr_.Release(); } 

	HTEX	CreateTexture( TImage *pImage,bool bTranslate);
	HTEX	GenTextureHandle( int w, int h, int attrib,bool bTranslate);
	void	ReleaseHandle( HTEX hTex)		{ handleMgr_.ReleaseHandle( hTex); }
	void    SetLeftTop(HTEX hTex,float left,float top);
			
	CTexture9*	GetTexture( HTEX hTex)		{ return handleMgr_.GetItem( hTex); }	
private:
	//int		GetFreeTexture();
};

extern CTexMgr9 texMgr9;


class CImage9{
	
	std::vector<HTEX>	hTexs_;	//真正的显存纹理.过对于大的图片，要切分成多个小图片。
	int		attrib_;			//图像属性
	int		w_, h_;				//图像宽度,高度
	int		wNum_, hNum_;		//列块个数，行块个数，

public:
	float  m_fleft;
	float  m_ftop;

	void SetCenterXY(float x,float y);
	
	CImage9()								{  m_ftop = 0.0f; m_fleft = 0.0f; };
	void	Release();

	void	Set( int w, int h, int attrib,bool bTransform);
	
	void	DrawImage( int left, int top, DWORD effect, DWORD dwColor);
	void    DrawImageEx( int left, int top, DWORD effect, DWORD dwColor);
	void	UpdataImage( int left, int top,int w,int h, TImage *pImage,int sleft,int stop);
	void    DrawImageArc(int left,int top,int angle,int rot,HIMG hImg,DWORD effect, DWORD dwColor);

	int		GetImageWidth()					{ return w_; }
	int		GetImageHeight()				{ return h_; }
	
	void	SnapScreen( int left, int top, int width, int height);
	static	TImage* SnapScreen2( int left, int top, int width, int height);
private:
	int		idx(int i, int j)				{ return wNum_*i + j; }

};

class CImageMgr9{

	CHandleMgr<CImage9> handleMgr_;

public:
	
	CImageMgr9()							{}		
	~CImageMgr9()							{}
	void	Release()						{ handleMgr_.Release(); } 

	HIMG	CreateImage( TImage *pImage,bool bTransform);
	HIMG	CreateImage( TImage *pImage,int left,int top,bool bTransform);
	HIMG	GenImageHandle( int w, int h, int attrib,bool bTransform);
	HIMG	GenImageHandle( int left, int top,int w, int h, int attrib,bool bTransform);
	void	ReleaseHandle( HIMG hImg)		{ handleMgr_.ReleaseHandle( hImg); }
			
	CImage9*	GetImage( HIMG hImg)			{ return handleMgr_.GetItem( hImg); }

	
};

extern CImageMgr9 imageMgr9;

#endif // !defined(AFX_TEXTURE6_H__FE17A7C1_506F_4BB8_AFFF_BB04FDE10D46__INCLUDED_)
