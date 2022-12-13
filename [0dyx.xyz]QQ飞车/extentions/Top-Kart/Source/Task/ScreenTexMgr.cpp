#include "Top-KartPCH.h"
#include "ScreenTexMgr.h"
#include "TaskScript.h"

CScreenTex::CScreenTex()
{
	m_spScreenElet = NiNew NiScreenElements(NiNew NiScreenElementsData(false, false, 1));
	m_spScreenElet->Insert(4);
	m_spScreenElet->SetTextures(0, 0, 0, 0, 1, 1);
		
	NiTexturingPropertyPtr spTexPro = NiNew NiTexturingProperty();
	spTexPro->SetApplyMode(NiTexturingProperty::APPLY_REPLACE);
	spTexPro->SetBaseClampMode(NiTexturingProperty::CLAMP_S_CLAMP_T);
	m_spScreenElet->AttachProperty(spTexPro);
	
	NiAlphaPropertyPtr spAlphaPro = NiNew NiAlphaProperty();
	spAlphaPro->SetAlphaBlending(true);
	spAlphaPro->SetSrcBlendMode(NiAlphaProperty::ALPHA_SRCALPHA);
	spAlphaPro->SetDestBlendMode(NiAlphaProperty::ALPHA_INVSRCALPHA);
	m_spScreenElet->AttachProperty(spAlphaPro);

	m_spScreenElet->UpdateBound();
	m_spScreenElet->UpdateProperties();
	m_spScreenElet->UpdateEffects();
	m_spScreenElet->Update(0);
}

CScreenTex::~CScreenTex()
{
	m_spScreenElet = 0;
}

void CScreenTex::SetTex(NiSourceTexturePtr spTex)
{
	NiTexturingPropertyPtr spTexPro = NiDynamicCast(NiTexturingProperty, 
					m_spScreenElet->GetProperty(NiProperty::TEXTURING));
	spTexPro->SetBaseTexture(spTex);
	m_spScreenElet->UpdateProperties();
	m_spScreenElet->UpdateEffects();
//	m_spScreenElet->Update(0);	
}

void CScreenTex::SetRectangle(float fLeft, float fTop, float fWidth, float fHeight)
{
	m_spScreenElet->SetRectangle(0, fLeft, fTop, fWidth, fHeight);
	m_spScreenElet->UpdateBound();
}

void CScreenTex::Draw(NiRendererPtr spRenderer)
{
	m_spScreenElet->Draw(spRenderer);
}

void CScreenTex::Update(float fTime)
{
	return;
}


//---------------------------------------------------------------------------------

CTScreenTexMgr::CTScreenTexMgr()
{

}

CTScreenTexMgr::~CTScreenTexMgr()
{

}

bool CTScreenTexMgr::Init()
{
	if(m_lsTex.DoFile("TaskConfig/screentex.lua") == 0)
	{
		CVarArray vtRoot("screentex");
		vtRoot.GetFromScript(&m_lsTex);
		for (int i = 0; i < vtRoot.GetNum(); ++i)
		{
			CVarTable* pSubVar = (CVarTable*)vtRoot.Element(i);
			SCREENTEXCFG tagCfg;
			_TReadIntFromScript(&tagCfg.dwID, pSubVar->Element("id"));
			_TReadFloatFromScript(&tagCfg.fX, pSubVar->Element("x"));
			_TReadFloatFromScript(&tagCfg.fY, pSubVar->Element("y"));
			_TReadFloatFromScript(&tagCfg.fWidth, pSubVar->Element("width"));
			_TReadFloatFromScript(&tagCfg.fHeight, pSubVar->Element("height"));
			_TReadStringFromScript(tagCfg.strTexPath, pSubVar->Element("texpath"));
			m_listTexCfg.push_back(tagCfg);
		}		
		return true;
	}
	return false;
}

CScreenTex* CTScreenTexMgr::CreateScreenTex(DWORD dwID)
{
	SCRTEXCFG_LIST_IT it = m_listTexCfg.begin();
	for (; it != m_listTexCfg.end(); ++it)
	{
		if(it->dwID == dwID)
		{
			NiSourceTexturePtr spTex = NiSourceTexture::Create(it->strTexPath.c_str());
			CScreenTex* pScrTex = new CScreenTex;
			pScrTex->SetTex(spTex);
			pScrTex->SetRectangle(it->fX, it->fY, it->fWidth, it->fHeight);
			return pScrTex;
		}
	}
	return NULL;
}
