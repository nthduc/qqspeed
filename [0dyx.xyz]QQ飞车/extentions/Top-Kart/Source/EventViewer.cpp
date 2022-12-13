#include "Top-KartPCH.h"
#include "EventViewer.h"
#include "common.h"
#include "Utility.h"
#include "variant.h"
#include "NxMath.h"

#define BACK_MIRROR_PIC "Map/BackEvent.dds"
#define ITEM_MIRROR_PIC "Map/ItemEvent.dds"

#define ITEM_VIEWER_VERTEX_NUM 22
#define BACK_VIEWER_VERTEX_NUM 4

#define DISSAPEAR_STAGE1 (DISSAPEAR_TIME * 0.75F)
#define DISSAPEAR_STAGE2 (DISSAPEAR_TIME * 0.50F)
#define DISSAPEAR_STAGE3 (DISSAPEAR_TIME * 0.25F)
#define DISSAPEAR_STAGE4 (DISSAPEAR_TIME * 0.05F)

EventViewer * EventViewer::CreateEventViewer(NiNode * pkNodeScene, EventViewer::ViewerType viewerType)
{
	EventViewer * pkViewer = NULL;

	switch(viewerType) {
	case BACK_VIEWER:
		{
			pkViewer = NiNew BackEventViewer(pkNodeScene);
		}
		break;
	case EVENT_VIEWER:
		{
			pkViewer = NiNew ItemEventViewer(pkNodeScene);
		}
		break;
	default:
		{
			LogText("EventViewer::CreateEventViewer Failed!, ViewerType = Unknown");
			assert(0);
		}
		break;
	}

	return pkViewer;
}

const NiColorA EventViewer::m_kBackGroundColor = NiColorA(0.2f, 0.2f, 0.2f, 0.1f);

EventViewer::EventViewer(NiNode * pkNodeScene)
: m_pkNodeScene(pkNodeScene)
, m_spCamera(0)
, m_fXScale(1.0f)
, m_fYScale(1.0f)
, m_fTotalRotate(0.0f)
, m_bVisible(false)
, m_bStartEvent(false)
, m_ViewerType(VIEWER_NONE)
, m_enStage(STAGE_NORMAL)
{
	assert(m_pkNodeScene);
	m_pkRenderer = NiRenderer::GetRenderer();
	assert(m_pkRenderer);
	const NiRenderTargetGroup* pkRTGroup = 
		m_pkRenderer->GetDefaultRenderTargetGroup();
	m_fRenderWidth = (float)pkRTGroup->GetWidth(0);
	m_fRenderHeight = (float)pkRTGroup->GetHeight(0);
	m_fAspectRatio = m_fRenderWidth / m_fRenderHeight;

	m_fViewerSize = 0.25f;	
}

EventViewer::~EventViewer(void)
{
	m_pkNodeScene = 0;
	m_pkRenderer = 0;
	m_spEventRegion = 0;
	m_spEventRegionMask = 0;
	m_spRenderTargetGroup = 0;		

	unsigned int uiVerTextNum = m_arrRect.GetEffectiveSize();
	for (unsigned int i = 0; i < uiVerTextNum; ++i)
	{
		NiPoint2 * pkPoint = m_arrRect.GetAt(i);
		SAFE_NIDELETE(pkPoint);
	}

	m_arrRect.RemoveAll();
}

void EventViewer::Destroy()
{
	NiDelete this;
}

void EventViewer::ReplaceDisplayTextureWithRenderedTexture()
{
	assert(m_spEventRegion);

	NiTexturingPropertyPtr spEyeMapTexProperty = (NiTexturingProperty*)m_spEventRegion->GetProperty(NiProperty::TEXTURING);
	assert(spEyeMapTexProperty);
	//不传format参数，Tnt2显卡会无法创建Texture
	NiTexture::FormatPrefs kFormatPerfs; 
	kFormatPerfs.m_ePixelLayout = NiTexture::FormatPrefs::TRUE_COLOR_32;
	kFormatPerfs.m_eMipMapped = NiTexture::FormatPrefs::YES;
	kFormatPerfs.m_eAlphaFmt = NiTexture::FormatPrefs::SMOOTH;
	m_spTexture = NiRenderedTexture::Create(512,512, m_pkRenderer,kFormatPerfs);
	assert(m_spTexture);

	m_spRenderTargetGroup = NiRenderTargetGroup::Create(
		m_spTexture->GetBuffer(), m_pkRenderer, true, true);
	assert(m_spRenderTargetGroup);

	spEyeMapTexProperty->SetBaseTexture(m_spTexture);

	spEyeMapTexProperty->SetApplyMode(NiTexturingProperty::APPLY_REPLACE);
}

NiScreenElements * EventViewer::CreateScreenElement(bool bWithRes)
{
	NiScreenElements *  pkScreenElements = NiNew NiScreenElements(NiNew NiScreenElementsData(false, false, 1));	

	if (m_uiVertexNum != 4) 
	{
		unsigned int uiTriangleNum = 20;
		unsigned short *ausTriList = new unsigned short[uiTriangleNum * 3];

		int triNum = 0;
		int x = 2;
		for (unsigned int i = 0; (i < m_uiVertexNum - 1) && i % 2 == 0; i += 2, triNum += 3)
		{			
			ausTriList[triNum] = i;
			ausTriList[triNum + 1] = i + 1;
			ausTriList[triNum + 2] = (i + 2) >= m_uiVertexNum ? 0 : (i + 2);
		}

		for (unsigned int i = 0; (i < m_uiVertexNum - 2) && i % 4 == 0; i += 4, triNum += 3)
		{
			ausTriList[triNum] = i;
			ausTriList[triNum + 1] = i + 2;
			ausTriList[triNum + 2] = (i + 4) >= m_uiVertexNum ? 0 : (i + 4);
		}

		for (unsigned int i = 0; (i < m_uiVertexNum - 4) && i % 8 == 0; i += 8, triNum += 3)
		{
			ausTriList[triNum] = i;
			ausTriList[triNum + 1] = i + 4;
			ausTriList[triNum + 2] = (i + 8) >= m_uiVertexNum ? 0 : (i + 8);
		}

		for (unsigned int i = 0; (i < m_uiVertexNum - 8) && i %16 == 0; i+= 16, triNum += 3)
		{
			ausTriList[triNum] = i;
			ausTriList[triNum + 1] = i + 8;
			ausTriList[triNum + 2] = (i + 16) >= m_uiVertexNum ? 0 : (i + 16);
		}

		pkScreenElements->Insert(m_uiVertexNum, triNum / 3, ausTriList);
		delete[] ausTriList;

		NiPoint2 * pkPoint = NULL;
		for (unsigned int i = 0; i < m_uiVertexNum; ++i)
		{
			pkPoint = m_arrRect.GetAt(i);	

			pkScreenElements->SetVertex(0, i, m_kLeftUpPosition + NiPoint2(pkPoint->x * m_fElementWidth, pkPoint->y * m_fElementHeight));//*pkPoint);//
			pkScreenElements->SetTexture(0, i, 0, *pkPoint);			
		}

	}
	else
	{
		pkScreenElements->Insert(4);

		if (m_ViewerType == BACK_VIEWER)
		{			
			float fDelta = 3.0f/512.f;
			float fDelta1 = 2.0f/512.f;
			float left = 138.f/512.f;
			float top  = 108.f/512.f;

				float right = 375.f/512.f;
			float bottom = 405.f/512.f;
			if (bWithRes)
			{
				pkScreenElements->SetRectangle(0, m_kLeftUpPosition.x, m_kLeftUpPosition.y, m_fElementWidth, m_fElementHeight);
				pkScreenElements->SetTextures(0, 0, left-fDelta, top-fDelta, right+fDelta, bottom + fDelta);
			}
			else
			{
				pkScreenElements->SetRectangle(0, m_kLeftUpPosition.x+fDelta1, m_kLeftUpPosition.y+fDelta1, m_fElementWidth-fDelta1*2.f, m_fElementHeight-fDelta1*2.f);
				pkScreenElements->SetTextures(0, 0, left+fDelta1, top+fDelta1, right-fDelta1, bottom-fDelta1);
			}

		}
		else
		{
			pkScreenElements->SetTextures(0, 0, 0.0f, 0.0f, 1.0f, 1.0f);
		}
	}

	pkScreenElements->UpdateBound();

	NiTexturingProperty * pkTexProperty = NULL;
	if (bWithRes) 
	{
		pkTexProperty = NiNew NiTexturingProperty(NiApplication::ConvertMediaFilename(m_kResPath));
	}
	else
	{
		pkTexProperty = NiNew NiTexturingProperty();
	}

	pkTexProperty->SetApplyMode(NiTexturingProperty::APPLY_REPLACE);
	pkTexProperty->SetBaseClampMode(NiTexturingProperty::CLAMP_S_CLAMP_T);
	pkScreenElements->AttachProperty(pkTexProperty);

	// use vertex colors

	NiVertexColorProperty* pkVertex = NiNew NiVertexColorProperty;
	pkVertex->SetSourceMode(NiVertexColorProperty::SOURCE_IGNORE);
	pkVertex->SetLightingMode(NiVertexColorProperty::LIGHTING_E);
	pkScreenElements->AttachProperty(pkVertex);


	if (bWithRes)
	{
		NiAlphaProperty* pkAlpha = NiNew NiAlphaProperty;
		pkAlpha->SetAlphaBlending(true);	
		pkScreenElements->AttachProperty(pkAlpha);
	}

	pkScreenElements->UpdateProperties();
	pkScreenElements->Update(0.0f);

	return pkScreenElements;
}
bool EventViewer::LoadSetting()
{
	const char * pcItemName = NULL;
	if (m_ViewerType == EVENT_VIEWER) 
	{
		pcItemName = "itemviewer";
	}
	else if (m_ViewerType == BACK_VIEWER)
	{
		pcItemName = "backviewer";
	}
	else
	{
		return false;
	}

	CVarTable viewerSetting(pcItemName);
	CVar::LoadVarsFromFile(&viewerSetting, NiApplication::ConvertMediaFilename("Map/viewer_setting.lua"));

	viewerSetting.Element("pos")->FloatArray((float*)&m_kLeftUpPosition, 2);

	m_kLeftUpPosition.x /= m_fRenderWidth;
	m_kLeftUpPosition.y /= m_fRenderHeight;

	m_fElementWidth  = viewerSetting.Element("width")->Float() / m_fRenderWidth;
	m_fElementHeight = viewerSetting.Element("height")->Float()/ m_fRenderHeight;

	float fGraphWidth = viewerSetting.Element("graphwidth")->Float();
	float fGraphHeight = viewerSetting.Element("graphheight")->Float();

	CVarArray * pkTexArray = (CVarArray*)viewerSetting.Element("texes");

	assert(pkTexArray && pkTexArray->GetType() == VAR_ARRAY);

	unsigned int uiNum = pkTexArray->GetNum(); 

	NiPoint2 * pkText = NULL;
	for (int i = 0; i < uiNum; ++i)
	{
		pkText = NiNew NiPoint2;
		pkTexArray->Element(i)->FloatArray((float*)pkText, 2);

		pkText->x /= fGraphWidth;
		pkText->y /= fGraphHeight;

		m_arrRect.Add(pkText);
	}

	m_uiVertexNum = m_arrRect.GetEffectiveSize();

	return true;
}

void EventViewer::DrawScreen()
{
	if (!m_bVisible)
	{
		return;
	}

	m_spEventRegion->Draw(m_pkRenderer);
	m_spEventRegionMask->Draw(m_pkRenderer);
}

void EventViewer::RenderScreen(NiCullingProcess & kCuller)
{
	if(!m_bVisible || !m_spCamera || !m_pkNodeScene)
	{
		return;
	}

	m_pkRenderer->SetBackgroundColor(m_kBackGroundColor);
	m_pkRenderer->BeginUsingRenderTargetGroup(m_spRenderTargetGroup, 
		NiRenderer::CLEAR_ALL); 

	//NiDrawScene(m_spCamera, m_pkNodeScene, kCuller);
	NiVisibleArray &kArray = GetMainApp()->GetVisibleArray();
	NiCullScene(m_spCamera, m_pkNodeScene, kCuller, kArray);
	NiDrawVisibleArray(m_spCamera, kArray);

	NxPlayMgr::DrawAllPlay(m_spCamera);

	m_pkRenderer->EndUsingRenderTargetGroup();
}

bool EventViewer::ReSize(NiScreenElements * pkScreenElem, const NiPoint2 & kLeftUpPos, float fXScale, float fYScale)
{
	if (fXScale > 0.0f && fYScale > 0.0f)
	{
		NiGeometryData * pkGeometryData = pkScreenElem->GetModelData();
		NiPoint3* pkVertex = pkGeometryData->GetVertices();//pkScreenElem->GetVertices(0);
		unsigned short usVerticesNum = pkGeometryData->GetVertexCount();
		
		for (int i = 0; i < (int)usVerticesNum; i++)
		{
			pkVertex->x = fXScale * (pkVertex->x - kLeftUpPos.x) + kLeftUpPos.x;
			pkVertex->y = fYScale * (pkVertex->y - kLeftUpPos.y) + kLeftUpPos.y;
			pkVertex++;
		}
		pkGeometryData->MarkAsChanged(NiGeometryData::VERTEX_MASK);
		pkScreenElem->UpdateBound();
		return true;
	}
	return false;
}

void EventViewer::SetScale(float fXScale, float fYScale)
{
	if (!m_spEventRegion || !m_spEventRegionMask) 
	{
		assert(0);
		return;
	}

	ReSize(m_spEventRegion, m_kLeftUpPosition, fXScale * m_fXScale, fYScale * m_fYScale);
	ReSize(m_spEventRegionMask, m_kLeftUpPosition, fXScale * m_fXScale, fYScale * m_fYScale);	

	m_fXScale = 1.0f / fXScale;
	m_fYScale = 1.0f / fYScale;

	//m_spEventRegion->Update(0.0f);
	//m_spEventRegionMask->Update(0.0f);
}

void EventViewer::Rotate(NiScreenElements * pkScreenElem, const NiPoint2 & kPos, float fAngle)
{
	NiGeometryData * pkGeometryData = pkScreenElem->GetModelData();
	NiPoint3* pkVertex = pkGeometryData->GetVertices();//pkScreenElem->GetVertices(0);
	unsigned short usVerticesNum = pkGeometryData->GetVertexCount();

	// Deg to rad
	float fRad = NxMath::degToRad(fAngle);

	//m_fTotalRotate += fRad;
	float fSin = 0.0f, fCos = 0.0f;
	NiSinCos(fRad, fSin, fCos);

	float fX1 = 0.0f, fY1 = 0.0f, fLen = 0.0f;

	for (int i = 0; i < (int)usVerticesNum; i++)
	{
		fX1 = (pkVertex->x - kPos.x);
		fY1 = (pkVertex->y - kPos.y);		
		pkVertex->x = (fX1 * fCos - fY1 * fSin) + kPos.x;
		pkVertex->y = (fY1 * fCos + fX1 * fSin) + kPos.y;
		pkVertex++;
	}
	pkGeometryData->MarkAsChanged(NiGeometryData::VERTEX_MASK);
	pkScreenElem->UpdateBound();
}

void EventViewer::SetRotate(NiPoint2 kPos, float fAngle)
{
	m_fTotalRotate += fAngle;
	m_fTotalRotate = NxMath::mod(m_fTotalRotate, 360);
	Rotate(m_spEventRegion, NiPoint2(0, 0), fAngle);
	Rotate(m_spEventRegionMask,  NiPoint2(0, 0), fAngle);
}

ItemEventViewer::ItemEventViewer(NiNode * pkNodeScene)
: EventViewer(pkNodeScene)
{
	m_ViewerType  = EVENT_VIEWER;
	m_uiVertexNum = ITEM_VIEWER_VERTEX_NUM;
	m_kResPath    = ITEM_MIRROR_PIC;

	if (!LoadSetting())
	{
		LogText("ItemEventViewer::ItemEventViewer, Load Setting Failed!");
		assert(0);
		return;
	}

												  		
	m_spEventRegion		= CreateScreenElement(false);
	m_spEventRegionMask = CreateScreenElement(true);

	assert(m_spEventRegion && m_spEventRegionMask);

	ReplaceDisplayTextureWithRenderedTexture();	
}

ItemEventViewer::~ItemEventViewer()
{

}

void ItemEventViewer::Update(float fDeltaTime)
{
	if (STAGE_NORMAL == m_enStage) 
	{
		return;
	}

	if ( m_fCloseLife < 0.0f) 
	{
		if (STAGE_START == m_enStage)
		{
			m_enStage = STAGE_NORMAL;
			SetRotate(NiPoint2/*(-20, -20)*/::ZERO, -m_fTotalRotate);
			
		}
		if (STAGE_DISSAPEAR == m_enStage)
		{
			m_enStage = STAGE_NORMAL;
			SetRotate(NiPoint2/*(-20, -20)*/::ZERO, -m_fTotalRotate);
			m_bVisible = false;
		}
		return;
	}

	if (fDeltaTime > DISSAPEAR_TIME) 
	{
		fDeltaTime = DISSAPEAR_TIME;
	}
	if (STAGE_START == m_enStage)
	{
		if (m_bStartEvent)
		{
			SetRotate(NiPoint2/*(-20, -20)*/::ZERO, -m_fTotalRotate-90);
			m_bVisible = true;
			m_bStartEvent = false;
		}
		else
			SetRotate(NiPoint2/*(-20, -20)*/::ZERO, fDeltaTime / DISSAPEAR_TIME * 90);
	}
	if (STAGE_DISSAPEAR == m_enStage)
	{
		SetRotate(NiPoint2/*(-20, -20)*/::ZERO, -fDeltaTime / DISSAPEAR_TIME * 90);
	}
	m_fCloseLife -= fDeltaTime;
}



BackEventViewer::BackEventViewer(NiNode * pkNodeScene)
: EventViewer(pkNodeScene)
{
	m_ViewerType  = BACK_VIEWER;
	m_uiVertexNum = BACK_VIEWER_VERTEX_NUM;
	m_kResPath    = BACK_MIRROR_PIC;

	if (!LoadSetting())
	{
		LogText("BackEventViewer::BackEventViewer, Load Setting Failed!");
		assert(0);
		return;
	}

	m_spEventRegion		= CreateScreenElement(false);
	m_spEventRegionMask = CreateScreenElement(true);

	assert(m_spEventRegion && m_spEventRegionMask);

	ReplaceDisplayTextureWithRenderedTexture();
}

BackEventViewer::~BackEventViewer()
{

}

void BackEventViewer::Update(float fDeltaTime)
{
	if (STAGE_NORMAL == m_enStage) 
	{
		return;
	}

	if ( m_fCloseLife < 0.0f) 
	{
		if (STAGE_START == m_enStage)
		{
			m_enStage = STAGE_NORMAL;			
		}
		if (STAGE_DISSAPEAR == m_enStage)
			m_bVisible = false;
		return;
	}

	if (m_bStartEvent) {
		m_bVisible = true; 
		m_bStartEvent = false;
	}

	float fRest = m_fCloseLife - fDeltaTime;
    
	if (STAGE_START == m_enStage)
	{
		if (m_fCloseLife > DISSAPEAR_STAGE1 &&  fRest < DISSAPEAR_STAGE1) 
		{
			SetScale(0.2f, 0.6f);			
		}
		else if (m_fCloseLife > DISSAPEAR_STAGE2 && fRest < DISSAPEAR_STAGE2)
		{
			SetScale(0.8f, 1.3f);
			
		}
		else if (m_fCloseLife > DISSAPEAR_STAGE3 && fRest < DISSAPEAR_STAGE3)
		{
			SetScale(1.0f, 1.0f);
		}		
	}

	else if (STAGE_DISSAPEAR == m_enStage)
	{
		if (m_fCloseLife > DISSAPEAR_STAGE1 &&  fRest < DISSAPEAR_STAGE1) 
		{
			SetScale(1.5f, 0.7f);
		}
		else if (m_fCloseLife > DISSAPEAR_STAGE2 && fRest < DISSAPEAR_STAGE2)
		{
			SetScale(1.0f, 1.0f);
		}
		else if (m_fCloseLife > DISSAPEAR_STAGE3 && fRest < DISSAPEAR_STAGE3)
		{
			SetScale(0.8f, 1.3f);
		}
		else if (m_fCloseLife > DISSAPEAR_STAGE4 && fRest < DISSAPEAR_STAGE4)
		{
			SetScale(0.2f, 0.6f);
		}
	}
	else
	{
		assert(!("这种情况不可能！"));
	}
	m_fCloseLife = fRest;	
	
}

EventViewerMgr * EventViewerMgr::ms_pEventViewerMgr = 0;

bool EventViewerMgr::CreateEventViewerMgr(NiNode * pkScene)
{
	if (!ms_pEventViewerMgr) 
	{
		if (!pkScene)
			return false;
		ms_pEventViewerMgr = NiNew EventViewerMgr;
		if (!ms_pEventViewerMgr || !ms_pEventViewerMgr->Init(pkScene))
		{
			return false;
		}
	}

	return true;
}

void EventViewerMgr::Destroy()
{
	SAFE_NIDELETE(ms_pEventViewerMgr);
}

EventViewerMgr::EventViewerMgr()
: m_pkBackEventViewer(0)
, m_pkItemEventViewer(0)
, m_bIsViewerOn(true)
{

}

EventViewerMgr::~EventViewerMgr()
{
	if (m_pkBackEventViewer)
		m_pkBackEventViewer->Destroy();
	m_pkBackEventViewer = 0;
	
	if (m_pkItemEventViewer)
		m_pkItemEventViewer->Destroy();
	m_pkItemEventViewer = 0;
}

bool EventViewerMgr::Init(NiNode * pkScene)
{
	m_pkItemEventViewer = (ItemEventViewer*)(ItemEventViewer::CreateEventViewer(pkScene, EventViewer::EVENT_VIEWER));
	m_pkBackEventViewer = (BackEventViewer*)(ItemEventViewer::CreateEventViewer(pkScene, EventViewer::BACK_VIEWER));

	if (m_pkBackEventViewer && m_pkBackEventViewer) 
	{
		return true;
	}

	return false;
}

void EventViewerMgr::StartItemEvent(NiCamera * pkCamera)
{
	if (ms_pEventViewerMgr)
	{
		assert(ms_pEventViewerMgr->m_pkItemEventViewer);
		ms_pEventViewerMgr->m_pkItemEventViewer->SetCameraAndPlayer(pkCamera);
		ms_pEventViewerMgr->m_pkItemEventViewer->Start();
	}
}
void EventViewerMgr::StartBackEvent(NiCamera * pkCamera)
{
	if (ms_pEventViewerMgr)
	{
		assert(ms_pEventViewerMgr->m_pkBackEventViewer);
		ms_pEventViewerMgr->m_pkBackEventViewer->SetCameraAndPlayer(pkCamera);
		ms_pEventViewerMgr->m_pkBackEventViewer->Start();
	}
}

void EventViewerMgr::CloseItemEvent()
{
	if (ms_pEventViewerMgr)
	{
		assert(ms_pEventViewerMgr->m_pkItemEventViewer);
		ms_pEventViewerMgr->m_pkItemEventViewer->Close();
	}
}
void EventViewerMgr::CloseBackEvent()
{
	if (ms_pEventViewerMgr)
	{
		assert(ms_pEventViewerMgr->m_pkBackEventViewer);
		ms_pEventViewerMgr->m_pkBackEventViewer->Close();
	}
}

void EventViewerMgr::Render()
{
	if (ms_pEventViewerMgr)
	{
		if(ms_pEventViewerMgr->m_bIsViewerOn)
		{
			if (ms_pEventViewerMgr->m_pkBackEventViewer) 
			{
				ms_pEventViewerMgr->m_pkBackEventViewer->RenderScreen(GetMainApp()->GetCuller());
			}

			if (ms_pEventViewerMgr->m_pkItemEventViewer)
			{
				ms_pEventViewerMgr->m_pkItemEventViewer->RenderScreen(GetMainApp()->GetCuller());
			}
		}

	}
}
void EventViewerMgr::Draw()
{
	if (ms_pEventViewerMgr)
	{
		if(ms_pEventViewerMgr->m_bIsViewerOn)
		{
			if (ms_pEventViewerMgr->m_pkBackEventViewer) 
			{
				ms_pEventViewerMgr->m_pkBackEventViewer->DrawScreen();
			}

			if (ms_pEventViewerMgr->m_pkItemEventViewer)
			{
				ms_pEventViewerMgr->m_pkItemEventViewer->DrawScreen();
			}
		}		
	}
}
void EventViewerMgr::Update(float fDeltaTime)
{
	if (ms_pEventViewerMgr)
	{
		if(ms_pEventViewerMgr->m_bIsViewerOn)
		{
			if (ms_pEventViewerMgr->m_pkBackEventViewer) 
			{
				ms_pEventViewerMgr->m_pkBackEventViewer->Update(fDeltaTime);
			}

			if (ms_pEventViewerMgr->m_pkItemEventViewer)
			{
				ms_pEventViewerMgr->m_pkItemEventViewer->Update(fDeltaTime);
			}
		}
		
	}
}


void EventViewerMgr::TurnOffViewer()
{
	ms_pEventViewerMgr->m_bIsViewerOn = false;
}


void EventViewerMgr::TurnOnViewer()
{
	ms_pEventViewerMgr->m_bIsViewerOn = true;
}
