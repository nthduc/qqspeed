#include "Top-KartPCH.h"
#include "GlobalMap.h"
#include "CheckEditor.h"
#include "../Utility.h"
#include "../NxSceneMgr.h"
#include "../SceneLevel.h"
#include "../levelmanager.h"

CGlobalMap * CGlobalMap::ms_pGlobalMap = 0;

bool CGlobalMap::Create()
{
	if (!ms_pGlobalMap)
	{
		ms_pGlobalMap = NiNew CGlobalMap();

		if (!ms_pGlobalMap || !ms_pGlobalMap->Init()) 
		{
			assert(0);
			return false;
		}
	}

	return true;
}

CGlobalMap * CGlobalMap::Get()
{
	return ms_pGlobalMap;
}

void CGlobalMap::Destroy()
{
	SAFE_NIDELETE(ms_pGlobalMap);
}

CGlobalMap::CGlobalMap(void)
{
	m_kLeftUp = NiPoint2(-135.868f, 137.218f);
	m_kRightDown = NiPoint2(166.819f, -96.602f);
	m_fAspect = 4.f/3.f;
	m_pkTexCoords0 = 0;
	m_pkTexCoords1 = 0;
	m_fMapSize = 0.7f;
	m_fCheckMarkSize = 0.1f;
	m_bShow = false;
	m_spFont = 0; 
}

CGlobalMap::~CGlobalMap(void)
{
	for (unsigned int i = 0; i < m_arrCheckMarks.size(); ++i)
	{
		DestroyCheckMark(m_arrCheckMarks[i]);
	}
    m_arrCheckMarks.clear();
	SAFE_NIDELETE_ARRAY(m_pkTexCoords0);
	SAFE_NIDELETE_ARRAY(m_pkTexCoords1);

	m_spFont = 0; 
}

void CGlobalMap::ShowGlobalMap(bool bShow)
{
	m_bShow = bShow;
}

bool CGlobalMap::Init()
{
	m_spGlobalMap = NiNew NiScreenElements(NiNew NiScreenElementsData(false, false, 1));
	m_spGlobalMap->Insert(4);

	const NiRenderTargetGroup* pkRTGroup = 
		NiRenderer::GetRenderer()->GetDefaultRenderTargetGroup();
	float fRendererWidth = (float)pkRTGroup->GetWidth(0);
	float fRendererHeight = (float)pkRTGroup->GetHeight(0);
	m_fAspect =  fRendererWidth / fRendererHeight;

	m_kMapScreenLeftTop = NiPoint2(0.5f - m_fMapSize/2.f, 0.5f - m_fMapSize / 2.f);
	m_spGlobalMap->SetRectangle(0, m_kMapScreenLeftTop.x, 
		m_kMapScreenLeftTop.y, m_fMapSize, m_fMapSize );//* m_fAspect);
	m_spGlobalMap->UpdateBound();
	m_spGlobalMap->SetTextures(0, 0, 0.0f, 0.0f, 1.0f, 1.0f);

	NiTexturingPropertyPtr spGlobalMapTexProperty = NiNew NiTexturingProperty(
		NiApplication::ConvertMediaFilename("Map/Common Map/Map01/map.tga"));
	spGlobalMapTexProperty->SetApplyMode(NiTexturingProperty::APPLY_REPLACE);
	spGlobalMapTexProperty->SetBaseClampMode(NiTexturingProperty::CLAMP_S_CLAMP_T);
	m_spGlobalMap->AttachProperty(spGlobalMapTexProperty);

	m_spGlobalMap->UpdateProperties();
	m_spGlobalMap->Update(0.0f);

	m_spFont = NiFont::Create(NiRenderer::GetRenderer(),
		NiApplication::ConvertMediaFilename("ScreenOverLay/Menu.NFF"));

	m_pkTexCoords0 = NiNew NiPoint2[4];

	m_pkTexCoords0[0] = NiPoint2(0, 0);
	m_pkTexCoords0[3] = NiPoint2(0.5f, 0);
	m_pkTexCoords0[2] = NiPoint2(0.5f, 1);
	m_pkTexCoords0[1] = NiPoint2(0, 1);

	m_pkTexCoords1 = NiNew NiPoint2[4];

	m_pkTexCoords1[0] = NiPoint2(0.5f, 0);
	m_pkTexCoords1[3] = NiPoint2(1, 0);
	m_pkTexCoords1[2] = NiPoint2(1, 1);
	m_pkTexCoords1[1] = NiPoint2(0.5f, 1);

	CLinks * pLinks = CLevelManager::Get()->GetLinks();
	if (!pLinks) 
	{
		assert(0);
		return false;
	}

	for (int i = 0; i < pLinks->GetCheckPointsNum(); ++i)
	{
		NiPoint3 kPos =	pLinks->GetCheckPointPos(i);
		ScreenCheckMark * pMark = CreateCheckMark(kPos, pLinks->GetCheckPointName(i));
		m_arrCheckMarks.push_back(pMark);
	}


	if (!m_spGlobalMap) 
	{
		return false;
	}

	if (!CLevelManager::Get()->GetLinks()) {
		return false;
	}
	CLevelManager::Get()->GetLinks()->Register(this);

	return true;
}

bool CGlobalMap::IsVisible()
{
	return m_bShow;
}

void CGlobalMap::Draw()
{
	if (!m_bShow) 
	{
		return;
	}
	m_spGlobalMap->Draw(NiRenderer::GetRenderer());

	for (unsigned int i = 0; i < m_arrCheckMarks.size(); ++i)
	{
		m_arrCheckMarks[i]->spScreenMark->Draw(NiRenderer::GetRenderer());
		m_arrCheckMarks[i]->spNameString->Draw(NiRenderer::GetRenderer());
	}

}


CGlobalMap::ScreenCheckMark * CGlobalMap::CreateCheckMark(const NiPoint3& kPos, const NiFixedString & kName)
{
	ScreenCheckMark * pMark = NiNew ScreenCheckMark;

	pMark->kName = kName;

	pMark->bHiLight = false;

	static const float fWidth =  m_kRightDown.x - m_kLeftUp.x;
	static const float fHeight= -m_kRightDown.y + m_kLeftUp.y;

	float fX = kPos.x - m_kLeftUp.x;
	float fY = fabs(kPos.y - m_kLeftUp.y);

	fX /= fWidth;
	fY /= fHeight;


	pMark->spScreenMark = NiNew NiScreenElements(NiNew NiScreenElementsData(false, false, 1));
	pMark->spScreenMark->Insert(4);

	for (int i = 0; i < 4; ++i)
	{
		pMark->spScreenMark->SetVertex(0, i, NiPoint2(m_pkTexCoords1[i].x,
			m_pkTexCoords1[i].y / m_fAspect));
		pMark->spScreenMark->SetTexture(0, i, 0, m_pkTexCoords1[i]);
	}

	pMark->spScreenMark->UpdateBound();
	
	NiTexturingPropertyPtr spGlobalMapTexProperty = NiNew NiTexturingProperty(
		NiApplication::ConvertMediaFilename("Map/Common Map/anniu.tga"));
	spGlobalMapTexProperty->SetApplyMode(NiTexturingProperty::APPLY_REPLACE);
	spGlobalMapTexProperty->SetBaseClampMode(NiTexturingProperty::CLAMP_S_CLAMP_T);

	pMark->spScreenMark->AttachProperty(spGlobalMapTexProperty);

	NiAlphaPropertyPtr spAlpha = NiNew NiAlphaProperty;
	spAlpha->SetAlphaBlending(true);
	//spAlpha->SetAlphaTesting(true);
	//spAlpha->SetTestMode(NiAlphaProperty::TEST_GREATEREQUAL);
    spAlpha->SetSrcBlendMode(NiAlphaProperty::ALPHA_SRCALPHA);
	spAlpha->SetDestBlendMode(NiAlphaProperty::ALPHA_ONE);

	pMark->spScreenMark->AttachProperty(spAlpha);

	pMark->spScreenMark->UpdateProperties();
	
	pMark->spScreenMark->SetTranslate(NiPoint3(m_kMapScreenLeftTop.x + fX * m_fMapSize  - m_fCheckMarkSize * 0.375f, 
		m_kMapScreenLeftTop.y + fY * m_fMapSize - m_fCheckMarkSize * 0.25f, 0.f));
	pMark->spScreenMark->SetScale(m_fCheckMarkSize * 0.5f);
	pMark->spScreenMark->Update(0.0f);

	const char * strTemp = (const char *)kName;
	const char * strText = strTemp + (strlen(strTemp) - 2);

	NiColorA kColor(0.9f, 0.1f, 0.2f, 1.0f);
	pMark->spNameString = NiNew NiString2D(m_spFont, NiFontString::COLORED, 32, 
		strText, kColor, (unsigned int)((m_kMapScreenLeftTop.x + fX * m_fMapSize  - m_fCheckMarkSize * 0.375f) * 1024.f), 
		(unsigned int)((m_kMapScreenLeftTop.y + fY * m_fMapSize - m_fCheckMarkSize * 0.25f) * 768.f));
	return pMark;
}

void CGlobalMap::DestroyCheckMark(ScreenCheckMark*& pMark)
{
	if (pMark) 
	{
		pMark->spNameString = 0;
		pMark->spScreenMark = 0;
		SAFE_NIDELETE(pMark);
	}
}

void CGlobalMap::ShiftState(bool bHiLight, CGlobalMap::ScreenCheckMark * pScreenElems)
{
	NiPoint2 * pkTexCoords = NULL;

	if (bHiLight == pScreenElems->bHiLight) 
	{
		return;
	}
	float fDelta = 0.0f;
	if (bHiLight) 
	{
		pkTexCoords = m_pkTexCoords0;
		fDelta      = +m_fCheckMarkSize * 0.25f;
	}
	else
	{
		pkTexCoords = m_pkTexCoords1;
		fDelta      = -m_fCheckMarkSize * 0.25f;
	}

	pScreenElems->bHiLight = bHiLight;

	for (int i = 0; i < 4; ++i)
	{
		pScreenElems->spScreenMark->SetVertex(0, i, NiPoint2(pkTexCoords[i].x,
			pkTexCoords[i].y / m_fAspect));
		pScreenElems->spScreenMark->SetTexture(0, i, 0, pkTexCoords[i]);		
	}

	if (pScreenElems->bHiLight) {
		pScreenElems->spNameString->SetColor(NiColorA(0.0f, 1.0f, 0.1f, 1.0f));
	}
	else
	{
		pScreenElems->spNameString->SetColor(NiColorA(0.9f, 0.1f, 0.2f, 1.0f));
	}
	
	pScreenElems->spScreenMark->SetTranslate(pScreenElems->spScreenMark->GetTranslate() + NiPoint3(fDelta, 0, 0));
	pScreenElems->spScreenMark->Update(0.0f);
}

void CGlobalMap::HiLight(const NiFixedString & kName, bool bHiLight)
{
	size_t size = m_arrCheckMarks.size();
	for (unsigned int i = 0; i < size; ++i)
	{
		if (m_arrCheckMarks[i]->kName == kName) 
		{
			ShiftState(bHiLight, m_arrCheckMarks[i]);
			break;
		}
	}
}

void CGlobalMap::HiLightAll(bool bHiLight)
{
	size_t size = m_arrCheckMarks.size();
	for (unsigned int i = 0; i < size; ++i)
	{
		ShiftState(bHiLight, m_arrCheckMarks[i]);
	}	
}

void CGlobalMap::OnLapUp()
{
	HiLightAll(false);
}

void CGlobalMap::OnEnterPoint(CCheckPoint * pChkPt)
{
	HiLight(pChkPt->kName, true);
}

void CGlobalMap::OnLeavePoint(CCheckPoint * pChkPt)
{
	
}
