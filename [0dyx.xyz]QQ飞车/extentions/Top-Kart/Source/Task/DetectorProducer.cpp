#include "Top-KartPCH.h"
#include "DetectorProducer.h"
#include "DetectorBase.h"
#include "TaskScript.h"
#include "BNPress_Det.h"
#include "BnPressTime_Det.h"
#include "Timer_Det.h"
#include "ExecuteCount_Det.h"
#include "CDTime_Det.h"
#include "Drift_Det.h"


CDetectorProducer::CDetectorProducer()
{

}

CDetectorProducer::~CDetectorProducer()
{

}

bool CDetectorProducer::Init()
{
	if(m_lsDetector.DoFile("TaskConfig/detector.lua") == 0)
	{
		return true;
	}
	return false;
}

CDetectorBase* CDetectorProducer::CreateDetector(DWORD dwID)
{
	CVarTable* vtCfg = NULL;
	CVarArray vtRoot("detector");
	vtRoot.GetFromScript(&m_lsDetector);
	for(int i = 0; i < vtRoot.GetNum(); ++i)
	{
		vtCfg =(CVarTable*)vtRoot.Element(i);
		DWORD dwDetectorId;
		_TReadIntFromScript(&dwDetectorId, vtCfg->Element("id"));
		if (dwDetectorId == dwID)
		{
			break;
		}
	}

	if(!vtCfg)
	{
		return NULL;
	}
	CDetectorBase* pDetector = NULL;
	switch(vtCfg->Element("type")->Integer())
	{
	case EDT_Timer:
		{
			pDetector = new CTimer_Det;
			pDetector->LoadConfig(vtCfg);
		}
		break;
	case EDT_BNPress:
		{
			pDetector = new CBNPress_Det;
			pDetector->LoadConfig(vtCfg);
		}
		break;
	case EDT_BNPressTime:
		{
			pDetector = new CBnPressTime_Det;
			pDetector->LoadConfig(vtCfg);
		}
		break;
	case EDT_ExecuteCount:
		{
			pDetector = new CExecuteCout_Det;
			pDetector->LoadConfig(vtCfg);
		}
		break;
	case EDT_CDTimer:
		{
			pDetector = new CCDTime_Det;
			pDetector->LoadConfig(vtCfg);
		}
		break;
	case EDT_Speed:
		{
			pDetector = new CSpeed_Det;
			pDetector->LoadConfig(vtCfg);
		}
		break;
	case EDT_Drift_Speed:
		{
			pDetector = new CDrift_Speed_Det;
			pDetector->LoadConfig(vtCfg);
		}
		break;
	case EDT_TurnCar_Drift:
		{
			pDetector = new CTurnCar_Drift_Det;
			pDetector->LoadConfig(vtCfg);
		}
		break;
	case EDT_SmallNo2:
		{
			pDetector = new CSmallNo2_Det;
			pDetector->LoadConfig(vtCfg);
		}
		break;
	default:
		break;

	}
	return pDetector;	
}

CVarTable* CDetectorProducer::_GetDetectorCfg(DWORD dwID)
{
	CVarArray vtRoot("detector");
	vtRoot.GetFromScript(&m_lsDetector);
	for(int i = 0; i < vtRoot.GetNum(); ++i)
	{
		CVarTable* pSub =(CVarTable*)vtRoot.Element(i);
		DWORD dwDetId;
		_TReadIntFromScript(&dwDetId, pSub->Element("id"));

		if (dwDetId == dwID)
		{
			return &(*pSub);
		}
	}
	return NULL;
}