////////////////////////////////////////////////
//     FileName:    AvatarScript.h
//     Author:      rayhuang
//     Date:        2007-4-24
//     Description: 读取道具脚本的函数
////////////////////////////////////////////////

#pragma once 

#include "variant.h"

//读入数字
template<class T>
inline BOOL _ATReadNumFromVar(T* pValue, CVar* pVar)
{
	if ( pVar && pVar->GetType() == VAR_FLOAT )
	{
		*pValue = pVar->Float();
		return TRUE;
	}
	else
	{
		*pValue = 0;
	}
	return FALSE;
}

//读入字符串
inline BOOL _ATReadStringFromVar(string& strValue, CVar* pVar)
{
	if ( pVar && pVar->GetType() == VAR_STRING )
	{
		strValue = pVar->String();
		return TRUE;
	}
	return FALSE;
}

//读入装备物理属性
inline void _ATReadPhysicProptFromVar(CVar* pVar, const char* key, PHYSICPROPERTY& tagPhyPropt)
{
	CVarTable* pSubVar = (CVarTable*) pVar->Element(key);
	if(pVar)
	{
		_ATReadNumFromVar(&tagPhyPropt.shOpertor, pSubVar->Element("operator"));
		_ATReadNumFromVar(&tagPhyPropt.dwSpeed, pSubVar->Element("speed"));
		_ATReadNumFromVar(&tagPhyPropt.dwErupt, pSubVar->Element("erupt"));
		_ATReadNumFromVar(&tagPhyPropt.dwAccumNo2, pSubVar->Element("accumno2"));
		_ATReadNumFromVar(&tagPhyPropt.dwBigNo2, pSubVar->Element("bigno2"));
		_ATReadNumFromVar(&tagPhyPropt.dwMiniNo2, pSubVar->Element("minino2"));
	}
	else
	{
		tagPhyPropt.shOpertor = 0; 
		tagPhyPropt.dwSpeed = 0;
		tagPhyPropt.dwErupt = 0;
		tagPhyPropt.dwAccumNo2 = 0;
		tagPhyPropt.dwBigNo2 = 0;
		tagPhyPropt.dwMiniNo2 = 0;		
	}
}

//读入装备算法
inline void _ATReadAlgorithmFromVar(CVar* pVar, const char* key, AlgDataList& listAlg)
{
	CVarArray* pSubVar = (CVarArray*)pVar->Element(key);
	if(pSubVar)
	{
		for( int i = 0; i < pSubVar->GetNum(); ++i)
		{
			ALGDATA tagAlgData;
			CVarTable* pSubVar0 = (CVarTable*)pSubVar->Element(i);
			_ATReadNumFromVar(&tagAlgData.dwType, pSubVar0->Element("type"));
			_ATReadNumFromVar(&tagAlgData.dwOccasion, pSubVar0->Element("occasion"));
			_ATReadNumFromVar(&tagAlgData.shOpertor, pSubVar0->Element("operator"));
			_ATReadNumFromVar(&tagAlgData.dwCondition, pSubVar0->Element("condition"));
			_ATReadNumFromVar(&tagAlgData.dwExpectation, pSubVar0->Element("expectation"));
			_ATReadNumFromVar(&tagAlgData.dwRate, pSubVar0->Element("rate"));
			listAlg.push_back(tagAlgData);
		}
	}
}


//读入装备行为
inline void _ATReadBehaveFromVar(CVar* pVar, const char* key, BehaveDataList& listBehave)
{
	CVarArray* pSubVar = (CVarArray*)pVar->Element(key);
	if(pSubVar)
	{
		for (int i = 0; i < pSubVar->GetNum(); ++i)
		{
			BEHAVEDATA tagBehaveData;
			CVarTable* pSubVar0 = (CVarTable*)pSubVar->Element(i);
			_ATReadNumFromVar(&tagBehaveData.dwType, pSubVar0->Element("type"));
			_ATReadNumFromVar(&tagBehaveData.dwOccasion, pSubVar0->Element("occasion"));
			_ATReadNumFromVar(&tagBehaveData.dwCondition, pSubVar0->Element("condition"));
			_ATReadNumFromVar(&tagBehaveData.dwLife, pSubVar0->Element("life"));
			_ATReadNumFromVar(&tagBehaveData.dwParam, pSubVar0->Element("param"));
			listBehave.push_back(tagBehaveData);
		}
	}
}

//读入装备特有动作
inline void _ATReadAnimFromVar(CVar* pVar, const char* key, AnimDataList& listAnim)
{
	CVarArray* pSubVar = (CVarArray*)pVar->Element(key);
	if(pSubVar)
	{
		for(int i = 0; i < pSubVar->GetNum(); ++i)
		{
			ANIMDATA tagAnimData;
			CVarTable* pSubVar0 = (CVarTable*) pSubVar->Element(i);
			_ATReadNumFromVar(&tagAnimData.dwCondition, pSubVar0->Element("condition"));
			_ATReadNumFromVar(&tagAnimData.dwExpectation, pSubVar0->Element("expectation"));
			listAnim.push_back(tagAnimData);
		}
	}
}



//读入装备属性
inline void _ATReadPropertyFromVar(CVar* pVar, const char* key, AVATARPROPERTY& tagProperty)
{
	CVarTable* pSubVar = (CVarTable*)pVar->Element(key);
	if(pSubVar)
	{
		_ATReadNumFromVar(&tagProperty.shSwitch, pSubVar);
		_ATReadNumFromVar(&tagProperty.shNoFog, pSubVar);
		_ATReadPhysicProptFromVar(pSubVar, "physicPropt", tagProperty.tagPhyPrp);
		_ATReadAnimFromVar(pSubVar, "specialAnim", tagProperty.listAnimData);
		_ATReadAlgorithmFromVar(pSubVar, "algorithm", tagProperty.listAlgData);
		_ATReadBehaveFromVar(pSubVar, "behave", tagProperty.listBehaveData);		
	}
	else
	{
		tagProperty.shSwitch = 0;
		tagProperty.shNoFog = 0;
		tagProperty.tagPhyPrp.shOpertor = 0; 
		tagProperty.tagPhyPrp.dwSpeed = 0;
		tagProperty.tagPhyPrp.dwErupt = 0;
		tagProperty.tagPhyPrp.dwAccumNo2 = 0;
		tagProperty.tagPhyPrp.dwBigNo2 = 0;
		tagProperty.tagPhyPrp.dwMiniNo2 = 0;	
	}
}

inline void _ATReadHitchFromVar(CVar* pVar, const char* key, ITEMHITCH_LIST& listHitch)
{
	CVarArray* pSubVar = (CVarArray*) pVar->Element(key);
	if(pSubVar)
	{
		UINT id;
		for(int i = 0; i < pSubVar->GetNum(); ++i)
		{
			/*
			UINT id;
			_ATReadNumFromVar(&id, pSubVar->Element(i)->Element("typeid"));
			*/
			id=pSubVar->Element(i)->Integer();
			listHitch.push_back(id);
		}
	}
}

//读入换色用色彩
inline void _ATReadColorFromVar(CVar* pVar, const char* key, SColorAttr& insColor)
{
	CVarArray* pSubVar = (CVarArray*) pVar->Element(key);
	if(pSubVar)
	{
		insColor.hue=pSubVar->Element(0)->Integer();
		insColor.sat=pSubVar->Element(1)->Float();
		insColor.val=pSubVar->Element(2)->Float();
	}
}

inline void _ATReadChgColorNameFromVar(CVar* pVar, const char* key, ItemUnit* pItem)
{
	CVarArray* pSubVar = (CVarArray*) pVar->Element(key);
	if(pSubVar)
	{
		std::string strTmp;
		for(int i = 0; i < pSubVar->GetNum(); ++i)
		{
			strTmp=pSubVar->Element(i)->String();
			pItem->deChgColorName.push_back(strTmp);
		}
	}
}

inline void _ATReadHideObjNameFromVar(CVar* pVar, const char* key, ItemUnit* pItem)
{
	CVarArray* pSubVar = (CVarArray*) pVar->Element(key);
	if(pSubVar)
	{
		std::string strTmp;
		for(int i = 0; i < pSubVar->GetNum(); ++i)
		{
			strTmp=pSubVar->Element(i)->String();
			pItem->deHideObjName.push_back(strTmp);
		}
	}
}