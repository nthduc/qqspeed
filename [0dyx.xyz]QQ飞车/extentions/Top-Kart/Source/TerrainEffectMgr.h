/********************************************************************
	作者:	　wincoqin
	时间:	  2007/04/13
    文件名:   TerrainEffectMgr.h
	说明:     [管理跟地表相关的特效]
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
	// 更新
	void UpdateTerrainProp(int nPropType);


	// get certain triangle material by vertex color	
	static int GetTriangleMaterial(int nTriangleIndex, NiTriShape * pTriShape);
	static int AnalysisMaterialByColor(NiColorA * pColors);
	bool LoadTerrainPropTab();

protected:
	// 更新地表特效 
	void UpdateTerrainEffect();

	// 进入/离开地表
	void OnEnterNewProp(int nNewPropType);
	void OnLeaveOldProp();

	// 停止/开始播放地表特效
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
