#include "Top-KartPCH.h"
#include "3dlinesmanager.h"
#include "NiDX9Renderer.h"

inline D3DCOLOR NiColor2D3DCOLOR(const NiColorA & kColor)
{
	D3DCOLOR d3dColor = D3DCOLOR_ARGB((int)(kColor.a*255), (int)(kColor.r*255), 
							(int)(kColor.g*255), (int)(kColor.b*255));
    return d3dColor;
}

struct VERTEX
{
	float x, y, z;
	D3DCOLOR color;
	//float tu, tv;

//	static DWORD FVF;
};



DWORD FVF = D3DFVF_XYZ | D3DFVF_DIFFUSE /*| D3DFVF_TEX1*/;

C3DLinesManager * C3DLinesManager::ms_p3DLinesMgr = NULL;

C3DLinesManager::C3DLinesManager(ID3DXSprite * pSprite, IDirect3DDevice9 * pDevice)
: m_pSprite(pSprite)
, m_pD3dDevice(pDevice)
, m_bOnSprite(false)
, m_bLostDevice(false)
{
	assert(pDevice);// && pSprite
}

C3DLinesManager::~C3DLinesManager(void)
{
	if(m_pSprite)
		m_pSprite->Release();
	m_pSprite = NULL;

	m_pD3dDevice = NULL;
}

C3DLinesManager * C3DLinesManager::Create(NiRenderer * pRenderer)
{
	if (!ms_p3DLinesMgr)
	{
		NiDX9Renderer *pDX9Renderer = NULL;
		pDX9Renderer = NiDynamicCast(NiDX9Renderer, pRenderer);
		IDirect3DDevice9 * pD3dDevice = pDX9Renderer->GetD3DDevice();
		ID3DXSprite * pSprite = NULL;
		//D3DXCreateSprite( pD3dDevice, &pSprite );

		ms_p3DLinesMgr = NiNew C3DLinesManager(pSprite, pD3dDevice);
	}
	
	return ms_p3DLinesMgr;
}

C3DLinesManager * C3DLinesManager::Get3DLinesManager()
{
	return ms_p3DLinesMgr;
}

void C3DLinesManager::Destroy3DLinesManager()
{
	if (ms_p3DLinesMgr)
		NiDelete ms_p3DLinesMgr;
	ms_p3DLinesMgr = NULL;
}

void C3DLinesManager::OnRestDevice()
{
	if (m_pSprite)
		m_pSprite->OnResetDevice();
	m_bLostDevice = false;
}

void C3DLinesManager::OnLostDevice()
{
	if (m_pSprite)
		m_pSprite->OnLostDevice();
	m_bLostDevice = true;
}

void C3DLinesManager::SetMatrix(const NiMatrix3 & kMatrix)
{
	if (m_pD3dDevice) 
	{
		//m_pD3dDevice->matri
	}
}

void C3DLinesManager::StartSprite()
{
	if (m_pSprite && !m_bOnSprite)
	{
		m_pSprite->Begin(D3DXSPRITE_ALPHABLEND);
		m_bOnSprite = true;
	}
}

void C3DLinesManager::EndSprite()
{
	if (m_pSprite && m_bOnSprite)
	{
		m_pSprite->End();
		m_bOnSprite = false;
	}
}


void C3DLinesManager::DrawLine(const NiPoint3 & kStart, const NiPoint3 & kEnd, const NiColorA & kColor)
{
	if (!m_pD3dDevice)
		return;

	//StartSprite();

	D3DCOLOR color = NiColor2D3DCOLOR(kColor);
	
	VERTEX vertices[2] =
	{
		kStart.x, kStart.y,	kStart.z,  color, /*0, 0,*/
		kEnd.x,	  kEnd.y,	kEnd.z,    color, /*0, 0, */
	};
	/*
	VERTEX vertices[3] =
	{
			kStart.x, kStart.y,	kStart.z,  1.0f, color, 
			kEnd.x,	  kEnd.y,	kEnd.z,    1.0f, color, 
			100.0f, 100.0f, 10.0f, 1.0f, color,
	};
	*/
	// Since we're doing our own drawing here we need to flush the sprites
	//m_pSprite->Flush();
	//IDirect3DVertexDeclaration9 *pDecl = NULL;
	//pd3dDevice->GetVertexDeclaration( &pDecl );  // Preserve the sprite's current vertex decl
	//NiDynamicCast(NiDX9Renderer, NiRenderer::GetRenderer())->GetRenderState()->SaveRenderState();
	
	m_pD3dDevice->SetFVF( FVF );
/*
	m_pD3dDevice->SetTextureStageState( 0, D3DTSS_COLOROP, D3DTOP_SELECTARG2 );
	m_pD3dDevice->SetTextureStageState( 0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG2 );
*/
	m_pD3dDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);

	m_pD3dDevice->SetTextureStageState( 0, D3DTSS_COLOROP, D3DTOP_DISABLE );
	m_pD3dDevice->SetTextureStageState( 0, D3DTSS_ALPHAOP, D3DTOP_DISABLE );
//	m_pD3dDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
/*
	m_pD3dDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	m_pD3dDevice->DrawPrimitiveUP(D3DPT_TRIANGLELIST, 1, vertices, sizeof(VERTEX));
	;*/
	m_pD3dDevice->DrawPrimitiveUP( D3DPT_LINESTRIP, 1, vertices, sizeof(VERTEX) );

	m_pD3dDevice->SetTextureStageState( 0, D3DTSS_COLOROP, D3DTOP_MODULATE );
	m_pD3dDevice->SetTextureStageState( 0, D3DTSS_ALPHAOP, D3DTOP_MODULATE );

	//NiDynamicCast(NiDX9Renderer, NiRenderer::GetRenderer())->GetRenderState()->RestoreRenderState();	
	// Restore the vertex decl
	//pd3dDevice->SetVertexDeclaration( pDecl );
	//pDecl->Release();
	//EndSprite();
}
void C3DLinesManager::DrawRect(const NiPoint3 & k1, const NiPoint3 & k2, const NiColorA & kColor)
{
	D3DCOLOR color = NiColor2D3DCOLOR(kColor);	
	VERTEX vertices[6] = 
	{
		k1.x, k1.y, k1.z,			   color,//1
		k2.x, k2.y, k2.z,			   color,//4
		k2.x, k1.y, (k1.z + k2.z)/2.f, color,//2
		k1.x, k1.y, k1.z,			   color,//1
		k1.x, k2.y, (k1.z + k2.z)/2.f, color,//3
		k2.x, k2.y, k2.z,			   color,//4
	};

	m_pD3dDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);

	m_pD3dDevice->SetTextureStageState( 0, D3DTSS_COLOROP, D3DTOP_DISABLE );
	m_pD3dDevice->SetTextureStageState( 0, D3DTSS_ALPHAOP, D3DTOP_DISABLE );
//	m_pD3dDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	m_pD3dDevice->DrawPrimitiveUP(D3DPT_TRIANGLELIST, 2, vertices, sizeof(VERTEX));
	

	m_pD3dDevice->SetTextureStageState( 0, D3DTSS_COLOROP, D3DTOP_MODULATE );
	m_pD3dDevice->SetTextureStageState( 0, D3DTSS_ALPHAOP, D3DTOP_MODULATE );
}

void C3DLinesManager::DrawRect(const NiPoint3 & k1, const NiPoint3 & k2, 
			  const NiPoint3 & k3, const NiPoint3 & k4, const NiColorA & kColor)
{
	D3DCOLOR color = NiColor2D3DCOLOR(kColor);	
	VERTEX vertices[6] = 
	{
		k1.x, k1.y, k1.z, color,//1
		k2.x, k2.y, k2.z, color,//2
		k3.x, k3.y, k3.z, color,//3
		k1.x, k1.y, k1.z, color,//1
		k3.x, k3.y, k3.z, color,//3
		k4.x, k4.y, k4.z, color,//4
	};

	m_pD3dDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);

	m_pD3dDevice->SetTextureStageState( 0, D3DTSS_COLOROP, D3DTOP_DISABLE );
	m_pD3dDevice->SetTextureStageState( 0, D3DTSS_ALPHAOP, D3DTOP_DISABLE );
//	m_pD3dDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	m_pD3dDevice->DrawPrimitiveUP(D3DPT_TRIANGLELIST, 2, vertices, sizeof(VERTEX));


	m_pD3dDevice->SetTextureStageState( 0, D3DTSS_COLOROP, D3DTOP_MODULATE );
	m_pD3dDevice->SetTextureStageState( 0, D3DTSS_ALPHAOP, D3DTOP_MODULATE );
}

void C3DLinesManager::DrawBox(const NiPoint3 & k1, const NiPoint3 & k2, 
							  const NiPoint3 & k3, const NiPoint3 & k4, const NiColorA & kColor)
{
	const NiPoint3 k5(k3.x, k1.y, (k1.z + k3.z)/2.f);
	const NiPoint3 k6(k1.x, k3.y, (k1.z + k3.z)/2.f);

	const NiPoint3 temp = k2 - k1;

	const NiPoint3 k7(k5 + temp);
	const NiPoint3 k8(k6 + temp);

	DrawRect(k1, k3, kColor);
	DrawRect(k1, k7, kColor);
	DrawRect(k1, k8, kColor);
	DrawRect(k4, k2, kColor);
	DrawRect(k4, k5, kColor);
	DrawRect(k4, k6, kColor);
}

void C3DLinesManager::DrawBox(const NiPoint3 & _k1, const NiPoint3 & _k2, 
							  float halfwidth, const NiColorA & kColor)
{
	NiPoint3 k1, k2, k3, k4, k5, k6, k7, k8;

	const NiPoint3 Right = _k2 - _k1;

	float len = Right.Length();

	NiPoint3 Up(0.0f, 0.707f, 0.707f);
	NiPoint3 Out = Right.Cross(Up);
	Out.Unitize();

	k1 = _k1 + halfwidth * Up;
	k1 = k1 - halfwidth * Out;

	k2 = _k1 - halfwidth * Up;
	k2 = k2 - halfwidth * Out;

	k3 = _k1 - halfwidth * Up;
	k3 = k3 + halfwidth * Out;

	k4 = _k1 + halfwidth * Up;
	k4 = k4 + halfwidth * Out;

	
	k5 = k1 + Right;
	k6 = k2 + Right;
	k7 = k3 + Right;
	k8 = k4 + Right;

	DrawRect(k1, k2, k3, k4, kColor);
	DrawRect(k1, k5, k6, k2, kColor);
	DrawRect(k1, k4, k8, k5, kColor);
	DrawRect(k7, k3, k2, k6, kColor);
	DrawRect(k7, k6, k5, k8, kColor);
	DrawRect(k7, k8, k4, k3, kColor);
}

void C3DLinesManager::DrawBox(const NiPoint3 & k, float halfWidth, const NiColorA & kColor)
{
	NiPoint3 k1(k), k2(k), k3(k), k4(k), k5(k), k6, k7, k8;
	k1.x -= halfWidth;
	k1.y -= halfWidth;
	k1.z += halfWidth;

	k2.x -= halfWidth;
	k2.y -= halfWidth;
	k2.z -= halfWidth;

	k3.x -= halfWidth;
	k3.y += halfWidth;
	k3.z -= halfWidth;

	k4.x -= halfWidth;
	k4.y += halfWidth;
	k4.z += halfWidth;

	k5.x += halfWidth;
	k5.y -= halfWidth;
	k5.z += halfWidth;

	NiPoint3 kTemp = k5 - k1;
	k6 = k2 + kTemp;
	k7 = k3 + kTemp;
	k8 = k4 + kTemp;

	DrawRect(k1, k2, k3, k4, kColor);
	DrawRect(k1, k5, k6, k2, kColor);
	DrawRect(k1, k4, k8, k5, kColor);
	DrawRect(k7, k3, k2, k6, kColor);
	DrawRect(k7, k6, k5, k8, kColor);
	DrawRect(k7, k8, k4, k3, kColor);
}

void C3DLinesManager::DrawBoxWire(const NiPoint3 & k1, const NiPoint3 & k2, 
							  const NiPoint3 & k3, const NiPoint3 & k4, const NiColorA & kColor)
{

}
