////////////////////////////////////////////////
//     FileName:    DetectorMgr.h
//     Author:      rayhuang
//     Date:        2007-5-18
//     Description: ¼ì²âÆ÷¿ØÖÆÀà
////////////////////////////////////////////////

#pragma once
#include "TaskDefine.h"
#include "DetectorBase.h"
#include "variant.h"
#include "singlet.h"

class CDetectorProducer : public CSingleT<CDetectorProducer>
{
public:
	CDetectorProducer();
	~CDetectorProducer();
	bool Init();
	CDetectorBase* CreateDetector(DWORD dwID);

private:
	CVarTable* _GetDetectorCfg(DWORD dwID);

private:
	CScriptState m_lsDetector;
};