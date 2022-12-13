#pragma once
//-----------------------------------------------------------------------
// create by hoverzhao 
//-----------------------------------------------------------------------
#include "levelmanager.h"

#if defined(___xxx)

#include <NiApplication.h>
#include <NiPhysX.h>
#include "ScreenOverlay.h"
#include "CameraController.h"
#include "EffectAccumulator.h"
#include "GameEffectMgr.h"
#include "KartCrack.h"
#include "CollisionManager.h"
#include <NiCullingProcess.h>
#include "NxSceneMgr.h"
//#include "PhysXManager.h"
#include "RMOperator.h"


class GameStatusManager;
class Play;
class PowerUpManager;
class SoundManager;
class EffectManager;
class ProfilingController;
class CLinks;
class CKartPlayBack;

class NiCamera;
class NiGeometryGroup;
class NiLines;
class NiGeomMorpherController;
class ScriptVM;
struct lua_State;

class TerrainManager;
class CFenceManager;
class PhysXManager;

NiSmartPointer(NiAlphaAccumulator);
NiSmartPointer(NiAmbientLight);

class CSceneHelper;
//-----------------------------------------------------------------------
class NiAVObject; 
class RMOperator;

extern const  float BF_CR;
extern const  float BF_CG;
extern const  float BF_CB;

class CSceneLevel : 
	//public NiMemObject,
	public CollisionManager,
	public RMOperator
{
public:
	enum eCameraType
	{
		CameraInvalid = 0,
		GameCamera  ,
		OverCamera  , 
	};
	enum WhichLevel
	{
		LEVEL_GAME,
		LEVEL_CARBARN,
		LEVEL_NONE
	};
//场景处理函数
public:
	virtual bool LoadLevel(UINT SceneID );
	virtual void UnloadLevel();
	void AttachObjectToScene(NiNodePtr pNode);
	///取消绑定在场景中的NiNode模型
	void DetachObjectToScene(NiNodePtr pNode);
	void SetRaceOver();
	STDMETHOD(WndProc)(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

public:
	virtual void Draw(){};
	virtual void PreDraw();
	virtual void EndDraw();
	virtual void Update(float fTime);

public:
//临时的
	void Draw_LGAME();
	void Draw_LSCAR();

//获得场景信息函数
public:
	NiAVObject	* GetScene() { return m_spScene; } 
	UINT		  GetSceneID(){ return m_nSceneID; } 
	CLinks		* GetLinks()	{ return m_pkLinks;}


//渲染处理函数
public:
	virtual void PartialRender(NiCamera* pkCamera, NiAVObject* pkObject);
	void	ImmRender(NiCamera* pkCamera,NiAVObject* pkObject);

public:
	float	GetLevelTime()	{   return m_fElapseTime;  }
	float	GetStartTime()	{   return m_fLevelStartTime;	}
	virtual void	Restart() {};


public:
	CKartPlayBack * GetPlayBack()	{ return m_pkPlayBack; }
//相机使用
public:
	void					UseCamera(eCameraType CameraType);		

	NiCamera		*		GetCamera(){ return NxSceneMgr::Get()->GetCameraController()->GetCamera() ;	}

//碰撞处理函数
public:
	void	AddCollider(NiAVObject* pkObject); 
	const	NiTPointerList<NiAVObject*> * GetColliders();
	void	RemoveAllColliders();

protected:	
	PhysXManager	 *	m_pPhysXManager;
	CKartPlayBack	 *  m_pkPlayBack;

public:
	//PhysX
	PhysXManager	*	GetPhysXManager();
	bool				GetPhysXDebugRender();
	void				SetPhysXDebugRender(const bool bRender) ;

//临时的
public:
	//Play* GetPlay()  { return m_pkPlay ;}
	

public :
	CSceneLevel(SHORT nLap);
	virtual ~CSceneLevel();


protected:
	UINT				m_nSceneID;
	SHORT				m_nLap;
	CLinks			 *  m_pkLinks;

	CSceneHelper     *  m_pSceneHelper;

// 临时的
public:
	CKartCrack			m_sKartCrack;





//临时的
protected:
	//WhichLevel m_eLevelLoaded;
	Play* m_pkPlay;   

protected:
	BOOL		m_bRaceOver;
	float		m_fElapseTime;
	float		m_fPrevFrameTime;
	// Scene data objects
	NiNodePtr	m_spScene;
	NiNodePtr	m_spStaticRoot;
	NiNodePtr	m_spLevelRoot;
	//NiNodePtr	m_spCameraNif;  // introduce camera
	NiNodePtr	m_spMoveNode;
	NiNodePtr   m_spSkyNode;
	float		m_fAppStartTime;
	NiCameraPtr m_spGameCamera;
	float		m_fLevelStartTime;

	NiAlphaAccumulatorPtr	m_spEffectSorter;
	NiCullingProcess		m_kCuller;
	NiVisibleArray			m_kVisible;

	NiRenderer			*	m_tpRenderer;
	NiCamera			*	m_tpCamera;

#if defined(WIN32)

#if defined (_DX8)
	NiDX8Renderer*    m_pkDXRenderer;
	LPDIRECT3DDEVICE8 m_pkDevice;
#elif defined (_DX9)
	NiDX9Renderer*        m_pkDXRenderer;
	LPDIRECT3DDEVICE9     m_pkDevice;
#endif
#endif //#if defined(WIN32)

};

class CRaceGameSceneLevel : public CSceneLevel
{
public:
	CRaceGameSceneLevel(SHORT nLap);
	
	virtual ~CRaceGameSceneLevel();
	
	virtual bool LoadLevel(UINT SceneID );
	virtual void UnloadLevel();

	virtual void Restart();

public:
	virtual void Draw();

protected:
	int m_nPlayerPosUpdateTaskID; // Add by wincoqin, 2007/1/22
};

class CItemGameSceneLevel : public CSceneLevel
{
public: 
	CItemGameSceneLevel(SHORT nLap):CSceneLevel(nLap){
	}
	virtual ~CItemGameSceneLevel(){
	}
	virtual bool LoadLevel(UINT SceneID ){ return true;}
	
};


class CKartStorageSceneLevel : public CSceneLevel
{
public:
	CKartStorageSceneLevel(SHORT nLap):CSceneLevel(nLap){
	}
	virtual ~CKartStorageSceneLevel(){
	}

	 virtual bool LoadLevel(UINT SceneID );
	 virtual void UnloadLevel();
	 void Draw();

};






#endif