#pragma once
#include <set>
class CSceneLevel;

#define SCENELEVEL_RACEGAME_MAP0    0
#define SCENELEVEL_RACEGAME_MAP1    1
#define SCENELEVEL_RACEGAME_MAP2    2

#define SCENELEVEL_LOGIN			10
#define SCENELEVEL_CARSHOP			11
#define SCENELEVEL_CARSTORAGE		0  // 原来的值为12，为了保持一致，修改为0，by wincoqin, 2007/2/13

#define SCENELEVEL_ITEMGAME_MAP0    100
#define SCENELEVEL_ITEMGAME_MAP1	101
#define SCENELEVEL_ITEMGAME_MAP2	102

#define SCENELEVEL_NONE				199
#define MAX_SCENELEVEL_COUNT		200

//-----------------------------------------------------------------------
/*
enum enmGamemodeBaseIndex
{
	EGBI_BASEMODE_RANDOM = 0,
	EGBI_BASEMODE_RACE = 1,
	EGBI_BASEMODE_ITEM = 2,
	EGBI_BASEMODE_MAX
};


enum enmGamemodeSubIndex
{
	EGSI_SUBMODE_RANDOM = 0,
	EGSI_SUBMODE_SINGLE = 1,
	EGSI_SUBMODE_TEAM = 2,
	EGSI_SUBMODE_MAX
};
*/
//-----------------------------------------------------------------------

#include <NiApplication.h>
#include <NiPhysX.h>
#include "ScreenOverlay.h"
#include "CameraController.h"
#include "EffectAccumulator.h"
#include "GameEffectMgr.h"
#include "KartCrack.h"
#include "CollisionManager.h"
#include "ScriptVM.h"
#define TP_USE_CONSLE


class GameStatusManager;
class Play;
class PowerUpManager;
class EffectManager;
class ProfilingController;
class CLinks;
class CKartPlayBack;
class CPowerController;
class CNO2Controller;
class CAcceleratePanel;
class CPlayerRankMgr;
class CCameraController;

class NiCamera;
class NiGeometryGroup;
class NiLines;
class NiGeomMorpherController;
struct lua_State;

class TerrainManager;
class CFenceManager;
class PhysXManager;
class CConsServer;

NiSmartPointer(NiAlphaAccumulator);
NiSmartPointer(NiAmbientLight);

//-----------------------------------------------------------------------

class NxSceneMgr: public NiMemObject
{

public:
	static NxSceneMgr * Get(){ return ms_pkSceneMgr;}
	static bool Create();
	static void Destory();

public:
	GameStatusManager		*		GetGameStatusManager()		{ return m_pkGameStatusManager;	}
	EffectManager			*		GetEffectManager()			{ return m_pkEffectManager;		}
	CKartPlayBack			*		GetPlayBack()				{ return m_pkPlayBack;			}
	ProfilingController		*		GetProfilingController()	{ return m_pkProfilingController;}

	PowerUpManager			*		GetPowerUpManager()			{ return m_pkPowerUpManager;	}
	CCameraController		*		GetCameraController()		{ return m_pkCameraController;	}
	/*
		修改人：	ray
		日期：	1/30/2007
		描叙：	提供比赛模式接口
	*/
	short							GetCurGameModeBase();		//得到比赛模式，竞速赛或道具赛
	short							GetCurGameModeSub();		//团体赛或个人赛


	//Script & Console
	ScriptVM* GetScriptVM()							{ return m_pScriptVM; }

	static void OnRecvConsleCommand(char* buffer,int len);
	static lua_State * GetLuaState()	{ return ms_pkSceneMgr->m_pScriptVM->GetLuaState();}

protected:
	NxSceneMgr();
	~NxSceneMgr();
	

private:
	static std::set< CSceneLevel * > m_AllScene;
	static UINT			  ms_nActiveSceneID;
	static NxSceneMgr	* ms_pkSceneMgr; 
//------------------------------------------------------------------------------
	/*
	修改人：	ray
	日期：	1/30/2007
	描叙：	提供比赛模式接口
	*/
//	short	m_sGameModeBase;			//竞速赛或道具赛
//	short	m_sGameModeSub;				//个人赛或团体赛
//------------------------------------------------------------------------------


private:
	EffectManager		* m_pkEffectManager;
	CKartPlayBack		* m_pkPlayBack;



private:
	CCameraController	*	m_pkCameraController;	
	GameStatusManager	* 	m_pkGameStatusManager;
	ScriptVM			*	m_pScriptVM;
    PowerUpManager		*	m_pkPowerUpManager;
	ProfilingController	*	m_pkProfilingController;	
	
	

public:
#ifdef TP_USE_CONSLE
	CConsServer			*	m_spServer;
#endif 

// bug 临时代码
public:
	void	SetGameSvrID( ULONG ID) {	m_unGameSvrID = ID ;}
	ULONG	GetGameSvrID() { return m_unGameSvrID; }
	ULONG   m_unGameSvrID;	

};
