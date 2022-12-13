#include "Top-KartPCH.h"
#include "Drift_Det.h"
#include "../DescConstant.h"
#include "../NO2Controller.h"
#include "../gpdefine.h"

CSpeed_Det::CSpeed_Det():
m_fTargetSpeed(0)
{

}

CSpeed_Det::~CSpeed_Det()
{

}

void CSpeed_Det::Update(float fTime)
{
	float fCurSpeed = NxPlayMgr::GetHostPlay()->GetKart()->GetVelocity() * GameSetting::SpeedDispCoef;
	if(fCurSpeed > m_fTargetSpeed)
	{
		_SetState(EDS_Success);
	}
	CDetectorBase::Update(fTime);
}

bool CSpeed_Det::LoadConfig(CVarTable* pTable)
{
	CVarTable* pVarParam = (CVarTable*)pTable->Element("params");
	if(pVarParam)
	{
		_TReadFloatFromScript(&m_fTargetSpeed, pVarParam->Element("targetspeed"));
		return true;
	}
	return false;
}

//----------------------------------------------------------------------
CDrift_Speed_Det::CDrift_Speed_Det():
m_fTargetSpeed(0)
{

}

CDrift_Speed_Det::~CDrift_Speed_Det()
{

}

void CDrift_Speed_Det::Update(float fTime)
{
	if(NxPlayMgr::GetHostPlay())
	{
		float fCurSpeed = NxPlayMgr::GetHostPlay()->GetKart()->GetVelocity() * GameSetting::SpeedDispCoef;
		if(fCurSpeed < m_fTargetSpeed)
		{
			_SetState(EDS_Fail);
		}
		else
		{
			for(int i = 0; i < 2; ++i)
			{
				if(!InputManager::keys[m_arrKey[i]])		//�Ƿ�ͬʱ����������Ư�ƣ�
				{
					return;
				}
			}
			_SetState(EDS_Success);			//�ɹ�
		}
		/*else if(NxPlayMgr::GetHostPlay()->GetKart()->IsDrifting())
		{
			_SetState(EDS_Success);
		}*/
	}
	CDetectorBase::Update(fTime);	
}

bool CDrift_Speed_Det::LoadConfig(CVarTable* pTable)
{
	CVarTable* pVarParam = (CVarTable*)pTable->Element("params");
	if(pVarParam)
	{
		_TReadFloatFromScript(&m_fTargetSpeed, pVarParam->Element("targetspeed"));
		DWORD dwKey;
		_TReadIntFromScript(&dwKey, pVarParam->Element("key1"));
		m_arrKey[0] = (NiInputKeyboard::KeyCode)dwKey;
		_TReadIntFromScript(&dwKey, pVarParam->Element("key2"));
		m_arrKey[1] = (NiInputKeyboard::KeyCode)dwKey;
		return true;
	}
	return false;
}


//----------------------------------------------------------------------

CTurnCar_Drift_Det::CTurnCar_Drift_Det()
{
	
}

CTurnCar_Drift_Det::~CTurnCar_Drift_Det()
{

}

void CTurnCar_Drift_Det::Update(float fTime)
{
	if(NxPlayMgr::GetHostPlay())
	{
		if(NxPlayMgr::GetHostPlay()->GetKart()->IsDrifting())	//����Ƿ���Ư��
		{
			if(CNO2Controller::Get()->GetActive())		//С���Ѿ���������ˣ�ʧ��
			{
				_SetState(EDS_Success);
			}
			//for(int i = 0; i < 2; ++i)
			//{
			//	if(!InputManager::keys[m_arrKey[i]])		//�Ƿ�ͬʱ�����������ذᳵͷ��
			//	{
			//		return;
			//	}
			//}
			//_SetState(EDS_Success);			//�ɹ�
		}
		else
		{
			_SetState(EDS_Fail);			//Ư���Ѿ�������ʧ��
		}
	}
	CDetectorBase::Update(fTime);
	
}

bool CTurnCar_Drift_Det::LoadConfig(CVarTable* pTable)
{
	CVarTable* pVarParam = (CVarTable*)pTable->Element("params");
	if(pVarParam)
	{
		DWORD dwKey;
		_TReadIntFromScript(&dwKey, pVarParam->Element("key1"));
		m_arrKey[0] = (NiInputKeyboard::KeyCode)dwKey;
		_TReadIntFromScript(&dwKey, pVarParam->Element("key2"));
		m_arrKey[1] = (NiInputKeyboard::KeyCode)dwKey;
		return true;
	}
	return false;
}

//------------------------------------------------------------------
CSmallNo2_Det::CSmallNo2_Det()
{

}

CSmallNo2_Det::~CSmallNo2_Det()
{

}

bool CSmallNo2_Det::_IsStartNo2()
{
	GP_DATA_LIST gp_List = NxPlayMgr::GetHostPlay()->GetAffectGPs();
	GP_DATA_LIST_Iterator gp_ListIt = gp_List.begin();
	for(; gp_ListIt != gp_List.end(); ++gp_ListIt)
	{
		if((*gp_ListIt).dwID == 2001)
		{
			return true;
		}
	}
	return false;
}

void CSmallNo2_Det::Update(float fTime)
{
	if(CNO2Controller::Get())
	{
		if(!CNO2Controller::Get()->GetActive())		//С���Ѿ���������ˣ�ʧ��
		{
			_SetState(EDS_Fail);
		}
		else if(_IsStartNo2())
		{
			_SetState(EDS_Success);				//����С��ɹ�
		}
	}
	CDetectorBase::Update(fTime);
}
	