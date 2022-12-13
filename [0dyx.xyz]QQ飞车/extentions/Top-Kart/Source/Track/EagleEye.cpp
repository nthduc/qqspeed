#include "Top-KartPCH.h"

#include "EagleEye.h"
#include "NiDrawSceneUtility.h"
#include "common.h"
#include "../utility.h"
#include "screendraw.h"
#include "../nifop.h"

CEagleEyeMap::CEagleEyeMap()
: m_spMap(0)
, m_spCamera(0)
, m_spTexture(0)
, m_spEyeMapRegion(0)
, m_spEyeMapMask(0)
, m_spKartMark(0)
,m_bShow(false)
{
	
}

CEagleEyeMap::~CEagleEyeMap()
{
	m_spMapBk = 0;
	m_spTexture = 0;
	m_spKartMark = 0;
	m_spMap = 0;
	m_spEyeMapRegion = 0;
	m_spEyeMapMask = 0;
	m_spCamera = 0;
	m_spRenderTargetGroup = 0;
	m_spEyeMapTexProperty = 0;
	m_spKartRemoteRes = 0;

	for (unsigned int i = 0; i < m_arrRemoteKartMarks.GetEffectiveSize(); ++i)
	{
		RemoteKartMark * pMark = m_arrRemoteKartMarks.GetAt(i);
		SAFE_NIDELETE(pMark);
	}

	m_arrRemoteKartMarks.RemoveAll();
}

bool CEagleEyeMap::Init(const char * pcFilePath)
{
	m_spEyeMapRegion = NiNew NiScreenElements(NiNew NiScreenElementsData(false, false, 1));
	m_spEyeMapRegion->Insert(4);

	// Position the arrow based on the size and position of the map itself

	const NiRenderTargetGroup* pkRTGroup = 
		NiRenderer::GetRenderer()->GetDefaultRenderTargetGroup();
	float fRendererWidth = (float)pkRTGroup->GetWidth(0);
	float fRendererHeight = (float)pkRTGroup->GetHeight(0);
	float fAspect = fRendererWidth / fRendererHeight;

	float fMapSize = 0.23f;
	m_spEyeMapRegion->SetRectangle(0, float(1 - fMapSize - 0.04), 
		float(1 - fMapSize * fAspect - 0.315), fMapSize, fMapSize * fAspect);
	m_spEyeMapRegion->UpdateBound();
	m_spEyeMapRegion->SetTextures(0, 0, 0.0f, 0.0f, 1.0f, 1.0f);

	NiTexturingPropertyPtr spEyeMapTexProperty = NiNew NiTexturingProperty();
	spEyeMapTexProperty->SetApplyMode(NiTexturingProperty::APPLY_REPLACE);
	spEyeMapTexProperty->SetBaseClampMode(NiTexturingProperty::CLAMP_S_CLAMP_T);
	m_spEyeMapRegion->AttachProperty(spEyeMapTexProperty);

	NiAlphaPropertyPtr spAlpha = NiNew NiAlphaProperty;
	spAlpha->SetAlphaBlending(true);
	spAlpha->SetSrcBlendMode(NiAlphaProperty::ALPHA_SRCCOLOR);
	spAlpha->SetDestBlendMode(NiAlphaProperty::ALPHA_INVSRCCOLOR);
	
	m_spEyeMapRegion->AttachProperty(spAlpha);

	m_spEyeMapRegion->UpdateProperties();
	m_spEyeMapRegion->Update(0.0f);

	m_spEyeMapMask = NiNew NiScreenElements(NiNew NiScreenElementsData(false, false, 1));
	m_spEyeMapMask->Insert(4);

	// Position the arrow based on the size and position of the map itself

	fMapSize = 0.25f;
	m_spEyeMapMask->SetRectangle(0, float(1 - fMapSize - 0.03), 
		float(1 - fMapSize * fAspect - 0.3), fMapSize, fMapSize * fAspect);
	m_spEyeMapMask->UpdateBound();
	m_spEyeMapMask->SetTextures(0, 0, 0.0f, 0.0f, 1.0f, 1.0f);

	NiTexturingPropertyPtr spEyeMapMaskTexProperty = NiNew NiTexturingProperty();
	spEyeMapMaskTexProperty->SetApplyMode(NiTexturingProperty::APPLY_REPLACE);
	spEyeMapMaskTexProperty->SetBaseClampMode(NiTexturingProperty::CLAMP_S_CLAMP_T);
	m_spEyeMapMask->AttachProperty(spEyeMapMaskTexProperty);
	NiSourceTexturePtr spMapMaskTex = NiSourceTexture::Create(
		NiApplication::ConvertMediaFilename("Textures/Ð¡µØÍ¼Ô¤ÀÀ¿ò.tga"));
	spEyeMapMaskTexProperty->SetBaseTexture(spMapMaskTex);

	NiAlphaPropertyPtr spMaskAlpha = NiNew NiAlphaProperty;
	spMaskAlpha->SetAlphaBlending(true);
	spMaskAlpha->SetSrcBlendMode(NiAlphaProperty::ALPHA_SRCCOLOR);
	spMaskAlpha->SetDestBlendMode(NiAlphaProperty::ALPHA_INVSRCCOLOR);

	m_spEyeMapMask->AttachProperty(spMaskAlpha);

	m_spEyeMapMask->UpdateProperties();
	m_spEyeMapMask->Update(0.0f);

	NiStream kStream;
//	CStringEx strTmp;
//	strTmp.Format("%smap.nif", pcFilePath);
	if (!kStream.Load(
		NiApplication::ConvertMediaFilename(pcFilePath))) 
	{
		return false;
	}

	m_spMap = NiDynamicCast(NiNode, kStream.GetObjectAt(0));

	if (!kStream.Load(
		NiApplication::ConvertMediaFilename("Map\\KartMark.nif"))) 
	{
		LogText("Load Res Map\\KartMark.nif Failed!\n");
		return false;
	}
	m_spKartMark = NiDynamicCast(NiNode, kStream.GetObjectAt(0));

	if (!kStream.Load(
		NiApplication::ConvertMediaFilename("Map\\RemoteKart.nif"))) 
	{
		LogText("Load Res Map\\RemoteKart.nif Failed!\n");
		return false;
	}
	m_spKartRemoteRes = NiDynamicCast(NiNode, kStream.GetObjectAt(0));

	if (!m_spMap || !m_spKartMark || !m_spKartRemoteRes) 
	{
		assert(0);
		return false;
	}

	//m_spMapBk = CScreenDraw::CreateTextureObject(NiApplication::ConvertMediaFilename("Textures/Ð¡µØÍ¼Ô¤ÀÀ¿ò.tga"));

	m_spMap->AttachChild(m_spKartMark);

	m_spMap->UpdateEffects();
	m_spMap->UpdateProperties();
	m_spMap->UpdateNodeBound();
	m_spMap->Update(0.0f);

	ReplaceDisplayTextureWithRenderedTexture();

	if (!m_spEyeMapRegion) 
	{
		return false;
	}

	return true;
}

void CEagleEyeMap::GetMappedKartPosition(const NiPoint3 & kKartPos)
{
	m_kMappedPos = kKartPos;	
	m_kMappedPos.z += 0.2f;
}

void CEagleEyeMap::UpdateMap(const NiPoint3 & kKartPos, const NiMatrix3 & kRotate, unsigned int unUin, bool bSelf)
{
	if(!m_bShow)
	{
		return;
	}
	GetMappedKartPosition(kKartPos);
	if (bSelf)
	{
		m_spKartMark->SetTranslate(m_kMappedPos);	
		m_spKartMark->SetRotate(kRotate);
		m_spKartMark->Update(0.0f);

		if (!m_spCamera) {
			m_spCamera = NiDynamicCast(NiCamera, NxSceneMgr::Get()->GetCameraController()
				->GetLogicCamera(CCameraBase::ECT_EAGLEMAP)->GetCamera()->Clone());
		}

		NiCamera * pkCamera = NxSceneMgr::Get()->GetCameraController()->GetLogicCamera(CCameraBase::ECT_EAGLEMAP)->GetCamera();

		if (!pkCamera) 
		{
			assert(0);
			return;
		}


		NiPoint3 kTrans = pkCamera->GetTranslate();
		kTrans -= pkCamera->GetWorldDirection() * 70.f - pkCamera->GetWorldUpVector() * 8.f;
		NiMatrix3 kRotate1 = pkCamera->GetRotate();

		NiMatrix3 kZRot10;//kYRot10, 
		//kYRot10.MakeYRotation(NI_PI/18.f);
		kZRot10.MakeZRotation(NI_PI/28.f);
		kRotate1 = kRotate1 * kZRot10;//* kYRot10 

		m_spCamera->SetTranslate(kTrans);
		m_spCamera->SetRotate(kRotate1);
		m_spCamera->Update(0.0f);
	}
	else
	{
		unsigned int unRemotePlayerNum = m_arrRemoteKartMarks.GetEffectiveSize();
		bool bFind = false;
		for (unsigned int i = 0; i < unRemotePlayerNum; ++i)
		{
			RemoteKartMark * pMark = m_arrRemoteKartMarks.GetAt(i);

			if (pMark->unUin == unUin) 
			{
				bFind = true;
				pMark->spRemoteKart->SetTranslate(m_kMappedPos);
				pMark->spRemoteKart->SetRotate(kRotate);
				pMark->spRemoteKart->Update(0.0f);
				return;
			}
		}

		if (!bFind) 
		{
			RemoteKartMark * pMark = NiNew RemoteKartMark;
			pMark->unUin = unUin;
			pMark->spRemoteKart = (NiNode *)(m_spKartRemoteRes->Clone());			
			pMark->spRemoteKart->SetTranslate(m_kMappedPos);
			pMark->spRemoteKart->SetRotate(kRotate);
			pMark->spRemoteKart->Update(0.0f);
			m_arrRemoteKartMarks.Add(pMark);
			m_spMap->AttachChild(pMark->spRemoteKart);

			m_spMap->UpdateEffects();
			m_spMap->UpdateProperties();
			m_spMap->UpdateNodeBound();
			m_spMap->Update(0.0f);
		}
	}
}


const  float BF_CR = 0.4f;
const  float BF_CG = 0.4f;
const  float BF_CB = 0.4f;

static const NiColorA kBackGround = NiColorA(BF_CR,BF_CG,BF_CB,0.1f);

void CEagleEyeMap::Render()
{	
	if(!m_bShow)
	{
		return;
	}

	NiRenderer* pkRender = NiRenderer::GetRenderer();
	pkRender->SetBackgroundColor(kBackGround);
	pkRender->BeginUsingRenderTargetGroup(m_spRenderTargetGroup, 
		NiRenderer::CLEAR_ALL); 

	NiDrawScene(m_spCamera, m_spMap, GetMainApp()->GetDefaultCuller());

	Statistics::AddNode(m_spMap, Statistics::GT_MINIMAP);

	pkRender->EndUsingRenderTargetGroup();
}

void CEagleEyeMap::Draw()
{
	if(!m_bShow)
	{
		return;
	}
	m_spEyeMapRegion->Draw(NiRenderer::GetRenderer());
	m_spEyeMapMask->Draw(NiRenderer::GetRenderer());
}

void CEagleEyeMap::ReplaceDisplayTextureWithRenderedTexture()
{
	m_spEyeMapTexProperty = (NiTexturingProperty*)m_spEyeMapRegion->GetProperty(NiProperty::TEXTURING);
	assert(m_spEyeMapTexProperty);

	NiTexture::FormatPrefs kFormatPerfs; 
	kFormatPerfs.m_ePixelLayout = NiTexture::FormatPrefs::TRUE_COLOR_32;
	kFormatPerfs.m_eMipMapped = NiTexture::FormatPrefs::YES;
	kFormatPerfs.m_eAlphaFmt = NiTexture::FormatPrefs::SMOOTH;
	m_spTexture = NiRenderedTexture::Create(256,256, NiRenderer::GetRenderer(),kFormatPerfs);
	assert(m_spTexture);

	m_spRenderTargetGroup = NiRenderTargetGroup::Create(
		m_spTexture->GetBuffer(), NiRenderer::GetRenderer(), true, true);
	assert(m_spRenderTargetGroup);

	m_spEyeMapTexProperty->SetBaseTexture(m_spTexture);

	m_spEyeMapTexProperty->SetApplyMode(NiTexturingProperty::APPLY_REPLACE);
}
//---------------------------------------------------------------------------
