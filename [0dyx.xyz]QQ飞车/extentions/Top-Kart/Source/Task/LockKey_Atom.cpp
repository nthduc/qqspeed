#include "Top-KartPCH.h"
#include "LockKey_Atom.h"
#include "../KeyEventManager.h"
#include "../Play.h"
#include "../PhysKart/TencentCar.h"

CLockKey_Atom::CLockKey_Atom()
{

}

CLockKey_Atom::~CLockKey_Atom()
{

}

void CLockKey_Atom::Start()
{
	if( ZKeyEventManager::Get() )
	{
		ZKeyEventManager::Get()->DeactivateKeyEvent(ZKeyEvent::EKET_DIR_NORMAL);
		ZKeyEventManager::Get()->DeactivateKeyEvent(ZKeyEvent::EKET_ACCE);
		ZKeyEventManager::Get()->DeactivateKeyEvent(ZKeyEvent::EKET_SHIFT);
	}	
	if (NxPlayMgr::GetHostPlay()->GetKart())
	{
		NxPlayMgr::GetHostPlay()->SetCharaterAnimation("Still");
		NxPlayMgr::GetHostPlay()->GetKart()->SetMovable(false);		
	}
}

void CLockKey_Atom::_Stop()
{
	if( ZKeyEventManager::Get() )
	{
		ZKeyEventManager::Get()->ActivateKeyEvent(ZKeyEvent::EKET_DIR_NORMAL);
		ZKeyEventManager::Get()->ActivateKeyEvent(ZKeyEvent::EKET_ACCE);
		ZKeyEventManager::Get()->ActivateKeyEvent(ZKeyEvent::EKET_SHIFT);
	}
	if (NxPlayMgr::GetHostPlay()->GetKart())
	{
		 NxPlayMgr::GetHostPlay()->GetKart()->SetMovable(true);
	}
}