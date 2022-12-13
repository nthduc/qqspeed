#ifndef D3D6WRAPPER_H

#define D3D6WRAPPER_H


#define DIRECT3D_VERSION         0x0900

#include "d3d9.h"
#include "d3dx9.h"
#include "D3d9types.h"

#include "Display.h"
#include "imagelib.h"

//#pragma  comment(lib,"d3d9.lib")
//#pragma  comment(lib,"d3dx9.lib")
//#pragma comment(lib,"dxguid.lib")
#ifndef _DEBUG
	#pragma comment(lib,"imagelib.lib") 
#endif



class D3D9 {
public :
	enum{
			TF_4444 = 0,
			TF_1555,
			TF_8888,
			TF_NUM,
	};
	
	struct TextureFormatWrapper{
		bool bSupport;
		TextureFormatWrapper() : bSupport(false){};
	}; 
	
	struct TextureSize{
		int w;
		int h;
		TextureSize() : w(0), h(0) {};
	};
	
	static TextureFormatWrapper s_TextureFormat[TF_NUM];
	static TextureSize          s_MaxTextureSize;
	static TextureSize          s_MinTextureSize;

	///æ≤Ã¨±‰¡ø
	static LPDIRECT3DTEXTURE9   s_textture;
	static LPDIRECT3DVERTEXBUFFER9  s_vertexbuffer;
	static D3DCOLOR             s_factor;
	static DWORD                s_effect;
	static DWORD                s_shader;
	static bool                 s_binit;
	static bool                 s_fill;

	static float                s_centerx;
	static float                s_centery;
	static float                s_angle;
	static float                s_scalex;
	static float                s_scaley;

	static int m_Height;
	static int m_Width;

};


template <class T>
void SafeRelease(T& pT)
{
	if(pT)
	{
		pT->Release();
		pT = NULL;
	}
}


extern LPDIRECT3D9				g_pD3D9;
extern LPDIRECT3DDEVICE9		g_pD3DDevice9;
extern int vp;
extern bool						m_bEnable;
extern int						m_clipleft,m_cliptop,m_clipwidth,m_clipheight;

#endif



