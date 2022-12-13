////////////////////////////////////////////////
//     FileName:    TaskScript.h
//     Author:      rayhuang
//     Date:        2007-5-17
//     Description: 任务配置脚本函数
////////////////////////////////////////////////

#pragma once 
#include "variant.h"
#include "TaskDefine.h"
#include <string>
using namespace std;

//从脚本中读入整数
inline void _TReadIntFromScript(DWORD* pValue, CVar* pVar)
{
	if(pVar && pVar->GetType() == VAR_FLOAT)
	{
		*pValue = pVar->Integer();
	}
	else
	{
		*pValue = 0;
	}
}
//从脚本中读浮点数
inline void _TReadFloatFromScript(float* pValue, CVar* pVar)
{
	if(pVar && pVar->GetType() == VAR_FLOAT)
	{
		*pValue = pVar->Float();
	}
	else
	{
		*pValue = 0;
	}
}
//从脚步中读bool值
inline void _TReadBoolFromScript(bool* pValue, CVar* pVar)
{
	if(pVar && pVar->GetType() == VAR_FLOAT)
	{
		if(pVar->Integer() != 0)
		{
			*pValue = true;
			return;
		}
	}
	*pValue = false;
}

//从脚本中读入字符串
inline void _TReadStringFromScript(string& strValue, CVar* pVar)
{
	if(pVar && pVar->GetType() == VAR_STRING)
	{
		strValue = pVar->String();
	}
	else
	{
		strValue = "";
	}
}

//从脚本中读入条件（钥匙）
inline void _TReadKeyFromScript(CVar* pVar, const char* key, PREKEY_LIST& listKey)
{
	CVarArray* pSubVar = (CVarArray*)pVar->Element(key);
	DWORD dwKey;
	for(int i = 0; i < pSubVar->GetNum(); ++i)
	{
		_TReadIntFromScript(&dwKey, pSubVar->Element(i));
		listKey.push_back(dwKey);
	}
	
}

//从脚本中读入原子配置
inline void _TReadAtomFromScript(CVar* pVar, ATOMCFG& tagAtom)
{
	if(pVar && pVar->GetType() == VAR_TABLE)
	{
		_TReadIntFromScript(&tagAtom.dwType, pVar->Element("Type"));
		_TReadIntFromScript(&tagAtom.dwParam, pVar->Element("Param"));
		_TReadIntFromScript(&tagAtom.dwOption, pVar->Element("Option"));
	}
	else
	{
		tagAtom.dwType = 0;
		tagAtom.dwParam = 0;
	}
}

//从脚本中读入原子配置链表
inline void _TReadAtomListFromScript(CVar* pVar, const char* key, ATOMCFG_LIST& listAtom)
{
	CVarArray* pSubVar = (CVarArray*)pVar->Element(key);
	if(pSubVar)
	{	
		ATOMCFG tagAtom;
		for(int i = 0; i < pSubVar->GetNum(); ++i)
		{			
			_TReadAtomFromScript(pSubVar->Element(i), tagAtom);
			listAtom.push_back(tagAtom);	
		}
	}

}

//从脚本中读入原子层
inline void _TReadAtomLevelFromScript(CVar* pVar, ATOMLEVELCFG& tagAtomLevel)
{
	if(pVar && pVar->GetType() == VAR_TABLE)
	{
		_TReadIntFromScript(&tagAtomLevel.dwDetectorID, pVar->Element("DetectorId"));
		_TReadAtomListFromScript(pVar, "AtomList", tagAtomLevel.listAtom);
	}
}

//从脚本中读入原子层链表
inline void _TReadAtomLevelListFromScript(CVar* pVar, const char* key, ATOMLEVELCFG_LIST& listAtomLevel)
{
	CVarArray* pSubVar = (CVarArray*) pVar->Element(key);
	if(pSubVar)
	{
		
		for(int i = 0; i < pSubVar->GetNum(); ++i)
		{
			ATOMLEVELCFG tagAtomLevel;
			_TReadAtomLevelFromScript(pSubVar->Element(i), tagAtomLevel);
			listAtomLevel.push_back(tagAtomLevel);
		}
	}
}

//从脚本中读入子任务配置
inline void _TReadSubTaskFromScript(CVar* pVar, SUBTASKCFG& tagSubTask)
{
	if(pVar && pVar->GetType() == VAR_TABLE)
	{
		_TReadIntFromScript(&tagSubTask.dwValidation, pVar->Element("Validation"));
		_TReadIntFromScript(&tagSubTask.dwProcessType, pVar->Element("ProcessType"));
		_TReadAtomListFromScript(pVar, "SubTaskPeriodAtom", tagSubTask.listSubTaskPeriodAtom);
		_TReadAtomLevelListFromScript(pVar, "AtomLevelList", tagSubTask.listAtomLevel);
	}
}

//从脚本中读入子任务配置链表
inline void _TReadSubTaskListFromScript(CVar* pVar, const char* key, SUBTASKCFG_LIST& listSubTask)
{
	CVarArray* pSubVar = (CVarArray*)pVar->Element(key);
	if(pSubVar)
	{
		for(int i = 0; i < pSubVar->GetNum(); ++i)
		{
			SUBTASKCFG tagSubTask;
			_TReadSubTaskFromScript(pSubVar->Element(i), tagSubTask);
			listSubTask.push_back(tagSubTask);
		}
	}
}

//从脚本中读开关表
inline void _TReadSwitchTableFromScript(CVar* pVar, const char* key, SWITCHTABLE& kSwitch)
{
	CVarTable* pSubVar = (CVarTable*)pVar->Element(key);
	if(pSubVar && pSubVar->GetType() == VAR_TABLE)
	{
		_TReadBoolFromScript(&kSwitch.bAccePlaneOff,		pSubVar->Element("acceplane"));
		_TReadBoolFromScript(&kSwitch.bCountDownTimeOff,	pSubVar->Element("countdowntime"));
		_TReadBoolFromScript(&kSwitch.bEagleMapOff,			pSubVar->Element("eaglemap"));
		_TReadBoolFromScript(&kSwitch.bElapseTimeOff,		pSubVar->Element("elapsetime"));
		_TReadBoolFromScript(&kSwitch.bItemColumnOff,		pSubVar->Element("itemcolumn"));
		_TReadBoolFromScript(&kSwitch.bPlayerRankOff,		pSubVar->Element("playerrank"));
		_TReadBoolFromScript(&kSwitch.bSinglePowerCtrlOff,	pSubVar->Element("singlepowerctrl"));
		_TReadBoolFromScript(&kSwitch.bTeamPowerCtrlOff,	pSubVar->Element("teampowerctrl"));
		_TReadBoolFromScript(&kSwitch.bResetOff,			pSubVar->Element("reset"));
	}
	else
	{
		kSwitch.bAccePlaneOff		= false;
		kSwitch.bCountDownTimeOff	= false;
		kSwitch.bEagleMapOff		= false;		
		kSwitch.bElapseTimeOff		= false;	
		kSwitch.bItemColumnOff		= false;		
		kSwitch.bPlayerRankOff		= false;		
		kSwitch.bSinglePowerCtrlOff	= false;
		kSwitch.bTeamPowerCtrlOff	= false;
		kSwitch.bResetOff			= false;			

	}
}

//从脚本中读入任务配置
inline void _TReadTaskFromScript(CVar* pVar, TASKCONFIG& tagTask)
{
	if(pVar && pVar->GetType() == VAR_TABLE)
	{
		_TReadIntFromScript(&tagTask.dwID, pVar->Element("Id"));
		_TReadIntFromScript(&tagTask.dwMapID, pVar->Element("MapId"));
		_TReadStringFromScript(tagTask.strDesc, pVar->Element("Desc"));
		_TReadKeyFromScript(pVar, "PreKey", tagTask.listPreKey);
		_TReadSwitchTableFromScript(pVar, "SwitchTable", tagTask.kSwitchTable);
		_TReadAtomListFromScript(pVar, "TaskPeriodAtom", tagTask.listTaskPeriodAtom);
		_TReadSubTaskListFromScript(pVar, "SubTask", tagTask.listSubTask);
	}
}