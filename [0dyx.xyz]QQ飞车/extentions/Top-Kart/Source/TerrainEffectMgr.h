/********************************************************************
	����:	��wincoqin
	ʱ��:	  2007/04/13
    �ļ���:   TerrainEffectMgr.h
	˵��:     [������ر���ص���Ч]
*********************************************************************/
#pragma once

// libfile
#include <vector>

// userfile
#include "singlet.h"
#include "TerrainEffectDef.h"
// predefine

class ITerrainPropEffect;
class TerrainEffectMgr
	: public CSingleT<TerrainEffectMgr>
{
public:
	TerrainEffectMgr(void);
	~TerrainEffectMgr(void);

	typedef std::vector<ITerrainPropEffect*>  TerrainPropList;
	typedef TerrainPropList::iterator		  TerrainPropIter;

	typedef std::map<int, TerrainEffect*>	  TerrainEffectPool;
	typedef TerrainEffectPool::iterator		  TerrainEffectIter;
public:
	// ����
	void UpdateTerrainProp(int nPropType);


	// get certain triangle material by vertex color	
	static int GetTriangleMaterial(int nTriangleIndex, NiTriShape * pTriShape);
	static int AnalysisMaterialByColor(NiColorA * pColors);
	bool LoadTerrainPropTab();

protected:
	// ���µر���Ч 
	void UpdateTerrainEffect();

	// ����/�뿪�ر�
	void OnEnterNewProp(int nNewPropType);
	void OnLeaveOldProp();

	// ֹͣ/��ʼ���ŵر���Ч
	void StopPropEffect(int nPropType);
	void StartPropEffect(int nPropType);

	void ClearTerrainPropTab();

protected:
	int m_nCurProp;
	TerrainPropList m_kCurTerrainPropList;
    

	static bool ms_bInit;
	static CVarArray * ms_pkTerrainEffectVar;
	static TerrainEffectPool ms_kTerrainEffectPool;	
};
