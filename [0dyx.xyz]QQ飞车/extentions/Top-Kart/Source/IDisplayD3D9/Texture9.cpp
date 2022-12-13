// Texture6.cpp: implementation of the CTexture6 class.
//
//////////////////////////////////////////////////////////////////////

#include "Top-KartPCH.h"
#include "Texture9.h"
#include "stdlib.h"
#include "math.h"
#include "IDisplayD3D91.h"
#include "masmlib.h"

#pragma comment(lib,"masmlib.lib")
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

#define _RGB16BIT565(r,g,b) ((b & 31) + ((g & 63) << 6) + ((r & 31) << 11))

extern float g_fXUIScale;
extern float g_fYUIScale;



static PANEL_CUSTOMVERTEX_T      m_vector[4];


namespace NImageMgr{

class CRect2{
public:
	CRect2()									{}
	CRect2(int ix, int iy, int iw, int ih)	{ Set( ix, iy, iw, ih); }

	int x, y, w, h;
	
	void	Set( int ix, int iy, int iw, int ih)	{ x = ix; y = iy; w = iw; h = ih; }
	CRect2	and( const CRect2& rect);
	bool	isEmpty()						{ return (0 == w) || (0 == h); }
	bool    IsPtInRect(int x,int y);
	bool    IsRectInRect(const CRect2& r);
	bool    IsQu(const CRect2& r);
};

bool CRect2::IsQu(const CRect2& r)
{
	if((r.x == x) && (r.y == y) && (r.w == w) && (r.h == h))
	{
		return true;
	}

	return false;
}

bool CRect2::IsPtInRect(int _x,int _y)
{
	if((_x >= x)&&(_y >= y)&&(_x <= x + w)&&( _y <= y + h))
	{
		return true;
	}

	return false;
}

bool CRect2::IsRectInRect(const CRect2& r)
{
	CRect2 temp;
	temp = and(r);

	if(temp.IsQu(r))
	{
		return true;
	}

	return false;
}


#ifndef max
#define max(a,b)            (((a) > (b)) ? (a) : (b))
#endif

#ifndef min
#define min(a,b)            (((a) < (b)) ? (a) : (b))
#endif

CRect2 CRect2::and( const CRect2& rect){
	CRect2 r( 0,0,0,0);
	int xStart	= max( x, rect.x);
	int xEnd	= min( x+w, rect.x+rect.w);
	if ( xStart>= xEnd)
		return r;
	
	int yStart	= max( y, rect.y);
	int yEnd	= min( y+h, rect.y+rect.h);
	if ( yStart>= yEnd)
		return r;
	
	r.Set( xStart, yStart, xEnd-xStart, yEnd-yStart);
	return r; 
}

};

using namespace NImageMgr;


static void SetColor(DWORD dwcolor)
{
	static DWORD alphaMask = 0xff000000;
	NiDX9Renderer* pD3D9 = NiDynamicCast(NiDX9Renderer,NiRenderer::GetRenderer());
	NiDX9RenderState* pRenderState = pD3D9->GetRenderState();
	
	pRenderState->SetRenderState(D3DRS_TEXTUREFACTOR,dwcolor);
				
	if(dwcolor != 0xffffffff)
	{									
		pRenderState->SetTextureStageState(0,D3DTSS_COLORARG1,D3DTA_TEXTURE);
		pRenderState->SetTextureStageState(0,D3DTSS_COLORARG2,D3DTA_TFACTOR);
		pRenderState->SetTextureStageState(0,D3DTSS_COLOROP,D3DTOP_MODULATE);
	}
	else
	{
	    pRenderState->SetTextureStageState(0,D3DTSS_COLORARG1,D3DTA_TEXTURE);
		pRenderState->SetTextureStageState(0,D3DTSS_COLOROP,D3DTOP_SELECTARG1);
	}
			
	if((dwcolor & alphaMask) != alphaMask)
	{
		pRenderState->SetTextureStageState(0,D3DTSS_ALPHAARG1,D3DTA_TEXTURE);
		pRenderState->SetTextureStageState(0,D3DTSS_ALPHAARG2,D3DTA_TFACTOR);
		pRenderState->SetTextureStageState(0,D3DTSS_ALPHAOP,D3DTOP_MODULATE);					
	}
	else
	{
		pRenderState->SetTextureStageState(0,D3DTSS_ALPHAARG1,D3DTA_TEXTURE);
	    pRenderState->SetTextureStageState(0,D3DTSS_ALPHAOP,D3DTOP_SELECTARG1);			
	}
				
	D3D9::s_factor = dwcolor;
				
}

static void SetUpRenderState(DWORD dweffect,DWORD dwcolor)
{
	dweffect &= ~fxGeoScale;
	dweffect &= ~fxGeoRotate;
	dweffect &= ~fxGeoMirrorX;
	dweffect &= ~fxGeoMirrorY;


	NiDX9Renderer* pD3D9 = NiDynamicCast(NiDX9Renderer,NiRenderer::GetRenderer());
	NiDX9RenderState* pRenderState = pD3D9->GetRenderState();

	bool bchange = false;

	if(!D3D9::s_binit)
	{
		D3D9::s_effect = dweffect;
		D3D9::s_binit = true;
		D3D9::s_factor = dwcolor;
	
		SetColor(dwcolor);

		bchange = true;
	}
	else
	{
		if(D3D9::s_effect != dweffect)
		{
			D3D9::s_effect = dweffect;
			D3D9::s_factor = dwcolor;
			
			bchange = true;
		}
		else
		{
			if(D3D9::s_factor != dwcolor)
			{
				SetColor(dwcolor);
			}
		}

	}

	if(!bchange)
	{
		return;
	}


	SetColor(dwcolor);

	if(D3D9::s_effect & fxTexTex)
	{
		pRenderState->SetTextureStageState(0,D3DTSS_COLORARG1,D3DTA_TEXTURE);
		pRenderState->SetTextureStageState(0,D3DTSS_COLOROP,D3DTOP_SELECTARG1);
	}
	else
	{
		pRenderState->SetTextureStageState(0,D3DTSS_COLORARG1,D3DTA_TEXTURE);
		pRenderState->SetTextureStageState(0,D3DTSS_COLOROP,D3DTOP_SELECTARG1);
	}

	
	///alpha渲染状态

	if(D3D9::s_effect & fxBlendAdd)
	{
		pRenderState->SetRenderState(D3DRS_SRCBLEND,D3DBLEND_SRCALPHA);
		pRenderState->SetRenderState(D3DRS_DESTBLEND,D3DBLEND_ONE);
	}
	else
	{
		if(D3D9::s_effect & fxBlendBlit)
		{
			pRenderState->SetRenderState(D3DRS_SRCBLEND,D3DBLEND_ONE);
			pRenderState->SetRenderState(D3DRS_DESTBLEND,D3DBLEND_ZERO);
		}
		else
		{
			pRenderState->SetRenderState(D3DRS_SRCBLEND,D3DBLEND_SRCALPHA);
			pRenderState->SetRenderState(D3DRS_DESTBLEND,D3DBLEND_INVSRCALPHA);
		}
	}

	


	if(D3D9::s_effect & fxBlendMax)
	{
		//暂不支持
	}

	
}

void ReleaseRaw( CTexture9 * pImg)
{
	pImg->Release();
}


CTexture9::CTexture9()
{
	m_pDT = NULL;
	m_pVT = NULL;
	m_bUsed = false;

	m_fleft = 0;
	m_ftop = 0;
}

CTexture9::~CTexture9()
{

}


bool CTexture9::CheckTextureSize()
{
	if(
		(m_nTextureWidth < D3D9::s_MinTextureSize.w)
		||
		(m_nTextureHeight < D3D9::s_MinTextureSize.h)
	  )
	{
		return false;
	}


	if(
		(m_nTextureWidth > D3D9::s_MaxTextureSize.w)
		||
		(m_nTextureHeight > D3D9::s_MaxTextureSize.h)
	  )
	{
		return false;
	}
	
	return true;
}

static int to2n( int num)
{
	int i;
	for(i = 1; i < num; i *= 2);//可能：最小尺寸是64！
	
	return i;
}


void  CTexture9::SetCenterPointer(float x,float y)
{
	m_fTextureCenterX = x;
	m_fTextureCenterY = y;
}

void  CTexture9::GetCenterPointer(float& x, float& y)
{
	x = m_fTextureCenterX;
	y = m_fTextureCenterY;
}


bool CTexture9::Set(int w,int h,int attrib)
{
	m_nRealWidth = w;
	m_nRealHeight = h;
	m_attrib = attrib;

	switch(m_attrib) {
	case  imageAttrib1555 :
		{
			if(D3D9::s_TextureFormat[D3D9::TF_1555].bSupport)
			{
				m_tft = D3DFMT_A1R5G5B5;
				m_bits = 2;
			}
			else
			{
				m_attrib =  imageAttrib8888;
				
				if(D3D9::s_TextureFormat[D3D9::TF_8888].bSupport)
				{
					m_tft = D3DFMT_A8R8G8B8;
					m_bits = 4;
				}
				else
				{
					return false;
				}
			}

			
		}
		break;
	case imageAttrib4444 :
		{
			if(D3D9::s_TextureFormat[D3D9::TF_4444].bSupport)
			{
				m_tft = D3DFMT_A4R4G4B4;
				m_bits = 2;
			}
			else
			{
				m_attrib =  imageAttrib8888;
				
				if(D3D9::s_TextureFormat[D3D9::TF_8888].bSupport)
				{
					m_tft = D3DFMT_A8R8G8B8;
					m_bits = 4;
				}
				else
				{
					return false;
				}
			}

			
		}
		break;

	case imageAttrib8888 :
		{
			if(D3D9::s_TextureFormat[D3D9::TF_8888].bSupport)
			{
				m_tft = D3DFMT_A8R8G8B8;
				m_bits = 4;
			}
			else
			{
				return false;
			}

			
		}
		break;
	default:
		{
			return false;
		}
		break;
	};


	m_nTextureWidth = to2n(m_nRealWidth);
	m_nTextureHeight = to2n(m_nRealHeight);

	return CheckTextureSize();
}

bool CTexture9::getVMem()
{
	HRESULT hr = g_pD3DDevice9->CreateTexture(
		m_nTextureWidth,
		m_nTextureHeight,
		1,
		NULL,
		m_tft,
		D3DPOOL_MANAGED,
		&m_pDT,
		NULL
		);

	if(FAILED(hr))
	{
		return false;
	}

	return true;
}

bool CTexture9::SetUse(bool bTranslate)
{
	CalcTextureAddr();
	m_btran = bTranslate;
	SetupVT();

	m_bUsed = true;
	return true;
}

bool CTexture9::UpdateImage(int dleft,int dtop,int w,int h,TImage* pimage,int sleft,int stop)
{
	HRESULT hr;
	unsigned char * dest;
	unsigned char * src;
	int destpitch;
	int srcpitch;
	int bits;
	D3DLOCKED_RECT lr;

	if(m_pDT)
	{
		hr = m_pDT->LockRect(0,&lr,NULL,D3DLOCK_NOSYSLOCK);

		if(FAILED(hr))
		{
			return false;
		}



		//////////////////////////////////////////////////////////////////////////
		dest = (unsigned char*)lr.pBits;
		destpitch = lr.Pitch;

		src =  (unsigned char*)pimage->data;
		srcpitch = pimage->pitch;

		D3DFORMAT srcf;
		switch(pimage->attrib) {
		case imageAttrib1555 :
			{
				srcf = D3DFMT_A1R5G5B5;
				bits = 2;
			}
			break;

		case imageAttrib4444 :
			{
				bits = 2;
				srcf = D3DFMT_A4R4G4B4;
			}
			break;
		case imageAttrib8888 :
			{
				bits = 4;
				srcf = D3DFMT_A8R8G8B8;
			}
			break;
		};

		src = src + stop * pimage->pitch + sleft * bits;
		dest = dest + dtop * destpitch + dleft * bits;
		//////////////////////////////////////////////////////////////////////


		if(bits == 2)
		{
			MemCopy(dest,destpitch,src,srcpitch,w,h);
		}
		else
		{
			MemCopy32(dest,destpitch,src,srcpitch,w,h);	
		}

		m_pDT->UnlockRect(0);

	}
	else
	{
		return false;
	}

	return true;
}





void CTexture9::Release()
{
	if(NULL != this)
	{
		if(m_bUsed)
		{
			SafeRelease(m_pDT);
			SafeRelease(m_pVT);

			m_bUsed = false;
		}
		delete this;
	}
}

static float ctx, cty;
//////////////////////////////////////////////////////////////////////////
///上图
///Render按照全局的中心点来上图, Render按照图片自己的中心点来上图
///CoreRender是上图的核心逻辑
//////////////////////////////////////////////////////////////////////////
void CTexture9::Render(int left,int top,DWORD dweffect,DWORD dwcolor /*= 0xffffffff*/)
{
	ctx = D3D9::s_centerx;
	cty = D3D9::s_centery;

	CoreRender(left,top,dweffect,dwcolor);
}


void CTexture9::RenderEx(int left,int top,DWORD dweffect,DWORD dwcolor /*= 0xffffffff*/)
{
	ctx =  m_fTextureCenterX;
	cty =  m_fTextureCenterY;

	CoreRender(left,top,dweffect,dwcolor);
}

inline float D3DMath_AngleToArc(float degree)
{
	degree *= (3.14159265359f/180);
	return degree;
}



inline void GetMaxRect(CRect2& t,PANEL_CUSTOMVERTEX_T* tl)
{
	float top,left,bottom,right;
	
	left = min(min(tl[0].x,tl[1].x),min(tl[2].x,tl[3].x));
	right = max(max(tl[0].x,tl[1].x),max(tl[2].x,tl[3].x));
	top = min(min(tl[0].y,tl[1].y),min(tl[2].y,tl[3].y));
	bottom = max(max(tl[0].y,tl[1].y),max(tl[2].y,tl[3].y));

	t.x = (int)left;
	t.y = (int)top;
	t.w = (int)(right - left);
	t.h = (int)(bottom - top);
}


void CalcXY(int angle,int w,int h,float&x,float&y,float&tu,float&tv,float textaddr[4][2])
{
	float r = D3DMath_AngleToArc(angle);

	if(angle<=45)
	{
		x = w/2;
		y = tan(r)*x;

		float s = (y + h/2) / h;

		tu = (1 - s) * textaddr[2][0] + s * textaddr[3][0];
		tv = (1 - s) * textaddr[2][1] + s * textaddr[3][1];

	}
	else
	{
		if(angle<=135)
		{
			y = h/2;
			x = y / tan(r);

			float s = (x + w/2) / w;

			tu = (1 - s) * textaddr[1][0] + s * textaddr[3][0];
			tv = (1 - s) * textaddr[1][1] + s * textaddr[3][1];

		}
		else
		{
			if(angle <= 225)
			{
				x = -w/2;
				y = tan(r)*x;



				float s = (y + h/2) / h;

				tu = (1 - s) * textaddr[0][0] + s * textaddr[1][0];
				tv = (1 - s) * textaddr[0][1] + s * textaddr[1][1];

			}
			else
			{

				if(angle <= 315)
				{
					y = -h/2;
					x = y / tan(r);

					float s = (x + w/2) / w;

					tu = (1 - s) * textaddr[0][0] + s * textaddr[2][0];
					tv = (1 - s) * textaddr[0][1] + s * textaddr[2][1];
				}
				else
				{
					x = w/2;
					y = tan(r)*x;

					float s = (y + h/2) / h;

					tu = (1 - s) * textaddr[2][0] + s * textaddr[3][0];
					tv = (1 - s) * textaddr[2][1] + s * textaddr[3][1];
				}
			}

		}
	}

	y = -y;
}
void CTexture9::DrawImageArc(int left,int top,int angle,int rot,HIMG hImg,DWORD effect, DWORD dwColor)
{ 
	//设定好渲染状态
	SetUpRenderState(effect,dwColor);


	if(angle < 0)
	{
		angle = 0;
	}

	if(angle == 0)
	{
		return;
	}

	angle %= 361;
 
	static PANEL_CUSTOMVERTEX_T arcv[20];

	arcv[0].x = 0;
	arcv[0].y = 0;
	arcv[0].z = 1;
	arcv[0].u = (m_textaddr[1][0] + m_textaddr[3][0])/2;
	arcv[0].v = (m_textaddr[0][1] + m_textaddr[1][1])/2;
	arcv[0].rhw = 1;

	int max_vec_num = 0;

	max_vec_num = (angle) / 45;

	CalcXY(angle,m_nRealWidth,m_nRealHeight,arcv[1].x,arcv[1].y,arcv[1].u,arcv[1].v,m_textaddr);
	arcv[1].z = 1;
	arcv[1].rhw = 1;

	int index = 2;
	for(int i = 0; i < max_vec_num; i++)
	{
		int an = (max_vec_num  - i) * 45;
		CalcXY(an,m_nRealWidth,m_nRealHeight,arcv[index].x,arcv[index].y,arcv[index].u,arcv[index].v,m_textaddr);
		arcv[index].z = 1;
		arcv[index].rhw = 1;
		index++;
	}

	CalcXY(0,m_nRealWidth,m_nRealHeight,arcv[index].x,arcv[index].y,arcv[index].u,arcv[index].v,m_textaddr);
	arcv[index].z = 1;
	arcv[index].rhw = 1;


	int num = index + 1;
	

	if(effect & fxGeoScale)
	{
		for(int i = 0; i < num; i++)
		{
			arcv[i].x *= D3D9::s_scalex;
			arcv[i].y *= D3D9::s_scaley;
		}
	}

	bool bSystemScale = (g_fXUIScale != 1.0f || g_fYUIScale != 1.0f);
	if (bSystemScale)
	{
		for(int i = 0; i < num; i++)
		{
			arcv[i].x *= g_fXUIScale;
			arcv[i].y *= g_fYUIScale;
		}
	}


	if(effect & fxGeoRotate)
	{
		float arc = D3DMath_AngleToArc(D3D9::s_angle);
		float si = sin(arc);
		float co = cos(arc);

		for(int i = 0; i < num; i++)
		{
			float tx,ty;
			tx = arcv[i].x;
			ty = arcv[i].y;

			arcv[i].x = tx*co - ty*si;
			arcv[i].y = tx*si + ty*co;
		} 
	}

	for(int i = 0; i < num; i++)
	{
		arcv[i].x += (float)left;
		arcv[i].y += (float)top;
	}

	SetTexture();
	SetVertexShader(PANEL_D3DFVF_CUSTOMVERTEX_T);

	g_pD3DDevice9->DrawPrimitiveUP(D3DPT_TRIANGLEFAN,num - 2,arcv,sizeof(PANEL_CUSTOMVERTEX_T));


}

void CTexture9::CoreRender(int left,int top,DWORD dweffect,DWORD dwcolor)
{
	HRESULT hr;
	//设定好渲染状态
	SetUpRenderState(dweffect,dwcolor);

	

	if(m_pVT && !m_btran)
	{
		
		static D3DXMATRIX tranpre3;
		static D3DXMATRIX tranpre;
		static D3DXMATRIX tran;
		static D3DXMATRIX rot;
		static D3DXMATRIX scale;

		float icx,icy;
		float tx,ty;

		icx = m_fleft + m_nRealWidth / 2.0f;
		icy = m_ftop + m_nRealHeight / 2.0f;


		tx = ctx - icx;
		ty = cty - icy;

		D3DXMatrixScaling(&tranpre3,m_nRealWidth,m_nRealHeight,1.0f);

		if(dweffect & fxGeoMirrorX)
		{
			D3DXMatrixIdentity(&scale);
			scale._11 = -1;
			D3DXMatrixTranslation(&tranpre,-tx,ty,0);
			D3DXMatrixMultiply(&scale,&scale,&tranpre3);
			D3DXMatrixMultiply(&tranpre,&scale,&tranpre);
		}
		else
		{
			D3DXMatrixTranslation(&tranpre,-tx,ty,0);
			D3DXMatrixMultiply(&tranpre,&tranpre3,&tranpre);
		}

		if(dweffect & fxGeoScale)
		{
			D3DXMatrixScaling(&scale,D3D9::s_scalex,D3D9::s_scaley,1.0f);
			D3DXMatrixMultiply(&tranpre,&tranpre,&scale);

		}

		if(dweffect & fxGeoRotate)
		{
			D3DXMatrixRotationZ(&rot,-D3DMath_AngleToArc(D3D9::s_angle));
			D3DXMatrixMultiply(&tranpre,&tranpre,&rot);
		}

		float cx,cy;
		cx = D3D9::m_Width / 2.0f;
		cy = D3D9::m_Height / 2.0f;

		tx = left - cx;
		ty = top - cy;

		D3DXMatrixTranslation(&tran,tx,-ty,0.0f);


		D3DXMatrixMultiply(&tranpre,&tranpre,&tran);

		g_pD3DDevice9->SetTransform(D3DTS_WORLD,&tranpre);
		
		SetVertexShader(PANEL_D3DFVF_CUSTOMVERTEX);
		SetTexture();
		SetVertex();
				
		g_pD3DDevice9->DrawPrimitive(D3DPT_TRIANGLESTRIP,0,2);
	    
	}
	else
	{
		float fleft, fright,ftop,fbottom;

		fleft = m_fleft - ctx;
		ftop = m_ftop - cty;
		fright = fleft + m_nRealWidth;
		fbottom = ftop + m_nRealHeight;

		bool bSystemScale = (g_fXUIScale != 1.0f || g_fYUIScale != 1.0f);
		if (bSystemScale)
		{
			fleft *= g_fXUIScale;
			ftop *= g_fYUIScale;
			fright *= g_fXUIScale;
			fbottom *= g_fYUIScale;
		}
		

		

		

		m_vector[0].x =  fleft;
		m_vector[0].y =  fbottom;
		m_vector[0].z =  1.0f;
		m_vector[0].u =  m_textaddr[0][0];
		m_vector[0].v =  m_textaddr[0][1];
		m_vector[0].rhw = 1.0f;


		m_vector[1].x =  fleft;
		m_vector[1].y =  ftop;
		m_vector[1].z =  1.0f;
		m_vector[1].u =  m_textaddr[1][0];
		m_vector[1].v =  m_textaddr[1][1];
		m_vector[1].rhw = 1.0f;



		m_vector[2].x =  fright;
		m_vector[2].y =  fbottom;
		m_vector[2].z =  1.0f;
		m_vector[2].u =  m_textaddr[2][0];
		m_vector[2].v =  m_textaddr[2][1];
		m_vector[2].rhw = 1.0f;



		m_vector[3].x =  fright;
		m_vector[3].y =  ftop;
		m_vector[3].z =  1.0f;
		m_vector[3].u =  m_textaddr[3][0];
		m_vector[3].v =  m_textaddr[3][1];
		m_vector[3].rhw = 1.0f;



		if(dweffect & fxGeoScale)
		{
			for(int i = 0; i < 4; i++)
			{
				m_vector[i].x *= D3D9::s_scalex;
				m_vector[i].y *= D3D9::s_scaley;
			}
		}


		if(dweffect & fxGeoRotate)
		{
			float arc = D3DMath_AngleToArc(D3D9::s_angle);
			float si = sin(arc);
			float co = cos(arc);

			for(int i = 0; i < 4; i++)
			{
				float tx,ty;
				tx = m_vector[i].x;
				ty = m_vector[i].y;

				m_vector[i].x = tx*co - ty*si;
				m_vector[i].y = tx*si + ty*co;
			} 
		}

		for(int i = 0; i < 4; i++)
		{
			m_vector[i].x += (float)left * g_fXUIScale;
			m_vector[i].y += (float)top * g_fYUIScale;
		}

		SetTexture();
		SetVertexShader(PANEL_D3DFVF_CUSTOMVERTEX_T);
		
		g_pD3DDevice9->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP,2,m_vector,sizeof(PANEL_CUSTOMVERTEX_T));
		
	}
}


void CTexture9::CalcTextureAddr()
{
	float xinter;
	float yinter;
	
	xinter = (float)m_nRealWidth;
	xinter += 0.5;
	xinter /= m_nTextureWidth;
	
	yinter = (float)m_nRealHeight;
	yinter += 0.5;
	yinter /= m_nTextureHeight;

	m_textaddr[0][0] = 0.0f;
	m_textaddr[0][1] = yinter;

	m_textaddr[1][0] = 0.0f;
	m_textaddr[1][1] = 0.0f;


	m_textaddr[2][0] = xinter;
	m_textaddr[2][1] = yinter;


	m_textaddr[3][0] = xinter;
	m_textaddr[3][1] = 0.0f;
}

void CTexture9::SetupVT()
{
	//Create the vertex buffer from our device.

	if(NULL == g_pD3DDevice9)
	{
		return;
	}

	if(!m_btran)
	{
		if(FAILED(g_pD3DDevice9->CreateVertexBuffer(4 * sizeof(PANEL_CUSTOMVERTEX),
			0, PANEL_D3DFVF_CUSTOMVERTEX,
			D3DPOOL_MANAGED, &m_pVT,NULL)))
		{
			return;
		}

		HRESULT hr;
		PANEL_CUSTOMVERTEX* pVT = NULL;
		hr = m_pVT->Lock(0, 4 * sizeof(PANEL_CUSTOMVERTEX), (void**)&pVT, 0);

		
		if(SUCCEEDED(hr))
		{
			pVT[0].x = -1.0f / 2.0f;
			pVT[0].y = -1.0f/ 2.0f;

			pVT[0].z = 0.5f;

			pVT[0].u = m_textaddr[0][0];
			pVT[0].v = m_textaddr[0][1];

			pVT[1].x =  -1.0f / 2.0f;
			pVT[1].y =   1.0f / 2.0f;

			pVT[1].z = 0.5f;

			pVT[1].u = m_textaddr[1][0];
			pVT[1].v = m_textaddr[1][1];


			pVT[2].x = 1.0f / 2.0f;
			pVT[2].y = -1.0f / 2.0f;

			pVT[2].z = 0.5f;

			pVT[2].u = m_textaddr[2][0];
			pVT[2].v = m_textaddr[2][1];


			pVT[3].x = 1.0f / 2.0f;
			pVT[3].y = 1.0f / 2.0f;

			pVT[3].z = 0.5f;

			pVT[3].u = m_textaddr[3][0];
			pVT[3].v = m_textaddr[3][1];

			m_pVT->Unlock();
		}
		else
		{
			m_pVT = NULL;
		}

	}
	else
	{
		m_pVT = NULL;
	}
	
}


HTEX CTexMgr9::CreateTexture(TImage *pImage,bool bTranslate)
{
	HTEX hTex = GenTextureHandle( pImage->w, pImage->h, pImage->attrib,bTranslate);
	if ( !hTex )
	{
		return 0;
	}
	
	if ( !handleMgr_.GetItem( hTex)->UpdateImage(0,0,pImage->w,pImage->h,pImage,0,0) )
	{
		return 0;
	}
	
	return hTex;
}


HTEX CTexMgr9::GenTextureHandle(int w, int h, int attrib,bool bTranslate)
{
	int hTex = handleMgr_.CreateHandle( new CTexture9());
	CTexture9* tex = handleMgr_.GetItem( hTex);
	if ( !tex->Set( w, h, attrib) )
	{
		return 0;
	}
	if ( !tex->getVMem())
	{
		return 0;
	};
	
	tex->SetUse(bTranslate);
	
	return hTex;
}

void CTexMgr9::SetLeftTop(HTEX hTex,float left,float top)
{
	CTexture9* ptex = handleMgr_.GetItem(hTex);

	if(ptex)
	{
		ptex->SetLeftTop(left,top);

	}
}


//o---------------------------------------------o
//|		CImage 贴图								|
//o---------------------------------------------o
static void cutPic( int w, int h, int i, int j, int& subW, int& subH){
	subW = w - j * D3D9::s_MaxTextureSize.w;		//left width
	if ( subW > D3D9::s_MaxTextureSize.w ) 
	{
		subW = D3D9::s_MaxTextureSize.w;
	}

	subH = h- i * D3D9::s_MaxTextureSize.h;		//left width
	if ( subH > D3D9::s_MaxTextureSize.h )
	{
		subH = D3D9::s_MaxTextureSize.h;
	}
}

void CImage9::Set( int w, int h, int attrib,bool bTransform)
{	
	assert( hTexs_.empty());
	w_ = w;
	h_ = h;
	wNum_ = (w + D3D9::s_MaxTextureSize.w - 1) / D3D9::s_MaxTextureSize.w;
	hNum_ = (h + D3D9::s_MaxTextureSize.h -1) / D3D9::s_MaxTextureSize.h;
	hTexs_.resize( wNum_ * hNum_ );

	for (int i = 0; i < hNum_; i++)
	{
		for (int j = 0; j < wNum_; j++)
		{
			int subW, subH;
			cutPic( w, h, i, j, subW, subH);
			hTexs_[idx(i,j)] = texMgr9.GenTextureHandle( subW, subH, attrib,bTransform);
			//设定左上角的坐标
			texMgr9.SetLeftTop(hTexs_[idx(i,j)],m_fleft + j * D3D9::s_MaxTextureSize.w, m_ftop + i * D3D9::s_MaxTextureSize.h);
		}
	} 
}



void CImage9::Release(){
	
	if ( hTexs_.empty())	
	{
		return;
	}
	
	for(int i = 0; i < hNum_; i++)
	{
		for(int j = 0; j < wNum_; j++)
		{
			texMgr9.ReleaseHandle(hTexs_[idx(i,j)]);
		}
	}

	hTexs_.clear();
	delete this;
}

void ReleaseRaw( CImage9* pImg)
{
	pImg->Release();
}




static bool cutPicEx( 
				   int left, 
				   int top, 
				   int width,
				   int height,
				   int h, 
				   int l, 
				   int& subLeft,
				   int& subTop,
				   CRect2& subRect)
{
	CRect2 src( left, top, width, height);
	CRect2 dst( 
		l * D3D9::s_MaxTextureSize.w , 
		h * D3D9::s_MaxTextureSize.h , 
		D3D9::s_MaxTextureSize.w , 
		D3D9::s_MaxTextureSize.h );
	
	subRect = src.and( dst);
	
	if ( subRect.isEmpty() )
	{
		return false;
	}
	
	subLeft = subRect.x - l * D3D9::s_MaxTextureSize.w;
	subTop	= subRect.y - h * D3D9::s_MaxTextureSize.h;
	
	subRect.x -= left;
	subRect.y -= top;

	return true;
}


void CImage9::UpdataImage( int left, int top, int wd,int ht,TImage *pImage,int sleft,int stop)
{
	assert( !hTexs_.empty());
	
	if ( 1 == hNum_ * wNum_ )
	{
		CTexture9* pTex = texMgr9.GetTexture( hTexs_[0]);
		pTex->UpdateImage(left,top,wd,ht,pImage,sleft,stop);
		return;
	}

	for( int h = 0; h < hNum_; h++)
	{
		for( int l = 0; l < wNum_; l++)
		{
			CTexture9* pTex = texMgr9.GetTexture( hTexs_[idx(h,l)]);

			int subLeft, subTop;
			CRect2 subRect;
			
			if(cutPicEx( left, top, wd, ht, h, l, subLeft, subTop,subRect))
			{
				pTex->UpdateImage(
					subLeft,
					subTop,
					subRect.w,
					subRect.h,
					pImage,
					subRect.x,
					subRect.y);
			}
		}
	}
}

extern	IDisplayD3D9* display3D_;

void CImage9::DrawImage( int left, int top, DWORD effect, DWORD dwColor)
{
	for(int h = 0; h < hNum_; h++)
	{
		for(int l = 0; l < wNum_; l++)
		{
			CTexture9* pTex = texMgr9.GetTexture(hTexs_[idx(h,l)]);
			pTex->Render( 
				left, 
				top, 
				effect,
				dwColor);
		}
	}
/*
if ( hNum_*wNum_ > 1 )
	for(int h = 0; h < hNum_; h++)
	{
		for(int l = 0; l < wNum_; l++)
		{
			display3D_->DrawBox( 
			left+l*D3D9::s_MaxTextureSize.w,
			top+h*D3D9::s_MaxTextureSize.h, 
			D3D9::s_MaxTextureSize.w,
			D3D9::s_MaxTextureSize.h,
			0x7fffffff);
		}
	}
*/

}

 void CImage9::DrawImageArc(int left,int top,int angle,int rot,HIMG hImg,DWORD effect, DWORD dwColor)
 {
	 for(int h = 0; h < hNum_; h++)
	 {
		 for(int l = 0; l < wNum_; l++)
		 {
			 CTexture9* pTex = texMgr9.GetTexture(hTexs_[idx(h,l)]);
			 pTex->DrawImageArc(left,top,angle,rot,hImg,effect,dwColor);
		 }
	 }
 }


void CImage9::DrawImageEx( int left, int top, DWORD effect, DWORD dwColor)
{
	for(int h = 0; h < hNum_; h++)
	{
		for(int l = 0; l < wNum_; l++)
		{
			CTexture9* pTex = texMgr9.GetTexture(hTexs_[idx(h,l)]);
			pTex->RenderEx( 
				left, 
				top, 
				effect,
				dwColor);
		}
	}

/*	if ( hNum_*wNum_ > 1 )
	for(int h = 0; h < hNum_; h++)
	{
		for(int l = 0; l < wNum_; l++)
		{
			display3D_->DrawBox( 
			left+l*MAX_TEX_BLOCK_SIZE,
			top+h*MAX_TEX_BLOCK_SIZE, 
			MAX_TEX_BLOCK_SIZE,
			MAX_TEX_BLOCK_SIZE,
			0x7fffffff);
		}
	}
*/

}

//o---------------------------------------------o
//|		CImageMgr 影像管理器					|
//o---------------------------------------------o
HIMG CImageMgr9::GenImageHandle( int w, int h, int attrib,bool bTransform){
	int hImg = handleMgr_.CreateHandle( new CImage9());
	
	CImage9* pImage = handleMgr_.GetItem( hImg);
	
	pImage->Set(w,h,attrib,bTransform);
	
	return hImg;
}

HIMG CImageMgr9::GenImageHandle( int left, int top,int w, int h, int attrib,bool bTransform)
{
	int hImg = handleMgr_.CreateHandle( new CImage9());
	
	CImage9* pImage = handleMgr_.GetItem( hImg);
	
	pImage->m_fleft = (float) left;
	pImage->m_ftop  = (float) top;

	
	pImage->Set(w,h,attrib,bTransform);
	
	return hImg;

}

HIMG CImageMgr9::CreateImage( TImage *pImage,bool bTransform){
	
	HIMG hImg = GenImageHandle( pImage->w, pImage->h, pImage->attrib,bTransform);
	
	if ( !hImg)
	{
		return 0;
	}

	handleMgr_.GetItem(hImg)->UpdataImage(0, 0, pImage->w,pImage->h,pImage,0,0);
	
	return hImg;
}

HIMG CImageMgr9::CreateImage( TImage *pImage,int left,int top,bool bTransform)
{
	HIMG hImg = GenImageHandle(left,top,pImage->w, pImage->h, pImage->attrib,bTransform);
	
	if ( !hImg)
	{
		return 0;
	}

	handleMgr_.GetItem(hImg)->UpdataImage(0, 0, pImage->w,pImage->h,pImage,0,0);
	
	return hImg;
}


void CImage9::SetCenterXY(float x, float y)
{
	for(int h = 0; h < hNum_; h++)
	{
		for(int l = 0; l < wNum_; l++)
		{
			CTexture9* pTex = texMgr9.GetTexture(hTexs_[idx(h,l)]);
			pTex->SetCenterPointer(x,y);
		}
	}
}

void CTexture9::SetLeftTop(float left, float top)
{
	m_fleft = left;
	m_ftop = top;
}




//画出贴图的 子矩形, 用来画汉字？
void CTexture9::Draw(  int x, int y, int w, int h,					//矩形屏幕位置
						int left, int top, int width, int height,DWORD dwcolor)	//矩形贴图位置
{
	SetUpRenderState(0,dwcolor);
	/*width = g_fXUIScale * width;
	height = g_fYUIScale * height;
	left   = g_fXUIScale * left;
	top    = g_fYUIScale * top;*/

	x *= g_fXUIScale;
	y *= g_fYUIScale;
	//w *= g_fXUIScale;
	//h *= g_fYUIScale;

	float rect[4][2] = {{x,y}, {x+w, y}, {x+w, y+h}, {x,y+h}};
	//Draw( rect, left, top, width, height);
	

	float tW = (float)width / (float)m_nTextureWidth;///g_fXUIScale;
	float tH = (float)height / (float)m_nTextureHeight;///g_fYUIScale;
	float tLeft = (float)left / (float)m_nTextureWidth;///g_fXUIScale;
	float tTop  = (float)top / (float)m_nTextureHeight;///g_fYUIScale;

	m_vector[0].x =  rect[3][0] - 0.5f;
	m_vector[0].y =  rect[3][1] - 0.5f;
	m_vector[0].z =  1.0f;
	m_vector[0].u =  tLeft;
	m_vector[0].v =  tTop + tH;
	m_vector[0].rhw = 1.0f;


	m_vector[1].x =  rect[0][0] - 0.5f;
	m_vector[1].y =  rect[0][1] - 0.5f;
	m_vector[1].z =  1.0f;
	m_vector[1].u =  tLeft;
	m_vector[1].v =  tTop;
	m_vector[1].rhw = 1.0f;



	m_vector[2].x =  rect[2][0] - 0.5f;
	m_vector[2].y =  rect[2][1] - 0.5f;
	m_vector[2].z =  1.0f;
	m_vector[2].u =  tLeft + tW;
	m_vector[2].v =  tTop + tH;
	m_vector[2].rhw = 1.0f;



	m_vector[3].x =  rect[1][0] - 0.5f*g_fXUIScale;
	m_vector[3].y =  rect[1][1] - 0.5f*g_fYUIScale;
	m_vector[3].z =  1.0f;
	m_vector[3].u =  tLeft + tW;
	m_vector[3].v =  tTop;
	m_vector[3].rhw = 1.0f;

	
	SetTexture();
	SetVertexShader(PANEL_D3DFVF_CUSTOMVERTEX_T);

	g_pD3DDevice9->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP,2,m_vector,sizeof(PANEL_CUSTOMVERTEX_T));
}


//画出贴图的 子矩形
void CTexture9::Draw( float rect[4][2],				//矩形屏幕位置, 顺时针方向
					 float left, float top, float width, float height)	//矩形贴图位置
{

	float tW = (float)width / (float)m_nTextureWidth;
	float tH = (float)height / (float)m_nTextureHeight;
	float tLeft = (float)left / (float)m_nTextureWidth;
	float tTop  = (float)top / (float)m_nTextureHeight;

	m_vector[0].x =  rect[3][0] - 0.5f;
	m_vector[0].y =  rect[3][1] - 0.5f;
	m_vector[0].z =  1.0f;
	m_vector[0].u =  tLeft;
	m_vector[0].v =  tTop + tH;
	m_vector[0].rhw = 1.0f;


	m_vector[1].x =  rect[0][0] - 0.5f;
	m_vector[1].y =  rect[0][1] - 0.5f;
	m_vector[1].z =  1.0f;
	m_vector[1].u =  tLeft;
	m_vector[1].v =  tTop;
	m_vector[1].rhw = 1.0f;


 
	m_vector[2].x =  rect[2][0] - 0.5f;
	m_vector[2].y =  rect[2][1] - 0.5f;
	m_vector[2].z =  1.0f;
	m_vector[2].u =  tLeft + tW;
	m_vector[2].v =  tTop + tH;
	m_vector[2].rhw = 1.0f;



	m_vector[3].x =  rect[1][0] - 0.5f;
	m_vector[3].y =  rect[1][1] - 0.5f;
	m_vector[3].z =  1.0f;
	m_vector[3].u =  tLeft + tW;
	m_vector[3].v =  tTop;
	m_vector[3].rhw = 1.0f;

	bool bSystemScale = (g_fXUIScale != 1.0f || g_fYUIScale != 1.0f);
	if (bSystemScale)
	{
		for(int i = 0; i < 4; i++)
		{
			m_vector[i].x *= g_fXUIScale;
			m_vector[i].y *= g_fYUIScale;
		}
	}

	SetTexture();
	SetVertexShader(PANEL_D3DFVF_CUSTOMVERTEX_T);

	g_pD3DDevice9->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP,2,m_vector,sizeof(PANEL_CUSTOMVERTEX_T));
	
}



void SetVertexShader(DWORD shader)
{
	if(D3D9::s_shader == shader)
	{
		return;
	}
	NiDX9Renderer* pD3D9 = NiDynamicCast(NiDX9Renderer,NiRenderer::GetRenderer());
	NiDX9RenderState* pRenderState = pD3D9->GetRenderState();
	pRenderState->SetFVF(shader, true);
	/*
	HRESULT hr;
	hr = g_pD3DDevice9->SetFVF(shader);
	*/
	D3D9::s_shader = shader;
}

void CTexture9::SetTexture()
{

	NiDX9Renderer* pD3D9 = NiDynamicCast(NiDX9Renderer,NiRenderer::GetRenderer());
	NiDX9RenderState* pRenderState = pD3D9->GetRenderState();
	pRenderState->SetTexture(0, m_pDT);
//	pRenderState->SetTexture(1, NULL);
//	pRenderState->SetTexture(2, NULL);
//	pRenderState->SetTexture(3, NULL);
	/*
	if(D3D9::s_textture != m_pDT)
	{
		g_pD3DDevice9->SetTexture(0,m_pDT);
		D3D9::s_textture = m_pDT;
	}
	*/
}

void CTexture9::SetVertex()
{
	HRESULT hr;
	if(D3D9::s_vertexbuffer != m_pVT)
	{
		if(!m_btran)
		{
			hr = g_pD3DDevice9->SetStreamSource(0,m_pVT,0,sizeof(PANEL_CUSTOMVERTEX));
		}
		else
		{
			g_pD3DDevice9->SetStreamSource(0,m_pVT,0,sizeof(PANEL_CUSTOMVERTEX_T));
		}

		D3D9::s_vertexbuffer = m_pVT;
	}
}
