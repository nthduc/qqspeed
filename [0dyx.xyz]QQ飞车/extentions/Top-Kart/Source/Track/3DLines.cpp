#include "Top-KartPCH.h"
#include "3dlines.h"
#include "assert.h"
#include "NiVertexColorProperty.h"
#include "NiAlphaProperty.h"

LineManager * LineManager::ms_pLineMgr = NULL;
unsigned int  LineManager::ms_uiLineID = 0;

LineManager::LineManager(NiNodePtr spRoot)
: m_spLineRoot(NULL)
{
	m_spLineRoot = NiNew NiNode;
	m_spLineRoot->SetName("Line Root");
	spRoot->AttachChild(m_spLineRoot);
}

LineManager::~LineManager(void)
{
	m_spLineRoot = NULL;
}


LineManager * LineManager::CreateLineManager(NiNodePtr spRoot)
{
	if (!ms_pLineMgr) 
	{
		ms_pLineMgr = NiNew LineManager(spRoot);
	}

	return ms_pLineMgr;
}

void LineManager::DestroyLineManager()
{
	if (ms_pLineMgr) 
	{
		NiDelete ms_pLineMgr;
		ms_pLineMgr = NULL;
	}
}

LineManager * LineManager::GetLineManager()
{
	return ms_pLineMgr;
}

unsigned int LineManager::AddLine(const NiPoint3& kStart, const NiPoint3& kEnd, const NiColorA & kColor)
{
	NiPoint3 * pVerts = NiNew NiPoint3[2];
	pVerts[0] = kStart; pVerts[1] = kEnd;
	
	NiColorA * pColor = NiNew NiColorA[2];
	pColor[0] = pColor[1] = kColor;

	NiPoint2 * pTexture = NiNew NiPoint2[2];
	pTexture[0] = pTexture[1] = NiPoint2(0.f, 0.f);

	NiBool * pConn = NiAlloc(NiBool, 2);
	pConn[0] = true; pConn[1] = false;
	
	NiLinesPtr spLine = NiNew NiLines(2, pVerts,pColor,pTexture,0,NiGeometryData::NBT_METHOD_NONE,pConn);

	NiVertexColorProperty* pkColorProp = NiNew NiVertexColorProperty;
	pkColorProp->SetLightingMode(NiVertexColorProperty::LIGHTING_E);
	pkColorProp->SetSourceMode(NiVertexColorProperty::SOURCE_EMISSIVE);
	spLine->AttachProperty(pkColorProp);
	NiAlphaProperty* pkAlphaProp = NiNew NiAlphaProperty;
	spLine->AttachProperty(pkAlphaProp);
	pkAlphaProp->SetAlphaBlending(true);

	spLine->UpdateProperties();
	
	++ms_uiLineID;
    m_LinesMap.SetAt(ms_uiLineID, spLine);

	m_spLineRoot->AttachChild(spLine);

	m_spLineRoot->UpdateProperties();
	m_spLineRoot->UpdateNodeBound();
	m_spLineRoot->Update(0.0f);

	return ms_uiLineID;
}

void LineManager::RemoveLine(unsigned int ID)
{
	assert(ID);

	NiLinesPtr spLine = NULL;
	if (m_LinesMap.GetAt(ID, spLine))
	{
		m_spLineRoot->DetachChild(spLine);
		m_LinesMap.RemoveAt(ID);
	}
}

void LineManager::ResetLine(unsigned int ID, const NiPoint3& kStart, const NiPoint3& kEnd)
{
	assert(ID);

	NiLinesPtr spLine = NULL;
	if (m_LinesMap.GetAt(ID, spLine))
	{
		assert(2 == spLine->GetVertexCount());
		NiPoint3 * pVerts = spLine->GetVertices();
		pVerts[0] = kStart;
		pVerts[1] = kStart;

		spLine->UpdateNodeBound();
		spLine->Update(0.0f);		
	}
}
