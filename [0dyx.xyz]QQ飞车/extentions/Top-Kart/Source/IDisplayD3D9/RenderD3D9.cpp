// RenderD3D9.cpp: implementation of the CRenderD3D9 class.
//
//////////////////////////////////////////////////////////////////////

#include "Top-KartPCH.h"
#include "RenderD3D9.h"
#include "Texture9.h"
#include "math.h"
#include "NiRenderer.h"
#include "NiDX9Renderer.h"
#include "IDisplayD3D91.h"

#pragma comment(lib,"d3d9.lib")
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CTexMgr9 texMgr9;
CImageMgr9 imageMgr9;
int vp = 0;
extern NiD3DRenderer* g_pkDX9Renderer;

extern float g_fXUIScale;
extern float g_fYUIScale;

extern float g_fXScale;
extern float g_fYScale;

extern float g_fOriginWidth;
extern float g_fOriginHeight;


CRenderD3D9::CRenderD3D9()
{

	m_bEnable = false;
	m_hWnd = NULL;
	m_bWin = false;
	m_bLinear = false;

	m_clipleft = 0;
	m_cliptop  = 0;
	m_clipwidth  = 0;
	m_clipheight = 0;

	m_bOK = false;

	m_Sprite = NULL;
}

CRenderD3D9::~CRenderD3D9()
{

}

bool OnLost(void* pData)
{
	CRenderD3D9* pRender = (CRenderD3D9*) pData;
	pRender->m_bOK = false;
	return true;
}

bool OnReset(bool bBeforeReset,void* pData)
{
	CRenderD3D9* pRender = (CRenderD3D9*) pData;

	if(bBeforeReset)
	{
	}
	else
	{
		pRender->m_bOK = true;
	}

	return true;
}


HRESULT CRenderD3D9::CreateDisplay()
{
	NiDX9Renderer *pDX9Renderer = NULL;
	NiRenderer* pRenderer = NiRenderer::GetRenderer();

	if(pRenderer == NULL)
	{
		return E_FAIL;
	}

	pDX9Renderer = NiSmartPointerCast(NiDX9Renderer,pRenderer);
	g_pD3DDevice9 = pDX9Renderer->GetD3DDevice();

	pDX9Renderer->AddLostDeviceNotificationFunc(OnLost,this);
	pDX9Renderer->AddResetNotificationFunc(OnReset,this);

	//////////////////////////////////////////////////////////////////////////
	///
	///
	//////////////////////////////////////////////////////////////////////////


	D3D9::s_TextureFormat[D3D9::TF_4444].bSupport = true;

	D3D9::s_TextureFormat[D3D9::TF_1555].bSupport = true;

	D3D9::s_TextureFormat[D3D9::TF_8888].bSupport = true;




	HRESULT hr;


	D3DCAPS9 decsaps9;
	hr = g_pD3DDevice9->GetDeviceCaps(&decsaps9);

	if(SUCCEEDED(hr))
	{	
		D3D9::s_MaxTextureSize.w = decsaps9.MaxTextureWidth;
		D3D9::s_MaxTextureSize.h = decsaps9.MaxTextureHeight;
		D3D9::s_MinTextureSize.w = 0;
		D3D9::s_MinTextureSize.h = 0;
	}



	if(D3D9::s_MaxTextureSize.w > 512)
	{
		D3D9::s_MaxTextureSize.w = 512;
	}

	if(D3D9::s_MaxTextureSize.h > 512)
	{
		D3D9::s_MaxTextureSize.h = 512;
	}

	if(
		(D3D9::s_MaxTextureSize.h == 0)
		||
		(D3D9::s_MaxTextureSize.w == 0)
		)
	{
		return E_FAIL;
	}

	D3DXCreateSprite( g_pD3DDevice9, &m_Sprite );

	m_bOK = true;

	return S_OK;
}


void Setup2DCamera()
{
	D3DXMATRIX matOrtho; 
	D3DXMATRIX matIdentity;

	//Setup the orthogonal projection matrix and the default world/view matrix
	D3DXMatrixOrthoLH(&matOrtho, (float)D3D9::m_Width, (float)D3D9::m_Height, 0.0f, 1.0f);
	D3DXMatrixIdentity(&matIdentity);


	g_pD3DDevice9->SetTransform(D3DTS_PROJECTION, &matOrtho);
	g_pD3DDevice9->SetTransform(D3DTS_WORLD, &matIdentity);
	g_pD3DDevice9->SetTransform(D3DTS_VIEW, &matIdentity);

}



HRESULT CRenderD3D9::PrepareState()
{
	Setup2DCamera();
	Init2DRenderState();
	return S_OK;
}

HRESULT CRenderD3D9::RestoreState()
{

	return S_OK;
}

HRESULT CRenderD3D9::CreateDirect3D()
{

	return S_OK;
}


#define SAVE_STATE(XXX,YYY) \
	g_pD3DDevice9->SetRenderState(XXX, YYY); \


static int  bw,bh;

void CRenderD3D9::Init2DRenderState()
{
	D3D9::s_binit = false;
	D3D9::s_textture = NULL;
	D3D9::s_shader = 0;

	NiDX9Renderer* pD3D9 = NiDynamicCast(NiDX9Renderer,NiRenderer::GetRenderer());
	NiDX9RenderState* pRenderState = pD3D9->GetRenderState();
	
	pRenderState->SaveRenderState();
	pRenderState->SetVertexShader(NULL, true);
	pRenderState->SetPixelShader(NULL, true);

	pRenderState->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_CLAMP);
	pRenderState->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_CLAMP);

	/*
	g_pD3DDevice9->SetVertexShader(NULL);
	g_pD3DDevice9->SetPixelShader(NULL);
	*/
	pRenderState->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW, true);
	//SAVE_STATE(D3DRS_CULLMODE,D3DCULL_CCW);
	pRenderState->SetRenderState(D3DRS_ZFUNC,D3DCMP_ALWAYS, true);
	//SAVE_STATE(D3DRS_ZFUNC,D3DCMP_ALWAYS);
	pRenderState->SetRenderState(D3DRS_ZWRITEENABLE,FALSE, true);
	//SAVE_STATE(D3DRS_ZWRITEENABLE,FALSE);
	pRenderState->SetRenderState(D3DRS_SCISSORTESTENABLE,FALSE, true);
	//SAVE_STATE(D3DRS_SCISSORTESTENABLE,FALSE);

	pRenderState->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE, true);
	pRenderState->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE, true);
	//SAVE_STATE(D3DRS_ALPHABLENDENABLE, TRUE);
	pRenderState->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA, true);
	//SAVE_STATE(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pRenderState->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA, true);
	//SAVE_STATE(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	pRenderState->SetRenderState(D3DRS_ZENABLE , D3DZB_FALSE, true);
	//SAVE_STATE(D3DRS_ZENABLE , D3DZB_FALSE);
	pRenderState->SetRenderState(D3DRS_ZWRITEENABLE , FALSE, true);
	//SAVE_STATE(D3DRS_ZWRITEENABLE , FALSE );
	pRenderState->SetRenderState(D3DRS_LIGHTING, FALSE, true);
	//SAVE_STATE(D3DRS_LIGHTING, FALSE);
	//for ( int i = 0; i < 4; i++ )
	int i = 0;
	{
		pRenderState->SetTextureStageState(i,D3DTSS_COLORARG1, D3DTA_TEXTURE, true);
		pRenderState->SetTextureStageState(i,D3DTSS_COLORARG2, D3DTA_CURRENT, true);
		pRenderState->SetTextureStageState(i,D3DTSS_COLOROP, D3DTOP_SELECTARG1, true);
		pRenderState->SetTextureStageState(i,D3DTSS_ALPHAARG1, D3DTA_TEXTURE, true);
		pRenderState->SetTextureStageState(i,D3DTSS_ALPHAARG2, D3DTA_CURRENT, true);
		pRenderState->SetTextureStageState(i,D3DTSS_ALPHAOP, D3DTSS_ALPHAARG1, true);
	}

	pRenderState->SetSamplerState(0,D3DSAMP_MAGFILTER,D3DTEXF_POINT, true);
	//g_pD3DDevice9->SetSamplerState(0,D3DSAMP_MAGFILTER,D3DTEXF_POINT);
	pRenderState->SetSamplerState(0,D3DSAMP_MINFILTER,D3DTEXF_POINT, true);
	//g_pD3DDevice9->SetSamplerState(0,D3DSAMP_MINFILTER,D3DTEXF_POINT);
	
	//g_pD3DDevice9->Clear(0,NULL,D3DCLEAR_ZBUFFER,0,1.0f,0);

	LPDIRECT3DSURFACE9 lpSur9 = NULL;

	g_pD3DDevice9->GetBackBuffer(0,0,D3DBACKBUFFER_TYPE_MONO,&lpSur9);

	if(lpSur9)
	{
		D3DSURFACE_DESC dsd;
		lpSur9->GetDesc(&dsd);
		D3DVIEWPORT9 p9;
		g_pD3DDevice9->GetViewport(&p9);
		p9.X = 0;
		p9.Y = 0;
		p9.Height = max((unsigned int)1, dsd.Height);
		p9.Width = max((unsigned int)1, dsd.Width);

		bw = dsd.Width;
		bh = dsd.Height;


		g_pD3DDevice9->SetViewport(&p9);

		lpSur9->Release();
	}

	pRenderState->SetTexture(0, NULL);
	//g_pD3DDevice9->SetTexture(0,NULL);
	D3D9::s_textture = NULL;

}

int g_pFrameRate;
HRESULT CRenderD3D9::BeginSence()
{	
	PrepareState();
	return S_OK;
}

extern IDisplayD3D9* display3D_;
HRESULT CRenderD3D9::EndSence()
{
	static char buffer[MAX_PATH];
	_snprintf(buffer,MAX_PATH,"%3d",g_pFrameRate);
	display3D_->SetFont(1);
	display3D_->DrawText(0,0, buffer,0xffffffff,9,0);

	NiDX9Renderer* pD3D9 = NiDynamicCast(NiDX9Renderer,NiRenderer::GetRenderer());

	if(pD3D9)
	{
		NiDX9RenderState* pRenderState = pD3D9->GetRenderState();
		/*pRenderState->RestoreRenderState(D3DRS_CULLMODE);
		pRenderState->RestoreRenderState(D3DRS_ZFUNC);
		pRenderState->RestoreRenderState(D3DRS_ZWRITEENABLE);
		pRenderState->RestoreRenderState(D3DRS_SCISSORTESTENABLE);
		pRenderState->RestoreRenderState(D3DRS_ALPHABLENDENABLE);
		pRenderState->RestoreRenderState(D3DRS_SRCBLEND);
		pRenderState->RestoreRenderState(D3DRS_DESTBLEND);
		pRenderState->RestoreRenderState(D3DRS_ZENABLE);
		pRenderState->RestoreRenderState(D3DRS_LIGHTING);
		pRenderState->RestoreRenderState(D3DRS_TEXTUREFACTOR);
		*/
		pRenderState->RestoreFVF();
		pRenderState->RestoreTextureStageState(0,D3DTSS_COLORARG1);
		pRenderState->RestoreTextureStageState(0,D3DTSS_COLORARG2);
		pRenderState->RestoreTextureStageState(0,D3DTSS_COLOROP);
		pRenderState->RestoreTextureStageState(0,D3DTSS_ALPHAARG1);
		pRenderState->RestoreTextureStageState(0,D3DTSS_ALPHAARG2);
		pRenderState->RestoreTextureStageState(0,D3DTSS_ALPHAOP);
		
		pRenderState->RestoreSamplerState(0,D3DSAMP_MAGFILTER);
		pRenderState->RestoreSamplerState(0,D3DSAMP_MINFILTER);
		pRenderState->RestoreRenderState();

		/*
		if(pRenderState)
		{
			SAVE_STATE(D3DRS_CULLMODE,pRenderState->GetRenderState(D3DRS_CULLMODE));
			SAVE_STATE(D3DRS_ZFUNC,pRenderState->GetRenderState(D3DRS_ZFUNC));
			SAVE_STATE(D3DRS_ZWRITEENABLE,pRenderState->GetRenderState(D3DRS_ZWRITEENABLE));
			SAVE_STATE(D3DRS_SCISSORTESTENABLE,pRenderState->GetRenderState(D3DRS_SCISSORTESTENABLE));

			SAVE_STATE(D3DRS_ALPHABLENDENABLE, pRenderState->GetRenderState(D3DRS_ALPHABLENDENABLE));
			SAVE_STATE(D3DRS_SRCBLEND, pRenderState->GetRenderState(D3DRS_SRCBLEND));
			SAVE_STATE(D3DRS_DESTBLEND, pRenderState->GetRenderState(D3DRS_DESTBLEND));
			SAVE_STATE(D3DRS_ZENABLE , pRenderState->GetRenderState(D3DRS_ZENABLE));
			SAVE_STATE(D3DRS_ZWRITEENABLE , pRenderState->GetRenderState(D3DRS_ZWRITEENABLE) );
			SAVE_STATE(D3DRS_LIGHTING, pRenderState->GetRenderState(D3DRS_LIGHTING));
		   
			g_pD3DDevice9->SetFVF(pRenderState->GetFVF());
			g_pD3DDevice9->SetRenderState(D3DRS_TEXTUREFACTOR,pRenderState->GetRenderState(D3DRS_TEXTUREFACTOR));
			g_pD3DDevice9->SetTextureStageState(0,D3DTSS_COLORARG1,pRenderState->GetTextureStageState(0,D3DTSS_COLORARG1));
			g_pD3DDevice9->SetTextureStageState(0,D3DTSS_COLORARG2,pRenderState->GetTextureStageState(0,D3DTSS_COLORARG2));
			g_pD3DDevice9->SetTextureStageState(0,D3DTSS_COLOROP,pRenderState->GetTextureStageState(0,D3DTSS_COLOROP));
			g_pD3DDevice9->SetTextureStageState(0,D3DTSS_ALPHAARG1,pRenderState->GetTextureStageState(0,D3DTSS_ALPHAARG1));
			g_pD3DDevice9->SetTextureStageState(0,D3DTSS_ALPHAARG2,pRenderState->GetTextureStageState(0,D3DTSS_ALPHAARG2));
			g_pD3DDevice9->SetTextureStageState(0,D3DTSS_ALPHAOP,pRenderState->GetTextureStageState(0,D3DTSS_ALPHAOP));		

			g_pD3DDevice9->SetSamplerState(0,D3DSAMP_MAGFILTER,pRenderState->GetSamplerState(0,D3DSAMP_MAGFILTER));
			g_pD3DDevice9->SetSamplerState(0,D3DSAMP_MINFILTER,pRenderState->GetSamplerState(0,D3DSAMP_MINFILTER));

			
		}*/
	}

	return S_OK;
}

HRESULT CRenderD3D9::Release()
{
	if(m_Sprite)
	{
		m_Sprite->Release();
		m_Sprite = NULL;
	}
	return S_OK;
}

void CRenderD3D9::EnableClip(bool bEnable)
{
	if(m_bEnable != bEnable)
	{
		m_bEnable = bEnable;
		//g_pD3DDevice9->SetRenderState(D3DRS_SCISSORTESTENABLE,bEnable ? TRUE : FALSE);

		if(m_bEnable)
		{
			SetClipRect(m_clipleft,m_cliptop,m_clipwidth,m_clipheight);
		}
		else
		{
			D3DXMATRIX matOrtho; 
			D3DXMatrixOrthoLH(&matOrtho, (float)D3D9::m_Width, (float)D3D9::m_Height, 0.0f, 1.0f);

			D3DVIEWPORT9 p9;
			g_pD3DDevice9->GetViewport(&p9);
			p9.X = 0;
			p9.Y = 0;
			p9.Height = max(bh, 1);
			p9.Width = max(bw, 1);

			g_pD3DDevice9->SetViewport(&p9);
		}
	} 
}

#ifndef max
#define max(a,b)            (((a) > (b)) ? (a) : (b))
#endif

#ifndef min
#define min(a,b)            (((a) < (b)) ? (a) : (b))
#endif

void CRenderD3D9::SetClipRect(int left, int top, int width, int height)
{
	m_clipleft = left;
	m_cliptop = top;
	m_clipwidth = width;
	m_clipheight = height;


	RECT rt1;
	RECT rt2;
	RECT out;
	::SetRect(&rt1,0,0,g_fOriginWidth, g_fOriginHeight);//D3D9::m_Width,D3D9::m_Height);
	::SetRect(&rt2,left,top,left+width,top+height);
	IntersectRect(&out,&rt1,&rt2);



	m_clipleft = out.left ;
	m_cliptop = out.top;
	m_clipwidth = (out.right - out.left) <= 0 ? 1 : (out.right - out.left);
	m_clipheight = (out.bottom - out.top) <= 0 ? 1 : (out.bottom - out.top);


	D3DXMATRIX mTran;
	D3DXMatrixTranslation(&mTran,-m_clipleft*g_fXScale,-m_cliptop*g_fYScale,0);
	D3DXMATRIX nTre;
	D3DXMatrixOrthoLH(&nTre,max(m_clipwidth*g_fXScale, 1),max(m_clipheight*g_fYScale, 1),0.0f,1.0f);

	D3DXMatrixMultiply(&nTre,&mTran,&nTre);

	g_pD3DDevice9->SetTransform(D3DTS_PROJECTION,&nTre);

	float sx,sy;

	sx = bw;
	sx /= D3D9::m_Width;

	sy = bh;
	sy /= D3D9::m_Height;

	int l,t,h,w;

	l = m_clipleft*g_fXUIScale;//g_fXScale;
	t = m_cliptop*g_fYUIScale;//g_fYScale;
	h = m_clipheight*g_fYUIScale;//g_fYScale;
	w = m_clipwidth*g_fXUIScale;//g_fXScale;

	D3DVIEWPORT9 p9;
	g_pD3DDevice9->GetViewport(&p9);
	p9.X = l;
	p9.Y = t;
	p9.Height = max(1, min(h,g_fOriginHeight));//bh);
	p9.Width = max(1, min(w,g_fOriginWidth));//bw);




	g_pD3DDevice9->SetViewport(&p9);
}



HIMG CRenderD3D9::SelectImage( TImage *pImage,bool Transform)
{
	return imageMgr9.CreateImage(pImage,Transform);	
}

HIMG CRenderD3D9::SelectImage( TImage *pImage,int left,int top,bool Transform)
{
	return imageMgr9.CreateImage(pImage,left,top,Transform);
}

HIMG CRenderD3D9::GenImageHandle( int w, int h, int attrib,bool Transform)
{
	return imageMgr9.GenImageHandle(w,h,attrib,Transform);
}

void CRenderD3D9::ReleaseImageHandle( HIMG hImg)
{
	imageMgr9.ReleaseHandle(hImg);
}


void CRenderD3D9::UpdateImage( HIMG hImg, 
							  int left, int top, 
							  int width,int height,
							  TImage *pImage,int sleft,int stop)
{	
	assert(pImage);

	CImage9* pimg = imageMgr9.GetImage(hImg);
	assert(pimg);


	if(pimg)
	{
		pimg->UpdataImage(left,top,width,height,pImage,sleft,stop);
	}
}

int	CRenderD3D9::GetImageWidth(HIMG hImg)
{
	CImage9* pimg = imageMgr9.GetImage(hImg);
	assert(pimg);

	if(pimg)
	{
		return pimg->GetImageWidth();
	}

	return 0;
}

int	CRenderD3D9::GetImageHeight(HIMG hImg)
{
	CImage9* pimg = imageMgr9.GetImage(hImg);
	assert(pimg);

	if(pimg)
	{
		return pimg->GetImageHeight();
	}

	return 0;
}

void CRenderD3D9::DrawImage(int left, int top, 
							HIMG hImg, 
							DWORD effect /*=0*/, DWORD dwColor /*=0xffffffff*/)
{
	if(m_bOK)
	{
		CImage9* pimg = imageMgr9.GetImage(hImg);
		assert(pimg);

		if(pimg)
		{
			pimg->DrawImage(left,top,effect,dwColor);
		}
	}
}


void CRenderD3D9::DrawImageArc(int left,int top,int angle,int rot,HIMG hImg,DWORD effect, DWORD dwColor)
{
	if(m_bOK)
	{
		CImage9* pimg = imageMgr9.GetImage(hImg);
		assert(pimg);

		if(pimg)
		{
			pimg->DrawImageArc(left,top,angle,rot,hImg,effect,dwColor);
		}
	}
}
void CRenderD3D9::DrawImageEx(int left, int top, 
							  HIMG hImg, 
							  DWORD effect /*=0*/, DWORD dwColor /*=0xffffffff*/)
{
	if(m_bOK)
	{
		CImage9* pimg = imageMgr9.GetImage(hImg);
		assert(pimg);

		if(pimg)
		{
			pimg->DrawImageEx(left,top,effect,dwColor);
		}
	}
}


void CRenderD3D9::SetImageCenter(float centerX, float centerY)
{
	D3D9::s_centerx = centerX;
	D3D9::s_centery = centerY;
}

void CRenderD3D9::SetImageScale(float scaleX, float scaleY)
{
	D3D9::s_scalex = scaleX;
	D3D9::s_scaley = scaleY;
}

void CRenderD3D9::SetImageRotate(float degree)
{
	D3D9::s_angle = degree;
}

void CRenderD3D9::SetImageCenterEx(HIMG hImg,float centerX, float centerY)
{
	CImage9* pimg = imageMgr9.GetImage(hImg);
	assert(pimg);

	if(pimg)
	{
		pimg->SetCenterXY(centerX,centerY);
	}
}

void CRenderD3D9::GetImageScale(float & scaleX, float & scaleY)
{
	scaleX = D3D9::s_scalex;
	scaleY = D3D9::s_scaley;
}


void CRenderD3D9::DrawPixel(int x, int y, DWORD dwColor)
{
	if(!m_bOK)
	{
		return;
	}

	PANEL_CUSTOMVERTEX_T_2 vertex;
	vertex.x = (float) x * g_fXUIScale;
	vertex.y = (float) y * g_fYUIScale;
	vertex.z = 1.0f;
	vertex.rhw = 1.0f;
	vertex.dwcolor = dwColor;

	NiDX9Renderer* pD3D9 = NiDynamicCast(NiDX9Renderer,NiRenderer::GetRenderer());
	NiDX9RenderState* pRenderState = pD3D9->GetRenderState();
	pRenderState->SetTexture(0, NULL);
	/*
	if(D3D9::s_textture != NULL)
	{
		g_pD3DDevice9->SetTexture(0,NULL);
		D3D9::s_textture = NULL;
	}
	*/
	SetVertexShader(PANEL_D3DFVF_CUSTOMVERTEX_T_2);
	g_pD3DDevice9->DrawPrimitiveUP(D3DPT_POINTLIST,1,&vertex,sizeof(PANEL_CUSTOMVERTEX_T_2));
}

void CRenderD3D9::DrawLine(int x1, int y1, int x2, int y2, DWORD dwColor)
{
	if(!m_bOK)
	{
		return;
	}

	PANEL_CUSTOMVERTEX_T_2 vertex[2];

	vertex[0].x = (float) x1 * g_fXUIScale;
	vertex[0].y = (float) y1 * g_fYUIScale;
	vertex[0].z = 1.0f;
	vertex[0].rhw = 1.0f;
	vertex[0].dwcolor = dwColor;


	vertex[1].x = (float) x2 * g_fXUIScale;
	vertex[1].y = (float) y2 * g_fYUIScale;
	vertex[1].z = 1.0f;
	vertex[1].rhw = 1.0f;
	vertex[1].dwcolor = dwColor;

	NiDX9Renderer* pD3D9 = NiDynamicCast(NiDX9Renderer,NiRenderer::GetRenderer());
	NiDX9RenderState* pRenderState = pD3D9->GetRenderState();
	pRenderState->SetTexture(0, NULL);
	/*
	if(D3D9::s_textture != NULL)
	{
		g_pD3DDevice9->SetTexture(0,NULL);
		D3D9::s_textture = NULL;
	}
	*/
	SetVertexShader(PANEL_D3DFVF_CUSTOMVERTEX_T_2);
	g_pD3DDevice9->DrawPrimitiveUP(D3DPT_LINELIST,2,vertex,sizeof(PANEL_CUSTOMVERTEX_T_2));
}

void CRenderD3D9::DrawPolygon( int numPoints, const float* xy, DWORD dwColor)
{
	if(!m_bOK)
	{
		return;
	}

	static PANEL_CUSTOMVERTEX_T_2 vertex[10];

	for(int i = 0; i < numPoints; i++)
	{
		vertex[i].x = xy[(i)*2] * g_fXUIScale;
		vertex[i].y = xy[(i)*2 + 1] * g_fYUIScale;
		vertex[i].z = 1.0f;
		vertex[i].rhw = 1.0f;
		vertex[i].dwcolor = dwColor;
	}

	NiDX9Renderer* pD3D9 = NiDynamicCast(NiDX9Renderer,NiRenderer::GetRenderer());
	NiDX9RenderState* pRenderState = pD3D9->GetRenderState();
	pRenderState->SetTexture(0, NULL);
	/*
	if(D3D9::s_textture != NULL)
	{
		g_pD3DDevice9->SetTexture(0,NULL);
		D3D9::s_textture = NULL;
	}
	*/
	SetVertexShader(PANEL_D3DFVF_CUSTOMVERTEX_T_2);
	g_pD3DDevice9->DrawPrimitiveUP(D3DPT_TRIANGLEFAN,numPoints - 2,vertex,sizeof(PANEL_CUSTOMVERTEX_T_2));
}

void CRenderD3D9::DrawPolygon( int numPoints, const float* xy, DWORD* dwColor)
{
	if(!m_bOK)
	{
		return;
	}

	static PANEL_CUSTOMVERTEX_T_2 vertex[10];

	for(int i = 0; i < numPoints; i++)
	{
		vertex[i].x = xy[(i)*2] * g_fXUIScale;
		vertex[i].y = xy[(i)*2 + 1] * g_fYUIScale;
		vertex[i].z = 1.0f;
		vertex[i].rhw = 1.0f;
		vertex[i].dwcolor = dwColor[i];
	}

	NiDX9Renderer* pD3D9 = NiDynamicCast(NiDX9Renderer,NiRenderer::GetRenderer());
	NiDX9RenderState* pRenderState = pD3D9->GetRenderState();
	pRenderState->SetTexture(0, NULL);
	/*
	if(D3D9::s_textture != NULL)
	{
		g_pD3DDevice9->SetTexture(0,NULL);
		D3D9::s_textture = NULL;
	}
	*/
	SetVertexShader(PANEL_D3DFVF_CUSTOMVERTEX_T_2);
	g_pD3DDevice9->DrawPrimitiveUP(D3DPT_TRIANGLEFAN,numPoints - 2,vertex,sizeof(PANEL_CUSTOMVERTEX_T_2));
}

void CRenderD3D9::DrawBar(int left, int top, int width, int height, DWORD dwColor)
{
	if(!m_bOK)
	{
		return;
	}

	PANEL_CUSTOMVERTEX_T_2 m_vector[4];

	m_vector[0].x =  left;
	m_vector[0].y =  top + height;
	m_vector[0].z =  1.0f;
	m_vector[0].dwcolor = dwColor;
	m_vector[0].rhw = 1.0f;


	m_vector[1].x =  left;
	m_vector[1].y =  top;
	m_vector[1].z =  1.0f;
	m_vector[1].dwcolor = dwColor;
	m_vector[1].rhw = 1.0f;



	m_vector[2].x =  left + width;
	m_vector[2].y =  top  + height ;
	m_vector[2].z =  1.0f;
	m_vector[2].dwcolor = dwColor;
	m_vector[2].rhw = 1.0f;



	m_vector[3].x =  left + width;
	m_vector[3].y =  top;
	m_vector[3].z =  1.0f;
	m_vector[3].dwcolor = dwColor;
	m_vector[3].rhw = 1.0f;


	bool bSystemScale = (g_fXUIScale != 1.0f || g_fYUIScale != 1.0f);

	if (bSystemScale)
	{
		for (int i = 0; i < 4; ++i)
		{
			m_vector[i].x *= g_fXUIScale;
			m_vector[i].y *= g_fYUIScale;
		}
	}

	NiDX9Renderer* pD3D9 = NiDynamicCast(NiDX9Renderer,NiRenderer::GetRenderer());
	NiDX9RenderState* pRenderState = pD3D9->GetRenderState();
	pRenderState->SetTexture(0, NULL);
	/*
	if(D3D9::s_textture != NULL)
	{
		g_pD3DDevice9->SetTexture(0,NULL);
		D3D9::s_textture = NULL;
	}
	*/

	SetVertexShader(PANEL_D3DFVF_CUSTOMVERTEX_T_2);
	HRESULT hr =g_pD3DDevice9->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP,2,m_vector,sizeof(PANEL_CUSTOMVERTEX_T_2));
}
void CRenderD3D9::DrawBox(int left, int top, int width, int height, DWORD dwColor)
{
	if(!m_bOK)
	{
		return;
	}

	PANEL_CUSTOMVERTEX_T_2 m_vector[5];

	m_vector[0].x =  left;
	m_vector[0].y =  top + height;
	m_vector[0].z =  1.0f;
	m_vector[0].dwcolor = dwColor;
	m_vector[0].rhw = 1.0f;


	m_vector[1].x =  left;
	m_vector[1].y =  top;
	m_vector[1].z =  1.0f;
	m_vector[1].dwcolor = dwColor;
	m_vector[1].rhw = 1.0f;



	m_vector[3].x =  left + width;
	m_vector[3].y =  top  + height ;
	m_vector[3].z =  1.0f;
	m_vector[3].dwcolor = dwColor;
	m_vector[3].rhw = 1.0f;



	m_vector[2].x =  left + width;
	m_vector[2].y =  top;
	m_vector[2].z =  1.0f;
	m_vector[2].dwcolor = dwColor;
	m_vector[2].rhw = 1.0f;


	m_vector[4].x =  left;
	m_vector[4].y =  top  + height;
	m_vector[4].z =  1.0f;
	m_vector[4].dwcolor = dwColor;
	m_vector[4].rhw = 1.0f;

	bool bSystemScale = (g_fXUIScale != 1.0f || g_fYUIScale != 1.0f);

	if (bSystemScale)
	{
		for (int i = 0; i < 5; ++i)
		{
			m_vector[i].x *= g_fXUIScale;
			m_vector[i].y *= g_fYUIScale;
		}
	}

	NiDX9Renderer* pD3D9 = NiDynamicCast(NiDX9Renderer,NiRenderer::GetRenderer());
	NiDX9RenderState* pRenderState = pD3D9->GetRenderState();
	pRenderState->SetTexture(0, NULL);
	/*
	if(D3D9::s_textture != NULL)
	{
		g_pD3DDevice9->SetTexture(0,NULL);
		D3D9::s_textture = NULL;
	}
	*/
	SetVertexShader(PANEL_D3DFVF_CUSTOMVERTEX_T_2);
	g_pD3DDevice9->DrawPrimitiveUP(D3DPT_LINESTRIP,4,m_vector,sizeof(PANEL_CUSTOMVERTEX_T_2));	
}

void CRenderD3D9::EnableTextureLinear(bool bEnable)
{
	if(m_bLinear == bEnable)
	{
		return;
	}

	NiDX9Renderer* pD3D9 = NiDynamicCast(NiDX9Renderer,NiRenderer::GetRenderer());
	NiDX9RenderState* pRenderState = pD3D9->GetRenderState();

	m_bLinear = bEnable;

	if(bEnable)
	{
		pRenderState->SetSamplerState(0,D3DSAMP_MAGFILTER,D3DTEXF_LINEAR);
		pRenderState->SetSamplerState(0,D3DSAMP_MINFILTER,D3DTEXF_LINEAR);

	}
	else
	{

		pRenderState->SetSamplerState(0,D3DSAMP_MAGFILTER,D3DTEXF_POINT);
		pRenderState->SetSamplerState(0,D3DSAMP_MINFILTER,D3DTEXF_POINT);

	}
}

HRESULT CRenderD3D9::ClearScreen(DWORD dwColor)
{
	if(!m_bOK)
	{
		return S_FALSE;
	}

	if(NULL == g_pD3DDevice9)
	{
		return E_FAIL;
	}

	return g_pD3DDevice9->Clear(0, NULL, D3DCLEAR_TARGET,dwColor, 1.0f, 0);
}

bool CRenderD3D9::IsClipEnable()
{
	return m_bEnable;
}

void CRenderD3D9::GetClipRect(int &left, int &top, int &width, int &height)
{
	left = m_clipleft;
	top  = m_cliptop;
	width = m_clipwidth;
	height = m_clipheight;

	if(!m_bEnable)
	{
		height = g_fOriginHeight;//bh;//D3D9::m_Height;
		width  = g_fOriginWidth;//bw;//D3D9::m_Width;
	}
}
