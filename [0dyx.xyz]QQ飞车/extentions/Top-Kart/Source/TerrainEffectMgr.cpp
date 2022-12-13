/********************************************************************
	作者:	　wincoqin
	时间:	  2007/04/13
    文件名:   TerrainEffectMgr.cpp
	说明：     []
*********************************************************************/
// precompile header
#include "Top-KartPCH.h"
// libfile


// userfile
#include "TerrainEffectMgr.h"
#include "BaseTerrainEffect.h"

bool TerrainEffectMgr::ms_bInit = false;
CVarArray * TerrainEffectMgr::ms_pkTerrainEffectVar = NULL;
TerrainEffectMgr::TerrainEffectPool TerrainEffectMgr::ms_kTerrainEffectPool;

TerrainEffectMgr::TerrainEffectMgr(void)
: m_nCurProp(-1)
{
	if (!LoadTerrainPropTab()) 
	{
		NiMessageBox("加载地表属性失败！", "ERROR", NULL);
	}
}

TerrainEffectMgr::~TerrainEffectMgr(void)
{
	ClearTerrainPropTab();
}
//---------------------------------------------------------------------------
bool TerrainEffectMgr::LoadTerrainPropTab()
{
	if (ms_bInit)
	{
		ms_bInit = false;
		ClearTerrainPropTab();
	}
	// Load Terrain Property table
	CNotifyGameBeginPara * pkGameBeginPara = CLevelManager::Get()->GetGameBeginPara();
	if (pkGameBeginPara)
	{
		char szDir[MAX_PATH] = "";
		_snprintf(szDir, MAX_PATH, "Map/Common Map/Map%0.2d/TerrainPropTab.lua", pkGameBeginPara->m_nMapID);
		
		ms_pkTerrainEffectVar = new CVarArray("TerrainProp");
		CVarArray::LoadVarsFromFile(ms_pkTerrainEffectVar, NiApplication::ConvertMediaFilename(szDir));

		int nNum = ms_pkTerrainEffectVar->GetNum();
		for (int i = 0; i < nNum; ++i)
		{
			CVarTable * pkVar = (CVarTable*)(ms_pkTerrainEffectVar->Element(i));
			assert(pkVar->GetType() == VAR_TABLE);

			TerrainEffect * pkTerrainEffect = new TerrainEffect;
			CVar * pSubVar = NULL;
			pSubVar = pkVar->Element("proptype");
			assert(pSubVar);
			if (pSubVar)
				pkTerrainEffect->m_nTerrainEffectType = pSubVar->Integer();
			pSubVar = pkVar->Element("discript");
			assert(pSubVar);
			if (pSubVar)
				pkTerrainEffect->m_kDescript		  = pSubVar->String();
			pSubVar = pkVar->Element("color");
			assert(pSubVar);
			if (pSubVar)
				pSubVar->FloatArray((float*)&pkTerrainEffect->m_kColor, 4);
			CVarArray * pEffectVar  = (CVarArray*)(pkVar->Element("effect"));
			assert(pEffectVar);
			if (pEffectVar)
			{
				 CVarTable * pPropVar = NULL;
				 int nEffectNum = (pEffectVar)->GetNum();
				 for (int i = 0; i < nEffectNum; ++i)
				 {
					 pPropVar = (CVarTable*)pEffectVar->Element(i);
					 assert(pPropVar && pPropVar->GetType() == VAR_TABLE);
					 TerrainPropEffect * pkTerrainPropEffect = new TerrainPropEffect;
					 pSubVar = pPropVar->Element("effecttype");
					 assert(pSubVar);
					 if (pSubVar)
						 pkTerrainPropEffect->m_nTerrainPropEffectType = pSubVar->Integer();
					 pSubVar = pPropVar->Element("condition");
					 assert(pSubVar);
					 if (pSubVar) 
					 {
						 pkTerrainPropEffect->m_nCondition = pSubVar->Integer();
					 }

/*#ifdef _DEBUG
					 pSubVar = pPropVar->Element("discript");
					 assert(pSubVar);
					 if (pSubVar)
						 pkTerrainPropEffect->m_kPropDiscript = pSubVar->String();
#endif
					*/ 
					 pkTerrainPropEffect->m_pkPropVarTable = pPropVar;
					// assert(pSubVar && pSubVar->GetType() == VAR_TABLE);

					 pkTerrainEffect->m_vecTerrainPropEffect.push_back(pkTerrainPropEffect);
				 }

				
			}

			ms_kTerrainEffectPool[pkTerrainEffect->m_nTerrainEffectType] = pkTerrainEffect;
		}
	}


	ms_bInit = true;



	return ms_bInit;
}

//---------------------------------------------------------------------------
int TerrainEffectMgr::GetTriangleMaterial(int nTriangleIndex, NiTriShape * pTriShape)
{
	if (!ms_bInit) 
	{
		assert(ms_bInit && "TerrainEffectMgr Not Initialized!");
		return 0;
	}
	assert(pTriShape);

	unsigned short i0 = 0, i1 = 0, i2 = 0;
	pTriShape->GetTriangleIndices(nTriangleIndex, i0, i1, i2);

	NiColorA * pkColors = pTriShape->GetColors();
	//assert(pkColors);

	int nMaterial = 0;

	if (pkColors)
	{
		NiColorA kColors[3];
		kColors[0] = pkColors[i0];
		kColors[1] = pkColors[i1];
		kColors[2] = pkColors[i2];	

		nMaterial = AnalysisMaterialByColor(&kColors[0]);
	}

	return nMaterial;
}

int TerrainEffectMgr::AnalysisMaterialByColor(NiColorA * pColors)
{
	int nProperty = 0;
	if (pColors[0] == pColors[1] && pColors[0] == pColors[2])
	{
		TerrainEffectIter iter    = ms_kTerrainEffectPool.begin();
		TerrainEffectIter iterEnd = ms_kTerrainEffectPool.end();
		
		for (; iter != iterEnd; ++iter)
		{
			if (fabsf(iter->second->m_kColor.r - pColors[0].r) < 0.001f && 
				fabsf(iter->second->m_kColor.g - pColors[0].g) < 0.001f && 
				fabsf(iter->second->m_kColor.b - pColors[0].b) < 0.001f)
			{
				nProperty = iter->first;
				break;
			}
		}
	}	

	return nProperty;
}
void TerrainEffectMgr::OnLeaveOldProp()
{
	StopPropEffect(m_nCurProp);
}

void TerrainEffectMgr::OnEnterNewProp(int nNewProp)
{
	// Update New Prop
	StartPropEffect(nNewProp);
	m_nCurProp = nNewProp;
}

void TerrainEffectMgr::UpdateTerrainProp(int nPropType)
{
	if (m_nCurProp != nPropType) 
	{
		OnLeaveOldProp();
		OnEnterNewProp(nPropType);
	}
	else
	{
		UpdateTerrainEffect();
	}
}

void TerrainEffectMgr::ClearTerrainPropTab()
{
	TerrainEffectIter iter		= ms_kTerrainEffectPool.begin();
	TerrainEffectIter iterEnd	= ms_kTerrainEffectPool.end();

	for (; iter != iterEnd; ++iter)
	{
		if (iter->second != NULL) 
		{
			SAFE_DELETE(iter->second);
		}
	}

	ms_kTerrainEffectPool.clear();
	SAFE_DELETE(ms_pkTerrainEffectVar);
}

void TerrainEffectMgr::StartPropEffect(int nPropType)
{
	//assert(m_kCurTerrainPropList.)

	TerrainEffectIter iter = ms_kTerrainEffectPool.find(nPropType);
	// Found!
	if (iter != ms_kTerrainEffectPool.end()) 
	{
		TerrainEffect * pkTerrainEffect = iter->second;
		assert(pkTerrainEffect);

		size_t size = pkTerrainEffect->m_vecTerrainPropEffect.size();

		for (size_t i = 0; i < size; ++i)
		{
			TerrainPropEffect * pkTerrainPropEffect = pkTerrainEffect->m_vecTerrainPropEffect[i];

			if (pkTerrainPropEffect) 
			{
				ITerrainPropEffect * pTerrainPropEffect = CBaseTerrainPropEffect::
					CreateTerrainPropEffect(pkTerrainPropEffect->m_nTerrainPropEffectType, pkTerrainPropEffect);

				if (pTerrainPropEffect)
				{
					pTerrainPropEffect->StartTerrainEffect();
					m_kCurTerrainPropList.push_back(pTerrainPropEffect);
				}
			}
		}
	}
}

void TerrainEffectMgr::StopPropEffect(int nPropType)
{
	TerrainPropIter iter = m_kCurTerrainPropList.begin();
	TerrainPropIter end  = m_kCurTerrainPropList.end();
	float fDeltaTime = GetMainApp()->GetFrameTime();
	for (; iter != end; ++iter)
	{
		(*iter)->StopTerrainEffect();
		SAFE_DELETE(*iter);
	}
	m_kCurTerrainPropList.clear();
}

void TerrainEffectMgr::UpdateTerrainEffect()
{
	TerrainPropIter iter = m_kCurTerrainPropList.begin();
	TerrainPropIter end  = m_kCurTerrainPropList.end();
	float fDeltaTime = GetMainApp()->GetFrameTime();
	for (; iter != end; ++iter)
	{
		(*iter)->Update(fDeltaTime);
	}
}

