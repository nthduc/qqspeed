#include "Top-KartPCH.h"

#include ".\statistics.h"

Statistics* Statistics::m_pStatistics = NULL;
unsigned int Statistics::m_anTriangles[GT_COUNT] = {0};
NiVisualTracker* Statistics::m_pTracker = NULL;
int Statistics::m_apGraphID[GT_COUNT] = {0};
char* Statistics::m_aszGraphName[GT_COUNT] = {"scene:", "car:", "minimap:","total:"};
std::vector<NiAVObject*> Statistics::m_vEdgeObjects;


void Statistics::Create()
{
	if (!m_pStatistics)
	{
		m_pStatistics = new Statistics();
	}
}

void Statistics::Release()
{
	if (m_pStatistics)
	{
		delete m_pStatistics;
		m_pStatistics = NULL;
	}
}

//Statistics* Statistics::Get()
//{
//	return m_pStatistics;
//}
void Statistics::AddVisibleArray(const NiVisibleArray* pVArray, eGROUPTYPE group)
{
	unsigned int nTris = GetTriangleNum(pVArray);
	m_anTriangles[group] += nTris;
}

void Statistics::AddNode(const NiNode* pkNode, eGROUPTYPE group)
{
	unsigned int nTris = GetTriangleNum(pkNode);
	m_anTriangles[group] += nTris;
}

Statistics::Statistics()
{

}

Statistics::~Statistics()
{

}


unsigned int Statistics::GetTriangleNum(const NiVisibleArray* pVArray)
{
	unsigned int nTris = 0;
	for(int i = 0; i < pVArray->GetCount(); i++)
	{
		const NiGeometry& geo = pVArray->GetAt(i);
		if (NiIsKindOf(NiTriBasedGeom, &geo))
		{
			NiTriBasedGeom* pTriGeo = NiDynamicCast(NiTriBasedGeom, &geo);
			if (pTriGeo)
			{
				nTris += pTriGeo->GetTriangleCount();
			}
		}
		else
		{
			//LogText("居然还有不是NiTriBasedGeom类型的东西！\n");
		}
	}

	return nTris;
}

unsigned int Statistics::GetTriangleNum(const NiAVObject* pkObject)
{
	int nRet = 0;
	if (NiIsKindOf(NiTriBasedGeom, pkObject))
	{
		NiTriBasedGeom* pGeo = NiDynamicCast(NiTriBasedGeom, pkObject);
		if (pGeo)
		{
			nRet = pGeo->GetTriangleCount();
		}
	}
	else
	{
		if (NiIsKindOf(NiNode, pkObject))
		{
			NiNode* pNode = NiDynamicCast(NiNode, pkObject);
			if (pNode)
			{
				for (int i = 0; i < pNode->GetChildCount(); i++)
				{
					nRet += GetTriangleNum(pNode->GetAt(i));
				}
			}
		}
		else
		{
			//LogText("采集数据发现不明类型：%s", pkObject->GetName());
		}
	}

	return nRet;
}

void Statistics::BeginCount()
{
	for (int i = 0; i < GT_COUNT; i++)
	{
		m_anTriangles[i] = 0;
	}
}
void Statistics::EndCount()
{
	for (int i = 0; i < GT_COUNT-1; i++)
	{
		m_anTriangles[GT_TOTAL]+= m_anTriangles[i];
	}

	static char buf[256];
	for (int i = 0; i < GT_COUNT; i++)
	{
		sprintf(buf, "%s %d\0", m_aszGraphName[i], m_anTriangles[i]);
		m_pTracker->SetName(buf, m_apGraphID[i]);
	}
}

unsigned int* Statistics::GetNum(eGROUPTYPE group)
{
	return &m_anTriangles[group];
}
void Statistics::GetAllEdgeObject()
{
	//m_vEdgeObjects.clear();

	//std::set< Play * > players& = NxPlayMgr::GetAllPlay();
	//static std::set< Play * >::iterator itr;

	//for (itr = players.begin(); itr != players.end(); ++itr)
	//{
	//	//Play* pPlay = (*itr);
	//	NiNode* pNode = (*itr)->GetCharacterNIFRoot();
	//	NiAVObject* pObj;
	//	pObj = FindObject(")

	//}
}

NiAVObject* Statistics::FindObject(const NiNode& node, const NiFixedString& szName)
{
//	return node.GetObjectByName(szName);
	return NULL;
}

void Statistics::ReplaceTexture(NiNode* pNode)
{

	NiTexturePtr spTextur = NiSourceTexture::Create("./data/win32/Textures/44.dds");
	ReplaceTextureRecursive(pNode, spTextur);

}
void Statistics::ReplaceTextureRecursive(NiAVObject* pObject, NiTexture* pText)
{
	if (NiIsKindOf(NiNode, pObject))
	{
		NiNode* pNode = NiDynamicCast(NiNode, pObject);
		for (int i = 0; i < pNode->GetChildCount(); i++)
			ReplaceTextureRecursive(pNode->GetAt(i), pText);
	}
	else
	{
		if (pObject->GetProperty(NiTexturingProperty::GetType()))
		{
			NiTexturingProperty* pTexProp = NiDynamicCast(NiTexturingProperty, pObject->GetProperty(NiTexturingProperty::GetType()));
			pTexProp->SetBaseTexture(pText);
			pTexProp->SetApplyMode(NiTexturingProperty::APPLY_REPLACE);
		}
	}
	
}

void Statistics::RemoveAlphaPropRecursive(NiAVObject* pObject)
{
	if (NiIsKindOf(NiNode, pObject))
	{
		NiNode* pNode = NiDynamicCast(NiNode, pObject);
		for (int i = 0; i < pNode->GetChildCount(); i++)
			RemoveAlphaPropRecursive(pNode->GetAt(i));
	}
	else
	{
		if (pObject->GetProperty(NiAlphaProperty::GetType()))
		{
			pObject->RemoveProperty(NiAlphaProperty::GetType());
			//NiAlphaProperty* pTexProp = NiDynamicCast(NiTexturingProperty, pObject->GetProperty(NiTexturingProperty::GetType()));
			//pTexProp->SetBaseTexture(pText);
			//pTexProp->SetApplyMode(NiTexturingProperty::APPLY_REPLACE);
		}
	}
}


void Statistics::CullSamllObjectRecursive(NiAVObject* pObject)
{
	if (NiIsKindOf(NiNode, pObject))
	{
		NiNode* pNode = NiDynamicCast(NiNode, pObject);
		for (int i = 0; i < pNode->GetChildCount(); i++)
			CullSamllObjectRecursive(pNode->GetAt(i));
	}
	else
	{
		if (NiIsKindOf(NiTriBasedGeom, pObject))
		{
			NiTriBasedGeom* pGeo = NiDynamicCast(NiTriBasedGeom, pObject);
			if (pGeo)
			{
				if (pGeo->GetTriangleCount() < 20)
					pGeo->SetAppCulled(true);
			}
		}
	}

}