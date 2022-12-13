#include "ScreenElementsManager.h"
#include "NiRenderTargetGroup.h"
#include "NiApplication.h"

unsigned int CScreenElement::ms_unCreatedScreenElemCounts = 0;

CScreenElement * CScreenElement::CreateScreenElement(const char * sResName)
{
	return NiNew CScreenElement(sResName);
}

CScreenElement::CScreenElement(const char * sResName)
: m_uiMapPointCount(0)
, m_pkTexCoords(NULL)
, m_skScreenElement(NULL)
, m_skTexturingProperty(NULL)
, m_fScale(1.0f)
, m_kPos(NiPoint2(0.f, 0.f))
, m_unWidth(10)
, m_unHeight(10)
{
	memset(m_bProp, 0, sizeof(bool)*Prop_Num);

	m_skScreenElement = NiNew NiScreenElements(NiNew NiScreenElementsData(false, false, 1));

	static char Name[64] = "";
	NiSprintf(Name, 64, "%d", ms_unCreatedScreenElemCounts++);

	SetName(Name);
	m_kResName = sResName;
	if (sResName != NULL && sResName[0] != 0)
	{
		m_skTexturingProperty = NiNew NiTexturingProperty(NiApplication::ConvertMediaFilename(sResName));
	}
}

CScreenElement::~CScreenElement()
{
	m_skScreenElement = NULL;
}

void CScreenElement::SetColor(const NiColorA& color)
{
	m_bProp[Prop_Color] = true;
	m_kColor = color;
	m_skScreenElement->SetColors(0, m_kColor);
}

void CScreenElement::SetScale(float fScale)
{
	m_bProp[Prop_Scale] = true;
	m_fScale = fScale;
	m_skScreenElement->SetScale(m_fScale);
}

void CScreenElement::SetPosAndSize(const NiPoint2& pos, unsigned int width, unsigned int height)
{
	m_kPos		= pos;
	m_unWidth	= width;
	m_unHeight	= height;
}

void CScreenElement::SetFigure(NiPoint2 * aPoints, unsigned int unPointsNum)
{
	m_bProp[Prop_Figure] = true;
	m_pkTexCoords = aPoints;
	m_uiMapPointCount = unPointsNum;	
}

bool CScreenElement::ApplySetting(bool bWithAlpha, NiAlphaProperty::AlphaFunction alphaSrc, 
								  NiAlphaProperty::AlphaFunction alphaDest)
{
	if (!(NiScreenElements *)m_skScreenElement)
	{
		return false;
	}

	
	float fRendererWidth = (float)NiApplication::ms_pkApplication->GetAppWindow()->GetWidth();
	float fRendererHeight = (float)NiApplication::ms_pkApplication->GetAppWindow()->GetHeight();
	float fAspect = fRendererWidth / fRendererHeight;
	float fMapWidth = (float)m_unWidth / fRendererWidth;
	float fMapHeight = (float)m_unHeight / fRendererHeight;

	NiPoint3 kTranslate(m_kPos.x / fRendererWidth, m_kPos.y / fRendererHeight, 0.3f);

    if ((NiTexturingProperty*)m_skTexturingProperty)
	{
		if (m_pkTexCoords)
		{
			m_skTexturingProperty->SetApplyMode(NiTexturingProperty::APPLY_REPLACE);
			m_skTexturingProperty->SetBaseClampMode(NiTexturingProperty::CLAMP_S_CLAMP_T);			

			m_skScreenElement->Insert(m_uiMapPointCount);
			for (unsigned int i = 0; i < m_uiMapPointCount; ++i)
			{
				m_pkTexCoords[i].y *= fAspect;			
				m_skScreenElement->SetVertex(0, i, m_pkTexCoords[i]);
				m_skScreenElement->SetTexture(0, i, 0, m_pkTexCoords[i]);
			}
			
			m_skScreenElement->UpdateBound();
			m_skScreenElement->AttachProperty(m_skTexturingProperty);
		}
		else
		{
			m_skTexturingProperty->SetApplyMode(NiTexturingProperty::APPLY_REPLACE);

			m_skScreenElement->Insert(4);
			m_skScreenElement->SetRectangle(0, 0.f, 0.f, fMapWidth, fMapHeight);
			m_skScreenElement->UpdateBound();
			m_skScreenElement->SetTextures(0, 0, 0.0f, 0.0f, 1.0f, 1.0f);
			m_skScreenElement->AttachProperty(m_skTexturingProperty);				
		}
	}
	else
	{
		m_skScreenElement->Insert(4);
		m_skScreenElement->SetRectangle(0, 0.0f, 0.0f, fMapWidth, fMapHeight);
        m_skScreenElement->UpdateBound();
		m_skScreenElement->SetTextures(0, 0, 0.0f, 0.0f, 1.0f, 1.0f);				
	}	

	if (bWithAlpha)
	{
		NiAlphaPropertyPtr spAlpha = NiNew NiAlphaProperty;
		spAlpha->SetAlphaBlending(true);
		spAlpha->SetSrcBlendMode(alphaSrc);
		spAlpha->SetDestBlendMode(alphaDest);

		m_skScreenElement->AttachProperty(spAlpha);
	}
	
	m_skScreenElement->SetTranslate(kTranslate);

	m_skScreenElement->UpdateProperties();
	m_skScreenElement->Update(0.0f);
	
	NiNode::AttachChild(m_skScreenElement);

	return true;
}

void CScreenElement::Draw(NiRenderer * pkRender)
{
	m_skScreenElement->Draw(pkRender);
}

CScreenElementsManager::CScreenElementsManager(void)
: m_skWindowUIRoot(NULL)
{
	m_skWindowUIRoot = NiNew NiNode;
}

CScreenElementsManager::~CScreenElementsManager(void)
{
	m_skWindowUIRoot = NULL;
}



CScreenElement * CScreenElementsManager::GetScreenElement(const NiFixedString & kName)
{	
	return (CScreenElement*)m_skWindowUIRoot->GetObjectByName(kName);
}

void CScreenElementsManager::AddScreenElement(CScreenElement * pkNode)
{
	if (!pkNode) 
	{
		assert(pkNode);
		return;
	}

	m_skWindowUIRoot->AttachChild(pkNode);
}

void CScreenElementsManager::RemoveScreenElement(const NiFixedString & kName)
{
	NiAVObject * pkObject = m_skWindowUIRoot->GetObjectByName(kName);
	if (pkObject)
	{
		NiAVObjectPtr skObject = m_skWindowUIRoot->DetachChild(pkObject);
		skObject = NULL;
	}
}

void CScreenElementsManager::RemoveAll()
{
	for (unsigned int i = 0; i < m_skWindowUIRoot->GetChildCount(); i++)
	{
		m_skWindowUIRoot->DetachChildAt(i);
	}	
}

void CScreenElementsManager::Draw()
{
	NiRenderer* pkRenderer = NiRenderer::GetRenderer();

	for (unsigned int i = 0; i < m_skWindowUIRoot->GetChildCount(); i++)
	{
		((CScreenElement *)m_skWindowUIRoot->GetAt(i))->Draw(pkRenderer);
	}	
}
#ifdef _DEBUG
void CScreenElementsManager::DumpAllElements()
{

}
#endif
