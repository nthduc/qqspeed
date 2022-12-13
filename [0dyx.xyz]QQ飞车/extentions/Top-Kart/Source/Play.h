

#ifndef MOMMAMANAGER_H
#define MOMMAMANAGER_H

#include "Actor.h"
#include "./PhysKart/kart.h"
#include "playerdata.h"
#include "NiActorManager.h"
#include "gptarget.h"
#include "AminEventMgr.h"
#include "PlayerRank.h"
#include "./Avatar/ATWearer.h"
#include <map>
#include <string>
#include "NiCriticalSection.h"

using namespace std;
#define SCALE_FACTOR  1000.0f

class MommaUpdateAnimationsTask;

class XCart;
#define MAX_CHARACTER_NUM		3
#define MAX_KART_NUM			3

//--------------------------------------------------------------------------------------------------
typedef std::map<string, int>				StringToIntMap;				
typedef std::map<int, string>				IntToStringMap;
typedef std::map<int, NiSourceTexturePtr>	IntToSpTexMap;

typedef std::multimap<int, string>			IntToStringMulMap;



typedef struct tagPlayerDesc					//play的信息。包括角色和车
{	
	unsigned int unCharacterID;
	NiFixedString strCharacterName;				//角色名字	
	NiFixedString strChacaterPath;				//角色模型的路径
	StringToIntMap mapCharacterAnim;			//角色动作图
	IntToSpTexMap mapCharacterImage;			//角色表情图	

	unsigned int unKartID;
	NiFixedString strKartName;					//车的名字
	NiFixedString strKartPath;					//车模型的路径
	StringToIntMap mapKartAnim;					//车动作图	
	IntToSpTexMap mapKartImage;					//车表情图
	IntToStringMulMap	mapKartEffect;			//车的特效信息

	unsigned int unHairID;						// 头发ID
	NiFixedString strHairName;					// 头发名字
	NiFixedString strHairPath;					// 头发模型路径
	StringToIntMap mapHairAnim;					// 头发动作


} PLAYERDESC;
//-----------------------------------------------------------------------------------------------------
class Play : public Actor, public CGPTargetBase, public CATWearer
{

public:
	enum ePlayCameraType
	{
		Play_GameStart_Camera,
		Play_GameOver_Camera
	};

	enum ePlayStates
	{
		In_None_State,
		In_Room_State,
		In_GamePlaying_State,
		In_GameAward_State
	};

	Play();
	virtual ~Play();
	void Release();
	virtual bool Initialize( KPlayInfo & Info, unsigned int uiMommaID = 0,
		unsigned int uiPadID = 0);
	virtual  void Update(float fTime, float fFrameTime, const NiPoint2& kDir, 
		const float& fStrafeDir, bool bAnimationUpdate);

	void SetPos(const NiPoint3& kPos);
	void SetRotate(const NiPoint3& kVer,const float& fAngle);
	void SetRotateInc(const NiPoint3& kVer,const float& fAngle);
	NiPoint3 GetPos(){return GetKartNIFRoot()->GetTranslate();};
	NiTransform GetTransform(){return GetKartNIFRoot()->GetWorldTransform();}
	NiNode* GetNode() { return GetKartNIFRoot(); }

	void InitPlayCamera();
	void UseWinCamera();
	void UseHeadCamra();
	void UseStartCamera();
	
	//bool CanUdpPass();

	//-----------------------------------------------------------------------
	
	void EnterPhyScene(const NiTransform& kTransform);
	virtual void UpdatePhy(float fTime);
	void StopPhy();
	void StartPhy();
	void LeavePhyScene();
	
	virtual void SetKartParam(void * pData , float fSwooshFactor = 1.0f );
	virtual void GetKartParam(void * pData);

	virtual bool IsFinish(){ return m_bIsFinish ;}
	virtual void SetFinish(bool bIsFinsh);// 标准化变量名
	virtual void SetWin(bool bWin);
	virtual bool IsWin() { return m_bIsWin;};

	float GetRestMileseage(){return m_fRestMileage;}

	void SendMovePkg();

	void SitPos(NiTransform  Trans);

	void SetPlayStatus( Play::ePlayStates eStatue) { m_nPlayState = eStatue;}
	int  GetPlayStatus() { return m_nPlayState ;}
	char *  GetPlayName() { return m_cQQName;}

	void ChangeTeam(DWORD dwTeamID = (DWORD)-1);
	SColorAttr* FindPaintColor();
//-----------------------------------------------------------------------－－－－－－－－－－－－－－－－－－－－－
	void SetCharaterAnimation(UINT seqID);					//设置角色的动作（不经过动作优先级处理）
	void SetCharaterAnimation(NiFixedString strAnimName);

	void SetKartAnimation(UINT seqID);						//设置车的动作（不经过动作优先级处理）
	void SetKartAnimation(NiFixedString strAnimName);

	UINT GetCharacterAnim();			//返回角色动作id
	UINT GetKartAnim();					//返回车动作id

	//push动作事件，相应的优先级等逻辑在CAnimEventMgr处理
	void PushPlayerAnimEvent(CAnimEventMgr::ENMANIMEVENTTYPE unEvent);	
	CAnimEventMgr* GetAnimEventMgr();		//获得CAnimEventMgr

	void ChangeAnimFaceImage(int ID);		//根据动作来更换表情动画

	void LoadCharacterDesc(unsigned int unID);		//载入人物信息
	void LoadKartDesc(unsigned int unID);			//载入车信息
	bool SetActiveCharacter(unsigned int unID);		//设置活跃人物
	bool SetActiveKart(unsigned int unID);			//设置活跃换车
	bool ChangeItem(unsigned int unID);				//更换Item
	//	增加人:	wincoqin
	//  时间:	2007/05/10
	//  目的:	增加换头发的接口  	
	bool ChangeHair(unsigned int unID);				// 换头发
	bool BindHairToHead();							// 把头发绑定到头上
	bool LoadHairDesc(unsigned int unID);			// 加载头发信息

//---------------------------------------------------------------------------------------------------------------
	//继承自CATWearer
	//处理特殊的avatar拥有动作
//	DWORD AnimTranform(DWORD dwID);

//-------------------------------------------------------------------------------
	//-----------------------------------------------------------------------

	//IGPTarget
	virtual DWORD		GetID();
	virtual DWORD		GetIndex();
	virtual DWORD		GetTeam();
	virtual NiPoint3	GetForward();
	//virtual NiPoint3	GetPos();  //已存在
	//virtual void		SetPos(const NiPoint3&);//已存在
	//virtual NiTransform GetTransform();//已存在
	//virtual NiNode*		GetNode();//已存在
	virtual NiPoint3	GetPhysXPos();
	virtual void		SetPhysXPos(const NiPoint3&);
	virtual NiPoint3	GetVelocity(BOOL bLocal);
	virtual void		ZeroVelocity();
	virtual BOOL		Attach(NiAVObject* pNode);
	virtual BOOL		Detach(NiAVObject* pNode);
	virtual BOOL		PlayAnimation(DWORD AnimID, BOOL bPlay);
	virtual BOOL		StopAnimation(DWORD AnimID, BOOL bPlay);
	virtual void		AddForce(float force[3], BOOL bLocal);
	virtual void		ControlSpeed(float fRatio);
	virtual BOOL		ToggleSim(BOOL);

protected:
	UINT m_unMovePackageIndex;
	bool m_bIsFinish;
	bool m_bIsWin;
	bool m_bIsStopPhy;
	float m_fSpeedDescentRatio;
	int   m_nPlayState;
	unsigned int m_uiActiveKartID;
	char m_cQQName[MAXNICKNAMELEN];
	CAnimEventMgr* m_pAnimEventMgr;

public:
	PLAYERDESC & GetCurPlayDesc() { return m_stCurPlayerDesc;}

public:
	PLAYERDESC m_stCurPlayerDesc;				//当前play的信息

public:
	bool IsExcutingAction();
	void Restart();
	NiPoint3 GetSpawnPoint();

	void SetAutoModeTime(float fTime);
	float GetAutoModeTime() const;

#ifndef _XCART_
	CKart * GetKart();
	void SetKart(CKart* pKart);
#else
	XCart* GetKart();
	void SetKart(XCart* pKart);
#endif

	virtual void AnimActivated(NiActorManager* pkManager,
		NiActorManager::SequenceID eSequenceID, float fCurrentTime,
		float fEventTime);

	virtual void EndOfSequence(NiActorManager *pkManager, 
		NiActorManager::SequenceID eSequenceID, float fCurrentTime, 
		float fEventTime);

	virtual void AnimDeactivated(NiActorManager* pkManager,
		NiActorManager::SequenceID eSequenceID, float fCurrentTime, 
		float fEventTime) ;

	virtual bool StartAnimation(NiActorManager::EventCode eCode);


protected:
	virtual void HandleTextKeyEvent();
	virtual NiActorManager::EventCode InterpretInput(const NiPoint2& kDir,
		const float& fStrafeDir, float fTime);
	virtual void InitializeRandomIdle();
	const NiPoint2 AutoDir(float fTime);
	void ComputeAutoGoal();


	NiPoint3 m_kPos;

	bool m_bRumble;

	bool m_bExcutingAction;

	NiActorManager::SequenceID m_ePostActionAnimation;

	NiGeometryPtr   spLeftFootEmitter;
	NiGeometryPtr   spRightFootEmitter;
	NiGeometryPtr   spFleeEmitter;

	// auto mode
	float m_fLastInputTime;
	float m_fAutoModeTime;
	NiPoint2 m_kAutoGoal;
	float m_fPauseTime;
	float m_fPausedTime;
	bool m_bAutoWalk;
	float m_fMaxAutoGoalSeekTime;
	bool m_bIsSpeedUp;
	float m_fRestMileage;
	bool m_bIsTurnBack;
	Swoosh* m_pkSwooshL;
	Swoosh* m_pkSwooshR;

#ifndef _XCART_
	CKart* m_pKart;
#else
	XCart* m_pKart;
#endif

	KPlayInfo  m_stPlayInfo;

public:
	void SetSpeedUpFlag( bool bFlag ){ m_bIsSpeedUp = bFlag;}
public:
	USHORT			GetPlayID()				{ return m_stPlayInfo.m_nPlayerID;}
	USHORT			GetKartID()				{ return m_stPlayInfo.m_nKartID;}
	bool			IsNewCar ();

	ULONG			GetPlayUin()			{ return m_stPlayInfo.m_unUin;}
	KPlayInfo &		GetPlayInfo()			{ return m_stPlayInfo;}



public:
	virtual bool IsHost(){ return true;}
public:
	void CollisionDetectionFromOtherPlayer(DWORD unID, CTencentCar * car, const NxVec3& velocity, const NxVec3& pos, const NxMat33& mat);

public:
	const NiPoint3* GetWheelsPos(){return m_arWheelPos;}
protected:
	NiPoint3 m_arWheelPos[4];
	virtual void InitWheelPos();
};

class RemotePlay : public Play
{
public:
	RemotePlay();
	~RemotePlay();
public:
	virtual  bool  Initialize(KPlayInfo & Info, unsigned int uiMommaID = 0, unsigned int uiPadID = 0 ) ;
	virtual  void Update(float fTime, float fFrameTime, const NiPoint2& kDir, 
		const float& fStrafeDir, bool bAnimationUpdate);

	virtual bool IsHost(){ 	return false; }
	virtual void SetKartParam(void * pData, float fSwooshFactor = 1.0f);
	virtual void GetKartParam(void * pData);
	virtual void UpdatePhy( float fTime);

	virtual NiActorManager::EventCode InterpretInput(const NiPoint2& kDir,
		const float& fStrafeDir, float fTime);

	virtual void AnimActivated(NiActorManager* pkManager,
		NiActorManager::SequenceID eSequenceID, float fCurrentTime,
		float fEventTime);

	virtual void EndOfSequence(NiActorManager *pkManager, 
		NiActorManager::SequenceID eSequenceID, float fCurrentTime, 
		float fEventTime);

	virtual void AnimDeactivated(NiActorManager* pkManager,
		NiActorManager::SequenceID eSequenceID, float fCurrentTime, 
		float fEventTime) {}

	virtual bool StartAnimation(NiActorManager::EventCode eCode){ return false;}
	virtual void InitWheelPos();

};

#include "Play.inl"    

#endif // #ifndef MOMMAMANAGER_H
