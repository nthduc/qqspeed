

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



typedef struct tagPlayerDesc					//play����Ϣ��������ɫ�ͳ�
{	
	unsigned int unCharacterID;
	NiFixedString strCharacterName;				//��ɫ����	
	NiFixedString strChacaterPath;				//��ɫģ�͵�·��
	StringToIntMap mapCharacterAnim;			//��ɫ����ͼ
	IntToSpTexMap mapCharacterImage;			//��ɫ����ͼ	

	unsigned int unKartID;
	NiFixedString strKartName;					//��������
	NiFixedString strKartPath;					//��ģ�͵�·��
	StringToIntMap mapKartAnim;					//������ͼ	
	IntToSpTexMap mapKartImage;					//������ͼ
	IntToStringMulMap	mapKartEffect;			//������Ч��Ϣ

	unsigned int unHairID;						// ͷ��ID
	NiFixedString strHairName;					// ͷ������
	NiFixedString strHairPath;					// ͷ��ģ��·��
	StringToIntMap mapHairAnim;					// ͷ������


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
	virtual void SetFinish(bool bIsFinsh);// ��׼��������
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
//-----------------------------------------------------------------------������������������������������������������
	void SetCharaterAnimation(UINT seqID);					//���ý�ɫ�Ķ������������������ȼ�����
	void SetCharaterAnimation(NiFixedString strAnimName);

	void SetKartAnimation(UINT seqID);						//���ó��Ķ������������������ȼ�����
	void SetKartAnimation(NiFixedString strAnimName);

	UINT GetCharacterAnim();			//���ؽ�ɫ����id
	UINT GetKartAnim();					//���س�����id

	//push�����¼�����Ӧ�����ȼ����߼���CAnimEventMgr����
	void PushPlayerAnimEvent(CAnimEventMgr::ENMANIMEVENTTYPE unEvent);	
	CAnimEventMgr* GetAnimEventMgr();		//���CAnimEventMgr

	void ChangeAnimFaceImage(int ID);		//���ݶ������������鶯��

	void LoadCharacterDesc(unsigned int unID);		//����������Ϣ
	void LoadKartDesc(unsigned int unID);			//���복��Ϣ
	bool SetActiveCharacter(unsigned int unID);		//���û�Ծ����
	bool SetActiveKart(unsigned int unID);			//���û�Ծ����
	bool ChangeItem(unsigned int unID);				//����Item
	//	������:	wincoqin
	//  ʱ��:	2007/05/10
	//  Ŀ��:	���ӻ�ͷ���Ľӿ�  	
	bool ChangeHair(unsigned int unID);				// ��ͷ��
	bool BindHairToHead();							// ��ͷ���󶨵�ͷ��
	bool LoadHairDesc(unsigned int unID);			// ����ͷ����Ϣ

//---------------------------------------------------------------------------------------------------------------
	//�̳���CATWearer
	//���������avatarӵ�ж���
//	DWORD AnimTranform(DWORD dwID);

//-------------------------------------------------------------------------------
	//-----------------------------------------------------------------------

	//IGPTarget
	virtual DWORD		GetID();
	virtual DWORD		GetIndex();
	virtual DWORD		GetTeam();
	virtual NiPoint3	GetForward();
	//virtual NiPoint3	GetPos();  //�Ѵ���
	//virtual void		SetPos(const NiPoint3&);//�Ѵ���
	//virtual NiTransform GetTransform();//�Ѵ���
	//virtual NiNode*		GetNode();//�Ѵ���
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
	PLAYERDESC m_stCurPlayerDesc;				//��ǰplay����Ϣ

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
