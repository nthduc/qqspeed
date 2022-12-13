
#include "Top-KartPCH.h"

#include "GameMain.h"
#include "InputManager.h"
#include "MommaTasks.h"

#include "LM_Anim.h"
#include "left_Anim.h"
#include "EventContainer.h"

#include "Scheduler.h"
#include "GameStatusManager.h"
#include "EffectManager.h"
#include "./PhysKart/NxAllVehicles.h"
#include "Track/KartPlayback.h"
#include "Track/KartStatusManager.h"
#include "Track/GlobalMap.h"

#include "NxSceneMgr.h"
#include "SceneLevel.h"

#ifdef WIN32
#include <NiDI8InputSystem.h>
#endif
InputManager* InputManager::ms_pkTheInputManager = 0;
const float InputManager::ms_fInputDelay = 0.25f;
const float InputManager::ms_fInputDelayThreshold = 0.1f;
float InputManager::ms_fLastInputTime = 0.0f;
bool InputManager::keys[] = {false};
//---------------------------------------------------------------------------
bool InputManager::Create()
{
    if (!ms_pkTheInputManager)
    {
        ms_pkTheInputManager = NiNew InputManager;

        if (!ms_pkTheInputManager || !ms_pkTheInputManager->Initialize())
            return false;
    }

    return true;
}
//---------------------------------------------------------------------------
void InputManager::Destroy()
{
    NiDelete ms_pkTheInputManager;

    ms_pkTheInputManager = 0;
}
//---------------------------------------------------------------------------
InputManager::InputManager()
{
	m_pEventMgr = NULL;
	m_bEnable = true;
 
}
//---------------------------------------------------------------------------
InputManager::~InputManager()
{
	m_spInputSystem = 0;
	if(m_pEventMgr)
		NiDelete m_pEventMgr;
}
//---------------------------------------------------------------------------
InputManager::InputData::InputData() :
    m_kRelativeDir(0.0f, 0.0f), 
    m_kA1Dir(0.0f, 0.0f),
    m_kA2Dir(0.0f, 0.0f),
    m_fZoomVel(1.0f),
    m_fStrafeDir(0.0f)
{
    /* */
}
//---------------------------------------------------------------------------
void InputManager::GameUpdate(const NiMatrix3& kRotation)
{

    if (!m_spInputSystem)
    {
        return;
    }

    if (m_spInputSystem->UpdateAllDevices() == NIIERR_DEVICELOST)
        m_spInputSystem->HandleDeviceChanges();

    float fXAngle, fYAngle, fZAngle;
    kRotation.ToEulerAnglesYXZ(fYAngle, fXAngle, fZAngle);
    if (fYAngle < 0.0f)
        fYAngle += NI_TWO_PI;
    float fCos = NiCos(fYAngle);
    float fSin = NiSin(fYAngle);

    PollGameInput(fSin, fCos);

	NiInputKeyboard* pkKeyboard = m_spInputSystem->GetKeyboard();

	if (pkKeyboard->KeyIsDown(NiInputKeyboard::KEY_G))
	{
		if (CLevelManager::Get() && CLevelManager::Get()->GetPhysXManager())
		{
			bool bDRender = CLevelManager::Get()->GetPhysXDebugRender();
			CLevelManager::Get()->SetPhysXDebugRender(!bDRender);
		}

		keys[NiInputKeyboard::KEY_G] = true;
	}
	else
		keys[NiInputKeyboard::KEY_G] = false;


	if (pkKeyboard->KeyIsDown(NiInputKeyboard::KEY_SPACE))
	{
		keys[NiInputKeyboard::KEY_SPACE] = true;
	}
	else
		keys[NiInputKeyboard::KEY_SPACE] = false;

}
//---------------------------------------------------------------------------
void InputManager::MenuUpdate()
{
    if (!m_spInputSystem)
    {
        return;
    }

    if (m_spInputSystem->UpdateAllDevices() == NIIERR_DEVICELOST)
        m_spInputSystem->HandleDeviceChanges();


    PollMenuInput();
}
//---------------------------------------------------------------------------
bool InputManager::Initialize()
{
    // Create and initialize parameters for the input system
#ifdef WIN32
    NiDI8InputSystem::DI8CreateParams kParams;
    kParams.SetRenderer(NiRenderer::GetRenderer());
    kParams.SetKeyboardUsage(
        NiInputSystem::FOREGROUND | NiInputSystem::NONEXCLUSIVE);

    unsigned int uiMouseFlags = NiInputSystem::FOREGROUND;
//#ifdef _DEBUG
    uiMouseFlags |= NiInputSystem::NONEXCLUSIVE;
//#else
//    uiMouseFlags |= NiInputSystem::EXCLUSIVE;
//#endif

    kParams.SetMouseUsage(uiMouseFlags);

    kParams.SetGamePadCount(2);
    kParams.SetAxisRange(-100, +100);
    kParams.SetOwnerInstance(
        NiApplication::ms_pkApplication->GetInstanceReference());
    kParams.SetOwnerWindow(
        NiApplication::ms_pkApplication->GetWindowReference());

    // Create the input system
    m_spInputSystem = NiInputSystem::Create(&kParams);
#endif

    if (!m_spInputSystem)
    {
        NiMessageBox("CreateInputSystem: Creation failed.", 
            "NiApplication Error");
        return false;
    }

    // The creation of the input system automatically starts an enumeration
    // of the devices. 
    NiInputErr eErr = m_spInputSystem->CheckEnumerationStatus();
    switch (eErr)
    {
    case NIIERR_ENUM_NOTRUNNING:
        assert(!"EnumerateDevices failed?");
        return false;
    case NIIERR_ENUM_FAILED:
        assert(!"CheckEnumerationStatus> FAILED!");
        return false;
    case NIIERR_ENUM_COMPLETE:
    case NIIERR_ENUM_NOTCOMPLETE:
    default:
        break;
    }

    // On Win32, assume there is a mouse and keyboard
#ifdef WIN32
    if (!m_spInputSystem->OpenMouse() || !m_spInputSystem->OpenKeyboard())
    {
        NiMessageBox("Mouse or keyboard failed to open.", 
            "Error");
        return false;
    }
#endif

    // Gamepad may not exist, but attempt to open first one
    m_spInputSystem->OpenGamePad(0, 0);

    // Create conversion factors for fast and accurate range conversion
    m_fAxisLowRangeOffset = NiAbs((float)m_spInputSystem->GetAxisRangeLow());
    m_fAxisTotalRange = (float)m_spInputSystem->GetAxisRangeHigh();
    m_fAxisTotalRange += m_fAxisLowRangeOffset;


	m_pEventMgr = ZKeyEventManager::Create();

    return true;
}
//---------------------------------------------------------------------------
void InputManager::PollMenuInput()
{
}

int g_dz = 0;

//---------------------------------------------------------------------------
void InputManager::PollGameInput(float fSin, float fCos)
{
    NiPoint2 kInput = NiPoint2::ZERO;
    float fStrafeDir = 0;
    bool bUsingKeyboardInput = false;
    

    NiInputKeyboard* pkKeyboard = m_spInputSystem->GetKeyboard();
    NiInputMouse* pkMouse = m_spInputSystem->GetMouse();
    NiInputGamePad* pkGamePad = InputManager::Get()->
        GetInputSystem()->GetGamePad(0,0);
   

    int iX, iY, iZ;
    iX = iY = iZ = 0;

#ifdef WIN32
    // We require the mouse and keyboard for any input parsing
    if (!pkMouse && !pkKeyboard)
        return;

	m_pEventMgr->Update();

//	m_pEventMgr->CheckCommonEvents();
//	m_pEventMgr->CheckSpecialEvents();

#endif
  /*

    // Adjust Camera
    if (!pkMouse->GetPositionDelta(iX, iY, iZ))
    {
        iX = iY = iZ = 0;
    }

    float fMouseScale = 15.0f;
    m_kInputData.m_kA2Dir.x = fMouseScale * (float)(iX) / 
        (float)NiApplication::ms_pkApplication->
        GetAppWindow()->GetWidth();
    m_kInputData.m_kA2Dir.y = fMouseScale * (float)(iY) / 
        (float)NiApplication::ms_pkApplication->
        GetAppWindow()->GetHeight();
    
	g_dz = iZ;

	if(pkMouse->ButtonIsDown(NiInputMouse::Button::NIM_LEFT))
	{
		LevelManager::GetPlay()->SetRotateInc(NiPoint3(0.0f,0.0f,1.0f),-m_kInputData.m_kA2Dir.x);
	}
    
#endif

    if (iZ == 0)
        m_kInputData.m_fZoomVel = 1.0f;
    else if (iZ > 0)
        m_kInputData.m_fZoomVel = 0.75f;
    else
        m_kInputData.m_fZoomVel = 1.25f;
#ifdef WIN32
    if (pkKeyboard->KeyIsDown(NiInputKeyboard::KEY_PERIOD))
    {
        m_kInputData.m_fZoomVel = 0.95f;
        bUsingKeyboardInput = true;
    }
    else if (pkKeyboard->KeyIsDown(NiInputKeyboard::KEY_COMMA))
    {
        m_kInputData.m_fZoomVel = 1.0505050505f;
        bUsingKeyboardInput = true;
    }
#endif

    if(pkGamePad && !bUsingKeyboardInput)
    {
        int iHorzAxisLeft;
        int iVertAxisLeft;
        int iHorzAxisRight;
        int iVertAxisRight;
        int iZAxis;
        NiPoint2 kLeftStick;

        pkGamePad->GetStickValue(NiInputGamePad::NIGP_STICK_LEFT,
            iHorzAxisLeft, iVertAxisLeft);
        pkGamePad->GetStickValue(NiInputGamePad::NIGP_STICK_RIGHT,
            iHorzAxisRight, iVertAxisRight);
        int iZAxisPC = 
            -pkGamePad->GetAxisValue(NiInputGamePad::NIGP_DEFAULT_X1_VERT);

        kLeftStick.x = -AdjustToAxisRange((float)iHorzAxisLeft);
        kLeftStick.y = -AdjustToAxisRange((float)iVertAxisLeft);

        // Introduce small delay to help protect the player from accidentally
        // causing the Momma to come to a sudden stop
        if (kLeftStick.SqrLength() < ms_fInputDelayThreshold)
        {
            if (NiGetCurrentTimeInSec() - ms_fLastInputTime > ms_fInputDelay)
            {
                kInput.x = 0;
                kInput.y = 0;
            }
            else
            {
                kInput = m_kInputData.m_kA1Dir;
            }
        }
        else
        {
            kInput.x = -AdjustToAxisRange((float)iHorzAxisLeft);
            kInput.y = -AdjustToAxisRange((float)iVertAxisLeft);
            ms_fLastInputTime = NiGetCurrentTimeInSec();
        }
        
        fStrafeDir = 0;
        
        if (iX == 0 && iY == 0 && iZ == 0)
        {
            m_kInputData.m_kA2Dir.x = AdjustToAxisRange((float)iHorzAxisRight);
            m_kInputData.m_kA2Dir.y = AdjustToAxisRange((float)iVertAxisRight);
        }

        unsigned char ucLeft, ucRight;
        ucLeft = pkGamePad->ButtonState(NiInputGamePad::NIGP_L2);
        ucRight = pkGamePad->ButtonState(NiInputGamePad::NIGP_R2);
        int iZAxisXenon = (ucRight / 2) - (ucLeft / 2);
        
        if (abs(iZAxisPC) > abs(iZAxisXenon))
            iZAxis = iZAxisPC;
        else
            iZAxis = iZAxisXenon;

        if (iZAxis < 0)
            fStrafeDir = 1.0f;
        else if (iZAxis > 0)
            fStrafeDir = -1.0f;

        if (pkGamePad->ButtonIsDown(NiInputGamePad::NIGP_L1))
        {
            m_kInputData.m_fZoomVel = 0.95f;
        }
        else if (pkGamePad->ButtonIsDown(NiInputGamePad::NIGP_R1))
        {
            m_kInputData.m_fZoomVel = 1.0505050505f;
        }
        
        if (pkGamePad->ButtonWasPressed(NiInputGamePad::NIGP_B))
        {
            ((GameMain*)(NiApplication::ms_pkApplication))->
                ToggleShowAllTrackers();
        }

        if (pkGamePad->ButtonWasPressed(NiInputGamePad::NIGP_RRIGHT) &&
            !LevelManager::Get()->GetPlay()->IsExcutingAction())
        {
            if (LevelManager::GetGameStatusManager()->GetBulletTime())
            {
                LevelManager::GetGameStatusManager()->
                    SetBulletTime(false, 1.0f);
            }
            else
            {
                LevelManager::GetGameStatusManager()->
                    SetBulletTime(true, 0.25);
                LevelManager::Get()->SetFirstBlurFrame(true);
                LevelManager::Get()->IncBulletTimeCount();
            }
        }

        if (pkGamePad->ButtonWasPressed(NiInputGamePad::NIGP_START) )
        {
            LevelManager::Get()->GetGameStatusManager()->SetGameState(
                GameStatusManager::IN_GAME_MENU);
        }
    }

    if (fStrafeDir != 0 && 
        !LevelManager::Get()->GetPlay()->IsExcutingAction())
    {
        // Momma is strafing, force her to face forward
        kInput.y = 1.0f;
        kInput.x = 0.0f;            
    }


    if (NiAbs(kInput.x) > 0.95f || NiAbs(kInput.y) > 0.95f)
    {
        kInput.Unitize();
    }

    m_kInputData.m_kA1Dir = kInput;
    m_kInputData.m_fStrafeDir = fStrafeDir;

    m_kInputData.m_kRelativeDir.x = 
        fSin * kInput.x + fCos * kInput.y;
    m_kInputData.m_kRelativeDir.y = 
        fCos * kInput.x - fSin * kInput.y;
		*/
}
//---------------------------------------------------------------------------
float InputManager::AdjustToAxisRange(float fAxisValue)
{
    // This method will take the given axis value and based on the axis range
    // of the input system will convert the range to a float between -1 and 1
    fAxisValue += m_fAxisLowRangeOffset;
    fAxisValue /= m_fAxisTotalRange;
    fAxisValue *= 2.0f;
    fAxisValue -= 1.0f;
    return fAxisValue;
}
//---------------------------------------------------------------------------
