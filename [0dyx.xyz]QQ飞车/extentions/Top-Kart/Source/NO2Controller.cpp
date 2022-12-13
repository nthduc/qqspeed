#include "Top-KartPCH.h"
#include "NO2Controller.h"
#include "KeyEventManager.h"

CNO2Controller::CNO2Controller(): m_unTerminateTime(500), m_unStartTime(0), m_bIsActive(false)
{

}

CNO2Controller::~CNO2Controller()
{

}

void CNO2Controller::ActivateNO2()
{
	if(ZKeyEventManager::Get())
	{
		ZKeyEventManager::Get()->ActivateKeyEvent(ZKeyEvent::EKET_NO2);
		m_bIsActive = true;
		m_unStartTime = GetTickCount();
	}
}

void CNO2Controller::DeactivateNO2()
{
	if(ZKeyEventManager::Get())
	{
		ZKeyEventManager::Get()->DeactivateKeyEvent(ZKeyEvent::EKET_NO2);
		m_bIsActive = false;
	}
}

void CNO2Controller::SetTerminateTime(unsigned int unElapse)
{
	m_unTerminateTime = unElapse;
}

bool CNO2Controller::GetActive()
{
	return m_bIsActive;
}

void CNO2Controller::Update()
{
	if(m_bIsActive)
	{	
		if((GetTickCount() - m_unStartTime) > m_unTerminateTime)
		{
			DeactivateNO2();
		}
	}
}