
#ifndef LENGUINSPCH_H
#define LENGUINSPCH_H

#if defined(NI_USE_PCH)
#pragma warning (disable:4530)

#include "CameraController.h"
#include "Clock.h"
#include "CollisionManager.h"
#include "EffectManager.h"
#include "Footprints.h"
#include "GameStatusManager.h"
#include "HUDTasks.h"
#include "EffectAccumulator.h"
#include "InputManager.h"
#include "Actor.h"
#include "GameMain.h"
#include "LevelManager.h"
#include "NxSceneMgr.h"
#include "SceneLevel.h"
#include "LevelTasks.h"
#include "Play.h"
#include "MommaTasks.h"
#include "ParticleEffect.h"
#include "PhysXUserData.h"
#include "PowerUp.h"
#include "PowerUpManager.h"
#include "PowerUpTasks.h"
#include "Scheduler.h"
#include "ScreenOverlay.h"
#include "ScreenOverlayTask.h"
#include "ShaderHelper.h"
#include "ShadowGeometry.h"
#include "SoundManager.h"
#include "SoundTasks.h"
#include "Task.h"
#include "DescConstant.h"
#include "variant.h"
#include "stl_include.h"
#include "statistics.h"

#ifdef WIN32
#include "SoundSystemSpecificInitialization.h"
#include "Win32SoundManager.h"


#define WIN32_LEAN_AND_MEAN		// Exclude rarely-used stuff from Windows headers
// Windows Header Files:
#include <windows.h>



#define _ATL_APARTMENT_THREADED

#include <atlbase.h>
//You may derive a class from CComModule and use it if you want to override
//something, but do not change the name of _Module
extern CComModule _Module;
#include <atlcom.h>

#endif

#include "NiMenu/MenuManager.h"
#include "NiMenu/NiMenu.h"
#include "NiMenu/NiMenuItem.h"
#include "NiMenu/NiMenuItemBooleanData.h"
#include "NiMenu/NiMenuItemFloatData.h"
#include "NiMenu/NiMenuItemIntData.h"
#include "NiMenu/NiMenuItemScaleData.h"
#include "NiMenu/NiScreenText.h"
#include "Utility.h"
#endif //#if defined(NI_USE_PCH)

#endif // #ifndef LENGUINSPCH_H