////////////////////////////////////////////////
//     FileName:    screenobjects.cpp
//     Author:      Fish
//     Date:        2007-3-26 15:26:08
//     Description: []
////////////////////////////////////////////////

#include "Top-KartPCH.h"
#include "screenobjects.h"
#include "Platform/GlobalInterface.h"
#include "framework/GameApp_i.h"
#include "IDisplayD3D9/Texture9.h"
#include "gpeffect.h"
#include "RMOperator.h"
#include "d3dstate.h"
#include "NxPlayMgr.h"
#pragma comment(lib, "Imm32")

static const NiColorA gs_cBKColor = NiColorA(0.0f, 0.0f, 0.0f, 0.0f);

CScreenObject::CScreenObject(NiAVObject* pkObj, NiCamera* pkCamera, bool bVisible):
m_dwHandle(0),
m_spRootObject(pkObj),
m_bVisible(bVisible), m_spCamera(pkCamera)
{
	NiRenderer * pkRenderer = NiRenderer::GetRenderer();
	assert(pkRenderer);

	//如果没有指定相机，则创建一个默认相机
	if ( !m_spCamera )
	{
		NiStream kStream;
		if ( kStream.Load(NiApplication::ConvertMediaFilename("Avatar/kart_base/10002/Model/car.nif")) )
		{
			NiAVObject* pkObj = NiDynamicCast(NiAVObject, kStream.GetObjectAt(0));
			//m_spCamera = CGPEffectMgr::FindCamera(pkObj);
			m_spCamera = (NiCamera*)((NiNode*)pkObj)->GetObjectByName("selfshow camera");
			_ASSERTE(m_spCamera);
			m_spCamera->Update(0);
		}
	}
	
	int nPortraitWidth = 256;
	CALL(GlobalInterface::m_pSettings, GetIntValue("PortraitWidth", &nPortraitWidth));
	
	NiTexture::FormatPrefs kFormatPerfs; 
	kFormatPerfs.m_ePixelLayout = NiTexture::FormatPrefs::TRUE_COLOR_32;
	kFormatPerfs.m_eMipMapped = NiTexture::FormatPrefs::YES;
	kFormatPerfs.m_eAlphaFmt = NiTexture::FormatPrefs::SMOOTH;
	m_spRenderedTexture = NiRenderedTexture::Create(nPortraitWidth, nPortraitWidth, pkRenderer, kFormatPerfs);
	assert(m_spRenderedTexture);

	m_spRenderTargetGroup = NiRenderTargetGroup::Create(
		m_spRenderedTexture->GetBuffer(), pkRenderer, true, true);
	assert(m_spRenderTargetGroup);

	//渲染目标是正方形的，所以把Frutum也改成1:1的
	const NiFrustum& kFrustum = m_spCamera->GetViewFrustum();
	NiFrustum kFrustrmNew;
	kFrustrmNew.m_bOrtho = kFrustum.m_bOrtho;
	kFrustrmNew.m_fNear = kFrustum.m_fNear;
	kFrustrmNew.m_fFar = kFrustum.m_fFar;
	float fWidth = abs(kFrustum.m_fRight - kFrustum.m_fLeft);
	float fHeight = abs(kFrustum.m_fTop - kFrustum.m_fBottom);
	if ( fWidth > fHeight )
	{
		fWidth = fHeight;
	}
	else
	{
		fHeight = fWidth;
	}
	kFrustrmNew.m_fLeft = -fWidth * 0.5;
	kFrustrmNew.m_fRight = fWidth * 0.5;
	kFrustrmNew.m_fBottom = -fHeight * 0.5;
	kFrustrmNew.m_fTop = fHeight * 0.5;
	m_spCamera->SetViewFrustum(kFrustrmNew);
}

CScreenObject::~CScreenObject()
{
	m_bVisible = false;
}

bool CScreenObject::GetVisible()
{
	return m_bVisible;
}

bool CScreenObject::SetVisible(bool bVisible)
{
	if ( m_bVisible == bVisible )
	{
		return bVisible;
	}

	bool bOld = m_bVisible;
	m_bVisible = bVisible;
	return bOld;
}

void CScreenObject::CacheRender()
{
	if( m_bVisible && m_spCamera )
	{
		NiRenderer * pkRenderer = NiRenderer::GetRenderer();
		assert(pkRenderer);

		m_spRootObject->UpdateProperties();
		m_spRootObject->Update(GetMainApp()->GetAccumTime());

		pkRenderer->SetBackgroundColor(gs_cBKColor);			
		pkRenderer->BeginUsingRenderTargetGroup(m_spRenderTargetGroup, 
			NiRenderer::CLEAR_ALL); 
		NiVisibleArray kArray(100,10);
		NiDrawScene(m_spCamera, m_spRootObject, GetMainApp()->GetDefaultCuller(), &kArray);
		pkRenderer->EndUsingRenderTargetGroup();
	}
}

void CScreenObject::Draw(float x, float y, float w, float h, float fScale, AlignMode Mode)
{
	if ( !m_bVisible || !m_spCamera || GameMain::IsDeviceLost())
	{
		return;
	}

	NiDX9Renderer* pRender = (NiDX9Renderer*) NiRenderer::GetRenderer();

	NiDX9RenderState* pRenderState = pRender->GetRenderState();

	LPDIRECT3DDEVICE9 pD3DDevice9 = pRender->GetD3DDevice();
	NiTexture::RendererData * pRenderData = m_spRenderedTexture->GetRendererData();
	NiDX9TextureData* pDx9Data = NiSmartPointerCast(NiDX9TextureData,pRenderData);
	D3DBaseTexturePtr spD3DBaseTex = pDx9Data->GetD3DTexture();

	float fRealWidth = m_spRenderTargetGroup->GetWidth(0);
	float fRealHeight = m_spRenderTargetGroup->GetHeight(0);

#define VERTEX_COUNT 6

	static PANEL_CUSTOMVERTEX_T      m_vector[VERTEX_COUNT];

	float fleft = x;
	float ftop = y;
	if ( w > fRealWidth )
	{
		w = fRealWidth;
		fleft += ((w - fRealWidth) * 0.5);
	}
	if ( h > fRealHeight )
	{
		h = fRealHeight;
		ftop += ((h - fRealHeight) * 0.5);
	}
	float fright = fleft + w;
	float fbottom = ftop + h;

	float fUMin = (fRealWidth * 0.5 - w * 0.5) / fRealWidth;
	float fUMax = (fRealWidth * 0.5 + w * 0.5) / fRealWidth;
	float fVMin = (fRealHeight * 0.5 - h * 0.5) / fRealHeight;
	float fVMax = (fRealHeight * 0.5 + h * 0.5) / fRealHeight;

	float xcenter = (fleft + fright) * 0.5;
	float ycenter = (ftop + fbottom) * 0.5;

	switch(Mode)
	{
	case AlignMode_Center:
		fleft = xcenter - w * 0.5 * fScale;
		fright = xcenter + w * 0.5 * fScale;
		ftop = ycenter - h * 0.5 * fScale;
		fbottom = ycenter + h * 0.5 * fScale;
		break;
	case AlignMode_TopLeft:
		fleft = x;
		fright = fleft + w * fScale;
		ftop = y;
		fbottom = ftop + h * fScale;
		break;
	}
	/*
	float fleft = 0;
	float fright = fRealWidth;
	float ftop = 0;
	float fbottom = fRealHeight;
	float fUMin = 0;
	float fUMax = 1.0;
	float fVMin = 0;
	float fVMax = 1.0;
	*/

	m_vector[0].x =  fleft;
	m_vector[0].y =  fbottom;
	m_vector[0].z =  1.0f;
	m_vector[0].u =  fUMin;
	m_vector[0].v =  fVMax;
	m_vector[0].rhw = 1.0f;

	m_vector[1].x =  fleft;
	m_vector[1].y =  ftop;
	m_vector[1].z =  1.0f;
	m_vector[1].u =  fUMin;
	m_vector[1].v =  fVMin;
	m_vector[1].rhw = 1.0f;



	m_vector[2].x =  fright;
	m_vector[2].y =  fbottom;
	m_vector[2].z =  1.0f;
	m_vector[2].u =  fUMax;
	m_vector[2].v =  fVMax;
	m_vector[2].rhw = 1.0f;

	m_vector[3] = m_vector[1];

	m_vector[4].x =  fright;
	m_vector[4].y =  ftop;
	m_vector[4].z =  1.0f;
	m_vector[4].u =  fUMax;
	m_vector[4].v =  fVMin;
	m_vector[4].rhw = 1.0f;

	m_vector[5] = m_vector[2];

	//	修改人:	wincoqin
	//  时间:	2007/05/09
	//  目的:	将D3D存取State方式改成NiDX9RenderState存取
	/*CD3DStateMgr kStateMgr(pD3DDevice9);

	kStateMgr.SetFVF(PANEL_D3DFVF_CUSTOMVERTEX_T);
	kStateMgr.SetTexture(0, spD3DBaseTex);
	kStateMgr.SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	kStateMgr.SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	kStateMgr.SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	kStateMgr.SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_SELECTARG1);
	kStateMgr.SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
	kStateMgr.SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);

	kStateMgr.SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_CLAMP);
	kStateMgr.SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_CLAMP);
	kStateMgr.SetSamplerState(0, D3DSAMP_ADDRESSW, D3DTADDRESS_CLAMP);

	kStateMgr.SetSamplerState(0,D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	kStateMgr.SetSamplerState(0,D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	kStateMgr.SetSamplerState(0,D3DSAMP_MIPFILTER, D3DTEXF_LINEAR);*/

	pRenderState->SetFVF(PANEL_D3DFVF_CUSTOMVERTEX_T);
	pRenderState->SetTexture(0, spD3DBaseTex);
	pRenderState->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	pRenderState->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pRenderState->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	pRenderState->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_SELECTARG1);
	pRenderState->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
	pRenderState->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);

	pRenderState->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_CLAMP);
	pRenderState->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_CLAMP);
	pRenderState->SetSamplerState(0, D3DSAMP_ADDRESSW, D3DTADDRESS_CLAMP);

	pRenderState->SetSamplerState(0,D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	pRenderState->SetSamplerState(0,D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	pRenderState->SetSamplerState(0,D3DSAMP_MIPFILTER, D3DTEXF_LINEAR);

	pD3DDevice9->DrawPrimitiveUP(D3DPT_TRIANGLELIST,2,m_vector,sizeof(PANEL_CUSTOMVERTEX_T));


	pRenderState->RestoreSamplerState(0, D3DSAMP_MIPFILTER);
	pRenderState->RestoreSamplerState(0, D3DSAMP_MINFILTER);
	pRenderState->RestoreSamplerState(0, D3DSAMP_MAGFILTER);

	pRenderState->RestoreSamplerState(0, D3DSAMP_ADDRESSW);
	pRenderState->RestoreSamplerState(0, D3DSAMP_ADDRESSV);
	pRenderState->RestoreSamplerState(0, D3DSAMP_ADDRESSU);

	pRenderState->RestoreTextureStageState(0, D3DTSS_ALPHAOP);
	pRenderState->RestoreTextureStageState(0, D3DTSS_COLORARG1);
	pRenderState->RestoreTextureStageState(0, D3DTSS_COLOROP);

	pRenderState->RestoreRenderState(D3DRS_DESTBLEND);
	pRenderState->RestoreRenderState(D3DRS_SRCBLEND);
	pRenderState->RestoreRenderState(D3DRS_ALPHABLENDENABLE);
	pRenderState->RestoreFVF();
	//kStateMgr.Restore();
}

CScreenObjectsMgr::CScreenObjectsMgr()
{
}

CScreenObjectsMgr::~CScreenObjectsMgr()
{
	map<DWORD, CScreenObject*>::iterator it = m_kObjects.begin();
	for ( ; it != m_kObjects.end(); ++it )
	{
		delete it->second;
	}
	m_kObjects.clear();
}

void CScreenObjectsMgr::CacheRender()
{
	map<DWORD, CScreenObject*>::iterator it = m_kObjects.begin();
	for ( ; it != m_kObjects.end(); ++it )
	{
		it->second->CacheRender();
	}
}

DWORD CScreenObjectsMgr::AddScreenObject(NiAVObject* pkObj, NiCamera* pkCamera, bool bVisible)
{
	assert(pkObj);
	CScreenObject* pObj = new CScreenObject(pkObj, pkCamera, bVisible);
	assert(pObj);
	DWORD dwHandle = m_kIDGen.Alloc();
	m_kObjects[dwHandle] = pObj;
	pObj->m_dwHandle = dwHandle;
	return dwHandle;
}

bool CScreenObjectsMgr::RemoveScreenObject(DWORD dwHandle)
{
	map<DWORD, CScreenObject*>::iterator itFind = m_kObjects.find(dwHandle);
	if ( itFind != m_kObjects.end() )
	{
		delete itFind->second;
		m_kObjects.erase(itFind);
		return true;
	}
	return false;
}

bool CScreenObjectsMgr::DrawScreenObject(DWORD dwHandle, float x, float y, float w, float h, float fScale, CScreenObject::AlignMode Mode)
{
	map<DWORD, CScreenObject*>::iterator itFind = m_kObjects.find(dwHandle);
	if ( itFind != m_kObjects.end() )
	{
		CScreenObject* pObj = itFind->second;
		if ( pObj->SetVisible(true) == true )
		{
			pObj->Draw(x, y, w, h, fScale, Mode);
		}
		return true;
	}
	return false;
}

bool CScreenObjectsMgr::SetVisible(DWORD dwHandle, bool bVisible)
{
	map<DWORD, CScreenObject*>::iterator itFind = m_kObjects.find(dwHandle);
	if ( itFind != m_kObjects.end() )
	{
		return itFind->second->SetVisible(bVisible);
	}
	return false;
}
