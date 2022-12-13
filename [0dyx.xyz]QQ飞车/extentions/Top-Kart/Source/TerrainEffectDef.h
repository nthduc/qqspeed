/********************************************************************
	����:	��wincoqin
	ʱ��:	  2007/04/13
    �ļ���:   TerrainEffectDef.h
	˵��:     []
*********************************************************************/
#pragma once
#include "NiMain.h"
#include "variant.h"
struct TerrainPropEffect;
struct TerrainEffect
{
	int			m_nTerrainEffectType;
	NiColorA	m_kColor;
	NiString	m_kDescript;		
	//CVarTable * m_pTerrainEffectVar;	
	std::vector<TerrainPropEffect*> m_vecTerrainPropEffect;
};

struct TerrainPropEffect
{
	int				m_nTerrainPropEffectType;
	int				m_nCondition;
#ifdef _DEBUG
	NiFixedString	m_kPropDiscript;
#endif	
	CVarTable *		m_pkPropVarTable;
};