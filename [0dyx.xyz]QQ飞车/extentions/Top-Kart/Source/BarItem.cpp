#include "Top-KartPCH.h"
#include "BarItem.h"

CBarItem::CBarItem():
m_fElementX(0),
m_fElementY(0),
m_fElementWidth(0),
m_fElementHeight(0),
m_fBarLenPerUnit(0),
m_fBarStartLen(0),
m_fBarCurWidth(0),
m_unCurUnits(0),
m_unMaxUnits(0),
m_dwAnimTime(0),
m_dwAnimLastTime(0)
{
	for(int i = 0; i < enBarNum; ++i)
	{
		m_aspBarElet[i] = NiNew NiScreenElements(NiNew NiScreenElementsData(false, false, 1));
		m_aspBarElet[i]->Insert(4);
		m_aspBarElet[i]->UpdateBound();
		m_aspBarElet[i]->Update(0);
	}
}

CBarItem::~CBarItem()
{
	for(int i = 0; i < enBarNum; ++i)
	{
		m_aspBarElet[i] = 0;
	}
}


void CBarItem::SetBarTex(unsigned int unType, NiSourceTexturePtr spTex)
{
	NiTexturingPropertyPtr spTexPro = NiNew NiTexturingProperty();
	NiAlphaPropertyPtr spAlphaPro = NiNew NiAlphaProperty();

	spTexPro->SetBaseTexture(spTex);
	spTexPro->SetApplyMode(NiTexturingProperty::APPLY_REPLACE);
	spTexPro->SetBaseClampMode(NiTexturingProperty::CLAMP_S_CLAMP_T);

	spAlphaPro->SetAlphaBlending(true);
	spAlphaPro->SetSrcBlendMode(NiAlphaProperty::ALPHA_SRCALPHA);
	spAlphaPro->SetDestBlendMode(NiAlphaProperty::ALPHA_INVSRCALPHA);

	m_aspBarElet[unType]->AttachProperty(spTexPro);
	m_aspBarElet[unType]->AttachProperty(spAlphaPro);

	m_aspBarElet[unType]->UpdateProperties();
	m_aspBarElet[unType]->UpdateEffects();
	m_aspBarElet[unType]->Update(0);
}

void CBarItem::ReSetBar()
{
	SetCurUnits(0);
}

void CBarItem::SetWidth(float fWidth)
{
	m_fElementWidth = fWidth;

}

void CBarItem::SetBarLenPerUnit(float fLen)
{
	m_fBarLenPerUnit = fLen;
}

void CBarItem::SetHeight(float fHeight)
{
	m_fElementHeight = fHeight;
}

void CBarItem::SetBarStartLen(float fLen)
{
	m_fBarStartLen = fLen;
}

void CBarItem::MoveTo(float fLeft, float fTop)
{
	m_fElementX = fLeft;
	m_fElementY = fTop;
	for(int i = 0; i < enBarNum; i++)
	{
		if(i == enPowerBar)
		{
			m_aspBarElet[enPowerBar]->SetRectangle(0, m_fElementX, m_fElementY, m_fBarStartLen , m_fElementHeight);
			m_aspBarElet[enPowerBar]->UpdateBound();
			m_aspBarElet[enPowerBar]->SetTextures(0, 0, 0, 0, m_fBarStartLen/m_fElementWidth, 1);
		}
		else if ( i== enAnimBar1 || i == enAnimBar2 )
		{
			const float fDelta = 0.0025;
			m_aspBarElet[enAnimBar1]->SetRectangle(0, m_fElementX-fDelta, m_fElementY, fDelta, m_fElementHeight);
			m_aspBarElet[enAnimBar1]->UpdateBound();
			m_aspBarElet[enAnimBar1]->SetTextures(0, 0, 0, 0, 0, 1);
		}
		else
		{
			m_aspBarElet[i]->SetRectangle(0, m_fElementX, m_fElementY, m_fElementWidth, m_fElementHeight);
			m_aspBarElet[i]->UpdateBound();
			m_aspBarElet[i]->SetTextures(0, 0, 0, 0, 1, 1);
		}

	}
}

void CBarItem::SetFullUnits(unsigned int unUnits)
{
	m_unMaxUnits = unUnits;
	
}



void CBarItem::SetCurUnits(unsigned int unUnits)
{
	m_unCurUnits = unUnits > m_unMaxUnits ? m_unMaxUnits : unUnits;

	m_fBarCurWidth = m_fBarStartLen + m_unCurUnits * m_fBarLenPerUnit;
	m_aspBarElet[enPowerBar]->SetRectangle(0, m_fElementX, m_fElementY, m_fBarCurWidth , m_fElementHeight);
	m_aspBarElet[enPowerBar]->SetTextures(0, 0, 0, 0, m_fBarCurWidth/m_fElementWidth, 1);

	const float fDelta = 0.0025;
	m_aspBarElet[enAnimBar1]->SetRectangle(0, m_fElementX+m_fBarCurWidth-fDelta, m_fElementY, fDelta, m_fElementHeight);
	m_aspBarElet[enAnimBar1]->SetTextures(0, 0, (m_fBarCurWidth-fDelta)/m_fElementWidth, 0, (m_fBarCurWidth+fDelta)/m_fElementWidth, 1);

	m_aspBarElet[enAnimBar2]->SetRectangle(0, m_fElementX+m_fBarCurWidth-fDelta, m_fElementY, fDelta, m_fElementHeight);
	m_aspBarElet[enAnimBar2]->SetTextures(0, 0, (m_fBarCurWidth-fDelta)/m_fElementWidth, 0, (m_fBarCurWidth+fDelta)/m_fElementWidth, 1);
}

void CBarItem::Draw(NiRendererPtr spRenderer)
{
	m_aspBarElet[enBaseBar]->Draw(spRenderer);
	m_aspBarElet[enPowerBar]->Draw(spRenderer);

	/*
	DWORD dwTime = GetTickCount();
	if ( m_dwAnimLastTime == 0 )
	{
		m_dwAnimLastTime = dwTime;
	}

	if ( m_dwAnimTime < 50 )
	{
		m_dwAnimTime += (dwTime - m_dwAnimLastTime);	
		m_aspBarElet[enAnimBar1]->Draw(spRenderer);
	}
	else if ( m_dwAnimTime < 100 )
	{
		m_dwAnimTime += (dwTime - m_dwAnimLastTime);	
		m_aspBarElet[enAnimBar2]->Draw(spRenderer);
	}
	else
	{
		m_dwAnimTime = 0;
	}
	m_dwAnimLastTime = dwTime;
	*/
}