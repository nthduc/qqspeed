//-----------------------------------------------------------------------
// Create by hoverzhao for snap Keyboard Event
//-----------------------------------------------------------------------


#include "Top-KartPCH.h"

#include "GameMain.h"
#include "MommaTasks.h"

#include "LM_Anim.h"
#include "left_Anim.h"
#include "EventContainer.h"

#include "Scheduler.h"
#include "GameStatusManager.h"
#include "EffectManager.h"
//#include "NiMenu/MenuManager.h"
#include "./PhysKart/NxAllVehicles.h"
#include "./physKart/TencentCar.h"
#include "./physKart/DriftCenter.h"
#include "Track/KartPlayback.h"
#include "Track/KartStatusManager.h"
#include "Track/GlobalMap.h"

#include "KeyEventManager.h"
#include "NxSceneMgr.h"
#include "PowerController.h"
#include "BaseItem.h"
#include "SceneLevel.h"
#include "./Platform/GlobalInterface.h"
#include "gpcontrol.h"
#include "NO2Controller.h"
#include "DataCollecter.h"
#include "fileop.h"

#ifdef WIN32
#include <NiDI8InputSystem.h>
#endif


static NiInputSystemPtr m_spStaticInputSystem;
ZKeyEventManager::ZKeyEventManager()
{
}

ZKeyEventManager::~ZKeyEventManager()
{
	UnInitEvents();
	m_pkZKeyEventManager = NULL;
}

#define INITKEYEVENTPARAM	 \
	NiInputKeyboard* pkKeyboard = m_pInputSystem->GetKeyboard();	\
	NiInputMouse* pkMouse = m_pInputSystem->GetMouse();		\
	NiInputGamePad* pkGamePad = InputManager::Get()->GetInputSystem()->GetGamePad(0,0);		\
	if (!pkMouse && !pkKeyboard)	\
		return;

#define ADDKEY_EVENT(x,y,z)	\
	y = NiNew x;	\
	z.push_back(y);

ZKeyEventManager * ZKeyEventManager::m_pkZKeyEventManager = NULL;

#include "nimemorydefines.h"
//-----------------------------------------------------------------------
bool ZKeyEventManager::InitEvents()
{

	ZKeyEvent * pEvent = NULL; 
	ADDKEY_EVENT( ZKeyFuntionKey,	   pEvent, m_listActiveEvent)
	ADDKEY_EVENT( ZKeyAcceSpeed,	   pEvent, m_listActiveEvent)
	ADDKEY_EVENT( ZKeySelectMenu,	   pEvent, m_listActiveEvent)
	ADDKEY_EVENT( ZKeyShift,		   pEvent, m_listActiveEvent)
	ADDKEY_EVENT( ZKeyDirection,	   pEvent, m_listActiveEvent)
	ADDKEY_EVENT( ZKeyReverseDir,	   pEvent, m_listInactiveEvent)
	ADDKEY_EVENT( ZKeyNo2,			   pEvent, m_listInactiveEvent)
	ADDKEY_EVENT( ZKeyJump,			   pEvent, m_listActiveEvent)
	// .....s
	InputManager::keys[NiInputKeyboard::KEY_LCONTROL] = false;
	return true;
}

//-----------------------------------------------------------------------
/*
void ZKeyEventManager::SetEnable(bool bEnable)
{
	for ( KeyEventIter it = m_listActivedEvent.begin(); it != m_listActivedEvent.end(); it++)
	{
		(*it)->SetEnable(bEnable);
	}
}
*/
//-----------------------------------------------------------------------

void ZKeyEventManager::Update()
{
/*	for ( KeyEventIter it = m_listActiveEvent.begin(); it != m_listActiveEvent.end(); it++)
	{
			(*it)->Trigger();		
	}
	for ( KeyEventIter it = m_listActiveEvent.begin(); it != m_listActiveEvent.end(); ++it)
	{
		if(!(*it)->IsActive())
		{
			KeyEventIter temp = it;
			it++;			
			DeactivateKeyEvent(temp);
		}
	}
*/
	KeyEventIter it = m_listActiveEvent.begin();
	while(it != m_listActiveEvent.end())
	{
		if((*it)->IsActive())
		{
			(*it)->Trigger();		
		}
		else
		{
			KeyEventIter temp_it = it;
			it++;	
			DeactivateKeyEvent(temp_it);			
			continue;
		}
		it++;
	}
}



/*
void ZKeyEventManager::CheckCommonEvents()
{
	for ( KeyEventIter it = m_listActivedEvent.begin(); it != m_listActivedEvent.end(); it++)
	{
		(*it)->Trigger();
	}
}
void ZKeyEventManager::CheckSpecialEvents()
{
	for ( KeyEventIter it = m_vpSpecialEvent.begin(); it != m_vpSpecialEvent.end(); it++)
	{
		(*it)->Trigger();
	}
}
*/
//-----------------------------------------------------------------------

ZKeyEventManager * ZKeyEventManager::Create()
{
	m_pkZKeyEventManager = NiNew ZKeyEventManager();
	if(m_pkZKeyEventManager)
	{
		if(m_pkZKeyEventManager->InitEvents())
		{	return m_pkZKeyEventManager;}
		else
		{	NiDelete m_pkZKeyEventManager; m_pkZKeyEventManager = NULL; }
	}

	return m_pkZKeyEventManager;
}

//-----------------------------------------------------------------------

void ZKeyEventManager::Destory()
{
	NiDelete m_pkZKeyEventManager;
}

//-----------------------------------------------------------------------

void ZKeyEventManager::UnInitEvents()
{
	while(!m_listActiveEvent.empty())
	{
		for( KeyEventIter it = m_listActiveEvent.begin(); it != m_listActiveEvent.end(); it++)
		{
			 NiDelete (*it);
			 m_listActiveEvent.erase(it);
			 break;
		}
	}
	while(!m_listInactiveEvent.empty())
	{
		for( KeyEventIter it = m_listInactiveEvent.begin(); it != m_listInactiveEvent.end(); it++)
		{
			NiDelete (*it);
			m_listInactiveEvent.erase(it);
			break;
		}
	}
}
//----------------------------------------------------------------------
KeyEventIter ZKeyEventManager::FindInList(KeyEventList &keyList, ZKeyEvent::ENMKEYEVENTTYPE eType)
{

	KeyEventIter key_it = keyList.begin();
	for(; key_it != keyList.end(); ++key_it)
	{
		if((*key_it)->GetType() == eType)
		{
			return key_it;
		}
	}
	return keyList.end();
}
//----------------------------------------------------------------------
void ZKeyEventManager::DeactivateKeyEvent(const KeyEventIter &it)
{
	(*it)->SetActive(false);
	m_listInactiveEvent.push_back(*it);
	m_listActiveEvent.erase(it);
}
//----------------------------------------------------------------------
void ZKeyEventManager::ActivateKeyEvent(ZKeyEvent::ENMKEYEVENTTYPE eType)
{

	if( FindInList(m_listActiveEvent, eType) == m_listActiveEvent.end() )
	{
		KeyEventIter inactiveKey_it = FindInList(m_listInactiveEvent, eType);
		if(inactiveKey_it != m_listInactiveEvent.end())
		{
			(*inactiveKey_it)->SetActive(true);
			m_listActiveEvent.push_back(*inactiveKey_it);
			m_listInactiveEvent.erase(inactiveKey_it);
		}

	}
}
//-----------------------------------------------------------------------
void ZKeyEventManager::DeactivateKeyEvent(ZKeyEvent::ENMKEYEVENTTYPE eType)
{
	KeyEventIter activeKey_it = FindInList(m_listActiveEvent, eType);
	if( activeKey_it != m_listActiveEvent.end() )
	{
		if( FindInList(m_listInactiveEvent, eType) == m_listInactiveEvent.end() )
		{
			(*activeKey_it)->SetActive(false);
			m_listInactiveEvent.push_back(*activeKey_it);
			m_listActiveEvent.erase(activeKey_it);
		}
	}
}
//-----------------------------------------------------------------------
void ZKeyEventManager::RegEvent(ZKeyEvent * pKeyEvent)
{
	if( FindInList(m_listActiveEvent, pKeyEvent->GetType()) == m_listActiveEvent.end()
		&& FindInList(m_listInactiveEvent, pKeyEvent->GetType()) == m_listInactiveEvent.end() )
	{
		m_listInactiveEvent.push_back(pKeyEvent);
	}	
}

//-----------------------------------------------------------------------

void ZKeyEventManager::UnRegEvent(ZKeyEvent::ENMKEYEVENTTYPE eType)
{
	DeactivateKeyEvent(eType);
	KeyEventIter inactive_it = FindInList(m_listInactiveEvent, eType);
	if(inactive_it != m_listInactiveEvent.end())
	{
		NiDelete (*inactive_it);
		m_listInactiveEvent.erase(inactive_it);
	}
}

	


//-----------------------------------------------------------------------

ZKeyEvent::ZKeyEvent(ENMKEYEVENTTYPE eKeyType)
{
	m_pInputSystem	= InputManager::Get()->GetInputSystem();
	m_eKeyEventType = eKeyType;
//	m_bIsActive		= true;
}

ZKeyEvent::ENMKEYEVENTTYPE ZKeyEvent::GetType()
{
	return m_eKeyEventType;
}

bool ZKeyEvent::IsActive()
{
	return m_bIsActive;
}

void ZKeyEvent::SetActive(bool state)
{
	m_bIsActive = state;
}

ZKeyEvent::~ZKeyEvent()
{ 
	m_pInputSystem = NULL;
};

//-----------------------------------------------------------------------
void ZKeyFuntionKey::Trigger()
{
	INITKEYEVENTPARAM
	if (pkKeyboard->KeyWasPressed(NiInputKeyboard::KEY_F11))
	{
		((GameMain*)
			(NiApplication::ms_pkApplication))->ToggleShowAllTrackers();
	}

	/*
	if (pkKeyboard->KeyIsDown(NiInputKeyboard::KEY_F1) ||
		pkKeyboard->KeyIsDown(NiInputKeyboard::KEY_F1))
	{
		m_pInputSystem->ConfigureDevices();
	}
	*/
	if (pkKeyboard->KeyWasPressed(NiInputKeyboard::KEY_F3)) 
	{
		CKartStatusManager::EnableReset(!CKartStatusManager::GetResetState());
	}

	if (pkKeyboard->KeyWasPressed(NiInputKeyboard::KEY_R))
	{
/*		if (m_bEnable) 
		{
			Sleep(300);
			CKartStatusManager::Get()->OnTriggerExceptionEvent(KART_EXCEPTSTATE_RESET, NULL);
			if (CLevelManager::Get()->GetPlayBack())
				CLevelManager::Get()->GetPlayBack()->OnKeyDown(NiInputKeyboard::KEY_F, 
				true, NiApplication::ms_pkApplication->GetCurrentTime());
		}

*/
		//Sleep(300);
		CKartStatusManager::Get()->OnTriggerExceptionEvent(KART_EXCEPTSTATE_RESET, NULL);
		if (CLevelManager::Get()->GetPlayBack())
			CLevelManager::Get()->GetPlayBack()->OnKeyDown(NiInputKeyboard::KEY_F, 
			true, NiApplication::ms_pkApplication->GetCurrentTime());
	}

	if (pkKeyboard->KeyWasPressed(NiInputKeyboard::KEY_F2)) 
	{
		if (CGlobalMap::Get())
			CGlobalMap::Get()->ShowGlobalMap(!(CGlobalMap::Get()->IsVisible()));    		
	}	
	if (pkKeyboard->KeyWasPressed(NiInputKeyboard::KEY_U))
	{
		CALL(CNO2Controller::Get(), ActivateNO2());
	}
	if (pkKeyboard->KeyWasPressed(NiInputKeyboard::KEY_I))
	{
		NxSceneMgr::Get()->GetCameraController()->SetForeCam(CCameraBase::ECT_AIRSCAPE);
		NxSceneMgr::Get()->GetCameraController()->GetForeCam()->SetTarget(NxPlayMgr::GetHostPlay());
	}
	if (pkKeyboard->KeyWasPressed(NiInputKeyboard::KEY_O))
	{
		NxSceneMgr::Get()->GetCameraController()->SetForeCam(CCameraBase::ECT_RACE);
		NxSceneMgr::Get()->GetCameraController()->GetForeCam()->SetTarget(NxPlayMgr::GetHostPlay());
		NxSceneMgr::Get()->GetCameraController()->GetForeCam()->SetRotate(NxPlayMgr::GetHostPlay()->GetKartNIFRoot()->GetRotate());
	}
	if (pkKeyboard->KeyWasPressed(NiInputKeyboard::KEY_L))
	{
		NxSceneMgr::Get()->GetCameraController()->LoadConfig();
	}
	if (pkKeyboard->KeyWasPressed(NiInputKeyboard::KEY_K))
	{
		Play * pHostPlay  = NULL;
		pHostPlay = NxPlayMgr::GetHostPlay();
		if( pHostPlay )
		{
			CTencentCar * pCar =  NULL;
			pCar = pHostPlay->GetKart()->GetVehicle();
			if (pCar)
			{
				pCar->GetDriftCenter()->LoadConfig();
				pCar->InitCarPhysParam();
				pCar->InitCarMotor();
			}

		}
	}

	if (pkKeyboard->KeyWasPressed(NiInputKeyboard::KEY_INSERT))
	{
		CALL(CDataCollecter::Get(), Start());
	}
	if (pkKeyboard->KeyWasPressed(NiInputKeyboard::KEY_DELETE))
	{
		CALL(CDataCollecter::Get(), Stop());
	}
}

//-----------------------------------------------------------------------
void ZKeySelectMenu::Trigger()
{
	INITKEYEVENTPARAM
}
//-----------------------------------------------------------------------

void ZKeyJump::Trigger()
{
	INITKEYEVENTPARAM
	Play * pPlay = NULL;
	CTencentCar * pCar = NULL;
	pPlay = NxPlayMgr::GetHostPlay();

	if (!pPlay )
	{
		return;
	}

	pCar = pPlay->GetKart()->GetVehicle();

	if ( !pCar )
	{
		return;
	}

	if ( pkKeyboard->KeyIsDown( NiInputKeyboard::KEY_SPACE)   )
	{
		if ( !FileExists("jump") )
			return;


		pCar->SetJumpAccumulate();
	}

	if ( pkKeyboard->KeyWasReleased( NiInputKeyboard::KEY_SPACE)  )
	{
		if ( !FileExists("jump") )
			return;

		pCar->TirggerJumpMotion();
	}

}


#include "one_anim.h"
//-----------------------------------------------------------------------
void ZKeyAcceSpeed::Trigger()
{
	INITKEYEVENTPARAM

/*	InputManager::keys[NiInputKeyboard::KEY_LCONTROL] = false;

	if( pkKeyboard->KeyWasPressed(NiInputKeyboard::KEY_LCONTROL) ||
		pkKeyboard->KeyWasPressed(NiInputKeyboard::KEY_RCONTROL))
	{
		EventContainer::Get()->PushAnimateEvent(EventContainer::ANIM_SPPEDUP);
		InputManager::keys[NiInputKeyboard::KEY_LCONTROL] = true;
		//LevelManager::Get()->GetPlay()->StartAnimation(one_Anim::SPEEDUP);
		if ( NxSceneMgr::Get()->GetCameraController())
		NxSceneMgr::Get()->GetCameraController()->SetCameraParamType(CameraController::CPT_SPEEDUP);

	}


	if( pkKeyboard->KeyWasReleased(NiInputKeyboard::KEY_LCONTROL) || 
		pkKeyboard->KeyWasReleased(NiInputKeyboard::KEY_RCONTROL ))
	{
		InputManager::keys[NiInputKeyboard::KEY_LCONTROL] = false;
	}

*/
	if(pkKeyboard->KeyWasPressed(NiInputKeyboard::KEY_LCONTROL)||
		pkKeyboard->KeyWasPressed(NiInputKeyboard::KEY_RCONTROL))
	{
		InputManager::keys[NiInputKeyboard::KEY_LCONTROL] = true;
		//if(CItemMgr::Get())
		//	CItemMgr::Get()->UseItem();		
	}
	else
	{
		InputManager::keys[NiInputKeyboard::KEY_LCONTROL] = false;
	}
}

void ZKeyShift::Trigger()
{
	INITKEYEVENTPARAM
    bool bUsingKeyboardInput = false;

    float fMimicGP = 1.0f;
	if (pkKeyboard->KeyIsDown(NiInputKeyboard::KEY_LSHIFT) ||
		pkKeyboard->KeyIsDown(NiInputKeyboard::KEY_RSHIFT))
	{
		fMimicGP = 0.25f;
		bUsingKeyboardInput = true;
/*		if(NxSceneMgr::Get()->GetPowerController())
			NxSceneMgr::Get()->GetPowerController()->AccumulatePower(2);
*/	}
	InputManager::keys[NiInputKeyboard::KEY_LSHIFT] = false;
	if (pkKeyboard->KeyIsDown(NiInputKeyboard::KEY_LSHIFT)||pkKeyboard->KeyIsDown(NiInputKeyboard::KEY_RSHIFT))
	{
		// bug 临时代码
		if ( NxSceneMgr::Get()->GetGameStatusManager()->GetGameState() == GameStatusManager::GST_ROOM)
			return;

		InputManager::keys[NiInputKeyboard::KEY_LSHIFT] = true;
		CLevelManager::Get()->GetPlayBack()->OnKeyDown(NiInputKeyboard::KEY_LSHIFT, true, 
			NiApplication::ms_pkApplication->GetCurrentTime());
	}

}

void ZKeyDirection::Trigger()
{
	INITKEYEVENTPARAM

	InputManager::keys[NiInputKeyboard::KEY_A] = false;
	InputManager::keys[NiInputKeyboard::KEY_D] = false;
	InputManager::keys[NiInputKeyboard::KEY_W] = false;
	InputManager::keys[NiInputKeyboard::KEY_S] = false;


	if (pkKeyboard->KeyIsDown(NiInputKeyboard::KEY_A)
		|| pkKeyboard->KeyIsDown(NiInputKeyboard::KEY_LEFT))
	{
		if(InputManager::keys[NiInputKeyboard::KEY_LSHIFT])
		{
			CALL(NxPlayMgr::GetHostPlay(), PushPlayerAnimEvent(CAnimEventMgr::EAET_TURNLEFTBIG));
		}
		else
		{
			CALL(NxPlayMgr::GetHostPlay(), PushPlayerAnimEvent(CAnimEventMgr::EAET_TURNLEFT));
		}
		InputManager::keys[NiInputKeyboard::KEY_A] = true;		
		CLevelManager::Get()->GetPlayBack()->OnKeyDown(NiInputKeyboard::KEY_A, 
			true, NiApplication::ms_pkApplication->GetCurrentTime());
	
	}

	if ( pkKeyboard->KeyWasReleased(NiInputKeyboard::KEY_A)
		|| ( pkKeyboard->KeyWasReleased(NiInputKeyboard::KEY_LEFT) && !pkKeyboard->KeyIsDown(NiInputKeyboard::KEY_RIGHT)))
	{
		if(NxPlayMgr::GetHostPlay())
			NxPlayMgr::GetHostPlay()->PushPlayerAnimEvent(CAnimEventMgr::EAET_STILL);
	}

	if (pkKeyboard->KeyIsDown(NiInputKeyboard::KEY_D)
		|| pkKeyboard->KeyIsDown(NiInputKeyboard::KEY_RIGHT))
	{
		if(InputManager::keys[NiInputKeyboard::KEY_LSHIFT])
		{
			CALL(NxPlayMgr::GetHostPlay(), PushPlayerAnimEvent(CAnimEventMgr::EAET_TURNRIGHTBIG));
		}
		else
		{
			CALL(NxPlayMgr::GetHostPlay(), PushPlayerAnimEvent(CAnimEventMgr::EAET_TURNRIGHT));
		}
		InputManager::keys[NiInputKeyboard::KEY_D] = true;
		CLevelManager::Get()->GetPlayBack()->OnKeyDown(NiInputKeyboard::KEY_D, 
			true, NiApplication::ms_pkApplication->GetCurrentTime());
	
	}

	if (pkKeyboard->KeyWasReleased(NiInputKeyboard::KEY_D)
		|| ( !pkKeyboard->KeyIsDown(NiInputKeyboard::KEY_LEFT) && pkKeyboard->KeyWasReleased(NiInputKeyboard::KEY_RIGHT)))
	{
		if(NxPlayMgr::GetHostPlay())
			NxPlayMgr::GetHostPlay()->PushPlayerAnimEvent(CAnimEventMgr::EAET_STILL);
	}



	if (pkKeyboard->KeyIsDown(NiInputKeyboard::KEY_W)
		|| pkKeyboard->KeyIsDown(NiInputKeyboard::KEY_UP))
	{
		InputManager::keys[NiInputKeyboard::KEY_W] = true;
		CLevelManager::Get()->GetPlayBack()->OnKeyDown(NiInputKeyboard::KEY_W, 
			true, NiApplication::ms_pkApplication->GetCurrentTime());
	}

	if (pkKeyboard->KeyIsDown(NiInputKeyboard::KEY_S)
		|| pkKeyboard->KeyIsDown(NiInputKeyboard::KEY_DOWN))
	{

		InputManager::keys[NiInputKeyboard::KEY_S] = true;
		CLevelManager::Get()->GetPlayBack()->OnKeyDown(NiInputKeyboard::KEY_S, 
			true, NiApplication::ms_pkApplication->GetCurrentTime());	
	}




}

/*
	修改人：	ray
	日期：	1/29/2007
	描叙：	增加反方向键盘事件
*/	
void ZKeyReverseDir::Trigger()
{
	INITKEYEVENTPARAM

	InputManager::keys[NiInputKeyboard::KEY_A] = false;
	InputManager::keys[NiInputKeyboard::KEY_D] = false;
	InputManager::keys[NiInputKeyboard::KEY_W] = false;
	InputManager::keys[NiInputKeyboard::KEY_S] = false;


	if (pkKeyboard->KeyIsDown(NiInputKeyboard::KEY_A)
		|| pkKeyboard->KeyIsDown(NiInputKeyboard::KEY_LEFT))
	{	
		InputManager::keys[NiInputKeyboard::KEY_D] = true;

		if(NxPlayMgr::GetHostPlay())
			NxPlayMgr::GetHostPlay()->PushPlayerAnimEvent(CAnimEventMgr::EAET_TURNRIGHT);
			
		CLevelManager::Get()->GetPlayBack()->OnKeyDown(NiInputKeyboard::KEY_D, 
			true, NiApplication::ms_pkApplication->GetCurrentTime());

	}

	if (pkKeyboard->KeyWasReleased(NiInputKeyboard::KEY_A)
		|| pkKeyboard->KeyWasReleased(NiInputKeyboard::KEY_LEFT))
	{
		if(NxPlayMgr::GetHostPlay())
			NxPlayMgr::GetHostPlay()->PushPlayerAnimEvent(CAnimEventMgr::EAET_STILL);
	}

	if (pkKeyboard->KeyIsDown(NiInputKeyboard::KEY_D)
		|| pkKeyboard->KeyIsDown(NiInputKeyboard::KEY_RIGHT))
	{

		if(NxPlayMgr::GetHostPlay())
			NxPlayMgr::GetHostPlay()->PushPlayerAnimEvent(CAnimEventMgr::EAET_TURNLEFT);

		InputManager::keys[NiInputKeyboard::KEY_A] = true;
		CLevelManager::Get()->GetPlayBack()->OnKeyDown(NiInputKeyboard::KEY_A, 
			true, NiApplication::ms_pkApplication->GetCurrentTime());

	}

	if (pkKeyboard->KeyWasReleased(NiInputKeyboard::KEY_D)
		|| pkKeyboard->KeyWasReleased(NiInputKeyboard::KEY_RIGHT))
	{
		if(NxPlayMgr::GetHostPlay())
			NxPlayMgr::GetHostPlay()->PushPlayerAnimEvent(CAnimEventMgr::EAET_STILL);
	}



	if (pkKeyboard->KeyIsDown(NiInputKeyboard::KEY_W)
		|| pkKeyboard->KeyIsDown(NiInputKeyboard::KEY_UP))
	{
		InputManager::keys[NiInputKeyboard::KEY_W] = true;
		CLevelManager::Get()->GetPlayBack()->OnKeyDown(NiInputKeyboard::KEY_W, 
			true, NiApplication::ms_pkApplication->GetCurrentTime());
	}

	if (pkKeyboard->KeyIsDown(NiInputKeyboard::KEY_S)
		|| pkKeyboard->KeyIsDown(NiInputKeyboard::KEY_DOWN))
	{

		if(NxPlayMgr::GetHostPlay())
			NxPlayMgr::GetHostPlay()->PushPlayerAnimEvent(CAnimEventMgr::EAET_TURNBACK);

		InputManager::keys[NiInputKeyboard::KEY_S] = true;
		CLevelManager::Get()->GetPlayBack()->OnKeyDown(NiInputKeyboard::KEY_S, 
			true, NiApplication::ms_pkApplication->GetCurrentTime());
	}

	if (pkKeyboard->KeyWasReleased(NiInputKeyboard::KEY_S)
		|| pkKeyboard->KeyWasReleased(NiInputKeyboard::KEY_DOWN))
	{
		if(NxPlayMgr::GetHostPlay())
			NxPlayMgr::GetHostPlay()->PushPlayerAnimEvent(CAnimEventMgr::EAET_STILL);
	}

}

void ZKeyNo2::Trigger()
{
	INITKEYEVENTPARAM

	if(pkKeyboard->KeyWasPressed(NiInputKeyboard::KEY_UP)||pkKeyboard->KeyWasPressed(NiInputKeyboard::KEY_W))
	{
		if( CGPControl::Get() )
		{
			GP_ID id;
			id.dwID = 2001;
			id.dwIndex = 0;
			CGPControl::Get()->OnUseGP(id, NxPlayMgr::GetHostPlay()->GetPlayUin(), 
					NxPlayMgr::GetHostPlay()->GetPlayUin());
			SetActive(false);
		}
	}
}
