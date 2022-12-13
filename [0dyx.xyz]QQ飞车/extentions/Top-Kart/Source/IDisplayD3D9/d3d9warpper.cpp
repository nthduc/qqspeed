#include "Top-KartPCH.h"
#include "stdio.h"
#include "d3d9wrapper.h"

//////////////////////////////////////////////////////////////////////
D3D9::TextureFormatWrapper	D3D9::s_TextureFormat[D3D9::TF_NUM];
D3D9::TextureSize			D3D9::s_MaxTextureSize;
D3D9::TextureSize			D3D9::s_MinTextureSize;
LPDIRECT3DTEXTURE9			D3D9::s_textture = NULL;
LPDIRECT3DVERTEXBUFFER9     D3D9::s_vertexbuffer = NULL;
D3DCOLOR					D3D9::s_factor = 0x00000000;
DWORD						D3D9::s_effect = 0;
DWORD						D3D9::s_shader = 0;
bool                        D3D9::s_fill  = false;
bool                        D3D9::s_binit = false;

float                D3D9::s_centerx = 0.0f;
float                D3D9::s_centery = 0.0f;
float                D3D9::s_angle	 = 0.0f;
float                D3D9::s_scalex  = 1.0f;
float                D3D9::s_scaley  = 1.0f;
int					 D3D9::m_Height = 0;
int					 D3D9::m_Width = 0;

//////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////
LPDIRECT3D9				g_pD3D9 = NULL;
LPDIRECT3DDEVICE9		g_pD3DDevice9 = NULL;
bool					m_bEnable = false;
int						m_clipleft,m_cliptop,m_clipwidth,m_clipheight;

//////////////////////////////////////////////////////////////////////////

