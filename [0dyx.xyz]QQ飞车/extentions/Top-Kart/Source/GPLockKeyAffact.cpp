#include "Top-KartPCH.h"
#include "GPLockKeyAffact.h"
#include "KeyEventManager.h"
#include "gpcontrol.h"

CGPLockKeyAffact::CGPLockKeyAffact():CGPAffectBase()
{

}


CGPLockKeyAffact::~CGPLockKeyAffact()
{

}

void CGPLockKeyAffact::Start()
{
	IGPTargetIterator* pIter = CGPControl::Get()->GetTargetIterator();
	if ( m_pDestTarget == pIter->GetHost() 
		&& ZKeyEventManager::Get() )
	{
		ZKeyEventManager::Get()->DeactivateKeyEvent(ZKeyEvent::EKET_DIR_NORMAL);
		ZKeyEventManager::Get()->DeactivateKeyEvent(ZKeyEvent::EKET_ACCE);
		ZKeyEventManager::Get()->DeactivateKeyEvent(ZKeyEvent::EKET_SHIFT);
	}
	CGPAffectBase::Start();
}


void CGPLockKeyAffact::Stop()
{
	IGPTargetIterator* pIter = CGPControl::Get()->GetTargetIterator();
	if ( m_pDestTarget == pIter->GetHost() 
		&& ZKeyEventManager::Get() )
	{
		ZKeyEventManager::Get()->ActivateKeyEvent(ZKeyEvent::EKET_DIR_NORMAL);
		ZKeyEventManager::Get()->ActivateKeyEvent(ZKeyEvent::EKET_ACCE);
		ZKeyEventManager::Get()->ActivateKeyEvent(ZKeyEvent::EKET_SHIFT);
	}
	CGPAffectBase::Stop();
}