#include "Top-KartPCH.h"
#include "GPReverseKeyAffact.h"
#include "KeyEventManager.h"
#include "gpcontrol.h"

unsigned int CGPReverseKeyAffect::m_unRefCout = 0;

CGPReverseKeyAffect::CGPReverseKeyAffect():CGPAffectBase()
{
	
}

CGPReverseKeyAffect::~CGPReverseKeyAffect()
{
	
}

void CGPReverseKeyAffect::Start()
{
	IGPTargetIterator* pIter = CGPControl::Get()->GetTargetIterator();
	if ( m_pDestTarget == pIter->GetHost() 
		&& ZKeyEventManager::Get() )
	{
		++m_unRefCout;
		ZKeyEventManager::Get()->ActivateKeyEvent(ZKeyEvent::EKET_DIR_REVERSE);
		ZKeyEventManager::Get()->DeactivateKeyEvent(ZKeyEvent::EKET_DIR_NORMAL);
	}
	CGPAffectBase::Start();
}

void CGPReverseKeyAffect::Stop()
{
	IGPTargetIterator* pIter = CGPControl::Get()->GetTargetIterator();
	if ( m_pDestTarget == pIter->GetHost() 
		&& ZKeyEventManager::Get() )
	{
		--m_unRefCout;
		if(m_unRefCout == 0)
		{
			ZKeyEventManager::Get()->DeactivateKeyEvent(ZKeyEvent::EKET_DIR_REVERSE);
			ZKeyEventManager::Get()->ActivateKeyEvent(ZKeyEvent::EKET_DIR_NORMAL);
		}		
	}		
	CGPAffectBase::Stop();
}