#include "Top-KartPCH.h"

#include "BlinkItem.h"

CBlinkItem::CBlinkItem():
m_unAnimStartTime(0),
m_unAnimTimeLen(800),
m_fElementX(0),
m_fElementY(0),
m_fElementWidth(0),
m_fElementHeight(0),
m_unShowTexId(0)
{
	for(int i = 0; i < enTexNum; ++i)
	{
		m_aspItemElet[i] = NiNew NiScreenElements(NiNew NiScreenElementsData(false, false, 1));
		m_aspItemElet[i]->Insert(4);
		m_aspItemElet[i]->UpdateBound();
		m_aspItemElet[i]->Update(0);
	}
	
}


CBlinkItem::~CBlinkItem()
{

}

void CBlinkItem::MoveTo(float fLeft , float fTop)
{
	m_fElementX = fLeft;
	m_fElementY = fTop;

	for(int i = 0; i < enTexNum; ++i)
	{
		m_aspItemElet[i]->SetRectangle(0, m_fElementX, m_fElementY, m_fElementWidth, m_fElementHeight);
		m_aspItemElet[i]->UpdateBound();
		m_aspItemElet[i]->SetTextures(0, 0, 0, 0, 1, 1);
	}

}

void CBlinkItem::SetTex(unsigned int unType, NiSourceTexturePtr spTex)
{
	NiTexturingPropertyPtr spTexPro = NiNew NiTexturingProperty();
	NiAlphaPropertyPtr spAlphaPro = NiNew NiAlphaProperty();

	spTexPro->SetBaseTexture(spTex);
	spTexPro->SetApplyMode(NiTexturingProperty::APPLY_REPLACE);
	spTexPro->SetBaseClampMode(NiTexturingProperty::CLAMP_S_CLAMP_T);

	spAlphaPro->SetAlphaBlending(true);
	spAlphaPro->SetSrcBlendMode(NiAlphaProperty::ALPHA_SRCALPHA);
	spAlphaPro->SetDestBlendMode(NiAlphaProperty::ALPHA_INVSRCALPHA);

	m_aspItemElet[unType]->AttachProperty(spTexPro);
	m_aspItemElet[unType]->AttachProperty(spAlphaPro);

	m_aspItemElet[unType]->UpdateProperties();
	m_aspItemElet[unType]->UpdateEffects();
	m_aspItemElet[unType]->Update(0);
}

void CBlinkItem::SetHeight(float fHeight)
{
	m_fElementHeight = fHeight;
}

void CBlinkItem::SetWidth(float fWidth)
{
	m_fElementWidth = fWidth;
}

void CBlinkItem::SetAnimTimeLen(unsigned int unTime)
{
	m_unAnimTimeLen = unTime;
}

unsigned int CBlinkItem::GetAnimTimeLen()
{
	return m_unAnimTimeLen;
}

void CBlinkItem::StartAnim()
{
	m_unAnimStartTime = GetTickCount();
	m_bIsAnim = true;
	m_unShowTexId = 0;
}

void CBlinkItem::StopAnim()
{
	m_bIsAnim = false;
	m_unShowTexId = 0;
}

void CBlinkItem::Draw(NiRendererPtr spRenderer)
{
	if(m_bIsAnim)
	{
		if(GetTickCount() - m_unAnimStartTime < m_unAnimTimeLen)
		{
			m_aspItemElet[m_unShowTexId]->Draw(spRenderer);
			m_unShowTexId = (m_unShowTexId + 1) % 2;
		}
		else
		{
			m_bIsAnim = false;
		}
	}
}