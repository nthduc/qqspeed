////////////////////////////////////////////////
//     FileName:    gpscript.h
//     Author:      Fish
//     Date:        2007-1-25 16:23:47
//     Description: 封装一些读取脚本的函数
////////////////////////////////////////////////

#ifndef _GPSCRIPT_H_
#define _GPSCRIPT_H_

#include "variant.h"

/*
_ReadNumFromVar：
读入数字
*/
template<class T>
inline BOOL _ReadNumFromVar(T* pValue, CVar* pVar)
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

/*
_ReadNumFromVar：
读入数字
*/
/*
inline BOOL _ReadNumFromVar(DWORD* pValue, CVar* pVar)
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
*/

/*
_ReadStringFromVar：
读入字符串
*/
inline BOOL _ReadStringFromVar(string& strValue, CVar* pVar)
{
	if ( pVar && pVar->GetType() == VAR_STRING )
	{
		strValue = pVar->String();
		return TRUE;
	}
	return FALSE;
}

/*
_ReadEffectFromVar：
读入Effect
*/
inline void _ReadEffectFromVar(CVar* pVar, const char* rootkey, GP_SPECIAL_EFFECT_LIST& listEffect)
{
	CVarArray* pSubVar = (CVarArray*)pVar->Element(rootkey);
	if ( pSubVar )
	{
		for ( int j = 0; j < pSubVar->GetNum(); ++j )
		{
			GP_SPECIAL_EFFECT gpEffect;
			CVarTable* pSubVar0 = (CVarTable*)pSubVar->Element(j);
			_ReadNumFromVar(&gpEffect.dwType, pSubVar0->Element("type"));
			_ReadNumFromVar(&gpEffect.dwParam, pSubVar0->Element("param"));
			listEffect.push_back(gpEffect);
		}
	}
}

/*
_ReadAffectsFromVar：
读入Affects
*/
inline void _ReadAffectsFromVar(CVar* pVar, const char* rootkey, GP_AFFECT_LIST& listAffect)
{
	CVarArray* pSubVar = (CVarArray*)pVar->Element(rootkey);
	if ( pSubVar )
	{
		for ( int j = 0; j < pSubVar->GetNum(); ++j )
		{
			GP_AFFECT gpAffect;
			CVarTable* pSubVar0 = (CVarTable*)pSubVar->Element(j);
			_ReadNumFromVar(&gpAffect.dwID, pSubVar0->Element("id"));
			_ReadNumFromVar(&gpAffect.fParam, pSubVar0->Element("param"));
			_ReadNumFromVar(&gpAffect.dwOption, pSubVar0->Element("option"));
			_ReadNumFromVar(&gpAffect.dwStartTime, pSubVar0->Element("starttime"));
			_ReadNumFromVar(&gpAffect.dwLifeTime, pSubVar0->Element("lifetime"));
			listAffect.push_back(gpAffect);
		}
	}
}

/*
_ReadBoundFromVar：
读入Bound
*/
inline void _ReadBoundFromVar(CVar* pVar, const char* rootkey, GP_BOUND* pBound)
{
	float x = 0, y = 0, z = 0;
	if ( pVar )
	{
		CVarTable* pVarTable = (CVarTable*)pVar->Element(rootkey);
		if ( pVarTable )
		{
			_ReadNumFromVar(&pBound->dwType, pVarTable->Element("type"));
			_ReadNumFromVar(&x, pVarTable->Element("x"));
			_ReadNumFromVar(&y, pVarTable->Element("y"));
			_ReadNumFromVar(&z, pVarTable->Element("z"));
		}
	}
	pBound->x = x;
	pBound->y = y;
	pBound->z = z;
}

/*
_ReadPositionFromVar：
读入Position
*/
inline void _ReadPositionFromVar(CVar* pVar, const char* rootkey, GP_POSITION* pPosition)
{
	float x = 0, y = 0, z = 0;
	if ( pVar )
	{
		CVarTable* pVarTable = (CVarTable*)pVar->Element(rootkey);
		if ( pVarTable )
		{
			_ReadNumFromVar(&pPosition->dwType, pVarTable->Element("type"));
			_ReadNumFromVar(&x, pVarTable->Element("x"));
			_ReadNumFromVar(&y, pVarTable->Element("y"));
			_ReadNumFromVar(&z, pVarTable->Element("z"));
		}
	}
	pPosition->x = x;
	pPosition->y = y;
	pPosition->z = z;
}


/*
_ReadMoveCtrlFromVar：
读入Position
*/
inline void _ReadMoveCtrlFromVar(CVar* pVar, const char* rootkey, GP_MOVECTRL* pMoveCtrl)
{
	DWORD dwMoveType = 0, dwStartTime = 0, dwLifeTime = 0, dwMoveNode = 0;
	if ( pVar )
	{
		CVarTable* pVarTable = (CVarTable*)pVar->Element(rootkey);
		if ( pVarTable )
		{
			_ReadNumFromVar(&dwMoveType, pVarTable->Element("type"));
			_ReadNumFromVar(&dwStartTime, pVarTable->Element("starttime"));
			_ReadNumFromVar(&dwLifeTime, pVarTable->Element("lifetime"));
			_ReadNumFromVar(&dwMoveNode, pVarTable->Element("movenode"));
			_ReadPositionFromVar(pVarTable, "srcpos", &pMoveCtrl->stSrcPosition);
			_ReadPositionFromVar(pVarTable, "destpos", &pMoveCtrl->stDestPosition);
		}
	}
	pMoveCtrl->dwMoveType = dwMoveType;
	pMoveCtrl->dwStartTime = dwStartTime;
	pMoveCtrl->dwLifeTime = dwLifeTime;
	pMoveCtrl->dwMoveNode = dwMoveNode;
}

#endif