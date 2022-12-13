////////////////////////////////////////////////
//     FileName:    lv.h
//     Author:      Fish
//     Date:        2006-10-24 17:43:49
//     Description: []
////////////////////////////////////////////////
#pragma once

#include "txtCtrlr.h"
#include "fileop.h"

#define LEVEL_CFG		"levels.txt"
inline void GetLevel(INT Exp, INT* Level, INT* SubExp, INT* LevelExp)
{
	*Level = 0;
	*SubExp = 0;
	*LevelExp = 0;
	CHAR szLevelFile[MAX_PATH] = "\0";
	FullName(LEVEL_CFG, szLevelFile);
	CtxtCtrlr ctrl;
	if ( !ctrl.Open(szLevelFile) )
		return;
	INT nIndex = 0;
	INT nLower = 0;
	INT nHigh = 0;
	INT nTotal = 0;
	while(1)
	{
		nIndex++;
		if ( ! ctrl.Next() )
			break;
		CtxtRecordset* pRecordset = ctrl.GetRecordset();
		if ( pRecordset->GetFieldCount() < 1 )
			break;
		nHigh = atoi(pRecordset->GetField(0));
		nTotal += nHigh;
		if ( nTotal >= Exp )
			break;
		else
			nLower += nHigh;
	}
	*Level = nIndex;
	*SubExp = Exp - nLower;
	*LevelExp = nHigh;
}